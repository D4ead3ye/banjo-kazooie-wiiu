# Banjo-Kazooie — Wii U port, with PC cross-play
<img width="1024" height="576" alt="da0e5f3a-683a-4b9e-80a4-2c2be79a3d06" src="https://github.com/user-attachments/assets/d8d2aca5-a76f-4372-957b-a7c2dce4e7b3" />

A fork of [HarbourMasters/Lighthouse](https://github.com/HarbourMasters/Lighthouse)
that adds a working **Wii U** target, a set of quality-of-life changes, and keeps
the **PC** target building from the same tree so the two can play together over
Anchor.

> **No game data is included.** These builds read your own Banjo-Kazooie (USA)
> ROM and extract what they need on first run. Nothing in this repository or its
> releases contains Nintendo or Rare assets.

## Downloads

| | |
|---|---|
| `Banjo-Kazooie-WiiU.zip` | Aroma CFW. Copy `wiiu/` to your SD card. |
| `Banjo-Kazooie-PC-Win64.zip` | Windows x64. Unzip and run. |

Each archive contains a `READ ME FIRST.txt` with setup and controls.

## Highlights

- **Wii U support** — GX2 renderer fixes, HOME menu suspend/resume, big-endian
  asset handling, GamePad input including the touchscreen and on-screen keyboard
- **Dual-analog camera** with a spring arm, underwater control, and the ability
  to override the game's fixed camera angles
- **Widescreen** (Hor+), 60fps, 720p internal resolution
- **Cross-play between Wii U and PC** over Anchor
- Button prompts rewritten for the GamePad; optional upscaled textures generated
  from your own ROM

See [CHANGELOG.md](CHANGELOG.md) for the full list, including the bugs found and
what caused them.

## Building

```
python dashboard.py --build --deploy    # Wii U (devkitPro + wut)
python dashboard.py --pc                # Windows (VS Build Tools)
```

Both targets build from one tree. For a little-endian target the assets must be
extracted with `TORCH_GEO_LITTLE_ENDIAN=1`; see the changelog for why.

## Credits

Lighthouse and libultraship are by **Harbour Masters** and contributors. The
Banjo-Kazooie decompilation is by the **BanjoDecomp** project. This fork only
adds the Wii U target and the changes listed in the changelog.

Licensed as upstream — see `LICENSE.md` in the source tree.
