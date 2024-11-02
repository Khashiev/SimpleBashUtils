filename=cat.c;

echo ----------------------------------------------*CAT 1
diff <(cat -b "$filename") <(./s21_cat -b "$filename") -s
echo ----------------------------------------------*CAT 2
diff <(cat -e "$filename") <(./s21_cat -e "$filename") -s
echo ----------------------------------------------*CAT 3
diff <(cat -n "$filename") <(./s21_cat -n "$filename") -s
echo ----------------------------------------------*CAT 4
diff <(cat "$filename" -n) <(./s21_cat "$filename" -n) -s
echo ----------------------------------------------*CAT 5
diff <(cat -s "$filename") <(./s21_cat -s "$filename") -s
echo ----------------------------------------------*CAT 6
diff <(cat -t "$filename") <(./s21_cat -t "$filename") -s

