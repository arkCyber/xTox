#!/bin/bash
#
# Build, deploy and run xTox on macOS (Apple Silicon / Homebrew Qt5).
#
# This wraps the manual steps required on modern macOS where:
#   * the linker's ad-hoc signature becomes invalid as soon as macdeployqt /
#     macfixrpath modify the bundle, so the bundle has to be re-signed;
#   * Homebrew's toxcore was built against an older libvpx (libvpx.7);
#   * macdeployqt does not bundle libsharpyuv/libjxl_cms/liblcms2.
#
# Usage: ./build_and_run_macos.sh [--no-run]
#
set -eu -o pipefail

QT_PREFIX="$(brew --prefix qt@5)"
BUILD_DIR="$(cd "$(dirname "$0")" && pwd)/_build"
APP="$BUILD_DIR/xtox.app"
FW="$APP/Contents/Frameworks"
EXE='@executable_path/../Frameworks'

RUN=1
[ "${1:-}" = "--no-run" ] && RUN=0

################################################################################
# 0. Prerequisite: make sure libvpx.7 is resolvable (Homebrew toxcore links it)
################################################################################
VPX_LIBDIR="$(brew --prefix libvpx)/lib"
if [ ! -e "$VPX_LIBDIR/libvpx.7.dylib" ]; then
    echo ">> libvpx.7.dylib is missing; linking a compatible copy"
    # Prefer a copy bundled with Homebrew's OpenCV (a genuine libvpx 7 build)
    VPX7="$(find /opt/homebrew/lib -maxdepth 6 -name 'libvpx.7.dylib' 2>/dev/null | head -n1)"
    if [ -z "$VPX7" ]; then
        echo "!! Could not find libvpx.7.dylib. Rebuild toxcore against the current" >&2
        echo "!! libvpx, or provide libvpx.7 manually." >&2
        exit 1
    fi
    ln -sf "$VPX7" "$VPX_LIBDIR/libvpx.7.dylib"
fi

TOXCORE_LIBDIR="$(brew --prefix toxcore)/lib"
if ! codesign --verify "$TOXCORE_LIBDIR/libtoxcore.2.dylib" >/dev/null 2>&1; then
    echo ">> Re-signing Homebrew libtoxcore (its signature was invalidated)"
    TOXCORE_REAL="$(python3 -c 'import os,sys;print(os.path.realpath(sys.argv[1]))' \
        "$TOXCORE_LIBDIR/libtoxcore.2.dylib")"
    chmod u+w "$TOXCORE_REAL"
    codesign --force --sign - "$TOXCORE_REAL"
fi

################################################################################
# 1. Configure + build
################################################################################
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
if [ ! -f CMakeCache.txt ]; then
    cmake .. -DCMAKE_PREFIX_PATH="$QT_PREFIX;$(brew --prefix opus)"
fi
cmake --build . -- -j"$(sysctl -n hw.ncpu)"

################################################################################
# 2. Deploy bundle (macdeployqt + macfixrpath) and create the dmg
################################################################################
make install

################################################################################
# 3. Patch libraries macdeployqt failed to bundle
################################################################################
copy_and_fix() { # <source> <dest-name>
    local src="$1" name="$2"
    [ -e "$FW/$name" ] || cp -f "$src" "$FW/$name"
    chmod u+w "$FW/$name"
    install_name_tool -id "$EXE/$name" "$FW/$name" 2>/dev/null || true
}

copy_and_fix "$(brew --prefix webp)/lib/libsharpyuv.0.dylib" libsharpyuv.0.dylib
copy_and_fix "$(brew --prefix jpeg-xl)/lib/libjxl_cms.0.11.dylib" libjxl_cms.0.11.dylib
copy_and_fix "$(brew --prefix little-cms2)/lib/liblcms2.2.dylib" liblcms2.2.dylib

install_name_tool -change "$(brew --prefix little-cms2)/lib/liblcms2.2.dylib" "$EXE/liblcms2.2.dylib" "$FW/libjxl_cms.0.11.dylib" 2>/dev/null || true
install_name_tool -change "$(brew --prefix highway)/lib/libhwy.1.dylib" "$EXE/libhwy.1.dylib" "$FW/libjxl_cms.0.11.dylib" 2>/dev/null || true

for lib in libwebp.7.dylib libwebpmux.3.dylib libwebpdemux.2.dylib; do
    install_name_tool -change @rpath/libsharpyuv.0.dylib "$EXE/libsharpyuv.0.dylib" "$FW/$lib" 2>/dev/null || true
    install_name_tool -change @rpath/libwebp.7.dylib "$EXE/libwebp.7.dylib" "$FW/$lib" 2>/dev/null || true
done
install_name_tool -change @rpath/libjxl_cms.0.11.dylib "$EXE/libjxl_cms.0.11.dylib" "$FW/libjxl.0.11.dylib" 2>/dev/null || true

################################################################################
# 4. Re-sign the bundle (macdeployqt/macfixrpath invalidated the signature)
################################################################################
codesign --force --deep --sign - "$APP"
codesign --verify --deep --strict "$APP" && echo ">> Bundle signature is valid"

echo ">> Built: $APP"
echo ">> Dmg:   $BUILD_DIR/xTox.dmg"

################################################################################
# 5. Run
################################################################################
if [ "$RUN" = "1" ]; then
    echo ">> Launching xTox..."
    open "$APP"
fi
