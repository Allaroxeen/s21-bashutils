#!/bin/bash

i = 0
DIFF_RES=""
TEST_FILE="bytes.txt"
LOG="log.txt"
ORIG="cat.txt"
CUSTOM="s21_cat.txt"
TEST_STATUS=""
echo "===Start testing===" > $LOG
./s21_cat $TEST_FILE > $CUSTOM
cat $TEST_FILE > $ORIG
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
for var in "-b" "-n" "-s" "-T" "-e" "-E" "-v" "-t"
  do
    TEST1="$var $TEST_FILE"
    ./s21_cat $TEST1 > $CUSTOM
    cat $TEST1 > $ORIG
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
for var1 in "-b" "-n" "-s" "-T" "-e" "-E" "-v" "-t"
  do
    for var2 in "-b" "-n" "-s" "-T" "-e" "-E" "-v" "-t"
    do
    if [ $var1 != $var2 ]
      then
      TEST1="$var1 $var2 $TEST_FILE"
      ./s21_cat $TEST1 > $CUSTOM
      cat $TEST1 > $ORIG
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
echo "SUCCESS: $i/65"
