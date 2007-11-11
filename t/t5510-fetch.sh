#!/bin/sh
# Copyright (c) 2006, Junio C Hamano.

test_description='Per branch config variables affects "git fetch".

'

. ./test-lib.sh

D=`pwd`

test_expect_success setup '
	echo >file original &&
	git add file &&
	git commit -a -m original'

test_expect_success "clone and setup child repos" '
	git clone . one &&
	cd one &&
	echo >file updated by one &&
	git commit -a -m "updated by one" &&
	cd .. &&
	git clone . two &&
	cd two &&
	git config branch.master.remote one &&
	git config remote.one.url ../one/.git/ &&
	git config remote.one.fetch refs/heads/master:refs/heads/one &&
	cd .. &&
	git clone . three &&
	cd three &&
	git config branch.master.remote two &&
	git config branch.master.merge refs/heads/one &&
	mkdir -p .git/remotes &&
	{
		echo "URL: ../two/.git/"
		echo "Pull: refs/heads/master:refs/heads/two"
		echo "Pull: refs/heads/one:refs/heads/one"
	} >.git/remotes/two &&
	cd .. &&
	git clone . bundle
'

test_expect_success "fetch test" '
	cd "$D" &&
	echo >file updated by origin &&
	git commit -a -m "updated by origin" &&
	cd two &&
	git fetch &&
	test -f .git/refs/heads/one &&
	mine=`git rev-parse refs/heads/one` &&
	his=`cd ../one && git rev-parse refs/heads/master` &&
	test "z$mine" = "z$his"
'

test_expect_success "fetch test for-merge" '
	cd "$D" &&
	cd three &&
	git fetch &&
	test -f .git/refs/heads/two &&
	test -f .git/refs/heads/one &&
	master_in_two=`cd ../two && git rev-parse master` &&
	one_in_two=`cd ../two && git rev-parse one` &&
	{
		echo "$master_in_two	not-for-merge"
		echo "$one_in_two	"
	} >expected &&
	cut -f -2 .git/FETCH_HEAD >actual &&
	diff expected actual'

test_expect_success 'fetch tags when there is no tags' '

    cd "$D" &&

    mkdir notags &&
    cd notags &&
    git init &&

    git fetch -t ..

'

test_expect_success 'fetch following tags' '

	cd "$D" &&
	git tag -a -m 'annotated' anno HEAD &&
	git tag light HEAD &&

	mkdir four &&
	cd four &&
	git init &&

	git fetch .. :track &&
	git show-ref --verify refs/tags/anno &&
	git show-ref --verify refs/tags/light

'

test_expect_failure 'fetch must not resolve short tag name' '

	cd "$D" &&

	mkdir five &&
	cd five &&
	git init &&

	git fetch .. anno:five

'

test_expect_failure 'fetch must not resolve short remote name' '

	cd "$D" &&
	git-update-ref refs/remotes/six/HEAD HEAD

	mkdir six &&
	cd six &&
	git init &&

	git fetch .. six:six

'

test_expect_success 'create bundle 1' '
	cd "$D" &&
	echo >file updated again by origin &&
	git commit -a -m "tip" &&
	git bundle create bundle1 master^..master
'

test_expect_success 'header of bundle looks right' '
	head -n 1 "$D"/bundle1 | grep "^#" &&
	head -n 2 "$D"/bundle1 | grep "^-[0-9a-f]\{40\} " &&
	head -n 3 "$D"/bundle1 | grep "^[0-9a-f]\{40\} " &&
	head -n 4 "$D"/bundle1 | grep "^$"
'

test_expect_success 'create bundle 2' '
	cd "$D" &&
	git bundle create bundle2 master~2..master
'

test_expect_failure 'unbundle 1' '
	cd "$D/bundle" &&
	git checkout -b some-branch &&
	git fetch "$D/bundle1" master:master
'

test_expect_success 'bundle 1 has only 3 files ' '
	cd "$D" &&
	(
		while read x && test -n "$x"
		do
			:;
		done
		cat
	) <bundle1 >bundle.pack &&
	git index-pack bundle.pack &&
	verify=$(git verify-pack -v bundle.pack) &&
	test 4 = $(echo "$verify" | wc -l)
'

test_expect_success 'unbundle 2' '
	cd "$D/bundle" &&
	git fetch ../bundle2 master:master &&
	test "tip" = "$(git log -1 --pretty=oneline master | cut -b42-)"
'

test_expect_success 'bundle does not prerequisite objects' '
	cd "$D" &&
	touch file2 &&
	git add file2 &&
	git commit -m add.file2 file2 &&
	git bundle create bundle3 -1 HEAD &&
	(
		while read x && test -n "$x"
		do
			:;
		done
		cat
	) <bundle3 >bundle.pack &&
	git index-pack bundle.pack &&
	test 4 = $(git verify-pack -v bundle.pack | wc -l)
'

test_expect_success 'bundle should be able to create a full history' '

	cd "$D" &&
	git tag -a -m '1.0' v1.0 master &&
	git bundle create bundle4 v1.0

'

test "$TEST_RSYNC" && {
test_expect_success 'fetch via rsync' '
	git pack-refs &&
	mkdir rsynced &&
	cd rsynced &&
	git init &&
	git fetch rsync://127.0.0.1$(pwd)/../.git master:refs/heads/master &&
	git gc --prune &&
	test $(git rev-parse master) = $(cd .. && git rev-parse master) &&
	git fsck --full
'

test_expect_success 'push via rsync' '
	mkdir ../rsynced2 &&
	(cd ../rsynced2 &&
	 git init) &&
	git push rsync://127.0.0.1$(pwd)/../rsynced2/.git master &&
	cd ../rsynced2 &&
	git gc --prune &&
	test $(git rev-parse master) = $(cd .. && git rev-parse master) &&
	git fsck --full
'

test_expect_success 'push via rsync' '
	cd .. &&
	mkdir rsynced3 &&
	(cd rsynced3 &&
	 git init) &&
	git push --all rsync://127.0.0.1$(pwd)/rsynced3/.git &&
	cd rsynced3 &&
	test $(git rev-parse master) = $(cd .. && git rev-parse master) &&
	git fsck --full
'
}

test_expect_success 'fetch with a non-applying branch.<name>.merge' '
	git config branch.master.remote yeti &&
	git config branch.master.merge refs/heads/bigfoot &&
	git config remote.blub.url one &&
	git config remote.blub.fetch "refs/heads/*:refs/remotes/one/*" &&
	git fetch blub
'

# the strange name is: a\!'b
test_expect_success 'quoting of a strangely named repo' '
	! git fetch "a\\!'\''b" > result 2>&1 &&
	cat result &&
	grep "fatal: '\''a\\\\!'\''b'\''" result
'

test_done
