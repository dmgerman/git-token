begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * rev-parse.c  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

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

begin_decl_stmt
DECL|variable|def
specifier|static
name|char
modifier|*
name|def
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|no_revs
specifier|static
name|int
name|no_revs
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|single_rev
specifier|static
name|int
name|single_rev
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|revs_only
specifier|static
name|int
name|revs_only
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|do_rev_argument
specifier|static
name|int
name|do_rev_argument
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|output_revs
specifier|static
name|int
name|output_revs
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|flags_only
specifier|static
name|int
name|flags_only
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|no_flags
specifier|static
name|int
name|no_flags
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|output_sq
specifier|static
name|int
name|output_sq
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|NORMAL
define|#
directive|define
name|NORMAL
value|0
end_define

begin_define
DECL|macro|REVERSED
define|#
directive|define
name|REVERSED
value|1
end_define

begin_decl_stmt
DECL|variable|show_type
specifier|static
name|int
name|show_type
init|=
name|NORMAL
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|static
name|int
name|get_extended_sha1
parameter_list|(
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Some arguments are relevant "revision" arguments,  * others are about output format or other details.  * This sorts it all out.  */
end_comment

begin_function
DECL|function|is_rev_argument
specifier|static
name|int
name|is_rev_argument
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|)
block|{
specifier|static
specifier|const
name|char
modifier|*
name|rev_args
index|[]
init|=
block|{
literal|"--max-count="
block|,
literal|"--max-age="
block|,
literal|"--min-age="
block|,
literal|"--merge-order"
block|,
name|NULL
block|}
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|p
init|=
name|rev_args
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
specifier|const
name|char
modifier|*
name|str
init|=
operator|*
name|p
operator|++
decl_stmt|;
name|int
name|len
decl_stmt|;
if|if
condition|(
operator|!
name|str
condition|)
return|return
literal|0
return|;
name|len
operator|=
name|strlen
argument_list|(
name|str
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
name|str
argument_list|,
name|len
argument_list|)
condition|)
return|return
literal|1
return|;
block|}
block|}
end_function

begin_function
DECL|function|show
specifier|static
name|void
name|show
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|)
block|{
if|if
condition|(
name|output_sq
condition|)
block|{
name|int
name|sq
init|=
literal|'\''
decl_stmt|,
name|ch
decl_stmt|;
name|putchar
argument_list|(
name|sq
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|ch
operator|=
operator|*
name|arg
operator|++
operator|)
condition|)
block|{
if|if
condition|(
name|ch
operator|==
name|sq
condition|)
name|fputs
argument_list|(
literal|"'\\'"
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
name|ch
argument_list|)
expr_stmt|;
block|}
name|putchar
argument_list|(
name|sq
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
literal|' '
argument_list|)
expr_stmt|;
block|}
else|else
name|puts
argument_list|(
name|arg
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_rev
specifier|static
name|void
name|show_rev
parameter_list|(
name|int
name|type
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
name|no_revs
condition|)
return|return;
name|output_revs
operator|++
expr_stmt|;
comment|/* Hexadecimal string plus possibly a carret; 	 * this does not have to be quoted even under output_sq. 	 */
name|printf
argument_list|(
literal|"%s%s%c"
argument_list|,
name|type
operator|==
name|show_type
condition|?
literal|""
else|:
literal|"^"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|output_sq
condition|?
literal|' '
else|:
literal|'\n'
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_rev_arg
specifier|static
name|void
name|show_rev_arg
parameter_list|(
name|char
modifier|*
name|rev
parameter_list|)
block|{
if|if
condition|(
name|no_revs
condition|)
return|return;
name|show
argument_list|(
name|rev
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_norev
specifier|static
name|void
name|show_norev
parameter_list|(
name|char
modifier|*
name|norev
parameter_list|)
block|{
if|if
condition|(
name|flags_only
condition|)
return|return;
if|if
condition|(
name|revs_only
condition|)
return|return;
name|show
argument_list|(
name|norev
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_arg
specifier|static
name|void
name|show_arg
parameter_list|(
name|char
modifier|*
name|arg
parameter_list|)
block|{
if|if
condition|(
name|no_flags
condition|)
return|return;
if|if
condition|(
name|do_rev_argument
operator|&&
name|is_rev_argument
argument_list|(
name|arg
argument_list|)
condition|)
name|show_rev_arg
argument_list|(
name|arg
argument_list|)
expr_stmt|;
else|else
name|show_norev
argument_list|(
name|arg
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_parent
specifier|static
name|int
name|get_parent
parameter_list|(
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|result
parameter_list|,
name|int
name|idx
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|ret
init|=
name|get_extended_sha1
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|)
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|p
decl_stmt|;
if|if
condition|(
name|ret
condition|)
return|return
name|ret
return|;
name|commit
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|parse_commit
argument_list|(
name|commit
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|idx
condition|)
block|{
name|memcpy
argument_list|(
name|result
argument_list|,
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|p
operator|=
name|commit
operator|->
name|parents
expr_stmt|;
while|while
condition|(
name|p
condition|)
block|{
if|if
condition|(
operator|!
operator|--
name|idx
condition|)
block|{
name|memcpy
argument_list|(
name|result
argument_list|,
name|p
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|p
operator|=
name|p
operator|->
name|next
expr_stmt|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|find_short_object_filename
specifier|static
name|int
name|find_short_object_filename
parameter_list|(
name|int
name|len
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
specifier|static
name|char
name|dirname
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|char
name|hex
index|[
literal|40
index|]
decl_stmt|;
name|DIR
modifier|*
name|dir
decl_stmt|;
name|int
name|found
decl_stmt|;
name|snprintf
argument_list|(
name|dirname
argument_list|,
sizeof|sizeof
argument_list|(
name|dirname
argument_list|)
argument_list|,
literal|"%s/%.2s"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|dir
operator|=
name|opendir
argument_list|(
name|dirname
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|hex
argument_list|,
literal|"%.2s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|found
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|dir
condition|)
block|{
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
while|while
condition|(
operator|(
name|de
operator|=
name|readdir
argument_list|(
name|dir
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
if|if
condition|(
name|strlen
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
operator|!=
literal|38
condition|)
continue|continue;
if|if
condition|(
name|memcmp
argument_list|(
name|de
operator|->
name|d_name
argument_list|,
name|name
operator|+
literal|2
argument_list|,
name|len
operator|-
literal|2
argument_list|)
condition|)
continue|continue;
name|memcpy
argument_list|(
name|hex
operator|+
literal|2
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
literal|38
argument_list|)
expr_stmt|;
if|if
condition|(
operator|++
name|found
operator|>
literal|1
condition|)
break|break;
block|}
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|found
operator|==
literal|1
condition|)
return|return
name|get_sha1_hex
argument_list|(
name|hex
argument_list|,
name|sha1
argument_list|)
operator|==
literal|0
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|match_sha
specifier|static
name|int
name|match_sha
parameter_list|(
name|unsigned
name|len
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|a
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|b
parameter_list|)
block|{
do|do
block|{
if|if
condition|(
operator|*
name|a
operator|!=
operator|*
name|b
condition|)
return|return
literal|0
return|;
name|a
operator|++
expr_stmt|;
name|b
operator|++
expr_stmt|;
name|len
operator|-=
literal|2
expr_stmt|;
block|}
do|while
condition|(
name|len
operator|>
literal|1
condition|)
do|;
if|if
condition|(
name|len
condition|)
if|if
condition|(
operator|(
operator|*
name|a
operator|^
operator|*
name|b
operator|)
operator|&
literal|0xf0
condition|)
return|return
literal|0
return|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|find_short_packed_object
specifier|static
name|int
name|find_short_packed_object
parameter_list|(
name|int
name|len
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|match
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|packed_git
modifier|*
name|p
decl_stmt|;
name|prepare_packed_git
argument_list|()
expr_stmt|;
for|for
control|(
name|p
operator|=
name|packed_git
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
name|unsigned
name|num
init|=
name|num_packed_objects
argument_list|(
name|p
argument_list|)
decl_stmt|;
name|unsigned
name|first
init|=
literal|0
decl_stmt|,
name|last
init|=
name|num
decl_stmt|;
while|while
condition|(
name|first
operator|<
name|last
condition|)
block|{
name|unsigned
name|mid
init|=
operator|(
name|first
operator|+
name|last
operator|)
operator|/
literal|2
decl_stmt|;
name|unsigned
name|char
name|now
index|[
literal|20
index|]
decl_stmt|;
name|int
name|cmp
decl_stmt|;
name|nth_packed_object_sha1
argument_list|(
name|p
argument_list|,
name|mid
argument_list|,
name|now
argument_list|)
expr_stmt|;
name|cmp
operator|=
name|memcmp
argument_list|(
name|match
argument_list|,
name|now
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cmp
condition|)
block|{
name|first
operator|=
name|mid
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
block|{
name|first
operator|=
name|mid
operator|+
literal|1
expr_stmt|;
continue|continue;
block|}
name|last
operator|=
name|mid
expr_stmt|;
block|}
if|if
condition|(
name|first
operator|<
name|num
condition|)
block|{
name|unsigned
name|char
name|now
index|[
literal|20
index|]
decl_stmt|,
name|next
index|[
literal|20
index|]
decl_stmt|;
name|nth_packed_object_sha1
argument_list|(
name|p
argument_list|,
name|first
argument_list|,
name|now
argument_list|)
expr_stmt|;
if|if
condition|(
name|match_sha
argument_list|(
name|len
argument_list|,
name|match
argument_list|,
name|now
argument_list|)
condition|)
block|{
if|if
condition|(
name|nth_packed_object_sha1
argument_list|(
name|p
argument_list|,
name|first
operator|+
literal|1
argument_list|,
name|next
argument_list|)
operator|||
operator|!
name|match_sha
argument_list|(
name|len
argument_list|,
name|match
argument_list|,
name|next
argument_list|)
condition|)
block|{
name|memcpy
argument_list|(
name|sha1
argument_list|,
name|now
argument_list|,
literal|20
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
block|}
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|get_short_sha1
specifier|static
name|int
name|get_short_sha1
parameter_list|(
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|char
name|canonical
index|[
literal|40
index|]
decl_stmt|;
name|unsigned
name|char
name|res
index|[
literal|20
index|]
decl_stmt|;
name|memset
argument_list|(
name|res
argument_list|,
literal|0
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|canonical
argument_list|,
literal|'x'
argument_list|,
literal|40
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
condition|;
name|i
operator|++
control|)
block|{
name|unsigned
name|char
name|c
init|=
name|name
index|[
name|i
index|]
decl_stmt|;
name|unsigned
name|char
name|val
decl_stmt|;
if|if
condition|(
operator|!
name|c
operator|||
name|i
operator|>
literal|40
condition|)
break|break;
if|if
condition|(
name|c
operator|>=
literal|'0'
operator|&&
name|c
operator|<=
literal|'9'
condition|)
name|val
operator|=
name|c
operator|-
literal|'0'
expr_stmt|;
elseif|else
if|if
condition|(
name|c
operator|>=
literal|'a'
operator|&&
name|c
operator|<=
literal|'f'
condition|)
name|val
operator|=
name|c
operator|-
literal|'a'
operator|+
literal|10
expr_stmt|;
elseif|else
if|if
condition|(
name|c
operator|>=
literal|'A'
operator|&&
name|c
operator|<=
literal|'F'
condition|)
block|{
name|val
operator|=
name|c
operator|-
literal|'A'
operator|+
literal|10
expr_stmt|;
name|c
operator|-=
literal|'A'
operator|-
literal|'a'
expr_stmt|;
block|}
else|else
return|return
operator|-
literal|1
return|;
name|canonical
index|[
name|i
index|]
operator|=
name|c
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|i
operator|&
literal|1
operator|)
condition|)
name|val
operator|<<=
literal|4
expr_stmt|;
name|res
index|[
name|i
operator|>>
literal|1
index|]
operator||=
name|val
expr_stmt|;
block|}
if|if
condition|(
name|i
operator|<
literal|4
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|find_short_object_filename
argument_list|(
name|i
argument_list|,
name|canonical
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|find_short_packed_object
argument_list|(
name|i
argument_list|,
name|res
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
literal|0
return|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_comment
comment|/*  * This is like "get_sha1()", except it allows "sha1 expressions",  * notably "xyz^" for "parent of xyz"  */
end_comment

begin_function
DECL|function|get_extended_sha1
specifier|static
name|int
name|get_extended_sha1
parameter_list|(
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|parent
decl_stmt|,
name|ret
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|name
argument_list|)
decl_stmt|;
name|parent
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|len
operator|>
literal|2
operator|&&
name|name
index|[
name|len
operator|-
literal|1
index|]
operator|>=
literal|'0'
operator|&&
name|name
index|[
name|len
operator|-
literal|1
index|]
operator|<=
literal|'9'
condition|)
block|{
name|parent
operator|=
name|name
index|[
name|len
operator|-
literal|1
index|]
operator|-
literal|'0'
expr_stmt|;
name|len
operator|--
expr_stmt|;
block|}
if|if
condition|(
name|len
operator|>
literal|1
operator|&&
name|name
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'^'
condition|)
block|{
name|name
index|[
name|len
operator|-
literal|1
index|]
operator|=
literal|0
expr_stmt|;
name|ret
operator|=
name|get_parent
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|,
name|parent
argument_list|)
expr_stmt|;
name|name
index|[
name|len
operator|-
literal|1
index|]
operator|=
literal|'^'
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
return|return
literal|0
return|;
block|}
name|ret
operator|=
name|get_sha1
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
return|return
literal|0
return|;
return|return
name|get_short_sha1
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|show_default
specifier|static
name|void
name|show_default
parameter_list|(
name|void
parameter_list|)
block|{
name|char
modifier|*
name|s
init|=
name|def
decl_stmt|;
if|if
condition|(
name|s
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|def
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
operator|!
name|get_extended_sha1
argument_list|(
name|s
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|show_rev
argument_list|(
name|NORMAL
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
return|return;
block|}
name|show_arg
argument_list|(
name|s
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|show_reference
specifier|static
name|int
name|show_reference
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
name|show_rev
argument_list|(
name|NORMAL
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
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
decl_stmt|,
name|as_is
init|=
literal|0
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
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
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
name|char
modifier|*
name|dotdot
decl_stmt|;
if|if
condition|(
name|as_is
condition|)
block|{
name|show_norev
argument_list|(
name|arg
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
block|{
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
name|show_default
argument_list|()
expr_stmt|;
if|if
condition|(
name|revs_only
condition|)
break|break;
name|as_is
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--default"
argument_list|)
condition|)
block|{
name|def
operator|=
name|argv
index|[
name|i
operator|+
literal|1
index|]
expr_stmt|;
name|i
operator|++
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
literal|"--revs-only"
argument_list|)
condition|)
block|{
name|revs_only
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
literal|"--no-revs"
argument_list|)
condition|)
block|{
name|no_revs
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
literal|"--flags"
argument_list|)
condition|)
block|{
name|flags_only
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
literal|"--no-flags"
argument_list|)
condition|)
block|{
name|no_flags
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
name|revs_only
operator|=
literal|1
expr_stmt|;
name|do_rev_argument
operator|=
literal|0
expr_stmt|;
name|single_rev
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
literal|"--sq"
argument_list|)
condition|)
block|{
name|output_sq
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
literal|"--not"
argument_list|)
condition|)
block|{
name|show_type
operator|^=
name|REVERSED
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
literal|"--all"
argument_list|)
condition|)
block|{
name|for_each_ref
argument_list|(
name|show_reference
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|show_arg
argument_list|(
name|arg
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|dotdot
operator|=
name|strstr
argument_list|(
name|arg
argument_list|,
literal|".."
argument_list|)
expr_stmt|;
if|if
condition|(
name|dotdot
condition|)
block|{
name|unsigned
name|char
name|end
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|n
init|=
name|dotdot
operator|+
literal|2
decl_stmt|;
operator|*
name|dotdot
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
name|get_extended_sha1
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
operator|*
name|n
condition|)
name|n
operator|=
literal|"HEAD"
expr_stmt|;
if|if
condition|(
operator|!
name|get_extended_sha1
argument_list|(
name|n
argument_list|,
name|end
argument_list|)
condition|)
block|{
if|if
condition|(
name|no_revs
condition|)
continue|continue;
name|def
operator|=
name|NULL
expr_stmt|;
name|show_rev
argument_list|(
name|NORMAL
argument_list|,
name|end
argument_list|)
expr_stmt|;
name|show_rev
argument_list|(
name|REVERSED
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
continue|continue;
block|}
block|}
operator|*
name|dotdot
operator|=
literal|'.'
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|get_extended_sha1
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
if|if
condition|(
name|no_revs
condition|)
continue|continue;
name|def
operator|=
name|NULL
expr_stmt|;
name|show_rev
argument_list|(
name|NORMAL
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|*
name|arg
operator|==
literal|'^'
operator|&&
operator|!
name|get_extended_sha1
argument_list|(
name|arg
operator|+
literal|1
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
if|if
condition|(
name|no_revs
condition|)
continue|continue;
name|def
operator|=
name|NULL
expr_stmt|;
name|show_rev
argument_list|(
name|REVERSED
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|show_default
argument_list|()
expr_stmt|;
name|show_norev
argument_list|(
name|arg
argument_list|)
expr_stmt|;
block|}
name|show_default
argument_list|()
expr_stmt|;
if|if
condition|(
name|single_rev
operator|&&
name|output_revs
operator|!=
literal|1
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Needed a single revision\n"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

