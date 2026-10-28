# Urbanize (SA) - Overview

## Paths
- Source drop: `C:\Users\josen\OneDrive\old\Área de Trabalho\SA_-_Urbanize_12-25_Final_1\`
  - CLEO: `CLEO/Urbanize/` (CLEO scripts/configs/props/audio/models)
  - Modloader: `modloader/Urbanize/` (`Urbanize (Junior_Djjr).asi`, `gta3img/djjr_urbanize.ifp`, `Map/`)
- Author: Junior_Djjr (MixMods.com.br)
- Version mentioned: 12-25 (02-26 update)

## Requirements (from readme)
- Extract `CLEO/` and `modloader/` to GTA SA folder
- Needs CLEO+ (in Essentials Pack)
- Needs Open Limit Adjuster v1.5.9+ (or F92LA configured)
- Optional map objects; can disable parts via Loader.txt
- Optional extras in `(extras)/`
- Settings in `(settings)/` (Low/Med/High etc for people count)

## Runtime structure
- CLEO root script: `CLEO/Urbanize/Urbanize - Root.cs` (binary/compiled CLEO form; header bytes `a4 03 09 55 52 42 41 15 41 49` starting as shown; contains strings like `URBA_AI`, `Urbanize (Junior_Djjr).asi`, `CLEO\Urbanize\Urbanize.ini`, gang/attractor/smoking/animation references)
- Config: `Urbanize.ini` (Density, PedPopulationMult, VehPopulationMult, Rain.NpcsRainBehavior, etc.)
- Data: `Urbanize.dat` - defines where scripts run (by area: LS/SF/LV/CS/DE/SA/IN, radius/positions), object spawns (OBJ), ped behaviors (PED), node behaviors (NODE), interiors (IN), etc. Very large spawn list (cars, peds, animations, props)
- ASI: `modloader/Urbanize/Urbanize (Junior_Djjr).asi` (core)
- IFP: `modloader/Urbanize/gta3img/djjr_urbanize.ifp` (animations)
- Props/models/audio under `CLEO/Urbanize/_Props/`, `_Models/`, `_Audio/`

## Areas/scripts referenced
CS scripts include many (APBAI, APHSP, APPDP, APTRP, BAR69, BCNHL, BOBBK, CSTLS, DLBAR, DLPDP, EBFRM, EBTRP, FLBEC, FLEI1, FLNFN, FLNGS, FLNR1, FLNTR, FLWHS, HLTPC, HLTPC, MCHB1, MCHTP, MGHSP, MGOGS, PACEM, PALO1, PALOF, PANO1, PANO2, RCRST, TFARM, TFARM_Mission.cm, WHTGS). NODE has CHAT1, SWEEP. PED has BEGA1, LEAN1, UMBRL.

## How it "spawns skate" / NPC skaters?
From `Urbanize.dat` PED section:
```
PED
BEGA1 0x30000 0x8 80
LEAN1 0x821FB6D8 0x8 30
UMBRL 0xBE0CFFF0 0x80 10000
End
```
Conditions are bitmasks (see comments in dat). Also `SKATER` is flag `0x40000000` (line ~1030 in dat). Urbanize uses area-based script spawning (`Urbanize.dat` LS/SF/LV/CS/DE/SA sections call scripts by name at coords/radii). NPC behavior scripts live in `CLEO/Urbanize/PED/`, `NODE/`, `CS/` etc.

Also note `Urbanize.dat` has many `VUNLD` (vehicle unload/spawn related?) entries and large spawn sets; skaters may be handled by PED behaviors and/or specific scripts. Need to trace BEGA1/LEAN1/UMBRL and also look for SKATE/board references inside Urbanize scripts/data.
