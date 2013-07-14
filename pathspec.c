begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"pathspec.h"
end_include

begin_comment
comment|/*  * Finds which of the given pathspecs match items in the index.  *  * For each pathspec, sets the corresponding entry in the seen[] array  * (which should be specs items long, i.e. the same size as pathspec)  * to the nature of the "closest" (i.e. most specific) match found for  * that pathspec in the index, if it was a closer type of match than  * the existing entry.  As an optimization, matching is skipped  * altogether if seen[] already only contains non-zero entries.  *  * If seen[] has not already been written to, it may make sense  * to use find_pathspecs_matching_against_index() instead.  */
end_comment

begin_function
DECL|function|add_pathspec_matches_against_index
name|void
name|add_pathspec_matches_against_index
parameter_list|(
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|,
name|char
modifier|*
name|seen
parameter_list|)
block|{
name|int
name|num_unmatched
init|=
literal|0
decl_stmt|,
name|i
decl_stmt|;
comment|/* 	 * Since we are walking the index as if we were walking the directory, 	 * we have to mark the matched pathspec as seen; otherwise we will 	 * mistakenly think that the user gave a pathspec that did not match 	 * anything. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|pathspec
operator|->
name|nr
condition|;
name|i
operator|++
control|)
if|if
condition|(
operator|!
name|seen
index|[
name|i
index|]
condition|)
name|num_unmatched
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|num_unmatched
condition|)
return|return;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|active_nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|active_cache
index|[
name|i
index|]
decl_stmt|;
name|match_pathspec_depth
argument_list|(
name|pathspec
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
argument_list|,
literal|0
argument_list|,
name|seen
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/*  * Finds which of the given pathspecs match items in the index.  *  * This is a one-shot wrapper around add_pathspec_matches_against_index()  * which allocates, populates, and returns a seen[] array indicating the  * nature of the "closest" (i.e. most specific) matches which each of the  * given pathspecs achieves against all items in the index.  */
end_comment

begin_function
DECL|function|find_pathspecs_matching_against_index
name|char
modifier|*
name|find_pathspecs_matching_against_index
parameter_list|(
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|)
block|{
name|char
modifier|*
name|seen
init|=
name|xcalloc
argument_list|(
name|pathspec
operator|->
name|nr
argument_list|,
literal|1
argument_list|)
decl_stmt|;
name|add_pathspec_matches_against_index
argument_list|(
name|pathspec
argument_list|,
name|seen
argument_list|)
expr_stmt|;
return|return
name|seen
return|;
block|}
end_function

begin_comment
comment|/*  * Magic pathspec  *  * Possible future magic semantics include stuff like:  *  *	{ PATHSPEC_NOGLOB, '!', "noglob" },  *	{ PATHSPEC_ICASE, '\0', "icase" },  *	{ PATHSPEC_RECURSIVE, '*', "recursive" },  *	{ PATHSPEC_REGEXP, '\0', "regexp" },  *  */
end_comment

begin_struct
DECL|struct|pathspec_magic
specifier|static
struct|struct
name|pathspec_magic
block|{
DECL|member|bit
name|unsigned
name|bit
decl_stmt|;
DECL|member|mnemonic
name|char
name|mnemonic
decl_stmt|;
comment|/* this cannot be ':'! */
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|variable|pathspec_magic
block|}
name|pathspec_magic
index|[]
init|=
block|{
block|{
name|PATHSPEC_FROMTOP
block|,
literal|'/'
block|,
literal|"top"
block|}
block|, }
struct|;
end_struct

begin_comment
comment|/*  * Take an element of a pathspec and check for magic signatures.  * Append the result to the prefix. Return the magic bitmap.  *  * For now, we only parse the syntax and throw out anything other than  * "top" magic.  *  * NEEDSWORK: This needs to be rewritten when we start migrating  * get_pathspec() users to use the "struct pathspec" interface.  For  * example, a pathspec element may be marked as case-insensitive, but  * the prefix part must always match literally, and a single stupid  * string cannot express such a case.  */
end_comment

begin_function
DECL|function|prefix_pathspec
specifier|static
name|unsigned
name|prefix_pathspec
parameter_list|(
name|struct
name|pathspec_item
modifier|*
name|item
parameter_list|,
name|unsigned
modifier|*
name|p_short_magic
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|raw
parameter_list|,
name|unsigned
name|flags
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|prefixlen
parameter_list|,
specifier|const
name|char
modifier|*
name|elt
parameter_list|)
block|{
name|unsigned
name|magic
init|=
literal|0
decl_stmt|,
name|short_magic
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|copyfrom
init|=
name|elt
decl_stmt|;
name|char
modifier|*
name|match
decl_stmt|;
name|int
name|i
decl_stmt|;
if|if
condition|(
name|elt
index|[
literal|0
index|]
operator|!=
literal|':'
condition|)
block|{
empty_stmt|;
comment|/* nothing to do */
block|}
elseif|else
if|if
condition|(
name|elt
index|[
literal|1
index|]
operator|==
literal|'('
condition|)
block|{
comment|/* longhand */
specifier|const
name|char
modifier|*
name|nextat
decl_stmt|;
for|for
control|(
name|copyfrom
operator|=
name|elt
operator|+
literal|2
init|;
operator|*
name|copyfrom
operator|&&
operator|*
name|copyfrom
operator|!=
literal|')'
condition|;
name|copyfrom
operator|=
name|nextat
control|)
block|{
name|size_t
name|len
init|=
name|strcspn
argument_list|(
name|copyfrom
argument_list|,
literal|",)"
argument_list|)
decl_stmt|;
if|if
condition|(
name|copyfrom
index|[
name|len
index|]
operator|==
literal|','
condition|)
name|nextat
operator|=
name|copyfrom
operator|+
name|len
operator|+
literal|1
expr_stmt|;
else|else
comment|/* handle ')' and '\0' */
name|nextat
operator|=
name|copyfrom
operator|+
name|len
expr_stmt|;
if|if
condition|(
operator|!
name|len
condition|)
continue|continue;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ARRAY_SIZE
argument_list|(
name|pathspec_magic
argument_list|)
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|strlen
argument_list|(
name|pathspec_magic
index|[
name|i
index|]
operator|.
name|name
argument_list|)
operator|==
name|len
operator|&&
operator|!
name|strncmp
argument_list|(
name|pathspec_magic
index|[
name|i
index|]
operator|.
name|name
argument_list|,
name|copyfrom
argument_list|,
name|len
argument_list|)
condition|)
block|{
name|magic
operator||=
name|pathspec_magic
index|[
name|i
index|]
operator|.
name|bit
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|ARRAY_SIZE
argument_list|(
name|pathspec_magic
argument_list|)
operator|<=
name|i
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"Invalid pathspec magic '%.*s' in '%s'"
argument_list|)
argument_list|,
operator|(
name|int
operator|)
name|len
argument_list|,
name|copyfrom
argument_list|,
name|elt
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|copyfrom
operator|!=
literal|')'
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"Missing ')' at the end of pathspec magic in '%s'"
argument_list|)
argument_list|,
name|elt
argument_list|)
expr_stmt|;
name|copyfrom
operator|++
expr_stmt|;
block|}
else|else
block|{
comment|/* shorthand */
for|for
control|(
name|copyfrom
operator|=
name|elt
operator|+
literal|1
init|;
operator|*
name|copyfrom
operator|&&
operator|*
name|copyfrom
operator|!=
literal|':'
condition|;
name|copyfrom
operator|++
control|)
block|{
name|char
name|ch
init|=
operator|*
name|copyfrom
decl_stmt|;
if|if
condition|(
operator|!
name|is_pathspec_magic
argument_list|(
name|ch
argument_list|)
condition|)
break|break;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ARRAY_SIZE
argument_list|(
name|pathspec_magic
argument_list|)
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|pathspec_magic
index|[
name|i
index|]
operator|.
name|mnemonic
operator|==
name|ch
condition|)
block|{
name|short_magic
operator||=
name|pathspec_magic
index|[
name|i
index|]
operator|.
name|bit
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|ARRAY_SIZE
argument_list|(
name|pathspec_magic
argument_list|)
operator|<=
name|i
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"Unimplemented pathspec magic '%c' in '%s'"
argument_list|)
argument_list|,
name|ch
argument_list|,
name|elt
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|copyfrom
operator|==
literal|':'
condition|)
name|copyfrom
operator|++
expr_stmt|;
block|}
name|magic
operator||=
name|short_magic
expr_stmt|;
operator|*
name|p_short_magic
operator|=
name|short_magic
expr_stmt|;
if|if
condition|(
name|magic
operator|&
name|PATHSPEC_FROMTOP
condition|)
block|{
name|match
operator|=
name|xstrdup
argument_list|(
name|copyfrom
argument_list|)
expr_stmt|;
name|prefixlen
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|match
operator|=
name|prefix_path_gently
argument_list|(
name|prefix
argument_list|,
name|prefixlen
argument_list|,
operator|&
name|prefixlen
argument_list|,
name|copyfrom
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|match
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"%s: '%s' is outside repository"
argument_list|)
argument_list|,
name|elt
argument_list|,
name|copyfrom
argument_list|)
expr_stmt|;
block|}
operator|*
name|raw
operator|=
name|item
operator|->
name|match
operator|=
name|match
expr_stmt|;
comment|/* 	 * Prefix the pathspec (keep all magic) and assign to 	 * original. Useful for passing to another command. 	 */
if|if
condition|(
name|flags
operator|&
name|PATHSPEC_PREFIX_ORIGIN
condition|)
block|{
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|strbuf_add
argument_list|(
operator|&
name|sb
argument_list|,
name|elt
argument_list|,
name|copyfrom
operator|-
name|elt
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|sb
argument_list|,
name|match
argument_list|)
expr_stmt|;
name|item
operator|->
name|original
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|sb
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
else|else
name|item
operator|->
name|original
operator|=
name|elt
expr_stmt|;
name|item
operator|->
name|len
operator|=
name|strlen
argument_list|(
name|item
operator|->
name|match
argument_list|)
expr_stmt|;
name|item
operator|->
name|prefix
operator|=
name|prefixlen
expr_stmt|;
if|if
condition|(
operator|(
name|flags
operator|&
name|PATHSPEC_STRIP_SUBMODULE_SLASH_CHEAP
operator|)
operator|&&
operator|(
name|item
operator|->
name|len
operator|>=
literal|1
operator|&&
name|item
operator|->
name|match
index|[
name|item
operator|->
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
operator|)
operator|&&
operator|(
name|i
operator|=
name|cache_name_pos
argument_list|(
name|item
operator|->
name|match
argument_list|,
name|item
operator|->
name|len
operator|-
literal|1
argument_list|)
operator|)
operator|>=
literal|0
operator|&&
name|S_ISGITLINK
argument_list|(
name|active_cache
index|[
name|i
index|]
operator|->
name|ce_mode
argument_list|)
condition|)
block|{
name|item
operator|->
name|len
operator|--
expr_stmt|;
name|match
index|[
name|item
operator|->
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
if|if
condition|(
name|flags
operator|&
name|PATHSPEC_STRIP_SUBMODULE_SLASH_EXPENSIVE
condition|)
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|active_nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|active_cache
index|[
name|i
index|]
decl_stmt|;
name|int
name|ce_len
init|=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|S_ISGITLINK
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|item
operator|->
name|len
operator|<=
name|ce_len
operator|||
name|match
index|[
name|ce_len
index|]
operator|!=
literal|'/'
operator|||
name|memcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|match
argument_list|,
name|ce_len
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|item
operator|->
name|len
operator|==
name|ce_len
operator|+
literal|1
condition|)
block|{
comment|/* strip trailing slash */
name|item
operator|->
name|len
operator|--
expr_stmt|;
name|match
index|[
name|item
operator|->
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
else|else
name|die
argument_list|(
name|_
argument_list|(
literal|"Pathspec '%s' is in submodule '%.*s'"
argument_list|)
argument_list|,
name|elt
argument_list|,
name|ce_len
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|limit_pathspec_to_literal
argument_list|()
condition|)
name|item
operator|->
name|nowildcard_len
operator|=
name|item
operator|->
name|len
expr_stmt|;
else|else
block|{
name|item
operator|->
name|nowildcard_len
operator|=
name|simple_length
argument_list|(
name|item
operator|->
name|match
argument_list|)
expr_stmt|;
if|if
condition|(
name|item
operator|->
name|nowildcard_len
operator|<
name|prefixlen
condition|)
name|item
operator|->
name|nowildcard_len
operator|=
name|prefixlen
expr_stmt|;
block|}
name|item
operator|->
name|flags
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|item
operator|->
name|nowildcard_len
operator|<
name|item
operator|->
name|len
operator|&&
name|item
operator|->
name|match
index|[
name|item
operator|->
name|nowildcard_len
index|]
operator|==
literal|'*'
operator|&&
name|no_wildcard
argument_list|(
name|item
operator|->
name|match
operator|+
name|item
operator|->
name|nowildcard_len
operator|+
literal|1
argument_list|)
condition|)
name|item
operator|->
name|flags
operator||=
name|PATHSPEC_ONESTAR
expr_stmt|;
comment|/* sanity checks, pathspec matchers assume these are sane */
name|assert
argument_list|(
name|item
operator|->
name|nowildcard_len
operator|<=
name|item
operator|->
name|len
operator|&&
name|item
operator|->
name|prefix
operator|<=
name|item
operator|->
name|len
argument_list|)
expr_stmt|;
return|return
name|magic
return|;
block|}
end_function

begin_function
DECL|function|pathspec_item_cmp
specifier|static
name|int
name|pathspec_item_cmp
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
name|pathspec_item
modifier|*
name|a
decl_stmt|,
modifier|*
name|b
decl_stmt|;
name|a
operator|=
operator|(
expr|struct
name|pathspec_item
operator|*
operator|)
name|a_
expr_stmt|;
name|b
operator|=
operator|(
expr|struct
name|pathspec_item
operator|*
operator|)
name|b_
expr_stmt|;
return|return
name|strcmp
argument_list|(
name|a
operator|->
name|match
argument_list|,
name|b
operator|->
name|match
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|unsupported_magic
specifier|static
name|void
name|NORETURN
name|unsupported_magic
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
name|unsigned
name|magic
parameter_list|,
name|unsigned
name|short_magic
parameter_list|)
block|{
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|i
decl_stmt|,
name|n
decl_stmt|;
for|for
control|(
name|n
operator|=
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ARRAY_SIZE
argument_list|(
name|pathspec_magic
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|struct
name|pathspec_magic
modifier|*
name|m
init|=
name|pathspec_magic
operator|+
name|i
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|magic
operator|&
name|m
operator|->
name|bit
operator|)
condition|)
continue|continue;
if|if
condition|(
name|sb
operator|.
name|len
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|sb
argument_list|,
literal|" "
argument_list|)
expr_stmt|;
if|if
condition|(
name|short_magic
operator|&
name|m
operator|->
name|bit
condition|)
name|strbuf_addf
argument_list|(
operator|&
name|sb
argument_list|,
literal|"'%c'"
argument_list|,
name|m
operator|->
name|mnemonic
argument_list|)
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
operator|&
name|sb
argument_list|,
literal|"'%s'"
argument_list|,
name|m
operator|->
name|name
argument_list|)
expr_stmt|;
name|n
operator|++
expr_stmt|;
block|}
comment|/* 	 * We may want to substitute "this command" with a command 	 * name. E.g. when add--interactive dies when running 	 * "checkout -p" 	 */
name|die
argument_list|(
name|_
argument_list|(
literal|"%s: pathspec magic not supported by this command: %s"
argument_list|)
argument_list|,
name|pattern
argument_list|,
name|sb
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * Given command line arguments and a prefix, convert the input to  * pathspec. die() if any magic in magic_mask is used.  */
end_comment

begin_function
DECL|function|parse_pathspec
name|void
name|parse_pathspec
parameter_list|(
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|,
name|unsigned
name|magic_mask
parameter_list|,
name|unsigned
name|flags
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|struct
name|pathspec_item
modifier|*
name|item
decl_stmt|;
specifier|const
name|char
modifier|*
name|entry
init|=
name|argv
condition|?
operator|*
name|argv
else|:
name|NULL
decl_stmt|;
name|int
name|i
decl_stmt|,
name|n
decl_stmt|,
name|prefixlen
decl_stmt|;
name|memset
argument_list|(
name|pathspec
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|pathspec
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|PATHSPEC_MAXDEPTH_VALID
condition|)
name|pathspec
operator|->
name|magic
operator||=
name|PATHSPEC_MAXDEPTH
expr_stmt|;
comment|/* No arguments, no prefix -> no pathspec */
if|if
condition|(
operator|!
name|entry
operator|&&
operator|!
name|prefix
condition|)
return|return;
if|if
condition|(
operator|(
name|flags
operator|&
name|PATHSPEC_PREFER_CWD
operator|)
operator|&&
operator|(
name|flags
operator|&
name|PATHSPEC_PREFER_FULL
operator|)
condition|)
name|die
argument_list|(
literal|"BUG: PATHSPEC_PREFER_CWD and PATHSPEC_PREFER_FULL are incompatible"
argument_list|)
expr_stmt|;
comment|/* No arguments with prefix -> prefix pathspec */
if|if
condition|(
operator|!
name|entry
condition|)
block|{
specifier|static
specifier|const
name|char
modifier|*
name|raw
index|[
literal|2
index|]
decl_stmt|;
if|if
condition|(
name|flags
operator|&
name|PATHSPEC_PREFER_FULL
condition|)
return|return;
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|PATHSPEC_PREFER_CWD
operator|)
condition|)
name|die
argument_list|(
literal|"BUG: PATHSPEC_PREFER_CWD requires arguments"
argument_list|)
expr_stmt|;
name|pathspec
operator|->
name|items
operator|=
name|item
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|item
argument_list|)
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|item
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|item
argument_list|)
argument_list|)
expr_stmt|;
name|item
operator|->
name|match
operator|=
name|prefix
expr_stmt|;
name|item
operator|->
name|original
operator|=
name|prefix
expr_stmt|;
name|item
operator|->
name|nowildcard_len
operator|=
name|item
operator|->
name|len
operator|=
name|strlen
argument_list|(
name|prefix
argument_list|)
expr_stmt|;
name|item
operator|->
name|prefix
operator|=
name|item
operator|->
name|len
expr_stmt|;
name|raw
index|[
literal|0
index|]
operator|=
name|prefix
expr_stmt|;
name|raw
index|[
literal|1
index|]
operator|=
name|NULL
expr_stmt|;
name|pathspec
operator|->
name|nr
operator|=
literal|1
expr_stmt|;
name|pathspec
operator|->
name|_raw
operator|=
name|raw
expr_stmt|;
return|return;
block|}
name|n
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|argv
index|[
name|n
index|]
condition|)
name|n
operator|++
expr_stmt|;
name|pathspec
operator|->
name|nr
operator|=
name|n
expr_stmt|;
name|pathspec
operator|->
name|items
operator|=
name|item
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|item
argument_list|)
operator|*
name|n
argument_list|)
expr_stmt|;
name|pathspec
operator|->
name|_raw
operator|=
name|argv
expr_stmt|;
name|prefixlen
operator|=
name|prefix
condition|?
name|strlen
argument_list|(
name|prefix
argument_list|)
else|:
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|n
condition|;
name|i
operator|++
control|)
block|{
name|unsigned
name|short_magic
decl_stmt|;
name|entry
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
name|item
index|[
name|i
index|]
operator|.
name|magic
operator|=
name|prefix_pathspec
argument_list|(
name|item
operator|+
name|i
argument_list|,
operator|&
name|short_magic
argument_list|,
name|argv
operator|+
name|i
argument_list|,
name|flags
argument_list|,
name|prefix
argument_list|,
name|prefixlen
argument_list|,
name|entry
argument_list|)
expr_stmt|;
if|if
condition|(
name|item
index|[
name|i
index|]
operator|.
name|magic
operator|&
name|magic_mask
condition|)
name|unsupported_magic
argument_list|(
name|entry
argument_list|,
name|item
index|[
name|i
index|]
operator|.
name|magic
operator|&
name|magic_mask
argument_list|,
name|short_magic
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|flags
operator|&
name|PATHSPEC_SYMLINK_LEADING_PATH
operator|)
operator|&&
name|has_symlink_leading_path
argument_list|(
name|item
index|[
name|i
index|]
operator|.
name|match
argument_list|,
name|item
index|[
name|i
index|]
operator|.
name|len
argument_list|)
condition|)
block|{
name|die
argument_list|(
name|_
argument_list|(
literal|"pathspec '%s' is beyond a symbolic link"
argument_list|)
argument_list|,
name|entry
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|item
index|[
name|i
index|]
operator|.
name|nowildcard_len
operator|<
name|item
index|[
name|i
index|]
operator|.
name|len
condition|)
name|pathspec
operator|->
name|has_wildcard
operator|=
literal|1
expr_stmt|;
name|pathspec
operator|->
name|magic
operator||=
name|item
index|[
name|i
index|]
operator|.
name|magic
expr_stmt|;
block|}
if|if
condition|(
name|pathspec
operator|->
name|magic
operator|&
name|PATHSPEC_MAXDEPTH
condition|)
block|{
if|if
condition|(
name|flags
operator|&
name|PATHSPEC_KEEP_ORDER
condition|)
name|die
argument_list|(
literal|"BUG: PATHSPEC_MAXDEPTH_VALID and PATHSPEC_KEEP_ORDER are incompatible"
argument_list|)
expr_stmt|;
name|qsort
argument_list|(
name|pathspec
operator|->
name|items
argument_list|,
name|pathspec
operator|->
name|nr
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|pathspec_item
argument_list|)
argument_list|,
name|pathspec_item_cmp
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/*  * N.B. get_pathspec() is deprecated in favor of the "struct pathspec"  * based interface - see pathspec.c:parse_pathspec().  *  * Arguments:  *  - prefix - a path relative to the root of the working tree  *  - pathspec - a list of paths underneath the prefix path  *  * Iterates over pathspec, prepending each path with prefix,  * and return the resulting list.  *  * If pathspec is empty, return a singleton list containing prefix.  *  * If pathspec and prefix are both empty, return an empty list.  *  * This is typically used by built-in commands such as add.c, in order  * to normalize argv arguments provided to the built-in into a list of  * paths to process, all relative to the root of the working tree.  */
end_comment

begin_function
DECL|function|get_pathspec
specifier|const
name|char
modifier|*
modifier|*
name|get_pathspec
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
block|{
name|struct
name|pathspec
name|ps
decl_stmt|;
name|parse_pathspec
argument_list|(
operator|&
name|ps
argument_list|,
name|PATHSPEC_ALL_MAGIC
operator|&
operator|~
name|PATHSPEC_FROMTOP
argument_list|,
name|PATHSPEC_PREFER_CWD
argument_list|,
name|prefix
argument_list|,
name|pathspec
argument_list|)
expr_stmt|;
return|return
name|ps
operator|.
name|_raw
return|;
block|}
end_function

begin_function
DECL|function|copy_pathspec
name|void
name|copy_pathspec
parameter_list|(
name|struct
name|pathspec
modifier|*
name|dst
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|src
parameter_list|)
block|{
operator|*
name|dst
operator|=
operator|*
name|src
expr_stmt|;
name|dst
operator|->
name|items
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|pathspec_item
argument_list|)
operator|*
name|dst
operator|->
name|nr
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|dst
operator|->
name|items
argument_list|,
name|src
operator|->
name|items
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|pathspec_item
argument_list|)
operator|*
name|dst
operator|->
name|nr
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|free_pathspec
name|void
name|free_pathspec
parameter_list|(
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|)
block|{
name|free
argument_list|(
name|pathspec
operator|->
name|items
argument_list|)
expr_stmt|;
name|pathspec
operator|->
name|items
operator|=
name|NULL
expr_stmt|;
block|}
end_function

end_unit

