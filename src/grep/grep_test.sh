#!/bin/bash

i = 0
DIFF_RES=""
TEST_FILE="ex.txt ex1.txt"
EXP="tw"
LOG="log.txt"
ORIG="grep.txt"
CUSTOM="s21_grep.txt"
TEST_STATUS=""
echo "===Start testing===" > $LOG
./s21_grep $EXP $TEST_FILE > $CUSTOM
grep $EXP $TEST_FILE > $ORIG
DIFF_RES="$(diff -s $CUSTOM $ORIG)"
if cmp -s $CUSTOM $ORIG
  then
    TEST_STATUS="PASSED"
    echo "Test without flags $TEST_STATUS"
    i=$((i+1))
  else
    TEST_STATUS="FAILED"
    echo "Test without flags $TEST_STATUS"
    echo "Failed noflags test" >> $LOG
fi
./s21_grep -e tw -e 12 $TEST_FILE > $CUSTOM
grep -e tw -e 12 $TEST_FILE > $ORIG
DIFF_RES="$(diff -s $CUSTOM $ORIG)"
if cmp -s $CUSTOM $ORIG
    then
      TEST_STATUS="PASSED"
      echo "Test double -e flag $TEST_STATUS"
      i=$((i+1))
    else
      TEST_STATUS="FAILED"
      echo "Test double -e flag $TEST_STATUS"
      echo "Failed enoflags test" >> $LOG
fi
for var in "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-o" "-e" "-f"
  do
    if [ $var = "-f" ]
      then
        TEST1="$var pattern.txt $TEST_FILE"
      else
        TEST1="$var $EXP $TEST_FILE"
    fi
    ./s21_grep $TEST1 > $CUSTOM
    grep $TEST1 > $ORIG
    DIFF_RES="$(diff -s $CUSTOM $ORIG)"
    if cmp -s $CUSTOM $ORIG
      then
        TEST_STATUS="PASSED"
        echo "Test $TEST1 $TEST_STATUS"
        i=$((i+1))
      else
        TEST_STATUS="FAILED"
        echo "Test $TEST1 $TEST_STATUS"
        echo "Failed $TEST1 test" >> $LOG
    fi
    rm $CUSTOM $ORIG
  done
for var1 in "-i" "-v" "-n" "-h" "-s" "-o" "-l" "-c"
  do
    for var2 in "-i" "-v" "-n" "-h" "-s" "-o" "-e" "-f" "-c" "-l"
    do
    if [ $var1 != $var2 ]
      then
      if [ $var2 = "-f" ]
        then
          TEST1="$var1 $var2 pattern.txt $TEST_FILE"
        else
          TEST1="$var1 $var2 $EXP $TEST_FILE"
      fi
      ./s21_grep $TEST1 > $CUSTOM
      grep $TEST1 > $ORIG
      if cmp -s $CUSTOM $ORIG
        then
          TEST_STATUS="PASSED"
          echo "Test $TEST1 $TEST_STATUS"
          i=$((i+1))
        else
          TEST_STATUS="FAILED"
          echo "Test $TEST1 $TEST_STATUS"
          echo "Failed $TEST1 test" >> $LOG
      fi
      rm $CUSTOM $ORIG
    fi
  done
done
echo "SUCCESS: $i/84"
