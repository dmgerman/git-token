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
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_include
include|#
directive|include
file|"pathspec.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_decl_stmt
DECL|variable|quiet
DECL|variable|verbose
DECL|variable|stdin_paths
DECL|variable|show_non_matching
DECL|variable|no_index
specifier|static
name|int
name|quiet
decl_stmt|,
name|verbose
decl_stmt|,
name|stdin_paths
decl_stmt|,
name|show_non_matching
decl_stmt|,
name|no_index
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_ignore_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|check_ignore_usage
index|[]
init|=
block|{
literal|"git check-ignore [<options>]<pathname>..."
block|,
literal|"git check-ignore [<options>] --stdin"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nul_term_line
specifier|static
name|int
name|nul_term_line
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_ignore_options
specifier|static
specifier|const
name|struct
name|option
name|check_ignore_options
index|[]
init|=
block|{
name|OPT__QUIET
argument_list|(
operator|&
name|quiet
argument_list|,
name|N_
argument_list|(
literal|"suppress progress reporting"
argument_list|)
argument_list|)
block|,
name|OPT__VERBOSE
argument_list|(
operator|&
name|verbose
argument_list|,
name|N_
argument_list|(
literal|"be verbose"
argument_list|)
argument_list|)
block|,
name|OPT_GROUP
argument_list|(
literal|""
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
name|nul_term_line
argument_list|,
name|N_
argument_list|(
literal|"terminate input and output records by a NUL character"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|'n'
argument_list|,
literal|"non-matching"
argument_list|,
operator|&
name|show_non_matching
argument_list|,
name|N_
argument_list|(
literal|"show non-matching input paths"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"no-index"
argument_list|,
operator|&
name|no_index
argument_list|,
name|N_
argument_list|(
literal|"ignore index when checking"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|output_exclude
specifier|static
name|void
name|output_exclude
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|exclude
modifier|*
name|exclude
parameter_list|)
block|{
name|char
modifier|*
name|bang
init|=
operator|(
name|exclude
operator|&&
name|exclude
operator|->
name|flags
operator|&
name|EXC_FLAG_NEGATIVE
operator|)
condition|?
literal|"!"
else|:
literal|""
decl_stmt|;
name|char
modifier|*
name|slash
init|=
operator|(
name|exclude
operator|&&
name|exclude
operator|->
name|flags
operator|&
name|EXC_FLAG_MUSTBEDIR
operator|)
condition|?
literal|"/"
else|:
literal|""
decl_stmt|;
if|if
condition|(
operator|!
name|nul_term_line
condition|)
block|{
if|if
condition|(
operator|!
name|verbose
condition|)
block|{
name|write_name_quoted
argument_list|(
name|path
argument_list|,
name|stdout
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|exclude
condition|)
block|{
name|quote_c_style
argument_list|(
name|exclude
operator|->
name|el
operator|->
name|src
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
literal|":%d:%s%s%s\t"
argument_list|,
name|exclude
operator|->
name|srcpos
argument_list|,
name|bang
argument_list|,
name|exclude
operator|->
name|pattern
argument_list|,
name|slash
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"::\t"
argument_list|)
expr_stmt|;
block|}
name|quote_c_style
argument_list|(
name|path
argument_list|,
name|NULL
argument_list|,
name|stdout
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|fputc
argument_list|(
literal|'\n'
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
if|if
condition|(
operator|!
name|verbose
condition|)
block|{
name|printf
argument_list|(
literal|"%s%c"
argument_list|,
name|path
argument_list|,
literal|'\0'
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|exclude
condition|)
name|printf
argument_list|(
literal|"%s%c%d%c%s%s%s%c%s%c"
argument_list|,
name|exclude
operator|->
name|el
operator|->
name|src
argument_list|,
literal|'\0'
argument_list|,
name|exclude
operator|->
name|srcpos
argument_list|,
literal|'\0'
argument_list|,
name|bang
argument_list|,
name|exclude
operator|->
name|pattern
argument_list|,
name|slash
argument_list|,
literal|'\0'
argument_list|,
name|path
argument_list|,
literal|'\0'
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%c%c%c%s%c"
argument_list|,
literal|'\0'
argument_list|,
literal|'\0'
argument_list|,
literal|'\0'
argument_list|,
name|path
argument_list|,
literal|'\0'
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|check_ignore
specifier|static
name|int
name|check_ignore
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|full_path
decl_stmt|;
name|char
modifier|*
name|seen
decl_stmt|;
name|int
name|num_ignored
init|=
literal|0
decl_stmt|,
name|dtype
init|=
name|DT_UNKNOWN
decl_stmt|,
name|i
decl_stmt|;
name|struct
name|exclude
modifier|*
name|exclude
decl_stmt|;
name|struct
name|pathspec
name|pathspec
decl_stmt|;
if|if
condition|(
operator|!
name|argc
condition|)
block|{
if|if
condition|(
operator|!
name|quiet
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"no pathspec given.\n"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
comment|/* 	 * check-ignore just needs paths. Magic beyond :/ is really 	 * irrelevant. 	 */
name|parse_pathspec
argument_list|(
operator|&
name|pathspec
argument_list|,
name|PATHSPEC_ALL_MAGIC
operator|&
operator|~
name|PATHSPEC_FROMTOP
argument_list|,
name|PATHSPEC_SYMLINK_LEADING_PATH
operator||
name|PATHSPEC_STRIP_SUBMODULE_SLASH_EXPENSIVE
operator||
name|PATHSPEC_KEEP_ORDER
argument_list|,
name|prefix
argument_list|,
name|argv
argument_list|)
expr_stmt|;
comment|/* 	 * look for pathspecs matching entries in the index, since these 	 * should not be ignored, in order to be consistent with 	 * 'git status', 'git add' etc. 	 */
name|seen
operator|=
name|find_pathspecs_matching_against_index
argument_list|(
operator|&
name|pathspec
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
name|pathspec
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|full_path
operator|=
name|pathspec
operator|.
name|items
index|[
name|i
index|]
operator|.
name|match
expr_stmt|;
name|exclude
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
operator|!
name|seen
index|[
name|i
index|]
condition|)
block|{
name|exclude
operator|=
name|last_exclude_matching
argument_list|(
name|dir
argument_list|,
name|full_path
argument_list|,
operator|&
name|dtype
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|quiet
operator|&&
operator|(
name|exclude
operator|||
name|show_non_matching
operator|)
condition|)
name|output_exclude
argument_list|(
name|pathspec
operator|.
name|items
index|[
name|i
index|]
operator|.
name|original
argument_list|,
name|exclude
argument_list|)
expr_stmt|;
if|if
condition|(
name|exclude
condition|)
name|num_ignored
operator|++
expr_stmt|;
block|}
name|free
argument_list|(
name|seen
argument_list|)
expr_stmt|;
return|return
name|num_ignored
return|;
block|}
end_function

begin_function
DECL|function|check_ignore_stdin_paths
specifier|static
name|int
name|check_ignore_stdin_paths
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
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
name|struct
name|strbuf
name|unquoted
init|=
name|STRBUF_INIT
decl_stmt|;
name|char
modifier|*
name|pathspec
index|[
literal|2
index|]
init|=
block|{
name|NULL
block|,
name|NULL
block|}
decl_stmt|;
name|strbuf_getline_fn
name|getline_fn
decl_stmt|;
name|int
name|num_ignored
init|=
literal|0
decl_stmt|;
name|getline_fn
operator|=
name|nul_term_line
condition|?
name|strbuf_getline_nul
else|:
name|strbuf_getline_lf
expr_stmt|;
while|while
condition|(
name|getline_fn
argument_list|(
operator|&
name|buf
argument_list|,
name|stdin
argument_list|)
operator|!=
name|EOF
condition|)
block|{
if|if
condition|(
operator|!
name|nul_term_line
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
name|unquoted
argument_list|)
expr_stmt|;
if|if
condition|(
name|unquote_c_style
argument_list|(
operator|&
name|unquoted
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
name|unquoted
argument_list|)
expr_stmt|;
block|}
name|pathspec
index|[
literal|0
index|]
operator|=
name|buf
operator|.
name|buf
expr_stmt|;
name|num_ignored
operator|+=
name|check_ignore
argument_list|(
name|dir
argument_list|,
name|prefix
argument_list|,
literal|1
argument_list|,
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|pathspec
argument_list|)
expr_stmt|;
name|maybe_flush_or_die
argument_list|(
name|stdout
argument_list|,
literal|"check-ignore to stdout"
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
name|unquoted
argument_list|)
expr_stmt|;
return|return
name|num_ignored
return|;
block|}
end_function

begin_function
DECL|function|cmd_check_ignore
name|int
name|cmd_check_ignore
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
name|num_ignored
decl_stmt|;
name|struct
name|dir_struct
name|dir
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
name|check_ignore_options
argument_list|,
name|check_ignore_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|stdin_paths
condition|)
block|{
if|if
condition|(
name|argc
operator|>
literal|0
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"cannot specify pathnames with --stdin"
argument_list|)
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|nul_term_line
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"-z only makes sense with --stdin"
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|==
literal|0
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"no path specified"
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|quiet
condition|)
block|{
if|if
condition|(
name|argc
operator|>
literal|1
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"--quiet is only valid with a single pathname"
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"cannot have both --quiet and --verbose"
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|show_non_matching
operator|&&
operator|!
name|verbose
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"--non-matching is only valid with --verbose"
argument_list|)
argument_list|)
expr_stmt|;
comment|/* read_cache() is only necessary so we can watch out for submodules. */
if|if
condition|(
operator|!
name|no_index
operator|&&
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"index file corrupt"
argument_list|)
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|dir
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|dir
argument_list|)
argument_list|)
expr_stmt|;
name|setup_standard_excludes
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|stdin_paths
condition|)
block|{
name|num_ignored
operator|=
name|check_ignore_stdin_paths
argument_list|(
operator|&
name|dir
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|num_ignored
operator|=
name|check_ignore
argument_list|(
operator|&
name|dir
argument_list|,
name|prefix
argument_list|,
name|argc
argument_list|,
name|argv
argument_list|)
expr_stmt|;
name|maybe_flush_or_die
argument_list|(
name|stdout
argument_list|,
literal|"ignore to stdout"
argument_list|)
expr_stmt|;
block|}
name|clear_directory
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
return|return
operator|!
name|num_ignored
return|;
block|}
end_function

end_unit

