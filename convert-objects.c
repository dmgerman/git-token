begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_struct
DECL|struct|entry
struct|struct
name|entry
block|{
DECL|member|old_sha1
name|unsigned
name|char
name|old_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|new_sha1
name|unsigned
name|char
name|new_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|converted
name|int
name|converted
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|MAXOBJECTS
define|#
directive|define
name|MAXOBJECTS
value|(1000000)
end_define

begin_decl_stmt
DECL|variable|convert
specifier|static
name|struct
name|entry
modifier|*
name|convert
index|[
name|MAXOBJECTS
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nr_convert
specifier|static
name|int
name|nr_convert
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|static
name|struct
name|entry
modifier|*
name|convert_entry
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|insert_new
specifier|static
name|struct
name|entry
modifier|*
name|insert_new
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|pos
parameter_list|)
block|{
name|struct
name|entry
modifier|*
name|new
init|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|entry
argument_list|)
argument_list|)
decl_stmt|;
name|hashcpy
argument_list|(
name|new
operator|->
name|old_sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|memmove
argument_list|(
name|convert
operator|+
name|pos
operator|+
literal|1
argument_list|,
name|convert
operator|+
name|pos
argument_list|,
operator|(
name|nr_convert
operator|-
name|pos
operator|)
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|entry
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|convert
index|[
name|pos
index|]
operator|=
name|new
expr_stmt|;
name|nr_convert
operator|++
expr_stmt|;
if|if
condition|(
name|nr_convert
operator|==
name|MAXOBJECTS
condition|)
name|die
argument_list|(
literal|"you're kidding me - hit maximum object limit"
argument_list|)
expr_stmt|;
return|return
name|new
return|;
block|}
end_function

begin_function
DECL|function|lookup_entry
specifier|static
name|struct
name|entry
modifier|*
name|lookup_entry
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|low
init|=
literal|0
decl_stmt|,
name|high
init|=
name|nr_convert
decl_stmt|;
while|while
condition|(
name|low
operator|<
name|high
condition|)
block|{
name|int
name|next
init|=
operator|(
name|low
operator|+
name|high
operator|)
operator|/
literal|2
decl_stmt|;
name|struct
name|entry
modifier|*
name|n
init|=
name|convert
index|[
name|next
index|]
decl_stmt|;
name|int
name|cmp
init|=
name|hashcmp
argument_list|(
name|sha1
argument_list|,
name|n
operator|->
name|old_sha1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|cmp
condition|)
return|return
name|n
return|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
block|{
name|high
operator|=
name|next
expr_stmt|;
continue|continue;
block|}
name|low
operator|=
name|next
operator|+
literal|1
expr_stmt|;
block|}
return|return
name|insert_new
argument_list|(
name|sha1
argument_list|,
name|low
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|convert_binary_sha1
specifier|static
name|void
name|convert_binary_sha1
parameter_list|(
name|void
modifier|*
name|buffer
parameter_list|)
block|{
name|struct
name|entry
modifier|*
name|entry
init|=
name|convert_entry
argument_list|(
name|buffer
argument_list|)
decl_stmt|;
name|hashcpy
argument_list|(
name|buffer
argument_list|,
name|entry
operator|->
name|new_sha1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|convert_ascii_sha1
specifier|static
name|void
name|convert_ascii_sha1
parameter_list|(
name|void
modifier|*
name|buffer
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
name|entry
modifier|*
name|entry
decl_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|buffer
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"expected sha1, got '%s'"
argument_list|,
operator|(
name|char
operator|*
operator|)
name|buffer
argument_list|)
expr_stmt|;
name|entry
operator|=
name|convert_entry
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|buffer
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|new_sha1
argument_list|)
argument_list|,
literal|40
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|convert_mode
specifier|static
name|unsigned
name|int
name|convert_mode
parameter_list|(
name|unsigned
name|int
name|mode
parameter_list|)
block|{
name|unsigned
name|int
name|newmode
decl_stmt|;
name|newmode
operator|=
name|mode
operator|&
name|S_IFMT
expr_stmt|;
if|if
condition|(
name|S_ISREG
argument_list|(
name|mode
argument_list|)
condition|)
name|newmode
operator||=
operator|(
name|mode
operator|&
literal|0100
operator|)
condition|?
literal|0755
else|:
literal|0644
expr_stmt|;
return|return
name|newmode
return|;
block|}
end_function

begin_function
DECL|function|write_subdirectory
specifier|static
name|int
name|write_subdirectory
parameter_list|(
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|)
block|{
name|char
modifier|*
name|new
init|=
name|xmalloc
argument_list|(
name|size
argument_list|)
decl_stmt|;
name|unsigned
name|long
name|newlen
init|=
literal|0
decl_stmt|;
name|unsigned
name|long
name|used
decl_stmt|;
name|used
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|size
condition|)
block|{
name|int
name|len
init|=
literal|21
operator|+
name|strlen
argument_list|(
name|buffer
argument_list|)
decl_stmt|;
name|char
modifier|*
name|path
init|=
name|strchr
argument_list|(
name|buffer
argument_list|,
literal|' '
argument_list|)
decl_stmt|;
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
name|char
modifier|*
name|slash
decl_stmt|,
modifier|*
name|origpath
decl_stmt|;
if|if
condition|(
operator|!
name|path
operator|||
name|sscanf
argument_list|(
name|buffer
argument_list|,
literal|"%o"
argument_list|,
operator|&
name|mode
argument_list|)
operator|!=
literal|1
condition|)
name|die
argument_list|(
literal|"bad tree conversion"
argument_list|)
expr_stmt|;
name|mode
operator|=
name|convert_mode
argument_list|(
name|mode
argument_list|)
expr_stmt|;
name|path
operator|++
expr_stmt|;
if|if
condition|(
name|memcmp
argument_list|(
name|path
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
condition|)
break|break;
name|origpath
operator|=
name|path
expr_stmt|;
name|path
operator|+=
name|baselen
expr_stmt|;
name|slash
operator|=
name|strchr
argument_list|(
name|path
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|slash
condition|)
block|{
name|newlen
operator|+=
name|sprintf
argument_list|(
name|new
operator|+
name|newlen
argument_list|,
literal|"%o %s"
argument_list|,
name|mode
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|new
index|[
name|newlen
operator|++
index|]
operator|=
literal|'\0'
expr_stmt|;
name|hashcpy
argument_list|(
operator|(
name|unsigned
name|char
operator|*
operator|)
name|new
operator|+
name|newlen
argument_list|,
operator|(
name|unsigned
name|char
operator|*
operator|)
name|buffer
operator|+
name|len
operator|-
literal|20
argument_list|)
expr_stmt|;
name|newlen
operator|+=
literal|20
expr_stmt|;
name|used
operator|+=
name|len
expr_stmt|;
name|size
operator|-=
name|len
expr_stmt|;
name|buffer
operator|=
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
name|len
expr_stmt|;
continue|continue;
block|}
name|newlen
operator|+=
name|sprintf
argument_list|(
name|new
operator|+
name|newlen
argument_list|,
literal|"%o %.*s"
argument_list|,
name|S_IFDIR
argument_list|,
call|(
name|int
call|)
argument_list|(
name|slash
operator|-
name|path
argument_list|)
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|new
index|[
name|newlen
operator|++
index|]
operator|=
literal|0
expr_stmt|;
name|sha1
operator|=
operator|(
name|unsigned
name|char
operator|*
operator|)
operator|(
name|new
operator|+
name|newlen
operator|)
expr_stmt|;
name|newlen
operator|+=
literal|20
expr_stmt|;
name|len
operator|=
name|write_subdirectory
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|,
name|origpath
argument_list|,
name|slash
operator|-
name|origpath
operator|+
literal|1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|used
operator|+=
name|len
expr_stmt|;
name|size
operator|-=
name|len
expr_stmt|;
name|buffer
operator|=
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
name|len
expr_stmt|;
block|}
name|write_sha1_file
argument_list|(
name|new
argument_list|,
name|newlen
argument_list|,
name|tree_type
argument_list|,
name|result_sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|new
argument_list|)
expr_stmt|;
return|return
name|used
return|;
block|}
end_function

begin_function
DECL|function|convert_tree
specifier|static
name|void
name|convert_tree
parameter_list|(
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|)
block|{
name|void
modifier|*
name|orig_buffer
init|=
name|buffer
decl_stmt|;
name|unsigned
name|long
name|orig_size
init|=
name|size
decl_stmt|;
while|while
condition|(
name|size
condition|)
block|{
name|int
name|len
init|=
literal|1
operator|+
name|strlen
argument_list|(
name|buffer
argument_list|)
decl_stmt|;
name|convert_binary_sha1
argument_list|(
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
name|len
argument_list|)
expr_stmt|;
name|len
operator|+=
literal|20
expr_stmt|;
if|if
condition|(
name|len
operator|>
name|size
condition|)
name|die
argument_list|(
literal|"corrupt tree object"
argument_list|)
expr_stmt|;
name|size
operator|-=
name|len
expr_stmt|;
name|buffer
operator|=
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
name|len
expr_stmt|;
block|}
name|write_subdirectory
argument_list|(
name|orig_buffer
argument_list|,
name|orig_size
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|,
name|result_sha1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|parse_oldstyle_date
specifier|static
name|unsigned
name|long
name|parse_oldstyle_date
parameter_list|(
specifier|const
name|char
modifier|*
name|buf
parameter_list|)
block|{
name|char
name|c
decl_stmt|,
modifier|*
name|p
decl_stmt|;
name|char
name|buffer
index|[
literal|100
index|]
decl_stmt|;
name|struct
name|tm
name|tm
decl_stmt|;
specifier|const
name|char
modifier|*
name|formats
index|[]
init|=
block|{
literal|"%c"
block|,
literal|"%a %b %d %T"
block|,
literal|"%Z"
block|,
literal|"%Y"
block|,
literal|" %Y"
block|,
name|NULL
block|}
decl_stmt|;
comment|/* We only ever did two timezones in the bad old format .. */
specifier|const
name|char
modifier|*
name|timezones
index|[]
init|=
block|{
literal|"PDT"
block|,
literal|"PST"
block|,
literal|"CEST"
block|,
name|NULL
block|}
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|fmt
init|=
name|formats
decl_stmt|;
name|p
operator|=
name|buffer
expr_stmt|;
while|while
condition|(
name|isspace
argument_list|(
name|c
operator|=
operator|*
name|buf
argument_list|)
condition|)
name|buf
operator|++
expr_stmt|;
while|while
condition|(
operator|(
name|c
operator|=
operator|*
name|buf
operator|++
operator|)
operator|!=
literal|'\n'
condition|)
operator|*
name|p
operator|++
operator|=
name|c
expr_stmt|;
operator|*
name|p
operator|++
operator|=
literal|0
expr_stmt|;
name|buf
operator|=
name|buffer
expr_stmt|;
name|memset
argument_list|(
operator|&
name|tm
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|tm
argument_list|)
argument_list|)
expr_stmt|;
do|do
block|{
specifier|const
name|char
modifier|*
name|next
init|=
name|strptime
argument_list|(
name|buf
argument_list|,
operator|*
name|fmt
argument_list|,
operator|&
name|tm
argument_list|)
decl_stmt|;
if|if
condition|(
name|next
condition|)
block|{
if|if
condition|(
operator|!
operator|*
name|next
condition|)
return|return
name|mktime
argument_list|(
operator|&
name|tm
argument_list|)
return|;
name|buf
operator|=
name|next
expr_stmt|;
block|}
else|else
block|{
specifier|const
name|char
modifier|*
modifier|*
name|p
init|=
name|timezones
decl_stmt|;
while|while
condition|(
name|isspace
argument_list|(
operator|*
name|buf
argument_list|)
condition|)
name|buf
operator|++
expr_stmt|;
while|while
condition|(
operator|*
name|p
condition|)
block|{
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|buf
argument_list|,
operator|*
name|p
argument_list|,
name|strlen
argument_list|(
operator|*
name|p
argument_list|)
argument_list|)
condition|)
block|{
name|buf
operator|+=
name|strlen
argument_list|(
operator|*
name|p
argument_list|)
expr_stmt|;
break|break;
block|}
name|p
operator|++
expr_stmt|;
block|}
block|}
name|fmt
operator|++
expr_stmt|;
block|}
do|while
condition|(
operator|*
name|buf
operator|&&
operator|*
name|fmt
condition|)
do|;
name|printf
argument_list|(
literal|"left: %s\n"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
return|return
name|mktime
argument_list|(
operator|&
name|tm
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|convert_date_line
specifier|static
name|int
name|convert_date_line
parameter_list|(
name|char
modifier|*
name|dst
parameter_list|,
name|void
modifier|*
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
modifier|*
name|sp
parameter_list|)
block|{
name|unsigned
name|long
name|size
init|=
operator|*
name|sp
decl_stmt|;
name|char
modifier|*
name|line
init|=
operator|*
name|buf
decl_stmt|;
name|char
modifier|*
name|next
init|=
name|strchr
argument_list|(
name|line
argument_list|,
literal|'\n'
argument_list|)
decl_stmt|;
name|char
modifier|*
name|date
init|=
name|strchr
argument_list|(
name|line
argument_list|,
literal|'>'
argument_list|)
decl_stmt|;
name|int
name|len
decl_stmt|;
if|if
condition|(
operator|!
name|next
operator|||
operator|!
name|date
condition|)
name|die
argument_list|(
literal|"missing or bad author/committer line %s"
argument_list|,
name|line
argument_list|)
expr_stmt|;
name|next
operator|++
expr_stmt|;
name|date
operator|+=
literal|2
expr_stmt|;
operator|*
name|buf
operator|=
name|next
expr_stmt|;
operator|*
name|sp
operator|=
name|size
operator|-
operator|(
name|next
operator|-
name|line
operator|)
expr_stmt|;
name|len
operator|=
name|date
operator|-
name|line
expr_stmt|;
name|memcpy
argument_list|(
name|dst
argument_list|,
name|line
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|dst
operator|+=
name|len
expr_stmt|;
comment|/* Is it already in new format? */
if|if
condition|(
name|isdigit
argument_list|(
operator|*
name|date
argument_list|)
condition|)
block|{
name|int
name|datelen
init|=
name|next
operator|-
name|date
decl_stmt|;
name|memcpy
argument_list|(
name|dst
argument_list|,
name|date
argument_list|,
name|datelen
argument_list|)
expr_stmt|;
return|return
name|len
operator|+
name|datelen
return|;
block|}
comment|/* 	 * Hacky hacky: one of the sparse old-style commits does not have 	 * any date at all, but we can fake it by using the committer date. 	 */
if|if
condition|(
operator|*
name|date
operator|==
literal|'\n'
operator|&&
name|strchr
argument_list|(
name|next
argument_list|,
literal|'>'
argument_list|)
condition|)
name|date
operator|=
name|strchr
argument_list|(
name|next
argument_list|,
literal|'>'
argument_list|)
operator|+
literal|2
expr_stmt|;
return|return
name|len
operator|+
name|sprintf
argument_list|(
name|dst
argument_list|,
literal|"%lu -0700\n"
argument_list|,
name|parse_oldstyle_date
argument_list|(
name|date
argument_list|)
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|convert_date
specifier|static
name|void
name|convert_date
parameter_list|(
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|)
block|{
name|char
modifier|*
name|new
init|=
name|xmalloc
argument_list|(
name|size
operator|+
literal|100
argument_list|)
decl_stmt|;
name|unsigned
name|long
name|newlen
init|=
literal|0
decl_stmt|;
comment|/* "tree<sha1>\n" */
name|memcpy
argument_list|(
name|new
operator|+
name|newlen
argument_list|,
name|buffer
argument_list|,
literal|46
argument_list|)
expr_stmt|;
name|newlen
operator|+=
literal|46
expr_stmt|;
name|buffer
operator|=
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
literal|46
expr_stmt|;
name|size
operator|-=
literal|46
expr_stmt|;
comment|/* "parent<sha1>\n" */
while|while
condition|(
operator|!
name|memcmp
argument_list|(
name|buffer
argument_list|,
literal|"parent "
argument_list|,
literal|7
argument_list|)
condition|)
block|{
name|memcpy
argument_list|(
name|new
operator|+
name|newlen
argument_list|,
name|buffer
argument_list|,
literal|48
argument_list|)
expr_stmt|;
name|newlen
operator|+=
literal|48
expr_stmt|;
name|buffer
operator|=
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
literal|48
expr_stmt|;
name|size
operator|-=
literal|48
expr_stmt|;
block|}
comment|/* "author xyz<xyz> date" */
name|newlen
operator|+=
name|convert_date_line
argument_list|(
name|new
operator|+
name|newlen
argument_list|,
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
comment|/* "committer xyz<xyz> date" */
name|newlen
operator|+=
name|convert_date_line
argument_list|(
name|new
operator|+
name|newlen
argument_list|,
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
comment|/* Rest */
name|memcpy
argument_list|(
name|new
operator|+
name|newlen
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|newlen
operator|+=
name|size
expr_stmt|;
name|write_sha1_file
argument_list|(
name|new
argument_list|,
name|newlen
argument_list|,
name|commit_type
argument_list|,
name|result_sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|new
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|convert_commit
specifier|static
name|void
name|convert_commit
parameter_list|(
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|)
block|{
name|void
modifier|*
name|orig_buffer
init|=
name|buffer
decl_stmt|;
name|unsigned
name|long
name|orig_size
init|=
name|size
decl_stmt|;
if|if
condition|(
name|memcmp
argument_list|(
name|buffer
argument_list|,
literal|"tree "
argument_list|,
literal|5
argument_list|)
condition|)
name|die
argument_list|(
literal|"Bad commit '%s'"
argument_list|,
operator|(
name|char
operator|*
operator|)
name|buffer
argument_list|)
expr_stmt|;
name|convert_ascii_sha1
argument_list|(
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
literal|5
argument_list|)
expr_stmt|;
name|buffer
operator|=
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
literal|46
expr_stmt|;
comment|/* "tree " + "hex sha1" + "\n" */
while|while
condition|(
operator|!
name|memcmp
argument_list|(
name|buffer
argument_list|,
literal|"parent "
argument_list|,
literal|7
argument_list|)
condition|)
block|{
name|convert_ascii_sha1
argument_list|(
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
literal|7
argument_list|)
expr_stmt|;
name|buffer
operator|=
operator|(
name|char
operator|*
operator|)
name|buffer
operator|+
literal|48
expr_stmt|;
block|}
name|convert_date
argument_list|(
name|orig_buffer
argument_list|,
name|orig_size
argument_list|,
name|result_sha1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|convert_entry
specifier|static
name|struct
name|entry
modifier|*
name|convert_entry
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|entry
modifier|*
name|entry
init|=
name|lookup_entry
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|void
modifier|*
name|buffer
decl_stmt|,
modifier|*
name|data
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
if|if
condition|(
name|entry
operator|->
name|converted
condition|)
return|return
name|entry
return|;
name|data
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
operator|&
name|type
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|data
condition|)
name|die
argument_list|(
literal|"unable to read object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|buffer
operator|=
name|xmalloc
argument_list|(
name|size
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|buffer
argument_list|,
name|data
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|==
name|OBJ_BLOB
condition|)
block|{
name|write_sha1_file
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|,
name|blob_type
argument_list|,
name|entry
operator|->
name|new_sha1
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|type
operator|==
name|OBJ_TREE
condition|)
name|convert_tree
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|,
name|entry
operator|->
name|new_sha1
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|type
operator|==
name|OBJ_COMMIT
condition|)
name|convert_commit
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|,
name|entry
operator|->
name|new_sha1
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
literal|"unknown object type %d in %s"
argument_list|,
name|type
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|entry
operator|->
name|converted
operator|=
literal|1
expr_stmt|;
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
return|return
name|entry
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
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|entry
modifier|*
name|entry
decl_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|2
condition|)
name|usage
argument_list|(
literal|"git-convert-objects<sha1>"
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|entry
operator|=
name|convert_entry
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"new sha1: %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|new_sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

