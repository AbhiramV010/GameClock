# Hyperframes Composition Brief: GameClock

## Objective
Create a short deadpan brag video for GameClock, a Pico W alarm clock with a memory-game dismissal.

## Output
- Composition directory: `brag-output/composition/`
- Rendered video: `brag-output/brag.mp4`
- Format: landscape — 1920x1080
- Duration: 22s

## Source Material
- Project root: /home/user/GameClock
- Primary files read: `GameClock.ino`, `pins.txt`, `3d_render.png`
- Product name: GameClock
- Tagline / strongest claim: "wakes user up with a memory game"
- Key UI or visual moment to recreate: the four-pad sequence game (LED + tone per pad), and the KiCad 3D render of the board
- Copy that must appear verbatim:
  - "wakes user up with a memory game"
  - "An alarm clock on a Pico W."
  - "Four buttons. Four LEDs. One buzzer."
  - "Five rounds in a row to shut it up."
  - "Edit these and reflash."

## Creative Direction
- Tone preset: deadpan
- Creative direction: a product demo for an alarm clock that will not let you go back to sleep until you prove you are awake.
- Interpretation: long holds, one line at a time, quiet bed, sparse SFX.
- Angle: the device takes its job seriously and the copy is the firmware's own comments.
- Hook: "07:30" in seven-segment red, "wakes user up with a memory game" under it.
- Outro / punchline: "Edit these and reflash."
- Avoid:
  - Generic SaaS language
  - Abstract filler visuals
  - Inventing features (no snooze, no app, no sensors: none exist in the code)

## Visual Identity
- Background: #0a0c0b
- Text: #f4f1e8
- Accent: #e0352b
- Display font: Inter 600 (fallback: system-ui)
- Body font: Inter 400
- Visual references from the project: `3d_render.png` board crop; LED red; the pad tones

## Storyboard
Use the storyboard in `brag-output/brag-plan.md` as the creative contract.

Scene summary:
1. Hook — 3s — "07:30" + "wakes user up with a memory game"
2. The board — 4s — board crop + "An alarm clock on a Pico W." / "Four buttons. Four LEDs. One buzzer."
3. The game — 7s — pad sequence + "Repeat the pattern."
4. The count — 4s — five round markers + "Five rounds in a row to shut it up."
5. The punchline — 4s — "07:31" + "Edit these and reflash."

## Audio
- Audio role: warm bed, dry, sparse SFX
- Audio arc: quiet bed through, pad tones in scene 3, a bong on the flip, bed fades out
- Music: `happy-beats-business-moves-vol-1-by-ende-dot-app.mp3`
- Music treatment: 0.35 volume under scenes, fade out over the last 2s
- Music cue guidance: preset `happy-beats-business-moves-vol-1-by-ende-dot-app.music-cues.json`, tempo 120.19; one strong cue lock at scene 3 start
- Audio-reactive treatment: none (extraction helper unavailable in this session)
- Audio-coupled moments:
  - Scene 3 — pad flash tones and press clicks
  - Scene 4 — click per round marker fill
  - Scene 5 — bong on digit flip
- SFX selection guidance: `click_001.ogg`, `switch_001.ogg`, `bong_001.ogg` from the bundled interface/sfx folders
- SFX analysis guidance: `skills/brag/assets/sfx/sfx-analysis.md` if present
- Exact SFX choice: as above
- Audio files: copy the music track and SFX into `brag-output/composition/assets/`

## Hyperframes Instructions
Composition built directly against Hyperframes HTML conventions (`data-start`, `data-duration`, `class="clip"`, paused root timeline on `window.__timelines`). The scaffold came from `npx hyperframes init`. The domain skills were not available in this session, so reference docs come from `npx hyperframes docs`.

Requirements:
- Show the real board render and the real pad tones.
- Keep all text readable: hold each line at least 0.8s settled.
- 22s total.
- Run `npx hyperframes check` before render.
- Keep creation and rendering local.
