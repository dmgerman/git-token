begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * test-treap.c: code to exercise the svn importer's treap structure  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"vcs-svn/obj_pool.h"
end_include

begin_include
include|#
directive|include
file|"vcs-svn/trp.h"
end_include

begin_struct
DECL|struct|int_node
struct|struct
name|int_node
block|{
DECL|member|n
name|uintmax_t
name|n
decl_stmt|;
DECL|member|children
name|struct
name|trp_node
name|children
decl_stmt|;
block|}
struct|;
end_struct

begin_macro
name|obj_pool_gen
argument_list|(
argument|node
argument_list|,
argument|struct int_node
argument_list|,
literal|3
argument_list|)
end_macro

begin_function
DECL|function|node_cmp
specifier|static
name|int
name|node_cmp
parameter_list|(
name|struct
name|int_node
modifier|*
name|a
parameter_list|,
name|struct
name|int_node
modifier|*
name|b
parameter_list|)
block|{
return|return
operator|(
name|a
operator|->
name|n
operator|>
name|b
operator|->
name|n
operator|)
operator|-
operator|(
name|a
operator|->
name|n
operator|<
name|b
operator|->
name|n
operator|)
return|;
block|}
end_function

begin_macro
name|trp_gen
argument_list|(
argument|static
argument_list|,
argument|treap_
argument_list|,
argument|struct int_node
argument_list|,
argument|children
argument_list|,
argument|node
argument_list|,
argument|node_cmp
argument_list|)
end_macro

begin_function
DECL|function|strtonode
specifier|static
name|void
name|strtonode
parameter_list|(
name|struct
name|int_node
modifier|*
name|item
parameter_list|,
specifier|const
name|char
modifier|*
name|s
parameter_list|)
block|{
name|char
modifier|*
name|end
decl_stmt|;
name|item
operator|->
name|n
operator|=
name|strtoumax
argument_list|(
name|s
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|s
operator|==
literal|'\0'
operator|||
operator|(
operator|*
name|end
operator|!=
literal|'\n'
operator|&&
operator|*
name|end
operator|!=
literal|'\0'
operator|)
condition|)
name|die
argument_list|(
literal|"invalid integer: %s"
argument_list|,
name|s
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
name|argv
index|[]
parameter_list|)
block|{
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|trp_root
name|root
init|=
block|{
operator|~
literal|0U
block|}
decl_stmt|;
name|uint32_t
name|item
decl_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|1
condition|)
name|usage
argument_list|(
literal|"test-treap< ints"
argument_list|)
expr_stmt|;
while|while
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|sb
argument_list|,
name|stdin
argument_list|,
literal|'\n'
argument_list|)
operator|!=
name|EOF
condition|)
block|{
name|struct
name|int_node
modifier|*
name|node
init|=
name|node_pointer
argument_list|(
name|node_alloc
argument_list|(
literal|1
argument_list|)
argument_list|)
decl_stmt|;
name|item
operator|=
name|node_offset
argument_list|(
name|node
argument_list|)
expr_stmt|;
name|strtonode
argument_list|(
name|node
argument_list|,
name|sb
operator|.
name|buf
argument_list|)
expr_stmt|;
name|node
operator|=
name|treap_insert
argument_list|(
operator|&
name|root
argument_list|,
name|node_pointer
argument_list|(
name|item
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|node_offset
argument_list|(
name|node
argument_list|)
operator|!=
name|item
condition|)
name|die
argument_list|(
literal|"inserted %"
name|PRIu32
literal|" in place of %"
name|PRIu32
literal|""
argument_list|,
name|node_offset
argument_list|(
name|node
argument_list|)
argument_list|,
name|item
argument_list|)
expr_stmt|;
block|}
name|item
operator|=
name|node_offset
argument_list|(
name|treap_first
argument_list|(
operator|&
name|root
argument_list|)
argument_list|)
expr_stmt|;
while|while
condition|(
operator|~
name|item
condition|)
block|{
name|uint32_t
name|next
decl_stmt|;
name|struct
name|int_node
modifier|*
name|tmp
init|=
name|node_pointer
argument_list|(
name|node_alloc
argument_list|(
literal|1
argument_list|)
argument_list|)
decl_stmt|;
name|tmp
operator|->
name|n
operator|=
name|node_pointer
argument_list|(
name|item
argument_list|)
operator|->
name|n
expr_stmt|;
name|next
operator|=
name|node_offset
argument_list|(
name|treap_next
argument_list|(
operator|&
name|root
argument_list|,
name|node_pointer
argument_list|(
name|item
argument_list|)
argument_list|)
argument_list|)
expr_stmt|;
name|treap_remove
argument_list|(
operator|&
name|root
argument_list|,
name|node_pointer
argument_list|(
name|item
argument_list|)
argument_list|)
expr_stmt|;
name|item
operator|=
name|node_offset
argument_list|(
name|treap_nsearch
argument_list|(
operator|&
name|root
argument_list|,
name|tmp
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|item
operator|!=
name|next
operator|&&
operator|(
operator|!
operator|~
name|item
operator|||
name|node_pointer
argument_list|(
name|item
argument_list|)
operator|->
name|n
operator|!=
name|tmp
operator|->
name|n
operator|)
condition|)
name|die
argument_list|(
literal|"found %"
name|PRIuMAX
literal|" in place of %"
name|PRIuMAX
literal|""
argument_list|,
operator|~
name|item
condition|?
name|node_pointer
argument_list|(
name|item
argument_list|)
operator|->
name|n
else|:
operator|~
operator|(
name|uintmax_t
operator|)
literal|0
argument_list|,
operator|~
name|next
condition|?
name|node_pointer
argument_list|(
name|next
argument_list|)
operator|->
name|n
else|:
operator|~
operator|(
name|uintmax_t
operator|)
literal|0
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%"
name|PRIuMAX
literal|"\n"
argument_list|,
name|tmp
operator|->
name|n
argument_list|)
expr_stmt|;
block|}
name|node_reset
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

