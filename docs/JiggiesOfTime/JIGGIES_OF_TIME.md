# Jiggies of Time (JoT)

A total conversion mod that reimagines Banjo-Kazooie as Ocarina of Time, replacing levels with Hyrule locations, enemies with Zelda creatures, and music with the OoT soundtrack.

- **Base ROM**: US v1.0
- **ROM size**: 64 MB (4x vanilla)
- **SHA-1**: `a9f62c403a21ae9d9a1bbf9e92d752fc2cf85c2b`
- **Game code**: `NBKE` (same as vanilla)

## Asset Changes

667 of 3314 assets modified (20%), same 5575-entry asset table as vanilla. No IDs added or removed — all replacements are in-place, no remap table needed.

| Type | Modified | Size Change | What Changed |
|------|----------|-------------|--------------|
| Level geometry | 150 | +10.5 MB | Every world replaced with OoT locations (Kokiri Forest, Hyrule Field, etc.) |
| Map setups | 110 | +116 KB | New object placements — Zelda enemies, items, NPCs, puzzle triggers |
| Models | 108 | +330 KB | OoT props (pots, arrow targets, stumps, race banners, barrels) |
| Dialog | 195 | +13 KB | Rewritten text for Zelda storyline and characters |
| Midi/music | 90 | +430 KB | OoT soundtrack (Kokiri Forest, Hyrule Field, boss themes, etc.) |
| Sprites | 14 | +128 KB | HUD elements and item icons |

## Code Changes

**28 patches in the core2 overlay** — 316 bytes modified in a 902 KB binary. All other overlays and the main binary init code are identical to vanilla (the 64 init region patches are purely DMA address relocations for the larger ROM).

### Boot sequence override (0x802C3834, 204 bytes)
The largest patch. Vanilla dispatches through a jump table to start the Rareware logo intro. JoT replaces this with direct code that:
1. DMA-loads custom functions from extended ROM space into RAM at 0x80400000+
2. Writes configuration constants to 0x803FFE00
3. Skips the Rareware/N64 logo and boots directly to file select

### Post-file-select boot map
Vanilla loads into Spiral Mountain after picking a save file. JoT redirects this to TTC / Treasure Trove Cove (repurposed as Kokiri Forest / Link's House). The mechanism for this redirect is likely in the JoT functions loaded at 0x80400000+ or the configuration data at 0x803FFE00 — not a simple constant patch in the vanilla code.

### Map ID remap (0x802C5118, 1 byte)
Changes a map constant from 0x85 (MAP_85_CS_SPIRAL_MOUNTAIN_3) to 0x8D (MAP_8D_MMM_INSIDE_LOGGO) — redirects a cutscene transition to a JoT-repurposed map.

### Item quantity rebalancing (0x802D1784, 25 bytes)
Adjusts pickup quantities for eggs/feathers/etc:
- 5 → 15
- 10 → 15
- 15 → 25
- 20 → 10

### Level overlay function redirects (22 patches, ~80 bytes total)

All 22 patches are `jal` redirections inside `overlay.c` — the level overlay init/update/release/draw callbacks. Each BK level has its own overlay that manages level-specific actors, minigames, and environmental effects. JoT replaces the vanilla overlay functions with its own implementations loaded at 0x80400000+.

Every patched function calls into the level overlay subsystem (the `func_8038xxxx` functions that each overlay provides). The source is [`src/core2/overlay.c`](src/core2/overlay.c).

| Level Overlay | Functions Patched | What JoT Replaces |
|--------------|-------------------|-------------------|
| MMM (Mad Monster Mansion) | init, release, update (3 calls each = 9 jals) | Haunted house actors → OoT dungeon mechanics |
| CC (Clanker's Cavern) | init, release, update, draw (2 calls each = 8 jals) | Whale interior → OoT water temple mechanics |
| GV (Gobi's Valley) | init, release, update (4 calls each = 12 jals) | Desert actors + matching game + water control → OoT desert mechanics |
| Lair (Gruntilda's Lair) | init, release, update, draw (1 call each = 4 jals) | Lair hub logic → OoT overworld hub |
| TTC (Treasure Trove Cove) | sandcastle release (1 jal) | Sandcastle codes → OoT chest mechanics |
| BGS (Bubblegloop Swamp) | init, release, update (4 calls each = 12 jals) | Swamp actors + Mr. Vile → OoT swamp mechanics |
| FP (Freezeezy Peak) | init, release, update (4 calls each = 12 jals) | Snow level actors → OoT ice cavern mechanics |

The new JoT function code is stored at ROM offset 0x3F00000 (beyond the vanilla 16 MB boundary) and DMA-loaded to RAM at 0x80400000 by the boot sequence override patch. This code segment is **480 KB containing 1,676 functions** (199 stubs, 1,477 with real code, plus 68 KB of data tables). 10 of the 61 redirected overlay functions are stubs (the vanilla behavior was simply removed). The remaining 51 have replacement implementations.

This is not a small patch — it's a parallel game engine for OoT-style gameplay layered on top of BK's core systems. Full decompilation would be a significant project, though the function signatures and call patterns are well understood from the vanilla decomp.

### Timer adjustments (0x80346xxx, 2 bytes)
- 50 → 80
- 10 → 20

Adjusts cutscene or animation timing values.

## Port Compatibility

**Torch extraction**: Works. Same asset table structure as v1.0, no remap needed. Generates a valid o2r.

**Runtime**: Partial. The decomp has vanilla code, so:
- Levels load and render (geometry is just asset data)
- Music plays (MIDI is asset data)
- Dialogs display (text is asset data)
- Models appear (props are asset data)
- Map setups place objects correctly (setup data is asset data)

But:
- The 22 overlay function redirects won't fire — levels will use vanilla BK overlay behavior (organ minigame in MMM, matching game in GV, sandcastle codes in TTC, etc.) instead of JoT's OoT replacements. The levels will look like OoT but play like BK.
- Boot sequence should be set to "File Select" (the port setting handles this)
- Item quantities will be vanilla (5/10/15/20 instead of 15/15/25/10)
- The post-file-select boot map won't redirect to Kokiri Forest (TTC)

**What would be needed for full support**:
1. Boot to file select — already supported via port setting
2. Post-file-select map override — needs a romhack config or auto-detection
3. Item quantity overrides — 4 constants in the decomp, trivial with a romhack config
4. The 22 overlay redirects — the JoT functions at 0x80400000+ need to be decompiled from the ROM and added as conditional overlay replacements in `overlay.c`. Since Mr-Wiseguy's [JiggiesOfTimeRecomp](https://github.com/Mr-Wiseguy/JiggiesOfTimeRecomp) already has C source for the Recomp version (note saving, TLUT fix, note signs), the overlay functions may be obtainable from the JoT community

## Notes

JoT is a good test case for romhack compatibility because it's primarily an asset swap with minimal code changes. The 22 function redirects are the only barrier to full gameplay — everything visual and auditory works through the standard asset pipeline.
