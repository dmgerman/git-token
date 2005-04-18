begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<stdlib.h>
end_include

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

begin_function
DECL|function|process_list
specifier|static
name|struct
name|commit
modifier|*
name|process_list
parameter_list|(
name|struct
name|commit_list
modifier|*
modifier|*
name|list_p
parameter_list|,
name|int
name|this_mark
parameter_list|,
name|int
name|other_mark
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|parent
decl_stmt|,
modifier|*
name|temp
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|posn
init|=
operator|*
name|list_p
decl_stmt|;
operator|*
name|list_p
operator|=
name|NULL
expr_stmt|;
while|while
condition|(
name|posn
condition|)
block|{
name|parse_commit
argument_list|(
name|posn
operator|->
name|item
argument_list|)
expr_stmt|;
if|if
condition|(
name|posn
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&
name|this_mark
condition|)
block|{
comment|/* 			  printf("%d already seen %s %x\n", 			  this_mark 			  sha1_to_hex(posn->parent->sha1), 			  posn->parent->flags); 			*/
comment|/* do nothing; this indicates that this side 			 * split and reformed, and we only need to 			 * mark it once. 			 */
block|}
elseif|else
if|if
condition|(
name|posn
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&
name|other_mark
condition|)
block|{
return|return
name|posn
operator|->
name|item
return|;
block|}
else|else
block|{
comment|/* 			  printf("%d based on %s\n", 			  this_mark, 			  sha1_to_hex(posn->parent->sha1)); 			*/
name|posn
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator||=
name|this_mark
expr_stmt|;
name|parent
operator|=
name|posn
operator|->
name|item
operator|->
name|parents
expr_stmt|;
while|while
condition|(
name|parent
condition|)
block|{
name|temp
operator|=
name|malloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|commit_list
argument_list|)
argument_list|)
expr_stmt|;
name|temp
operator|->
name|next
operator|=
operator|*
name|list_p
expr_stmt|;
name|temp
operator|->
name|item
operator|=
name|parent
operator|->
name|item
expr_stmt|;
operator|*
name|list_p
operator|=
name|temp
expr_stmt|;
name|parent
operator|=
name|parent
operator|->
name|next
expr_stmt|;
block|}
block|}
name|posn
operator|=
name|posn
operator|->
name|next
expr_stmt|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|common_ancestor
name|struct
name|commit
modifier|*
name|common_ancestor
parameter_list|(
name|struct
name|commit
modifier|*
name|rev1
parameter_list|,
name|struct
name|commit
modifier|*
name|rev2
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|rev1list
init|=
name|malloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|commit_list
argument_list|)
argument_list|)
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|rev2list
init|=
name|malloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|commit_list
argument_list|)
argument_list|)
decl_stmt|;
name|rev1list
operator|->
name|item
operator|=
name|rev1
expr_stmt|;
name|rev1list
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|rev2list
operator|->
name|item
operator|=
name|rev2
expr_stmt|;
name|rev2list
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
while|while
condition|(
name|rev1list
operator|||
name|rev2list
condition|)
block|{
name|struct
name|commit
modifier|*
name|ret
decl_stmt|;
name|ret
operator|=
name|process_list
argument_list|(
operator|&
name|rev1list
argument_list|,
literal|0x1
argument_list|,
literal|0x2
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
block|{
comment|/* XXXX free lists */
return|return
name|ret
return|;
block|}
name|ret
operator|=
name|process_list
argument_list|(
operator|&
name|rev2list
argument_list|,
literal|0x2
argument_list|,
literal|0x1
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
block|{
comment|/* XXXX free lists */
return|return
name|ret
return|;
block|}
block|}
return|return
name|NULL
return|;
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
modifier|*
name|argv
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|rev1
decl_stmt|,
modifier|*
name|rev2
decl_stmt|,
modifier|*
name|ret
decl_stmt|;
name|unsigned
name|char
name|rev1key
index|[
literal|20
index|]
decl_stmt|,
name|rev2key
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|3
operator|||
name|get_sha1_hex
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|rev1key
argument_list|)
operator|||
name|get_sha1_hex
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|rev2key
argument_list|)
condition|)
block|{
name|usage
argument_list|(
literal|"merge-base<commit-id><commit-id>"
argument_list|)
expr_stmt|;
block|}
name|rev1
operator|=
name|lookup_commit
argument_list|(
name|rev1key
argument_list|)
expr_stmt|;
name|rev2
operator|=
name|lookup_commit
argument_list|(
name|rev2key
argument_list|)
expr_stmt|;
name|ret
operator|=
name|common_ancestor
argument_list|(
name|rev1
argument_list|,
name|rev2
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
return|return
literal|1
return|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|ret
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

