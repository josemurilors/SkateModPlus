# Skate Mod - Audio and Animation details

## Audio
- Loads `CLEO\CLEO_AUDIO\SKATE.MP3` via Audiostream
- Volume from ini `Volume` (float) multiplied with menu volume, applied via `set_audio_stream_volume`
- Played on actions (play/release pattern)

## Animations
- Uses animation "CHOPPA_sprint" from IFP "CHOPPA" for mounting/riding
- Model group is bmx/chopper style (per Vehicles.ide: anim group CHOPPERB, bmx/bicycle style)
- Also has `choppa.ifp` in root
- `bikearmfix.asi` suggests arm/animation fixes

## Goal: Add Tony Hawk's American Wasteland animations
THAW has a wide range of skate animations (tricks, manuals, grinds, etc.). We need to:
1. Find or obtain THAW animation files (.ifp)
2. Convert/integrate them into the skate system
3. Expand animation set beyond just mount animation
4. Make it work with ASI port

But we don't have THAW animations in the current files. This might need to be added separately. Let us look up if there are common THAW animation packs or if we need to source them. But the user says "colocar as animações do Tony Hawks american wasteland no mod" - so we need to add them. Maybe look up common naming/conversion patterns for SA.

Also let us look up more about the full Skate Mod behavior to understand what needs to be ported.
