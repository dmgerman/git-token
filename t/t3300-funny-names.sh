#!/bin/sh
#
# Copyright (c) 2005 Junio C Hamano
#

test_description='Pathnames with funny characters.

This test tries pathnames with funny characters in the working
tree, index, and tree objects.
'

. ./test-lib.sh

p0='no-funny'
p1='tabs	and spaces'

cat >"$p0" <<\EOF
1. A quick brown fox jumps over the lazy cat, oops dog.
2. A quick brown fox jumps over the lazy cat, oops dog.
3. A quick brown fox jumps over the lazy cat, oops dog.
EOF

cat >"$p1" "$p0"

echo 'no-funny' >expected
test_expect_success 'git-ls-files no-funny' \
	'git-update-index --add "$p0" &&
	git-ls-files >current &&
	diff -u expected current'

t0=`git-write-tree`
echo "$t0" >t0

echo 'no-funny
"tabs\tand spaces"' >expected
test_expect_success 'git-ls-files with-funny' \
	'git-update-index --add "$p1" &&
	git-ls-files >current &&
	diff -u expected current'

echo 'no-funny
tabs	and spaces' >expected
test_expect_success 'git-ls-files -z with-funny' \
	'git-ls-files -z | tr \\0 \\012 >current &&
	diff -u expected current'

t1=`git-write-tree`
echo "$t1" >t1

echo 'no-funny
"tabs\tand spaces"' >expected
test_expect_success 'git-ls-tree with funny' \
	'git-ls-tree -r $t1 | sed -e "s/^[^	]*	//" >current &&
	 diff -u expected current'

echo 'A	"tabs\tand spaces"' >expected
test_expect_success 'git-diff-index with-funny' \
	'git-diff-index --name-status $t0 >current &&
	diff -u expected current'

test_expect_success 'git-diff-tree with-funny' \
	'git-diff-tree --name-status $t0 $t1 >current &&
	diff -u expected current'

echo 'A
tabs	and spaces' >expected
test_expect_success 'git-diff-index -z with-funny' \
	'git-diff-index -z --name-status $t0 | tr \\0 \\012 >current &&
	diff -u expected current'

test_expect_success 'git-diff-tree -z with-funny' \
	'git-diff-tree -z --name-status $t0 $t1 | tr \\0 \\012 >current &&
	diff -u expected current'

echo 'CNUM	no-funny	"tabs\tand spaces"' >expected
test_expect_success 'git-diff-tree -C with-funny' \
	'git-diff-tree -C --find-copies-harder --name-status \
		$t0 $t1 | sed -e 's/^C[0-9]*/CNUM/' >current &&
	diff -u expected current'

echo 'RNUM	no-funny	"tabs\tand spaces"' >expected
test_expect_success 'git-diff-tree delete with-funny' \
	'git-update-index --force-remove "$p0" &&
	git-diff-index -M --name-status \
		$t0 | sed -e 's/^R[0-9]*/RNUM/' >current &&
	diff -u expected current'

echo 'diff --git a/no-funny "b/tabs\tand spaces"
similarity index NUM%
rename from no-funny
rename to "tabs\tand spaces"' >expected

test_expect_success 'git-diff-tree delete with-funny' \
	'git-diff-index -M -p $t0 |
	 sed -e "s/index [0-9]*%/index NUM%/" >current &&
	 diff -u expected current'

chmod +x "$p1"
echo 'diff --git a/no-funny "b/tabs\tand spaces"
old mode 100644
new mode 100755
similarity index NUM%
rename from no-funny
rename to "tabs\tand spaces"' >expected

test_expect_success 'git-diff-tree delete with-funny' \
	'git-diff-index -M -p $t0 |
	 sed -e "s/index [0-9]*%/index NUM%/" >current &&
	 diff -u expected current'

echo >expected ' "tabs\tand spaces"
 1 files changed, 0 insertions(+), 0 deletions(-)'
test_expect_success 'git-diff-tree rename with-funny applied' \
	'git-diff-index -M -p $t0 |
	 git-apply --stat | sed -e "s/|.*//" -e "s/ *\$//" >current &&
	 diff -u expected current'

echo >expected ' no-funny
 "tabs\tand spaces"
 2 files changed, 3 insertions(+), 3 deletions(-)'

test_expect_success 'git-diff-tree delete with-funny applied' \
	'git-diff-index -p $t0 |
	 git-apply --stat | sed -e "s/|.*//" -e "s/ *\$//" >current &&
	 diff -u expected current'

test_expect_success 'git-apply non-git diff' \
	'git-diff-index -p $t0 |
	 sed -ne "/^[-+@]/p" |
	 git-apply --stat | sed -e "s/|.*//" -e "s/ *\$//" >current &&
	 diff -u expected current'

test_done
