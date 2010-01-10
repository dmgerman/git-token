#!/bin/sh
# git-difftool--helper is a GIT_EXTERNAL_DIFF-compatible diff tool launcher.
# This script is typically launched by using the 'git difftool'
# convenience command.
#
# Copyright (c) 2009-2010 David Aguilar

TOOL_MODE=diff
. git-mergetool--lib

# difftool.prompt controls the default prompt/no-prompt behavior
# and is overridden with $GIT_DIFFTOOL*_PROMPT.
should_prompt () {
	prompt=$(git config --bool difftool.prompt || echo true)
	if test "$prompt" = true; then
		test -z "$GIT_DIFFTOOL_NO_PROMPT"
	else
		test -n "$GIT_DIFFTOOL_PROMPT"
	fi
}

# Indicates that --extcmd=... was specified
use_ext_cmd () {
	test -n "$GIT_DIFFTOOL_EXTCMD"
}

launch_merge_tool () {
	# Merged is the filename as it appears in the work tree
	# Local is the contents of a/filename
	# Remote is the contents of b/filename
	# Custom merge tool commands might use $BASE so we provide it
	MERGED="$1"
	LOCAL="$2"
	REMOTE="$3"
	BASE="$1"

	# $LOCAL and $REMOTE are temporary files so prompt
	# the user with the real $MERGED name before launching $merge_tool.
	if should_prompt; then
		printf "\nViewing: '$MERGED'\n"
		if use_ext_cmd; then
			printf "Hit return to launch '%s': " \
				"$GIT_DIFFTOOL_EXTCMD"
		else
			printf "Hit return to launch '%s': " "$merge_tool"
		fi
		read ans
	fi

	if use_ext_cmd; then
		$GIT_DIFFTOOL_EXTCMD "$LOCAL" "$REMOTE"
	else
		run_merge_tool "$merge_tool"
	fi

}

if ! use_ext_cmd; then
	if test -n "$GIT_DIFF_TOOL"; then
		merge_tool="$GIT_DIFF_TOOL"
	else
		merge_tool="$(get_merge_tool)" || exit
	fi
fi

# Launch the merge tool on each path provided by 'git diff'
while test $# -gt 6
do
	launch_merge_tool "$1" "$2" "$5"
	shift 7
done
