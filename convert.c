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

begin_include
include|#
directive|include
file|"run-command.h"
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
name|int
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
name|size_t
name|len
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|int
name|action
parameter_list|)
block|{
name|struct
name|text_stat
name|stats
decl_stmt|;
name|char
modifier|*
name|dst
decl_stmt|;
if|if
condition|(
operator|(
name|action
operator|==
name|CRLF_BINARY
operator|)
operator|||
operator|!
name|auto_crlf
operator|||
operator|!
name|len
condition|)
return|return
literal|0
return|;
name|gather_stats
argument_list|(
name|src
argument_list|,
name|len
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
literal|0
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
literal|0
return|;
comment|/* 		 * And add some heuristics for binary vs text, of course... 		 */
if|if
condition|(
name|is_binary
argument_list|(
name|len
argument_list|,
operator|&
name|stats
argument_list|)
condition|)
return|return
literal|0
return|;
block|}
comment|/* only grow if not in place */
if|if
condition|(
name|strbuf_avail
argument_list|(
name|buf
argument_list|)
operator|+
name|buf
operator|->
name|len
operator|<
name|len
condition|)
name|strbuf_grow
argument_list|(
name|buf
argument_list|,
name|len
operator|-
name|buf
operator|->
name|len
argument_list|)
expr_stmt|;
name|dst
operator|=
name|buf
operator|->
name|buf
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
name|len
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
name|len
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
name|len
condition|)
do|;
block|}
name|strbuf_setlen
argument_list|(
name|buf
argument_list|,
name|dst
operator|-
name|buf
operator|->
name|buf
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|crlf_to_worktree
specifier|static
name|int
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
name|size_t
name|len
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|int
name|action
parameter_list|)
block|{
name|char
modifier|*
name|to_free
init|=
name|NULL
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
name|CRLF_INPUT
operator|)
operator|||
name|auto_crlf
operator|<=
literal|0
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|len
condition|)
return|return
literal|0
return|;
name|gather_stats
argument_list|(
name|src
argument_list|,
name|len
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
literal|0
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
literal|0
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
literal|0
return|;
if|if
condition|(
name|is_binary
argument_list|(
name|len
argument_list|,
operator|&
name|stats
argument_list|)
condition|)
return|return
literal|0
return|;
block|}
comment|/* are we "faking" in place editing ? */
if|if
condition|(
name|src
operator|==
name|buf
operator|->
name|buf
condition|)
name|to_free
operator|=
name|strbuf_detach
argument_list|(
name|buf
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|strbuf_grow
argument_list|(
name|buf
argument_list|,
name|len
operator|+
name|stats
operator|.
name|lf
operator|-
name|stats
operator|.
name|crlf
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
specifier|const
name|char
modifier|*
name|nl
init|=
name|memchr
argument_list|(
name|src
argument_list|,
literal|'\n'
argument_list|,
name|len
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|nl
condition|)
break|break;
if|if
condition|(
name|nl
operator|>
name|src
operator|&&
name|nl
index|[
operator|-
literal|1
index|]
operator|==
literal|'\r'
condition|)
block|{
name|strbuf_add
argument_list|(
name|buf
argument_list|,
name|src
argument_list|,
name|nl
operator|+
literal|1
operator|-
name|src
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|strbuf_add
argument_list|(
name|buf
argument_list|,
name|src
argument_list|,
name|nl
operator|-
name|src
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
name|buf
argument_list|,
literal|"\r\n"
argument_list|)
expr_stmt|;
block|}
name|len
operator|-=
name|nl
operator|+
literal|1
operator|-
name|src
expr_stmt|;
name|src
operator|=
name|nl
operator|+
literal|1
expr_stmt|;
block|}
name|strbuf_add
argument_list|(
name|buf
argument_list|,
name|src
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|to_free
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_struct
DECL|struct|filter_params
struct|struct
name|filter_params
block|{
DECL|member|src
specifier|const
name|char
modifier|*
name|src
decl_stmt|;
DECL|member|size
name|unsigned
name|long
name|size
decl_stmt|;
DECL|member|cmd
specifier|const
name|char
modifier|*
name|cmd
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|filter_buffer
specifier|static
name|int
name|filter_buffer
parameter_list|(
name|int
name|fd
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
comment|/* 	 * Spawn cmd and feed the buffer contents through its stdin. 	 */
name|struct
name|child_process
name|child_process
decl_stmt|;
name|struct
name|filter_params
modifier|*
name|params
init|=
operator|(
expr|struct
name|filter_params
operator|*
operator|)
name|data
decl_stmt|;
name|int
name|write_err
decl_stmt|,
name|status
decl_stmt|;
specifier|const
name|char
modifier|*
name|argv
index|[]
init|=
block|{
literal|"sh"
block|,
literal|"-c"
block|,
name|params
operator|->
name|cmd
block|,
name|NULL
block|}
decl_stmt|;
name|memset
argument_list|(
operator|&
name|child_process
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|child_process
argument_list|)
argument_list|)
expr_stmt|;
name|child_process
operator|.
name|argv
operator|=
name|argv
expr_stmt|;
name|child_process
operator|.
name|in
operator|=
operator|-
literal|1
expr_stmt|;
name|child_process
operator|.
name|out
operator|=
name|fd
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|child_process
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"cannot fork to run external filter %s"
argument_list|,
name|params
operator|->
name|cmd
argument_list|)
return|;
name|write_err
operator|=
operator|(
name|write_in_full
argument_list|(
name|child_process
operator|.
name|in
argument_list|,
name|params
operator|->
name|src
argument_list|,
name|params
operator|->
name|size
argument_list|)
operator|<
literal|0
operator|)
expr_stmt|;
if|if
condition|(
name|close
argument_list|(
name|child_process
operator|.
name|in
argument_list|)
condition|)
name|write_err
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|write_err
condition|)
name|error
argument_list|(
literal|"cannot feed the input to external filter %s"
argument_list|,
name|params
operator|->
name|cmd
argument_list|)
expr_stmt|;
name|status
operator|=
name|finish_command
argument_list|(
operator|&
name|child_process
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
condition|)
name|error
argument_list|(
literal|"external filter %s failed %d"
argument_list|,
name|params
operator|->
name|cmd
argument_list|,
operator|-
name|status
argument_list|)
expr_stmt|;
return|return
operator|(
name|write_err
operator|||
name|status
operator|)
return|;
block|}
end_function

begin_function
DECL|function|apply_filter
specifier|static
name|int
name|apply_filter
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
name|size_t
name|len
parameter_list|,
name|struct
name|strbuf
modifier|*
name|dst
parameter_list|,
specifier|const
name|char
modifier|*
name|cmd
parameter_list|)
block|{
comment|/* 	 * Create a pipeline to have the command filter the buffer's 	 * contents. 	 * 	 * (child --> cmd) --> us 	 */
name|int
name|ret
init|=
literal|1
decl_stmt|;
name|struct
name|strbuf
name|nbuf
decl_stmt|;
name|struct
name|async
name|async
decl_stmt|;
name|struct
name|filter_params
name|params
decl_stmt|;
if|if
condition|(
operator|!
name|cmd
condition|)
return|return
literal|0
return|;
name|memset
argument_list|(
operator|&
name|async
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|async
argument_list|)
argument_list|)
expr_stmt|;
name|async
operator|.
name|proc
operator|=
name|filter_buffer
expr_stmt|;
name|async
operator|.
name|data
operator|=
operator|&
name|params
expr_stmt|;
name|params
operator|.
name|src
operator|=
name|src
expr_stmt|;
name|params
operator|.
name|size
operator|=
name|len
expr_stmt|;
name|params
operator|.
name|cmd
operator|=
name|cmd
expr_stmt|;
name|fflush
argument_list|(
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|start_async
argument_list|(
operator|&
name|async
argument_list|)
condition|)
return|return
literal|0
return|;
comment|/* error was already reported */
name|strbuf_init
argument_list|(
operator|&
name|nbuf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|strbuf_read
argument_list|(
operator|&
name|nbuf
argument_list|,
name|async
operator|.
name|out
argument_list|,
name|len
argument_list|)
operator|<
literal|0
condition|)
block|{
name|error
argument_list|(
literal|"read from external filter %s failed"
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|close
argument_list|(
name|async
operator|.
name|out
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"read from external filter %s failed"
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|finish_async
argument_list|(
operator|&
name|async
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"external filter %s failed"
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|ret
condition|)
block|{
name|strbuf_swap
argument_list|(
name|dst
argument_list|,
operator|&
name|nbuf
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|nbuf
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_struct
DECL|struct|convert_driver
specifier|static
struct|struct
name|convert_driver
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|next
name|struct
name|convert_driver
modifier|*
name|next
decl_stmt|;
DECL|member|smudge
name|char
modifier|*
name|smudge
decl_stmt|;
DECL|member|clean
name|char
modifier|*
name|clean
decl_stmt|;
DECL|variable|user_convert
DECL|variable|user_convert_tail
block|}
modifier|*
name|user_convert
struct|,
modifier|*
modifier|*
name|user_convert_tail
struct|;
end_struct

begin_function
DECL|function|read_convert_config
specifier|static
name|int
name|read_convert_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|ep
decl_stmt|,
modifier|*
name|name
decl_stmt|;
name|int
name|namelen
decl_stmt|;
name|struct
name|convert_driver
modifier|*
name|drv
decl_stmt|;
comment|/* 	 * External conversion drivers are configured using 	 * "filter.<name>.variable". 	 */
if|if
condition|(
name|prefixcmp
argument_list|(
name|var
argument_list|,
literal|"filter."
argument_list|)
operator|||
operator|(
name|ep
operator|=
name|strrchr
argument_list|(
name|var
argument_list|,
literal|'.'
argument_list|)
operator|)
operator|==
name|var
operator|+
literal|6
condition|)
return|return
literal|0
return|;
name|name
operator|=
name|var
operator|+
literal|7
expr_stmt|;
name|namelen
operator|=
name|ep
operator|-
name|name
expr_stmt|;
for|for
control|(
name|drv
operator|=
name|user_convert
init|;
name|drv
condition|;
name|drv
operator|=
name|drv
operator|->
name|next
control|)
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|drv
operator|->
name|name
argument_list|,
name|name
argument_list|,
name|namelen
argument_list|)
operator|&&
operator|!
name|drv
operator|->
name|name
index|[
name|namelen
index|]
condition|)
break|break;
if|if
condition|(
operator|!
name|drv
condition|)
block|{
name|drv
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|convert_driver
argument_list|)
argument_list|)
expr_stmt|;
name|drv
operator|->
name|name
operator|=
name|xmemdupz
argument_list|(
name|name
argument_list|,
name|namelen
argument_list|)
expr_stmt|;
operator|*
name|user_convert_tail
operator|=
name|drv
expr_stmt|;
name|user_convert_tail
operator|=
operator|&
operator|(
name|drv
operator|->
name|next
operator|)
expr_stmt|;
block|}
name|ep
operator|++
expr_stmt|;
comment|/* 	 * filter.<name>.smudge and filter.<name>.clean specifies 	 * the command line: 	 * 	 *	command-line 	 * 	 * The command-line will not be interpolated in any way. 	 */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"smudge"
argument_list|,
name|ep
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|value
condition|)
return|return
name|error
argument_list|(
literal|"%s: lacks value"
argument_list|,
name|var
argument_list|)
return|;
name|drv
operator|->
name|smudge
operator|=
name|strdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"clean"
argument_list|,
name|ep
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|value
condition|)
return|return
name|error
argument_list|(
literal|"%s: lacks value"
argument_list|,
name|var
argument_list|)
return|;
name|drv
operator|->
name|clean
operator|=
name|strdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
return|return
literal|0
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
specifier|static
name|struct
name|git_attr
modifier|*
name|attr_ident
decl_stmt|;
specifier|static
name|struct
name|git_attr
modifier|*
name|attr_filter
decl_stmt|;
if|if
condition|(
operator|!
name|attr_crlf
condition|)
block|{
name|attr_crlf
operator|=
name|git_attr
argument_list|(
literal|"crlf"
argument_list|,
literal|4
argument_list|)
expr_stmt|;
name|attr_ident
operator|=
name|git_attr
argument_list|(
literal|"ident"
argument_list|,
literal|5
argument_list|)
expr_stmt|;
name|attr_filter
operator|=
name|git_attr
argument_list|(
literal|"filter"
argument_list|,
literal|6
argument_list|)
expr_stmt|;
name|user_convert_tail
operator|=
operator|&
name|user_convert
expr_stmt|;
name|git_config
argument_list|(
name|read_convert_config
argument_list|)
expr_stmt|;
block|}
name|check
index|[
literal|0
index|]
operator|.
name|attr
operator|=
name|attr_crlf
expr_stmt|;
name|check
index|[
literal|1
index|]
operator|.
name|attr
operator|=
name|attr_ident
expr_stmt|;
name|check
index|[
literal|2
index|]
operator|.
name|attr
operator|=
name|attr_filter
expr_stmt|;
block|}
end_function

begin_function
DECL|function|count_ident
specifier|static
name|int
name|count_ident
parameter_list|(
specifier|const
name|char
modifier|*
name|cp
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
comment|/* 	 * "$Id: 0000000000000000000000000000000000000000 $"<=> "$Id$" 	 */
name|int
name|cnt
init|=
literal|0
decl_stmt|;
name|char
name|ch
decl_stmt|;
while|while
condition|(
name|size
condition|)
block|{
name|ch
operator|=
operator|*
name|cp
operator|++
expr_stmt|;
name|size
operator|--
expr_stmt|;
if|if
condition|(
name|ch
operator|!=
literal|'$'
condition|)
continue|continue;
if|if
condition|(
name|size
operator|<
literal|3
condition|)
break|break;
if|if
condition|(
name|memcmp
argument_list|(
literal|"Id"
argument_list|,
name|cp
argument_list|,
literal|2
argument_list|)
condition|)
continue|continue;
name|ch
operator|=
name|cp
index|[
literal|2
index|]
expr_stmt|;
name|cp
operator|+=
literal|3
expr_stmt|;
name|size
operator|-=
literal|3
expr_stmt|;
if|if
condition|(
name|ch
operator|==
literal|'$'
condition|)
name|cnt
operator|++
expr_stmt|;
comment|/* $Id$ */
if|if
condition|(
name|ch
operator|!=
literal|':'
condition|)
continue|continue;
comment|/* 		 * "$Id: ... "; scan up to the closing dollar sign and discard. 		 */
while|while
condition|(
name|size
condition|)
block|{
name|ch
operator|=
operator|*
name|cp
operator|++
expr_stmt|;
name|size
operator|--
expr_stmt|;
if|if
condition|(
name|ch
operator|==
literal|'$'
condition|)
block|{
name|cnt
operator|++
expr_stmt|;
break|break;
block|}
block|}
block|}
return|return
name|cnt
return|;
block|}
end_function

begin_function
DECL|function|ident_to_git
specifier|static
name|int
name|ident_to_git
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
name|size_t
name|len
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|int
name|ident
parameter_list|)
block|{
name|char
modifier|*
name|dst
decl_stmt|,
modifier|*
name|dollar
decl_stmt|;
if|if
condition|(
operator|!
name|ident
operator|||
operator|!
name|count_ident
argument_list|(
name|src
argument_list|,
name|len
argument_list|)
condition|)
return|return
literal|0
return|;
comment|/* only grow if not in place */
if|if
condition|(
name|strbuf_avail
argument_list|(
name|buf
argument_list|)
operator|+
name|buf
operator|->
name|len
operator|<
name|len
condition|)
name|strbuf_grow
argument_list|(
name|buf
argument_list|,
name|len
operator|-
name|buf
operator|->
name|len
argument_list|)
expr_stmt|;
name|dst
operator|=
name|buf
operator|->
name|buf
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|dollar
operator|=
name|memchr
argument_list|(
name|src
argument_list|,
literal|'$'
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dollar
condition|)
break|break;
name|memcpy
argument_list|(
name|dst
argument_list|,
name|src
argument_list|,
name|dollar
operator|+
literal|1
operator|-
name|src
argument_list|)
expr_stmt|;
name|dst
operator|+=
name|dollar
operator|+
literal|1
operator|-
name|src
expr_stmt|;
name|len
operator|-=
name|dollar
operator|+
literal|1
operator|-
name|src
expr_stmt|;
name|src
operator|=
name|dollar
operator|+
literal|1
expr_stmt|;
if|if
condition|(
name|len
operator|>
literal|3
operator|&&
operator|!
name|memcmp
argument_list|(
name|src
argument_list|,
literal|"Id:"
argument_list|,
literal|3
argument_list|)
condition|)
block|{
name|dollar
operator|=
name|memchr
argument_list|(
name|src
operator|+
literal|3
argument_list|,
literal|'$'
argument_list|,
name|len
operator|-
literal|3
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dollar
condition|)
break|break;
name|memcpy
argument_list|(
name|dst
argument_list|,
literal|"Id$"
argument_list|,
literal|3
argument_list|)
expr_stmt|;
name|dst
operator|+=
literal|3
expr_stmt|;
name|len
operator|-=
name|dollar
operator|+
literal|1
operator|-
name|src
expr_stmt|;
name|src
operator|=
name|dollar
operator|+
literal|1
expr_stmt|;
block|}
block|}
name|memcpy
argument_list|(
name|dst
argument_list|,
name|src
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|strbuf_setlen
argument_list|(
name|buf
argument_list|,
name|dst
operator|+
name|len
operator|-
name|buf
operator|->
name|buf
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|ident_to_worktree
specifier|static
name|int
name|ident_to_worktree
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
name|size_t
name|len
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|int
name|ident
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|to_free
init|=
name|NULL
decl_stmt|,
modifier|*
name|dollar
decl_stmt|;
name|int
name|cnt
decl_stmt|;
if|if
condition|(
operator|!
name|ident
condition|)
return|return
literal|0
return|;
name|cnt
operator|=
name|count_ident
argument_list|(
name|src
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cnt
condition|)
return|return
literal|0
return|;
comment|/* are we "faking" in place editing ? */
if|if
condition|(
name|src
operator|==
name|buf
operator|->
name|buf
condition|)
name|to_free
operator|=
name|strbuf_detach
argument_list|(
name|buf
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|hash_sha1_file
argument_list|(
name|src
argument_list|,
name|len
argument_list|,
literal|"blob"
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|strbuf_grow
argument_list|(
name|buf
argument_list|,
name|len
operator|+
name|cnt
operator|*
literal|43
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
comment|/* step 1: run to the next '$' */
name|dollar
operator|=
name|memchr
argument_list|(
name|src
argument_list|,
literal|'$'
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dollar
condition|)
break|break;
name|strbuf_add
argument_list|(
name|buf
argument_list|,
name|src
argument_list|,
name|dollar
operator|+
literal|1
operator|-
name|src
argument_list|)
expr_stmt|;
name|len
operator|-=
name|dollar
operator|+
literal|1
operator|-
name|src
expr_stmt|;
name|src
operator|=
name|dollar
operator|+
literal|1
expr_stmt|;
comment|/* step 2: does it looks like a bit like Id:xxx$ or Id$ ? */
if|if
condition|(
name|len
operator|<
literal|3
operator|||
name|memcmp
argument_list|(
literal|"Id"
argument_list|,
name|src
argument_list|,
literal|2
argument_list|)
condition|)
continue|continue;
comment|/* step 3: skip over Id$ or Id:xxxxx$ */
if|if
condition|(
name|src
index|[
literal|2
index|]
operator|==
literal|'$'
condition|)
block|{
name|src
operator|+=
literal|3
expr_stmt|;
name|len
operator|-=
literal|3
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|src
index|[
literal|2
index|]
operator|==
literal|':'
condition|)
block|{
comment|/* 			 * It's possible that an expanded Id has crept its way into the 			 * repository, we cope with that by stripping the expansion out 			 */
name|dollar
operator|=
name|memchr
argument_list|(
name|src
operator|+
literal|3
argument_list|,
literal|'$'
argument_list|,
name|len
operator|-
literal|3
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dollar
condition|)
block|{
comment|/* incomplete keyword, no more '$', so just quit the loop */
break|break;
block|}
name|len
operator|-=
name|dollar
operator|+
literal|1
operator|-
name|src
expr_stmt|;
name|src
operator|=
name|dollar
operator|+
literal|1
expr_stmt|;
block|}
else|else
block|{
comment|/* it wasn't a "Id$" or "Id:xxxx$" */
continue|continue;
block|}
comment|/* step 4: substitute */
name|strbuf_addstr
argument_list|(
name|buf
argument_list|,
literal|"Id: "
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
name|buf
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
literal|40
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
name|buf
argument_list|,
literal|" $"
argument_list|)
expr_stmt|;
block|}
name|strbuf_add
argument_list|(
name|buf
argument_list|,
name|src
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|to_free
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
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
DECL|function|git_path_check_convert
specifier|static
name|struct
name|convert_driver
modifier|*
name|git_path_check_convert
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
name|struct
name|convert_driver
modifier|*
name|drv
decl_stmt|;
if|if
condition|(
name|ATTR_TRUE
argument_list|(
name|value
argument_list|)
operator|||
name|ATTR_FALSE
argument_list|(
name|value
argument_list|)
operator|||
name|ATTR_UNSET
argument_list|(
name|value
argument_list|)
condition|)
return|return
name|NULL
return|;
for|for
control|(
name|drv
operator|=
name|user_convert
init|;
name|drv
condition|;
name|drv
operator|=
name|drv
operator|->
name|next
control|)
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|value
argument_list|,
name|drv
operator|->
name|name
argument_list|)
condition|)
return|return
name|drv
return|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|git_path_check_ident
specifier|static
name|int
name|git_path_check_ident
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
return|return
operator|!
operator|!
name|ATTR_TRUE
argument_list|(
name|value
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|convert_to_git
name|int
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
name|size_t
name|len
parameter_list|,
name|struct
name|strbuf
modifier|*
name|dst
parameter_list|)
block|{
name|struct
name|git_attr_check
name|check
index|[
literal|3
index|]
decl_stmt|;
name|int
name|crlf
init|=
name|CRLF_GUESS
decl_stmt|;
name|int
name|ident
init|=
literal|0
decl_stmt|,
name|ret
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|filter
init|=
name|NULL
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
name|ARRAY_SIZE
argument_list|(
name|check
argument_list|)
argument_list|,
name|check
argument_list|)
condition|)
block|{
name|struct
name|convert_driver
modifier|*
name|drv
decl_stmt|;
name|crlf
operator|=
name|git_path_check_crlf
argument_list|(
name|path
argument_list|,
name|check
operator|+
literal|0
argument_list|)
expr_stmt|;
name|ident
operator|=
name|git_path_check_ident
argument_list|(
name|path
argument_list|,
name|check
operator|+
literal|1
argument_list|)
expr_stmt|;
name|drv
operator|=
name|git_path_check_convert
argument_list|(
name|path
argument_list|,
name|check
operator|+
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|drv
operator|&&
name|drv
operator|->
name|clean
condition|)
name|filter
operator|=
name|drv
operator|->
name|clean
expr_stmt|;
block|}
name|ret
operator||=
name|apply_filter
argument_list|(
name|path
argument_list|,
name|src
argument_list|,
name|len
argument_list|,
name|dst
argument_list|,
name|filter
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
block|{
name|src
operator|=
name|dst
operator|->
name|buf
expr_stmt|;
name|len
operator|=
name|dst
operator|->
name|len
expr_stmt|;
block|}
name|ret
operator||=
name|crlf_to_git
argument_list|(
name|path
argument_list|,
name|src
argument_list|,
name|len
argument_list|,
name|dst
argument_list|,
name|crlf
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
block|{
name|src
operator|=
name|dst
operator|->
name|buf
expr_stmt|;
name|len
operator|=
name|dst
operator|->
name|len
expr_stmt|;
block|}
return|return
name|ret
operator||
name|ident_to_git
argument_list|(
name|path
argument_list|,
name|src
argument_list|,
name|len
argument_list|,
name|dst
argument_list|,
name|ident
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|convert_to_working_tree
name|int
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
name|size_t
name|len
parameter_list|,
name|struct
name|strbuf
modifier|*
name|dst
parameter_list|)
block|{
name|struct
name|git_attr_check
name|check
index|[
literal|3
index|]
decl_stmt|;
name|int
name|crlf
init|=
name|CRLF_GUESS
decl_stmt|;
name|int
name|ident
init|=
literal|0
decl_stmt|,
name|ret
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|filter
init|=
name|NULL
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
name|ARRAY_SIZE
argument_list|(
name|check
argument_list|)
argument_list|,
name|check
argument_list|)
condition|)
block|{
name|struct
name|convert_driver
modifier|*
name|drv
decl_stmt|;
name|crlf
operator|=
name|git_path_check_crlf
argument_list|(
name|path
argument_list|,
name|check
operator|+
literal|0
argument_list|)
expr_stmt|;
name|ident
operator|=
name|git_path_check_ident
argument_list|(
name|path
argument_list|,
name|check
operator|+
literal|1
argument_list|)
expr_stmt|;
name|drv
operator|=
name|git_path_check_convert
argument_list|(
name|path
argument_list|,
name|check
operator|+
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|drv
operator|&&
name|drv
operator|->
name|smudge
condition|)
name|filter
operator|=
name|drv
operator|->
name|smudge
expr_stmt|;
block|}
name|ret
operator||=
name|ident_to_worktree
argument_list|(
name|path
argument_list|,
name|src
argument_list|,
name|len
argument_list|,
name|dst
argument_list|,
name|ident
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
block|{
name|src
operator|=
name|dst
operator|->
name|buf
expr_stmt|;
name|len
operator|=
name|dst
operator|->
name|len
expr_stmt|;
block|}
name|ret
operator||=
name|crlf_to_worktree
argument_list|(
name|path
argument_list|,
name|src
argument_list|,
name|len
argument_list|,
name|dst
argument_list|,
name|crlf
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
block|{
name|src
operator|=
name|dst
operator|->
name|buf
expr_stmt|;
name|len
operator|=
name|dst
operator|->
name|len
expr_stmt|;
block|}
return|return
name|ret
operator||
name|apply_filter
argument_list|(
name|path
argument_list|,
name|src
argument_list|,
name|len
argument_list|,
name|dst
argument_list|,
name|filter
argument_list|)
return|;
block|}
end_function

end_unit

