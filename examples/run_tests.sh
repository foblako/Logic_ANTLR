#!/usr/bin/env bash
# run_tests.sh — прогоняет все примеры через транслятор и проверяет результат.
# Использование: ./examples/run_tests.sh <путь_до_транслятора>
# Пример:        ./examples/run_tests.sh ./build/main

set -euo pipefail

TRANSLATOR="${1:-}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"

# ── цвета ────────────────────────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# ── проверка аргумента ───────────────────────────────────────────────────────
if [[ -z "$TRANSLATOR" ]]; then
  echo -e "${RED}Ошибка: укажите путь до транслятора.${NC}"
  echo "Использование: $0 <путь_до_транслятора>"
  echo "Пример:        $0 ./build/main"
  exit 1
fi

if [[ ! -x "$TRANSLATOR" ]]; then
  echo -e "${RED}Ошибка: '${TRANSLATOR}' не найден или не является исполняемым файлом.${NC}"
  exit 1
fi

VALID_DIR="${SCRIPT_DIR}/valid"
INVALID_DIR="${SCRIPT_DIR}/invalid"

passed=0
failed=0
total=0

run_test() {
  local file="$1"
  local expect_success="$2"   # 1 = ожидаем успех (код 0), 0 = ожидаем ошибку (код != 0)
  local label
  label="$(basename "$file")"

  total=$((total + 1))

  # Запускаем транслятор, подавляем вывод
  if "$TRANSLATOR" "$file" > /dev/null 2>&1; then
    actual_ok=1
  else
    actual_ok=0
  fi

  if [[ "$expect_success" -eq 1 && "$actual_ok" -eq 1 ]]; then
    echo -e "  ${GREEN}PASS${NC}  valid/${label}"
    passed=$((passed + 1))
  elif [[ "$expect_success" -eq 0 && "$actual_ok" -eq 0 ]]; then
    echo -e "  ${GREEN}PASS${NC}  invalid/${label}"
    passed=$((passed + 1))
  elif [[ "$expect_success" -eq 1 && "$actual_ok" -eq 0 ]]; then
    echo -e "  ${RED}FAIL${NC}  valid/${label}  <- транслятор вернул ошибку, ожидался успех"
    failed=$((failed + 1))
  else
    echo -e "  ${RED}FAIL${NC}  invalid/${label}  <- транслятор завершился успешно, ожидалась ошибка"
    failed=$((failed + 1))
  fi
}

# ── положительные примеры ────────────────────────────────────────────────────
echo ""
echo "Транслятор: ${TRANSLATOR}"
echo "──────────────────────────────────────────"

if [[ -d "$VALID_DIR" ]]; then
  valid_files=("$VALID_DIR"/*.*)
  if [[ "${#valid_files[@]}" -gt 0 && -f "${valid_files[0]}" ]]; then
    echo -e "${YELLOW}Положительные примеры (valid/):${NC}"
    for f in "${valid_files[@]}"; do
      run_test "$f" 1
    done
  else
    echo -e "${YELLOW}valid/${NC}: файлы не найдены, пропускаем."
  fi
else
  echo -e "${YELLOW}Папка valid/ не найдена, пропускаем.${NC}"
fi

echo ""

# ── негативные примеры ───────────────────────────────────────────────────────
if [[ -d "$INVALID_DIR" ]]; then
  invalid_files=("$INVALID_DIR"/*.*)
  if [[ "${#invalid_files[@]}" -gt 0 && -f "${invalid_files[0]}" ]]; then
    echo -e "${YELLOW}Негативные примеры (invalid/):${NC}"
    for f in "${invalid_files[@]}"; do
      run_test "$f" 0
    done
  else
    echo -e "${YELLOW}invalid/${NC}: файлы не найдены, пропускаем."
  fi
else
  echo -e "${YELLOW}Папка invalid/ не найдена, пропускаем.${NC}"
fi

# ── итог ─────────────────────────────────────────────────────────────────────
echo ""
echo "──────────────────────────────────────────"
echo -e "Итог: ${GREEN}${passed} прошло${NC} / ${RED}${failed} упало${NC} / ${total} всего"
echo ""

if [[ "$failed" -gt 0 ]]; then
  exit 1
fi
