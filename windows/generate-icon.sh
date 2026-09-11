#!/bin/bash

#   Copyright © 2017-2019 The xTox Project Contributors
#
#   This file is part of xTox, a Qt-based graphical interface for Tox.
#   xTox is libre software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   xTox is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with xTox.  If not, see <http://www.gnu.org/licenses/>

# Generates xtox.ico from xtox.svg
#
# Dependencies:
#   base64 8.25
#   ImageMagick 7.0.4
#   icoutils 0.31.0

# Black, gray and transparent colors from Windows 16-color palette
base64 -d <<< R0lGODlhBAABAPEAAAAAAAAAAICAgMDAwCH5BAEAAAAALAAAAAAEAAEAAAIDjAYFADs= > pal.gif
# Generate 32 bpp images
convert -background none -depth 8 xtox.svg xtox_256_256_32.png
convert -resize 64x64 xtox_256_256_32.png xtox_64_64_32.png
convert -resize 48x48 xtox_256_256_32.png xtox_48_48_32.png
convert -resize 32x32 xtox_256_256_32.png xtox_32_32_32.png
convert -resize 24x24 xtox_256_256_32.png xtox_24_24_32.png
convert -resize 16x16 xtox_256_256_32.png xtox_16_16_32.png
# Generate 8 bpp images
convert +dither xtox_48_48_32.png png8:xtox_48_48_8.png
convert +dither xtox_32_32_32.png png8:xtox_32_32_8.png
convert +dither xtox_16_16_32.png png8:xtox_16_16_8.png
# Generate 1 bpp images
convert +dither -remap pal.gif xtox_32_32_8.png png8:xtox_32_32_1.png
convert +dither -remap pal.gif xtox_16_16_8.png png8:xtox_16_16_1.png
# Hack for Windows XP file properties page
convert -modulate 99.5 -strip xtox_256_256_32.png xtox_256_256_32.png
convert -modulate 99.5 xtox_64_64_32.png xtox_64_64_32.png
convert -modulate 99.5 xtox_48_48_32.png xtox_48_48_32.png
convert -modulate 99.5 xtox_32_32_32.png xtox_32_32_32.png
convert -modulate 99.5 xtox_24_24_32.png xtox_24_24_32.png
convert -modulate 99.5 xtox_16_16_32.png xtox_16_16_32.png
# Build .ico file from .png images
icotool -c -t 32 \
  xtox_32_32_1.png \
  xtox_16_16_1.png \
  xtox_48_48_8.png \
  xtox_32_32_8.png \
  xtox_16_16_8.png \
  --raw=xtox_256_256_32.png \
  xtox_64_64_32.png \
  xtox_48_48_32.png \
  xtox_32_32_32.png \
  xtox_24_24_32.png \
  xtox_16_16_32.png \
  > xtox.ico
# Show debug information
icotool -l xtox.ico
