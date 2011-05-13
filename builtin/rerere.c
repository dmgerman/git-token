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
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_include
include|#
directive|include
file|"rerere.h"
end_include

begin_include
include|#
directive|include
file|"xdiff/xdiff.h"
end_include

begin_include
include|#
directive|include
file|"xdiff-interface.h"
end_include

begin_decl_stmt
DECL|variable|rerere_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|rerere_usage
index|[]
init|=
block|{
literal|"git rerere [clear | forget path... | status | remaining | diff | gc]"
block|,
name|NULL
block|, }
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|outf
specifier|static
name|int
name|outf
parameter_list|(
name|void
modifier|*
name|dummy
parameter_list|,
name|mmbuffer_t
modifier|*
name|ptr
parameter_list|,
name|int
name|nbuf
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nbuf
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|write_in_full
argument_list|(
literal|1
argument_list|,
name|ptr
index|[
name|i
index|]
operator|.
name|ptr
argument_list|,
name|ptr
index|[
name|i
index|]
operator|.
name|size
argument_list|)
operator|!=
name|ptr
index|[
name|i
index|]
operator|.
name|size
condition|)
return|return
operator|-
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diff_two
specifier|static
name|int
name|diff_two
parameter_list|(
specifier|const
name|char
modifier|*
name|file1
parameter_list|,
specifier|const
name|char
modifier|*
name|label1
parameter_list|,
specifier|const
name|char
modifier|*
name|file2
parameter_list|,
specifier|const
name|char
modifier|*
name|label2
parameter_list|)
block|{
name|xpparam_t
name|xpp
decl_stmt|;
name|xdemitconf_t
name|xecfg
decl_stmt|;
name|xdemitcb_t
name|ecb
decl_stmt|;
name|mmfile_t
name|minus
decl_stmt|,
name|plus
decl_stmt|;
if|if
condition|(
name|read_mmfile
argument_list|(
operator|&
name|minus
argument_list|,
name|file1
argument_list|)
operator|||
name|read_mmfile
argument_list|(
operator|&
name|plus
argument_list|,
name|file2
argument_list|)
condition|)
return|return
literal|1
return|;
name|printf
argument_list|(
literal|"--- a/%s\n+++ b/%s\n"
argument_list|,
name|label1
argument_list|,
name|label2
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|xpp
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|xpp
argument_list|)
argument_list|)
expr_stmt|;
name|xpp
operator|.
name|flags
operator|=
literal|0
expr_stmt|;
name|memset
argument_list|(
operator|&
name|xecfg
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|xecfg
argument_list|)
argument_list|)
expr_stmt|;
name|xecfg
operator|.
name|ctxlen
operator|=
literal|3
expr_stmt|;
name|ecb
operator|.
name|outf
operator|=
name|outf
expr_stmt|;
name|xdi_diff
argument_list|(
operator|&
name|minus
argument_list|,
operator|&
name|plus
argument_list|,
operator|&
name|xpp
argument_list|,
operator|&
name|xecfg
argument_list|,
operator|&
name|ecb
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|minus
operator|.
name|ptr
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|plus
operator|.
name|ptr
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_rerere
name|int
name|cmd_rerere
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
name|string_list
name|merge_rr
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
name|int
name|i
decl_stmt|,
name|fd
decl_stmt|,
name|autoupdate
init|=
operator|-
literal|1
decl_stmt|,
name|flags
init|=
literal|0
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"rerere-autoupdate"
argument_list|,
operator|&
name|autoupdate
argument_list|,
literal|"register clean resolutions in index"
argument_list|,
literal|1
argument_list|)
block|,
name|OPT_END
argument_list|()
block|, 	}
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
name|rerere_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|autoupdate
operator|==
literal|1
condition|)
name|flags
operator|=
name|RERERE_AUTOUPDATE
expr_stmt|;
if|if
condition|(
name|autoupdate
operator|==
literal|0
condition|)
name|flags
operator|=
name|RERERE_NOAUTOUPDATE
expr_stmt|;
if|if
condition|(
name|argc
operator|<
literal|1
condition|)
return|return
name|rerere
argument_list|(
name|flags
argument_list|)
return|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
literal|"forget"
argument_list|)
condition|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
decl_stmt|;
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
name|warning
argument_list|(
literal|"'git rerere forget' without paths is deprecated"
argument_list|)
expr_stmt|;
name|pathspec
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
return|return
name|rerere_forget
argument_list|(
name|pathspec
argument_list|)
return|;
block|}
name|fd
operator|=
name|setup_rerere
argument_list|(
operator|&
name|merge_rr
argument_list|,
name|flags
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
literal|"clear"
argument_list|)
condition|)
block|{
name|rerere_clear
argument_list|(
operator|&
name|merge_rr
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
literal|"gc"
argument_list|)
condition|)
name|rerere_gc
argument_list|(
operator|&
name|merge_rr
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
literal|"status"
argument_list|)
condition|)
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|merge_rr
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
literal|"remaining"
argument_list|)
condition|)
block|{
name|rerere_remaining
argument_list|(
operator|&
name|merge_rr
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
name|merge_rr
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|util
operator|!=
name|RERERE_RESOLVED
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
else|else
comment|/* prepare for later call to 				 * string_list_clear() */
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|util
operator|=
name|NULL
expr_stmt|;
block|}
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
literal|"diff"
argument_list|)
condition|)
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|merge_rr
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
decl_stmt|;
specifier|const
name|char
modifier|*
name|name
init|=
operator|(
specifier|const
name|char
operator|*
operator|)
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|util
decl_stmt|;
name|diff_two
argument_list|(
name|rerere_path
argument_list|(
name|name
argument_list|,
literal|"preimage"
argument_list|)
argument_list|,
name|path
argument_list|,
name|path
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
else|else
name|usage_with_options
argument_list|(
name|rerere_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|string_list_clear
argument_list|(
operator|&
name|merge_rr
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

