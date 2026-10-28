# Integration notes (Urbanize + Skate Mod)

## Where Urbanize interacts with Skate Mod
- `Urbanize.dat` line ~1029 defines PED flag `0x40000000 SKATER`.
- `Urbanize.dat` PED section lists behavior scripts: `BEGA1`, `LEAN1`, `UMBRL` with conditions/masks/chances (see PED entries). These drive NPC behaviors in zones.
- `CLEO/Urbanize/LS/GLNPS.cs` contains string `sktbd` and reads `CLEO\skateboard.ini` (WeaponType/WeaponModel). Also uses animation names from Urbanize anim set (`URB_SIT_LAUGH_1`, etc.). This suggests Urbanize has code/areas that interact with skateboarding - possibly spawning skate-related objects/NPCs or integrating with the skate mod.
- The Skate Mod is installed separately (cleo/skateboard.cs + models/ini + audio + fxt). Urbanize appears to be aware of it (GLNPS references it). 
- "onde o skate mod já opera originalmente" - Urbanize may have original integration points (spawning skate pickups/NPCs doing skate-related things). Need to understand those integration points to port/enhance properly.

## Skate Mod technical details
- Implemented as vehicle (ID 74 by default, model sktbd.dff/txd, handling SKTBD, anim group choppa/bmx). Controlled by CLEO script.
- Weapon config in ini (WeaponType/WeaponModel) - maybe used to give skateboard as weapon or for detection.
- Audio `cleo_audio/skate.mp3`, text `SKTBD` in fxt.
- Pickup positions changeable via `ChangePickups` setting.
- Installed with modloader lines adding handling/vehicles.ide entries.

## Goal for next steps
- Reverse engineer existing Skate Mod CLEO script logic (state machine, controls, animations, collision/physics hooks) - but script is binary; need to extract more strings/structure.
- Understand Urbanize integration points (GLNPS.cs, how it spawns/uses skaters). Also look up other CS/LS files for "skate"/"sktbd".
- Plan port to ASI (maybe rewrite/port logic to ASI plugin) while keeping compatibility or improving.
- Add Tony Hawk's American Wasteland animations to the mod and integrate with Urbanize.
- Document everything in mds and make commits.

## Next actions
1. Search more for skater/NPC skate behavior in Urbanize (all .cs files)
2. Look at GLNPS.cs more closely (strings around sktbd)
3. Understand Skate Mod script better - look for common CLEO patterns, function names, opcodes usage from strings
4. Check if there are any docs/specs in the mod folders
5. Create git repo in E:/projetos/SkateMod+/ and start committing docs as we go
