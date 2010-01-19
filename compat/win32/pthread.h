begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Header used to adapt pthread-based POSIX code to Windows API threads.  *  * Copyright (C) 2009 Andrzej K. Haczewski<ahaczewski@gmail.com>  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|PTHREAD_H
end_ifndef

begin_define
DECL|macro|PTHREAD_H
define|#
directive|define
name|PTHREAD_H
end_define

begin_ifndef
ifndef|#
directive|ifndef
name|WIN32_LEAN_AND_MEAN
end_ifndef

begin_define
DECL|macro|WIN32_LEAN_AND_MEAN
define|#
directive|define
name|WIN32_LEAN_AND_MEAN
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|<windows.h>
end_include

begin_comment
comment|/*  * Defines that adapt Windows API threads to pthreads API  */
end_comment

begin_define
DECL|macro|pthread_mutex_t
define|#
directive|define
name|pthread_mutex_t
value|CRITICAL_SECTION
end_define

begin_define
DECL|macro|pthread_mutex_init
define|#
directive|define
name|pthread_mutex_init
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|InitializeCriticalSection((a))
end_define

begin_define
DECL|macro|pthread_mutex_destroy
define|#
directive|define
name|pthread_mutex_destroy
parameter_list|(
name|a
parameter_list|)
value|DeleteCriticalSection((a))
end_define

begin_define
DECL|macro|pthread_mutex_lock
define|#
directive|define
name|pthread_mutex_lock
value|EnterCriticalSection
end_define

begin_define
DECL|macro|pthread_mutex_unlock
define|#
directive|define
name|pthread_mutex_unlock
value|LeaveCriticalSection
end_define

begin_comment
comment|/*  * Implement simple condition variable for Windows threads, based on ACE  * implementation.  *  * See original implementation: http://bit.ly/1vkDjo  * ACE homepage: http://www.cse.wustl.edu/~schmidt/ACE.html  * See also: http://www.cse.wustl.edu/~schmidt/win32-cv-1.html  */
end_comment

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|waiters
specifier|volatile
name|LONG
name|waiters
decl_stmt|;
DECL|member|sema
name|HANDLE
name|sema
decl_stmt|;
DECL|typedef|pthread_cond_t
block|}
name|pthread_cond_t
typedef|;
end_typedef

begin_function_decl
specifier|extern
name|int
name|pthread_cond_init
parameter_list|(
name|pthread_cond_t
modifier|*
name|cond
parameter_list|,
specifier|const
name|void
modifier|*
name|unused
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|pthread_cond_destroy
parameter_list|(
name|pthread_cond_t
modifier|*
name|cond
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|pthread_cond_wait
parameter_list|(
name|pthread_cond_t
modifier|*
name|cond
parameter_list|,
name|CRITICAL_SECTION
modifier|*
name|mutex
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|pthread_cond_signal
parameter_list|(
name|pthread_cond_t
modifier|*
name|cond
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Simple thread creation implementation using pthread API  */
end_comment

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|handle
name|HANDLE
name|handle
decl_stmt|;
DECL|member|start_routine
name|void
modifier|*
function_decl|(
modifier|*
name|start_routine
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|)
function_decl|;
DECL|member|arg
name|void
modifier|*
name|arg
decl_stmt|;
DECL|typedef|pthread_t
block|}
name|pthread_t
typedef|;
end_typedef

begin_function_decl
specifier|extern
name|int
name|pthread_create
parameter_list|(
name|pthread_t
modifier|*
name|thread
parameter_list|,
specifier|const
name|void
modifier|*
name|unused
parameter_list|,
name|void
modifier|*
function_decl|(
modifier|*
name|start_routine
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|)
parameter_list|,
name|void
modifier|*
name|arg
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * To avoid the need of copying a struct, we use small macro wrapper to pass  * pointer to win32_pthread_join instead.  */
end_comment

begin_define
DECL|macro|pthread_join
define|#
directive|define
name|pthread_join
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|win32_pthread_join(&(a), (b))
end_define

begin_function_decl
specifier|extern
name|int
name|win32_pthread_join
parameter_list|(
name|pthread_t
modifier|*
name|thread
parameter_list|,
name|void
modifier|*
modifier|*
name|value_ptr
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PTHREAD_H */
end_comment

end_unit

