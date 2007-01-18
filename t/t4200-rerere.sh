#!/bin/sh
#
# Copyright (c) 2006 Johannes E. Schindelin
#

test_description='git-rerere
'

. ./test-lib.sh

cat > a1 << EOF
Whether 'tis nobler in the mind to suffer
The slings and arrows of outrageous fortune,
Or to take arms against a sea of troubles,
And by opposing end them? To die: to sleep;
No more; and by a sleep to say we end
The heart-ache and the thousand natural shocks
That flesh is heir to, 'tis a consummation
Devoutly to be wish'd.
EOF

git add a1
git commit -q -a -m initial

git checkout -b first
cat >> a1 << EOF
To die, to sleep;
To sleep: perchance to dream: ay, there's the rub;
For in that sleep of death what dreams may come
When we have shuffled off this mortal coil,
Must give us pause: there's the respect
That makes calamity of so long life;
EOF
git commit -q -a -m first

git checkout -b second master
git show first:a1 | sed 's/To die, t/To die! T/' > a1
git commit -q -a -m second

# activate rerere
mkdir .git/rr-cache

test_expect_failure 'conflicting merge' 'git pull . first'

sha1=4f58849a60b4f969a2848966b6d02893b783e8fb
rr=.git/rr-cache/$sha1
test_expect_success 'recorded preimage' "grep ======= $rr/preimage"

test_expect_success 'no postimage or thisimage yet' \
	"test ! -f $rr/postimage -a ! -f $rr/thisimage"

git show first:a1 > a1

cat > expect << EOF
--- a/a1
+++ b/a1
@@ -6,11 +6,7 @@
 The heart-ache and the thousand natural shocks
 That flesh is heir to, 'tis a consummation
 Devoutly to be wish'd.
-<<<<<<<
-To die! To sleep;
-=======
 To die, to sleep;
->>>>>>>
 To sleep: perchance to dream: ay, there's the rub;
 For in that sleep of death what dreams may come
 When we have shuffled off this mortal coil,
EOF

git rerere diff > out

test_expect_success 'rerere diff' 'diff -u expect out'

cat > expect << EOF
a1
EOF

git rerere status > out

test_expect_success 'rerere status' 'diff -u expect out'

test_expect_success 'commit succeeds' \
	"git commit -q -a -m 'prefer first over second'"

test_expect_success 'recorded postimage' "test -f $rr/postimage"

git checkout -b third master
git show second^:a1 | sed 's/To die: t/To die! T/' > a1
git commit -q -a -m third

test_expect_failure 'another conflicting merge' 'git pull . first'

git show first:a1 | sed 's/To die: t/To die! T/' > expect
test_expect_success 'rerere kicked in' "! grep ======= a1"

test_expect_success 'rerere prefers first change' 'diff -u a1 expect'

rm $rr/postimage
echo "$sha1	a1" | tr '\012' '\0' > .git/rr-cache/MERGE_RR

test_expect_success 'rerere clear' 'git rerere clear'

test_expect_success 'clear removed the directory' "test ! -d $rr"

mkdir $rr
echo Hello > $rr/preimage
echo World > $rr/postimage

sha2=4000000000000000000000000000000000000000
rr2=.git/rr-cache/$sha2
mkdir $rr2
echo Hello > $rr2/preimage

case "$(date -d @11111111 +%s 2>/dev/null)" in
[1-9]*)
	# it is a recent GNU date. good.
	now=$(date +%s)
	almost_15_days_ago=$(($now+60-15*86400))
	just_over_15_days_ago=$(($now-1-15*86400))
	almost_60_days_ago=$(($now+60-60*86400))
	just_over_60_days_ago=$(($now-1-60*86400))
	predate1="$(date -d "@$almost_60_days_ago" +%Y%m%d%H%M.%S)"
	predate2="$(date -d "@$almost_15_days_ago" +%Y%m%d%H%M.%S)"
	postdate1="$(date -d "@$just_over_60_days_ago" +%Y%m%d%H%M.%S)"
	postdate2="$(date -d "@$just_over_15_days_ago" +%Y%m%d%H%M.%S)"
	;;
*)
	# it is not GNU date. oh, well.
	predate1="$(date +%Y%m%d%H%M.%S)"
	predate2="$(date +%Y%m%d%H%M.%S)"
	postdate1='200610010000.00'
	postdate2='200612010000.00'
esac

touch -m -t "$predate1" $rr/preimage
touch -m -t "$predate2" $rr2/preimage

test_expect_success 'garbage collection (part1)' 'git rerere gc'

test_expect_success 'young records still live' \
	"test -f $rr/preimage -a -f $rr2/preimage"

touch -m -t "$postdate1" $rr/preimage
touch -m -t "$postdate2" $rr2/preimage

test_expect_success 'garbage collection (part2)' 'git rerere gc'

test_expect_success 'old records rest in peace' \
	"test ! -f $rr/preimage -a ! -f $rr2/preimage"

test_done


