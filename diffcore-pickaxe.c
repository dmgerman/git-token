begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2005 Junio C Hamano  * Copyright (C) 2010 Google Inc.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_include
include|#
directive|include
file|"xdiff-interface.h"
end_include

begin_include
include|#
directive|include
file|"kwset.h"
end_include

begin_typedef
DECL|typedef|pickaxe_fn
typedef|typedef
name|int
function_decl|(
modifier|*
name|pickaxe_fn
function_decl|)
parameter_list|(
name|mmfile_t
modifier|*
name|one
parameter_list|,
name|mmfile_t
modifier|*
name|two
parameter_list|,
name|struct
name|diff_options
modifier|*
name|o
parameter_list|,
name|regex_t
modifier|*
name|regexp
parameter_list|,
name|kwset_t
name|kws
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|diffgrep_cb
struct|struct
name|diffgrep_cb
block|{
DECL|member|regexp
name|regex_t
modifier|*
name|regexp
decl_stmt|;
DECL|member|hit
name|int
name|hit
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|diffgrep_consume
specifier|static
name|void
name|diffgrep_consume
parameter_list|(
name|void
modifier|*
name|priv
parameter_list|,
name|char
modifier|*
name|line
parameter_list|,
name|unsigned
name|long
name|len
parameter_list|)
block|{
name|struct
name|diffgrep_cb
modifier|*
name|data
init|=
name|priv
decl_stmt|;
name|regmatch_t
name|regmatch
decl_stmt|;
name|int
name|hold
decl_stmt|;
if|if
condition|(
name|line
index|[
literal|0
index|]
operator|!=
literal|'+'
operator|&&
name|line
index|[
literal|0
index|]
operator|!=
literal|'-'
condition|)
return|return;
if|if
condition|(
name|data
operator|->
name|hit
condition|)
comment|/* 		 * NEEDSWORK: we should have a way to terminate the 		 * caller early. 		 */
return|return;
comment|/* Yuck -- line ought to be "const char *"! */
name|hold
operator|=
name|line
index|[
name|len
index|]
expr_stmt|;
name|line
index|[
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
name|data
operator|->
name|hit
operator|=
operator|!
name|regexec
argument_list|(
name|data
operator|->
name|regexp
argument_list|,
name|line
operator|+
literal|1
argument_list|,
literal|1
argument_list|,
operator|&
name|regmatch
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|line
index|[
name|len
index|]
operator|=
name|hold
expr_stmt|;
block|}
end_function

begin_function
DECL|function|diff_grep
specifier|static
name|int
name|diff_grep
parameter_list|(
name|mmfile_t
modifier|*
name|one
parameter_list|,
name|mmfile_t
modifier|*
name|two
parameter_list|,
name|struct
name|diff_options
modifier|*
name|o
parameter_list|,
name|regex_t
modifier|*
name|regexp
parameter_list|,
name|kwset_t
name|kws
parameter_list|)
block|{
name|regmatch_t
name|regmatch
decl_stmt|;
name|struct
name|diffgrep_cb
name|ecbdata
decl_stmt|;
name|xpparam_t
name|xpp
decl_stmt|;
name|xdemitconf_t
name|xecfg
decl_stmt|;
if|if
condition|(
operator|!
name|one
condition|)
return|return
operator|!
name|regexec
argument_list|(
name|regexp
argument_list|,
name|two
operator|->
name|ptr
argument_list|,
literal|1
argument_list|,
operator|&
name|regmatch
argument_list|,
literal|0
argument_list|)
return|;
if|if
condition|(
operator|!
name|two
condition|)
return|return
operator|!
name|regexec
argument_list|(
name|regexp
argument_list|,
name|one
operator|->
name|ptr
argument_list|,
literal|1
argument_list|,
operator|&
name|regmatch
argument_list|,
literal|0
argument_list|)
return|;
comment|/* 	 * We have both sides; need to run textual diff and see if 	 * the pattern appears on added/deleted lines. 	 */
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
name|ecbdata
operator|.
name|regexp
operator|=
name|regexp
expr_stmt|;
name|ecbdata
operator|.
name|hit
operator|=
literal|0
expr_stmt|;
name|xecfg
operator|.
name|ctxlen
operator|=
name|o
operator|->
name|context
expr_stmt|;
name|xecfg
operator|.
name|interhunkctxlen
operator|=
name|o
operator|->
name|interhunkcontext
expr_stmt|;
name|xdi_diff_outf
argument_list|(
name|one
argument_list|,
name|two
argument_list|,
name|diffgrep_consume
argument_list|,
operator|&
name|ecbdata
argument_list|,
operator|&
name|xpp
argument_list|,
operator|&
name|xecfg
argument_list|)
expr_stmt|;
return|return
name|ecbdata
operator|.
name|hit
return|;
block|}
end_function

begin_function
DECL|function|contains
specifier|static
name|unsigned
name|int
name|contains
parameter_list|(
name|mmfile_t
modifier|*
name|mf
parameter_list|,
name|regex_t
modifier|*
name|regexp
parameter_list|,
name|kwset_t
name|kws
parameter_list|)
block|{
name|unsigned
name|int
name|cnt
decl_stmt|;
name|unsigned
name|long
name|sz
decl_stmt|;
specifier|const
name|char
modifier|*
name|data
decl_stmt|;
name|sz
operator|=
name|mf
operator|->
name|size
expr_stmt|;
name|data
operator|=
name|mf
operator|->
name|ptr
expr_stmt|;
name|cnt
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|regexp
condition|)
block|{
name|regmatch_t
name|regmatch
decl_stmt|;
name|int
name|flags
init|=
literal|0
decl_stmt|;
name|assert
argument_list|(
name|data
index|[
name|sz
index|]
operator|==
literal|'\0'
argument_list|)
expr_stmt|;
while|while
condition|(
operator|*
name|data
operator|&&
operator|!
name|regexec
argument_list|(
name|regexp
argument_list|,
name|data
argument_list|,
literal|1
argument_list|,
operator|&
name|regmatch
argument_list|,
name|flags
argument_list|)
condition|)
block|{
name|flags
operator||=
name|REG_NOTBOL
expr_stmt|;
name|data
operator|+=
name|regmatch
operator|.
name|rm_eo
expr_stmt|;
if|if
condition|(
operator|*
name|data
operator|&&
name|regmatch
operator|.
name|rm_so
operator|==
name|regmatch
operator|.
name|rm_eo
condition|)
name|data
operator|++
expr_stmt|;
name|cnt
operator|++
expr_stmt|;
block|}
block|}
else|else
block|{
comment|/* Classic exact string match */
while|while
condition|(
name|sz
condition|)
block|{
name|struct
name|kwsmatch
name|kwsm
decl_stmt|;
name|size_t
name|offset
init|=
name|kwsexec
argument_list|(
name|kws
argument_list|,
name|data
argument_list|,
name|sz
argument_list|,
operator|&
name|kwsm
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|found
decl_stmt|;
if|if
condition|(
name|offset
operator|==
operator|-
literal|1
condition|)
break|break;
else|else
name|found
operator|=
name|data
operator|+
name|offset
expr_stmt|;
name|sz
operator|-=
name|found
operator|-
name|data
operator|+
name|kwsm
operator|.
name|size
index|[
literal|0
index|]
expr_stmt|;
name|data
operator|=
name|found
operator|+
name|kwsm
operator|.
name|size
index|[
literal|0
index|]
expr_stmt|;
name|cnt
operator|++
expr_stmt|;
block|}
block|}
return|return
name|cnt
return|;
block|}
end_function

begin_function
DECL|function|has_changes
specifier|static
name|int
name|has_changes
parameter_list|(
name|mmfile_t
modifier|*
name|one
parameter_list|,
name|mmfile_t
modifier|*
name|two
parameter_list|,
name|struct
name|diff_options
modifier|*
name|o
parameter_list|,
name|regex_t
modifier|*
name|regexp
parameter_list|,
name|kwset_t
name|kws
parameter_list|)
block|{
name|unsigned
name|int
name|one_contains
init|=
name|one
condition|?
name|contains
argument_list|(
name|one
argument_list|,
name|regexp
argument_list|,
name|kws
argument_list|)
else|:
literal|0
decl_stmt|;
name|unsigned
name|int
name|two_contains
init|=
name|two
condition|?
name|contains
argument_list|(
name|two
argument_list|,
name|regexp
argument_list|,
name|kws
argument_list|)
else|:
literal|0
decl_stmt|;
return|return
name|one_contains
operator|!=
name|two_contains
return|;
block|}
end_function

begin_function
DECL|function|pickaxe_match
specifier|static
name|int
name|pickaxe_match
parameter_list|(
name|struct
name|diff_filepair
modifier|*
name|p
parameter_list|,
name|struct
name|diff_options
modifier|*
name|o
parameter_list|,
name|regex_t
modifier|*
name|regexp
parameter_list|,
name|kwset_t
name|kws
parameter_list|,
name|pickaxe_fn
name|fn
parameter_list|)
block|{
name|struct
name|userdiff_driver
modifier|*
name|textconv_one
init|=
name|NULL
decl_stmt|;
name|struct
name|userdiff_driver
modifier|*
name|textconv_two
init|=
name|NULL
decl_stmt|;
name|mmfile_t
name|mf1
decl_stmt|,
name|mf2
decl_stmt|;
name|int
name|ret
decl_stmt|;
if|if
condition|(
operator|!
name|o
operator|->
name|pickaxe
index|[
literal|0
index|]
condition|)
return|return
literal|0
return|;
comment|/* ignore unmerged */
if|if
condition|(
operator|!
name|DIFF_FILE_VALID
argument_list|(
name|p
operator|->
name|one
argument_list|)
operator|&&
operator|!
name|DIFF_FILE_VALID
argument_list|(
name|p
operator|->
name|two
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|DIFF_OPT_TST
argument_list|(
name|o
argument_list|,
name|ALLOW_TEXTCONV
argument_list|)
condition|)
block|{
name|textconv_one
operator|=
name|get_textconv
argument_list|(
name|p
operator|->
name|one
argument_list|)
expr_stmt|;
name|textconv_two
operator|=
name|get_textconv
argument_list|(
name|p
operator|->
name|two
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * If we have an unmodified pair, we know that the count will be the 	 * same and don't even have to load the blobs. Unless textconv is in 	 * play, _and_ we are using two different textconv filters (e.g., 	 * because a pair is an exact rename with different textconv attributes 	 * for each side, which might generate different content). 	 */
if|if
condition|(
name|textconv_one
operator|==
name|textconv_two
operator|&&
name|diff_unmodified_pair
argument_list|(
name|p
argument_list|)
condition|)
return|return
literal|0
return|;
name|mf1
operator|.
name|size
operator|=
name|fill_textconv
argument_list|(
name|textconv_one
argument_list|,
name|p
operator|->
name|one
argument_list|,
operator|&
name|mf1
operator|.
name|ptr
argument_list|)
expr_stmt|;
name|mf2
operator|.
name|size
operator|=
name|fill_textconv
argument_list|(
name|textconv_two
argument_list|,
name|p
operator|->
name|two
argument_list|,
operator|&
name|mf2
operator|.
name|ptr
argument_list|)
expr_stmt|;
name|ret
operator|=
name|fn
argument_list|(
name|DIFF_FILE_VALID
argument_list|(
name|p
operator|->
name|one
argument_list|)
condition|?
operator|&
name|mf1
else|:
name|NULL
argument_list|,
name|DIFF_FILE_VALID
argument_list|(
name|p
operator|->
name|two
argument_list|)
condition|?
operator|&
name|mf2
else|:
name|NULL
argument_list|,
name|o
argument_list|,
name|regexp
argument_list|,
name|kws
argument_list|)
expr_stmt|;
if|if
condition|(
name|textconv_one
condition|)
name|free
argument_list|(
name|mf1
operator|.
name|ptr
argument_list|)
expr_stmt|;
if|if
condition|(
name|textconv_two
condition|)
name|free
argument_list|(
name|mf2
operator|.
name|ptr
argument_list|)
expr_stmt|;
name|diff_free_filespec_data
argument_list|(
name|p
operator|->
name|one
argument_list|)
expr_stmt|;
name|diff_free_filespec_data
argument_list|(
name|p
operator|->
name|two
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|pickaxe
specifier|static
name|void
name|pickaxe
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
name|q
parameter_list|,
name|struct
name|diff_options
modifier|*
name|o
parameter_list|,
name|regex_t
modifier|*
name|regexp
parameter_list|,
name|kwset_t
name|kws
parameter_list|,
name|pickaxe_fn
name|fn
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|struct
name|diff_queue_struct
name|outq
decl_stmt|;
name|DIFF_QUEUE_CLEAR
argument_list|(
operator|&
name|outq
argument_list|)
expr_stmt|;
if|if
condition|(
name|o
operator|->
name|pickaxe_opts
operator|&
name|DIFF_PICKAXE_ALL
condition|)
block|{
comment|/* Showing the whole changeset if needle exists */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|diff_filepair
modifier|*
name|p
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|pickaxe_match
argument_list|(
name|p
argument_list|,
name|o
argument_list|,
name|regexp
argument_list|,
name|kws
argument_list|,
name|fn
argument_list|)
condition|)
return|return;
comment|/* do not munge the queue */
block|}
comment|/* 		 * Otherwise we will clear the whole queue by copying 		 * the empty outq at the end of this function, but 		 * first clear the current entries in the queue. 		 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
name|diff_free_filepair
argument_list|(
name|q
operator|->
name|queue
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* Showing only the filepairs that has the needle */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|diff_filepair
modifier|*
name|p
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|pickaxe_match
argument_list|(
name|p
argument_list|,
name|o
argument_list|,
name|regexp
argument_list|,
name|kws
argument_list|,
name|fn
argument_list|)
condition|)
name|diff_q
argument_list|(
operator|&
name|outq
argument_list|,
name|p
argument_list|)
expr_stmt|;
else|else
name|diff_free_filepair
argument_list|(
name|p
argument_list|)
expr_stmt|;
block|}
block|}
name|free
argument_list|(
name|q
operator|->
name|queue
argument_list|)
expr_stmt|;
operator|*
name|q
operator|=
name|outq
expr_stmt|;
block|}
end_function

begin_function
DECL|function|diffcore_pickaxe
name|void
name|diffcore_pickaxe
parameter_list|(
name|struct
name|diff_options
modifier|*
name|o
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|needle
init|=
name|o
operator|->
name|pickaxe
decl_stmt|;
name|int
name|opts
init|=
name|o
operator|->
name|pickaxe_opts
decl_stmt|;
name|regex_t
name|regex
decl_stmt|,
modifier|*
name|regexp
init|=
name|NULL
decl_stmt|;
name|kwset_t
name|kws
init|=
name|NULL
decl_stmt|;
if|if
condition|(
name|opts
operator|&
operator|(
name|DIFF_PICKAXE_REGEX
operator||
name|DIFF_PICKAXE_KIND_G
operator|)
condition|)
block|{
name|int
name|err
decl_stmt|;
name|int
name|cflags
init|=
name|REG_EXTENDED
operator||
name|REG_NEWLINE
decl_stmt|;
if|if
condition|(
name|DIFF_OPT_TST
argument_list|(
name|o
argument_list|,
name|PICKAXE_IGNORE_CASE
argument_list|)
condition|)
name|cflags
operator||=
name|REG_ICASE
expr_stmt|;
name|err
operator|=
name|regcomp
argument_list|(
operator|&
name|regex
argument_list|,
name|needle
argument_list|,
name|cflags
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
condition|)
block|{
comment|/* The POSIX.2 people are surely sick */
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
name|regex
argument_list|,
name|errbuf
argument_list|,
literal|1024
argument_list|)
expr_stmt|;
name|regfree
argument_list|(
operator|&
name|regex
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"invalid regex: %s"
argument_list|,
name|errbuf
argument_list|)
expr_stmt|;
block|}
name|regexp
operator|=
operator|&
name|regex
expr_stmt|;
block|}
else|else
block|{
name|kws
operator|=
name|kwsalloc
argument_list|(
name|DIFF_OPT_TST
argument_list|(
name|o
argument_list|,
name|PICKAXE_IGNORE_CASE
argument_list|)
condition|?
name|tolower_trans_tbl
else|:
name|NULL
argument_list|)
expr_stmt|;
name|kwsincr
argument_list|(
name|kws
argument_list|,
name|needle
argument_list|,
name|strlen
argument_list|(
name|needle
argument_list|)
argument_list|)
expr_stmt|;
name|kwsprep
argument_list|(
name|kws
argument_list|)
expr_stmt|;
block|}
comment|/* Might want to warn when both S and G are on; I don't care... */
name|pickaxe
argument_list|(
operator|&
name|diff_queued_diff
argument_list|,
name|o
argument_list|,
name|regexp
argument_list|,
name|kws
argument_list|,
operator|(
name|opts
operator|&
name|DIFF_PICKAXE_KIND_G
operator|)
condition|?
name|diff_grep
else|:
name|has_changes
argument_list|)
expr_stmt|;
if|if
condition|(
name|regexp
condition|)
name|regfree
argument_list|(
name|regexp
argument_list|)
expr_stmt|;
else|else
name|kwsfree
argument_list|(
name|kws
argument_list|)
expr_stmt|;
return|return;
block|}
end_function

end_unit

