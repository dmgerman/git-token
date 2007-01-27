begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_define
DECL|macro|SEEN
define|#
directive|define
name|SEEN
value|(1u<<0)
end_define

begin_define
DECL|macro|MAX_TAGS
define|#
directive|define
name|MAX_TAGS
value|(FLAG_BITS - 1)
end_define

begin_decl_stmt
DECL|variable|describe_usage
specifier|static
specifier|const
name|char
name|describe_usage
index|[]
init|=
literal|"git-describe [--all] [--tags] [--abbrev=<n>]<committish>*"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|debug
specifier|static
name|int
name|debug
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|debug
comment|/* Display lots of verbose info */
end_comment

begin_decl_stmt
DECL|variable|all
specifier|static
name|int
name|all
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|all
comment|/* Default to annotated tags only */
end_comment

begin_decl_stmt
DECL|variable|tags
specifier|static
name|int
name|tags
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|tags
comment|/* But allow any tags if --tags is specified */
end_comment

begin_decl_stmt
DECL|variable|abbrev
specifier|static
name|int
name|abbrev
init|=
name|DEFAULT_ABBREV
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|max_candidates
specifier|static
name|int
name|max_candidates
init|=
literal|10
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|commit_name
struct|struct
name|commit_name
block|{
DECL|member|prio
name|int
name|prio
decl_stmt|;
comment|/* annotated tag = 2, tag = 1, head = 0 */
DECL|member|path
name|char
name|path
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
comment|/* more */
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|prio_names
specifier|static
specifier|const
name|char
modifier|*
name|prio_names
index|[]
init|=
block|{
literal|"head"
block|,
literal|"lightweight"
block|,
literal|"annotated"
block|, }
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_to_known_names
specifier|static
name|void
name|add_to_known_names
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|int
name|prio
parameter_list|)
block|{
name|struct
name|commit_name
modifier|*
name|e
init|=
name|commit
operator|->
name|util
decl_stmt|;
if|if
condition|(
operator|!
name|e
operator|||
name|e
operator|->
name|prio
operator|<
name|prio
condition|)
block|{
name|size_t
name|len
init|=
name|strlen
argument_list|(
name|path
argument_list|)
operator|+
literal|1
decl_stmt|;
name|free
argument_list|(
name|e
argument_list|)
expr_stmt|;
name|e
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|commit_name
argument_list|)
operator|+
name|len
argument_list|)
expr_stmt|;
name|e
operator|->
name|prio
operator|=
name|prio
expr_stmt|;
name|memcpy
argument_list|(
name|e
operator|->
name|path
argument_list|,
name|path
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|commit
operator|->
name|util
operator|=
name|e
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|get_name
specifier|static
name|int
name|get_name
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|flag
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|lookup_commit_reference_gently
argument_list|(
name|sha1
argument_list|,
literal|1
argument_list|)
decl_stmt|;
name|struct
name|object
modifier|*
name|object
decl_stmt|;
name|int
name|prio
decl_stmt|;
if|if
condition|(
operator|!
name|commit
condition|)
return|return
literal|0
return|;
name|object
operator|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
comment|/* If --all, then any refs are used. 	 * If --tags, then any tags are used. 	 * Otherwise only annotated tags are used. 	 */
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|path
argument_list|,
literal|"refs/tags/"
argument_list|,
literal|10
argument_list|)
condition|)
block|{
if|if
condition|(
name|object
operator|->
name|type
operator|==
name|OBJ_TAG
condition|)
name|prio
operator|=
literal|2
expr_stmt|;
else|else
name|prio
operator|=
literal|1
expr_stmt|;
block|}
else|else
name|prio
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
name|all
condition|)
block|{
if|if
condition|(
operator|!
name|prio
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|tags
operator|&&
name|prio
operator|<
literal|2
condition|)
return|return
literal|0
return|;
block|}
name|add_to_known_names
argument_list|(
name|all
condition|?
name|path
operator|+
literal|5
else|:
name|path
operator|+
literal|10
argument_list|,
name|commit
argument_list|,
name|prio
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_struct
DECL|struct|possible_tag
struct|struct
name|possible_tag
block|{
DECL|member|name
name|struct
name|commit_name
modifier|*
name|name
decl_stmt|;
DECL|member|depth
name|int
name|depth
decl_stmt|;
DECL|member|found_order
name|int
name|found_order
decl_stmt|;
DECL|member|flag_within
name|unsigned
name|flag_within
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|compare_pt
specifier|static
name|int
name|compare_pt
parameter_list|(
specifier|const
name|void
modifier|*
name|a_
parameter_list|,
specifier|const
name|void
modifier|*
name|b_
parameter_list|)
block|{
name|struct
name|possible_tag
modifier|*
name|a
init|=
operator|(
expr|struct
name|possible_tag
operator|*
operator|)
name|a_
decl_stmt|;
name|struct
name|possible_tag
modifier|*
name|b
init|=
operator|(
expr|struct
name|possible_tag
operator|*
operator|)
name|b_
decl_stmt|;
if|if
condition|(
name|a
operator|->
name|name
operator|->
name|prio
operator|!=
name|b
operator|->
name|name
operator|->
name|prio
condition|)
return|return
name|b
operator|->
name|name
operator|->
name|prio
operator|-
name|a
operator|->
name|name
operator|->
name|prio
return|;
if|if
condition|(
name|a
operator|->
name|depth
operator|!=
name|b
operator|->
name|depth
condition|)
return|return
name|a
operator|->
name|depth
operator|-
name|b
operator|->
name|depth
return|;
if|if
condition|(
name|a
operator|->
name|found_order
operator|!=
name|b
operator|->
name|found_order
condition|)
return|return
name|a
operator|->
name|found_order
operator|-
name|b
operator|->
name|found_order
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|finish_depth_computation
specifier|static
name|unsigned
name|long
name|finish_depth_computation
parameter_list|(
name|struct
name|commit_list
modifier|*
modifier|*
name|list
parameter_list|,
name|struct
name|possible_tag
modifier|*
name|best
parameter_list|)
block|{
name|unsigned
name|long
name|seen_commits
init|=
literal|0
decl_stmt|;
while|while
condition|(
operator|*
name|list
condition|)
block|{
name|struct
name|commit
modifier|*
name|c
init|=
name|pop_commit
argument_list|(
name|list
argument_list|)
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|parents
init|=
name|c
operator|->
name|parents
decl_stmt|;
name|seen_commits
operator|++
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|object
operator|.
name|flags
operator|&
name|best
operator|->
name|flag_within
condition|)
block|{
name|struct
name|commit_list
modifier|*
name|a
init|=
operator|*
name|list
decl_stmt|;
while|while
condition|(
name|a
condition|)
block|{
name|struct
name|commit
modifier|*
name|i
init|=
name|a
operator|->
name|item
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|i
operator|->
name|object
operator|.
name|flags
operator|&
name|best
operator|->
name|flag_within
operator|)
condition|)
break|break;
name|a
operator|=
name|a
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|a
condition|)
break|break;
block|}
else|else
name|best
operator|->
name|depth
operator|++
expr_stmt|;
while|while
condition|(
name|parents
condition|)
block|{
name|struct
name|commit
modifier|*
name|p
init|=
name|parents
operator|->
name|item
decl_stmt|;
name|parse_commit
argument_list|(
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|p
operator|->
name|object
operator|.
name|flags
operator|&
name|SEEN
operator|)
condition|)
name|insert_by_date
argument_list|(
name|p
argument_list|,
name|list
argument_list|)
expr_stmt|;
name|p
operator|->
name|object
operator|.
name|flags
operator||=
name|c
operator|->
name|object
operator|.
name|flags
expr_stmt|;
name|parents
operator|=
name|parents
operator|->
name|next
expr_stmt|;
block|}
block|}
return|return
name|seen_commits
return|;
block|}
end_function

begin_function
DECL|function|describe
specifier|static
name|void
name|describe
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|last_one
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|commit
modifier|*
name|cmit
decl_stmt|,
modifier|*
name|gave_up_on
init|=
name|NULL
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|list
decl_stmt|;
specifier|static
name|int
name|initialized
init|=
literal|0
decl_stmt|;
name|struct
name|commit_name
modifier|*
name|n
decl_stmt|;
name|struct
name|possible_tag
name|all_matches
index|[
name|MAX_TAGS
index|]
decl_stmt|;
name|unsigned
name|int
name|match_cnt
init|=
literal|0
decl_stmt|,
name|annotated_cnt
init|=
literal|0
decl_stmt|,
name|cur_match
decl_stmt|;
name|unsigned
name|long
name|seen_commits
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|cmit
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cmit
condition|)
name|die
argument_list|(
literal|"%s is not a valid '%s' object"
argument_list|,
name|arg
argument_list|,
name|commit_type
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|initialized
condition|)
block|{
name|initialized
operator|=
literal|1
expr_stmt|;
name|for_each_ref
argument_list|(
name|get_name
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
name|n
operator|=
name|cmit
operator|->
name|util
expr_stmt|;
if|if
condition|(
name|n
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|n
operator|->
name|path
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|debug
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"searching to describe %s\n"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|list
operator|=
name|NULL
expr_stmt|;
name|cmit
operator|->
name|object
operator|.
name|flags
operator|=
name|SEEN
expr_stmt|;
name|commit_list_insert
argument_list|(
name|cmit
argument_list|,
operator|&
name|list
argument_list|)
expr_stmt|;
while|while
condition|(
name|list
condition|)
block|{
name|struct
name|commit
modifier|*
name|c
init|=
name|pop_commit
argument_list|(
operator|&
name|list
argument_list|)
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|parents
init|=
name|c
operator|->
name|parents
decl_stmt|;
name|seen_commits
operator|++
expr_stmt|;
name|n
operator|=
name|c
operator|->
name|util
expr_stmt|;
if|if
condition|(
name|n
condition|)
block|{
if|if
condition|(
name|match_cnt
operator|<
name|max_candidates
condition|)
block|{
name|struct
name|possible_tag
modifier|*
name|t
init|=
operator|&
name|all_matches
index|[
name|match_cnt
operator|++
index|]
decl_stmt|;
name|t
operator|->
name|name
operator|=
name|n
expr_stmt|;
name|t
operator|->
name|depth
operator|=
name|seen_commits
operator|-
literal|1
expr_stmt|;
name|t
operator|->
name|flag_within
operator|=
literal|1u
operator|<<
name|match_cnt
expr_stmt|;
name|t
operator|->
name|found_order
operator|=
name|match_cnt
expr_stmt|;
name|c
operator|->
name|object
operator|.
name|flags
operator||=
name|t
operator|->
name|flag_within
expr_stmt|;
if|if
condition|(
name|n
operator|->
name|prio
operator|==
literal|2
condition|)
name|annotated_cnt
operator|++
expr_stmt|;
block|}
else|else
block|{
name|gave_up_on
operator|=
name|c
expr_stmt|;
break|break;
block|}
block|}
for|for
control|(
name|cur_match
operator|=
literal|0
init|;
name|cur_match
operator|<
name|match_cnt
condition|;
name|cur_match
operator|++
control|)
block|{
name|struct
name|possible_tag
modifier|*
name|t
init|=
operator|&
name|all_matches
index|[
name|cur_match
index|]
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|c
operator|->
name|object
operator|.
name|flags
operator|&
name|t
operator|->
name|flag_within
operator|)
condition|)
name|t
operator|->
name|depth
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|annotated_cnt
operator|&&
operator|!
name|list
condition|)
block|{
if|if
condition|(
name|debug
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"finished search at %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|c
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
break|break;
block|}
while|while
condition|(
name|parents
condition|)
block|{
name|struct
name|commit
modifier|*
name|p
init|=
name|parents
operator|->
name|item
decl_stmt|;
name|parse_commit
argument_list|(
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|p
operator|->
name|object
operator|.
name|flags
operator|&
name|SEEN
operator|)
condition|)
name|insert_by_date
argument_list|(
name|p
argument_list|,
operator|&
name|list
argument_list|)
expr_stmt|;
name|p
operator|->
name|object
operator|.
name|flags
operator||=
name|c
operator|->
name|object
operator|.
name|flags
expr_stmt|;
name|parents
operator|=
name|parents
operator|->
name|next
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|match_cnt
condition|)
name|die
argument_list|(
literal|"cannot describe '%s'"
argument_list|,
name|sha1_to_hex
argument_list|(
name|cmit
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|qsort
argument_list|(
name|all_matches
argument_list|,
name|match_cnt
argument_list|,
sizeof|sizeof
argument_list|(
name|all_matches
index|[
literal|0
index|]
argument_list|)
argument_list|,
name|compare_pt
argument_list|)
expr_stmt|;
if|if
condition|(
name|gave_up_on
condition|)
block|{
name|insert_by_date
argument_list|(
name|gave_up_on
argument_list|,
operator|&
name|list
argument_list|)
expr_stmt|;
name|seen_commits
operator|--
expr_stmt|;
block|}
name|seen_commits
operator|+=
name|finish_depth_computation
argument_list|(
operator|&
name|list
argument_list|,
operator|&
name|all_matches
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|free_commit_list
argument_list|(
name|list
argument_list|)
expr_stmt|;
if|if
condition|(
name|debug
condition|)
block|{
for|for
control|(
name|cur_match
operator|=
literal|0
init|;
name|cur_match
operator|<
name|match_cnt
condition|;
name|cur_match
operator|++
control|)
block|{
name|struct
name|possible_tag
modifier|*
name|t
init|=
operator|&
name|all_matches
index|[
name|cur_match
index|]
decl_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|" %-11s %8d %s\n"
argument_list|,
name|prio_names
index|[
name|t
operator|->
name|name
operator|->
name|prio
index|]
argument_list|,
name|t
operator|->
name|depth
argument_list|,
name|t
operator|->
name|name
operator|->
name|path
argument_list|)
expr_stmt|;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"traversed %lu commits\n"
argument_list|,
name|seen_commits
argument_list|)
expr_stmt|;
if|if
condition|(
name|gave_up_on
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"more than %i tags found; listed %i most recent\n"
literal|"gave up search at %s\n"
argument_list|,
name|max_candidates
argument_list|,
name|max_candidates
argument_list|,
name|sha1_to_hex
argument_list|(
name|gave_up_on
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|abbrev
operator|==
literal|0
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|all_matches
index|[
literal|0
index|]
operator|.
name|name
operator|->
name|path
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s-%d-g%s\n"
argument_list|,
name|all_matches
index|[
literal|0
index|]
operator|.
name|name
operator|->
name|path
argument_list|,
name|all_matches
index|[
literal|0
index|]
operator|.
name|depth
argument_list|,
name|find_unique_abbrev
argument_list|(
name|cmit
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|abbrev
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|last_one
condition|)
name|clear_commit_marks
argument_list|(
name|cmit
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_describe
name|int
name|cmd_describe
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
name|i
decl_stmt|;
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
operator|*
name|arg
operator|!=
literal|'-'
condition|)
break|break;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--debug"
argument_list|)
condition|)
name|debug
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--all"
argument_list|)
condition|)
name|all
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--tags"
argument_list|)
condition|)
name|tags
operator|=
literal|1
expr_stmt|;
elseif|else
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
name|strtoul
argument_list|(
name|arg
operator|+
literal|9
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|abbrev
operator|!=
literal|0
operator|&&
operator|(
name|abbrev
operator|<
name|MINIMUM_ABBREV
operator|||
literal|40
operator|<
name|abbrev
operator|)
condition|)
name|abbrev
operator|=
name|DEFAULT_ABBREV
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--candidates="
argument_list|,
literal|13
argument_list|)
condition|)
block|{
name|max_candidates
operator|=
name|strtoul
argument_list|(
name|arg
operator|+
literal|13
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|max_candidates
operator|<
literal|1
condition|)
name|max_candidates
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
name|max_candidates
operator|>
name|MAX_TAGS
condition|)
name|max_candidates
operator|=
name|MAX_TAGS
expr_stmt|;
block|}
else|else
name|usage
argument_list|(
name|describe_usage
argument_list|)
expr_stmt|;
block|}
name|save_commit_buffer
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|argc
operator|<=
name|i
condition|)
name|describe
argument_list|(
literal|"HEAD"
argument_list|,
literal|1
argument_list|)
expr_stmt|;
else|else
while|while
condition|(
name|i
operator|<
name|argc
condition|)
block|{
name|describe
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
operator|(
name|i
operator|==
name|argc
operator|-
literal|1
operator|)
argument_list|)
expr_stmt|;
name|i
operator|++
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

