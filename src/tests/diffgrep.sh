filename1=diffgrep.sh;
filename2=diffcat.sh;
filename3="../test.txt";
echo ----------------------------------------------*GREP 1
diff <(grep int "$filename2") <(../grep/s21_grep int "$filename2") -s -q
echo ----------------------------------------------*GREP 1 -e
diff <(grep -e int "$filename2") <(../grep/s21_grep -e int "$filename2") -s -q
echo ----------------------------------------------*GREP 2
diff <(grep -i int "$filename1") <(../grep/s21_grep -i int "$filename1") -s -q
echo ----------------------------------------------*GREP 3
diff <(grep -v int "$filename1") <(../grep/s21_grep -v int "$filename1") -s -q
echo ----------------------------------------------*GREP 4
diff <(grep -c int "$filename1") <(../grep/s21_grep -c int "$filename1") -s -q
echo ----------------------------------------------*GREP 5
diff <(grep -l int "$filename2" "$filename3" "$filename1") <(../grep/s21_grep -l int "$filename2" "$filename3" "$filename1") -s -q
echo ----------------------------------------------*GREP 6
diff <(grep -n int "$filename1") <(../grep/s21_grep -n int "$filename1") -s -q
