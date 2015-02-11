begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_function
DECL|function|show_merge_base
specifier|static
name|int
name|show_merge_base
parameter_list|(
name|struct
name|commit
modifier|*
modifier|*
name|rev
parameter_list|,
name|int
name|rev_nr
parameter_list|,
name|int
name|show_all
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|result
decl_stmt|;
name|result
operator|=
name|get_merge_bases_many_dirty
argument_list|(
name|rev
index|[
literal|0
index|]
argument_list|,
name|rev_nr
operator|-
literal|1
argument_list|,
name|rev
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|result
condition|)
return|return
literal|1
return|;
while|while
condition|(
name|result
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|result
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|show_all
condition|)
return|return
literal|0
return|;
name|result
operator|=
name|result
operator|->
name|next
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|merge_base_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|merge_base_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git merge-base [-a | --all]<commit><commit>..."
argument_list|)
block|,
name|N_
argument_list|(
literal|"git merge-base [-a | --all] --octopus<commit>..."
argument_list|)
block|,
name|N_
argument_list|(
literal|"git merge-base --independent<commit>..."
argument_list|)
block|,
name|N_
argument_list|(
literal|"git merge-base --is-ancestor<commit><commit>"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git merge-base --fork-point<ref> [<commit>]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|get_commit_reference
specifier|static
name|struct
name|commit
modifier|*
name|get_commit_reference
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|)
block|{
name|unsigned
name|char
name|revkey
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|commit
modifier|*
name|r
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|arg
argument_list|,
name|revkey
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|r
operator|=
name|lookup_commit_reference
argument_list|(
name|revkey
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|r
condition|)
name|die
argument_list|(
literal|"Not a valid commit name %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
return|return
name|r
return|;
block|}
end_function

begin_function
DECL|function|handle_independent
specifier|static
name|int
name|handle_independent
parameter_list|(
name|int
name|count
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|args
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|revs
init|=
name|NULL
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|result
decl_stmt|;
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
name|count
operator|-
literal|1
init|;
name|i
operator|>=
literal|0
condition|;
name|i
operator|--
control|)
name|commit_list_insert
argument_list|(
name|get_commit_reference
argument_list|(
name|args
index|[
name|i
index|]
argument_list|)
argument_list|,
operator|&
name|revs
argument_list|)
expr_stmt|;
name|result
operator|=
name|reduce_heads
argument_list|(
name|revs
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|result
condition|)
return|return
literal|1
return|;
while|while
condition|(
name|result
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|result
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|result
operator|=
name|result
operator|->
name|next
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|handle_octopus
specifier|static
name|int
name|handle_octopus
parameter_list|(
name|int
name|count
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|args
parameter_list|,
name|int
name|show_all
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|revs
init|=
name|NULL
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|result
decl_stmt|;
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
name|count
operator|-
literal|1
init|;
name|i
operator|>=
literal|0
condition|;
name|i
operator|--
control|)
name|commit_list_insert
argument_list|(
name|get_commit_reference
argument_list|(
name|args
index|[
name|i
index|]
argument_list|)
argument_list|,
operator|&
name|revs
argument_list|)
expr_stmt|;
name|result
operator|=
name|reduce_heads
argument_list|(
name|get_octopus_merge_bases
argument_list|(
name|revs
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|result
condition|)
return|return
literal|1
return|;
while|while
condition|(
name|result
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|result
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|show_all
condition|)
return|return
literal|0
return|;
name|result
operator|=
name|result
operator|->
name|next
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|handle_is_ancestor
specifier|static
name|int
name|handle_is_ancestor
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|one
decl_stmt|,
modifier|*
name|two
decl_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|2
condition|)
name|die
argument_list|(
literal|"--is-ancestor takes exactly two commits"
argument_list|)
expr_stmt|;
name|one
operator|=
name|get_commit_reference
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|two
operator|=
name|get_commit_reference
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|in_merge_bases
argument_list|(
name|one
argument_list|,
name|two
argument_list|)
condition|)
return|return
literal|0
return|;
else|else
return|return
literal|1
return|;
block|}
end_function

begin_struct
DECL|struct|rev_collect
struct|struct
name|rev_collect
block|{
DECL|member|commit
name|struct
name|commit
modifier|*
modifier|*
name|commit
decl_stmt|;
DECL|member|nr
name|int
name|nr
decl_stmt|;
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|initial
name|unsigned
name|int
name|initial
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|add_one_commit
specifier|static
name|void
name|add_one_commit
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|rev_collect
modifier|*
name|revs
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
if|if
condition|(
name|is_null_sha1
argument_list|(
name|sha1
argument_list|)
condition|)
return|return;
name|commit
operator|=
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
operator|||
operator|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|TMP_MARK
operator|)
operator|||
name|parse_commit
argument_list|(
name|commit
argument_list|)
condition|)
return|return;
name|ALLOC_GROW
argument_list|(
name|revs
operator|->
name|commit
argument_list|,
name|revs
operator|->
name|nr
operator|+
literal|1
argument_list|,
name|revs
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|revs
operator|->
name|commit
index|[
name|revs
operator|->
name|nr
operator|++
index|]
operator|=
name|commit
expr_stmt|;
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|TMP_MARK
expr_stmt|;
block|}
end_function

begin_function
DECL|function|collect_one_reflog_ent
specifier|static
name|int
name|collect_one_reflog_ent
parameter_list|(
name|unsigned
name|char
modifier|*
name|osha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|nsha1
parameter_list|,
specifier|const
name|char
modifier|*
name|ident
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|,
name|int
name|tz
parameter_list|,
specifier|const
name|char
modifier|*
name|message
parameter_list|,
name|void
modifier|*
name|cbdata
parameter_list|)
block|{
name|struct
name|rev_collect
modifier|*
name|revs
init|=
name|cbdata
decl_stmt|;
if|if
condition|(
name|revs
operator|->
name|initial
condition|)
block|{
name|revs
operator|->
name|initial
operator|=
literal|0
expr_stmt|;
name|add_one_commit
argument_list|(
name|osha1
argument_list|,
name|revs
argument_list|)
expr_stmt|;
block|}
name|add_one_commit
argument_list|(
name|nsha1
argument_list|,
name|revs
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|handle_fork_point
specifier|static
name|int
name|handle_fork_point
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|refname
decl_stmt|;
specifier|const
name|char
modifier|*
name|commitname
decl_stmt|;
name|struct
name|rev_collect
name|revs
decl_stmt|;
name|struct
name|commit
modifier|*
name|derived
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|bases
decl_stmt|;
name|int
name|i
decl_stmt|,
name|ret
init|=
literal|0
decl_stmt|;
switch|switch
condition|(
name|dwim_ref
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
name|strlen
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|)
argument_list|,
name|sha1
argument_list|,
operator|&
name|refname
argument_list|)
condition|)
block|{
case|case
literal|0
case|:
name|die
argument_list|(
literal|"No such ref: '%s'"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
case|case
literal|1
case|:
break|break;
comment|/* good */
default|default:
name|die
argument_list|(
literal|"Ambiguous refname: '%s'"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
block|}
name|commitname
operator|=
operator|(
name|argc
operator|==
literal|2
operator|)
condition|?
name|argv
index|[
literal|1
index|]
else|:
literal|"HEAD"
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|commitname
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name: '%s'"
argument_list|,
name|commitname
argument_list|)
expr_stmt|;
name|derived
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|revs
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|revs
argument_list|)
argument_list|)
expr_stmt|;
name|revs
operator|.
name|initial
operator|=
literal|1
expr_stmt|;
name|for_each_reflog_ent
argument_list|(
name|refname
argument_list|,
name|collect_one_reflog_ent
argument_list|,
operator|&
name|revs
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
name|revs
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|revs
operator|.
name|commit
index|[
name|i
index|]
operator|->
name|object
operator|.
name|flags
operator|&=
operator|~
name|TMP_MARK
expr_stmt|;
name|bases
operator|=
name|get_merge_bases_many_dirty
argument_list|(
name|derived
argument_list|,
name|revs
operator|.
name|nr
argument_list|,
name|revs
operator|.
name|commit
argument_list|)
expr_stmt|;
comment|/* 	 * There should be one and only one merge base, when we found 	 * a common ancestor among reflog entries. 	 */
if|if
condition|(
operator|!
name|bases
operator|||
name|bases
operator|->
name|next
condition|)
block|{
name|ret
operator|=
literal|1
expr_stmt|;
goto|goto
name|cleanup_return
goto|;
block|}
comment|/* And the found one must be one of the reflog entries */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|revs
operator|.
name|nr
condition|;
name|i
operator|++
control|)
if|if
condition|(
operator|&
name|bases
operator|->
name|item
operator|->
name|object
operator|==
operator|&
name|revs
operator|.
name|commit
index|[
name|i
index|]
operator|->
name|object
condition|)
break|break;
comment|/* found */
if|if
condition|(
name|revs
operator|.
name|nr
operator|<=
name|i
condition|)
block|{
name|ret
operator|=
literal|1
expr_stmt|;
comment|/* not found */
goto|goto
name|cleanup_return
goto|;
block|}
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|bases
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|cleanup_return
label|:
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
DECL|function|cmd_merge_base
name|int
name|cmd_merge_base
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
name|struct
name|commit
modifier|*
modifier|*
name|rev
decl_stmt|;
name|int
name|rev_nr
init|=
literal|0
decl_stmt|;
name|int
name|show_all
init|=
literal|0
decl_stmt|;
name|int
name|cmdmode
init|=
literal|0
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_BOOL
argument_list|(
literal|'a'
argument_list|,
literal|"all"
argument_list|,
operator|&
name|show_all
argument_list|,
name|N_
argument_list|(
literal|"output all common ancestors"
argument_list|)
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|0
argument_list|,
literal|"octopus"
argument_list|,
operator|&
name|cmdmode
argument_list|,
name|N_
argument_list|(
literal|"find ancestors for a single n-way merge"
argument_list|)
argument_list|,
literal|'o'
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|0
argument_list|,
literal|"independent"
argument_list|,
operator|&
name|cmdmode
argument_list|,
name|N_
argument_list|(
literal|"list revs not reachable from others"
argument_list|)
argument_list|,
literal|'r'
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|0
argument_list|,
literal|"is-ancestor"
argument_list|,
operator|&
name|cmdmode
argument_list|,
name|N_
argument_list|(
literal|"is the first one ancestor of the other?"
argument_list|)
argument_list|,
literal|'a'
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|0
argument_list|,
literal|"fork-point"
argument_list|,
operator|&
name|cmdmode
argument_list|,
name|N_
argument_list|(
literal|"find where<commit> forked from reflog of<ref>"
argument_list|)
argument_list|,
literal|'f'
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
name|options
argument_list|,
name|merge_base_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmdmode
operator|==
literal|'a'
condition|)
block|{
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
name|usage_with_options
argument_list|(
name|merge_base_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_all
condition|)
name|die
argument_list|(
literal|"--is-ancestor cannot be used with --all"
argument_list|)
expr_stmt|;
return|return
name|handle_is_ancestor
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
return|;
block|}
if|if
condition|(
name|cmdmode
operator|==
literal|'r'
operator|&&
name|show_all
condition|)
name|die
argument_list|(
literal|"--independent cannot be used with --all"
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmdmode
operator|==
literal|'o'
condition|)
return|return
name|handle_octopus
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|show_all
argument_list|)
return|;
if|if
condition|(
name|cmdmode
operator|==
literal|'r'
condition|)
return|return
name|handle_independent
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
return|;
if|if
condition|(
name|cmdmode
operator|==
literal|'f'
condition|)
block|{
if|if
condition|(
name|argc
operator|<
literal|1
operator|||
literal|2
operator|<
name|argc
condition|)
name|usage_with_options
argument_list|(
name|merge_base_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
return|return
name|handle_fork_point
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
return|;
block|}
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
name|usage_with_options
argument_list|(
name|merge_base_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|rev
operator|=
name|xmalloc
argument_list|(
name|argc
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|rev
argument_list|)
argument_list|)
expr_stmt|;
while|while
condition|(
name|argc
operator|--
operator|>
literal|0
condition|)
name|rev
index|[
name|rev_nr
operator|++
index|]
operator|=
name|get_commit_reference
argument_list|(
operator|*
name|argv
operator|++
argument_list|)
expr_stmt|;
return|return
name|show_merge_base
argument_list|(
name|rev
argument_list|,
name|rev_nr
argument_list|,
name|show_all
argument_list|)
return|;
block|}
end_function

end_unit

