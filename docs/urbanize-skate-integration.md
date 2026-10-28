# Urbanize + Skate Mod integration details

## GLNPS.cs
Found in `CLEO/Urbanize/LS/GLNPS.cs` - contains string `sktbd` and reads `CLEO\skateboard.ini` (WeaponType/WeaponModel). Also uses Urbanize animation names (URB_SIT_LAUGH_1, URB_LOOK_1, URB_SHAKEHEAD_1, URB_LEAN_B, etc.). This is a location script (Glen Park/Santa Maria Beach area?) that integrates with skate mod - possibly spawns skaters/NPCs or creates skate-related elements when skate mod is present.

## Urbanize PED behaviors
PED section in Urbanize.dat:
- BEGA1 (chances 80) with conditions 0x30000 0x8
- LEAN1 (30) with 0x821FB6D8 0x8  
- UMBRL (10000) with 0xBE0CFFF0 0x80

UMBRL relates to umbrellas in rain (reads Urbanize.ini Rain.NpcsRainBehavior). BEGA1/LEAN1 are loitering/leaning behaviors. No obvious "SKATE" behavior script name in PED list, but SKATER flag is defined (0x40000000). Maybe skaters are spawned via other mechanisms or the skate mod itself spawns them; Urbanize might affect zones/population.

Also GLNPS references skate mod directly - integration point is there. Need to understand more of what GLNPS does.

## Integration points to investigate
1. Does Urbanize spawn skate pickups or NPC skaters? Check if any script creates sktbd vehicles/peds with skate behavior
2. How does GLNPS.cs use sktbd? Does it look for skate mod presence and enable features?
3. Are there other integration points elsewhere?
4. For porting to ASI + adding THAW animations, we need to understand the full picture of both mods working together

## Next steps
- Extract more from GLNPS.cs strings around sktbd usage
- Look up other files for skater spawning
- Create comprehensive map of both mods' APIs/integration
- Start planning ASI port architecture
