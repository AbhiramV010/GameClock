# Brag Plan: GameClock

## What is this app?
A Raspberry Pi Pico W alarm clock. At 07:30 it chirps, then makes you repeat a growing four-pad light-and-tone sequence five rounds in a row before it stops.

## The angle
Second-person ad. The viewer is the one in bed: asleep, hit by the alarm, reaching for a snooze that the firmware never implemented, then playing their way out. Product claims come from the code; the framing is invented.

## Hook (first 2-3 seconds)
"07:29" in dim red with "You're asleep." It flips to "07:30" with the firmware's own alarm chirp (two 150 ms pulses at 1000 Hz), red screen flashes, and camera shake.

## Key moments
- "Reach for snooze." / "There isn't one." (no snooze exists in `GameClock.ino`)
- The enclosure: lid slams onto the base on the music drop, display reading 07:30 through the lid window.
- The game in POV: tilted pads, "Watch." then "Your turn.", tap ripples, the real pad tones (330/392/494/587 Hz).
- The fail: wrong pad, red flash, "Wrong. Back to round 1." (`playGame` restarts from level 1).
- The win: 1-2-3-4-5 slam in on the beat. "Five rounds in a row."

## Outro / punchline
Music cuts dead. "07:34" / "Now you're awake." End card: "GameClock" / "An alarm clock on a Pico W." with a bell hit.

## User flow worth showing
Alarm fires → watch the sequence → repeat it → fail and restart → five rounds → silence.

## Tone
- Preset: cinematic
- Creative direction: fast-paced, second-person product ad.
- Interpretation: 2.39:1 letterbox, hard cuts, flashes and shake on hits, music drop on the product reveal, hard music stop before the end card.

## Format: landscape — 1920x1080 (letterboxed to 2.39:1)
## Duration: 23s

## Visual identity
- Background: #050606
- Accent: #ff3b2e (LED red)
- Text: #f4f1e8
- Case: #2d6a4f (board green), from `enclosure/base.stl` + `lid.stl`
- Display font: Inter 800; digits in bold monospace

## Share copy (draft)
07:30. Your alarm goes off, and there's no snooze. Just a memory game that won't stop until you beat it five rounds in a row. GameClock: an alarm clock on a Pico W.

## Audio direction
- Role: cinematic support with a hard stop
- Music: `happy-beats-business-moves-vol-1-by-ende-dot-app.mp3`, track offset 13.92s, placed at 3.0s, cut at 18.6s
- Music cue guidance: preset strongCue 16.02s lands at 5.10s (lid slam). Beat grid at 0.5s drives the pad sequence (9.6/10.1/10.6), presses (11.6/12.1/12.6), and numeral slams (16.1-18.1).
- Audio-reactive treatment: none (extraction helper unavailable)
- SFX: generated 1000 Hz alarm chirp and pad tones from the firmware constants; click on taps; error on the wrong press; impact on numerals and the slam; bell on the end card.
- Restraint rule: silence from 18.6s until the end-card bell.

## Storyboard
1. 0.0-1.8 — Asleep — "07:29", "You're asleep." Slow push-in.
2. 1.8-3.0 — Alarm — "07:30", chirp, two red flashes, shake.
3. 3.0-3.9 — "Reach for snooze."
4. 3.9-4.8 — "There isn't one." Slam + shake.
5. 4.8-9.1 — Case — lid slams at 5.10 (beat-locked); "Meet GameClock." then "It wakes you up with a memory game."
6. 9.1-13.6 — POV game, round 3 — "Watch." sequence, "Your turn." three taps.
7. 13.6-16.1 — Round 4 — fast sequence, wrong tap at 14.6, "Wrong." / "Back to round 1."
8. 16.1-18.6 — "Five rounds in a row." 1-5 slam on beats.
9. 18.6-20.4 — Silence — "07:34", "Now you're awake."
10. 20.4-23.0 — End card — "GameClock", "An alarm clock on a Pico W.", fade to black.
