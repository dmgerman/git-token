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
file|"path-list.h"
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
DECL|variable|git_rerere_usage
specifier|static
specifier|const
name|char
name|git_rerere_usage
index|[]
init|=
literal|"git-rerere [clear | status | diff | gc]"
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* these values are days */
end_comment

begin_decl_stmt
DECL|variable|cutoff_noresolve
specifier|static
name|int
name|cutoff_noresolve
init|=
literal|15
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cutoff_resolve
specifier|static
name|int
name|cutoff_resolve
init|=
literal|60
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|rr_path
specifier|static
specifier|const
name|char
modifier|*
name|rr_path
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|file
parameter_list|)
block|{
return|return
name|git_path
argument_list|(
literal|"rr-cache/%s/%s"
argument_list|,
name|name
argument_list|,
name|file
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|rerere_created_at
specifier|static
name|time_t
name|rerere_created_at
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
return|return
name|stat
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"preimage"
argument_list|)
argument_list|,
operator|&
name|st
argument_list|)
condition|?
operator|(
name|time_t
operator|)
literal|0
else|:
name|st
operator|.
name|st_mtime
return|;
block|}
end_function

begin_function
DECL|function|has_resolution
specifier|static
name|int
name|has_resolution
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
return|return
operator|!
name|stat
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"postimage"
argument_list|)
argument_list|,
operator|&
name|st
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|unlink_rr_item
specifier|static
name|void
name|unlink_rr_item
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|unlink
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"thisimage"
argument_list|)
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"preimage"
argument_list|)
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"postimage"
argument_list|)
argument_list|)
expr_stmt|;
name|rmdir
argument_list|(
name|git_path
argument_list|(
literal|"rr-cache/%s"
argument_list|,
name|name
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_rerere_gc_config
specifier|static
name|int
name|git_rerere_gc_config
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
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"gc.rerereresolved"
argument_list|)
condition|)
name|cutoff_resolve
operator|=
name|git_config_int
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"gc.rerereunresolved"
argument_list|)
condition|)
name|cutoff_noresolve
operator|=
name|git_config_int
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
else|else
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
name|cb
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|garbage_collect
specifier|static
name|void
name|garbage_collect
parameter_list|(
name|struct
name|path_list
modifier|*
name|rr
parameter_list|)
block|{
name|struct
name|path_list
name|to_remove
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|1
block|}
decl_stmt|;
name|DIR
modifier|*
name|dir
decl_stmt|;
name|struct
name|dirent
modifier|*
name|e
decl_stmt|;
name|int
name|i
decl_stmt|,
name|cutoff
decl_stmt|;
name|time_t
name|now
init|=
name|time
argument_list|(
name|NULL
argument_list|)
decl_stmt|,
name|then
decl_stmt|;
name|git_config
argument_list|(
name|git_rerere_gc_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|dir
operator|=
name|opendir
argument_list|(
name|git_path
argument_list|(
literal|"rr-cache"
argument_list|)
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|e
operator|=
name|readdir
argument_list|(
name|dir
argument_list|)
operator|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|name
init|=
name|e
operator|->
name|d_name
decl_stmt|;
if|if
condition|(
name|name
index|[
literal|0
index|]
operator|==
literal|'.'
operator|&&
operator|(
name|name
index|[
literal|1
index|]
operator|==
literal|'\0'
operator|||
operator|(
name|name
index|[
literal|1
index|]
operator|==
literal|'.'
operator|&&
name|name
index|[
literal|2
index|]
operator|==
literal|'\0'
operator|)
operator|)
condition|)
continue|continue;
name|then
operator|=
name|rerere_created_at
argument_list|(
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|then
condition|)
continue|continue;
name|cutoff
operator|=
operator|(
name|has_resolution
argument_list|(
name|name
argument_list|)
condition|?
name|cutoff_resolve
else|:
name|cutoff_noresolve
operator|)
expr_stmt|;
if|if
condition|(
name|then
operator|<
name|now
operator|-
name|cutoff
operator|*
literal|86400
condition|)
name|path_list_append
argument_list|(
name|name
argument_list|,
operator|&
name|to_remove
argument_list|)
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|to_remove
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|unlink_rr_item
argument_list|(
name|to_remove
operator|.
name|items
index|[
name|i
index|]
operator|.
name|path
argument_list|)
expr_stmt|;
name|path_list_clear
argument_list|(
operator|&
name|to_remove
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

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
name|xpp
operator|.
name|flags
operator|=
name|XDF_NEED_MINIMAL
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
name|path_list
name|merge_rr
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|1
block|}
decl_stmt|;
name|int
name|i
decl_stmt|,
name|fd
decl_stmt|;
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
return|return
name|rerere
argument_list|()
return|;
name|fd
operator|=
name|setup_rerere
argument_list|(
operator|&
name|merge_rr
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
literal|1
index|]
argument_list|,
literal|"clear"
argument_list|)
condition|)
block|{
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
if|if
condition|(
operator|!
name|has_resolution
argument_list|(
name|name
argument_list|)
condition|)
name|unlink_rr_item
argument_list|(
name|name
argument_list|)
expr_stmt|;
block|}
name|unlink
argument_list|(
name|git_path
argument_list|(
literal|"rr-cache/MERGE_RR"
argument_list|)
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
literal|1
index|]
argument_list|,
literal|"gc"
argument_list|)
condition|)
name|garbage_collect
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
literal|1
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
name|path
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
literal|1
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
name|path
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
name|rr_path
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
name|usage
argument_list|(
name|git_rerere_usage
argument_list|)
expr_stmt|;
name|path_list_clear
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

