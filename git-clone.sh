#!/bin/sh
#
# Copyright (c) 2005, Linus Torvalds
# Copyright (c) 2005, Junio C Hamano
# 
# Clone a repository into a different directory that does not yet exist.

# See git-sh-setup why.
unset CDPATH

usage() {
	echo >&2 "Usage: $0 [--template=<template_directory>] [--use-separate-remote] [--reference <reference-repo>] [--bare] [-l [-s]] [-q] [-u <upload-pack>] [--origin <name>] [-n] <repo> [<dir>]"
	exit 1
}

get_repo_base() {
	(cd "$1" && (cd .git ; pwd)) 2> /dev/null
}

if [ -n "$GIT_SSL_NO_VERIFY" ]; then
    curl_extra_args="-k"
fi

http_fetch () {
	# $1 = Remote, $2 = Local
	curl -nsfL $curl_extra_args "$1" >"$2"
}

clone_dumb_http () {
	# $1 - remote, $2 - local
	cd "$2" &&
	clone_tmp="$GIT_DIR/clone-tmp" &&
	mkdir -p "$clone_tmp" || exit 1
	if [ -n "$GIT_CURL_FTP_NO_EPSV" -o \
		"`git-repo-config --bool http.noEPSV`" = true ]; then
		curl_extra_args="${curl_extra_args} --disable-epsv"
	fi
	http_fetch "$1/info/refs" "$clone_tmp/refs" || {
		echo >&2 "Cannot get remote repository information.
Perhaps git-update-server-info needs to be run there?"
		exit 1;
	}
	while read sha1 refname
	do
		name=`expr "z$refname" : 'zrefs/\(.*\)'` &&
		case "$name" in
		*^*)	continue;;
		esac
		if test -n "$use_separate_remote" &&
		   branch_name=`expr "z$name" : 'zheads/\(.*\)'`
		then
			tname="remotes/$origin/$branch_name"
		else
			tname=$name
		fi
		git-http-fetch -v -a -w "$tname" "$name" "$1/" || exit 1
	done <"$clone_tmp/refs"
	rm -fr "$clone_tmp"
	http_fetch "$1/HEAD" "$GIT_DIR/REMOTE_HEAD" ||
	rm -f "$GIT_DIR/REMOTE_HEAD"
}

# Read git-fetch-pack -k output and store the remote branches.
copy_refs='
use File::Path qw(mkpath);
use File::Basename qw(dirname);
my $git_dir = $ARGV[0];
my $use_separate_remote = $ARGV[1];
my $origin = $ARGV[2];

my $branch_top = ($use_separate_remote ? "remotes/$origin" : "heads");
my $tag_top = "tags";

sub store {
	my ($sha1, $name, $top) = @_;
	$name = "$git_dir/refs/$top/$name";
	mkpath(dirname($name));
	open O, ">", "$name";
	print O "$sha1\n";
	close O;
}

open FH, "<", "$git_dir/CLONE_HEAD";
while (<FH>) {
	my ($sha1, $name) = /^([0-9a-f]{40})\s(.*)$/;
	next if ($name =~ /\^\173/);
	if ($name eq "HEAD") {
		open O, ">", "$git_dir/REMOTE_HEAD";
		print O "$sha1\n";
		close O;
		next;
	}
	if ($name =~ s/^refs\/heads\///) {
		store($sha1, $name, $branch_top);
		next;
	}
	if ($name =~ s/^refs\/tags\///) {
		store($sha1, $name, $tag_top);
		next;
	}
}
close FH;
'

quiet=
local=no
use_local=no
local_shared=no
unset template
no_checkout=
upload_pack=
bare=
reference=
origin=
origin_override=
use_separate_remote=
while
	case "$#,$1" in
	0,*) break ;;
	*,-n|*,--no|*,--no-|*,--no-c|*,--no-ch|*,--no-che|*,--no-chec|\
	*,--no-check|*,--no-checko|*,--no-checkou|*,--no-checkout)
	  no_checkout=yes ;;
	*,--na|*,--nak|*,--nake|*,--naked|\
	*,-b|*,--b|*,--ba|*,--bar|*,--bare) bare=yes ;;
	*,-l|*,--l|*,--lo|*,--loc|*,--loca|*,--local) use_local=yes ;;
        *,-s|*,--s|*,--sh|*,--sha|*,--shar|*,--share|*,--shared) 
          local_shared=yes; use_local=yes ;;
	1,--template) usage ;;
	*,--template)
		shift; template="--template=$1" ;;
	*,--template=*)
	  template="$1" ;;
	*,-q|*,--quiet) quiet=-q ;;
	*,--use-separate-remote)
		use_separate_remote=t ;;
	1,--reference) usage ;;
	*,--reference)
		shift; reference="$1" ;;
	*,--reference=*)
		reference=`expr "z$1" : 'z--reference=\(.*\)'` ;;
	*,-o|*,--or|*,--ori|*,--orig|*,--origi|*,--origin)
		case "$2" in
		'')
		    usage ;;
		*/*)
		    echo >&2 "'$2' is not suitable for an origin name"
		    exit 1
		esac
		git-check-ref-format "heads/$2" || {
		    echo >&2 "'$2' is not suitable for a branch name"
		    exit 1
		}
		test -z "$origin_override" || {
		    echo >&2 "Do not give more than one --origin options."
		    exit 1
		}
		origin_override=yes
		origin="$2"; shift
		;;
	1,-u|1,--upload-pack) usage ;;
	*,-u|*,--upload-pack)
		shift
		upload_pack="--exec=$1" ;;
	*,-*) usage ;;
	*) break ;;
	esac
do
	shift
done

repo="$1"
if test -z "$repo"
then
    echo >&2 'you must specify a repository to clone.'
    exit 1
fi

# --bare implies --no-checkout
if test yes = "$bare"
then
	if test yes = "$origin_override"
	then
		echo >&2 '--bare and --origin $origin options are incompatible.'
		exit 1
	fi
	if test t = "$use_separate_remote"
	then
		echo >&2 '--bare and --use-separate-remote options are incompatible.'
		exit 1
	fi
	no_checkout=yes
fi

if test -z "$origin"
then
	origin=origin
fi

# Turn the source into an absolute path if
# it is local
if base=$(get_repo_base "$repo"); then
	repo="$base"
	local=yes
fi

dir="$2"
# Try using "humanish" part of source repo if user didn't specify one
[ -z "$dir" ] && dir=$(echo "$repo" | sed -e 's|/$||' -e 's|:*/*\.git$||' -e 's|.*[/:]||g')
[ -e "$dir" ] && echo "$dir already exists." && usage
mkdir -p "$dir" &&
D=$(cd "$dir" && pwd) &&
trap 'err=$?; cd ..; rm -rf "$D"; exit $err' 0
case "$bare" in
yes)
	GIT_DIR="$D" ;;
*)
	GIT_DIR="$D/.git" ;;
esac && export GIT_DIR && git-init-db ${template+"$template"} || usage

if test -n "$reference"
then
	if test -d "$reference"
	then
		if test -d "$reference/.git/objects"
		then
			reference="$reference/.git"
		fi
		reference=$(cd "$reference" && pwd)
		echo "$reference/objects" >"$GIT_DIR/objects/info/alternates"
		(cd "$reference" && tar cf - refs) |
		(cd "$GIT_DIR/refs" &&
		 mkdir reference-tmp &&
		 cd reference-tmp &&
		 tar xf -)
	else
		echo >&2 "$reference: not a local directory." && usage
	fi
fi

rm -f "$GIT_DIR/CLONE_HEAD"

# We do local magic only when the user tells us to.
case "$local,$use_local" in
yes,yes)
	( cd "$repo/objects" ) || {
		echo >&2 "-l flag seen but $repo is not local."
		exit 1
	}

	case "$local_shared" in
	no)
	    # See if we can hardlink and drop "l" if not.
	    sample_file=$(cd "$repo" && \
			  find objects -type f -print | sed -e 1q)

	    # objects directory should not be empty since we are cloning!
	    test -f "$repo/$sample_file" || exit

	    l=
	    if ln "$repo/$sample_file" "$GIT_DIR/objects/sample" 2>/dev/null
	    then
		    l=l
	    fi &&
	    rm -f "$GIT_DIR/objects/sample" &&
	    cd "$repo" &&
	    find objects -depth -print | cpio -pumd$l "$GIT_DIR/" || exit 1
	    ;;
	yes)
	    mkdir -p "$GIT_DIR/objects/info"
	    echo "$repo/objects" >> "$GIT_DIR/objects/info/alternates"
	    ;;
	esac
	git-ls-remote "$repo" >"$GIT_DIR/CLONE_HEAD" || exit 1
	;;
*)
	case "$repo" in
	rsync://*)
		rsync $quiet -av --ignore-existing  \
			--exclude info "$repo/objects/" "$GIT_DIR/objects/" ||
		exit
		# Look at objects/info/alternates for rsync -- http will
		# support it natively and git native ones will do it on the
		# remote end.  Not having that file is not a crime.
		rsync -q "$repo/objects/info/alternates" \
			"$GIT_DIR/TMP_ALT" 2>/dev/null ||
			rm -f "$GIT_DIR/TMP_ALT"
		if test -f "$GIT_DIR/TMP_ALT"
		then
		    ( cd "$D" &&
		      . git-parse-remote &&
		      resolve_alternates "$repo" <"$GIT_DIR/TMP_ALT" ) |
		    while read alt
		    do
			case "$alt" in 'bad alternate: '*) die "$alt";; esac
			case "$quiet" in
			'')	echo >&2 "Getting alternate: $alt" ;;
			esac
			rsync $quiet -av --ignore-existing  \
			    --exclude info "$alt" "$GIT_DIR/objects" || exit
		    done
		    rm -f "$GIT_DIR/TMP_ALT"
		fi
		git-ls-remote "$repo" >"$GIT_DIR/CLONE_HEAD" || exit 1
		;;
	https://*|http://*|ftp://*)
		if test -z "@@NO_CURL@@"
		then
			clone_dumb_http "$repo" "$D"
		else
			echo >&2 "http transport not supported, rebuild Git with curl support"
			exit 1
		fi
		;;
	*)
		case "$upload_pack" in
		'') git-fetch-pack --all -k $quiet "$repo" ;;
		*) git-fetch-pack --all -k $quiet "$upload_pack" "$repo" ;;
		esac >"$GIT_DIR/CLONE_HEAD" || {
			echo >&2 "fetch-pack from '$repo' failed."
			exit 1
		}
		;;
	esac
	;;
esac
test -d "$GIT_DIR/refs/reference-tmp" && rm -fr "$GIT_DIR/refs/reference-tmp"

if test -f "$GIT_DIR/CLONE_HEAD"
then
	# Read git-fetch-pack -k output and store the remote branches.
	@@PERL@@ -e "$copy_refs" "$GIT_DIR" "$use_separate_remote" "$origin" ||
	exit
fi

cd "$D" || exit

if test -z "$bare" && test -f "$GIT_DIR/REMOTE_HEAD"
then
	# Figure out which remote branch HEAD points at.
	case "$use_separate_remote" in
	'')	remote_top=refs/heads ;;
	*)	remote_top="refs/remotes/$origin" ;;
	esac

	head_sha1=`cat "$GIT_DIR/REMOTE_HEAD"`
	case "$head_sha1" in
	'ref: refs/'*)
		# Uh-oh, the remote told us (http transport done against
		# new style repository with a symref HEAD).
		# Ideally we should skip the guesswork but for now
		# opt for minimum change.
		head_sha1=`expr "z$head_sha1" : 'zref: refs/heads/\(.*\)'`
		head_sha1=`cat "$GIT_DIR/$remote_top/$head_sha1"`
		;;
	esac

	# The name under $remote_top the remote HEAD seems to point at.
	head_points_at=$(
		(
			echo "master"
			cd "$GIT_DIR/$remote_top" &&
			find . -type f -print | sed -e 's/^\.\///'
		) | (
		done=f
		while read name
		do
			test t = $done && continue
			branch_tip=`cat "$GIT_DIR/$remote_top/$name"`
			if test "$head_sha1" = "$branch_tip"
			then
				echo "$name"
				done=t
			fi
		done
		)
	)

	# Write out remotes/$origin file, and update our "$head_points_at".
	case "$head_points_at" in
	?*)
		mkdir -p "$GIT_DIR/remotes" &&
		git-symbolic-ref HEAD "refs/heads/$head_points_at" &&
		case "$use_separate_remote" in
		t)	origin_track="$remote_top/$head_points_at"
			git-update-ref HEAD "$head_sha1" ;;
		*)	origin_track="$remote_top/$origin"
			git-update-ref "refs/heads/$origin" "$head_sha1" ;;
		esac &&
		echo >"$GIT_DIR/remotes/$origin" \
		"URL: $repo
Pull: refs/heads/$head_points_at:$origin_track" &&
		(cd "$GIT_DIR/$remote_top" && find . -type f -print) |
		while read dotslref
		do
			name=`expr "$dotslref" : './\(.*\)'`
			if test "z$head_points_at" = "z$name"
			then
				continue
			fi
			if test "$use_separate_remote" = '' &&
			   test "z$origin" = "z$name"
			then
				continue
			fi
			echo "Pull: refs/heads/${name}:$remote_top/${name}"
		done >>"$GIT_DIR/remotes/$origin" &&
		case "$use_separate_remote" in
		t)
			rm -f "refs/remotes/$origin/HEAD"
			git-symbolic-ref "refs/remotes/$origin/HEAD" \
				"refs/remotes/$origin/$head_points_at"
		esac
	esac

	case "$no_checkout" in
	'')
		git-read-tree -m -u -v HEAD HEAD
	esac
fi
rm -f "$GIT_DIR/CLONE_HEAD" "$GIT_DIR/REMOTE_HEAD"

trap - 0

