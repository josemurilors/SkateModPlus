# Plan - Next Steps

## What we know so far
- **Urbanize**: Large mod, CLEO scripts + ASI + animations. Has integration with skate mod (GLNPS.cs reads skate mod ini, references sktbd). Spawns various NPCs/objects via Urbanize.dat.
- **Skate Mod (existing)**: Vehicle-based (skateboard as vehicle), CLEO script, uses CHOPPA animations, has pickups, audio, configurable via ini.
- **Goal**: Improve existing Skate Mod, port to ASI, add Tony Hawk's American Wasteland animations, integrate with Urbanize.

## Immediate next steps (documentation + understanding)
1. [x] Document Urbanize structure
2. [x] Document existing Skate Mod  
3. [x] Find integration points (GLNPS)
4. [x] Viability assessment
5. [ ] Look for more integration points between them
6. [ ] Understand if there might be THAW animations somewhere else or need to source them
7. [ ] Create detailed technical plan for ASI port

## For THAW animations
Tony Hawk's American Wasteland has skate animations. We need to find them. They might be in .ifp format. Let us search more broadly - maybe in other mod folders, or common naming patterns.

## For ASI port
Need to understand SA plugin development. Common approaches:
- Use plugin-sdk (https://github.com/DK22Pac/plugin-sdk)
- Or use CLEO Redux style
- Or write custom ASI

But we need to look up what tools/libraries might be available or common patterns. Also the user wants us to "port to ASI" - so create ASI plugin version.

## Questions to answer
- What exactly does GLNPS do with skate mod? (beyond reading ini)
- Are there other Urbanize scripts that interact with skate?
- How to properly integrate with Urbanize?
- Where do we get THAW animations from?
- What's the best ASI approach for SA?

Let us keep digging.
