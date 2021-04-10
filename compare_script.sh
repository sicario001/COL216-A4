for i in tests/*;do echo "$i"; diff -cw <(./sim0 "$i" true < inp) <(./sim "$i" < inp);done
