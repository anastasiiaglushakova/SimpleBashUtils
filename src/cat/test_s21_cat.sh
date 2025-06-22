#!/bin/bash

SYS_CAT=cat
S21_CAT=./s21_cat
TEST_FILE="test.txt"

CAT_COMMANDS=(
  "-b $TEST_FILE"
  "-e $TEST_FILE"
  "-n $TEST_FILE"
  "-s $TEST_FILE"
  "-t $TEST_FILE"
  "--number-nonblank $TEST_FILE"
  "--number $TEST_FILE"
  "--squeeze-blank $TEST_FILE"
  "-E $TEST_FILE"
  "-T $TEST_FILE"
  "-b -e $TEST_FILE"
  "-n -e $TEST_FILE"
  "-s -n $TEST_FILE"
  "-s -b $TEST_FILE"
  "-e -t $TEST_FILE"
  "--number --squeeze-blank $TEST_FILE"
  "--number-nonblank -E $TEST_FILE"
  "-n -E -T $TEST_FILE"
  "-b -e -s -t -n $TEST_FILE"
)

INVALID_FLAGS_LIST=(
  "-z"
  "--invalid"
  "-q"
  "-n --badflag"
)

mkdir -p test_outputs
success_count=0
fail_count=0

echo "--- ✅ Запуск тестов с корректными аргументами ---"
for cmd in "${CAT_COMMANDS[@]}"; do
  sys_out="test_outputs/sys_cat.txt"
  s21_out="test_outputs/s21_cat.txt"

  $SYS_CAT $cmd > "$sys_out" 2> /dev/null
  $S21_CAT $cmd > "$s21_out" 2> /dev/null

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

echo -e "\n--- 🧪 Проверка нечитаемого файла ---"
UNREADABLE_FILE="unreadable.txt"
touch "$UNREADABLE_FILE"
chmod 000 "$UNREADABLE_FILE"

$S21_CAT "$UNREADABLE_FILE" > /dev/null 2> test_outputs/s21_err_file.txt
if grep -qE "No such file|Permission denied" test_outputs/s21_err_file.txt; then
  echo "✅ PASS: ошибка на нечитаемый файл"
  ((success_count++))
else
  echo "❌ FAIL: ошибка на нечитаемый файл"
  cat test_outputs/s21_err_file.txt
  ((fail_count++))
fi
chmod 644 "$UNREADABLE_FILE"
rm "$UNREADABLE_FILE"

echo -e "\n--- 🧪 Проверка неверных флагов ---"
for flag in "${INVALID_FLAGS_LIST[@]}"; do
  $S21_CAT $flag "$TEST_FILE" > /dev/null 2> test_outputs/s21_err_flag.txt
  if grep -qiE "Error|invalid|illegal option|unrecognized option" test_outputs/s21_err_flag.txt; then
    echo "✅ PASS: неверный флаг '$flag'"
    ((success_count++))
  else
    echo "❌ FAIL: неверный флаг '$flag'"
    cat test_outputs/s21_err_flag.txt
    ((fail_count++))
  fi
done

echo -e "\n=========================="
echo "✅ Total passed: $success_count"
echo "❌ Total failed: $fail_count"

rm -rf test_outputs
