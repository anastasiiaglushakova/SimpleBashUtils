#!/bin/bash

SYS_GREP=grep
S21_GREP=./s21_grep
PATTERN="pattern"
TEST_FILE=test.txt

GREP_COMMANDS=(
  "-e $PATTERN $TEST_FILE"
  "-i $PATTERN $TEST_FILE"
  "-v $PATTERN $TEST_FILE"
  "-c $PATTERN $TEST_FILE"
  "-l $PATTERN $TEST_FILE"
  "-n $PATTERN $TEST_FILE"
  "-i -v $PATTERN $TEST_FILE"
  "-i -c $PATTERN $TEST_FILE"
  "-i -l $PATTERN $TEST_FILE"
  "-n -c $PATTERN $TEST_FILE"
  "-n -v $PATTERN $TEST_FILE"
  "-n -i -c -v $PATTERN $TEST_FILE"
)

INVALID_FLAGS_LIST=(
  "-x"
  "--wrong"
  "-z"
  "--unsupported"
)

mkdir -p test_outputs
success_count=0
fail_count=0

echo "--- ✅ Запуск тестов с корректными аргументами ---"
for cmd in "${GREP_COMMANDS[@]}"; do
  sys_out="test_outputs/sys_grep.txt"
  s21_out="test_outputs/s21_grep.txt"

  $SYS_GREP $cmd > "$sys_out" 2> /dev/null
  $S21_GREP $cmd > "$s21_out" 2> /dev/null

  if diff -u "$sys_out" "$s21_out" > /dev/null; then
    echo "✅ PASS: $cmd"
    ((success_count++))
  else
    echo "❌ FAIL: $cmd"
    echo "🔍 diff:"
    diff -u "$sys_out" "$s21_out" | head -n 10
    ((fail_count++))
  fi
done

echo -e "\n--- 🧪 Проверка несуществующего файла ---"
MISSING_FILE="missing.txt"
$S21_GREP -e "$PATTERN" "$MISSING_FILE" > /dev/null 2> test_outputs/s21_err_missing.txt
if grep -qE "No such file|cannot open|open failed" test_outputs/s21_err_missing.txt; then
  echo "✅ PASS: ошибка на несуществующий файл"
  ((success_count++))
else
  echo "❌ FAIL: ошибка на несуществующий файл"
  cat test_outputs/s21_err_missing.txt
  ((fail_count++))
fi

echo -e "\n--- 🧪 Проверка неверных флагов ---"
for flag in "${INVALID_FLAGS_LIST[@]}"; do
  $S21_GREP $flag "$TEST_FILE" > /dev/null 2> test_outputs/s21_err_flag.txt
  if grep -qE "Error|Unknown option|invalid option|unrecognized option" test_outputs/s21_err_flag.txt; then
    echo "✅ PASS: неверный флаг '$flag'"
    ((success_count++))
  else
    echo "❌ FAIL: неверный флаг '$flag'"
    cat test_outputs/s21_err_flag.txt
    ((fail_count++))
  fi
done

echo -e "\n--- 🧪 Проверка флага -e без шаблона ---"
$S21_GREP -e > /dev/null 2> test_outputs/s21_err_noarg.txt
if grep -qE "Error|missing pattern|invalid|option requires an argument" test_outputs/s21_err_noarg.txt; then
  echo "✅ PASS: ошибка при -e без шаблона"
  ((success_count++))
else
  echo "❌ FAIL: ошибка при -e без шаблона"
  cat test_outputs/s21_err_noarg.txt
  ((fail_count++))
fi

echo -e "\n=========================="
echo "✅ Total passed: $success_count"
echo "❌ Total failed: $fail_count"

rm -rf test_outputs