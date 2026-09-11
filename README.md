<div align="center">

<img src="img/icons/xtox.svg" alt="xTox logo" width="128" />

# xTox

**A free, encrypted, peer-to-peer instant messenger with chat, voice, video and
file transfer, built on the [Tox](https://tox.chat) protocol.**

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3%2B-blue.svg)](LICENSE)
[![Latest release](https://img.shields.io/github/v/release/xTox/xTox?include_prereleases&sort=semver)](https://github.com/xTox/xTox/releases/latest)
[![Build status](https://github.com/xTox/xTox/actions/workflows/build-test-deploy.yaml/badge.svg)](https://github.com/xTox/xTox/actions/workflows/build-test-deploy.yaml)
[![Issues](https://img.shields.io/github/issues/xTox/xTox)](https://github.com/xTox/xTox/issues)
[![Platforms](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows%20%7C%20FreeBSD-informational)](#installation)
[![Qt 5.15](https://img.shields.io/badge/Qt-5.15-brightgreen)](https://www.qt.io)

</div>

---

## Table of contents

- [About](#about)
- [Features](#features)
- [Screenshots](#screenshots)
- [Installation](#installation)
- [Building from source](#building-from-source)
- [Contributing](#contributing)
- [Translations](#translations)
- [Security](#security)
- [License](#license)
- [Acknowledgements](#acknowledgements)

---

## About

xTox is a Qt 5 based client for the [Tox](https://tox.chat) protocol — a
distributed, peer-to-peer network where messages, calls and files are exchanged
directly between users and are end-to-end encrypted. There is no central server
that stores your conversations.

It is a community fork of [qTox](https://github.com/qTox/qTox). See
[Acknowledgements](#acknowledgements) for details; the original GPLv3 licensing
and upstream attribution are preserved (see [`NOTICE`](NOTICE)).

## Features

- One-to-one chats and group chats
- End-to-end encrypted messaging
- Audio calls, including group calls
- Video calls and video device selection
- File transfers with inline previews
- Avatars and identicons
- Message history with search
- Screenshots with an integrated region selector
- Emoticons and text styling (markdown-like)
- Faux offline messaging
- Tox URI (`tox:`) and `tox://` handling; ToxMe support
- Portable profiles and profile encryption
- Translations in 30+ languages
- Automatic update checks (Windows and packages on Linux)

## Screenshots

> Screenshots may lag behind the current UI, but they give a good idea of the
> general look and feel.

![Screenshot: chat](https://i.imgur.com/olb89CN.png)
![Screenshot: group chat](https://i.imgur.com/tmX8z9s.png)

## Installation

Prebuilt installers and packages, when available, are published on the
[releases page](https://github.com/xTox/xTox/releases).

| Platform | Options |
|----------|---------|
| Linux    | Distribution packages, AppImage, Flatpak, or [build from source](#building-from-source) |
| Windows  | 64-bit installer (and nightly builds) |
| macOS    | `.dmg` bundle (and nightly builds) |
| FreeBSD  | Ports/packages |

Builds that are not installed through a package manager do not update
automatically — check the releases page periodically.

## Building from source

Full, per-platform instructions (dependencies, CMake options, packaging) live in
[`INSTALL.md`](INSTALL.md).

The short version on Linux/macOS:

```bash
git clone --recurse-submodules https://github.com/xTox/xTox.git
cd xTox
mkdir -p _build && cd _build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j"$(nproc)"
```

Dependencies: a C++11 compiler, CMake ≥ 3.7.2, Qt 5.15, c-toxcore, OpenAL or
OpenAL Soft, FFmpeg/libav, libsodium, Opus, libexif, libqrencode, SQLCipher and
KDE Frameworks (Sonnet, depending on configuration).

## Contributing

Contributions are welcome — bug reports, translations, documentation, design and
code. Please read [`CONTRIBUTING.md`](CONTRIBUTING.md) before opening a pull
request.

- **Bugs / features:** open an [issue](https://github.com/xTox/xTox/issues).
- **Pull requests:** fork the repository, create a topic branch, and follow the
  commit message format checked by `verify-commit-messages.sh`.
- **Code style:** see [`doc/coding_standards.md`](doc/coding_standards.md) and
  run `tools/format-code.sh`.

All participants are expected to follow our
[Code of Conduct](CODE_OF_CONDUCT.md).

## Translations

xTox is translated by the community. See
[`translations/README.md`](translations/README.md) for how to help.

## Security

Please **do not** report security vulnerabilities in public issues. See
[`SECURITY.md`](SECURITY.md) for the private disclosure process.

## License

xTox is free software, licensed under the **GNU General Public License v3.0 or
later** ([`LICENSE`](LICENSE)). You may use, study, share and modify it under the
terms of that license.

## Acknowledgements

xTox is a fork of [qTox](https://github.com/qTox/qTox) and is built on the work
of the qTox and [Tox](https://github.com/TokTok/c-toxcore) communities. The
original copyright notices and the GPLv3 license are retained; see
[`NOTICE`](NOTICE) for the full attribution. Thanks to the upstream maintainers
and all contributors.
