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
file|"attr.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_decl_stmt
DECL|variable|all_attrs
specifier|static
name|int
name|all_attrs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cached_attrs
specifier|static
name|int
name|cached_attrs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|stdin_paths
specifier|static
name|int
name|stdin_paths
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_attr_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|check_attr_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git check-attr [-a | --all | attr...] [--] pathname..."
argument_list|)
block|,
name|N_
argument_list|(
literal|"git check-attr --stdin [-z] [-a | --all | attr...]<<list-of-paths>"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|null_term_line
specifier|static
name|int
name|null_term_line
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_attr_options
specifier|static
specifier|const
name|struct
name|option
name|check_attr_options
index|[]
init|=
block|{
name|OPT_BOOL
argument_list|(
literal|'a'
argument_list|,
literal|"all"
argument_list|,
operator|&
name|all_attrs
argument_list|,
name|N_
argument_list|(
literal|"report all attributes set on file"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"cached"
argument_list|,
operator|&
name|cached_attrs
argument_list|,
name|N_
argument_list|(
literal|"use .gitattributes only from the index"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"stdin"
argument_list|,
operator|&
name|stdin_paths
argument_list|,
name|N_
argument_list|(
literal|"read file names from stdin"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|'z'
argument_list|,
name|NULL
argument_list|,
operator|&
name|null_term_line
argument_list|,
name|N_
argument_list|(
literal|"input paths are terminated by a null character"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|output_attr
specifier|static
name|void
name|output_attr
parameter_list|(
name|int
name|cnt
parameter_list|,
name|struct
name|git_attr_check
modifier|*
name|check
parameter_list|,
specifier|const
name|char
modifier|*
name|file
parameter_list|)
block|{
name|int
name|j
decl_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|cnt
condition|;
name|j
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|value
init|=
name|check
index|[
name|j
index|]
operator|.
name|value
decl_stmt|;
if|if
condition|(
name|ATTR_TRUE
argument_list|(
name|value
argument_list|)
condition|)
name|value
operator|=
literal|"set"
expr_stmt|;
elseif|else
if|if
condition|(
name|ATTR_FALSE
argument_list|(
name|value
argument_list|)
condition|)
name|value
operator|=
literal|"unset"
expr_stmt|;
elseif|else
if|if
condition|(
name|ATTR_UNSET
argument_list|(
name|value
argument_list|)
condition|)
name|value
operator|=
literal|"unspecified"
expr_stmt|;
name|quote_c_style
argument_list|(
name|file
argument_list|,
name|NULL
argument_list|,
name|stdout
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|": %s: %s\n"
argument_list|,
name|git_attr_name
argument_list|(
name|check
index|[
name|j
index|]
operator|.
name|attr
argument_list|)
argument_list|,
name|value
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|check_attr
specifier|static
name|void
name|check_attr
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|cnt
parameter_list|,
name|struct
name|git_attr_check
modifier|*
name|check
parameter_list|,
specifier|const
name|char
modifier|*
name|file
parameter_list|)
block|{
name|char
modifier|*
name|full_path
init|=
name|prefix_path
argument_list|(
name|prefix
argument_list|,
name|prefix
condition|?
name|strlen
argument_list|(
name|prefix
argument_list|)
else|:
literal|0
argument_list|,
name|file
argument_list|)
decl_stmt|;
if|if
condition|(
name|check
operator|!=
name|NULL
condition|)
block|{
if|if
condition|(
name|git_check_attr
argument_list|(
name|full_path
argument_list|,
name|cnt
argument_list|,
name|check
argument_list|)
condition|)
name|die
argument_list|(
literal|"git_check_attr died"
argument_list|)
expr_stmt|;
name|output_attr
argument_list|(
name|cnt
argument_list|,
name|check
argument_list|,
name|file
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|git_all_attrs
argument_list|(
name|full_path
argument_list|,
operator|&
name|cnt
argument_list|,
operator|&
name|check
argument_list|)
condition|)
name|die
argument_list|(
literal|"git_all_attrs died"
argument_list|)
expr_stmt|;
name|output_attr
argument_list|(
name|cnt
argument_list|,
name|check
argument_list|,
name|file
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|check
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|full_path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|check_attr_stdin_paths
specifier|static
name|void
name|check_attr_stdin_paths
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|cnt
parameter_list|,
name|struct
name|git_attr_check
modifier|*
name|check
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
decl_stmt|,
name|nbuf
decl_stmt|;
name|int
name|line_termination
init|=
name|null_term_line
condition|?
literal|0
else|:
literal|'\n'
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|buf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|nbuf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
while|while
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|buf
argument_list|,
name|stdin
argument_list|,
name|line_termination
argument_list|)
operator|!=
name|EOF
condition|)
block|{
if|if
condition|(
name|line_termination
operator|&&
name|buf
operator|.
name|buf
index|[
literal|0
index|]
operator|==
literal|'"'
condition|)
block|{
name|strbuf_reset
argument_list|(
operator|&
name|nbuf
argument_list|)
expr_stmt|;
if|if
condition|(
name|unquote_c_style
argument_list|(
operator|&
name|nbuf
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|NULL
argument_list|)
condition|)
name|die
argument_list|(
literal|"line is badly quoted"
argument_list|)
expr_stmt|;
name|strbuf_swap
argument_list|(
operator|&
name|buf
argument_list|,
operator|&
name|nbuf
argument_list|)
expr_stmt|;
block|}
name|check_attr
argument_list|(
name|prefix
argument_list|,
name|cnt
argument_list|,
name|check
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|maybe_flush_or_die
argument_list|(
name|stdout
argument_list|,
literal|"attribute to stdout"
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|nbuf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|error_with_usage
specifier|static
name|NORETURN
name|void
name|error_with_usage
parameter_list|(
specifier|const
name|char
modifier|*
name|msg
parameter_list|)
block|{
name|error
argument_list|(
literal|"%s"
argument_list|,
name|msg
argument_list|)
expr_stmt|;
name|usage_with_options
argument_list|(
name|check_attr_usage
argument_list|,
name|check_attr_options
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_check_attr
name|int
name|cmd_check_attr
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
name|git_attr_check
modifier|*
name|check
decl_stmt|;
name|int
name|cnt
decl_stmt|,
name|i
decl_stmt|,
name|doubledash
decl_stmt|,
name|filei
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
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
name|check_attr_options
argument_list|,
name|check_attr_usage
argument_list|,
name|PARSE_OPT_KEEP_DASHDASH
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
block|{
name|die
argument_list|(
literal|"invalid cache"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|cached_attrs
condition|)
name|git_attr_set_direction
argument_list|(
name|GIT_ATTR_INDEX
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|doubledash
operator|=
operator|-
literal|1
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|doubledash
operator|<
literal|0
operator|&&
name|i
operator|<
name|argc
condition|;
name|i
operator|++
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
name|doubledash
operator|=
name|i
expr_stmt|;
block|}
comment|/* Process --all and/or attribute arguments: */
if|if
condition|(
name|all_attrs
condition|)
block|{
if|if
condition|(
name|doubledash
operator|>=
literal|1
condition|)
name|error_with_usage
argument_list|(
literal|"Attributes and --all both specified"
argument_list|)
expr_stmt|;
name|cnt
operator|=
literal|0
expr_stmt|;
name|filei
operator|=
name|doubledash
operator|+
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|doubledash
operator|==
literal|0
condition|)
block|{
name|error_with_usage
argument_list|(
literal|"No attribute specified"
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|doubledash
operator|<
literal|0
condition|)
block|{
if|if
condition|(
operator|!
name|argc
condition|)
name|error_with_usage
argument_list|(
literal|"No attribute specified"
argument_list|)
expr_stmt|;
if|if
condition|(
name|stdin_paths
condition|)
block|{
comment|/* Treat all arguments as attribute names. */
name|cnt
operator|=
name|argc
expr_stmt|;
name|filei
operator|=
name|argc
expr_stmt|;
block|}
else|else
block|{
comment|/* Treat exactly one argument as an attribute name. */
name|cnt
operator|=
literal|1
expr_stmt|;
name|filei
operator|=
literal|1
expr_stmt|;
block|}
block|}
else|else
block|{
name|cnt
operator|=
name|doubledash
expr_stmt|;
name|filei
operator|=
name|doubledash
operator|+
literal|1
expr_stmt|;
block|}
comment|/* Check file argument(s): */
if|if
condition|(
name|stdin_paths
condition|)
block|{
if|if
condition|(
name|filei
operator|<
name|argc
condition|)
name|error_with_usage
argument_list|(
literal|"Can't specify files with --stdin"
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|filei
operator|>=
name|argc
condition|)
name|error_with_usage
argument_list|(
literal|"No file specified"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|all_attrs
condition|)
block|{
name|check
operator|=
name|NULL
expr_stmt|;
block|}
else|else
block|{
name|check
operator|=
name|xcalloc
argument_list|(
name|cnt
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|check
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
name|cnt
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
name|struct
name|git_attr
modifier|*
name|a
decl_stmt|;
name|name
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
name|a
operator|=
name|git_attr
argument_list|(
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|a
condition|)
return|return
name|error
argument_list|(
literal|"%s: not a valid attribute name"
argument_list|,
name|name
argument_list|)
return|;
name|check
index|[
name|i
index|]
operator|.
name|attr
operator|=
name|a
expr_stmt|;
block|}
block|}
if|if
condition|(
name|stdin_paths
condition|)
name|check_attr_stdin_paths
argument_list|(
name|prefix
argument_list|,
name|cnt
argument_list|,
name|check
argument_list|)
expr_stmt|;
else|else
block|{
for|for
control|(
name|i
operator|=
name|filei
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
name|check_attr
argument_list|(
name|prefix
argument_list|,
name|cnt
argument_list|,
name|check
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|maybe_flush_or_die
argument_list|(
name|stdout
argument_list|,
literal|"attribute to stdout"
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

