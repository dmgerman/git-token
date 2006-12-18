begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git branch"  *  * Copyright (c) 2006 Kristian Høgsberg<krh@redhat.com>  * Based on git-branch.sh by Junio C Hamano.  */
end_comment

begin_include
include|#
directive|include
file|"color.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_decl_stmt
DECL|variable|builtin_branch_usage
specifier|static
specifier|const
name|char
name|builtin_branch_usage
index|[]
init|=
literal|"git-branch [-r] (-d | -D)<branchname> | [-l] [-f]<branchname> [<start-point>] | (-m | -M) [<oldbranch>]<newbranch> | [-r | -a] [-v [--abbrev=<length>]]"
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|REF_UNKNOWN_TYPE
define|#
directive|define
name|REF_UNKNOWN_TYPE
value|0x00
end_define

begin_define
DECL|macro|REF_LOCAL_BRANCH
define|#
directive|define
name|REF_LOCAL_BRANCH
value|0x01
end_define

begin_define
DECL|macro|REF_REMOTE_BRANCH
define|#
directive|define
name|REF_REMOTE_BRANCH
value|0x02
end_define

begin_define
DECL|macro|REF_TAG
define|#
directive|define
name|REF_TAG
value|0x04
end_define

begin_decl_stmt
DECL|variable|head
specifier|static
specifier|const
name|char
modifier|*
name|head
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|head_sha1
specifier|static
name|unsigned
name|char
name|head_sha1
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|branch_use_color
specifier|static
name|int
name|branch_use_color
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|branch_colors
specifier|static
name|char
name|branch_colors
index|[]
index|[
name|COLOR_MAXLEN
index|]
init|=
block|{
literal|"\033[m"
block|,
comment|/* reset */
literal|""
block|,
comment|/* PLAIN (normal) */
literal|"\033[31m"
block|,
comment|/* REMOTE (red) */
literal|""
block|,
comment|/* LOCAL (normal) */
literal|"\033[32m"
block|,
comment|/* CURRENT (green) */
block|}
decl_stmt|;
end_decl_stmt

begin_enum
DECL|enum|color_branch
enum|enum
name|color_branch
block|{
DECL|enumerator|COLOR_BRANCH_RESET
name|COLOR_BRANCH_RESET
init|=
literal|0
block|,
DECL|enumerator|COLOR_BRANCH_PLAIN
name|COLOR_BRANCH_PLAIN
init|=
literal|1
block|,
DECL|enumerator|COLOR_BRANCH_REMOTE
name|COLOR_BRANCH_REMOTE
init|=
literal|2
block|,
DECL|enumerator|COLOR_BRANCH_LOCAL
name|COLOR_BRANCH_LOCAL
init|=
literal|3
block|,
DECL|enumerator|COLOR_BRANCH_CURRENT
name|COLOR_BRANCH_CURRENT
init|=
literal|4
block|, }
enum|;
end_enum

begin_function
DECL|function|parse_branch_color_slot
specifier|static
name|int
name|parse_branch_color_slot
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
name|int
name|ofs
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|ofs
argument_list|,
literal|"plain"
argument_list|)
condition|)
return|return
name|COLOR_BRANCH_PLAIN
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|ofs
argument_list|,
literal|"reset"
argument_list|)
condition|)
return|return
name|COLOR_BRANCH_RESET
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|ofs
argument_list|,
literal|"remote"
argument_list|)
condition|)
return|return
name|COLOR_BRANCH_REMOTE
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|ofs
argument_list|,
literal|"local"
argument_list|)
condition|)
return|return
name|COLOR_BRANCH_LOCAL
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|ofs
argument_list|,
literal|"current"
argument_list|)
condition|)
return|return
name|COLOR_BRANCH_CURRENT
return|;
name|die
argument_list|(
literal|"bad config variable '%s'"
argument_list|,
name|var
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_branch_config
name|int
name|git_branch_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"color.branch"
argument_list|)
condition|)
block|{
name|branch_use_color
operator|=
name|git_config_colorbool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|var
argument_list|,
literal|"color.branch."
argument_list|,
literal|13
argument_list|)
condition|)
block|{
name|int
name|slot
init|=
name|parse_branch_color_slot
argument_list|(
name|var
argument_list|,
literal|13
argument_list|)
decl_stmt|;
name|color_parse
argument_list|(
name|value
argument_list|,
name|var
argument_list|,
name|branch_colors
index|[
name|slot
index|]
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|branch_get_color
specifier|const
name|char
modifier|*
name|branch_get_color
parameter_list|(
name|enum
name|color_branch
name|ix
parameter_list|)
block|{
if|if
condition|(
name|branch_use_color
condition|)
return|return
name|branch_colors
index|[
name|ix
index|]
return|;
return|return
literal|""
return|;
block|}
end_function

begin_function
DECL|function|in_merge_bases
specifier|static
name|int
name|in_merge_bases
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|commit
modifier|*
name|rev1
parameter_list|,
name|struct
name|commit
modifier|*
name|rev2
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|bases
decl_stmt|,
modifier|*
name|b
decl_stmt|;
name|int
name|ret
init|=
literal|0
decl_stmt|;
name|bases
operator|=
name|get_merge_bases
argument_list|(
name|rev1
argument_list|,
name|rev2
argument_list|,
literal|1
argument_list|)
expr_stmt|;
for|for
control|(
name|b
operator|=
name|bases
init|;
name|b
condition|;
name|b
operator|=
name|b
operator|->
name|next
control|)
block|{
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|sha1
argument_list|,
name|b
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
block|{
name|ret
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
name|free_commit_list
argument_list|(
name|bases
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|delete_branches
specifier|static
name|int
name|delete_branches
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|int
name|force
parameter_list|,
name|int
name|kinds
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|rev
decl_stmt|,
modifier|*
name|head_rev
init|=
name|head_rev
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|name
init|=
name|NULL
decl_stmt|;
specifier|const
name|char
modifier|*
name|fmt
decl_stmt|,
modifier|*
name|remote
decl_stmt|;
name|int
name|i
decl_stmt|;
name|int
name|ret
init|=
literal|0
decl_stmt|;
switch|switch
condition|(
name|kinds
condition|)
block|{
case|case
name|REF_REMOTE_BRANCH
case|:
name|fmt
operator|=
literal|"refs/remotes/%s"
expr_stmt|;
name|remote
operator|=
literal|"remote "
expr_stmt|;
name|force
operator|=
literal|1
expr_stmt|;
break|break;
case|case
name|REF_LOCAL_BRANCH
case|:
name|fmt
operator|=
literal|"refs/heads/%s"
expr_stmt|;
name|remote
operator|=
literal|""
expr_stmt|;
break|break;
default|default:
name|die
argument_list|(
literal|"cannot use -a with -d"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|force
condition|)
block|{
name|head_rev
operator|=
name|lookup_commit_reference
argument_list|(
name|head_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|head_rev
condition|)
name|die
argument_list|(
literal|"Couldn't look up commit object for HEAD"
argument_list|)
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|kinds
operator|==
name|REF_LOCAL_BRANCH
operator|&&
operator|!
name|strcmp
argument_list|(
name|head
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"Cannot delete the branch '%s' "
literal|"which you are currently on."
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|name
condition|)
name|free
argument_list|(
name|name
argument_list|)
expr_stmt|;
name|name
operator|=
name|xstrdup
argument_list|(
name|mkpath
argument_list|(
name|fmt
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|resolve_ref
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"%sbranch '%s' not found."
argument_list|,
name|remote
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|rev
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|rev
condition|)
block|{
name|error
argument_list|(
literal|"Couldn't look up commit object for '%s'"
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
comment|/* This checks whether the merge bases of branch and 		 * HEAD contains branch -- which means that the HEAD 		 * contains everything in both. 		 */
if|if
condition|(
operator|!
name|force
operator|&&
operator|!
name|in_merge_bases
argument_list|(
name|sha1
argument_list|,
name|rev
argument_list|,
name|head_rev
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"The branch '%s' is not a strict subset of "
literal|"your current HEAD.\n"
literal|"If you are sure you want to delete it, "
literal|"run 'git branch -D %s'."
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|delete_ref
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"Error deleting %sbranch '%s'"
argument_list|,
name|remote
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
block|}
else|else
name|printf
argument_list|(
literal|"Deleted %sbranch %s.\n"
argument_list|,
name|remote
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|name
condition|)
name|free
argument_list|(
name|name
argument_list|)
expr_stmt|;
return|return
operator|(
name|ret
operator|)
return|;
block|}
end_function

begin_struct
DECL|struct|ref_item
struct|struct
name|ref_item
block|{
DECL|member|name
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|kind
name|unsigned
name|int
name|kind
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|ref_list
struct|struct
name|ref_list
block|{
DECL|member|index
DECL|member|alloc
DECL|member|maxwidth
name|int
name|index
decl_stmt|,
name|alloc
decl_stmt|,
name|maxwidth
decl_stmt|;
DECL|member|list
name|struct
name|ref_item
modifier|*
name|list
decl_stmt|;
DECL|member|kinds
name|int
name|kinds
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|append_ref
specifier|static
name|int
name|append_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|flags
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|struct
name|ref_list
modifier|*
name|ref_list
init|=
operator|(
expr|struct
name|ref_list
operator|*
operator|)
operator|(
name|cb_data
operator|)
decl_stmt|;
name|struct
name|ref_item
modifier|*
name|newitem
decl_stmt|;
name|int
name|kind
init|=
name|REF_UNKNOWN_TYPE
decl_stmt|;
name|int
name|len
decl_stmt|;
comment|/* Detect kind */
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|refname
argument_list|,
literal|"refs/heads/"
argument_list|,
literal|11
argument_list|)
condition|)
block|{
name|kind
operator|=
name|REF_LOCAL_BRANCH
expr_stmt|;
name|refname
operator|+=
literal|11
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|refname
argument_list|,
literal|"refs/remotes/"
argument_list|,
literal|13
argument_list|)
condition|)
block|{
name|kind
operator|=
name|REF_REMOTE_BRANCH
expr_stmt|;
name|refname
operator|+=
literal|13
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|refname
argument_list|,
literal|"refs/tags/"
argument_list|,
literal|10
argument_list|)
condition|)
block|{
name|kind
operator|=
name|REF_TAG
expr_stmt|;
name|refname
operator|+=
literal|10
expr_stmt|;
block|}
comment|/* Don't add types the caller doesn't want */
if|if
condition|(
operator|(
name|kind
operator|&
name|ref_list
operator|->
name|kinds
operator|)
operator|==
literal|0
condition|)
return|return
literal|0
return|;
comment|/* Resize buffer */
if|if
condition|(
name|ref_list
operator|->
name|index
operator|>=
name|ref_list
operator|->
name|alloc
condition|)
block|{
name|ref_list
operator|->
name|alloc
operator|=
name|alloc_nr
argument_list|(
name|ref_list
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|ref_list
operator|->
name|list
operator|=
name|xrealloc
argument_list|(
name|ref_list
operator|->
name|list
argument_list|,
name|ref_list
operator|->
name|alloc
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|ref_item
argument_list|)
argument_list|)
expr_stmt|;
block|}
comment|/* Record the new item */
name|newitem
operator|=
operator|&
operator|(
name|ref_list
operator|->
name|list
index|[
name|ref_list
operator|->
name|index
operator|++
index|]
operator|)
expr_stmt|;
name|newitem
operator|->
name|name
operator|=
name|xstrdup
argument_list|(
name|refname
argument_list|)
expr_stmt|;
name|newitem
operator|->
name|kind
operator|=
name|kind
expr_stmt|;
name|hashcpy
argument_list|(
name|newitem
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|newitem
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>
name|ref_list
operator|->
name|maxwidth
condition|)
name|ref_list
operator|->
name|maxwidth
operator|=
name|len
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|free_ref_list
specifier|static
name|void
name|free_ref_list
parameter_list|(
name|struct
name|ref_list
modifier|*
name|ref_list
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ref_list
operator|->
name|index
condition|;
name|i
operator|++
control|)
name|free
argument_list|(
name|ref_list
operator|->
name|list
index|[
name|i
index|]
operator|.
name|name
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|ref_list
operator|->
name|list
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ref_cmp
specifier|static
name|int
name|ref_cmp
parameter_list|(
specifier|const
name|void
modifier|*
name|r1
parameter_list|,
specifier|const
name|void
modifier|*
name|r2
parameter_list|)
block|{
name|struct
name|ref_item
modifier|*
name|c1
init|=
operator|(
expr|struct
name|ref_item
operator|*
operator|)
operator|(
name|r1
operator|)
decl_stmt|;
name|struct
name|ref_item
modifier|*
name|c2
init|=
operator|(
expr|struct
name|ref_item
operator|*
operator|)
operator|(
name|r2
operator|)
decl_stmt|;
if|if
condition|(
name|c1
operator|->
name|kind
operator|!=
name|c2
operator|->
name|kind
condition|)
return|return
name|c1
operator|->
name|kind
operator|-
name|c2
operator|->
name|kind
return|;
return|return
name|strcmp
argument_list|(
name|c1
operator|->
name|name
argument_list|,
name|c2
operator|->
name|name
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|print_ref_info
specifier|static
name|void
name|print_ref_info
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|abbrev
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|char
name|subject
index|[
literal|256
index|]
decl_stmt|;
name|commit
operator|=
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit
operator|&&
operator|!
name|parse_commit
argument_list|(
name|commit
argument_list|)
condition|)
name|pretty_print_commit
argument_list|(
name|CMIT_FMT_ONELINE
argument_list|,
name|commit
argument_list|,
operator|~
literal|0
argument_list|,
name|subject
argument_list|,
sizeof|sizeof
argument_list|(
name|subject
argument_list|)
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
else|else
name|strcpy
argument_list|(
name|subject
argument_list|,
literal|" **** invalid ref ****"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|" %s %s\n"
argument_list|,
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|abbrev
argument_list|)
argument_list|,
name|subject
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|print_ref_list
specifier|static
name|void
name|print_ref_list
parameter_list|(
name|int
name|kinds
parameter_list|,
name|int
name|verbose
parameter_list|,
name|int
name|abbrev
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|char
name|c
decl_stmt|;
name|struct
name|ref_list
name|ref_list
decl_stmt|;
name|int
name|color
decl_stmt|;
name|memset
argument_list|(
operator|&
name|ref_list
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|ref_list
argument_list|)
argument_list|)
expr_stmt|;
name|ref_list
operator|.
name|kinds
operator|=
name|kinds
expr_stmt|;
name|for_each_ref
argument_list|(
name|append_ref
argument_list|,
operator|&
name|ref_list
argument_list|)
expr_stmt|;
name|qsort
argument_list|(
name|ref_list
operator|.
name|list
argument_list|,
name|ref_list
operator|.
name|index
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|ref_item
argument_list|)
argument_list|,
name|ref_cmp
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ref_list
operator|.
name|index
condition|;
name|i
operator|++
control|)
block|{
switch|switch
condition|(
name|ref_list
operator|.
name|list
index|[
name|i
index|]
operator|.
name|kind
condition|)
block|{
case|case
name|REF_LOCAL_BRANCH
case|:
name|color
operator|=
name|COLOR_BRANCH_LOCAL
expr_stmt|;
break|break;
case|case
name|REF_REMOTE_BRANCH
case|:
name|color
operator|=
name|COLOR_BRANCH_REMOTE
expr_stmt|;
break|break;
default|default:
name|color
operator|=
name|COLOR_BRANCH_PLAIN
expr_stmt|;
break|break;
block|}
name|c
operator|=
literal|' '
expr_stmt|;
if|if
condition|(
name|ref_list
operator|.
name|list
index|[
name|i
index|]
operator|.
name|kind
operator|==
name|REF_LOCAL_BRANCH
operator|&&
operator|!
name|strcmp
argument_list|(
name|ref_list
operator|.
name|list
index|[
name|i
index|]
operator|.
name|name
argument_list|,
name|head
argument_list|)
condition|)
block|{
name|c
operator|=
literal|'*'
expr_stmt|;
name|color
operator|=
name|COLOR_BRANCH_CURRENT
expr_stmt|;
block|}
if|if
condition|(
name|verbose
condition|)
block|{
name|printf
argument_list|(
literal|"%c %s%-*s%s"
argument_list|,
name|c
argument_list|,
name|branch_get_color
argument_list|(
name|color
argument_list|)
argument_list|,
name|ref_list
operator|.
name|maxwidth
argument_list|,
name|ref_list
operator|.
name|list
index|[
name|i
index|]
operator|.
name|name
argument_list|,
name|branch_get_color
argument_list|(
name|COLOR_BRANCH_RESET
argument_list|)
argument_list|)
expr_stmt|;
name|print_ref_info
argument_list|(
name|ref_list
operator|.
name|list
index|[
name|i
index|]
operator|.
name|sha1
argument_list|,
name|abbrev
argument_list|)
expr_stmt|;
block|}
else|else
name|printf
argument_list|(
literal|"%c %s%s%s\n"
argument_list|,
name|c
argument_list|,
name|branch_get_color
argument_list|(
name|color
argument_list|)
argument_list|,
name|ref_list
operator|.
name|list
index|[
name|i
index|]
operator|.
name|name
argument_list|,
name|branch_get_color
argument_list|(
name|COLOR_BRANCH_RESET
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|free_ref_list
argument_list|(
operator|&
name|ref_list
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|create_branch
specifier|static
name|void
name|create_branch
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|start
parameter_list|,
name|int
name|force
parameter_list|,
name|int
name|reflog
parameter_list|)
block|{
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
name|ref
index|[
name|PATH_MAX
index|]
decl_stmt|,
name|msg
index|[
name|PATH_MAX
operator|+
literal|20
index|]
decl_stmt|;
name|snprintf
argument_list|(
name|ref
argument_list|,
sizeof|sizeof
name|ref
argument_list|,
literal|"refs/heads/%s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|check_ref_format
argument_list|(
name|ref
argument_list|)
condition|)
name|die
argument_list|(
literal|"'%s' is not a valid branch name."
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|resolve_ref
argument_list|(
name|ref
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|force
condition|)
name|die
argument_list|(
literal|"A branch named '%s' already exists."
argument_list|,
name|name
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|head
argument_list|,
name|name
argument_list|)
condition|)
name|die
argument_list|(
literal|"Cannot force update the current branch."
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|get_sha1
argument_list|(
name|start
argument_list|,
name|sha1
argument_list|)
operator|||
operator|(
name|commit
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1
argument_list|)
operator|)
operator|==
name|NULL
condition|)
name|die
argument_list|(
literal|"Not a valid branch point: '%s'."
argument_list|,
name|start
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|sha1
argument_list|,
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
expr_stmt|;
name|lock
operator|=
name|lock_any_ref_for_update
argument_list|(
name|ref
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lock
condition|)
name|die
argument_list|(
literal|"Failed to lock ref for update: %s."
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|reflog
condition|)
block|{
name|log_all_ref_updates
operator|=
literal|1
expr_stmt|;
name|snprintf
argument_list|(
name|msg
argument_list|,
sizeof|sizeof
name|msg
argument_list|,
literal|"branch: Created from %s"
argument_list|,
name|start
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|write_ref_sha1
argument_list|(
name|lock
argument_list|,
name|sha1
argument_list|,
name|msg
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Failed to write ref: %s."
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|rename_branch
specifier|static
name|void
name|rename_branch
parameter_list|(
specifier|const
name|char
modifier|*
name|oldname
parameter_list|,
specifier|const
name|char
modifier|*
name|newname
parameter_list|,
name|int
name|force
parameter_list|)
block|{
name|char
name|oldref
index|[
name|PATH_MAX
index|]
decl_stmt|,
name|newref
index|[
name|PATH_MAX
index|]
decl_stmt|,
name|logmsg
index|[
name|PATH_MAX
operator|*
literal|2
operator|+
literal|100
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|snprintf
argument_list|(
name|oldref
argument_list|,
sizeof|sizeof
argument_list|(
name|oldref
argument_list|)
argument_list|,
literal|"refs/heads/%s"
argument_list|,
name|oldname
argument_list|)
operator|>
sizeof|sizeof
argument_list|(
name|oldref
argument_list|)
condition|)
name|die
argument_list|(
literal|"Old branchname too long"
argument_list|)
expr_stmt|;
if|if
condition|(
name|check_ref_format
argument_list|(
name|oldref
argument_list|)
condition|)
name|die
argument_list|(
literal|"Invalid branch name: %s"
argument_list|,
name|oldref
argument_list|)
expr_stmt|;
if|if
condition|(
name|snprintf
argument_list|(
name|newref
argument_list|,
sizeof|sizeof
argument_list|(
name|newref
argument_list|)
argument_list|,
literal|"refs/heads/%s"
argument_list|,
name|newname
argument_list|)
operator|>
sizeof|sizeof
argument_list|(
name|newref
argument_list|)
condition|)
name|die
argument_list|(
literal|"New branchname too long"
argument_list|)
expr_stmt|;
if|if
condition|(
name|check_ref_format
argument_list|(
name|newref
argument_list|)
condition|)
name|die
argument_list|(
literal|"Invalid branch name: %s"
argument_list|,
name|newref
argument_list|)
expr_stmt|;
if|if
condition|(
name|resolve_ref
argument_list|(
name|newref
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
operator|&&
operator|!
name|force
condition|)
name|die
argument_list|(
literal|"A branch named '%s' already exists."
argument_list|,
name|newname
argument_list|)
expr_stmt|;
name|snprintf
argument_list|(
name|logmsg
argument_list|,
sizeof|sizeof
argument_list|(
name|logmsg
argument_list|)
argument_list|,
literal|"Branch: renamed %s to %s"
argument_list|,
name|oldref
argument_list|,
name|newref
argument_list|)
expr_stmt|;
if|if
condition|(
name|rename_ref
argument_list|(
name|oldref
argument_list|,
name|newref
argument_list|,
name|logmsg
argument_list|)
condition|)
name|die
argument_list|(
literal|"Branch rename failed"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|oldname
argument_list|,
name|head
argument_list|)
operator|&&
name|create_symref
argument_list|(
literal|"HEAD"
argument_list|,
name|newref
argument_list|)
condition|)
name|die
argument_list|(
literal|"Branch renamed to %s, but HEAD is not updated!"
argument_list|,
name|newname
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_branch
name|int
name|cmd_branch
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|int
name|delete
init|=
literal|0
decl_stmt|,
name|force_delete
init|=
literal|0
decl_stmt|,
name|force_create
init|=
literal|0
decl_stmt|;
name|int
name|rename
init|=
literal|0
decl_stmt|,
name|force_rename
init|=
literal|0
decl_stmt|;
name|int
name|verbose
init|=
literal|0
decl_stmt|,
name|abbrev
init|=
name|DEFAULT_ABBREV
decl_stmt|;
name|int
name|reflog
init|=
literal|0
decl_stmt|;
name|int
name|kinds
init|=
name|REF_LOCAL_BRANCH
decl_stmt|;
name|int
name|i
decl_stmt|;
name|setup_ident
argument_list|()
expr_stmt|;
name|git_config
argument_list|(
name|git_branch_config
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|arg
index|[
literal|0
index|]
operator|!=
literal|'-'
condition|)
break|break;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--"
argument_list|)
condition|)
block|{
name|i
operator|++
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-d"
argument_list|)
condition|)
block|{
name|delete
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-D"
argument_list|)
condition|)
block|{
name|delete
operator|=
literal|1
expr_stmt|;
name|force_delete
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-f"
argument_list|)
condition|)
block|{
name|force_create
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-m"
argument_list|)
condition|)
block|{
name|rename
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-M"
argument_list|)
condition|)
block|{
name|rename
operator|=
literal|1
expr_stmt|;
name|force_rename
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-r"
argument_list|)
condition|)
block|{
name|kinds
operator|=
name|REF_REMOTE_BRANCH
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-a"
argument_list|)
condition|)
block|{
name|kinds
operator|=
name|REF_REMOTE_BRANCH
operator||
name|REF_LOCAL_BRANCH
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-l"
argument_list|)
condition|)
block|{
name|reflog
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--abbrev="
argument_list|,
literal|9
argument_list|)
condition|)
block|{
name|abbrev
operator|=
name|atoi
argument_list|(
name|arg
operator|+
literal|9
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-v"
argument_list|)
condition|)
block|{
name|verbose
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--color"
argument_list|)
condition|)
block|{
name|branch_use_color
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--no-color"
argument_list|)
condition|)
block|{
name|branch_use_color
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|builtin_branch_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|delete
operator|&&
name|rename
operator|)
operator|||
operator|(
name|delete
operator|&&
name|force_create
operator|)
operator|||
operator|(
name|rename
operator|&&
name|force_create
operator|)
condition|)
name|usage
argument_list|(
name|builtin_branch_usage
argument_list|)
expr_stmt|;
name|head
operator|=
name|xstrdup
argument_list|(
name|resolve_ref
argument_list|(
literal|"HEAD"
argument_list|,
name|head_sha1
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|head
condition|)
name|die
argument_list|(
literal|"Failed to resolve HEAD as a valid ref."
argument_list|)
expr_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
name|head
argument_list|,
literal|"refs/heads/"
argument_list|,
literal|11
argument_list|)
condition|)
name|die
argument_list|(
literal|"HEAD not found below refs/heads!"
argument_list|)
expr_stmt|;
name|head
operator|+=
literal|11
expr_stmt|;
if|if
condition|(
name|delete
condition|)
return|return
name|delete_branches
argument_list|(
name|argc
operator|-
name|i
argument_list|,
name|argv
operator|+
name|i
argument_list|,
name|force_delete
argument_list|,
name|kinds
argument_list|)
return|;
elseif|else
if|if
condition|(
name|i
operator|==
name|argc
condition|)
name|print_ref_list
argument_list|(
name|kinds
argument_list|,
name|verbose
argument_list|,
name|abbrev
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|rename
operator|&&
operator|(
name|i
operator|==
name|argc
operator|-
literal|1
operator|)
condition|)
name|rename_branch
argument_list|(
name|head
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|force_rename
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|rename
operator|&&
operator|(
name|i
operator|==
name|argc
operator|-
literal|2
operator|)
condition|)
name|rename_branch
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|argv
index|[
name|i
operator|+
literal|1
index|]
argument_list|,
name|force_rename
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|i
operator|==
name|argc
operator|-
literal|1
condition|)
name|create_branch
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|head
argument_list|,
name|force_create
argument_list|,
name|reflog
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|i
operator|==
name|argc
operator|-
literal|2
condition|)
name|create_branch
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|argv
index|[
name|i
operator|+
literal|1
index|]
argument_list|,
name|force_create
argument_list|,
name|reflog
argument_list|)
expr_stmt|;
else|else
name|usage
argument_list|(
name|builtin_branch_usage
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

