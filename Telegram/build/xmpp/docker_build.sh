#!/bin/bash
# Runs inside the centos_env image: builds QXmpp into out-qxmpp-install,
# then runs the regular tdesktop build with that prefix on CMAKE_PREFIX_PATH.
# Takes the same arguments as Telegram/build/docker/centos_env/build.sh.
set -euo pipefail

ScriptPath="$(cd "$(dirname "$0")" && pwd)"
RepoPath="$(cd "$ScriptPath/../../.." && pwd)"
Prefix="$RepoPath/out-qxmpp-install"

"$ScriptPath/build_qxmpp.sh" "$Prefix"

# The image puts its own libraries first in LD_LIBRARY_PATH, which makes the
# system Python load an older libexpat and fail to import pyexpat
# (undefined symbol XML_SetBillionLaughsAttackProtectionMaximumAmplification).
# That breaks the build-time Python scripts, e.g. the AppStream changelog
# generator; upstream CI fails the same way. Run Python with the system
# library path only.
PythonWrapper="$RepoPath/out-qxmpp/python3"
mkdir -p "$(dirname "$PythonWrapper")"
SystemPython="$(command -v python3.12 || command -v python3)"
cat > "$PythonWrapper" <<EOF
#!/bin/bash
exec env -u LD_LIBRARY_PATH "$SystemPython" "\$@"
EOF
chmod +x "$PythonWrapper"

# Report every failing file in one run (see build.sh).
export KEEP_GOING="${KEEP_GOING:-1}"
"$RepoPath/Telegram/build/docker/centos_env/build.sh" \
  -D CMAKE_PREFIX_PATH="$Prefix" \
  -D Python3_EXECUTABLE="$PythonWrapper" \
  "$@"
