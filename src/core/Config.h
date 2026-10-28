#pragma once

#include <string>

class Config
{
public:
    static void Load(const std::string& path = "cleo\\skateboard.ini");

    // Compatibilidade com o skate mod original
    static int WeaponType;      // Padrão: 15
    static int WeaponModel;     // Padrão: 326
    static int VehicleID;       // Padrão: 74
    static float Volume;        // Padrão: 0.3
    static bool ChangePickups;  // Padrão: true

    static std::string GetIniPath();
};
