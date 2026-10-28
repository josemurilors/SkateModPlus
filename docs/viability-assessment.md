# Viability Assessment

## Current state
- **Urbanize** is a large CLEO-based mod (Urbanize - Root.cs) with ASI component (Urbanize (Junior_Djjr).asi), animations (djjr_urbanize.ifp), extensive data-driven spawning (Urbanize.dat). It has integration points for skateboarding - GLNPS.cs references sktbd and reads skate mod config.
- **Skate Mod (existing)** is CLEO-based (skateboard.cs), vehicle-based implementation (model sktbd as vehicle ID 74, uses choppa/bmx anims), has config ini, audio, pickups. Works with Urbanize in areas where integrated.

## Goal
1. Improve existing Skate Mod
2. Port to ASI
3. Add Tony Hawk's American Wasteland animations
4. Integrate with Urbanize (where it already operates)

## Viability
**High feasibility** if we have:
- Clear understanding of existing behavior (we're extracting it)
- Knowledge of SA modding: CLEO, ASI, handling, animations (IFP), models
- Urbanize integration points mapped (GLNPS + general awareness)

**Challenges:**
- Existing scripts are compiled CLEO (.cs files are compiled) - hard to fully decompile to readable source. We have string extraction which gives us behavior patterns.
- Porting CLEO logic to ASI requires reimplementation using plugin SDK (e.g., plugin-sdk, cleo-redux patterns, or direct memory hooks). Need to understand exact mechanics.
- Adding THAW animations means getting/converting IFP animations and integrating them into the skate system. Need animation names/structure.
- Maintaining compatibility with Urbanize integration (GLNPS reads skate mod ini, expects certain behavior)

## Recommended approach
1. **Document everything first** (in progress) - map all integration points, understand full behavior
2. **Create clean ASI implementation** that replicates/improves current behavior
3. **Add animation support** for THAW animations - need to analyze THAW animation format/names
4. **Ensure Urbanize compatibility** - keep config format compatible or update integration points
5. **Test incrementally**

## Next steps
- Get THAW animations if available? User says "colocar as animações do Tony Hawks american wasteland no mod" - we need to find THAW animation assets. Let us look around more - maybe in other locations?
- Look up if there are THAW-related files anywhere
- Start planning ASI structure
- Continue deeper reverse engineering of both mods
