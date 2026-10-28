# Technical Summary - SkateMod+ Project

## Project Goal
Improve existing Skate Mod, port to ASI, add Tony Hawk's American Wasteland animations, integrate with Urbanize (where skate mod already operates).

## What we've documented

### Urbanize (SA_-_Urbanize_12-25_Final_1)
- CLEO-based with ASI component (`Urbanize (Junior_Djjr).asi`)
- Data-driven spawning via `Urbanize.dat`
- Has animation set in `djjr_urbanize.ifp`
- Integration with skate mod found in `CLEO/Urbanize/LS/GLNPS.cs` - reads skate mod ini, references sktbd model
- Defines SKATER ped flag (0x40000000)
- Has ped behavior scripts (BEGA1, LEAN1, UMBRL) and others

### Skate Mod (existing)
- Vehicle-based skateboard implementation (model sktbd as vehicle ID 74)
- CLEO script `cleo/skateboard.cs` - mounts skateboard as vehicle when player has weapon, uses CHOPPA animations
- Config: WeaponType, WeaponModel, VehicleID, Volume, ChangePickups
- Audio via audiostream (skate.mp3)
- Pickups placed around map if enabled
- Uses `choppa.ifp` animations currently
- Integration with Urbanize confirmed via GLNPS

## Viability
Project is viable. Main tasks:
1. Reverse engineer full Skate Mod behavior (in progress - have good picture)
2. Port to ASI plugin
3. Add THAW animations support
4. Ensure proper Urbanize integration
5. Document everything (commits made)

## Architecture considerations for ASI port
- Need to hook into game events (keypress, player state)
- Handle vehicle creation/attachment
- Manage animations
- Audio playback
- Config reading
- Maintain compatibility with Urbanize expectations (GLNPS reads ini, may expect certain behavior)

## Next steps
- Continue deeper reverse engineering of Skate Mod script logic
- Look up THAW animation sources if they exist somewhere
- Start planning ASI implementation structure
- Maybe look up existing ASI skateboard mods for patterns?
- Create initial ASI project structure in the repo
