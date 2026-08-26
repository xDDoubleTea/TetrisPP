#!/usr/bin/env sh
# Install the build dependencies for TetrisPP.
#
# All that is needed is a C++17 compiler, make, pkg-config and Allegro 5 with
# the image/font/ttf/primitives/audio/acodec/color addons. Every supported
# platform ships a recent enough Allegro (5.2.x) in its package manager, so
# there is nothing to build from source.
set -eu

die() { echo "error: $*" >&2; exit 1; }

if command -v pacman >/dev/null 2>&1; then
    echo "==> Arch (pacman)"
    sudo pacman -S --needed --noconfirm base-devel pkgconf allegro

elif command -v apt-get >/dev/null 2>&1; then
    echo "==> Debian/Ubuntu (apt)"
    sudo apt-get update
    sudo apt-get install -y build-essential pkg-config \
        liballegro5-dev liballegro-image5-dev liballegro-ttf5-dev \
        liballegro-audio5-dev liballegro-acodec5-dev

elif command -v dnf >/dev/null 2>&1; then
    echo "==> Fedora (dnf)"
    sudo dnf install -y gcc-c++ make pkgconf-pkg-config \
        allegro5-devel allegro5-addon-image-devel allegro5-addon-ttf-devel \
        allegro5-addon-audio-devel allegro5-addon-acodec-devel

elif command -v zypper >/dev/null 2>&1; then
    echo "==> openSUSE (zypper)"
    sudo zypper install -y gcc-c++ make pkg-config allegro5-devel

elif command -v brew >/dev/null 2>&1; then
    echo "==> macOS (Homebrew)"
    brew install allegro pkg-config

else
    die "no supported package manager found (pacman/apt/dnf/zypper/brew).
Install Allegro 5 and pkg-config by hand, then run: make
On Windows, use MSYS2: pacman -S mingw-w64-ucrt-x86_64-{gcc,pkgconf,allegro}"
fi

echo
if pkg-config --exists allegro-5; then
    echo "Allegro $(pkg-config --modversion allegro-5) found. Now run: make"
else
    die "Allegro installed but pkg-config cannot see it.
If it lives under a custom prefix, set PKG_CONFIG_PATH to its lib/pkgconfig."
fi
