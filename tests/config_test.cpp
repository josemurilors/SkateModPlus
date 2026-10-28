// Teste do leitor de ini: usa as linhas reais do skateboard.ini (com comentários "# ..." na mesma linha).
#include "core/Config.h"

#include <Windows.h>
#include <cassert>
#include <cstdio>

int main()
{
    const char* path = "config_test.ini";
    FILE* f = fopen(path, "w");
    assert(f);
    fputs("[Settings]\n"
          "WeaponType    = 16  # https://wiki.multitheftauto.com/wiki/Weapons\n"
          "WeaponModel   = 327  # comentario\n"
          "VehicleID     = 12250  # comentario\n"
          "Volume        = 0.5  # (0.0 ~ 1.0)\n"
          "ChangePickups = 0    # EN: set 0 if you are using total conversions mods.\n", f);
    fclose(f);

    Config::Load(path);

    assert(Config::WeaponType == 16);
    assert(Config::WeaponModel == 327);
    assert(Config::VehicleID == 12250);
    assert(Config::Volume > 0.49f && Config::Volume < 0.51f);
    assert(Config::ChangePickups == false);

    // Arquivo inexistente mantém os valores atuais
    Config::Load("nao_existe.ini");
    assert(Config::VehicleID == 12250);

    DeleteFileA(path);
    puts("config_test: ok");
    return 0;
}
