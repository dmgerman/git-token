begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"sha1-lookup.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_struct
DECL|struct|replace_object
specifier|static
struct|struct
name|replace_object
block|{
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|2
index|]
index|[
literal|20
index|]
decl_stmt|;
DECL|variable|replace_object
block|}
modifier|*
modifier|*
name|replace_object
struct|;
end_struct

begin_decl_stmt
DECL|variable|replace_object_alloc
DECL|variable|replace_object_nr
specifier|static
name|int
name|replace_object_alloc
decl_stmt|,
name|replace_object_nr
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|replace_sha1_access
specifier|static
specifier|const
name|unsigned
name|char
modifier|*
name|replace_sha1_access
parameter_list|(
name|size_t
name|index
parameter_list|,
name|void
modifier|*
name|table
parameter_list|)
block|{
name|struct
name|replace_object
modifier|*
modifier|*
name|replace
init|=
name|table
decl_stmt|;
return|return
name|replace
index|[
name|index
index|]
operator|->
name|sha1
index|[
literal|0
index|]
return|;
block|}
end_function

begin_function
DECL|function|replace_object_pos
specifier|static
name|int
name|replace_object_pos
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
return|return
name|sha1_pos
argument_list|(
name|sha1
argument_list|,
name|replace_object
argument_list|,
name|replace_object_nr
argument_list|,
name|replace_sha1_access
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|register_replace_object
specifier|static
name|int
name|register_replace_object
parameter_list|(
name|struct
name|replace_object
modifier|*
name|replace
parameter_list|,
name|int
name|ignore_dups
parameter_list|)
block|{
name|int
name|pos
init|=
name|replace_object_pos
argument_list|(
name|replace
operator|->
name|sha1
index|[
literal|0
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
literal|0
operator|<=
name|pos
condition|)
block|{
if|if
condition|(
name|ignore_dups
condition|)
name|free
argument_list|(
name|replace
argument_list|)
expr_stmt|;
else|else
block|{
name|free
argument_list|(
name|replace_object
index|[
name|pos
index|]
argument_list|)
expr_stmt|;
name|replace_object
index|[
name|pos
index|]
operator|=
name|replace
expr_stmt|;
block|}
return|return
literal|1
return|;
block|}
name|pos
operator|=
operator|-
name|pos
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|replace_object_alloc
operator|<=
operator|++
name|replace_object_nr
condition|)
block|{
name|replace_object_alloc
operator|=
name|alloc_nr
argument_list|(
name|replace_object_alloc
argument_list|)
expr_stmt|;
name|replace_object
operator|=
name|xrealloc
argument_list|(
name|replace_object
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|replace_object
argument_list|)
operator|*
name|replace_object_alloc
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|pos
operator|<
name|replace_object_nr
condition|)
name|memmove
argument_list|(
name|replace_object
operator|+
name|pos
operator|+
literal|1
argument_list|,
name|replace_object
operator|+
name|pos
argument_list|,
operator|(
name|replace_object_nr
operator|-
name|pos
operator|-
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|replace_object
argument_list|)
argument_list|)
expr_stmt|;
name|replace_object
index|[
name|pos
index|]
operator|=
name|replace
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|register_replace_ref
specifier|static
name|int
name|register_replace_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|flag
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
comment|/* Get sha1 from refname */
specifier|const
name|char
modifier|*
name|slash
init|=
name|strrchr
argument_list|(
name|refname
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|hash
init|=
name|slash
condition|?
name|slash
operator|+
literal|1
else|:
name|refname
decl_stmt|;
name|struct
name|replace_object
modifier|*
name|repl_obj
init|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|repl_obj
argument_list|)
argument_list|)
decl_stmt|;
if|if
condition|(
name|strlen
argument_list|(
name|hash
argument_list|)
operator|!=
literal|40
operator|||
name|get_sha1_hex
argument_list|(
name|hash
argument_list|,
name|repl_obj
operator|->
name|sha1
index|[
literal|0
index|]
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|repl_obj
argument_list|)
expr_stmt|;
name|warning
argument_list|(
literal|"bad replace ref name: %s"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
comment|/* Copy sha1 from the read ref */
name|hashcpy
argument_list|(
name|repl_obj
operator|->
name|sha1
index|[
literal|1
index|]
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
comment|/* Register new object */
if|if
condition|(
name|register_replace_object
argument_list|(
name|repl_obj
argument_list|,
literal|1
argument_list|)
condition|)
name|die
argument_list|(
literal|"duplicate replace ref: %s"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|prepare_replace_object
specifier|static
name|void
name|prepare_replace_object
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|int
name|replace_object_prepared
decl_stmt|;
if|if
condition|(
name|replace_object_prepared
condition|)
return|return;
name|for_each_replace_ref
argument_list|(
name|register_replace_ref
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|replace_object_prepared
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|replace_object_nr
condition|)
name|read_replace_refs
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_comment
comment|/* We allow "recursive" replacement. Only within reason, though */
end_comment

begin_define
DECL|macro|MAXREPLACEDEPTH
define|#
directive|define
name|MAXREPLACEDEPTH
value|5
end_define

begin_function
DECL|function|do_lookup_replace_object
specifier|const
name|unsigned
name|char
modifier|*
name|do_lookup_replace_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|pos
decl_stmt|,
name|depth
init|=
name|MAXREPLACEDEPTH
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|cur
init|=
name|sha1
decl_stmt|;
if|if
condition|(
operator|!
name|read_replace_refs
condition|)
return|return
name|sha1
return|;
name|prepare_replace_object
argument_list|()
expr_stmt|;
comment|/* Try to recursively replace the object */
do|do
block|{
if|if
condition|(
operator|--
name|depth
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"replace depth too high for object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|pos
operator|=
name|replace_object_pos
argument_list|(
name|cur
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|pos
condition|)
name|cur
operator|=
name|replace_object
index|[
name|pos
index|]
operator|->
name|sha1
index|[
literal|1
index|]
expr_stmt|;
block|}
do|while
condition|(
literal|0
operator|<=
name|pos
condition|)
do|;
return|return
name|cur
return|;
block|}
end_function

end_unit

