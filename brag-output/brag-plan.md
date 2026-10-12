# Brag Plan: GameClock

## What is this app?
A Raspberry Pi Pico W alarm clock. At 07:30 it chirps, then makes you repeat a growing four-pad light-and-tone sequence five rounds in a row before it stops.

## The angle
Deadpan. The device is a hardware alarm clock, and the joke is how seriously it takes its own job. The firmware comments are the voice: "wakes user up with a memory game", "rounds in a row to shut it up", "edit these and reflash". Nothing gets invented about the product.

## Hook (first 2-3 seconds)
Black screen. Digits "07:30" in LED red, colon blinking. Under them: "wakes user up with a memory game." The number is the alarm time from `ALARM_HOUR` and `ALARM_MINUTE`.

## Key moments (the middle)
- The board itself: the KiCad 3D render (`3d_render.png`, cropped to the board) with the parts called out in text. Four buttons, four LEDs, one buzzer.
- The game: four pads light in sequence, each with its tone (330, 392, 494, 587 Hz). The sequence grows one step per round.
- The count: five round markers fill in one at a time.

## Outro / punchline
Display flips to 07:31. Text: "Edit these and reflash." Last beat, no logo.

## User flow worth showing
Entry: 07:30, the alarm chirps (all four LEDs flash twice). Key action: repeat the sequence by pressing the matching buttons. Result: five rounds in a row, LEDs go dark, the clock shows the time again.

## Tone
- Preset: deadpan
- Creative direction: a product demo for an alarm clock that will not let you go back to sleep until you prove you are awake.
- Interpretation: long holds, one line of text at a time, no music swells on the jokes. Quiet and dry.

## Format: landscape — 1920x1080
## Duration: 22s

## Visual identity (from the project)
No CSS or web assets exist, so the palette comes from the board render.
- Background: #0a0c0b (near-black, the enclosure/viewer dark)
- Accent: #e0352b (LED red, the lit pads and the seven-segment digits)
- Text: #f4f1e8 (warm off-white)
- Board green for the render frame border: #0f4a2b
- Display font: Inter 600 for text lines; monospace bold for the digits
- Body font: Inter 400
- Strongest visual element: the KiCad 3D render, and the red LED pads

## Share copy (draft)
I built an alarm clock that makes you repeat a memory game five rounds in a row before it shuts up. Pico W, four buttons, four LEDs, one buzzer. Edit these and reflash.

## Audio direction
- Role: warm bed, dry. Sparse SFX on presses only.
- Music: `happy-beats-business-moves-vol-1-by-ende-dot-app.mp3`, low under the whole video, from 0s.
- Music treatment: volume at 0.35 under the scenes, fade out over the last 2s.
- Music cue guidance: preset `happy-beats-business-moves-vol-1-by-ende-dot-app.music-cues.json` (tempo 120.19). One strong-cue lock: the fourth round marker at 16.02s (preset strongCue). Sequential pad flashes snap to the beat grid only if they stay readable (≥0.4s apart).
- Audio-reactive treatment: none. The extraction helper is not available in this session, so it is skipped and noted here.
- Audio-coupled moments: the pad flashes (tone per pad), the round markers (click per fill), the final dismissal (one bong).
- SFX posture: sparse. Use `click_001.ogg` for round markers, `switch_001.ogg` for button presses, `bong_001.ogg` for the final beat.
- Restraint rule: no SFX on text reveals, no music swell on the punchline.

## Storyboard

### Scene 1 — Hook — 3s
Black. "07:30" in seven-segment red digits, colon blinks. Under: "wakes user up with a memory game."
Sequential/interaction: none
Audio intent: one dry tick on the colon.
Audio-coupled idea: none
Music: bed starts at 0s
Transition mood: hard → Scene 2

### Scene 2 — The board — 4s
Crop of `3d_render.png` (board area only) pushes in slowly. Text: "An alarm clock on a Pico W." Then "Four buttons. Four LEDs. One buzzer." on a second line.
Sequential/interaction: yes — the two text lines appear one after the other, each held ~1.4s.
Audio intent: quiet, no SFX.
Audio-coupled idea: none
Music: bed continues
Transition mood: soft → Scene 3

### Scene 3 — The game — 7s
Four square pads in a row, unlit, labeled with their tones (330, 392, 494, 587 Hz). All four flash twice (the alarm chirp). Then the sequence plays: pad 2, then pad 4, then pad 1, each lighting with its tone. Text under the row: "Repeat the pattern." Cursor presses pad 2, then pad 4, then pad 1 in order.
Sequential/interaction: yes — pads light one at a time; cursor simulates the three presses.
Audio intent: the pad flashes and press clicks carry this scene.
Audio-coupled idea: each pad flash on its tone; the press clicks land with the cursor.
Music: bed continues, one strong-cue lock at 0s of this scene.
Transition mood: hard → Scene 4

### Scene 4 — The count — 4s
Five round markers in a row. Each fills red at a 0.6s interval. Text: "Five rounds in a row to shut it up."
Sequential/interaction: yes — markers fill one by one.
Audio intent: a click per fill.
Audio-coupled idea: click on each fill.
Music: bed continues
Transition mood: soft → Scene 5

### Scene 5 — The punchline — 4s
Digits switch to "07:31". Text: "Edit these and reflash." Hold.
Sequential/interaction: none
Audio intent: one bong on the digit change, then the bed fades under the last 2s.
Audio-coupled idea: bong on the digit flip.
Music: fades out over the last 2s.
Transition mood: slow fade to black.

**Music mood for this video:** deadpan
**Audio summary:** one dry tick, a quiet bed, pad tones and clicks during the game, a bong on the digit flip, bed fades to black.
