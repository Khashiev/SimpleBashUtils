echo ----------------------------------------------*CAT 1
diff <(cat -b ../test.txt) <(../cat/s21_cat -b ../test.txt) -s
echo ----------------------------------------------*CAT 2
diff <(cat -e "../test.txt") <(../cat/s21_cat -e "../test.txt") -s
echo ----------------------------------------------*CAT 3
diff <(cat -n "../test.txt") <(../cat/s21_cat -n "../test.txt") -s
echo ----------------------------------------------*CAT 4
diff <(cat -s "../test.txt") <(../cat/s21_cat -s "../test.txt") -s
echo ----------------------------------------------*CAT 5
diff <(cat -t "../test.txt") <(../cat/s21_cat -t "../test.txt") -s
