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

begin_define
DECL|macro|SEEN
define|#
directive|define
name|SEEN
value|(1u<< 0)
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
DECL|variable|all
specifier|static
name|int
name|all
init|=
literal|0
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
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|tags
comment|/* But allow any tags if --tags is specified */
end_comment

begin_define
DECL|macro|DEFAULT_ABBREV
define|#
directive|define
name|DEFAULT_ABBREV
value|8
end_define

begin_comment
DECL|macro|DEFAULT_ABBREV
comment|/* maybe too many */
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
DECL|variable|names
DECL|variable|allocs
specifier|static
name|int
name|names
init|=
literal|0
decl_stmt|,
name|allocs
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|commit_name
specifier|static
struct|struct
name|commit_name
block|{
DECL|member|commit
specifier|const
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
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
DECL|variable|name_array
block|}
modifier|*
modifier|*
name|name_array
init|=
name|NULL
struct|;
end_struct

begin_function
DECL|function|match
specifier|static
name|struct
name|commit_name
modifier|*
name|match
parameter_list|(
name|struct
name|commit
modifier|*
name|cmit
parameter_list|)
block|{
name|int
name|i
init|=
name|names
decl_stmt|;
name|struct
name|commit_name
modifier|*
modifier|*
name|p
init|=
name|name_array
decl_stmt|;
while|while
condition|(
name|i
operator|--
operator|>
literal|0
condition|)
block|{
name|struct
name|commit_name
modifier|*
name|n
init|=
operator|*
name|p
operator|++
decl_stmt|;
if|if
condition|(
name|n
operator|->
name|commit
operator|==
name|cmit
condition|)
return|return
name|n
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

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
specifier|const
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|int
name|prio
parameter_list|)
block|{
name|int
name|idx
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|path
argument_list|)
operator|+
literal|1
decl_stmt|;
name|struct
name|commit_name
modifier|*
name|name
init|=
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
decl_stmt|;
name|name
operator|->
name|commit
operator|=
name|commit
expr_stmt|;
name|name
operator|->
name|prio
operator|=
name|prio
expr_stmt|;
name|memcpy
argument_list|(
name|name
operator|->
name|path
argument_list|,
name|path
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|idx
operator|=
name|names
expr_stmt|;
if|if
condition|(
name|idx
operator|>=
name|allocs
condition|)
block|{
name|allocs
operator|=
operator|(
name|idx
operator|+
literal|50
operator|)
operator|*
literal|3
operator|/
literal|2
expr_stmt|;
name|name_array
operator|=
name|xrealloc
argument_list|(
name|name_array
argument_list|,
name|allocs
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|name_array
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|name_array
index|[
name|idx
index|]
operator|=
name|name
expr_stmt|;
name|names
operator|=
operator|++
name|idx
expr_stmt|;
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
name|tag_type
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

begin_function
DECL|function|compare_names
specifier|static
name|int
name|compare_names
parameter_list|(
specifier|const
name|void
modifier|*
name|_a
parameter_list|,
specifier|const
name|void
modifier|*
name|_b
parameter_list|)
block|{
name|struct
name|commit_name
modifier|*
name|a
init|=
operator|*
operator|(
expr|struct
name|commit_name
operator|*
operator|*
operator|)
name|_a
decl_stmt|;
name|struct
name|commit_name
modifier|*
name|b
init|=
operator|*
operator|(
expr|struct
name|commit_name
operator|*
operator|*
operator|)
name|_b
decl_stmt|;
name|unsigned
name|long
name|a_date
init|=
name|a
operator|->
name|commit
operator|->
name|date
decl_stmt|;
name|unsigned
name|long
name|b_date
init|=
name|b
operator|->
name|commit
operator|->
name|date
decl_stmt|;
if|if
condition|(
name|a
operator|->
name|prio
operator|!=
name|b
operator|->
name|prio
condition|)
return|return
name|b
operator|->
name|prio
operator|-
name|a
operator|->
name|prio
return|;
return|return
operator|(
name|a_date
operator|>
name|b_date
operator|)
condition|?
operator|-
literal|1
else|:
operator|(
name|a_date
operator|==
name|b_date
operator|)
condition|?
literal|0
else|:
literal|1
return|;
block|}
end_function

begin_function
DECL|function|describe
specifier|static
name|void
name|describe
parameter_list|(
name|char
modifier|*
name|arg
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
if|if
condition|(
name|get_sha1
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|usage
argument_list|(
name|describe_usage
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
name|usage
argument_list|(
name|describe_usage
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
argument_list|)
expr_stmt|;
name|qsort
argument_list|(
name|name_array
argument_list|,
name|names
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|name_array
argument_list|)
argument_list|,
name|compare_names
argument_list|)
expr_stmt|;
block|}
name|n
operator|=
name|match
argument_list|(
name|cmit
argument_list|)
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
name|list
operator|=
name|NULL
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
name|pop_most_recent_commit
argument_list|(
operator|&
name|list
argument_list|,
name|SEEN
argument_list|)
decl_stmt|;
name|n
operator|=
name|match
argument_list|(
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
condition|)
block|{
name|printf
argument_list|(
literal|"%s-g%s\n"
argument_list|,
name|n
operator|->
name|path
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
break|break;
block|}
block|}
name|clear_commit_marks
argument_list|(
name|cmit
argument_list|,
name|SEEN
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
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
operator|<
literal|4
operator|||
literal|40
operator|<=
name|abbrev
condition|)
name|abbrev
operator|=
name|DEFAULT_ABBREV
expr_stmt|;
block|}
else|else
name|usage
argument_list|(
name|describe_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|i
operator|==
name|argc
condition|)
name|describe
argument_list|(
literal|"HEAD"
argument_list|)
expr_stmt|;
else|else
while|while
condition|(
name|i
operator|<
name|argc
condition|)
name|describe
argument_list|(
name|argv
index|[
name|i
operator|++
index|]
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

