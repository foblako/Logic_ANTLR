#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="${PROJECT_DIR:-/workspace}"
BUILD_DIR="${BUILD_DIR:-${PROJECT_DIR}/build}"
BIN="${BIN:-${BUILD_DIR}/main}"
TEST_SCRIPT="${TEST_SCRIPT:-${PROJECT_DIR}/examples/run_tests.sh}"
MODE="${1:-test}"

build_translator() {
  echo "=== Сборка проекта ==="
  cmake -S "$PROJECT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
  cmake --build "$BUILD_DIR" --parallel "$(nproc)"
  echo ""
  echo "=== Сборка завершена ==="
  echo ""
}

run_tests_script() {
  if [[ ! -f "$TEST_SCRIPT" ]]; then
    echo "Test script not found: $TEST_SCRIPT"
    exit 1
  fi

  # Windows-safe: strip CRLF in-place before execution
  sed -i 's/\r$//' "$TEST_SCRIPT"
  bash "$TEST_SCRIPT" "$BIN"
}

if [[ "$#" -gt 0 ]]; then
  shift
fi

case "$MODE" in
  bash|sh)
    exec /bin/bash "$@"
    ;;
  run)
    if [[ "$#" -lt 1 ]]; then
      echo "Usage: run <path-to-input-file>"
      echo "Example: run /workspace/examples/valid/08_complex.example"
      exit 1
    fi
    build_translator
    exec "$BIN" "$1"
    ;;
  test)
    build_translator
    run_tests_script
    ;;
  *)
    build_translator
    exec "$BIN" "$MODE" "$@"
    ;;
esac
