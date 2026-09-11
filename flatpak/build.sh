#!/usr/bin/env bash

# SPDX-License-Identifier: GPL-3.0+
#
#   Copyright © 2018-2021 by The xTox Project Contributors

# Fail out on error
set -exuo pipefail

# use multiple cores when building
export MAKEFLAGS="-j$(nproc)"
FLATPAK_DESCRIPTOR=$(dirname $(realpath $0))/io.github.xtox.xTox.json

mkdir -p /flatpak-build
cd /flatpak-build

# Build the xTox flatpak
flatpak-builder --disable-rofiles-fuse --install-deps-from=flathub --force-clean --repo=xtox-repo build "$FLATPAK_DESCRIPTOR"

# Create a bundle for distribution
flatpak build-bundle xtox-repo xtox.flatpak io.github.xtox.xTox

cp xtox.flatpak /xtox

