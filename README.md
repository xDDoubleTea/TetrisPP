# TetrisPP

A guideline-style Tetris game with a tower-defense layer, written in C++17 on
top of [Allegro 5](https://liballeg.org/).

[![build-linux](https://github.com/xDDoubleTea/TetrisPP/actions/workflows/build-linux.yml/badge.svg)](https://github.com/xDDoubleTea/TetrisPP/actions/workflows/build-linux.yml)
[![build-macOS](https://github.com/xDDoubleTea/TetrisPP/actions/workflows/build-macOS.yml/badge.svg)](https://github.com/xDDoubleTea/TetrisPP/actions/workflows/build-macOS.yml)
[![build-windows](https://github.com/xDDoubleTea/TetrisPP/actions/workflows/build-windows.yml/badge.svg)](https://github.com/xDDoubleTea/TetrisPP/actions/workflows/build-windows.yml)

## Gameplay

A 10×20 board with the standard guideline mechanics:

- **Super Rotation System** with wall kicks, including 180° rotation.
- **7-bag randomiser**, a 5-piece next queue, hold, and a ghost piece.
- **Spin detection** — T-spins and all-spins, both scored.
- **Garbage** — incoming attacks queue up, rise from the bottom with a single
  hole per line, and are cancelled by clearing lines.
- **Tower-defense layer** — a shooter and advancing enemies share the field;
  line clears deal damage and enemies send garbage back.
- **Damage** from line clears, scaled by combo, back-to-back, spins and perfect
  clears.
- **DAS/ARR** horizontal movement and a lock timer that resets on rotation or
  movement.

Live stats are drawn under the board: lines cleared, pieces placed, T-spins,
all-spins, perfect clears, back-to-back and combo counts, attacks sent and
received, and APM / PPS / APP.

### Controls

| Key                       | Action                   |
| ------------------------- | ------------------------ |
| <kbd>←</kbd> <kbd>→</kbd> | Move                     |
| <kbd>↑</kbd>              | Rotate clockwise         |
| <kbd>Z</kbd>              | Rotate counter-clockwise |
| <kbd>A</kbd>              | Rotate 180°              |
| <kbd>↓</kbd>              | Soft drop                |
| <kbd>Space</kbd>          | Hard drop                |
| <kbd>C</kbd>              | Hold                     |
| <kbd>Enter</kbd>          | Start                    |
| <kbd>P</kbd>              | Pause                    |

## Build

Install the dependencies, then build:

```sh
./scripts/install-deps.sh   # detects pacman / apt / dnf / zypper / brew
make
./game
```

To install them yourself, you need a C++17 compiler, `make`, `pkg-config` and
Allegro 5 with the image, ttf, font, primitives, audio, acodec and color addons:

| Platform      | Command                                                                                                                                              |
| ------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| Arch          | `sudo pacman -S --needed base-devel pkgconf allegro`                                                                                                 |
| Debian/Ubuntu | `sudo apt install build-essential pkg-config liballegro5-dev liballegro-image5-dev liballegro-ttf5-dev liballegro-audio5-dev liballegro-acodec5-dev` |
| Fedora        | `sudo dnf install gcc-c++ make pkgconf-pkg-config allegro5-devel allegro5-addon-{image,ttf,audio,acodec}-devel`                                      |
| macOS         | `brew install allegro pkg-config`                                                                                                                    |
| Windows       | via [MSYS2](https://www.msys2.org/): `pacman -S make mingw-w64-ucrt-x86_64-{gcc,pkgconf,allegro}`                                                    |

Any Allegro 5.2.x works. If it is installed under a non-standard prefix, point
`pkg-config` at it:

```sh
make PKG_CONFIG_PATH=/your/prefix/lib/pkgconfig
```

**Run the game from the repository root** — assets are resolved relative to the
working directory (`./assets/...`).

### Assets

The repository ships only fonts (Inter and JetBrains Mono, both SIL OFL 1.1).
Backgrounds, sprites, music and sound effects are not included, and the game
runs without them: missing images become a checkerboard placeholder, missing
animations are skipped, and missing audio plays silently. See
[`assets/README.md`](assets/README.md) for the paths to drop your own files
into.

### Make targets

| Target                  | Effect                                                       |
| ----------------------- | ------------------------------------------------------------ |
| `make`                  | Optimised build (`-O2`) → `./game`                           |
| `make debug`            | Debug build (`-g -O0 -DDEBUG`) → `./game`                    |
| `make run`              | Build, then launch                                           |
| `make clean`            | Remove `build/` and the binary                               |
| `make compile_commands` | Regenerate `compile_commands.json` for clangd (needs `bear`) |

Objects are written to `build/release` and `build/debug`, so the two
configurations can coexist. `make -j$(nproc)` builds in parallel.

`./game --test` initialises the game without opening a window and exits, which
is what CI uses as a smoke test.

## Layout

| Path                    | Contents                                                     |
| ----------------------- | ------------------------------------------------------------ |
| `tetris/`               | Board, tetrimino logic, SRS tables, statistics               |
| `data/`                 | Asset caches (image, font, GIF, sound) and shared game state |
| `shapes/`               | Point, circle and rectangle primitives with collision tests  |
| `peashooter/`, `enemy/` | Plants-vs-Zombies entities                                   |
| `algif5/`               | Vendored GIF decoder for Allegro                             |
| `assets/`               | Sprites, GIFs, fonts, music and sound effects                |
| `scripts/`              | Dependency installer                                         |

## Resources

- Allegro downloads: <https://github.com/liballeg/allegro5/releases>
- Allegro documentation: <https://liballeg.org/a5docs/trunk/>
- GIF convert: <https://ezgif.com/video-to-gif>

## Road Map

- [ ] Increase difficulty as time goes on
- [ ] Enable users to define their control settings with precise numbers in frames.
- [ ] Add more enemy types
- [ ] Boss fights
- [x] More music
- [x] Load assets at the start of the game instead of after starting the game

## Licence

MIT, see [LICENSE](LICENSE). The bundled fonts are under the SIL Open Font
License 1.1.
