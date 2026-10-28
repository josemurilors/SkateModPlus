#include "Config.h"

#include <Windows.h>
#include <cstdlib>

int Config::WeaponType = 15;
int Config::WeaponModel = 326;
int Config::VehicleID = 74;
float Config::Volume = 0.3f;
bool Config::ChangePickups = true;

std::string Config::GetIniPath()
{
    return "cleo\\skateboard.ini";
}

// O leitor de ini do Windows ignora o texto depois do valor, como o do CLEO
// (o skateboard.ini traz comentários "# ..." na mesma linha).
void Config::Load(const std::string& path)
{
    // Caminho relativo sem pasta completa seria procurado na pasta do Windows
    char full[MAX_PATH];
    if (!GetFullPathNameA(path.c_str(), MAX_PATH, full, nullptr))
        return;

    if (GetFileAttributesA(full) == INVALID_FILE_ATTRIBUTES)
        return;

    WeaponType    = GetPrivateProfileIntA("Settings", "WeaponType", WeaponType, full);
    WeaponModel   = GetPrivateProfileIntA("Settings", "WeaponModel", WeaponModel, full);
    VehicleID     = GetPrivateProfileIntA("Settings", "VehicleID", VehicleID, full);
    ChangePickups = GetPrivateProfileIntA("Settings", "ChangePickups", ChangePickups, full) != 0;

    char buf[32];
    if (GetPrivateProfileStringA("Settings", "Volume", "", buf, sizeof(buf), full) > 0)
        Volume = static_cast<float>(atof(buf));
}
