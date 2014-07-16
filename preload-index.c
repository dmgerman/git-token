begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2008 Linus Torvalds  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"pathspec.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_ifdef
ifdef|#
directive|ifdef
name|NO_PTHREADS
end_ifdef

begin_function
DECL|function|preload_index
specifier|static
name|void
name|preload_index
parameter_list|(
name|struct
name|index_state
modifier|*
name|index
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|)
block|{
empty_stmt|;
comment|/* nothing */
block|}
end_function

begin_else
else|#
directive|else
end_else

begin_include
include|#
directive|include
file|<pthread.h>
end_include

begin_comment
comment|/*  * Mostly randomly chosen maximum thread counts: we  * cap the parallelism to 20 threads, and we want  * to have at least 500 lstat's per thread for it to  * be worth starting a thread.  */
end_comment

begin_define
DECL|macro|MAX_PARALLEL
define|#
directive|define
name|MAX_PARALLEL
value|(20)
end_define

begin_define
DECL|macro|THREAD_COST
define|#
directive|define
name|THREAD_COST
value|(500)
end_define

begin_struct
DECL|struct|thread_data
struct|struct
name|thread_data
block|{
DECL|member|pthread
name|pthread_t
name|pthread
decl_stmt|;
DECL|member|index
name|struct
name|index_state
modifier|*
name|index
decl_stmt|;
DECL|member|pathspec
name|struct
name|pathspec
name|pathspec
decl_stmt|;
DECL|member|offset
DECL|member|nr
name|int
name|offset
decl_stmt|,
name|nr
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|preload_thread
specifier|static
name|void
modifier|*
name|preload_thread
parameter_list|(
name|void
modifier|*
name|_data
parameter_list|)
block|{
name|int
name|nr
decl_stmt|;
name|struct
name|thread_data
modifier|*
name|p
init|=
name|_data
decl_stmt|;
name|struct
name|index_state
modifier|*
name|index
init|=
name|p
operator|->
name|index
decl_stmt|;
name|struct
name|cache_entry
modifier|*
modifier|*
name|cep
init|=
name|index
operator|->
name|cache
operator|+
name|p
operator|->
name|offset
decl_stmt|;
name|struct
name|cache_def
name|cache
init|=
name|CACHE_DEF_INIT
decl_stmt|;
name|nr
operator|=
name|p
operator|->
name|nr
expr_stmt|;
if|if
condition|(
name|nr
operator|+
name|p
operator|->
name|offset
operator|>
name|index
operator|->
name|cache_nr
condition|)
name|nr
operator|=
name|index
operator|->
name|cache_nr
operator|-
name|p
operator|->
name|offset
expr_stmt|;
do|do
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
operator|*
name|cep
operator|++
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|S_ISGITLINK
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|ce_uptodate
argument_list|(
name|ce
argument_list|)
condition|)
continue|continue;
if|if
condition|(
operator|!
name|ce_path_match
argument_list|(
name|ce
argument_list|,
operator|&
name|p
operator|->
name|pathspec
argument_list|,
name|NULL
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|threaded_has_symlink_leading_path
argument_list|(
operator|&
name|cache
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|lstat
argument_list|(
name|ce
operator|->
name|name
argument_list|,
operator|&
name|st
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|ie_match_stat
argument_list|(
name|index
argument_list|,
name|ce
argument_list|,
operator|&
name|st
argument_list|,
name|CE_MATCH_RACY_IS_DIRTY
argument_list|)
condition|)
continue|continue;
name|ce_mark_uptodate
argument_list|(
name|ce
argument_list|)
expr_stmt|;
block|}
do|while
condition|(
operator|--
name|nr
operator|>
literal|0
condition|)
do|;
name|cache_def_clear
argument_list|(
operator|&
name|cache
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|preload_index
specifier|static
name|void
name|preload_index
parameter_list|(
name|struct
name|index_state
modifier|*
name|index
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|)
block|{
name|int
name|threads
decl_stmt|,
name|i
decl_stmt|,
name|work
decl_stmt|,
name|offset
decl_stmt|;
name|struct
name|thread_data
name|data
index|[
name|MAX_PARALLEL
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|core_preload_index
condition|)
return|return;
name|threads
operator|=
name|index
operator|->
name|cache_nr
operator|/
name|THREAD_COST
expr_stmt|;
if|if
condition|(
name|threads
operator|<
literal|2
condition|)
return|return;
if|if
condition|(
name|threads
operator|>
name|MAX_PARALLEL
condition|)
name|threads
operator|=
name|MAX_PARALLEL
expr_stmt|;
name|offset
operator|=
literal|0
expr_stmt|;
name|work
operator|=
name|DIV_ROUND_UP
argument_list|(
name|index
operator|->
name|cache_nr
argument_list|,
name|threads
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|data
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|data
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
name|threads
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|thread_data
modifier|*
name|p
init|=
name|data
operator|+
name|i
decl_stmt|;
name|p
operator|->
name|index
operator|=
name|index
expr_stmt|;
if|if
condition|(
name|pathspec
condition|)
name|copy_pathspec
argument_list|(
operator|&
name|p
operator|->
name|pathspec
argument_list|,
name|pathspec
argument_list|)
expr_stmt|;
name|p
operator|->
name|offset
operator|=
name|offset
expr_stmt|;
name|p
operator|->
name|nr
operator|=
name|work
expr_stmt|;
name|offset
operator|+=
name|work
expr_stmt|;
if|if
condition|(
name|pthread_create
argument_list|(
operator|&
name|p
operator|->
name|pthread
argument_list|,
name|NULL
argument_list|,
name|preload_thread
argument_list|,
name|p
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to create threaded lstat"
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
name|threads
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|thread_data
modifier|*
name|p
init|=
name|data
operator|+
name|i
decl_stmt|;
if|if
condition|(
name|pthread_join
argument_list|(
name|p
operator|->
name|pthread
argument_list|,
name|NULL
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to join threaded lstat"
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|read_index_preload
name|int
name|read_index_preload
parameter_list|(
name|struct
name|index_state
modifier|*
name|index
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|)
block|{
name|int
name|retval
init|=
name|read_index
argument_list|(
name|index
argument_list|)
decl_stmt|;
name|preload_index
argument_list|(
name|index
argument_list|,
name|pathspec
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

end_unit

