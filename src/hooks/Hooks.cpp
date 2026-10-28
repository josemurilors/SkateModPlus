#include "Hooks.h"
#include <injector/injector.hpp>
#include <cstdint>

void Hooks::Install()
{
    // Bike Arm Fix:
    // No GTA SA 1.0 US, desativa o IK de braços para bicicletas,
    // permitindo que o modelo humano reproduza a pose de skate livremente
    // sem puxar as mãos para um guidão invisível.
    // Endereços originais no executável: 0x6B8202 e 0x6B8559
    // Instrução original: 0F 84 ... (jz); só patcha se os bytes forem exatamente esses
    // Patch: 90 E9 ... (nop; jmp)
    if (*(uint16_t*)0x6B8202 == 0x840F)
    {
        injector::WriteMemory<uint16_t>(0x6B8202, 0xE990, true);
    }
    if (*(uint16_t*)0x6B8559 == 0x840F)
    {
        injector::WriteMemory<uint16_t>(0x6B8559, 0xE990, true);
    }
}
