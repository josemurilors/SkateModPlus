# GLNPS.cs analysis (Urbanize + Skate Mod)

GLNPS is a CLEO script in Urbanize (LS area). From strings extracted:
- References `sktbd` (skateboard model/vehicle)
- Reads `CLEO\skateboard.ini` for `WeaponType` and `WeaponModel`
- Uses Urbanize animations: URB_SIT_LAUGH_1, URB_LOOK_1, URB_SHAKEHEAD_1, URB_LEAN_B, URB_LEAN_B_F, etc.
- Uses Urbanize animation groups with DJJR_URBANIZE
- References `VehFuncs.asi`, `Ext_SetCarSeed`

This suggests GLNPS integrates with the skate mod - possibly checks if skate mod is installed (reads its ini), spawns skate-related NPCs/objects, or creates scenarios involving skateboards. The integration is bidirectional in effect - Urbanize knows about Skate Mod and Skate Mod operates in Urbanize areas.

## Implications for the project
To "port Skate Mod to ASI and integrate with Urbanize (where skate mod already operates originally)", we need to:
1. Understand the full integration contract between them (how GLNPS uses skate mod, what it expects)
2. Port the CLEO-based Skate Mod to ASI while maintaining compatibility with Urbanize's expectations
3. Add Tony Hawk's American Wasteland animations
4. Document everything with markdowns and commits

We need to understand the full picture better - maybe look up how others interpret this or look for more clues in Urbanize scripts. But given we have the actual files, we need to do deeper reverse engineering.

## Next steps
- Look up more Urbanize scripts for patterns of how they interact with other mods
- Understand the full Skate Mod behavior (we have good strings)
- Plan the ASI port - need to see if there are any existing ASI versions or common patterns
- Also look up "Urbanize skate" integration more broadly from the files
- Create a detailed technical spec doc
