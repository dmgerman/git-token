begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"attr.h"
end_include

begin_comment
comment|/*  * convert.c - convert a file when checking it out and checking it in.  *  * This should use the pathname to decide on whether it wants to do some  * more interesting conversions (automatic gzip/unzip, general format  * conversions etc etc), but by default it just does automatic CRLF<->LF  * translation when the "auto_crlf" option is set.  */
end_comment

begin_define
DECL|macro|CRLF_GUESS
define|#
directive|define
name|CRLF_GUESS
value|(-1)
end_define

begin_define
DECL|macro|CRLF_BINARY
define|#
directive|define
name|CRLF_BINARY
value|0
end_define

begin_define
DECL|macro|CRLF_TEXT
define|#
directive|define
name|CRLF_TEXT
value|1
end_define

begin_define
DECL|macro|CRLF_INPUT
define|#
directive|define
name|CRLF_INPUT
value|2
end_define

begin_struct
DECL|struct|text_stat
struct|struct
name|text_stat
block|{
comment|/* CR, LF and CRLF counts */
DECL|member|cr
DECL|member|lf
DECL|member|crlf
name|unsigned
name|cr
decl_stmt|,
name|lf
decl_stmt|,
name|crlf
decl_stmt|;
comment|/* These are just approximations! */
DECL|member|printable
DECL|member|nonprintable
name|unsigned
name|printable
decl_stmt|,
name|nonprintable
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|gather_stats
specifier|static
name|void
name|gather_stats
parameter_list|(
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|struct
name|text_stat
modifier|*
name|stats
parameter_list|)
block|{
name|unsigned
name|long
name|i
decl_stmt|;
name|memset
argument_list|(
name|stats
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|stats
argument_list|)
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|size
condition|;
name|i
operator|++
control|)
block|{
name|unsigned
name|char
name|c
init|=
name|buf
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\r'
condition|)
block|{
name|stats
operator|->
name|cr
operator|++
expr_stmt|;
if|if
condition|(
name|i
operator|+
literal|1
operator|<
name|size
operator|&&
name|buf
index|[
name|i
operator|+
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|stats
operator|->
name|crlf
operator|++
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|c
operator|==
literal|'\n'
condition|)
block|{
name|stats
operator|->
name|lf
operator|++
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|c
operator|==
literal|127
condition|)
comment|/* DEL */
name|stats
operator|->
name|nonprintable
operator|++
expr_stmt|;
elseif|else
if|if
condition|(
name|c
operator|<
literal|32
condition|)
block|{
switch|switch
condition|(
name|c
condition|)
block|{
comment|/* BS, HT, ESC and FF */
case|case
literal|'\b'
case|:
case|case
literal|'\t'
case|:
case|case
literal|'\033'
case|:
case|case
literal|'\014'
case|:
name|stats
operator|->
name|printable
operator|++
expr_stmt|;
break|break;
default|default:
name|stats
operator|->
name|nonprintable
operator|++
expr_stmt|;
block|}
block|}
else|else
name|stats
operator|->
name|printable
operator|++
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/*  * The same heuristics as diff.c::mmfile_is_binary()  */
end_comment

begin_function
DECL|function|is_binary
specifier|static
name|int
name|is_binary
parameter_list|(
name|unsigned
name|long
name|size
parameter_list|,
name|struct
name|text_stat
modifier|*
name|stats
parameter_list|)
block|{
if|if
condition|(
operator|(
name|stats
operator|->
name|printable
operator|>>
literal|7
operator|)
operator|<
name|stats
operator|->
name|nonprintable
condition|)
return|return
literal|1
return|;
comment|/* 	 * Other heuristics? Average line length might be relevant, 	 * as might LF vs CR vs CRLF counts.. 	 * 	 * NOTE! It might be normal to have a low ratio of CRLF to LF 	 * (somebody starts with a LF-only file and edits it with an editor 	 * that adds CRLF only to lines that are added..). But do  we 	 * want to support CR-only? Probably not. 	 */
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|crlf_to_git
specifier|static
name|char
modifier|*
name|crlf_to_git
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|,
name|unsigned
name|long
modifier|*
name|sizep
parameter_list|,
name|int
name|action
parameter_list|)
block|{
name|char
modifier|*
name|buffer
decl_stmt|,
modifier|*
name|dst
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|,
name|nsize
decl_stmt|;
name|struct
name|text_stat
name|stats
decl_stmt|;
if|if
condition|(
operator|(
name|action
operator|==
name|CRLF_BINARY
operator|)
operator|||
operator|(
name|action
operator|==
name|CRLF_GUESS
operator|&&
operator|!
name|auto_crlf
operator|)
condition|)
return|return
name|NULL
return|;
name|size
operator|=
operator|*
name|sizep
expr_stmt|;
if|if
condition|(
operator|!
name|size
condition|)
return|return
name|NULL
return|;
name|gather_stats
argument_list|(
name|src
argument_list|,
name|size
argument_list|,
operator|&
name|stats
argument_list|)
expr_stmt|;
comment|/* No CR? Nothing to convert, regardless. */
if|if
condition|(
operator|!
name|stats
operator|.
name|cr
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|action
operator|==
name|CRLF_GUESS
condition|)
block|{
comment|/* 		 * We're currently not going to even try to convert stuff 		 * that has bare CR characters. Does anybody do that crazy 		 * stuff? 		 */
if|if
condition|(
name|stats
operator|.
name|cr
operator|!=
name|stats
operator|.
name|crlf
condition|)
return|return
name|NULL
return|;
comment|/* 		 * And add some heuristics for binary vs text, of course... 		 */
if|if
condition|(
name|is_binary
argument_list|(
name|size
argument_list|,
operator|&
name|stats
argument_list|)
condition|)
return|return
name|NULL
return|;
block|}
comment|/* 	 * Ok, allocate a new buffer, fill it in, and return it 	 * to let the caller know that we switched buffers. 	 */
name|nsize
operator|=
name|size
operator|-
name|stats
operator|.
name|crlf
expr_stmt|;
name|buffer
operator|=
name|xmalloc
argument_list|(
name|nsize
argument_list|)
expr_stmt|;
operator|*
name|sizep
operator|=
name|nsize
expr_stmt|;
name|dst
operator|=
name|buffer
expr_stmt|;
if|if
condition|(
name|action
operator|==
name|CRLF_GUESS
condition|)
block|{
comment|/* 		 * If we guessed, we already know we rejected a file with 		 * lone CR, and we can strip a CR without looking at what 		 * follow it. 		 */
do|do
block|{
name|unsigned
name|char
name|c
init|=
operator|*
name|src
operator|++
decl_stmt|;
if|if
condition|(
name|c
operator|!=
literal|'\r'
condition|)
operator|*
name|dst
operator|++
operator|=
name|c
expr_stmt|;
block|}
do|while
condition|(
operator|--
name|size
condition|)
do|;
block|}
else|else
block|{
do|do
block|{
name|unsigned
name|char
name|c
init|=
operator|*
name|src
operator|++
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|c
operator|==
literal|'\r'
operator|&&
operator|(
literal|1
operator|<
name|size
operator|&&
operator|*
name|src
operator|==
literal|'\n'
operator|)
operator|)
condition|)
operator|*
name|dst
operator|++
operator|=
name|c
expr_stmt|;
block|}
do|while
condition|(
operator|--
name|size
condition|)
do|;
block|}
return|return
name|buffer
return|;
block|}
end_function

begin_function
DECL|function|crlf_to_worktree
specifier|static
name|char
modifier|*
name|crlf_to_worktree
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|,
name|unsigned
name|long
modifier|*
name|sizep
parameter_list|,
name|int
name|action
parameter_list|)
block|{
name|char
modifier|*
name|buffer
decl_stmt|,
modifier|*
name|dst
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|,
name|nsize
decl_stmt|;
name|struct
name|text_stat
name|stats
decl_stmt|;
name|unsigned
name|char
name|last
decl_stmt|;
if|if
condition|(
operator|(
name|action
operator|==
name|CRLF_BINARY
operator|)
operator|||
operator|(
name|action
operator|==
name|CRLF_INPUT
operator|)
operator|||
operator|(
name|action
operator|==
name|CRLF_GUESS
operator|&&
name|auto_crlf
operator|<=
literal|0
operator|)
condition|)
return|return
name|NULL
return|;
name|size
operator|=
operator|*
name|sizep
expr_stmt|;
if|if
condition|(
operator|!
name|size
condition|)
return|return
name|NULL
return|;
name|gather_stats
argument_list|(
name|src
argument_list|,
name|size
argument_list|,
operator|&
name|stats
argument_list|)
expr_stmt|;
comment|/* No LF? Nothing to convert, regardless. */
if|if
condition|(
operator|!
name|stats
operator|.
name|lf
condition|)
return|return
name|NULL
return|;
comment|/* Was it already in CRLF format? */
if|if
condition|(
name|stats
operator|.
name|lf
operator|==
name|stats
operator|.
name|crlf
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|action
operator|==
name|CRLF_GUESS
condition|)
block|{
comment|/* If we have any bare CR characters, we're not going to touch it */
if|if
condition|(
name|stats
operator|.
name|cr
operator|!=
name|stats
operator|.
name|crlf
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|is_binary
argument_list|(
name|size
argument_list|,
operator|&
name|stats
argument_list|)
condition|)
return|return
name|NULL
return|;
block|}
comment|/* 	 * Ok, allocate a new buffer, fill it in, and return it 	 * to let the caller know that we switched buffers. 	 */
name|nsize
operator|=
name|size
operator|+
name|stats
operator|.
name|lf
operator|-
name|stats
operator|.
name|crlf
expr_stmt|;
name|buffer
operator|=
name|xmalloc
argument_list|(
name|nsize
argument_list|)
expr_stmt|;
operator|*
name|sizep
operator|=
name|nsize
expr_stmt|;
name|last
operator|=
literal|0
expr_stmt|;
name|dst
operator|=
name|buffer
expr_stmt|;
do|do
block|{
name|unsigned
name|char
name|c
init|=
operator|*
name|src
operator|++
decl_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\n'
operator|&&
name|last
operator|!=
literal|'\r'
condition|)
operator|*
name|dst
operator|++
operator|=
literal|'\r'
expr_stmt|;
operator|*
name|dst
operator|++
operator|=
name|c
expr_stmt|;
name|last
operator|=
name|c
expr_stmt|;
block|}
do|while
condition|(
operator|--
name|size
condition|)
do|;
return|return
name|buffer
return|;
block|}
end_function

begin_function
DECL|function|setup_convert_check
specifier|static
name|void
name|setup_convert_check
parameter_list|(
name|struct
name|git_attr_check
modifier|*
name|check
parameter_list|)
block|{
specifier|static
name|struct
name|git_attr
modifier|*
name|attr_crlf
decl_stmt|;
if|if
condition|(
operator|!
name|attr_crlf
condition|)
name|attr_crlf
operator|=
name|git_attr
argument_list|(
literal|"crlf"
argument_list|,
literal|4
argument_list|)
expr_stmt|;
name|check
operator|->
name|attr
operator|=
name|attr_crlf
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_path_check_crlf
specifier|static
name|int
name|git_path_check_crlf
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|git_attr_check
modifier|*
name|check
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|value
init|=
name|check
operator|->
name|value
decl_stmt|;
if|if
condition|(
name|ATTR_TRUE
argument_list|(
name|value
argument_list|)
condition|)
return|return
name|CRLF_TEXT
return|;
elseif|else
if|if
condition|(
name|ATTR_FALSE
argument_list|(
name|value
argument_list|)
condition|)
return|return
name|CRLF_BINARY
return|;
elseif|else
if|if
condition|(
name|ATTR_UNSET
argument_list|(
name|value
argument_list|)
condition|)
empty_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|value
argument_list|,
literal|"input"
argument_list|)
condition|)
return|return
name|CRLF_INPUT
return|;
return|return
name|CRLF_GUESS
return|;
block|}
end_function

begin_function
DECL|function|convert_to_git
name|char
modifier|*
name|convert_to_git
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|,
name|unsigned
name|long
modifier|*
name|sizep
parameter_list|)
block|{
name|struct
name|git_attr_check
name|check
index|[
literal|1
index|]
decl_stmt|;
name|int
name|crlf
init|=
name|CRLF_GUESS
decl_stmt|;
name|setup_convert_check
argument_list|(
name|check
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_checkattr
argument_list|(
name|path
argument_list|,
literal|1
argument_list|,
name|check
argument_list|)
condition|)
block|{
name|crlf
operator|=
name|git_path_check_crlf
argument_list|(
name|path
argument_list|,
name|check
argument_list|)
expr_stmt|;
block|}
return|return
name|crlf_to_git
argument_list|(
name|path
argument_list|,
name|src
argument_list|,
name|sizep
argument_list|,
name|crlf
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|convert_to_working_tree
name|char
modifier|*
name|convert_to_working_tree
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|,
name|unsigned
name|long
modifier|*
name|sizep
parameter_list|)
block|{
name|struct
name|git_attr_check
name|check
index|[
literal|1
index|]
decl_stmt|;
name|int
name|crlf
init|=
name|CRLF_GUESS
decl_stmt|;
name|setup_convert_check
argument_list|(
name|check
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_checkattr
argument_list|(
name|path
argument_list|,
literal|1
argument_list|,
name|check
argument_list|)
condition|)
block|{
name|crlf
operator|=
name|git_path_check_crlf
argument_list|(
name|path
argument_list|,
name|check
argument_list|)
expr_stmt|;
block|}
return|return
name|crlf_to_worktree
argument_list|(
name|path
argument_list|,
name|src
argument_list|,
name|sizep
argument_list|,
name|crlf
argument_list|)
return|;
block|}
end_function

end_unit

