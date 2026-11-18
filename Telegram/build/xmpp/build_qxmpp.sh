#!/bin/bash
# Builds the QXmpp submodule as a static library against the static Qt 6
# from the centos_env image and installs it into the given prefix.
# Usage: build_qxmpp.sh <install-prefix>
set -euo pipefail

Prefix="${1:?usage: build_qxmpp.sh <install-prefix>}"
ScriptPath="$(cd "$(dirname "$0")" && pwd)"
Source="$ScriptPath/../../ThirdParty/qxmpp"
Build="$ScriptPath/../../../out-qxmpp"

if [ ! -f "$Source/CMakeLists.txt" ]; then
  echo "QXmpp sources not found in $Source (run: git submodule update --init Telegram/ThirdParty/qxmpp)"
  exit 1
fi

if [ -f "$Prefix/lib64/cmake/QXmppQt6/QXmppQt6Config.cmake" ] \
  || [ -f "$Prefix/lib/cmake/QXmppQt6/QXmppQt6Config.cmake" ]; then
  echo "QXmpp already installed in $Prefix"
  exit 0
fi

cmake -S "$Source" -B "$Build" \
  -D CMAKE_BUILD_TYPE=Release \
  -D CMAKE_INSTALL_PREFIX="$Prefix" \
  -D CMAKE_PREFIX_PATH=/usr/local \
  -D CMAKE_POSITION_INDEPENDENT_CODE=ON \
  -D BUILD_SHARED=OFF \
  -D BUILD_TESTING=OFF \
  -D BUILD_DOCUMENTATION=OFF \
  -D BUILD_EXAMPLES=OFF \
  -D BUILD_OMEMO=OFF \
  -D WITH_GSTREAMER=OFF \
  -D WITH_ENCRYPTION=OFF

cmake --build "$Build" --parallel "$(nproc)"
cmake --install "$Build"
