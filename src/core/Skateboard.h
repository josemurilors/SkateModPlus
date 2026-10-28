#pragma once

#include <plugin.h>
#include <CPlayerPed.h>
#include <CVehicle.h>
#include <CVector.h>

class Skateboard
{
public:
    static void Init();
    static void Process();

    static bool CanMount();
    static void Mount();
    static void Dismount();

private:
    static bool       m_bOnSkateboard;
    static CVehicle*  m_pSkateVehicle;
    static unsigned int m_nMountCooldown;
    static bool       m_bPrevActionKeyState;
    static bool       m_bPickupsCreated;

    static void CheckInput();
    static void UpdateAudio();
    static void CreatePickups();
    static bool IsSurfaceClear(CPlayerPed* pPlayer, CVector& outSpawnPos, float& outHeading);
};
