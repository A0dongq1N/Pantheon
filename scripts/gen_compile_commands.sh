#!/usr/bin/env bash
set -euo pipefail

# 生成/更新所有子项目的 compile_commands.json
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

for SUBDIR in "$ROOT_DIR"/*; do
  [[ -d "$SUBDIR" ]] || continue
  [[ -f "$SUBDIR/CMakeLists.txt" ]] || { echo "skip (no CMakeLists.txt): $SUBDIR"; continue; }
  (
    set -e
    cd "$SUBDIR"
    if cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null; then
      ln -sf build/compile_commands.json ./compile_commands.json
      echo "updated: $SUBDIR/compile_commands.json"
    else
      echo "failed: $SUBDIR" >&2
    fi
  ) || true
done

echo "done."


