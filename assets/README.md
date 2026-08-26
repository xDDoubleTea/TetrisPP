# Assets

This directory ships only fonts. Everything the game draws on top of the board
— backgrounds, character sprites, music and sound effects — is deliberately
absent, and the game runs without it: missing images are replaced by a
magenta/black checkerboard, missing animations are skipped, and missing audio
plays silently.

## Fonts

| File | Face | Licence |
| --- | --- | --- |
| `font/Inter-Bold.ttf` | Headings and menu text (`FontCenter::ui`) | SIL OFL 1.1 — `font/Inter-OFL.txt` |
| `font/JetBrainsMono-Bold.ttf` | Board overlay and statistics (`FontCenter::mono`) | SIL OFL 1.1 — `font/JetBrainsMono-OFL.txt` |

## Supplying your own art and audio

Drop files at the paths below and they are picked up on the next run. The paths
are defined in `data/DataCenter.h` (sound effects) and `Game.cpp` (backgrounds
and music).

```
assets/
  background/  pvzbg.png, pvz_playfield.png, gameover.jpg
  gifs/        PeaShooter_Idle1.gif, PeaShooter_Spitnew.gif, ProjectilePea.png,
               plants-vs-zombies-zombie.gif, pvz-zombieatt.gif
  music/       menu.wav, PVZBGM.wav, gameover_sound.wav
  sfx/         harddrop.ogg, rotate.ogg, clearline.ogg, clearquad.ogg,
               clearspin.ogg, combo_1.ogg … combo_16.ogg, i/j/l/o/s/t/z.ogg, …
```

Images load through Allegro's image addon (PNG, JPG, …), animations through the
bundled `algif5` GIF decoder, and audio through the acodec addon (OGG and WAV;
MP3 is not used).
