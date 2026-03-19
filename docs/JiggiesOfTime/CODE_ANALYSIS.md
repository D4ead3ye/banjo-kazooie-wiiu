# JoT Code Analysis

Decompilation reference for the 61 JoT overlay replacement functions. These replace vanilla level overlay callbacks in [`src/core2/overlay.c`](../../src/core2/overlay.c).

The JoT code segment lives at ROM 0x3F00000, loaded to RAM 0x80400000 at boot. 480 KB total, 1,676 functions (199 stubs, 1,477 with code), 68 KB data tables.

---

## Function Inventory

### Stubs (10 functions — just `return;`)

| Address | Label | Replaces |
|---------|-------|----------|
| 0x80406D5C | MMM_release_1 | func_8038953C |
| 0x804074F8 | MMM_release_2 | MMM_func_80389CD8 (organ game release) |
| 0x80407500 | MMM_init_2 | func_80389CE0 (organ game init) |
| 0x804138C0 | GV_release_1 | func_8038FF60 |
| 0x80413954 | GV_update_1 | func_8038FFF4 |
| 0x80413A58 | GV_release_2 | func_803900F8 |
| 0x8041434C | GV_release_4 | gv_waterCtrl_end |
| 0x8043A248 | BGS_release_1 | — |
| 0x8043A418 | BGS_release_3 | — |
| 0x8043B268 | BGS_init_4 | — |

### Tiny (<100B, 1-3 calls — easy decompile)

| Address | Size | Calls | Label | Replaces |
|---------|------|-------|-------|----------|
| 0x804081B4 | 32B | 1 | MMM_release_4 | func_8038A994 |
| 0x80401CE4 | 48B | 1 | CC_init_2 | func_80388104 (Clanker init) |
| 0x80413A60 | 56B | 2 | GV_init_2 | func_80390100 |
| 0x8044131C | 56B | 1 | FP_release_2 | — |
| 0x8041A0CC | 72B | 3 | TTC_release | maCastle_release (sandcastle) |
| 0x80413D4C | 32B | 1 | GV_release_3 | gv_matchingGame_reset |
| 0x804458868 | 92B | 3 | LAIR_draw | lair_func_8038E768 |
| 0x80441354 | 92B | 3 | FP_init_2 | — |

### Small (100-300B, 3-10 calls — moderate decompile)

| Address | Size | Calls | Label |
|---------|------|-------|-------|
| 0x80406D64 | 108B | 2 | MMM_init_1 |
| 0x80401D14 | 120B | 3 | CC_update_1 |
| 0x804081D4 | 124B | 7 | MMM_init_3 |
| 0x804030B0 | 128B | 3 | patched asset loader |
| 0x80413A98 | 100B | 3 | GV_update_2 |
| 0x804138C8 | 140B | 6 | GV_init_1 |
| 0x804028C4 | 160B | 9 | CC_release_1 |
| 0x80414354 | 160B | 7 | GV_init_4 (water ctrl) |
| 0x80413D6C | 156B | 4 | GV_init_3 (matching game) |
| 0x8043A1B0 | 152B | 4 | BGS_release_2 |
| 0x8043A380 | 152B | 4 | BGS_update_1 |
| 0x80441280 | 156B | 4 | FP_init_1 |
| 0x80456E48 | 184B | 8 | LAIR_release |
| 0x8043A250 | 232B | 3 | BGS_init_1 |
| 0x8043A420 | 224B | 4 | BGS_init_3 |
| 0x8041EC80 | 228B | 9 | TTC_update (sandcastle) |
| 0x8043B164 | 260B | 3 | BGS_release_4 |
| 0x80414354 | 160B | 7 | GV_init_4 |

### Medium (300-700B, 10-30 calls — needs work)

| Address | Size | Calls | Label |
|---------|------|-------|-------|
| 0x80408264 | 372B | 15 | MMM_update_3 |
| 0x804077E0 | 380B | 9 | MMM_update_2 (organ replacement) |
| 0x8045F570 | 380B | 15 | unknown_6 |
| 0x804433D0 | 388B | 16 | FP_update |
| 0x80406E8C | 412B | 12 | MMM_update_1 |
| 0x80417B40 | 440B | 19 | unknown_3 |
| 0x80407614 | 460B | 6 | MMM_release_3 |
| 0x804019B0 | 472B | 21 | boot_init |
| 0x80401D8C | 476B | 16 | CC_update_2 (Clanker update) |
| 0x80406900 | 616B | 27 | unknown_1 |
| 0x8041A370 | 608B | 26 | TTC_init (sandcastle) |
| 0x8043AF04 | 608B | 13 | BGS_update_3 (Mr. Vile) |
| 0x80428E60 | 648B | 29 | unknown_4 |
| 0x80457018 | 404B | 21 | LAIR_init |

### Large (700B+, 30+ calls — complex, needs SoH reference)

| Address | Size | Calls | Label |
|---------|------|-------|-------|
| 0x80412AB4 | 876B | 41 | unknown_2 |
| 0x80413E08 | 976B | 32 | GV_update_3 (matching game replacement) |
| 0x80402370 | 1004B | 32 | CC_draw |
| 0x80440CE4 | 1056B | 48 | FP_release_1 |
| 0x8047322C | 1088B | 51 | unknown_7 |
| 0x8042B728 | 1092B | 52 | unknown_5 |
| 0x80402B5C | 1364B | 30 | CC_init_1 |
| 0x804541C4 | 1476B | 70 | FP_init_3 |
| 0x8044DFE0 | 1600B | 78 | FP_release_3 |
| 0x804581B0 | 1720B | 68 | LAIR_update |
| 0x80402964 | 336B | 14 | CC_release_2 (Clanker release) |

---

## Decompilation Progress

- [ ] Stubs (10/10 — trivially done)
- [ ] Tiny functions (0/8)
- [ ] Small functions (0/18)
- [ ] Medium functions (0/14)
- [ ] Large functions (0/11)

## Notes

- All functions call into known BK decomp APIs (the `func_803xxxxx` addresses map to functions in `port_prototypes.h`)
- The "unknown" functions need caller analysis to determine which overlay they belong to
- Ship of Harkinian (OoT decomp) is a reference for understanding the OoT gameplay patterns these functions implement
