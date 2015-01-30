#!/bin/sh

test_description='apply to deeper directory without getting fooled with symlink'
. ./test-lib.sh

lecho () {
	for l_
	do
		echo "$l_"
	done
}

test_expect_success setup '

	mkdir -p arch/i386/boot arch/x86_64 &&
	lecho 1 2 3 4 5 >arch/i386/boot/Makefile &&
	test_ln_s_add ../i386/boot arch/x86_64/boot &&
	git add . &&
	test_tick &&
	git commit -m initial &&
	git branch test &&

	rm arch/x86_64/boot &&
	mkdir arch/x86_64/boot &&
	lecho 2 3 4 5 6 >arch/x86_64/boot/Makefile &&
	git add . &&
	test_tick &&
	git commit -a -m second &&

	git format-patch --binary -1 --stdout >test.patch

'

test_expect_success apply '

	git checkout test &&
	git diff --exit-code test &&
	git diff --exit-code --cached test &&
	git apply --index test.patch

'

test_expect_success 'check result' '

	git diff --exit-code master &&
	git diff --exit-code --cached master &&
	test_tick &&
	git commit -m replay &&
	T1=$(git rev-parse "master^{tree}") &&
	T2=$(git rev-parse "HEAD^{tree}") &&
	test "z$T1" = "z$T2"

'

test_expect_success SYMLINKS 'do not read from beyond symbolic link' '
	git reset --hard &&
	mkdir -p arch/x86_64/dir &&
	>arch/x86_64/dir/file &&
	git add arch/x86_64/dir/file &&
	echo line >arch/x86_64/dir/file &&
	git diff >patch &&
	git reset --hard &&

	mkdir arch/i386/dir &&
	>arch/i386/dir/file &&
	ln -s ../i386/dir arch/x86_64/dir &&

	test_must_fail git apply patch &&
	test_must_fail git apply --cached patch &&
	test_must_fail git apply --index patch

'

test_done
