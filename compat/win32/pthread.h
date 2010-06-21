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
value|(InitializeCriticalSection((a)), 0)
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

begin_typedef
DECL|typedef|pthread_mutexattr_t
typedef|typedef
name|int
name|pthread_mutexattr_t
typedef|;
end_typedef

begin_define
DECL|macro|pthread_mutexattr_init
define|#
directive|define
name|pthread_mutexattr_init
parameter_list|(
name|a
parameter_list|)
value|(*(a) = 0)
end_define

begin_define
DECL|macro|pthread_mutexattr_destroy
define|#
directive|define
name|pthread_mutexattr_destroy
parameter_list|(
name|a
parameter_list|)
value|do {} while (0)
end_define

begin_define
DECL|macro|pthread_mutexattr_settype
define|#
directive|define
name|pthread_mutexattr_settype
parameter_list|(
name|a
parameter_list|,
name|t
parameter_list|)
value|0
end_define

begin_define
DECL|macro|PTHREAD_MUTEX_RECURSIVE
define|#
directive|define
name|PTHREAD_MUTEX_RECURSIVE
value|0
end_define

begin_comment
comment|/*  * Implement simple condition variable for Windows threads, based on ACE  * implementation.  *  * See original implementation: http://bit.ly/1vkDjo  * ACE homepage: http://www.cse.wustl.edu/~schmidt/ACE.html  * See also: http://www.cse.wustl.edu/~schmidt/win32-cv-1.html  */
end_comment

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|waiters
name|LONG
name|waiters
decl_stmt|;
DECL|member|was_broadcast
name|int
name|was_broadcast
decl_stmt|;
DECL|member|waiters_lock
name|CRITICAL_SECTION
name|waiters_lock
decl_stmt|;
DECL|member|sema
name|HANDLE
name|sema
decl_stmt|;
DECL|member|continue_broadcast
name|HANDLE
name|continue_broadcast
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

begin_function_decl
specifier|extern
name|int
name|pthread_cond_broadcast
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
DECL|member|tid
name|DWORD
name|tid
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

begin_define
DECL|macro|pthread_equal
define|#
directive|define
name|pthread_equal
parameter_list|(
name|t1
parameter_list|,
name|t2
parameter_list|)
value|((t1).tid == (t2).tid)
end_define

begin_function_decl
specifier|extern
name|pthread_t
name|pthread_self
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|pthread_exit
specifier|static
specifier|inline
name|int
name|pthread_exit
parameter_list|(
name|void
modifier|*
name|ret
parameter_list|)
block|{
name|ExitThread
argument_list|(
operator|(
name|DWORD
operator|)
name|ret
argument_list|)
expr_stmt|;
block|}
end_function

begin_typedef
DECL|typedef|pthread_key_t
typedef|typedef
name|DWORD
name|pthread_key_t
typedef|;
end_typedef

begin_function
DECL|function|pthread_key_create
specifier|static
specifier|inline
name|int
name|pthread_key_create
parameter_list|(
name|pthread_key_t
modifier|*
name|keyp
parameter_list|,
name|void
function_decl|(
modifier|*
name|destructor
function_decl|)
parameter_list|(
name|void
modifier|*
name|value
parameter_list|)
parameter_list|)
block|{
return|return
operator|(
operator|*
name|keyp
operator|=
name|TlsAlloc
argument_list|()
operator|)
operator|==
name|TLS_OUT_OF_INDEXES
condition|?
name|EAGAIN
else|:
literal|0
return|;
block|}
end_function

begin_function
DECL|function|pthread_setspecific
specifier|static
specifier|inline
name|int
name|pthread_setspecific
parameter_list|(
name|pthread_key_t
name|key
parameter_list|,
specifier|const
name|void
modifier|*
name|value
parameter_list|)
block|{
return|return
name|TlsSetValue
argument_list|(
name|key
argument_list|,
operator|(
name|void
operator|*
operator|)
name|value
argument_list|)
condition|?
literal|0
else|:
name|EINVAL
return|;
block|}
end_function

begin_function
DECL|function|pthread_getspecific
specifier|static
specifier|inline
name|void
modifier|*
name|pthread_getspecific
parameter_list|(
name|pthread_key_t
name|key
parameter_list|)
block|{
return|return
name|TlsGetValue
argument_list|(
name|key
argument_list|)
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PTHREAD_H */
end_comment

end_unit

