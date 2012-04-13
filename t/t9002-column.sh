#!/bin/sh

test_description='git column'
. ./test-lib.sh

test_expect_success 'setup' '
	cat >lista <<\EOF
one
two
three
four
five
six
seven
eight
nine
ten
eleven
EOF
'

test_expect_success 'never' '
	git column --indent=Z --mode=never <lista >actual &&
	test_cmp lista actual
'

test_expect_success 'always' '
	cat >expected <<\EOF &&
Zone
Ztwo
Zthree
Zfour
Zfive
Zsix
Zseven
Zeight
Znine
Zten
Zeleven
EOF
	git column --indent=Z --mode=plain <lista >actual &&
	test_cmp expected actual
'

test_expect_success '80 columns' '
	cat >expected <<\EOF &&
one    two    three  four   five   six    seven  eight  nine   ten    eleven
EOF
	COLUMNS=80 git column --mode=column <lista >actual &&
	test_cmp expected actual
'

test_expect_success 'COLUMNS = 1' '
	cat >expected <<\EOF &&
one
two
three
four
five
six
seven
eight
nine
ten
eleven
EOF
	COLUMNS=1 git column --mode=column <lista >actual &&
	test_cmp expected actual
'

test_expect_success 'width = 1' '
	git column --mode=column --width=1 <lista >actual &&
	test_cmp expected actual
'

COLUMNS=20
export COLUMNS

test_expect_success '20 columns' '
	cat >expected <<\EOF &&
one    seven
two    eight
three  nine
four   ten
five   eleven
six
EOF
	git column --mode=column <lista >actual &&
	test_cmp expected actual
'

test_expect_success '20 columns, padding 2' '
	cat >expected <<\EOF &&
one     seven
two     eight
three   nine
four    ten
five    eleven
six
EOF
	git column --mode=column --padding 2 <lista >actual &&
	test_cmp expected actual
'

test_expect_success '20 columns, indented' '
	cat >expected <<\EOF &&
  one    seven
  two    eight
  three  nine
  four   ten
  five   eleven
  six
EOF
	git column --mode=column --indent="  " <lista >actual &&
	test_cmp expected actual
'

test_expect_success '20 columns, row first' '
	cat >expected <<\EOF &&
one    two
three  four
five   six
seven  eight
nine   ten
eleven
EOF
	git column --mode=row <lista >actual &&
	test_cmp expected actual
'

test_done
