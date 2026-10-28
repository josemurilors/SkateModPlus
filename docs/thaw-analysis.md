# THAW (Tony Hawk's American Wasteland) Analysis

## Location
`E:\downloads\Tony-Hawk-s-American-Wasteland_Win_EN_Full-Rip\THAW\Game\`

## Structure
- `THAW.exe`, `THAWPM.exe` (patched version with partymod)
- `data/anims/` contains `standardkeyQ.bin`, `standardkeyT.bin` (2048 bytes each) - likely animation/keyframe data
- `data/scripts/engine/engine_params.qb.wpc` (binary script/params)
- `data/scripts/` has scripts
- Many other data dirs (models, textures, sounds, etc.)

## Observations
- Using REA to reverse engineer - opened THAW.exe
- Animation files are small binary blobs (2KB) - might be compressed/packed animation data
- Partymod is used for input handling improvements

## Next steps
- Use REA to look up strings in THAW.exe for trick names, physics, grind detection
- Look up symbols if possible
- Examine animation data format
- Document tricks/physics/mechanics we can extract and adapt
- Create plan for integration with our ASI skate mod
- Make commits as we progress
