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

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
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
modifier|*
specifier|const
name|describe_usage
index|[]
init|=
block|{
literal|"git-describe [options]<committish>*"
block|,
name|NULL
block|}
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
DECL|variable|longformat
specifier|static
name|int
name|longformat
decl_stmt|;
end_decl_stmt

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

begin_decl_stmt
DECL|variable|pattern
specifier|const
name|char
modifier|*
name|pattern
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|commit_name
struct|struct
name|commit_name
block|{
DECL|member|tag
name|struct
name|tag
modifier|*
name|tag
decl_stmt|;
DECL|member|prio
name|int
name|prio
decl_stmt|;
comment|/* annotated tag = 2, tag = 1, head = 0 */
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
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
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
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
name|tag
operator|=
name|NULL
expr_stmt|;
name|e
operator|->
name|prio
operator|=
name|prio
expr_stmt|;
name|hashcpy
argument_list|(
name|e
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|)
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
name|int
name|might_be_tag
init|=
operator|!
name|prefixcmp
argument_list|(
name|path
argument_list|,
literal|"refs/tags/"
argument_list|)
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|struct
name|object
modifier|*
name|object
decl_stmt|;
name|unsigned
name|char
name|peeled
index|[
literal|20
index|]
decl_stmt|;
name|int
name|is_tag
decl_stmt|,
name|prio
decl_stmt|;
if|if
condition|(
operator|!
name|all
operator|&&
operator|!
name|might_be_tag
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|peel_ref
argument_list|(
name|path
argument_list|,
name|peeled
argument_list|)
operator|&&
operator|!
name|is_null_sha1
argument_list|(
name|peeled
argument_list|)
condition|)
block|{
name|commit
operator|=
name|lookup_commit_reference_gently
argument_list|(
name|peeled
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
condition|)
return|return
literal|0
return|;
name|is_tag
operator|=
operator|!
operator|!
name|hashcmp
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
block|}
else|else
block|{
name|commit
operator|=
name|lookup_commit_reference_gently
argument_list|(
name|sha1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|object
operator|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
operator|||
operator|!
name|object
condition|)
return|return
literal|0
return|;
name|is_tag
operator|=
name|object
operator|->
name|type
operator|==
name|OBJ_TAG
expr_stmt|;
block|}
comment|/* If --all, then any refs are used. 	 * If --tags, then any tags are used. 	 * Otherwise only annotated tags are used. 	 */
if|if
condition|(
name|might_be_tag
condition|)
block|{
if|if
condition|(
name|is_tag
condition|)
block|{
name|prio
operator|=
literal|2
expr_stmt|;
if|if
condition|(
name|pattern
operator|&&
name|fnmatch
argument_list|(
name|pattern
argument_list|,
name|path
operator|+
literal|10
argument_list|,
literal|0
argument_list|)
condition|)
name|prio
operator|=
literal|0
expr_stmt|;
block|}
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
argument_list|,
name|sha1
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
DECL|function|display_name
specifier|static
name|void
name|display_name
parameter_list|(
name|struct
name|commit_name
modifier|*
name|n
parameter_list|)
block|{
if|if
condition|(
name|n
operator|->
name|prio
operator|==
literal|2
operator|&&
operator|!
name|n
operator|->
name|tag
condition|)
block|{
name|n
operator|->
name|tag
operator|=
name|lookup_tag
argument_list|(
name|n
operator|->
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|parse_tag
argument_list|(
name|n
operator|->
name|tag
argument_list|)
operator|||
operator|!
name|n
operator|->
name|tag
operator|||
operator|!
name|n
operator|->
name|tag
operator|->
name|tag
condition|)
name|die
argument_list|(
literal|"annotated tag %s not available"
argument_list|,
name|n
operator|->
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|n
operator|->
name|tag
operator|->
name|tag
argument_list|,
name|n
operator|->
name|path
argument_list|)
condition|)
name|warning
argument_list|(
literal|"tag '%s' is really '%s' here"
argument_list|,
name|n
operator|->
name|tag
operator|->
name|tag
argument_list|,
name|n
operator|->
name|path
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|n
operator|->
name|tag
condition|)
name|printf
argument_list|(
literal|"%s"
argument_list|,
name|n
operator|->
name|tag
operator|->
name|tag
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s"
argument_list|,
name|n
operator|->
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_suffix
specifier|static
name|void
name|show_suffix
parameter_list|(
name|int
name|depth
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|printf
argument_list|(
literal|"-%d-g%s"
argument_list|,
name|depth
argument_list|,
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|abbrev
argument_list|)
argument_list|)
expr_stmt|;
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
comment|/* 		 * Exact match to an existing ref. 		 */
name|display_name
argument_list|(
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|longformat
condition|)
name|show_suffix
argument_list|(
literal|0
argument_list|,
name|n
operator|->
name|tag
operator|->
name|tagged
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
operator|!
name|max_candidates
condition|)
name|die
argument_list|(
literal|"no tag exactly matches '%s'"
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
name|display_name
argument_list|(
name|all_matches
index|[
literal|0
index|]
operator|.
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|abbrev
condition|)
name|show_suffix
argument_list|(
name|all_matches
index|[
literal|0
index|]
operator|.
name|depth
argument_list|,
name|cmit
operator|->
name|object
operator|.
name|sha1
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
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
name|contains
init|=
literal|0
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"contains"
argument_list|,
operator|&
name|contains
argument_list|,
literal|"find the tag that comes after the commit"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"debug"
argument_list|,
operator|&
name|debug
argument_list|,
literal|"debug search strategy on stderr"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"all"
argument_list|,
operator|&
name|all
argument_list|,
literal|"use any ref in .git/refs"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"tags"
argument_list|,
operator|&
name|tags
argument_list|,
literal|"use any tag in .git/refs/tags"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"long"
argument_list|,
operator|&
name|longformat
argument_list|,
literal|"always use long format"
argument_list|)
block|,
name|OPT__ABBREV
argument_list|(
operator|&
name|abbrev
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"exact-match"
argument_list|,
operator|&
name|max_candidates
argument_list|,
literal|"only output exact matches"
argument_list|,
literal|0
argument_list|)
block|,
name|OPT_INTEGER
argument_list|(
literal|0
argument_list|,
literal|"candidates"
argument_list|,
operator|&
name|max_candidates
argument_list|,
literal|"consider<n> most recent tags (default: 10)"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"match"
argument_list|,
operator|&
name|pattern
argument_list|,
literal|"pattern"
argument_list|,
literal|"only consider tags matching<pattern>"
argument_list|)
block|,
name|OPT_END
argument_list|()
block|, 	}
decl_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|options
argument_list|,
name|describe_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|max_candidates
operator|<
literal|0
condition|)
name|max_candidates
operator|=
literal|0
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
name|save_commit_buffer
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|longformat
operator|&&
name|abbrev
operator|==
literal|0
condition|)
name|die
argument_list|(
literal|"--long is incompatible with --abbrev=0"
argument_list|)
expr_stmt|;
if|if
condition|(
name|contains
condition|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|args
init|=
name|xmalloc
argument_list|(
operator|(
literal|6
operator|+
name|argc
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
decl_stmt|;
name|int
name|i
init|=
literal|0
decl_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"name-rev"
expr_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"--name-only"
expr_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"--no-undefined"
expr_stmt|;
if|if
condition|(
operator|!
name|all
condition|)
block|{
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"--tags"
expr_stmt|;
if|if
condition|(
name|pattern
condition|)
block|{
name|char
modifier|*
name|s
init|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
literal|"--refs=refs/tags/"
argument_list|)
operator|+
name|strlen
argument_list|(
name|pattern
argument_list|)
operator|+
literal|1
argument_list|)
decl_stmt|;
name|sprintf
argument_list|(
name|s
argument_list|,
literal|"--refs=refs/tags/%s"
argument_list|,
name|pattern
argument_list|)
expr_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
name|s
expr_stmt|;
block|}
block|}
name|memcpy
argument_list|(
name|args
operator|+
name|i
argument_list|,
name|argv
argument_list|,
name|argc
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|args
index|[
name|i
operator|+
name|argc
index|]
operator|=
name|NULL
expr_stmt|;
return|return
name|cmd_name_rev
argument_list|(
name|i
operator|+
name|argc
argument_list|,
name|args
argument_list|,
name|prefix
argument_list|)
return|;
block|}
if|if
condition|(
name|argc
operator|==
literal|0
condition|)
block|{
name|describe
argument_list|(
literal|"HEAD"
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
else|else
block|{
while|while
condition|(
name|argc
operator|--
operator|>
literal|0
condition|)
block|{
name|describe
argument_list|(
operator|*
name|argv
operator|++
argument_list|,
name|argc
operator|==
literal|0
argument_list|)
expr_stmt|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

