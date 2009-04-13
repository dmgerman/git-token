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

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_decl_stmt
DECL|variable|show_ref_usage
specifier|static
specifier|const
name|char
name|show_ref_usage
index|[]
init|=
literal|"git show-ref [-q|--quiet] [--verify] [-h|--head] [-d|--dereference] [-s|--hash[=<length>]] [--abbrev[=<length>]] [--tags] [--heads] [--] [pattern*]< ref-list"
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
DECL|variable|abbrev
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
decl_stmt|,
name|abbrev
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
DECL|function|show_one
specifier|static
name|void
name|show_one
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
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|hex
init|=
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|abbrev
argument_list|)
decl_stmt|;
if|if
condition|(
name|hash_only
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|hex
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s %s\n"
argument_list|,
name|hex
argument_list|,
name|refname
argument_list|)
expr_stmt|;
block|}
end_function

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
specifier|const
name|char
modifier|*
name|hex
decl_stmt|;
name|unsigned
name|char
name|peeled
index|[
literal|20
index|]
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
name|prefixcmp
argument_list|(
name|refname
argument_list|,
literal|"refs/heads/"
argument_list|)
expr_stmt|;
name|match
operator||=
name|tags_only
operator|&&
operator|!
name|prefixcmp
argument_list|(
name|refname
argument_list|,
literal|"refs/tags/"
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
comment|/* This changes the semantics slightly that even under quiet we 	 * detect and return error if the repository is corrupt and 	 * ref points at a nonexistent object. 	 */
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"git show-ref: bad ref %s (%s)"
argument_list|,
name|refname
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|quiet
condition|)
return|return
literal|0
return|;
name|show_one
argument_list|(
name|refname
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|deref_tags
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|(
name|flag
operator|&
name|REF_ISPACKED
operator|)
operator|&&
operator|!
name|peel_ref
argument_list|(
name|refname
argument_list|,
name|peeled
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|is_null_sha1
argument_list|(
name|peeled
argument_list|)
condition|)
block|{
name|hex
operator|=
name|find_unique_abbrev
argument_list|(
name|peeled
argument_list|,
name|abbrev
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s %s^{}\n"
argument_list|,
name|hex
argument_list|,
name|refname
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
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
name|die
argument_list|(
literal|"git show-ref: bad ref %s (%s)"
argument_list|,
name|refname
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
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
if|if
condition|(
operator|!
name|obj
condition|)
name|die
argument_list|(
literal|"git show-ref: bad tag at ref %s (%s)"
argument_list|,
name|refname
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|hex
operator|=
name|find_unique_abbrev
argument_list|(
name|obj
operator|->
name|sha1
argument_list|,
name|abbrev
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s %s^{}\n"
argument_list|,
name|hex
argument_list|,
name|refname
argument_list|)
expr_stmt|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|add_existing
specifier|static
name|int
name|add_existing
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
name|string_list
modifier|*
name|list
init|=
operator|(
expr|struct
name|string_list
operator|*
operator|)
name|cbdata
decl_stmt|;
name|string_list_insert
argument_list|(
name|refname
argument_list|,
name|list
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * read "^(?:<anything>\s)?<refname>(?:\^\{\})?$" from the standard input,  * and  * (1) strip "^{}" at the end of line if any;  * (2) ignore if match is provided and does not head-match refname;  * (3) warn if refname is not a well-formed refname and skip;  * (4) ignore if refname is a ref that exists in the local repository;  * (5) otherwise output the line.  */
end_comment

begin_function
DECL|function|exclude_existing
specifier|static
name|int
name|exclude_existing
parameter_list|(
specifier|const
name|char
modifier|*
name|match
parameter_list|)
block|{
specifier|static
name|struct
name|string_list
name|existing_refs
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
name|char
name|buf
index|[
literal|1024
index|]
decl_stmt|;
name|int
name|matchlen
init|=
name|match
condition|?
name|strlen
argument_list|(
name|match
argument_list|)
else|:
literal|0
decl_stmt|;
name|for_each_ref
argument_list|(
name|add_existing
argument_list|,
operator|&
name|existing_refs
argument_list|)
expr_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
name|stdin
argument_list|)
condition|)
block|{
name|char
modifier|*
name|ref
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|buf
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|>
literal|0
operator|&&
name|buf
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|buf
index|[
operator|--
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
literal|3
operator|<=
name|len
operator|&&
operator|!
name|strcmp
argument_list|(
name|buf
operator|+
name|len
operator|-
literal|3
argument_list|,
literal|"^{}"
argument_list|)
condition|)
block|{
name|len
operator|-=
literal|3
expr_stmt|;
name|buf
index|[
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
for|for
control|(
name|ref
operator|=
name|buf
operator|+
name|len
init|;
name|buf
operator|<
name|ref
condition|;
name|ref
operator|--
control|)
if|if
condition|(
name|isspace
argument_list|(
name|ref
index|[
operator|-
literal|1
index|]
argument_list|)
condition|)
break|break;
if|if
condition|(
name|match
condition|)
block|{
name|int
name|reflen
init|=
name|buf
operator|+
name|len
operator|-
name|ref
decl_stmt|;
if|if
condition|(
name|reflen
operator|<
name|matchlen
condition|)
continue|continue;
if|if
condition|(
name|strncmp
argument_list|(
name|ref
argument_list|,
name|match
argument_list|,
name|matchlen
argument_list|)
condition|)
continue|continue;
block|}
if|if
condition|(
name|check_ref_format
argument_list|(
name|ref
argument_list|)
condition|)
block|{
name|warning
argument_list|(
literal|"ref '%s' ignored"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|string_list_has_string
argument_list|(
operator|&
name|existing_refs
argument_list|,
name|ref
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
block|}
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
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--hash="
argument_list|)
operator|||
operator|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--abbrev"
argument_list|)
operator|&&
operator|(
name|arg
index|[
literal|8
index|]
operator|==
literal|'='
operator|||
name|arg
index|[
literal|8
index|]
operator|==
literal|'\0'
operator|)
operator|)
condition|)
block|{
if|if
condition|(
name|arg
index|[
literal|2
index|]
operator|!=
literal|'h'
operator|&&
operator|!
name|arg
index|[
literal|8
index|]
condition|)
comment|/* --abbrev only */
name|abbrev
operator|=
name|DEFAULT_ABBREV
expr_stmt|;
else|else
block|{
comment|/* --hash= or --abbrev= */
name|char
modifier|*
name|end
decl_stmt|;
if|if
condition|(
name|arg
index|[
literal|2
index|]
operator|==
literal|'h'
condition|)
block|{
name|hash_only
operator|=
literal|1
expr_stmt|;
name|arg
operator|+=
literal|7
expr_stmt|;
block|}
else|else
name|arg
operator|+=
literal|9
expr_stmt|;
name|abbrev
operator|=
name|strtoul
argument_list|(
name|arg
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|end
operator|||
name|abbrev
operator|>
literal|40
condition|)
name|usage
argument_list|(
name|show_ref_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|abbrev
operator|<
name|MINIMUM_ABBREV
condition|)
name|abbrev
operator|=
name|MINIMUM_ABBREV
expr_stmt|;
block|}
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--exclude-existing"
argument_list|)
condition|)
return|return
name|exclude_existing
argument_list|(
name|NULL
argument_list|)
return|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--exclude-existing="
argument_list|)
condition|)
return|return
name|exclude_existing
argument_list|(
name|arg
operator|+
literal|19
argument_list|)
return|;
name|usage
argument_list|(
name|show_ref_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|verify
condition|)
block|{
if|if
condition|(
operator|!
name|pattern
condition|)
name|die
argument_list|(
literal|"--verify requires a reference"
argument_list|)
expr_stmt|;
while|while
condition|(
operator|*
name|pattern
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
operator|*
name|pattern
argument_list|,
literal|"refs/"
argument_list|)
operator|&&
name|resolve_ref
argument_list|(
operator|*
name|pattern
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
name|quiet
condition|)
name|show_one
argument_list|(
operator|*
name|pattern
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|quiet
condition|)
name|die
argument_list|(
literal|"'%s' - not a valid ref"
argument_list|,
operator|*
name|pattern
argument_list|)
expr_stmt|;
else|else
return|return
literal|1
return|;
name|pattern
operator|++
expr_stmt|;
block|}
return|return
literal|0
return|;
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

