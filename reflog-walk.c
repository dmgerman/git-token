begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"path-list.h"
end_include

begin_include
include|#
directive|include
file|"reflog-walk.h"
end_include

begin_struct
DECL|struct|complete_reflogs
struct|struct
name|complete_reflogs
block|{
DECL|member|ref
name|char
modifier|*
name|ref
decl_stmt|;
DECL|struct|reflog_info
struct|struct
name|reflog_info
block|{
DECL|member|osha1
DECL|member|nsha1
name|unsigned
name|char
name|osha1
index|[
literal|20
index|]
decl_stmt|,
name|nsha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|email
name|char
modifier|*
name|email
decl_stmt|;
DECL|member|timestamp
name|unsigned
name|long
name|timestamp
decl_stmt|;
DECL|member|tz
name|int
name|tz
decl_stmt|;
DECL|member|message
name|char
modifier|*
name|message
decl_stmt|;
DECL|member|items
block|}
modifier|*
name|items
struct|;
DECL|member|nr
DECL|member|alloc
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|read_one_reflog
specifier|static
name|int
name|read_one_reflog
parameter_list|(
name|unsigned
name|char
modifier|*
name|osha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|nsha1
parameter_list|,
specifier|const
name|char
modifier|*
name|email
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|,
name|int
name|tz
parameter_list|,
specifier|const
name|char
modifier|*
name|message
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|struct
name|complete_reflogs
modifier|*
name|array
init|=
name|cb_data
decl_stmt|;
name|struct
name|reflog_info
modifier|*
name|item
decl_stmt|;
if|if
condition|(
name|array
operator|->
name|nr
operator|>=
name|array
operator|->
name|alloc
condition|)
block|{
name|array
operator|->
name|alloc
operator|=
name|alloc_nr
argument_list|(
name|array
operator|->
name|nr
operator|+
literal|1
argument_list|)
expr_stmt|;
name|array
operator|->
name|items
operator|=
name|xrealloc
argument_list|(
name|array
operator|->
name|items
argument_list|,
name|array
operator|->
name|alloc
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|reflog_info
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|item
operator|=
name|array
operator|->
name|items
operator|+
name|array
operator|->
name|nr
expr_stmt|;
name|memcpy
argument_list|(
name|item
operator|->
name|osha1
argument_list|,
name|osha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|item
operator|->
name|nsha1
argument_list|,
name|nsha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|item
operator|->
name|email
operator|=
name|xstrdup
argument_list|(
name|email
argument_list|)
expr_stmt|;
name|item
operator|->
name|timestamp
operator|=
name|timestamp
expr_stmt|;
name|item
operator|->
name|tz
operator|=
name|tz
expr_stmt|;
name|item
operator|->
name|message
operator|=
name|xstrdup
argument_list|(
name|message
argument_list|)
expr_stmt|;
name|array
operator|->
name|nr
operator|++
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|read_complete_reflog
specifier|static
name|struct
name|complete_reflogs
modifier|*
name|read_complete_reflog
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|)
block|{
name|struct
name|complete_reflogs
modifier|*
name|reflogs
init|=
name|xcalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|complete_reflogs
argument_list|)
argument_list|,
literal|1
argument_list|)
decl_stmt|;
name|reflogs
operator|->
name|ref
operator|=
name|xstrdup
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|for_each_reflog_ent
argument_list|(
name|ref
argument_list|,
name|read_one_reflog
argument_list|,
name|reflogs
argument_list|)
expr_stmt|;
if|if
condition|(
name|reflogs
operator|->
name|nr
operator|==
literal|0
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|name
init|=
name|resolve_ref
argument_list|(
name|ref
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
decl_stmt|;
if|if
condition|(
name|name
condition|)
name|for_each_reflog_ent
argument_list|(
name|name
argument_list|,
name|read_one_reflog
argument_list|,
name|reflogs
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|reflogs
operator|->
name|nr
operator|==
literal|0
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|ref
argument_list|)
decl_stmt|;
name|char
modifier|*
name|refname
init|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|12
argument_list|)
decl_stmt|;
name|sprintf
argument_list|(
name|refname
argument_list|,
literal|"refs/%s"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
name|for_each_reflog_ent
argument_list|(
name|refname
argument_list|,
name|read_one_reflog
argument_list|,
name|reflogs
argument_list|)
expr_stmt|;
if|if
condition|(
name|reflogs
operator|->
name|nr
operator|==
literal|0
condition|)
block|{
name|sprintf
argument_list|(
name|refname
argument_list|,
literal|"refs/heads/%s"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
name|for_each_reflog_ent
argument_list|(
name|refname
argument_list|,
name|read_one_reflog
argument_list|,
name|reflogs
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|refname
argument_list|)
expr_stmt|;
block|}
return|return
name|reflogs
return|;
block|}
end_function

begin_function
DECL|function|get_reflog_recno_by_time
specifier|static
name|int
name|get_reflog_recno_by_time
parameter_list|(
name|struct
name|complete_reflogs
modifier|*
name|array
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
name|array
operator|->
name|nr
operator|-
literal|1
init|;
name|i
operator|>=
literal|0
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|timestamp
operator|>=
name|array
operator|->
name|items
index|[
name|i
index|]
operator|.
name|timestamp
condition|)
return|return
name|i
return|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_struct
DECL|struct|commit_info_lifo
struct|struct
name|commit_info_lifo
block|{
DECL|struct|commit_info
struct|struct
name|commit_info
block|{
DECL|member|commit
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
DECL|member|util
name|void
modifier|*
name|util
decl_stmt|;
DECL|member|items
block|}
modifier|*
name|items
struct|;
DECL|member|nr
DECL|member|alloc
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|get_commit_info
specifier|static
name|struct
name|commit_info
modifier|*
name|get_commit_info
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|struct
name|commit_info_lifo
modifier|*
name|lifo
parameter_list|,
name|int
name|pop
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
name|lifo
operator|->
name|nr
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|lifo
operator|->
name|items
index|[
name|i
index|]
operator|.
name|commit
operator|==
name|commit
condition|)
block|{
name|struct
name|commit_info
modifier|*
name|result
init|=
operator|&
name|lifo
operator|->
name|items
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|pop
condition|)
block|{
if|if
condition|(
name|i
operator|+
literal|1
operator|<
name|lifo
operator|->
name|nr
condition|)
name|memmove
argument_list|(
name|lifo
operator|->
name|items
operator|+
name|i
argument_list|,
name|lifo
operator|->
name|items
operator|+
name|i
operator|+
literal|1
argument_list|,
operator|(
name|lifo
operator|->
name|nr
operator|-
name|i
operator|)
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|commit_info
argument_list|)
argument_list|)
expr_stmt|;
name|lifo
operator|->
name|nr
operator|--
expr_stmt|;
block|}
return|return
name|result
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|add_commit_info
specifier|static
name|void
name|add_commit_info
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|void
modifier|*
name|util
parameter_list|,
name|struct
name|commit_info_lifo
modifier|*
name|lifo
parameter_list|)
block|{
name|struct
name|commit_info
modifier|*
name|info
decl_stmt|;
if|if
condition|(
name|lifo
operator|->
name|nr
operator|>=
name|lifo
operator|->
name|alloc
condition|)
block|{
name|lifo
operator|->
name|alloc
operator|=
name|alloc_nr
argument_list|(
name|lifo
operator|->
name|nr
operator|+
literal|1
argument_list|)
expr_stmt|;
name|lifo
operator|->
name|items
operator|=
name|xrealloc
argument_list|(
name|lifo
operator|->
name|items
argument_list|,
name|lifo
operator|->
name|alloc
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|commit_info
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|info
operator|=
name|lifo
operator|->
name|items
operator|+
name|lifo
operator|->
name|nr
expr_stmt|;
name|info
operator|->
name|commit
operator|=
name|commit
expr_stmt|;
name|info
operator|->
name|util
operator|=
name|util
expr_stmt|;
name|lifo
operator|->
name|nr
operator|++
expr_stmt|;
block|}
end_function

begin_struct
DECL|struct|commit_reflog
struct|struct
name|commit_reflog
block|{
DECL|member|flag
DECL|member|recno
name|int
name|flag
decl_stmt|,
name|recno
decl_stmt|;
DECL|member|reflogs
name|struct
name|complete_reflogs
modifier|*
name|reflogs
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|reflog_walk_info
struct|struct
name|reflog_walk_info
block|{
DECL|member|reflogs
name|struct
name|commit_info_lifo
name|reflogs
decl_stmt|;
DECL|member|complete_reflogs
name|struct
name|path_list
name|complete_reflogs
decl_stmt|;
DECL|member|last_commit_reflog
name|struct
name|commit_reflog
modifier|*
name|last_commit_reflog
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|init_reflog_walk
name|void
name|init_reflog_walk
parameter_list|(
name|struct
name|reflog_walk_info
modifier|*
modifier|*
name|info
parameter_list|)
block|{
operator|*
name|info
operator|=
name|xcalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|reflog_walk_info
argument_list|)
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_reflog_for_walk
name|void
name|add_reflog_for_walk
parameter_list|(
name|struct
name|reflog_walk_info
modifier|*
name|info
parameter_list|,
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|unsigned
name|long
name|timestamp
init|=
literal|0
decl_stmt|;
name|int
name|recno
init|=
operator|-
literal|1
decl_stmt|;
name|struct
name|path_list_item
modifier|*
name|item
decl_stmt|;
name|struct
name|complete_reflogs
modifier|*
name|reflogs
decl_stmt|;
name|char
modifier|*
name|branch
decl_stmt|,
modifier|*
name|at
init|=
name|strchr
argument_list|(
name|name
argument_list|,
literal|'@'
argument_list|)
decl_stmt|;
name|struct
name|commit_reflog
modifier|*
name|commit_reflog
decl_stmt|;
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|UNINTERESTING
condition|)
name|die
argument_list|(
literal|"Cannot walk reflogs for %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|branch
operator|=
name|xstrdup
argument_list|(
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|at
operator|&&
name|at
index|[
literal|1
index|]
operator|==
literal|'{'
condition|)
block|{
name|char
modifier|*
name|ep
decl_stmt|;
name|branch
index|[
name|at
operator|-
name|name
index|]
operator|=
literal|'\0'
expr_stmt|;
name|recno
operator|=
name|strtoul
argument_list|(
name|at
operator|+
literal|2
argument_list|,
operator|&
name|ep
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|ep
operator|!=
literal|'}'
condition|)
block|{
name|recno
operator|=
operator|-
literal|1
expr_stmt|;
name|timestamp
operator|=
name|approxidate
argument_list|(
name|at
operator|+
literal|2
argument_list|)
expr_stmt|;
block|}
block|}
else|else
name|recno
operator|=
literal|0
expr_stmt|;
name|item
operator|=
name|path_list_lookup
argument_list|(
name|branch
argument_list|,
operator|&
name|info
operator|->
name|complete_reflogs
argument_list|)
expr_stmt|;
if|if
condition|(
name|item
condition|)
name|reflogs
operator|=
name|item
operator|->
name|util
expr_stmt|;
else|else
block|{
name|reflogs
operator|=
name|read_complete_reflog
argument_list|(
name|branch
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|reflogs
operator|||
name|reflogs
operator|->
name|nr
operator|==
literal|0
condition|)
name|die
argument_list|(
literal|"No reflogs found for '%s'"
argument_list|,
name|branch
argument_list|)
expr_stmt|;
name|path_list_insert
argument_list|(
name|branch
argument_list|,
operator|&
name|info
operator|->
name|complete_reflogs
argument_list|)
operator|->
name|util
operator|=
name|reflogs
expr_stmt|;
block|}
name|commit_reflog
operator|=
name|xcalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|commit_reflog
argument_list|)
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|recno
operator|<
literal|0
condition|)
block|{
name|commit_reflog
operator|->
name|flag
operator|=
literal|1
expr_stmt|;
name|commit_reflog
operator|->
name|recno
operator|=
name|get_reflog_recno_by_time
argument_list|(
name|reflogs
argument_list|,
name|timestamp
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit_reflog
operator|->
name|recno
operator|<
literal|0
condition|)
block|{
name|free
argument_list|(
name|branch
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|commit_reflog
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
else|else
name|commit_reflog
operator|->
name|recno
operator|=
name|reflogs
operator|->
name|nr
operator|-
name|recno
operator|-
literal|1
expr_stmt|;
name|commit_reflog
operator|->
name|reflogs
operator|=
name|reflogs
expr_stmt|;
name|add_commit_info
argument_list|(
name|commit
argument_list|,
name|commit_reflog
argument_list|,
operator|&
name|info
operator|->
name|reflogs
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|fake_reflog_parent
name|void
name|fake_reflog_parent
parameter_list|(
name|struct
name|reflog_walk_info
modifier|*
name|info
parameter_list|,
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
name|struct
name|commit_info
modifier|*
name|commit_info
init|=
name|get_commit_info
argument_list|(
name|commit
argument_list|,
operator|&
name|info
operator|->
name|reflogs
argument_list|,
literal|0
argument_list|)
decl_stmt|;
name|struct
name|commit_reflog
modifier|*
name|commit_reflog
decl_stmt|;
name|struct
name|reflog_info
modifier|*
name|reflog
decl_stmt|;
name|info
operator|->
name|last_commit_reflog
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
operator|!
name|commit_info
condition|)
return|return;
name|commit_reflog
operator|=
name|commit_info
operator|->
name|util
expr_stmt|;
if|if
condition|(
name|commit_reflog
operator|->
name|recno
operator|<
literal|0
condition|)
block|{
name|commit
operator|->
name|parents
operator|=
name|NULL
expr_stmt|;
return|return;
block|}
name|reflog
operator|=
operator|&
name|commit_reflog
operator|->
name|reflogs
operator|->
name|items
index|[
name|commit_reflog
operator|->
name|recno
index|]
expr_stmt|;
name|info
operator|->
name|last_commit_reflog
operator|=
name|commit_reflog
expr_stmt|;
name|commit_reflog
operator|->
name|recno
operator|--
expr_stmt|;
name|commit_info
operator|->
name|commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|parse_object
argument_list|(
name|reflog
operator|->
name|osha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit_info
operator|->
name|commit
condition|)
block|{
name|commit
operator|->
name|parents
operator|=
name|NULL
expr_stmt|;
return|return;
block|}
name|commit
operator|->
name|parents
operator|=
name|xcalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|commit_list
argument_list|)
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|commit
operator|->
name|parents
operator|->
name|item
operator|=
name|commit_info
operator|->
name|commit
expr_stmt|;
name|commit
operator|->
name|object
operator|.
name|flags
operator|&=
operator|~
operator|(
name|ADDED
operator||
name|SEEN
operator||
name|SHOWN
operator|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_reflog_message
name|void
name|show_reflog_message
parameter_list|(
name|struct
name|reflog_walk_info
modifier|*
name|info
parameter_list|,
name|int
name|oneline
parameter_list|)
block|{
if|if
condition|(
name|info
operator|&&
name|info
operator|->
name|last_commit_reflog
condition|)
block|{
name|struct
name|commit_reflog
modifier|*
name|commit_reflog
init|=
name|info
operator|->
name|last_commit_reflog
decl_stmt|;
name|struct
name|reflog_info
modifier|*
name|info
decl_stmt|;
name|info
operator|=
operator|&
name|commit_reflog
operator|->
name|reflogs
operator|->
name|items
index|[
name|commit_reflog
operator|->
name|recno
operator|+
literal|1
index|]
expr_stmt|;
if|if
condition|(
name|oneline
condition|)
block|{
name|printf
argument_list|(
literal|"%s@{"
argument_list|,
name|commit_reflog
operator|->
name|reflogs
operator|->
name|ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit_reflog
operator|->
name|flag
condition|)
name|printf
argument_list|(
literal|"%s"
argument_list|,
name|show_date
argument_list|(
name|info
operator|->
name|timestamp
argument_list|,
literal|0
argument_list|,
literal|1
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%d"
argument_list|,
name|commit_reflog
operator|->
name|reflogs
operator|->
name|nr
operator|-
literal|2
operator|-
name|commit_reflog
operator|->
name|recno
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"}: "
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"Reflog: %s@{"
argument_list|,
name|commit_reflog
operator|->
name|reflogs
operator|->
name|ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit_reflog
operator|->
name|flag
condition|)
name|printf
argument_list|(
literal|"%s"
argument_list|,
name|show_rfc2822_date
argument_list|(
name|info
operator|->
name|timestamp
argument_list|,
name|info
operator|->
name|tz
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%d"
argument_list|,
name|commit_reflog
operator|->
name|reflogs
operator|->
name|nr
operator|-
literal|2
operator|-
name|commit_reflog
operator|->
name|recno
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"} (%s)\nReflog message: %s"
argument_list|,
name|info
operator|->
name|email
argument_list|,
name|info
operator|->
name|message
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

end_unit

