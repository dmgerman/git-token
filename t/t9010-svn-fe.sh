#!/bin/sh

test_description='check svn dumpfile importer'

. ./test-lib.sh

reinit_git () {
	if ! test_declared_prereq PIPE
	then
		echo >&4 "reinit_git: need to declare PIPE prerequisite"
		return 127
	fi
	rm -fr .git &&
	rm -f stream backflow &&
	git init &&
	mkfifo stream backflow
}

try_dump () {
	input=$1 &&
	maybe_fail=${2:+test_$2} &&

	{
		$maybe_fail test-svn-fe "$input" >stream 3<backflow &
	} &&
	git fast-import --cat-blob-fd=3 <stream 3>backflow &&
	wait $!
}

properties () {
	while test "$#" -ne 0
	do
		property="$1" &&
		value="$2" &&
		printf "%s\n" "K ${#property}" &&
		printf "%s\n" "$property" &&
		printf "%s\n" "V ${#value}" &&
		printf "%s\n" "$value" &&
		shift 2 ||
		return 1
	done
}

text_no_props () {
	text="$1
" &&
	printf "%s\n" "Prop-content-length: 10" &&
	printf "%s\n" "Text-content-length: ${#text}" &&
	printf "%s\n" "Content-length: $((${#text} + 10))" &&
	printf "%s\n" "" "PROPS-END" &&
	printf "%s\n" "$text"
}

>empty

test_expect_success 'setup: have pipes?' '
	rm -f frob &&
	if mkfifo frob
	then
		test_set_prereq PIPE
	fi
'

test_expect_success PIPE 'empty dump' '
	reinit_git &&
	echo "SVN-fs-dump-format-version: 2" >input &&
	try_dump input
'

test_expect_success PIPE 'v4 dumps not supported' '
	reinit_git &&
	echo "SVN-fs-dump-format-version: 4" >v4.dump &&
	try_dump v4.dump must_fail
'

test_expect_failure PIPE 'empty revision' '
	reinit_git &&
	printf "rev <nobody, nobody@local>: %s\n" "" "" >expect &&
	cat >emptyrev.dump <<-\EOF &&
	SVN-fs-dump-format-version: 3

	Revision-number: 1
	Prop-content-length: 0
	Content-length: 0

	Revision-number: 2
	Prop-content-length: 0
	Content-length: 0

	EOF
	try_dump emptyrev.dump &&
	git log -p --format="rev <%an, %ae>: %s" HEAD >actual &&
	test_cmp expect actual
'

test_expect_success PIPE 'empty properties' '
	reinit_git &&
	printf "rev <nobody, nobody@local>: %s\n" "" "" >expect &&
	cat >emptyprop.dump <<-\EOF &&
	SVN-fs-dump-format-version: 3

	Revision-number: 1
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Revision-number: 2
	Prop-content-length: 10
	Content-length: 10

	PROPS-END
	EOF
	try_dump emptyprop.dump &&
	git log -p --format="rev <%an, %ae>: %s" HEAD >actual &&
	test_cmp expect actual
'

test_expect_success PIPE 'author name and commit message' '
	reinit_git &&
	echo "<author@example.com, author@example.com@local>" >expect.author &&
	cat >message <<-\EOF &&
	A concise summary of the change

	A detailed description of the change, why it is needed, what
	was broken and why applying this is the best course of action.

	* file.c
	    Details pertaining to an individual file.
	EOF
	{
		properties \
			svn:author author@example.com \
			svn:log "$(cat message)" &&
		echo PROPS-END
	} >props &&
	{
		echo "SVN-fs-dump-format-version: 3" &&
		echo &&
		echo "Revision-number: 1" &&
		echo Prop-content-length: $(wc -c <props) &&
		echo Content-length: $(wc -c <props) &&
		echo &&
		cat props
	} >log.dump &&
	try_dump log.dump &&
	git log -p --format="%B" HEAD >actual.log &&
	git log --format="<%an, %ae>" >actual.author &&
	test_cmp message actual.log &&
	test_cmp expect.author actual.author
'

test_expect_success PIPE 'unsupported properties are ignored' '
	reinit_git &&
	echo author >expect &&
	cat >extraprop.dump <<-\EOF &&
	SVN-fs-dump-format-version: 3

	Revision-number: 1
	Prop-content-length: 56
	Content-length: 56

	K 8
	nonsense
	V 1
	y
	K 10
	svn:author
	V 6
	author
	PROPS-END
	EOF
	try_dump extraprop.dump &&
	git log -p --format=%an HEAD >actual &&
	test_cmp expect actual
'

test_expect_failure PIPE 'timestamp and empty file' '
	echo author@example.com >expect.author &&
	echo 1999-01-01 >expect.date &&
	echo file >expect.files &&
	reinit_git &&
	{
		properties \
			svn:author author@example.com \
			svn:date "1999-01-01T00:01:002.000000Z" \
			svn:log "add empty file" &&
		echo PROPS-END
	} >props &&
	{
		cat <<-EOF &&
		SVN-fs-dump-format-version: 3

		Revision-number: 1
		EOF
		echo Prop-content-length: $(wc -c <props) &&
		echo Content-length: $(wc -c <props) &&
		echo &&
		cat props &&
		cat <<-\EOF

		Node-path: empty-file
		Node-kind: file
		Node-action: add
		Content-length: 0

		EOF
	} >emptyfile.dump &&
	try_dump emptyfile.dump &&
	git log --format=%an HEAD >actual.author &&
	git log --date=short --format=%ad HEAD >actual.date &&
	git ls-tree -r --name-only HEAD >actual.files &&
	test_cmp expect.author actual.author &&
	test_cmp expect.date actual.date &&
	test_cmp expect.files actual.files &&
	git checkout HEAD empty-file &&
	test_cmp empty file
'

test_expect_success PIPE 'directory with files' '
	reinit_git &&
	printf "%s\n" directory/file1 directory/file2 >expect.files &&
	echo hi >hi &&
	echo hello >hello &&
	{
		properties \
			svn:author author@example.com \
			svn:date "1999-02-01T00:01:002.000000Z" \
			svn:log "add directory with some files in it" &&
		echo PROPS-END
	} >props &&
	{
		cat <<-EOF &&
		SVN-fs-dump-format-version: 3

		Revision-number: 1
		EOF
		echo Prop-content-length: $(wc -c <props) &&
		echo Content-length: $(wc -c <props) &&
		echo &&
		cat props &&
		cat <<-\EOF &&

		Node-path: directory
		Node-kind: dir
		Node-action: add
		Prop-content-length: 10
		Content-length: 10

		PROPS-END

		Node-path: directory/file1
		Node-kind: file
		Node-action: add
		EOF
		text_no_props hello &&
		cat <<-\EOF &&
		Node-path: directory/file2
		Node-kind: file
		Node-action: add
		EOF
		text_no_props hi
	} >directory.dump &&
	try_dump directory.dump &&

	git ls-tree -r --name-only HEAD >actual.files &&
	git checkout HEAD directory &&
	test_cmp expect.files actual.files &&
	test_cmp hello directory/file1 &&
	test_cmp hi directory/file2
'

test_expect_success PIPE 'node without action' '
	reinit_git &&
	cat >inaction.dump <<-\EOF &&
	SVN-fs-dump-format-version: 3

	Revision-number: 1
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: directory
	Node-kind: dir
	Prop-content-length: 10
	Content-length: 10

	PROPS-END
	EOF
	try_dump inaction.dump must_fail
'

test_expect_success PIPE 'action: add node without text' '
	reinit_git &&
	cat >textless.dump <<-\EOF &&
	SVN-fs-dump-format-version: 3

	Revision-number: 1
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: textless
	Node-kind: file
	Node-action: add
	Prop-content-length: 10
	Content-length: 10

	PROPS-END
	EOF
	try_dump textless.dump must_fail
'

test_expect_failure PIPE 'change file mode but keep old content' '
	reinit_git &&
	cat >expect <<-\EOF &&
	OBJID
	:120000 100644 OBJID OBJID T	greeting
	OBJID
	:100644 120000 OBJID OBJID T	greeting
	OBJID
	:000000 100644 OBJID OBJID A	greeting
	EOF
	echo "link hello" >expect.blob &&
	echo hello >hello &&
	cat >filemode.dump <<-\EOF &&
	SVN-fs-dump-format-version: 3

	Revision-number: 1
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: greeting
	Node-kind: file
	Node-action: add
	Prop-content-length: 10
	Text-content-length: 11
	Content-length: 21

	PROPS-END
	link hello

	Revision-number: 2
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: greeting
	Node-kind: file
	Node-action: change
	Prop-content-length: 33
	Content-length: 33

	K 11
	svn:special
	V 1
	*
	PROPS-END

	Revision-number: 3
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: greeting
	Node-kind: file
	Node-action: change
	Prop-content-length: 10
	Content-length: 10

	PROPS-END
	EOF
	try_dump filemode.dump &&
	{
		git rev-list HEAD |
		git diff-tree --root --stdin |
		sed "s/$_x40/OBJID/g"
	} >actual &&
	git show HEAD:greeting >actual.blob &&
	git show HEAD^:greeting >actual.target &&
	test_cmp expect actual &&
	test_cmp expect.blob actual.blob &&
	test_cmp hello actual.target
'

test_expect_success PIPE 'change file mode and reiterate content' '
	reinit_git &&
	cat >expect <<-\EOF &&
	OBJID
	:120000 100644 OBJID OBJID T	greeting
	OBJID
	:100644 120000 OBJID OBJID T	greeting
	OBJID
	:000000 100644 OBJID OBJID A	greeting
	EOF
	echo "link hello" >expect.blob &&
	echo hello >hello &&
	cat >filemode2.dump <<-\EOF &&
	SVN-fs-dump-format-version: 3

	Revision-number: 1
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: greeting
	Node-kind: file
	Node-action: add
	Prop-content-length: 10
	Text-content-length: 11
	Content-length: 21

	PROPS-END
	link hello

	Revision-number: 2
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: greeting
	Node-kind: file
	Node-action: change
	Prop-content-length: 33
	Text-content-length: 11
	Content-length: 44

	K 11
	svn:special
	V 1
	*
	PROPS-END
	link hello

	Revision-number: 3
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: greeting
	Node-kind: file
	Node-action: change
	Prop-content-length: 10
	Text-content-length: 11
	Content-length: 21

	PROPS-END
	link hello
	EOF
	try_dump filemode2.dump &&
	{
		git rev-list HEAD |
		git diff-tree --root --stdin |
		sed "s/$_x40/OBJID/g"
	} >actual &&
	git show HEAD:greeting >actual.blob &&
	git show HEAD^:greeting >actual.target &&
	test_cmp expect actual &&
	test_cmp expect.blob actual.blob &&
	test_cmp hello actual.target
'

test_expect_success PIPE 'deltas not supported' '
	reinit_git &&
	{
		# (old) h + (inline) ello + (old) \n
		printf "SVNQ%b%b%s" "Q\003\006\005\004" "\001Q\0204\001\002" "ello" |
		q_to_nul
	} >delta &&
	{
		properties \
			svn:author author@example.com \
			svn:date "1999-01-05T00:01:002.000000Z" \
			svn:log "add greeting" &&
		echo PROPS-END
	} >props &&
	{
		properties \
			svn:author author@example.com \
			svn:date "1999-01-06T00:01:002.000000Z" \
			svn:log "change it" &&
		echo PROPS-END
	} >props2 &&
	{
		echo SVN-fs-dump-format-version: 3 &&
		echo &&
		echo Revision-number: 1 &&
		echo Prop-content-length: $(wc -c <props) &&
		echo Content-length: $(wc -c <props) &&
		echo &&
		cat props &&
		cat <<-\EOF &&

		Node-path: hello
		Node-kind: file
		Node-action: add
		Prop-content-length: 10
		Text-content-length: 3
		Content-length: 13

		PROPS-END
		hi

		EOF
		echo Revision-number: 2 &&
		echo Prop-content-length: $(wc -c <props2) &&
		echo Content-length: $(wc -c <props2) &&
		echo &&
		cat props2 &&
		cat <<-\EOF &&

		Node-path: hello
		Node-kind: file
		Node-action: change
		Text-delta: true
		Prop-content-length: 10
		EOF
		echo Text-content-length: $(wc -c <delta) &&
		echo Content-length: $((10 + $(wc -c <delta))) &&
		echo &&
		echo PROPS-END &&
		cat delta
	} >delta.dump &&
	test_must_fail try_dump delta.dump
'

test_expect_success PIPE 'property deltas supported' '
	reinit_git &&
	cat >expect <<-\EOF &&
	OBJID
	:100755 100644 OBJID OBJID M	script.sh
	EOF
	{
		properties \
			svn:author author@example.com \
			svn:date "1999-03-06T00:01:002.000000Z" \
			svn:log "make an executable, or chmod -x it" &&
		echo PROPS-END
	} >revprops &&
	{
		echo SVN-fs-dump-format-version: 3 &&
		echo &&
		echo Revision-number: 1 &&
		echo Prop-content-length: $(wc -c <revprops) &&
		echo Content-length: $(wc -c <revprops) &&
		echo &&
		cat revprops &&
		echo &&
		cat <<-\EOF &&
		Node-path: script.sh
		Node-kind: file
		Node-action: add
		Text-content-length: 0
		Prop-content-length: 39
		Content-length: 39

		K 14
		svn:executable
		V 4
		true
		PROPS-END

		EOF
		echo Revision-number: 2 &&
		echo Prop-content-length: $(wc -c <revprops) &&
		echo Content-length: $(wc -c <revprops) &&
		echo &&
		cat revprops &&
		echo &&
		cat <<-\EOF
		Node-path: script.sh
		Node-kind: file
		Node-action: change
		Prop-delta: true
		Prop-content-length: 30
		Content-length: 30

		D 14
		svn:executable
		PROPS-END
		EOF
	} >propdelta.dump &&
	try_dump propdelta.dump &&
	{
		git rev-list HEAD |
		git diff-tree --stdin |
		sed "s/$_x40/OBJID/g"
	} >actual &&
	test_cmp expect actual
'

test_expect_success PIPE 'properties on /' '
	reinit_git &&
	cat <<-\EOF >expect &&
	OBJID
	OBJID
	:000000 100644 OBJID OBJID A	greeting
	EOF
	sed -e "s/X$//" <<-\EOF >changeroot.dump &&
	SVN-fs-dump-format-version: 3

	Revision-number: 1
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: greeting
	Node-kind: file
	Node-action: add
	Text-content-length: 0
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Revision-number: 2
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: X
	Node-kind: dir
	Node-action: change
	Prop-delta: true
	Prop-content-length: 43
	Content-length: 43

	K 10
	svn:ignore
	V 11
	build-area

	PROPS-END
	EOF
	try_dump changeroot.dump &&
	{
		git rev-list HEAD |
		git diff-tree --root --always --stdin |
		sed "s/$_x40/OBJID/g"
	} >actual &&
	test_cmp expect actual
'

test_expect_success PIPE 'deltas for typechange' '
	reinit_git &&
	cat >expect <<-\EOF &&
	OBJID
	:120000 100644 OBJID OBJID T	test-file
	OBJID
	:100755 120000 OBJID OBJID T	test-file
	OBJID
	:000000 100755 OBJID OBJID A	test-file
	EOF
	cat >deleteprop.dump <<-\EOF &&
	SVN-fs-dump-format-version: 3

	Revision-number: 1
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: test-file
	Node-kind: file
	Node-action: add
	Prop-delta: true
	Prop-content-length: 35
	Text-content-length: 17
	Content-length: 52

	K 14
	svn:executable
	V 0

	PROPS-END
	link testing 123

	Revision-number: 2
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: test-file
	Node-kind: file
	Node-action: change
	Prop-delta: true
	Prop-content-length: 53
	Text-content-length: 17
	Content-length: 70

	K 11
	svn:special
	V 1
	*
	D 14
	svn:executable
	PROPS-END
	link testing 231

	Revision-number: 3
	Prop-content-length: 10
	Content-length: 10

	PROPS-END

	Node-path: test-file
	Node-kind: file
	Node-action: change
	Prop-delta: true
	Prop-content-length: 27
	Text-content-length: 17
	Content-length: 44

	D 11
	svn:special
	PROPS-END
	link testing 321
	EOF
	try_dump deleteprop.dump &&
	{
		git rev-list HEAD |
		git diff-tree --root --stdin |
		sed "s/$_x40/OBJID/g"
	} >actual &&
	test_cmp expect actual
'


test_expect_success 'set up svn repo' '
	svnconf=$PWD/svnconf &&
	mkdir -p "$svnconf" &&

	if
		svnadmin -h >/dev/null 2>&1 &&
		svnadmin create simple-svn &&
		svnadmin load simple-svn <"$TEST_DIRECTORY/t9135/svn.dump" &&
		svn export --config-dir "$svnconf" "file://$PWD/simple-svn" simple-svnco
	then
		test_set_prereq SVNREPO
	fi
'

test_expect_success SVNREPO,PIPE 't9135/svn.dump' '
	mkdir -p simple-git &&
	(
		cd simple-git &&
		reinit_git &&
		try_dump "$TEST_DIRECTORY/t9135/svn.dump"
	) &&
	(
		cd simple-svnco &&
		git init &&
		git add . &&
		git fetch ../simple-git master &&
		git diff --exit-code FETCH_HEAD
	)
'

test_done
