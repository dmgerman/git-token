# bash/zsh git prompt support
#
# Copyright (C) 2006,2007 Shawn O. Pearce <spearce@spearce.org>
# Distributed under the GNU General Public License, version 2.0.
#
# This script allows you to see the current branch in your prompt.
#
# To enable:
#
#    1) Copy this file to somewhere (e.g. ~/.git-prompt.sh).
#    2) Add the following line to your .bashrc/.zshrc:
#        source ~/.git-prompt.sh
#    3a) Change your PS1 to call __git_ps1 as
#        command-substitution:
#        Bash: PS1='[\u@\h \W$(__git_ps1 " (%s)")]\$ '
#        ZSH:  PS1='[%n@%m %c$(__git_ps1 " (%s)")]\$ '
#        the optional argument will be used as format string.
#    3b) Alternatively, if you are using bash, __git_ps1 can be
#        used for PROMPT_COMMAND with two parameters, <pre> and
#        <post>, which are strings you would put in $PS1 before
#        and after the status string generated by the git-prompt
#        machinery.  e.g.
#        Bash: PROMPT_COMMAND='__git_ps1 "\u@\h:\w" "\\\$ "'
#        ZSH:  precmd () { __git_ps1 "%n" ":%~$ " "|%s" }
#        will show username, at-sign, host, colon, cwd, then
#        various status string, followed by dollar and SP, as
#        your prompt.
#        Optionally, you can supply a third argument with a printf
#        format string to finetune the output of the branch status
#
# The argument to __git_ps1 will be displayed only if you are currently
# in a git repository.  The %s token will be the name of the current
# branch.
#
# In addition, if you set GIT_PS1_SHOWDIRTYSTATE to a nonempty value,
# unstaged (*) and staged (+) changes will be shown next to the branch
# name.  You can configure this per-repository with the
# bash.showDirtyState variable, which defaults to true once
# GIT_PS1_SHOWDIRTYSTATE is enabled.
#
# You can also see if currently something is stashed, by setting
# GIT_PS1_SHOWSTASHSTATE to a nonempty value. If something is stashed,
# then a '$' will be shown next to the branch name.
#
# If you would like to see if there're untracked files, then you can set
# GIT_PS1_SHOWUNTRACKEDFILES to a nonempty value. If there're untracked
# files, then a '%' will be shown next to the branch name.  You can
# configure this per-repository with the bash.showUntrackedFiles
# variable, which defaults to true once GIT_PS1_SHOWUNTRACKEDFILES is
# enabled.
#
# If you would like to see the difference between HEAD and its upstream,
# set GIT_PS1_SHOWUPSTREAM="auto".  A "<" indicates you are behind, ">"
# indicates you are ahead, "<>" indicates you have diverged and "="
# indicates that there is no difference. You can further control
# behaviour by setting GIT_PS1_SHOWUPSTREAM to a space-separated list
# of values:
#
#     verbose       show number of commits ahead/behind (+/-) upstream
#     legacy        don't use the '--count' option available in recent
#                   versions of git-rev-list
#     git           always compare HEAD to @{upstream}
#     svn           always compare HEAD to your SVN upstream
#
# By default, __git_ps1 will compare HEAD to your SVN upstream if it can
# find one, or @{upstream} otherwise.  Once you have set
# GIT_PS1_SHOWUPSTREAM, you can override it on a per-repository basis by
# setting the bash.showUpstream config variable.
#
# If you would like to see more information about the identity of
# commits checked out as a detached HEAD, set GIT_PS1_DESCRIBE_STYLE
# to one of these values:
#
#     contains      relative to newer annotated tag (v1.6.3.2~35)
#     branch        relative to newer tag or branch (master~4)
#     describe      relative to older annotated tag (v1.6.3.1-13-gdd42c2f)
#     default       exactly matching tag
#
# If you would like a colored hint about the current dirty state, set
# GIT_PS1_SHOWCOLORHINTS to a nonempty value. The colors are based on
# the colored output of "git status -sb".

# stores the divergence from upstream in $p
# used by GIT_PS1_SHOWUPSTREAM
__git_ps1_show_upstream ()
{
	local key value
	local svn_remote svn_url_pattern count n
	local upstream=git legacy="" verbose=""

	svn_remote=()
	# get some config options from git-config
	local output="$(git config -z --get-regexp '^(svn-remote\..*\.url|bash\.showupstream)$' 2>/dev/null | tr '\0\n' '\n ')"
	while read -r key value; do
		case "$key" in
		bash.showupstream)
			GIT_PS1_SHOWUPSTREAM="$value"
			if [[ -z "${GIT_PS1_SHOWUPSTREAM}" ]]; then
				p=""
				return
			fi
			;;
		svn-remote.*.url)
			svn_remote[$((${#svn_remote[@]} + 1))]="$value"
			svn_url_pattern+="\\|$value"
			upstream=svn+git # default upstream is SVN if available, else git
			;;
		esac
	done <<< "$output"

	# parse configuration values
	for option in ${GIT_PS1_SHOWUPSTREAM}; do
		case "$option" in
		git|svn) upstream="$option" ;;
		verbose) verbose=1 ;;
		legacy)  legacy=1  ;;
		esac
	done

	# Find our upstream
	case "$upstream" in
	git)    upstream="@{upstream}" ;;
	svn*)
		# get the upstream from the "git-svn-id: ..." in a commit message
		# (git-svn uses essentially the same procedure internally)
		local -a svn_upstream
		svn_upstream=($(git log --first-parent -1 \
					--grep="^git-svn-id: \(${svn_url_pattern#??}\)" 2>/dev/null))
		if [[ 0 -ne ${#svn_upstream[@]} ]]; then
			svn_upstream=${svn_upstream[${#svn_upstream[@]} - 2]}
			svn_upstream=${svn_upstream%@*}
			local n_stop="${#svn_remote[@]}"
			for ((n=1; n <= n_stop; n++)); do
				svn_upstream=${svn_upstream#${svn_remote[$n]}}
			done

			if [[ -z "$svn_upstream" ]]; then
				# default branch name for checkouts with no layout:
				upstream=${GIT_SVN_ID:-git-svn}
			else
				upstream=${svn_upstream#/}
			fi
		elif [[ "svn+git" = "$upstream" ]]; then
			upstream="@{upstream}"
		fi
		;;
	esac

	# Find how many commits we are ahead/behind our upstream
	if [[ -z "$legacy" ]]; then
		count="$(git rev-list --count --left-right \
				"$upstream"...HEAD 2>/dev/null)"
	else
		# produce equivalent output to --count for older versions of git
		local commits
		if commits="$(git rev-list --left-right "$upstream"...HEAD 2>/dev/null)"
		then
			local commit behind=0 ahead=0
			for commit in $commits
			do
				case "$commit" in
				"<"*) ((behind++)) ;;
				*)    ((ahead++))  ;;
				esac
			done
			count="$behind	$ahead"
		else
			count=""
		fi
	fi

	# calculate the result
	if [[ -z "$verbose" ]]; then
		case "$count" in
		"") # no upstream
			p="" ;;
		"0	0") # equal to upstream
			p="=" ;;
		"0	"*) # ahead of upstream
			p=">" ;;
		*"	0") # behind upstream
			p="<" ;;
		*)	    # diverged from upstream
			p="<>" ;;
		esac
	else
		case "$count" in
		"") # no upstream
			p="" ;;
		"0	0") # equal to upstream
			p=" u=" ;;
		"0	"*) # ahead of upstream
			p=" u+${count#0	}" ;;
		*"	0") # behind upstream
			p=" u-${count%	0}" ;;
		*)	    # diverged from upstream
			p=" u+${count#*	}-${count%	*}" ;;
		esac
	fi

}

# Helper function that is meant to be called from __git_ps1.  It
# builds up a gitstring injecting color codes into the appropriate
# places.
__git_ps1_colorize_gitstring ()
{
	if [[ -n ${ZSH_VERSION-} ]]; then
		local c_red='%F{red}'
		local c_green='%F{green}'
		local c_lblue='%F{blue}'
		local c_clear='%f'
		local bad_color=$c_red
		local ok_color=$c_green
		local branch_color="$c_clear"
		local flags_color="$c_lblue"
		local branchstring="$c${b##refs/heads/}"

		if [ $detached = no ]; then
			branch_color="$ok_color"
		else
			branch_color="$bad_color"
		fi

		gitstring="$branch_color$branchstring$c_clear"

		if [ -n "$w$i$s$u$r$p" ]; then
			gitstring="$gitstring$z"
		fi
		if [ "$w" = "*" ]; then
			gitstring="$gitstring$bad_color$w"
		fi
		if [ -n "$i" ]; then
			gitstring="$gitstring$ok_color$i"
		fi
		if [ -n "$s" ]; then
			gitstring="$gitstring$flags_color$s"
		fi
		if [ -n "$u" ]; then
			gitstring="$gitstring$bad_color$u"
		fi
		gitstring="$gitstring$c_clear$r$p"
		return
	fi
	local c_red='\e[31m'
	local c_green='\e[32m'
	local c_lblue='\e[1;34m'
	local c_clear='\e[0m'
	local bad_color=$c_red
	local ok_color=$c_green
	local branch_color="$c_clear"
	local flags_color="$c_lblue"
	local branchstring="$c${b##refs/heads/}"

	if [ $detached = no ]; then
		branch_color="$ok_color"
	else
		branch_color="$bad_color"
	fi

	# Setting gitstring directly with \[ and \] around colors
	# is necessary to prevent wrapping issues!
	gitstring="\[$branch_color\]$branchstring\[$c_clear\]"

	if [ -n "$w$i$s$u$r$p" ]; then
		gitstring="$gitstring$z"
	fi
	if [ "$w" = "*" ]; then
		gitstring="$gitstring\[$bad_color\]$w"
	fi
	if [ -n "$i" ]; then
		gitstring="$gitstring\[$ok_color\]$i"
	fi
	if [ -n "$s" ]; then
		gitstring="$gitstring\[$flags_color\]$s"
	fi
	if [ -n "$u" ]; then
		gitstring="$gitstring\[$bad_color\]$u"
	fi
	gitstring="$gitstring\[$c_clear\]$r$p"
}

# __git_ps1 accepts 0 or 1 arguments (i.e., format string)
# when called from PS1 using command substitution
# in this mode it prints text to add to bash PS1 prompt (includes branch name)
#
# __git_ps1 requires 2 or 3 arguments when called from PROMPT_COMMAND (pc)
# in that case it _sets_ PS1. The arguments are parts of a PS1 string.
# when two arguments are given, the first is prepended and the second appended
# to the state string when assigned to PS1.
# The optional third parameter will be used as printf format string to further
# customize the output of the git-status string.
# In this mode you can request colored hints using GIT_PS1_SHOWCOLORHINTS=true
__git_ps1 ()
{
	local pcmode=no
	local detached=no
	local ps1pc_start='\u@\h:\w '
	local ps1pc_end='\$ '
	local printf_format=' (%s)'

	case "$#" in
		2|3)	pcmode=yes
			ps1pc_start="$1"
			ps1pc_end="$2"
			printf_format="${3:-$printf_format}"
		;;
		0|1)	printf_format="${1:-$printf_format}"
		;;
		*)	return
		;;
	esac

	local g="$(git rev-parse --git-dir 2>/dev/null)"
	if [ -z "$g" ]; then
		if [ $pcmode = yes ]; then
			#In PC mode PS1 always needs to be set
			PS1="$ps1pc_start$ps1pc_end"
		fi
		return
	fi

	local r=""
	local b=""
	local step=""
	local total=""
	if [ -d "$g/rebase-merge" ]; then
		read b 2>/dev/null <"$g/rebase-merge/head-name"
		read step 2>/dev/null <"$g/rebase-merge/msgnum"
		read total 2>/dev/null <"$g/rebase-merge/end"
		if [ -f "$g/rebase-merge/interactive" ]; then
			r="|REBASE-i"
		else
			r="|REBASE-m"
		fi
	else
		if [ -d "$g/rebase-apply" ]; then
			read step 2>/dev/null <"$g/rebase-apply/next"
			read total 2>/dev/null <"$g/rebase-apply/last"
			if [ -f "$g/rebase-apply/rebasing" ]; then
				read b 2>/dev/null <"$g/rebase-apply/head-name"
				r="|REBASE"
			elif [ -f "$g/rebase-apply/applying" ]; then
				r="|AM"
			else
				r="|AM/REBASE"
			fi
		elif [ -f "$g/MERGE_HEAD" ]; then
			r="|MERGING"
		elif [ -f "$g/CHERRY_PICK_HEAD" ]; then
			r="|CHERRY-PICKING"
		elif [ -f "$g/REVERT_HEAD" ]; then
			r="|REVERTING"
		elif [ -f "$g/BISECT_LOG" ]; then
			r="|BISECTING"
		fi

		if [ -n "$b" ]; then
			:
		elif [ -h "$g/HEAD" ]; then
			# symlink symbolic ref
			b="$(git symbolic-ref HEAD 2>/dev/null)"
		else
			local head=""
			if ! read head 2>/dev/null <"$g/HEAD"; then
				if [ $pcmode = yes ]; then
					PS1="$ps1pc_start$ps1pc_end"
				fi
				return
			fi
			# is it a symbolic ref?
			b="${head#ref: }"
			if [ "$head" = "$b" ]; then
				detached=yes
				b="$(
				case "${GIT_PS1_DESCRIBE_STYLE-}" in
				(contains)
					git describe --contains HEAD ;;
				(branch)
					git describe --contains --all HEAD ;;
				(describe)
					git describe HEAD ;;
				(* | default)
					git describe --tags --exact-match HEAD ;;
				esac 2>/dev/null)" ||

				b="$(git rev-parse --short HEAD 2>/dev/null)..." ||
				b="unknown"
				b="($b)"
			fi
		fi
	fi

	if [ -n "$step" ] && [ -n "$total" ]; then
		r="$r $step/$total"
	fi

	local w=""
	local i=""
	local s=""
	local u=""
	local c=""
	local p=""

	if [ "true" = "$(git rev-parse --is-inside-git-dir 2>/dev/null)" ]; then
		if [ "true" = "$(git rev-parse --is-bare-repository 2>/dev/null)" ]; then
			c="BARE:"
		else
			b="GIT_DIR!"
		fi
	elif [ "true" = "$(git rev-parse --is-inside-work-tree 2>/dev/null)" ]; then
		if [ -n "${GIT_PS1_SHOWDIRTYSTATE-}" ] &&
		   [ "$(git config --bool bash.showDirtyState)" != "false" ]
		then
			git diff --no-ext-diff --quiet --exit-code || w="*"
			if git rev-parse --quiet --verify HEAD >/dev/null; then
				git diff-index --cached --quiet HEAD -- || i="+"
			else
				i="#"
			fi
		fi
		if [ -n "${GIT_PS1_SHOWSTASHSTATE-}" ]; then
			git rev-parse --verify refs/stash >/dev/null 2>&1 && s="$"
		fi

		if [ -n "${GIT_PS1_SHOWUNTRACKEDFILES-}" ] &&
		   [ "$(git config --bool bash.showUntrackedFiles)" != "false" ] &&
		   [ -n "$(git ls-files --others --exclude-standard)" ]
		then
			u="%${ZSH_VERSION+%}"
		fi

		if [ -n "${GIT_PS1_SHOWUPSTREAM-}" ]; then
			__git_ps1_show_upstream
		fi
	fi

	local z="${GIT_PS1_STATESEPARATOR-" "}"
	local f="$w$i$s$u"
	if [ $pcmode = yes ]; then
		local gitstring=
		if [ -n "${GIT_PS1_SHOWCOLORHINTS-}" ]; then
			__git_ps1_colorize_gitstring
		else
			gitstring="$c${b##refs/heads/}${f:+$z$f}$r$p"
		fi
		gitstring=$(printf -- "$printf_format" "$gitstring")
		PS1="$ps1pc_start$gitstring$ps1pc_end"
	else
		# NO color option unless in PROMPT_COMMAND mode
		printf -- "$printf_format" "$c${b##refs/heads/}${f:+$z$f}$r$p"
	fi
}
