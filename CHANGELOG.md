# Changelog

Everything below is relative to upstream **[HarbourMasters/Lighthouse]** 1.1.0
"Hatteras Alfa". Lighthouse is the Banjo-Kazooie decompilation port built on
libultraship; this fork adds a working Wii U target and a set of quality-of-life
changes, and keeps the PC target building from the same tree so the two can play
together.

[HarbourMasters/Lighthouse]: https://github.com/HarbourMasters/Lighthouse

---

## Wii U support

The Wii U was a new target. It is 32-bit and big-endian, and both of those
surfaced problems no other platform had hit.

### Rendering (GX2 / Latte)

- **Vertex ring race** — the draw buffer was reset every frame with no GPU
  fence, so the GPU read vertices the CPU had already overwritten. This was the
  cause of the coloured triangles smeared across the screen. The ring now only
  wraps after `GX2DrawDone()`.
- **Scissor rectangle** — rewritten to mirror the viewport's y-flip with a real
  clamp against the buffer, instead of `min(width, x)`.
- **Unbound samplers** — a 1×1 white dummy texture is bound to any sampler the
  shader reads but nothing has filled, which was rendering black.
- **Sampler state** targeted the wrong tile, so filter and clamp modes landed on
  whichever texture happened to be current.
- **Framebuffer-as-texture staleness** — the colour and texture views share one
  allocation; the texture unit kept serving what it had cached, which is why the
  pause and game-over screens showed a stale image.
- **Framebuffer sampler clobbered by shader loads.** `SelectTextureFb` binds a
  framebuffer to sampler 0, but `LoadShader` runs *afterwards* and re-bound the
  ordinary texture array over it. Framebuffers bypass that array, so the pause
  screen sampled the last HUD texture instead — a magnified digit that tracked
  your jiggy count. The binding is now tracked separately.
- **CPU frustum clipping** for geometry Latte will not clip itself.
- Zero-dimension and non-finite-vertex guards.

### System integration

- **HOME menu handover.** MEM1 is foreground memory and is reclaimed by the
  system; the render surfaces live there. They are now released and rebuilt
  around the handover, drawing is gated on owning the foreground, and the VI
  cadence is suspended so retraces do not pile up against a thread that cannot
  consume them.
- **Secondary framebuffers were never rebuilt** after a handover. The release
  path zeroed their dimensions expecting `UpdateFramebufferParameters` to
  restore them, but that is only called when the *requested* size changes — and
  it does not change across a handover. Each framebuffer now records its
  parameters and the restore replays them.
- **Queue deadlock.** A single `wake` flag served both the not-full and
  not-empty waits on a message queue, so either side could consume the other's
  wakeup and clear it, leaving the real waiter asleep beside a queue with data in
  it. Two HOME presses in a row hit that window reliably. The waits now test the
  queue itself.
- **Missing MEM1 free** in the frame-heap teardown.
- **TV and DRC scale** were set from the default framebuffer size rather than the
  configured one, putting the image in a corner after the 720p change.
- Internal resolution set to 1280×720.

### Input

- **The enhancements menu was unreachable on console.** Three separate defects,
  each sufficient on its own: `Fast3dGui::HandleWindowEvents` had no GX2 case, so
  the controller snapshot was built and discarded; the SDL→VPAD reconstruction
  that fills it was commented out, so the snapshot was empty; and `io.DisplaySize`
  was never set on this backend, leaving ImGui's `(-1,-1)` default — the GX2
  renderer returns early on a non-positive framebuffer size, so the overlay had
  never drawn a pixel.
- **Touch input.** `tpNormal` holds *raw* panel coordinates on hardware and
  consumers calibrate it; the shim was writing screen pixels, so the system
  keyboard — which calibrates internally — collapsed every tap into one corner.
  The panel's raw Y also runs opposite to screen Y, which came out inverted.
- **Menu input sampled once per frame.** The main loop iterates ~2500 times a
  second and the GUI was fed on every pass, stepping the on-screen keyboard ~40
  times per displayed frame.
- **Event trickling disabled for touch.** A touch has no hover phase, so ImGui
  spreading position and press across frames landed clicks at a stale cursor.
- SDL event-queue drain (the leak behind a progressive frame-rate collapse),
  controller re-enumeration fixes, and Pro Controller support.

### Assets and audio

- Geo command tree byte order (see **Cross-platform** below).
- Audio heap sizing on 32-bit targets.
- Save file replacement made crash-safe: write to a temporary, rotate through a
  `.bak`, restore the original if the rename fails.

---

## Quality of life

Everything here is a CVar and can be toggled in-game.

### Camera

- **Dual-analog free look**, on by default, with an eased turn rate and a curved
  stick response rather than driving the angle straight from the stick.
- **Underwater camera control.** The game pins the camera to a fixed state while
  submerged; free look can now take it over.
- **Fixed camera nodes can be overridden.** Level-placed zoom and pivot nodes
  re-assert every frame; they now stand down while free look is driving.
  `Camera.FreeLook.OverrideFixed`
- **Steering survives a collision.** The collision resolve re-derived the orbit
  yaw from wherever the camera slid to, discarding your input — a wall captured
  the camera until you out-rotated it. Collision still owns the position.
- **Spring arm.** The orbit shortens against geometry and eases back out, with a
  dwell so it settles instead of pumping in doorways.
  `Camera.FreeLook.WallSmoothing`

### Presentation

- **Widescreen** (Hor+): the projection follows the real render aspect, so the
  extra width is extra view rather than stretch.
- **Cutscene actor culling** keeps vanilla 4:3 culling during cutscenes, so
  actors waiting off-camera stay hidden. `Graphics.CutsceneActorCull`
- **Upscaled texture pack** (optional, `mods/`): 5,388 textures at 2× via
  Scale2x, with the intro's sun and clouds re-done at 8× with a smooth filter —
  Scale2x is built for pixel art and left those gradients faceted. The iris
  transition mask is synthesised at 512×512 from the original's alpha profile
  rather than upscaled, because it is a mathematical shape.
- Wii U menu icon and boot splashes.

### Controls and text

- Modern control scheme applied at startup (the settings menu that applied it
  was unreachable on console).
- **Button prompts rewritten** for the GamePad — "PRESS THE A BUTTON" and the
  C-button wording remapped. Every replacement is no longer than what it
  replaces, because the text box wraps on a fixed width.
  `Controls.RewritePrompts`
- Underwater dive controls un-inverted, correctly scoped to submerged movement
  only — surface swimming uses the same axis for forward and back.

### Randomizer

- **Seed derivation.** The fallback seed came from `std::chrono::system_clock`,
  which needs an RTC the Wii U does not reliably expose — the same seed could
  come up every boot. It now mixes the wall clock, a monotonic clock, a call
  counter and the console tick counter through a splitmix64 finaliser. The call
  counter matters: the seed is drawn once per pool, and two draws in the same
  tick were handing both pools an identical shuffle.

---

## Cross-platform play

Anchor (Lighthouse's co-op layer) now works between Wii U and PC.

- **Networking enabled on Wii U.** The transport compiled but never linked: the
  CMake `CafeOS` branch did not link SDL2_net, unlike the Windows and Unix
  branches. `SDLNet_Init()` was also never called.
- **Receive-loop busy-spin.** `SDLNet_CheckSockets` was polled with a zero
  timeout, so with no traffic the loop span a core flat out. A desktop absorbs
  that; on three cores it starved the main loop and hung the console at the HOME
  menu. It now blocks with a 16 ms timeout.
- **The PC target builds again from this tree.** Five defects, all latent
  because only one target had ever been compiled:
  - libultraship's backend-exclusion filters match a path layout that no longer
    exists, so they never fire — and nothing excluded the GX2 backend at all,
    which only compiles against devkitPro headers.
  - 17 headers had no include guard. GCC tolerated it; MSVC reported a class
    redefinition.
  - `WIIU_TRACE` used unguarded while its header was included only under
    `#ifdef __WIIU__`, plus 13 direct `WHBLogPrintf` calls with no off-console
    definition.
  - `gfx_wiiu_has_foreground` and `OSSleepTicks` referenced from the shared main
    loop.
- **Geo tree byte order is now a choice at extraction time.** Upstream changed
  Torch to emit the tree big-endian for the Wii U, which broke every
  little-endian target. Swapping it at load was tried twice and does not work —
  the commands are neither contiguous nor all fixed-length, and walking the
  buffer desynchronises. Torch knows every field exactly, so it now writes the
  order the reading machine needs: `TORCH_GEO_LITTLE_ENDIAN=1` when extracting
  for a little-endian target, big-endian by default.

Because Anchor speaks JSON over TCP, the endianness gap between PowerPC and x86
does not reach the wire format. Both clients must be built from the same commit:
the protocol carries save-check semantics that changed between 1.1.0 and this
tree, and a mismatch desyncs items and map transitions.

---

## Known issues

- **Dialogue text can overflow the text box.** The frame's geometry comes from
  `gFramebufferWidth` with hardcoded offsets while the text is laid out by a
  separate path, and neither consults the aspect ratio. Present in upstream and
  in the official 1.1.0 build; not introduced here.
- **The intro logo model does not appear.** Unresolved.
- **Bottles' Bonus is harder than intended.** The picture is a live render of the
  world, and at 720p with upscaled textures the moving character is legible in a
  way it was not at native resolution.
- Occasional startup hang showing the boot splash over the Wii U menu. Rare and
  long-standing.
