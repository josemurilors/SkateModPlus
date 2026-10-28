# Skate Mod (existing) - Overview

## Paths
- Source drop: `C:\Users\josen\OneDrive\old\Área de Trabalho\Skate Mod\`
  - `cleo/skateboard.cs` (binary/compiled CLEO script)
  - `cleo/skateboard.ini` (config)
  - `cleo/cleo_text/skateboard.fxt` (`SKTBD Skateboard`)
  - `cleo/cleo_audio/skate.mp3`
  - `cleo/` other files
  - Models/textures at root: `sktbd.dff`, `sktbd.txd`, `gun_cane.dff`, `gun_cane.txd`, `choppa.ifp`, `bikearmfix.asi`
  - `Linhas.txt` - modloader integration lines (Handling.cfg, Vehicles.ide entries for SKTBD)

## Config (`cleo/skateboard.ini`)
```ini
[Settings]
WeaponType    = 15   # Weapon slot/type
WeaponModel   = 326  # Weapon model
VehicleID     = 74   # Vehicle model to use as skateboard
Volume        = 0.3
ChangePickups = 1    # change skate pickups position
```

## Strings from script (extracted)
From `skateboard.cs`: references to `cleo\skateboard.ini`, `RIXA`, `FV$E`, `CHOPPA_sprint`, `CHOPPA`, `CLEO\CLEO_AUDIO\SKATE.MP3`, lots of game vars/flags (interiors, actors, missions), and standard SA script tokens. Also references to audio and animation names like `CHOPPA*`.

## Models/handling
`Linhas.txt` adds:
- Handling.cfg: `SKTBD 500.0 ... R P ...` (bike-like/vehicle handling)
- Vehicles.ide: `74, sktbd, sktbd, bmx, CHOPPERB, SKTBD, choppa, bicycle, 7,0,0,23,...` (vehicle ID 74 uses model sktbd, anim group choppa/bmx style)
- Also mentions 509/bike line present

So skateboard is implemented as a rideable vehicle (model `sktbd`, handling SKTBD, anims from `choppa.ifp`/bmx groups) controlled via CLEO script, weapon/slot config, audio. Pickups can be moved.

## Goal
"Melhorar o Skate Mod já existente e portar para ASI, e colocar as animações do Tony Hawks american wasteland no mod e integrar juntamente ao urbanize (onde o skate mod já opera originalmente)."
"Precisamos entender o Skate Mod e o Urbanize (onde ele spawna o skate para você e o npcs de skate) e fazer uma engenharia reversa e verifique a viabilidade do projeto. Documente tudo com mds e commits."
"Vamos fazer primeiro os mods já existentes, depois partimos para o Tony Hawk."
