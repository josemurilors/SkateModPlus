# THAW Mechanics Research

## Tricks found
From game data references: ollie, kickflip, heelflip, grind, manual, revert, nollie appear in mission text. Also BS/FS variations common in TH series.

## Animation files
- `data/anims/standardkeyQ.bin`, `standardkeyT.bin` - 2048 bytes each, binary format
- May contain animation key mappings or animation data

## Engine params
- `data/scripts/engine/engine_params.qb.wpc` - likely contains physics/trick parameters

## Goal
Extract useful mechanics from THAW to improve our skate mod:
1. Better trick system (flip tricks, grab tricks, grinds, manuals)
2. Improved physics/balance
3. More animations
4. Grind detection and mechanics
5. Revert mechanics
6. Combo system

## Approach
- Use REA to decompile/analyze THAW.exe more deeply, look for trick/physics related code
- Examine binary animation formats
- Look up common THAW modding knowledge patterns
- Document everything for implementation in ASI port
