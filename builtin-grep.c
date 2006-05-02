begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git grep"  *  * Copyright (c) 2006 Junio C Hamano  */
end_comment

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
file|"tree.h"
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
file|"tree-walk.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|<regex.h>
end_include

begin_include
include|#
directive|include
file|<fnmatch.h>
end_include

begin_comment
comment|/*  * git grep pathspecs are somewhat different from diff-tree pathspecs;  * pathname wildcards are allowed.  */
end_comment

begin_function
DECL|function|pathspec_matches
specifier|static
name|int
name|pathspec_matches
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|paths
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|int
name|namelen
decl_stmt|,
name|i
decl_stmt|;
if|if
condition|(
operator|!
name|paths
operator|||
operator|!
operator|*
name|paths
condition|)
return|return
literal|1
return|;
name|namelen
operator|=
name|strlen
argument_list|(
name|name
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|paths
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|match
init|=
name|paths
index|[
name|i
index|]
decl_stmt|;
name|int
name|matchlen
init|=
name|strlen
argument_list|(
name|match
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|slash
decl_stmt|,
modifier|*
name|cp
decl_stmt|;
if|if
condition|(
operator|(
name|matchlen
operator|<=
name|namelen
operator|)
operator|&&
operator|!
name|strncmp
argument_list|(
name|name
argument_list|,
name|match
argument_list|,
name|matchlen
argument_list|)
operator|&&
operator|(
name|match
index|[
name|matchlen
operator|-
literal|1
index|]
operator|==
literal|'/'
operator|||
name|name
index|[
name|matchlen
index|]
operator|==
literal|'\0'
operator|||
name|name
index|[
name|matchlen
index|]
operator|==
literal|'/'
operator|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
name|fnmatch
argument_list|(
name|match
argument_list|,
name|name
argument_list|,
literal|0
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|name
index|[
name|namelen
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
continue|continue;
comment|/* We are being asked if the name directory is worth 		 * descending into. 		 * 		 * Find the longest leading directory name that does 		 * not have metacharacter in the pathspec; the name 		 * we are looking at must overlap with that directory. 		 */
for|for
control|(
name|cp
operator|=
name|match
operator|,
name|slash
operator|=
name|NULL
init|;
name|cp
operator|-
name|match
operator|<
name|matchlen
condition|;
name|cp
operator|++
control|)
block|{
name|char
name|ch
init|=
operator|*
name|cp
decl_stmt|;
if|if
condition|(
name|ch
operator|==
literal|'/'
condition|)
name|slash
operator|=
name|cp
expr_stmt|;
if|if
condition|(
name|ch
operator|==
literal|'*'
operator|||
name|ch
operator|==
literal|'['
condition|)
break|break;
block|}
if|if
condition|(
operator|!
name|slash
condition|)
name|slash
operator|=
name|match
expr_stmt|;
comment|/* toplevel */
else|else
name|slash
operator|++
expr_stmt|;
if|if
condition|(
name|namelen
operator|<=
name|slash
operator|-
name|match
condition|)
block|{
comment|/* Looking at "Documentation/" and 			 * the pattern says "Documentation/howto/", or 			 * "Documentation/diff*.txt". 			 */
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|match
argument_list|,
name|name
argument_list|,
name|namelen
argument_list|)
condition|)
return|return
literal|1
return|;
block|}
else|else
block|{
comment|/* Looking at "Documentation/howto/" and 			 * the pattern says "Documentation/h*". 			 */
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|match
argument_list|,
name|name
argument_list|,
name|slash
operator|-
name|match
argument_list|)
condition|)
return|return
literal|1
return|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_struct
DECL|struct|grep_opt
struct|struct
name|grep_opt
block|{
DECL|member|pattern
specifier|const
name|char
modifier|*
name|pattern
decl_stmt|;
DECL|member|regexp
name|regex_t
name|regexp
decl_stmt|;
DECL|member|linenum
name|unsigned
name|linenum
range|:
literal|1
decl_stmt|;
DECL|member|invert
name|unsigned
name|invert
range|:
literal|1
decl_stmt|;
DECL|member|name_only
name|unsigned
name|name_only
range|:
literal|1
decl_stmt|;
DECL|member|regflags
name|int
name|regflags
decl_stmt|;
DECL|member|pre_context
name|unsigned
name|pre_context
decl_stmt|;
DECL|member|post_context
name|unsigned
name|post_context
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|end_of_line
specifier|static
name|char
modifier|*
name|end_of_line
parameter_list|(
name|char
modifier|*
name|cp
parameter_list|,
name|unsigned
name|long
modifier|*
name|left
parameter_list|)
block|{
name|unsigned
name|long
name|l
init|=
operator|*
name|left
decl_stmt|;
while|while
condition|(
name|l
operator|&&
operator|*
name|cp
operator|!=
literal|'\n'
condition|)
block|{
name|l
operator|--
expr_stmt|;
name|cp
operator|++
expr_stmt|;
block|}
operator|*
name|left
operator|=
name|l
expr_stmt|;
return|return
name|cp
return|;
block|}
end_function

begin_function
DECL|function|show_line
specifier|static
name|void
name|show_line
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|bol
parameter_list|,
specifier|const
name|char
modifier|*
name|eol
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|lno
parameter_list|,
name|char
name|sign
parameter_list|)
block|{
name|printf
argument_list|(
literal|"%s%c"
argument_list|,
name|name
argument_list|,
name|sign
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt
operator|->
name|linenum
condition|)
name|printf
argument_list|(
literal|"%d%c"
argument_list|,
name|lno
argument_list|,
name|sign
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%.*s\n"
argument_list|,
call|(
name|int
call|)
argument_list|(
name|eol
operator|-
name|bol
argument_list|)
argument_list|,
name|bol
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|grep_buffer
specifier|static
name|int
name|grep_buffer
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|char
modifier|*
name|bol
init|=
name|buf
decl_stmt|;
name|unsigned
name|long
name|left
init|=
name|size
decl_stmt|;
name|unsigned
name|lno
init|=
literal|1
decl_stmt|;
struct|struct
name|pre_context_line
block|{
name|char
modifier|*
name|bol
decl_stmt|;
name|char
modifier|*
name|eol
decl_stmt|;
block|}
modifier|*
name|prev
init|=
name|NULL
struct|,
modifier|*
name|pcl
struct|;
name|unsigned
name|last_hit
init|=
literal|0
decl_stmt|;
name|unsigned
name|last_shown
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|hunk_mark
init|=
literal|""
decl_stmt|;
if|if
condition|(
name|opt
operator|->
name|pre_context
condition|)
name|prev
operator|=
name|xcalloc
argument_list|(
name|opt
operator|->
name|pre_context
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|prev
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt
operator|->
name|pre_context
operator|||
name|opt
operator|->
name|post_context
condition|)
name|hunk_mark
operator|=
literal|"--\n"
expr_stmt|;
while|while
condition|(
name|left
condition|)
block|{
name|regmatch_t
name|pmatch
index|[
literal|10
index|]
decl_stmt|;
name|char
modifier|*
name|eol
decl_stmt|,
name|ch
decl_stmt|;
name|int
name|hit
decl_stmt|;
name|eol
operator|=
name|end_of_line
argument_list|(
name|bol
argument_list|,
operator|&
name|left
argument_list|)
expr_stmt|;
name|ch
operator|=
operator|*
name|eol
expr_stmt|;
operator|*
name|eol
operator|=
literal|0
expr_stmt|;
name|hit
operator|=
operator|!
name|regexec
argument_list|(
operator|&
name|opt
operator|->
name|regexp
argument_list|,
name|bol
argument_list|,
name|ARRAY_SIZE
argument_list|(
name|pmatch
argument_list|)
argument_list|,
name|pmatch
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt
operator|->
name|invert
condition|)
name|hit
operator|=
operator|!
name|hit
expr_stmt|;
if|if
condition|(
name|hit
condition|)
block|{
if|if
condition|(
name|opt
operator|->
name|name_only
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
comment|/* Hit at this line.  If we haven't shown the 			 * pre-context lines, we would need to show them. 			 */
if|if
condition|(
name|opt
operator|->
name|pre_context
condition|)
block|{
name|unsigned
name|from
decl_stmt|;
if|if
condition|(
name|opt
operator|->
name|pre_context
operator|<
name|lno
condition|)
name|from
operator|=
name|lno
operator|-
name|opt
operator|->
name|pre_context
expr_stmt|;
else|else
name|from
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|from
operator|<=
name|last_shown
condition|)
name|from
operator|=
name|last_shown
operator|+
literal|1
expr_stmt|;
if|if
condition|(
name|last_shown
operator|&&
name|from
operator|!=
name|last_shown
operator|+
literal|1
condition|)
name|printf
argument_list|(
name|hunk_mark
argument_list|)
expr_stmt|;
while|while
condition|(
name|from
operator|<
name|lno
condition|)
block|{
name|pcl
operator|=
operator|&
name|prev
index|[
name|lno
operator|-
name|from
operator|-
literal|1
index|]
expr_stmt|;
name|show_line
argument_list|(
name|opt
argument_list|,
name|pcl
operator|->
name|bol
argument_list|,
name|pcl
operator|->
name|eol
argument_list|,
name|name
argument_list|,
name|from
argument_list|,
literal|'-'
argument_list|)
expr_stmt|;
name|from
operator|++
expr_stmt|;
block|}
name|last_shown
operator|=
name|lno
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|last_shown
operator|&&
name|lno
operator|!=
name|last_shown
operator|+
literal|1
condition|)
name|printf
argument_list|(
name|hunk_mark
argument_list|)
expr_stmt|;
name|show_line
argument_list|(
name|opt
argument_list|,
name|bol
argument_list|,
name|eol
argument_list|,
name|name
argument_list|,
name|lno
argument_list|,
literal|':'
argument_list|)
expr_stmt|;
name|last_shown
operator|=
name|last_hit
operator|=
name|lno
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|last_hit
operator|&&
name|lno
operator|<=
name|last_hit
operator|+
name|opt
operator|->
name|post_context
condition|)
block|{
comment|/* If the last hit is within the post context, 			 * we need to show this line. 			 */
if|if
condition|(
name|last_shown
operator|&&
name|lno
operator|!=
name|last_shown
operator|+
literal|1
condition|)
name|printf
argument_list|(
name|hunk_mark
argument_list|)
expr_stmt|;
name|show_line
argument_list|(
name|opt
argument_list|,
name|bol
argument_list|,
name|eol
argument_list|,
name|name
argument_list|,
name|lno
argument_list|,
literal|'-'
argument_list|)
expr_stmt|;
name|last_shown
operator|=
name|lno
expr_stmt|;
block|}
if|if
condition|(
name|opt
operator|->
name|pre_context
condition|)
block|{
name|memmove
argument_list|(
name|prev
operator|+
literal|1
argument_list|,
name|prev
argument_list|,
operator|(
name|opt
operator|->
name|pre_context
operator|-
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|prev
argument_list|)
argument_list|)
expr_stmt|;
name|prev
operator|->
name|bol
operator|=
name|bol
expr_stmt|;
name|prev
operator|->
name|eol
operator|=
name|eol
expr_stmt|;
block|}
operator|*
name|eol
operator|=
name|ch
expr_stmt|;
name|bol
operator|=
name|eol
operator|+
literal|1
expr_stmt|;
name|left
operator|--
expr_stmt|;
name|lno
operator|++
expr_stmt|;
block|}
return|return
operator|!
operator|!
name|last_hit
return|;
block|}
end_function

begin_function
DECL|function|grep_sha1
specifier|static
name|int
name|grep_sha1
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|unsigned
name|long
name|size
decl_stmt|;
name|char
modifier|*
name|data
decl_stmt|;
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|int
name|hit
decl_stmt|;
name|data
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
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
block|{
name|error
argument_list|(
literal|"'%s': unable to read %s"
argument_list|,
name|name
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|hit
operator|=
name|grep_buffer
argument_list|(
name|opt
argument_list|,
name|name
argument_list|,
name|data
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
return|return
name|hit
return|;
block|}
end_function

begin_function
DECL|function|grep_file
specifier|static
name|int
name|grep_file
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|int
name|i
decl_stmt|;
name|char
modifier|*
name|data
decl_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|filename
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
block|{
name|err_ret
label|:
if|if
condition|(
name|errno
operator|!=
name|ENOENT
condition|)
name|error
argument_list|(
literal|"'%s': %s"
argument_list|,
name|filename
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|st
operator|.
name|st_size
condition|)
return|return
literal|0
return|;
comment|/* empty file -- no grep hit */
if|if
condition|(
operator|!
name|S_ISREG
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
return|return
literal|0
return|;
name|i
operator|=
name|open
argument_list|(
name|filename
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|i
operator|<
literal|0
condition|)
goto|goto
name|err_ret
goto|;
name|data
operator|=
name|xmalloc
argument_list|(
name|st
operator|.
name|st_size
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|st
operator|.
name|st_size
operator|!=
name|xread
argument_list|(
name|i
argument_list|,
name|data
argument_list|,
name|st
operator|.
name|st_size
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"'%s': short read %s"
argument_list|,
name|filename
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|i
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|close
argument_list|(
name|i
argument_list|)
expr_stmt|;
name|i
operator|=
name|grep_buffer
argument_list|(
name|opt
argument_list|,
name|filename
argument_list|,
name|data
argument_list|,
name|st
operator|.
name|st_size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
return|return
name|i
return|;
block|}
end_function

begin_function
DECL|function|grep_cache
specifier|static
name|int
name|grep_cache
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|paths
parameter_list|,
name|int
name|cached
parameter_list|)
block|{
name|int
name|hit
init|=
literal|0
decl_stmt|;
name|int
name|nr
decl_stmt|;
name|read_cache
argument_list|()
expr_stmt|;
for|for
control|(
name|nr
operator|=
literal|0
init|;
name|nr
operator|<
name|active_nr
condition|;
name|nr
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
name|nr
index|]
decl_stmt|;
if|if
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
operator|||
operator|!
name|S_ISREG
argument_list|(
name|ntohl
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
argument_list|)
condition|)
continue|continue;
if|if
condition|(
operator|!
name|pathspec_matches
argument_list|(
name|paths
argument_list|,
name|ce
operator|->
name|name
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|cached
condition|)
name|hit
operator||=
name|grep_sha1
argument_list|(
name|opt
argument_list|,
name|ce
operator|->
name|sha1
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
else|else
name|hit
operator||=
name|grep_file
argument_list|(
name|opt
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
return|return
name|hit
return|;
block|}
end_function

begin_function
DECL|function|grep_tree
specifier|static
name|int
name|grep_tree
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|paths
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|tree
parameter_list|,
specifier|const
name|char
modifier|*
name|tree_name
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
name|unsigned
name|mode
decl_stmt|;
name|int
name|len
decl_stmt|;
name|int
name|hit
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|char
modifier|*
name|down
decl_stmt|;
name|char
modifier|*
name|path_buf
init|=
name|xmalloc
argument_list|(
name|PATH_MAX
operator|+
name|strlen
argument_list|(
name|tree_name
argument_list|)
operator|+
literal|100
argument_list|)
decl_stmt|;
if|if
condition|(
name|tree_name
index|[
literal|0
index|]
condition|)
block|{
name|int
name|offset
init|=
name|sprintf
argument_list|(
name|path_buf
argument_list|,
literal|"%s:"
argument_list|,
name|tree_name
argument_list|)
decl_stmt|;
name|down
operator|=
name|path_buf
operator|+
name|offset
expr_stmt|;
name|strcat
argument_list|(
name|down
argument_list|,
name|base
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|down
operator|=
name|path_buf
expr_stmt|;
name|strcpy
argument_list|(
name|down
argument_list|,
name|base
argument_list|)
expr_stmt|;
block|}
name|len
operator|=
name|strlen
argument_list|(
name|path_buf
argument_list|)
expr_stmt|;
while|while
condition|(
name|tree
operator|->
name|size
condition|)
block|{
name|int
name|pathlen
decl_stmt|;
name|sha1
operator|=
name|tree_entry_extract
argument_list|(
name|tree
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|mode
argument_list|)
expr_stmt|;
name|pathlen
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|path_buf
operator|+
name|len
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
comment|/* Match "abc/" against pathspec to 			 * decide if we want to descend into "abc" 			 * directory. 			 */
name|strcpy
argument_list|(
name|path_buf
operator|+
name|len
operator|+
name|pathlen
argument_list|,
literal|"/"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pathspec_matches
argument_list|(
name|paths
argument_list|,
name|down
argument_list|)
condition|)
empty_stmt|;
elseif|else
if|if
condition|(
name|S_ISREG
argument_list|(
name|mode
argument_list|)
condition|)
name|hit
operator||=
name|grep_sha1
argument_list|(
name|opt
argument_list|,
name|sha1
argument_list|,
name|path_buf
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
block|{
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|tree_desc
name|sub
decl_stmt|;
name|void
modifier|*
name|data
decl_stmt|;
name|data
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|sub
operator|.
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
literal|"unable to read tree (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|sub
operator|.
name|buf
operator|=
name|data
expr_stmt|;
name|hit
operator||=
name|grep_tree
argument_list|(
name|opt
argument_list|,
name|paths
argument_list|,
operator|&
name|sub
argument_list|,
name|tree_name
argument_list|,
name|down
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
block|}
name|update_tree_entry
argument_list|(
name|tree
argument_list|)
expr_stmt|;
block|}
return|return
name|hit
return|;
block|}
end_function

begin_function
DECL|function|grep_object
specifier|static
name|int
name|grep_object
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|paths
parameter_list|,
name|struct
name|object
modifier|*
name|obj
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|obj
operator|->
name|type
argument_list|,
name|blob_type
argument_list|)
condition|)
return|return
name|grep_sha1
argument_list|(
name|opt
argument_list|,
name|obj
operator|->
name|sha1
argument_list|,
name|name
argument_list|)
return|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|obj
operator|->
name|type
argument_list|,
name|commit_type
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|obj
operator|->
name|type
argument_list|,
name|tree_type
argument_list|)
condition|)
block|{
name|struct
name|tree_desc
name|tree
decl_stmt|;
name|void
modifier|*
name|data
decl_stmt|;
name|int
name|hit
decl_stmt|;
name|data
operator|=
name|read_object_with_reference
argument_list|(
name|obj
operator|->
name|sha1
argument_list|,
name|tree_type
argument_list|,
operator|&
name|tree
operator|.
name|size
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|data
condition|)
name|die
argument_list|(
literal|"unable to read tree (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|tree
operator|.
name|buf
operator|=
name|data
expr_stmt|;
name|hit
operator|=
name|grep_tree
argument_list|(
name|opt
argument_list|,
name|paths
argument_list|,
operator|&
name|tree
argument_list|,
name|name
argument_list|,
literal|""
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
return|return
name|hit
return|;
block|}
name|die
argument_list|(
literal|"unable to grep from object of type %s"
argument_list|,
name|obj
operator|->
name|type
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|builtin_grep_usage
specifier|static
specifier|const
name|char
name|builtin_grep_usage
index|[]
init|=
literal|"git-grep<option>*<rev>* [-e]<pattern> [<path>...]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_grep
name|int
name|cmd_grep
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
name|char
modifier|*
modifier|*
name|envp
parameter_list|)
block|{
name|int
name|err
decl_stmt|;
name|int
name|hit
init|=
literal|0
decl_stmt|;
name|int
name|no_more_flags
init|=
literal|0
decl_stmt|;
name|int
name|seen_noncommit
init|=
literal|0
decl_stmt|;
name|int
name|cached
init|=
literal|0
decl_stmt|;
name|struct
name|grep_opt
name|opt
decl_stmt|;
name|struct
name|object_list
modifier|*
name|list
decl_stmt|,
modifier|*
modifier|*
name|tail
decl_stmt|,
modifier|*
name|object_list
init|=
name|NULL
decl_stmt|;
specifier|const
name|char
modifier|*
name|prefix
init|=
name|setup_git_directory
argument_list|()
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|paths
init|=
name|NULL
decl_stmt|;
name|memset
argument_list|(
operator|&
name|opt
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|opt
argument_list|)
argument_list|)
expr_stmt|;
name|opt
operator|.
name|regflags
operator|=
name|REG_NEWLINE
expr_stmt|;
comment|/* 	 * No point using rev_info, really. 	 */
while|while
condition|(
literal|1
operator|<
name|argc
condition|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
literal|1
index|]
decl_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"--cached"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|cached
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
literal|"-i"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"--ignore-case"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|opt
operator|.
name|regflags
operator||=
name|REG_ICASE
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"-v"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"--invert-match"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|opt
operator|.
name|invert
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
literal|"-E"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"--extended-regexp"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|opt
operator|.
name|regflags
operator||=
name|REG_EXTENDED
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"-G"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"--basic-regexp"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|opt
operator|.
name|regflags
operator|&=
operator|~
name|REG_EXTENDED
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"-n"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|opt
operator|.
name|linenum
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
literal|"-H"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
comment|/* We always show the pathname, so this 			 * is a noop. 			 */
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"-l"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"--files-with-matches"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|opt
operator|.
name|name_only
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
literal|"-A"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"-B"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"-C"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|unsigned
name|num
decl_stmt|;
if|if
condition|(
name|argc
operator|<=
literal|1
operator|||
name|sscanf
argument_list|(
operator|*
operator|++
name|argv
argument_list|,
literal|"%u"
argument_list|,
operator|&
name|num
argument_list|)
operator|!=
literal|1
condition|)
name|usage
argument_list|(
name|builtin_grep_usage
argument_list|)
expr_stmt|;
name|argc
operator|--
expr_stmt|;
switch|switch
condition|(
name|arg
index|[
literal|1
index|]
condition|)
block|{
case|case
literal|'A'
case|:
name|opt
operator|.
name|post_context
operator|=
name|num
expr_stmt|;
break|break;
case|case
literal|'C'
case|:
name|opt
operator|.
name|post_context
operator|=
name|num
expr_stmt|;
case|case
literal|'B'
case|:
name|opt
operator|.
name|pre_context
operator|=
name|num
expr_stmt|;
break|break;
block|}
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"-e"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
if|if
condition|(
literal|1
operator|<
name|argc
condition|)
block|{
comment|/* We probably would want to do 				 * -e pat1 -e pat2 as well later... 				 */
if|if
condition|(
name|opt
operator|.
name|pattern
condition|)
name|die
argument_list|(
literal|"more than one pattern?"
argument_list|)
expr_stmt|;
name|opt
operator|.
name|pattern
operator|=
operator|*
operator|++
name|argv
expr_stmt|;
name|argc
operator|--
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|builtin_grep_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"--"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|no_more_flags
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
comment|/* Either unrecognized option or a single pattern */
if|if
condition|(
operator|!
name|no_more_flags
operator|&&
operator|*
name|arg
operator|==
literal|'-'
condition|)
name|usage
argument_list|(
name|builtin_grep_usage
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|opt
operator|.
name|pattern
condition|)
block|{
name|opt
operator|.
name|pattern
operator|=
name|arg
expr_stmt|;
break|break;
block|}
else|else
block|{
comment|/* We are looking at the first path or rev; 			 * it is found at argv[0] after leaving the 			 * loop. 			 */
name|argc
operator|++
expr_stmt|;
name|argv
operator|--
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
operator|!
name|opt
operator|.
name|pattern
condition|)
name|die
argument_list|(
literal|"no pattern given."
argument_list|)
expr_stmt|;
name|err
operator|=
name|regcomp
argument_list|(
operator|&
name|opt
operator|.
name|regexp
argument_list|,
name|opt
operator|.
name|pattern
argument_list|,
name|opt
operator|.
name|regflags
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
condition|)
block|{
name|char
name|errbuf
index|[
literal|1024
index|]
decl_stmt|;
name|regerror
argument_list|(
name|err
argument_list|,
operator|&
name|opt
operator|.
name|regexp
argument_list|,
name|errbuf
argument_list|,
literal|1024
argument_list|)
expr_stmt|;
name|regfree
argument_list|(
operator|&
name|opt
operator|.
name|regexp
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"'%s': %s"
argument_list|,
name|opt
operator|.
name|pattern
argument_list|,
name|errbuf
argument_list|)
expr_stmt|;
block|}
name|tail
operator|=
operator|&
name|object_list
expr_stmt|;
while|while
condition|(
literal|1
operator|<
name|argc
condition|)
block|{
name|struct
name|object
modifier|*
name|object
decl_stmt|;
name|struct
name|object_list
modifier|*
name|elem
decl_stmt|;
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
literal|1
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
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
break|break;
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
name|object
condition|)
name|die
argument_list|(
literal|"bad object %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|elem
operator|=
name|object_list_insert
argument_list|(
name|object
argument_list|,
name|tail
argument_list|)
expr_stmt|;
name|elem
operator|->
name|name
operator|=
name|arg
expr_stmt|;
name|tail
operator|=
operator|&
name|elem
operator|->
name|next
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
if|if
condition|(
literal|1
operator|<
name|argc
condition|)
name|paths
operator|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
operator|+
literal|1
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|prefix
condition|)
block|{
name|paths
operator|=
name|xcalloc
argument_list|(
literal|2
argument_list|,
sizeof|sizeof
argument_list|(
specifier|const
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|paths
index|[
literal|0
index|]
operator|=
name|prefix
expr_stmt|;
name|paths
index|[
literal|1
index|]
operator|=
name|NULL
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|object_list
condition|)
return|return
operator|!
name|grep_cache
argument_list|(
operator|&
name|opt
argument_list|,
name|paths
argument_list|,
name|cached
argument_list|)
return|;
comment|/* 	 * Do not walk "grep -e foo master next pu -- Documentation/" 	 * but do walk "grep -e foo master..next -- Documentation/". 	 * Ranged request mixed with a blob or tree object, like 	 * "grep -e foo v1.0.0:Documentation/ master..next" 	 * so detect that and complain. 	 */
for|for
control|(
name|list
operator|=
name|object_list
init|;
name|list
condition|;
name|list
operator|=
name|list
operator|->
name|next
control|)
block|{
name|struct
name|object
modifier|*
name|real_obj
decl_stmt|;
name|real_obj
operator|=
name|deref_tag
argument_list|(
name|list
operator|->
name|item
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|real_obj
operator|->
name|type
argument_list|,
name|commit_type
argument_list|)
condition|)
name|seen_noncommit
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|cached
condition|)
name|die
argument_list|(
literal|"both --cached and revisions given."
argument_list|)
expr_stmt|;
for|for
control|(
name|list
operator|=
name|object_list
init|;
name|list
condition|;
name|list
operator|=
name|list
operator|->
name|next
control|)
block|{
name|struct
name|object
modifier|*
name|real_obj
decl_stmt|;
name|real_obj
operator|=
name|deref_tag
argument_list|(
name|list
operator|->
name|item
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|grep_object
argument_list|(
operator|&
name|opt
argument_list|,
name|paths
argument_list|,
name|real_obj
argument_list|,
name|list
operator|->
name|name
argument_list|)
condition|)
name|hit
operator|=
literal|1
expr_stmt|;
block|}
return|return
operator|!
name|hit
return|;
block|}
end_function

end_unit

