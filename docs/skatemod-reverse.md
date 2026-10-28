# Skate Mod (existing) - Reverse engineering notes

## Core mechanics (from strings/structure)
- Uses CLEO script `cleo/skateboard.cs`. Config from `cleo/skateboard.ini`: WeaponType (default 15), WeaponModel (326), VehicleID (74), Volume (0.3), ChangePickups (1).
- When player has weapon 14@ (set from WeaponType) and not driving/in air/controllable conditions met, pressing key (00E1 key 4) spawns skateboard vehicle.
- Vehicle model/ID from ini (13@ = VehicleID). Model loaded/destroyed. Car created at offset in front of player.
- Player put in vehicle and animation "CHOPPA_sprint" from IFP "CHOPPA" applied (framedelta 4.0, loopA 0, locks etc, time -1).
- Weapons hidden briefly during mount/unmount sequence.
- Audio: loads `CLEO\CLEO_AUDIO\SKATE.MP3` as audiostream, volume from ini (15@), played on mount/start (Audiostream.Play).
- Pickups created at fixed locations if ChangePickups != 0 (26@ weapon/model based). When ChangePickups is set, it creates pickups at many coordinates across SA.
- Skateboard is treated as vehicle (model sktbd.dff/txd, handling SKTBD per Linhas.txt). Animations use choppa/bmx style.

## Files
- `cleo/skateboard.cs` (CLEO script, binary/compiled)
- `cleo/skateboard.ini`
- `cleo/cleo_text/skateboard.fxt` (SKTBD)
- `cleo/cleo_audio/skate.mp3`
- Models: `sktbd.dff/txd`, `gun_cane.*`, `choppa.ifp`, `bikearmfix.asi`
- Modloader: `Linhas.txt` adds to Handling.cfg and Vehicles.ide

## Observations
- This is a vehicle-based skateboard implementation (common approach in SA). Integration with Urbanize happens because GLNPS.cs in Urbanize references `sktbd` and reads skate mod ini - suggests Urbanize may spawn skaters/pickups or have specific behavior tied to skate mod presence.
- To "port to ASI" means moving from CLEO script to an ASI plugin (likely C++/CLEO Redux style or SA:MP/Plugin SDK) for better integration/control. Also want to add Tony Hawk's American Wasteland animations.

## Next steps
- Look up GLNPS.cs more - understand what Urbanize does with sktbd
- Look for other Urbanize files that might reference skate/NPC skaters
- Understand how Urbanize spawns NPCs (PED scripts) and if there is skate-specific NPC behavior
- Plan ASI port structure - need to understand SA plugin APIs used, but we're doing reverse engineering/documentation first as requested
- Document everything and commit
