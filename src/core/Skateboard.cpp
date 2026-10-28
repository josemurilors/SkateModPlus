#include "Skateboard.h"
#include "Config.h"
#include "Audio.h"

#include <plugin.h>
#include <CPlayerPed.h>
#include <CVehicle.h>
#include <CPools.h>
#include <CPickups.h>
#include <CTimer.h>
#include <CPad.h>
#include <extensions/ScriptCommands.h>
#include <Windows.h>
#include <CModelInfo.h>
#include <cmath>
#include <cstdio>
#include <string>

using namespace plugin;

bool         Skateboard::m_bOnSkateboard       = false;
CVehicle*    Skateboard::m_pSkateVehicle       = nullptr;
unsigned int Skateboard::m_nMountCooldown      = 0;
bool         Skateboard::m_bPrevActionKeyState = false;
bool         Skateboard::m_bPickupsCreated     = false;

// Handle validado pela pool: o ponteiro cru fica dangling se o jogo remover o veículo.
static int s_hSkateVehicle = -1;

// Diagnóstico: motivo pelo qual o Tab não montou (SkateModPlus.log na pasta do jogo)
static void Log(const char* msg)
{
    if (FILE* f = fopen("SkateModPlus.log", "a")) { fputs(msg, f); fputc(10, f); fclose(f); }
}

// ID real do modelo "sktbd" pelo nome (o modloader pode atribuir ID livre); cai no skateboard.ini se não achar
static int ResolveVehicleId()
{
    int id = -1;
    if (CModelInfo::GetModelInfo("sktbd", &id) && id >= 0)
        return id;
    return Config::VehicleID;
}

static CVehicle* ResolveSkateVehicle()
{
    return s_hSkateVehicle < 0 ? nullptr : CPools::GetVehicle(s_hSkateVehicle);
}

void Skateboard::Init()
{
    Config::Load(Config::GetIniPath());
    Audio::Init();

    m_bOnSkateboard       = false;
    m_pSkateVehicle       = nullptr;
    s_hSkateVehicle       = -1;
    m_nMountCooldown      = 0;
    m_bPrevActionKeyState = false;
    m_bPickupsCreated     = false;

}

bool Skateboard::IsSurfaceClear(CPlayerPed* pPlayer, CVector& outSpawnPos, float& outHeading)
{
    if (!pPlayer)
        return false;

    Command<Commands::GET_CHAR_HEADING>(pPlayer, &outHeading);

    // Ponto 0.3m à frente do jogador
    CVector p1;
    Command<Commands::GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS>(pPlayer, 0.0f, 0.3f, 0.0f, &p1.x, &p1.y, &p1.z);
    float groundZ1 = 0.0f;
    Command<Commands::GET_GROUND_Z_FOR_3D_COORD>(p1.x, p1.y, p1.z, &groundZ1);
    if (groundZ1 > 0.0f)
        p1.z = groundZ1 + 0.3f;

    // Ponto 3.0m à frente do jogador
    CVector p2;
    Command<Commands::GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS>(pPlayer, 0.0f, 3.0f, 0.0f, &p2.x, &p2.y, &p2.z);
    float groundZ2 = 0.0f;
    Command<Commands::GET_GROUND_Z_FOR_3D_COORD>(p2.x, p2.y, p2.z, &groundZ2);
    if (groundZ2 > 0.0f)
        p2.z = groundZ2 + 0.3f;

    // Linha de visão sem obstáculos sólidos
    bool clear = Command<Commands::IS_LINE_OF_SIGHT_CLEAR>(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 1, 1, 1, 1, 0);
    if (!clear)
        return false;

    outSpawnPos = p1;
    return true;
}

bool Skateboard::CanMount()
{
    if (m_bOnSkateboard)
        { Log("CanMount: ja montado"); return false; }

    if (CTimer::m_snTimeInMilliseconds < m_nMountCooldown)
        { Log("CanMount: cooldown"); return false; }

    CPlayerPed* pPlayer = FindPlayerPed();
    if (!pPlayer || !pPlayer->IsAlive())
        { Log("CanMount: jogador morto/nulo"); return false; }

    if (pPlayer->m_pVehicle)
        { Log("CanMount: jogador em veiculo"); return false; }

    if (pPlayer->bSubmergedInWater)
        { Log("CanMount: na agua"); return false; }

    // Verifica se a arma em mãos é a configurada (padrão: 15 / cane)
    CWeapon* pWeapon = pPlayer->GetWeapon();
    if (!pWeapon || pWeapon->m_eWeaponType != static_cast<eWeaponType>(Config::WeaponType))
        { Log("CanMount: arma equipada diferente de WeaponType"); return false; }

    // Verifica se o jogador está no ar
    if (Command<Commands::IS_CHAR_IN_AIR>(pPlayer))
        { Log("CanMount: no ar"); return false; }

    // Verifica controle ativo
    if (!Command<Commands::IS_PLAYER_CONTROL_ON>(0))
        { Log("CanMount: controle desligado"); return false; }

    return true;
}

void Skateboard::Mount()
{
    CPlayerPed* pPlayer = FindPlayerPed();
    if (!pPlayer)
        return;

    CVector spawnPos;
    float heading = 0.0f;
    if (!IsSurfaceClear(pPlayer, spawnPos, heading))
    {
        Log("Mount: superficie nao livre (line of sight)");
        return;
    }

    // ID ocupado por um ped (ex.: Restored Unused Peds usa o 74) faz o CREATE_CAR crashar no GetWheelPosn
    const int vehicleId = ResolveVehicleId();
    CBaseModelInfo* pInfo = (vehicleId >= 0 && vehicleId < 20000) ? CModelInfo::GetModelInfo(vehicleId) : nullptr;
    if (!pInfo || pInfo->GetModelType() == MODEL_INFO_PED)
    {
        Log("Mount: modelo sktbd nao existe ou o ID e um ped. Confira o Vehicles.ide e o VehicleID do skateboard.ini");
        return;
    }

    Log(("Mount: VehicleID resolvido = " + std::to_string(vehicleId)).c_str());

    // Carrega modelos e animações
    Command<Commands::REQUEST_MODEL>(vehicleId);
    Command<Commands::REQUEST_ANIMATION>("CHOPPA");
    Command<Commands::LOAD_ALL_MODELS_NOW>();

    // Cria o veículo do skate (modelo sktbd)
    CVehicle* pVehicle = nullptr;
    Command<Commands::CREATE_CAR>(vehicleId, spawnPos.x, spawnPos.y, spawnPos.z, &pVehicle);
    Command<Commands::MARK_MODEL_AS_NO_LONGER_NEEDED>(vehicleId);
    if (!pVehicle)
    {
        Log("Mount: CREATE_CAR falhou");
        return;
    }

    m_pSkateVehicle = pVehicle;
    s_hSkateVehicle = CPools::GetVehicleRef(pVehicle);
    Command<Commands::SET_CAR_HEADING>(pVehicle, heading);

    // Vincula ao interior ativo
    int interior = 0;
    Command<Commands::GET_AREA_VISIBLE>(&interior);
    Command<Commands::SET_VEHICLE_AREA_VISIBLE>(pVehicle, interior);

    // Ajusta altura no solo
    float groundZ = 0.0f;
    Command<Commands::GET_GROUND_Z_FOR_3D_COORD>(spawnPos.x, spawnPos.y, spawnPos.z, &groundZ);
    if (groundZ > 0.0f)
    {
        spawnPos.z = groundZ + 0.2f;
        Command<Commands::SET_CHAR_COORDINATES_DONT_WARP_GANG>(pPlayer, spawnPos.x, spawnPos.y, spawnPos.z);
    }

    // Coloca o jogador no veículo
    Command<Commands::WARP_CHAR_INTO_CAR>(pPlayer, pVehicle);

    // Aplica animação CHOPPA_sprint
    Command<Commands::TASK_PLAY_ANIM>(pPlayer, "CHOPPA_sprint", "CHOPPA", 4.0f, 0, 0, 0, 0, -1);

    // Impulso inicial para frente
    Command<Commands::SET_CAR_FORWARD_SPEED>(pVehicle, 7.0f);

    // Truque para desarmar as mãos do ped
    Command<Commands::HIDE_CHAR_WEAPON_FOR_SCRIPTED_CUTSCENE>(pPlayer, 1);
    Command<Commands::SET_CURRENT_CHAR_WEAPON>(pPlayer, 0);
    Command<Commands::HIDE_CHAR_WEAPON_FOR_SCRIPTED_CUTSCENE>(pPlayer, 0);
    pPlayer->RemoveWeaponModel(Config::WeaponModel);

    // Inicia som de rolagem
    Audio::PlaySkateSound();

    m_bOnSkateboard = true;
    m_nMountCooldown = CTimer::m_snTimeInMilliseconds + 600;
}

void Skateboard::Dismount()
{
    Audio::StopSkateSound();

    CPlayerPed* pPlayer = FindPlayerPed();
    CVehicle* pVehicle = ResolveSkateVehicle();

    if (pVehicle)
    {
        // Apagar o carro com o jogador dentro deixa pPlayer->m_pVehicle dangling (crash)
        if (pPlayer && pPlayer->m_pVehicle == pVehicle)
        {
            CVector pos = pVehicle->GetPosition();
            Command<Commands::WARP_CHAR_FROM_CAR_TO_COORD>(pPlayer, pos.x, pos.y, pos.z);
        }
        Command<Commands::DELETE_CAR>(pVehicle);
    }
    m_pSkateVehicle = nullptr;
    s_hSkateVehicle = -1;

    if (pPlayer && pPlayer->IsAlive())
    {
        // Re-equipa o skate nas mãos
        Command<Commands::REQUEST_MODEL>(Config::WeaponModel);
        Command<Commands::LOAD_ALL_MODELS_NOW>();
        Command<Commands::SET_CURRENT_CHAR_WEAPON>(pPlayer, Config::WeaponType);
        Command<Commands::MARK_MODEL_AS_NO_LONGER_NEEDED>(Config::WeaponModel);
    }

    m_bOnSkateboard = false;
    m_nMountCooldown = CTimer::m_snTimeInMilliseconds + 600;
}

void Skateboard::CheckInput()
{
    CPad* pPad = CPad::GetPad(0);
    if (!pPad)
        return;

    // Alt+Tab / janela sem foco não pode disparar o skate
    bool tabDown = GetActiveWindow() != nullptr && (GetAsyncKeyState(VK_TAB) & 0x8000) != 0 && (GetAsyncKeyState(VK_MENU) & 0x8000) == 0;
    // Tecla 4 do script original (L1 no controle): só monta, como lá
    bool actionPressed = pPad->NewState.LeftShoulder1 != 0 || tabDown;

    if (actionPressed && !m_bPrevActionKeyState)
    {
        Log("Tab detectado");
        m_bPrevActionKeyState = true;
        if (!m_bOnSkateboard)
        {
            if (CanMount())
                Mount();
        }
        // Guardar o skate (extra do port): só TAB no teclado. No controle o L1 pertence à bike (pulo/tricks).
        else if (tabDown)
        {
            Dismount();
        }
    }
    else if (!actionPressed)
    {
        m_bPrevActionKeyState = false;
    }
}

void Skateboard::UpdateAudio()
{
    if (!m_bOnSkateboard || !m_pSkateVehicle)
    {
        Audio::SetVolume(0.0f);
        return;
    }

    bool inAir = Command<Commands::IS_CAR_IN_AIR_PROPER>(m_pSkateVehicle);
    bool stopped = Command<Commands::IS_CAR_STOPPED>(m_pSkateVehicle);

    if (inAir || stopped)
    {
        Audio::SetVolume(0.0f);
        return;
    }

    float speed = 0.0f;
    Command<Commands::GET_CAR_SPEED>(m_pSkateVehicle, &speed);

    CVector carPos;
    Command<Commands::GET_CAR_COORDINATES>(m_pSkateVehicle, &carPos.x, &carPos.y, &carPos.z);

    CVector camPos;
    Command<Commands::GET_ACTIVE_CAMERA_COORDINATES>(&camPos.x, &camPos.y, &camPos.z);

    float dx = carPos.x - camPos.x;
    float dy = carPos.y - camPos.y;
    float dz = carPos.z - camPos.z;
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz) / 2.0f;
    if (dist < 1.0f) dist = 1.0f;
    float distVol = 1.0f / dist;

    float speedVol = speed / 20.0f;
    if (speedVol > 1.0f) speedVol = 1.0f;
    if (speedVol < 0.0f) speedVol = 0.0f;

    // Volume master SFX (float em 0xB5FCCC = 11926732 no script original)
    float masterVol = *(float*)0xB5FCCC;

    float finalVol = distVol * speedVol * masterVol * Config::Volume;
    if (finalVol > 1.0f) finalVol = 1.0f;
    if (finalVol < 0.0f) finalVol = 0.0f;

    Audio::SetVolume(finalVol);
}

void Skateboard::CreatePickups()
{
    if (!Config::ChangePickups || m_bPickupsCreated)
        return;

    static const CVector pickupCoords[17] = {
        { 1868.6021f, -1363.297f, 13.5179f },
        { 1307.5206f, -801.7294f, 84.1406f },
        { 1660.5605f, -2106.447f, 13.5686f },
        { 2164.0317f, -1988.3718f, 13.9981f },
        { 2606.0625f, -1080.0363f, 69.6401f },
        { -2334.5938f, -1676.3059f, 483.4468f },
        { -2540.8188f, -623.9956f, 132.8262f },
        { -2569.1216f, 983.2914f, 78.3996f },
        { -2116.1057f, 926.8377f, 86.1553f },
        { 2629.3921f, 2418.3245f, 10.857f },
        { 1323.5667f, 206.4454f, 19.5342f },
        { 2536.2729f, -1665.4052f, 15.1234f },
        { 947.5957f, -721.8701f, 122.1615f },
        { 400.4942f, -2062.9158f, 10.7062f },
        { 1207.9354f, -2070.637f, 69.0643f },
        { 1359.1188f, -1718.8069f, 8.7049f },
        { -1267.2449f, 966.0405f, 133.0514f }
    };

    Command<Commands::REQUEST_MODEL>(Config::WeaponModel);
    Command<Commands::LOAD_ALL_MODELS_NOW>();

    for (int i = 0; i < 17; ++i)
    {
        // O save já guarda os pickups da sessão anterior: não cria de novo no mesmo ponto
        bool exists = false;
        for (unsigned int j = 0; j < MAX_NUM_PICKUPS && !exists; ++j)
        {
            CPickup& p = CPickups::aPickUps[j];
            exists = p.m_nPickupType != 0 && p.m_nModelIndex == Config::WeaponModel
                && (p.GetPosn() - pickupCoords[i]).Magnitude() < 1.0f;
        }
        if (exists)
            continue;

        int pickupHandle = 0;
        // O script original usa WeaponType como tipo do pickup
        Command<Commands::CREATE_PICKUP>(Config::WeaponModel, Config::WeaponType, pickupCoords[i].x, pickupCoords[i].y, pickupCoords[i].z, &pickupHandle);
    }

    Command<Commands::MARK_MODEL_AS_NO_LONGER_NEEDED>(Config::WeaponModel);
    m_bPickupsCreated = true;
}

void Skateboard::Process()
{
    CreatePickups();
    CheckInput();

    if (m_bOnSkateboard)
    {
        CPlayerPed* pPlayer = FindPlayerPed();

        m_pSkateVehicle = ResolveSkateVehicle();

        // Se o veículo foi destruído ou o jogador saiu do skate
        if (!pPlayer || !pPlayer->IsAlive() || !m_pSkateVehicle || pPlayer->m_pVehicle != m_pSkateVehicle)
        {
            Dismount();
            return;
        }

        // Mantém desarmado durante a condução (a cane/skate de mão não pode ficar visível)
        if (pPlayer->m_nSelectedWepSlot != 0)
        {
            Command<Commands::SET_CURRENT_CHAR_WEAPON>(pPlayer, 0);
        }
        if (pPlayer->m_pWeaponObject)
            pPlayer->RemoveWeaponModel(Config::WeaponModel);

        UpdateAudio();
    }
}
