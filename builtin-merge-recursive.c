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
file|"merge-recursive.h"
end_include

begin_function
DECL|function|better_branch_name
specifier|static
specifier|const
name|char
modifier|*
name|better_branch_name
parameter_list|(
specifier|const
name|char
modifier|*
name|branch
parameter_list|)
block|{
specifier|static
name|char
name|githead_env
index|[
literal|8
operator|+
literal|40
operator|+
literal|1
index|]
decl_stmt|;
name|char
modifier|*
name|name
decl_stmt|;
if|if
condition|(
name|strlen
argument_list|(
name|branch
argument_list|)
operator|!=
literal|40
condition|)
return|return
name|branch
return|;
name|sprintf
argument_list|(
name|githead_env
argument_list|,
literal|"GITHEAD_%s"
argument_list|,
name|branch
argument_list|)
expr_stmt|;
name|name
operator|=
name|getenv
argument_list|(
name|githead_env
argument_list|)
expr_stmt|;
return|return
name|name
condition|?
name|name
else|:
name|branch
return|;
block|}
end_function

begin_function
DECL|function|cmd_merge_recursive
name|int
name|cmd_merge_recursive
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
specifier|const
name|unsigned
name|char
modifier|*
name|bases
index|[
literal|21
index|]
decl_stmt|;
name|unsigned
name|bases_count
init|=
literal|0
decl_stmt|;
name|int
name|i
decl_stmt|,
name|failed
decl_stmt|;
name|unsigned
name|char
name|h1
index|[
literal|20
index|]
decl_stmt|,
name|h2
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|merge_options
name|o
decl_stmt|;
name|struct
name|commit
modifier|*
name|result
decl_stmt|;
name|init_merge_options
argument_list|(
operator|&
name|o
argument_list|)
expr_stmt|;
if|if
condition|(
name|argv
index|[
literal|0
index|]
condition|)
block|{
name|int
name|namelen
init|=
name|strlen
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
literal|8
operator|<
name|namelen
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|0
index|]
operator|+
name|namelen
operator|-
literal|8
argument_list|,
literal|"-subtree"
argument_list|)
condition|)
name|o
operator|.
name|subtree_merge
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|<
literal|4
condition|)
name|die
argument_list|(
literal|"Usage: %s<base>... --<head><remote> ..."
argument_list|,
name|argv
index|[
literal|0
index|]
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
operator|++
name|i
control|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--"
argument_list|)
condition|)
break|break;
if|if
condition|(
name|bases_count
operator|<
name|ARRAY_SIZE
argument_list|(
name|bases
argument_list|)
operator|-
literal|1
condition|)
block|{
name|unsigned
name|char
modifier|*
name|sha
init|=
name|xmalloc
argument_list|(
literal|20
argument_list|)
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|sha
argument_list|)
condition|)
name|die
argument_list|(
literal|"Could not parse object '%s'"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|bases
index|[
name|bases_count
operator|++
index|]
operator|=
name|sha
expr_stmt|;
block|}
else|else
name|warning
argument_list|(
literal|"Cannot handle more than %d bases. "
literal|"Ignoring %s."
argument_list|,
operator|(
name|int
operator|)
name|ARRAY_SIZE
argument_list|(
name|bases
argument_list|)
operator|-
literal|1
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
name|argc
operator|-
name|i
operator|!=
literal|3
condition|)
comment|/* "--" "<head>" "<remote>" */
name|die
argument_list|(
literal|"Not handling anything other than two heads merge."
argument_list|)
expr_stmt|;
name|o
operator|.
name|branch1
operator|=
name|argv
index|[
operator|++
name|i
index|]
expr_stmt|;
name|o
operator|.
name|branch2
operator|=
name|argv
index|[
operator|++
name|i
index|]
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|o
operator|.
name|branch1
argument_list|,
name|h1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Could not resolve ref '%s'"
argument_list|,
name|o
operator|.
name|branch1
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|o
operator|.
name|branch2
argument_list|,
name|h2
argument_list|)
condition|)
name|die
argument_list|(
literal|"Could not resolve ref '%s'"
argument_list|,
name|o
operator|.
name|branch2
argument_list|)
expr_stmt|;
name|o
operator|.
name|branch1
operator|=
name|better_branch_name
argument_list|(
name|o
operator|.
name|branch1
argument_list|)
expr_stmt|;
name|o
operator|.
name|branch2
operator|=
name|better_branch_name
argument_list|(
name|o
operator|.
name|branch2
argument_list|)
expr_stmt|;
if|if
condition|(
name|o
operator|.
name|verbosity
operator|>=
literal|3
condition|)
name|printf
argument_list|(
literal|"Merging %s with %s\n"
argument_list|,
name|o
operator|.
name|branch1
argument_list|,
name|o
operator|.
name|branch2
argument_list|)
expr_stmt|;
name|failed
operator|=
name|merge_recursive_generic
argument_list|(
operator|&
name|o
argument_list|,
name|h1
argument_list|,
name|h2
argument_list|,
name|bases_count
argument_list|,
name|bases
argument_list|,
operator|&
name|result
argument_list|)
expr_stmt|;
if|if
condition|(
name|failed
operator|<
literal|0
condition|)
return|return
literal|128
return|;
comment|/* die() error code */
return|return
name|failed
return|;
block|}
end_function

end_unit

