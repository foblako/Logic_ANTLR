#!/usr/bin/env bash
set -euo pipefail

TRANSLATOR="${1:-}"

if [[ -z "$TRANSLATOR" ]]; then
  echo "Ошибка: укажите путь до транслятора."
  echo "Использование: $0 <путь_до_транслятора>"
  exit 1
fi

if [[ ! -x "$TRANSLATOR" ]]; then
  echo "Ошибка: '$TRANSLATOR' не найден или не является исполняемым файлом."
  exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
VALID_DIR="$SCRIPT_DIR/valid"
INVALID_DIR="$SCRIPT_DIR/invalid"

passed=0
failed=0
total=0

run_test() {
  local file="$1"
  local expect_success="$2"
  local label
  label="$(basename "$file")"
  total=$((total + 1))

  # Запускаем через cmd.exe для поддержки Windows .exe
  if cmd.exe /c "$TRANSLATOR" "$(wslpath -w "$file" 2>/dev/null || echo "$file")" > /dev/null 2>&1; then
    actual_ok=1
  else
    actual_ok=0
  fi

  if [[ "$expect_success" -eq 1 && "$actual_ok" -eq 1 ]]; then
    echo "  PASS  valid/${label}"
    passed=$((passed + 1))
  elif [[ "$expect_success" -eq 0 && "$actual_ok" -eq 0 ]]; then
    echo "  PASS  invalid/${label}"
    passed=$((passed + 1))
  elif [[ "$expect_success" -eq 1 && "$actual_ok" -eq 0 ]]; then
    echo "  FAIL  valid/${label}"
    failed=$((failed + 1))
  else
    echo "  FAIL  invalid/${label}"
    failed=$((failed + 1))
  fi
}

echo ""
echo "Транслятор: ${TRANSLATOR}"
echo "──────────────────────────────────────────"

if [[ -d "$VALID_DIR" ]]; then
  echo "Положительные примеры (valid/):"
  for f in "$VALID_DIR"/*; do
    [[ -f "$f" ]] && run_test "$f" 1
  done
fi

echo ""

if [[ -d "$INVALID_DIR" ]]; then
  echo "Негативные примеры (invalid/):"
  for f in "$INVALID_DIR"/*; do
    [[ -f "$f" ]] && run_test "$f" 0
  done
fi

echo ""
echo "──────────────────────────────────────────"
echo "Итог: $passed прошло / $failed упало / $total всего"
echo ""

if [[ "$failed" -gt 0 ]]; then
  exit 1
fi
