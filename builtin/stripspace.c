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
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_function
DECL|function|comment_lines
specifier|static
name|void
name|comment_lines
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|)
block|{
name|char
modifier|*
name|msg
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|msg
operator|=
name|strbuf_detach
argument_list|(
name|buf
argument_list|,
operator|&
name|len
argument_list|)
expr_stmt|;
name|strbuf_add_commented_lines
argument_list|(
name|buf
argument_list|,
name|msg
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|msg
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|stripspace_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|stripspace_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git stripspace [-s | --strip-comments]"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git stripspace [-c | --comment-lines]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_enum
DECL|enum|stripspace_mode
enum|enum
name|stripspace_mode
block|{
DECL|enumerator|STRIP_DEFAULT
name|STRIP_DEFAULT
init|=
literal|0
block|,
DECL|enumerator|STRIP_COMMENTS
name|STRIP_COMMENTS
block|,
DECL|enumerator|COMMENT_LINES
name|COMMENT_LINES
block|}
enum|;
end_enum

begin_function
DECL|function|cmd_stripspace
name|int
name|cmd_stripspace
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
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|enum
name|stripspace_mode
name|mode
init|=
name|STRIP_DEFAULT
decl_stmt|;
specifier|const
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_CMDMODE
argument_list|(
literal|'s'
argument_list|,
literal|"strip-comments"
argument_list|,
operator|&
name|mode
argument_list|,
name|N_
argument_list|(
literal|"skip and remove all lines starting with comment character"
argument_list|)
argument_list|,
name|STRIP_COMMENTS
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|'c'
argument_list|,
literal|"comment-lines"
argument_list|,
operator|&
name|mode
argument_list|,
name|N_
argument_list|(
literal|"prepend comment character and space to each line"
argument_list|)
argument_list|,
name|COMMENT_LINES
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
name|options
argument_list|,
name|stripspace_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
condition|)
name|usage_with_options
argument_list|(
name|stripspace_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|mode
operator|==
name|STRIP_COMMENTS
operator|||
name|mode
operator|==
name|COMMENT_LINES
condition|)
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|strbuf_read
argument_list|(
operator|&
name|buf
argument_list|,
literal|0
argument_list|,
literal|1024
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"could not read the input"
argument_list|)
expr_stmt|;
if|if
condition|(
name|mode
operator|==
name|STRIP_DEFAULT
operator|||
name|mode
operator|==
name|STRIP_COMMENTS
condition|)
name|strbuf_stripspace
argument_list|(
operator|&
name|buf
argument_list|,
name|mode
operator|==
name|STRIP_COMMENTS
argument_list|)
expr_stmt|;
else|else
name|comment_lines
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|buf
operator|.
name|len
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

