for i in tests/*;do exho "$i"; diff -cw <(./sim0 "$i" true < inp) <(./sim "$i" < inp);done
