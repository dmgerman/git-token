begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_decl_stmt
DECL|variable|show_ref_usage
specifier|static
specifier|const
name|char
name|show_ref_usage
index|[]
init|=
literal|"git show-ref [-q|--quiet] [--verify] [-h|--head] [-d|--dereference] [-s|--hash] [--tags] [--heads] [--] [pattern*]"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|deref_tags
DECL|variable|show_head
DECL|variable|tags_only
DECL|variable|heads_only
specifier|static
name|int
name|deref_tags
init|=
literal|0
decl_stmt|,
name|show_head
init|=
literal|0
decl_stmt|,
name|tags_only
init|=
literal|0
decl_stmt|,
name|heads_only
init|=
literal|0
decl_stmt|,
DECL|variable|found_match
DECL|variable|verify
DECL|variable|quiet
DECL|variable|hash_only
name|found_match
init|=
literal|0
decl_stmt|,
name|verify
init|=
literal|0
decl_stmt|,
name|quiet
init|=
literal|0
decl_stmt|,
name|hash_only
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pattern
specifier|static
specifier|const
name|char
modifier|*
modifier|*
name|pattern
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|show_ref
specifier|static
name|int
name|show_ref
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
name|flag
parameter_list|,
name|void
modifier|*
name|cbdata
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
decl_stmt|;
if|if
condition|(
name|tags_only
operator|||
name|heads_only
condition|)
block|{
name|int
name|match
decl_stmt|;
name|match
operator|=
name|heads_only
operator|&&
operator|!
name|strncmp
argument_list|(
name|refname
argument_list|,
literal|"refs/heads/"
argument_list|,
literal|11
argument_list|)
expr_stmt|;
name|match
operator||=
name|tags_only
operator|&&
operator|!
name|strncmp
argument_list|(
name|refname
argument_list|,
literal|"refs/tags/"
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|match
condition|)
return|return
literal|0
return|;
block|}
if|if
condition|(
name|pattern
condition|)
block|{
name|int
name|reflen
init|=
name|strlen
argument_list|(
name|refname
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|p
init|=
name|pattern
decl_stmt|,
modifier|*
name|m
decl_stmt|;
while|while
condition|(
operator|(
name|m
operator|=
operator|*
name|p
operator|++
operator|)
operator|!=
name|NULL
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|m
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|>
name|reflen
condition|)
continue|continue;
if|if
condition|(
name|memcmp
argument_list|(
name|m
argument_list|,
name|refname
operator|+
name|reflen
operator|-
name|len
argument_list|,
name|len
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|len
operator|==
name|reflen
condition|)
goto|goto
name|match
goto|;
comment|/* "--verify" requires an exact match */
if|if
condition|(
name|verify
condition|)
continue|continue;
if|if
condition|(
name|refname
index|[
name|reflen
operator|-
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
goto|goto
name|match
goto|;
block|}
return|return
literal|0
return|;
block|}
name|match
label|:
name|found_match
operator|++
expr_stmt|;
name|obj
operator|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
block|{
if|if
condition|(
name|quiet
condition|)
return|return
literal|0
return|;
name|die
argument_list|(
literal|"git-show-ref: bad ref %s (%s)"
argument_list|,
name|refname
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|quiet
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|hash_only
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|refname
argument_list|)
expr_stmt|;
if|if
condition|(
name|deref_tags
operator|&&
name|obj
operator|->
name|type
operator|==
name|OBJ_TAG
condition|)
block|{
name|obj
operator|=
name|deref_tag
argument_list|(
name|obj
argument_list|,
name|refname
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s %s^{}\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|,
name|refname
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_show_ref
name|int
name|cmd_show_ref
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
block|{
name|pattern
operator|=
name|argv
operator|+
name|i
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
literal|"--"
argument_list|)
condition|)
block|{
name|pattern
operator|=
name|argv
operator|+
name|i
operator|+
literal|1
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|pattern
condition|)
name|pattern
operator|=
name|NULL
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
literal|"-q"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--quiet"
argument_list|)
condition|)
block|{
name|quiet
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
literal|"-h"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--head"
argument_list|)
condition|)
block|{
name|show_head
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
literal|"-d"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--dereference"
argument_list|)
condition|)
block|{
name|deref_tags
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
literal|"-s"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--hash"
argument_list|)
condition|)
block|{
name|hash_only
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
literal|"--verify"
argument_list|)
condition|)
block|{
name|verify
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
literal|"--tags"
argument_list|)
condition|)
block|{
name|tags_only
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
literal|"--heads"
argument_list|)
condition|)
block|{
name|heads_only
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|show_ref_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|show_head
condition|)
name|head_ref
argument_list|(
name|show_ref
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|for_each_ref
argument_list|(
name|show_ref
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|found_match
condition|)
block|{
if|if
condition|(
name|verify
operator|&&
operator|!
name|quiet
condition|)
name|die
argument_list|(
literal|"No match"
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

