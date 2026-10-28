#include <plugin.h>
#include "core/Skateboard.h"
#include "hooks/Hooks.h"

using namespace plugin;

// SkateMod+
// Conceito e mod original: Linevariy, Junior_Djjr, M. Osborn
// Port ASI e arquitetura: SkateMod+ Team

class SkateModPlus
{
public:
    SkateModPlus()
    {
        Events::initGameEvent += []()
        {
            Hooks::Install();
            Skateboard::Init();
        };

        Events::processScriptsEvent += []()
        {
            Skateboard::Process();
        };
    }
} skateModPlus;
