#!/usr/bin/env bash
set -euo pipefail

TRANSLATOR="${1:-}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"

if [[ -z "$TRANSLATOR" ]]; then
  echo "Ошибка: укажите путь до транслятора."
  echo "Использование: $0 <путь_до_транслятора>"
  exit 1
fi

if [[ ! -x "$TRANSLATOR" ]]; then
  echo "Ошибка: '$TRANSLATOR' не найден или не является исполняемым файлом."
  exit 1
fi

VALID_DIR="${SCRIPT_DIR}/valid"
INVALID_DIR="${SCRIPT_DIR}/invalid"

passed=0
failed=0
total=0

run_test() {
  local file="$1"
  local expect_success="$2"
  local label
  label="$(basename "$file")"
  total=$((total + 1))

  if "$TRANSLATOR" "$file" > /dev/null 2>&1; then
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
  valid_files=("$VALID_DIR"/*.*)
  if [[ "${#valid_files[@]}" -gt 0 && -f "${valid_files[0]}" ]]; then
    echo "Положительные примеры (valid/):"
    for f in "${valid_files[@]}"; do
      run_test "$f" 1
    done
  fi
fi

echo ""

if [[ -d "$INVALID_DIR" ]]; then
  invalid_files=("$INVALID_DIR"/*.*)
  if [[ "${#invalid_files[@]}" -gt 0 && -f "${invalid_files[0]}" ]]; then
    echo "Негативные примеры (invalid/):"
    for f in "${invalid_files[@]}"; do
      run_test "$f" 0
    done
  fi
fi

echo ""
echo "──────────────────────────────────────────"
echo "Итог: $passed прошло / $failed упало / $total всего"
echo ""

if [[ "$failed" -gt 0 ]]; then
  exit 1
fi
