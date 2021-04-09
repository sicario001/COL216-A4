for i in tests/*;do diff -cw <(./sim0 tests/test_part2.asm < inp) <(./sim tests/test_part2.asm < inp);done
