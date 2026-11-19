#!/bin/bash
# Runs inside the centos_env image: builds QXmpp into out-qxmpp-install,
# then runs the regular tdesktop build with that prefix on CMAKE_PREFIX_PATH.
# Takes the same arguments as Telegram/build/docker/centos_env/build.sh.
set -euo pipefail

ScriptPath="$(cd "$(dirname "$0")" && pwd)"
RepoPath="$(cd "$ScriptPath/../../.." && pwd)"
Prefix="$RepoPath/out-qxmpp-install"

"$ScriptPath/build_qxmpp.sh" "$Prefix"

# Report every failing file in one run (see build.sh).
export KEEP_GOING="${KEEP_GOING:-1}"
"$RepoPath/Telegram/build/docker/centos_env/build.sh" \
  -D CMAKE_PREFIX_PATH="$Prefix" \
  "$@"
