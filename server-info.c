begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_comment
comment|/* refs */
end_comment

begin_decl_stmt
DECL|variable|info_ref_fp
specifier|static
name|FILE
modifier|*
name|info_ref_fp
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_info_ref
specifier|static
name|int
name|add_info_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|fprintf
argument_list|(
name|info_ref_fp
argument_list|,
literal|"%s	%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|o
operator|->
name|type
operator|==
name|tag_type
condition|)
block|{
name|o
operator|=
name|deref_tag
argument_list|(
name|o
argument_list|,
name|path
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|o
condition|)
name|fprintf
argument_list|(
name|info_ref_fp
argument_list|,
literal|"%s	%s^{}\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|o
operator|->
name|sha1
argument_list|)
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|update_info_refs
specifier|static
name|int
name|update_info_refs
parameter_list|(
name|int
name|force
parameter_list|)
block|{
name|char
modifier|*
name|path0
init|=
name|strdup
argument_list|(
name|git_path
argument_list|(
literal|"info/refs"
argument_list|)
argument_list|)
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|path0
argument_list|)
decl_stmt|;
name|char
modifier|*
name|path1
init|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|2
argument_list|)
decl_stmt|;
name|strcpy
argument_list|(
name|path1
argument_list|,
name|path0
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|path1
operator|+
name|len
argument_list|,
literal|"+"
argument_list|)
expr_stmt|;
name|safe_create_leading_directories
argument_list|(
name|path0
argument_list|)
expr_stmt|;
name|info_ref_fp
operator|=
name|fopen
argument_list|(
name|path1
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|info_ref_fp
condition|)
return|return
name|error
argument_list|(
literal|"unable to update %s"
argument_list|,
name|path0
argument_list|)
return|;
name|for_each_ref
argument_list|(
name|add_info_ref
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|info_ref_fp
argument_list|)
expr_stmt|;
name|rename
argument_list|(
name|path1
argument_list|,
name|path0
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|path0
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|path1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/* packs */
end_comment

begin_struct
DECL|struct|pack_info
specifier|static
struct|struct
name|pack_info
block|{
DECL|member|p
name|struct
name|packed_git
modifier|*
name|p
decl_stmt|;
DECL|member|old_num
name|int
name|old_num
decl_stmt|;
DECL|member|new_num
name|int
name|new_num
decl_stmt|;
DECL|member|nr_alloc
name|int
name|nr_alloc
decl_stmt|;
DECL|member|nr_heads
name|int
name|nr_heads
decl_stmt|;
DECL|member|head
name|unsigned
name|char
argument_list|(
operator|*
name|head
argument_list|)
decl|[20]
decl_stmt|;
DECL|variable|info
block|}
modifier|*
modifier|*
name|info
struct|;
end_struct

begin_decl_stmt
DECL|variable|num_pack
specifier|static
name|int
name|num_pack
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|objdir
specifier|static
specifier|const
name|char
modifier|*
name|objdir
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|objdirlen
specifier|static
name|int
name|objdirlen
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|parse_object_cheap
specifier|static
name|struct
name|object
modifier|*
name|parse_object_cheap
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|o
decl_stmt|;
if|if
condition|(
operator|(
name|o
operator|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
operator|)
operator|==
name|NULL
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|o
operator|->
name|type
operator|==
name|commit_type
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o
decl_stmt|;
name|free
argument_list|(
name|commit
operator|->
name|buffer
argument_list|)
expr_stmt|;
name|commit
operator|->
name|buffer
operator|=
name|NULL
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|o
operator|->
name|type
operator|==
name|tree_type
condition|)
block|{
name|struct
name|tree
modifier|*
name|tree
init|=
operator|(
expr|struct
name|tree
operator|*
operator|)
name|o
decl_stmt|;
name|struct
name|tree_entry_list
modifier|*
name|e
decl_stmt|,
modifier|*
name|n
decl_stmt|;
for|for
control|(
name|e
operator|=
name|tree
operator|->
name|entries
init|;
name|e
condition|;
name|e
operator|=
name|n
control|)
block|{
name|free
argument_list|(
name|e
operator|->
name|name
argument_list|)
expr_stmt|;
name|e
operator|->
name|name
operator|=
name|NULL
expr_stmt|;
name|n
operator|=
name|e
operator|->
name|next
expr_stmt|;
name|free
argument_list|(
name|e
argument_list|)
expr_stmt|;
block|}
name|tree
operator|->
name|entries
operator|=
name|NULL
expr_stmt|;
block|}
return|return
name|o
return|;
block|}
end_function

begin_function
DECL|function|find_pack_by_name
specifier|static
name|struct
name|pack_info
modifier|*
name|find_pack_by_name
parameter_list|(
specifier|const
name|char
modifier|*
name|name
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
name|num_pack
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|packed_git
modifier|*
name|p
init|=
name|info
index|[
name|i
index|]
operator|->
name|p
decl_stmt|;
comment|/* skip "/pack/" after ".git/objects" */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|p
operator|->
name|pack_name
operator|+
name|objdirlen
operator|+
literal|6
argument_list|,
name|name
argument_list|)
condition|)
return|return
name|info
index|[
name|i
index|]
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|find_pack_by_old_num
specifier|static
name|struct
name|pack_info
modifier|*
name|find_pack_by_old_num
parameter_list|(
name|int
name|old_num
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
name|num_pack
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|info
index|[
name|i
index|]
operator|->
name|old_num
operator|==
name|old_num
condition|)
return|return
name|info
index|[
name|i
index|]
return|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|add_head_def
specifier|static
name|int
name|add_head_def
parameter_list|(
name|struct
name|pack_info
modifier|*
name|this
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
name|this
operator|->
name|nr_alloc
operator|<=
name|this
operator|->
name|nr_heads
condition|)
block|{
name|this
operator|->
name|nr_alloc
operator|=
name|alloc_nr
argument_list|(
name|this
operator|->
name|nr_alloc
argument_list|)
expr_stmt|;
name|this
operator|->
name|head
operator|=
name|xrealloc
argument_list|(
name|this
operator|->
name|head
argument_list|,
name|this
operator|->
name|nr_alloc
operator|*
literal|20
argument_list|)
expr_stmt|;
block|}
name|memcpy
argument_list|(
name|this
operator|->
name|head
index|[
name|this
operator|->
name|nr_heads
operator|++
index|]
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/* Returns non-zero when we detect that the info in the  * old file is useless.  */
end_comment

begin_function
DECL|function|parse_pack_def
specifier|static
name|int
name|parse_pack_def
parameter_list|(
specifier|const
name|char
modifier|*
name|line
parameter_list|,
name|int
name|old_cnt
parameter_list|)
block|{
name|struct
name|pack_info
modifier|*
name|i
init|=
name|find_pack_by_name
argument_list|(
name|line
operator|+
literal|2
argument_list|)
decl_stmt|;
if|if
condition|(
name|i
condition|)
block|{
name|i
operator|->
name|old_num
operator|=
name|old_cnt
expr_stmt|;
return|return
literal|0
return|;
block|}
else|else
block|{
comment|/* The file describes a pack that is no longer here */
return|return
literal|1
return|;
block|}
block|}
end_function

begin_comment
comment|/* Returns non-zero when we detect that the info in the old file is useless.  */
end_comment

begin_function
DECL|function|parse_head_def
specifier|static
name|int
name|parse_head_def
parameter_list|(
name|char
modifier|*
name|line
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|long
name|num
decl_stmt|;
name|char
modifier|*
name|cp
decl_stmt|,
modifier|*
name|ep
decl_stmt|;
name|struct
name|pack_info
modifier|*
name|this
decl_stmt|;
name|struct
name|object
modifier|*
name|o
decl_stmt|;
name|cp
operator|=
name|line
operator|+
literal|2
expr_stmt|;
name|num
operator|=
name|strtoul
argument_list|(
name|cp
argument_list|,
operator|&
name|ep
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|ep
operator|==
name|cp
operator|||
operator|*
name|ep
operator|++
operator|!=
literal|' '
condition|)
return|return
name|error
argument_list|(
literal|"invalid input ix %s"
argument_list|,
name|line
argument_list|)
return|;
name|this
operator|=
name|find_pack_by_old_num
argument_list|(
name|num
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|this
condition|)
return|return
literal|1
return|;
comment|/* You know the drill. */
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|ep
argument_list|,
name|sha1
argument_list|)
operator|||
name|ep
index|[
literal|40
index|]
operator|!=
literal|' '
condition|)
return|return
name|error
argument_list|(
literal|"invalid input sha1 %s (%s)"
argument_list|,
name|line
argument_list|,
name|ep
argument_list|)
return|;
if|if
condition|(
operator|(
name|o
operator|=
name|parse_object_cheap
argument_list|(
name|sha1
argument_list|)
operator|)
operator|==
name|NULL
condition|)
return|return
name|error
argument_list|(
literal|"no such object: %s"
argument_list|,
name|line
argument_list|)
return|;
return|return
name|add_head_def
argument_list|(
name|this
argument_list|,
name|sha1
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* Returns non-zero when we detect that the info in the  * old file is useless.  */
end_comment

begin_function
DECL|function|read_pack_info_file
specifier|static
name|int
name|read_pack_info_file
parameter_list|(
specifier|const
name|char
modifier|*
name|infofile
parameter_list|)
block|{
name|FILE
modifier|*
name|fp
decl_stmt|;
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|int
name|old_cnt
init|=
literal|0
decl_stmt|;
name|fp
operator|=
name|fopen
argument_list|(
name|infofile
argument_list|,
literal|"r"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|fp
condition|)
return|return
literal|1
return|;
comment|/* nonexisting is not an error. */
while|while
condition|(
name|fgets
argument_list|(
name|line
argument_list|,
sizeof|sizeof
argument_list|(
name|line
argument_list|)
argument_list|,
name|fp
argument_list|)
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|line
argument_list|)
decl_stmt|;
if|if
condition|(
name|line
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|line
index|[
name|len
operator|-
literal|1
index|]
operator|=
literal|0
expr_stmt|;
switch|switch
condition|(
name|line
index|[
literal|0
index|]
condition|)
block|{
case|case
literal|'P'
case|:
comment|/* P name */
if|if
condition|(
name|parse_pack_def
argument_list|(
name|line
argument_list|,
name|old_cnt
operator|++
argument_list|)
condition|)
goto|goto
name|out_stale
goto|;
break|break;
case|case
literal|'D'
case|:
comment|/* we used to emit D but that was misguided. */
goto|goto
name|out_stale
goto|;
break|break;
case|case
literal|'T'
case|:
comment|/* T ix sha1 type */
if|if
condition|(
name|parse_head_def
argument_list|(
name|line
argument_list|)
condition|)
goto|goto
name|out_stale
goto|;
break|break;
default|default:
name|error
argument_list|(
literal|"unrecognized: %s"
argument_list|,
name|line
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
name|fclose
argument_list|(
name|fp
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
name|out_stale
label|:
name|fclose
argument_list|(
name|fp
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|compare_info
specifier|static
name|int
name|compare_info
parameter_list|(
specifier|const
name|void
modifier|*
name|a_
parameter_list|,
specifier|const
name|void
modifier|*
name|b_
parameter_list|)
block|{
name|struct
name|pack_info
modifier|*
specifier|const
modifier|*
name|a
init|=
name|a_
decl_stmt|;
name|struct
name|pack_info
modifier|*
specifier|const
modifier|*
name|b
init|=
name|b_
decl_stmt|;
if|if
condition|(
literal|0
operator|<=
operator|(
operator|*
name|a
operator|)
operator|->
name|old_num
operator|&&
literal|0
operator|<=
operator|(
operator|*
name|b
operator|)
operator|->
name|old_num
condition|)
comment|/* Keep the order in the original */
return|return
operator|(
operator|*
name|a
operator|)
operator|->
name|old_num
operator|-
operator|(
operator|*
name|b
operator|)
operator|->
name|old_num
return|;
elseif|else
if|if
condition|(
literal|0
operator|<=
operator|(
operator|*
name|a
operator|)
operator|->
name|old_num
condition|)
comment|/* Only A existed in the original so B is obviously newer */
return|return
operator|-
literal|1
return|;
elseif|else
if|if
condition|(
literal|0
operator|<=
operator|(
operator|*
name|b
operator|)
operator|->
name|old_num
condition|)
comment|/* The other way around. */
return|return
literal|1
return|;
comment|/* then it does not matter but at least keep the comparison stable */
return|return
operator|(
operator|*
name|a
operator|)
operator|->
name|p
operator|-
operator|(
operator|*
name|b
operator|)
operator|->
name|p
return|;
block|}
end_function

begin_function
DECL|function|init_pack_info
specifier|static
name|void
name|init_pack_info
parameter_list|(
specifier|const
name|char
modifier|*
name|infofile
parameter_list|,
name|int
name|force
parameter_list|)
block|{
name|struct
name|packed_git
modifier|*
name|p
decl_stmt|;
name|int
name|stale
decl_stmt|;
name|int
name|i
init|=
literal|0
decl_stmt|;
name|objdir
operator|=
name|get_object_directory
argument_list|()
expr_stmt|;
name|objdirlen
operator|=
name|strlen
argument_list|(
name|objdir
argument_list|)
expr_stmt|;
name|prepare_packed_git
argument_list|()
expr_stmt|;
for|for
control|(
name|p
operator|=
name|packed_git
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
comment|/* we ignore things on alternate path since they are 		 * not available to the pullers in general. 		 */
if|if
condition|(
name|strncmp
argument_list|(
name|p
operator|->
name|pack_name
argument_list|,
name|objdir
argument_list|,
name|objdirlen
argument_list|)
operator|||
name|strncmp
argument_list|(
name|p
operator|->
name|pack_name
operator|+
name|objdirlen
argument_list|,
literal|"/pack/"
argument_list|,
literal|6
argument_list|)
condition|)
continue|continue;
name|i
operator|++
expr_stmt|;
block|}
name|num_pack
operator|=
name|i
expr_stmt|;
name|info
operator|=
name|xcalloc
argument_list|(
name|num_pack
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|pack_info
operator|*
argument_list|)
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
operator|,
name|p
operator|=
name|packed_git
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
if|if
condition|(
name|strncmp
argument_list|(
name|p
operator|->
name|pack_name
argument_list|,
name|objdir
argument_list|,
name|objdirlen
argument_list|)
operator|||
name|p
operator|->
name|pack_name
index|[
name|objdirlen
index|]
operator|!=
literal|'/'
condition|)
continue|continue;
name|info
index|[
name|i
index|]
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|pack_info
argument_list|)
argument_list|)
expr_stmt|;
name|info
index|[
name|i
index|]
operator|->
name|p
operator|=
name|p
expr_stmt|;
name|info
index|[
name|i
index|]
operator|->
name|old_num
operator|=
operator|-
literal|1
expr_stmt|;
name|i
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|infofile
operator|&&
operator|!
name|force
condition|)
name|stale
operator|=
name|read_pack_info_file
argument_list|(
name|infofile
argument_list|)
expr_stmt|;
else|else
name|stale
operator|=
literal|1
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|num_pack
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|stale
condition|)
block|{
name|info
index|[
name|i
index|]
operator|->
name|old_num
operator|=
operator|-
literal|1
expr_stmt|;
name|info
index|[
name|i
index|]
operator|->
name|nr_heads
operator|=
literal|0
expr_stmt|;
block|}
block|}
comment|/* renumber them */
name|qsort
argument_list|(
name|info
argument_list|,
name|num_pack
argument_list|,
sizeof|sizeof
argument_list|(
name|info
index|[
literal|0
index|]
argument_list|)
argument_list|,
name|compare_info
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
name|num_pack
condition|;
name|i
operator|++
control|)
name|info
index|[
name|i
index|]
operator|->
name|new_num
operator|=
name|i
expr_stmt|;
block|}
end_function

begin_function
DECL|function|write_pack_info_file
specifier|static
name|void
name|write_pack_info_file
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|num_pack
condition|;
name|i
operator|++
control|)
name|fprintf
argument_list|(
name|fp
argument_list|,
literal|"P %s\n"
argument_list|,
name|info
index|[
name|i
index|]
operator|->
name|p
operator|->
name|pack_name
operator|+
name|objdirlen
operator|+
literal|6
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
name|num_pack
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|pack_info
modifier|*
name|this
init|=
name|info
index|[
name|i
index|]
decl_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|this
operator|->
name|nr_heads
condition|;
name|j
operator|++
control|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
name|lookup_object
argument_list|(
name|this
operator|->
name|head
index|[
name|j
index|]
argument_list|)
decl_stmt|;
name|fprintf
argument_list|(
name|fp
argument_list|,
literal|"T %1d %s %s\n"
argument_list|,
name|i
argument_list|,
name|sha1_to_hex
argument_list|(
name|this
operator|->
name|head
index|[
name|j
index|]
argument_list|)
argument_list|,
name|o
operator|->
name|type
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_define
DECL|macro|REFERENCED
define|#
directive|define
name|REFERENCED
value|01
end_define

begin_define
DECL|macro|EMITTED
define|#
directive|define
name|EMITTED
value|04
end_define

begin_function
DECL|function|show
specifier|static
name|void
name|show
parameter_list|(
name|struct
name|object
modifier|*
name|o
parameter_list|,
name|int
name|pack_ix
parameter_list|)
block|{
comment|/* 	 * We are interested in objects that are not referenced, 	 */
if|if
condition|(
name|o
operator|->
name|flags
operator|&
name|EMITTED
condition|)
return|return;
if|if
condition|(
operator|!
operator|(
name|o
operator|->
name|flags
operator|&
name|REFERENCED
operator|)
condition|)
name|add_head_def
argument_list|(
name|info
index|[
name|pack_ix
index|]
argument_list|,
name|o
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|o
operator|->
name|flags
operator||=
name|EMITTED
expr_stmt|;
block|}
end_function

begin_function
DECL|function|find_pack_info_one
specifier|static
name|void
name|find_pack_info_one
parameter_list|(
name|int
name|pack_ix
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|object
modifier|*
name|o
decl_stmt|;
name|int
name|i
decl_stmt|;
name|struct
name|packed_git
modifier|*
name|p
init|=
name|info
index|[
name|pack_ix
index|]
operator|->
name|p
decl_stmt|;
name|int
name|num
init|=
name|num_packed_objects
argument_list|(
name|p
argument_list|)
decl_stmt|;
comment|/* Scan objects, clear flags from all the edge ones and 	 * internal ones, possibly marked in the previous round. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|num
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|nth_packed_object_sha1
argument_list|(
name|p
argument_list|,
name|i
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt pack file %s?"
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|o
operator|=
name|parse_object_cheap
argument_list|(
name|sha1
argument_list|)
operator|)
operator|==
name|NULL
condition|)
name|die
argument_list|(
literal|"cannot parse %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|o
operator|->
name|refs
condition|)
block|{
name|struct
name|object_refs
modifier|*
name|refs
init|=
name|o
operator|->
name|refs
decl_stmt|;
name|int
name|j
decl_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|refs
operator|->
name|count
condition|;
name|j
operator|++
control|)
name|refs
operator|->
name|ref
index|[
name|j
index|]
operator|->
name|flags
operator|=
literal|0
expr_stmt|;
block|}
name|o
operator|->
name|flags
operator|=
literal|0
expr_stmt|;
block|}
comment|/* Mark all the referenced ones */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|num
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|nth_packed_object_sha1
argument_list|(
name|p
argument_list|,
name|i
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt pack file %s?"
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|o
operator|=
name|lookup_object
argument_list|(
name|sha1
argument_list|)
operator|)
operator|==
name|NULL
condition|)
name|die
argument_list|(
literal|"cannot find %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|o
operator|->
name|refs
condition|)
block|{
name|struct
name|object_refs
modifier|*
name|refs
init|=
name|o
operator|->
name|refs
decl_stmt|;
name|int
name|j
decl_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|refs
operator|->
name|count
condition|;
name|j
operator|++
control|)
name|refs
operator|->
name|ref
index|[
name|j
index|]
operator|->
name|flags
operator||=
name|REFERENCED
expr_stmt|;
block|}
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|num
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|nth_packed_object_sha1
argument_list|(
name|p
argument_list|,
name|i
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt pack file %s?"
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|o
operator|=
name|lookup_object
argument_list|(
name|sha1
argument_list|)
operator|)
operator|==
name|NULL
condition|)
name|die
argument_list|(
literal|"cannot find %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|show
argument_list|(
name|o
argument_list|,
name|pack_ix
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|find_pack_info
specifier|static
name|void
name|find_pack_info
parameter_list|(
name|void
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
name|num_pack
condition|;
name|i
operator|++
control|)
block|{
comment|/* The packed objects are cast in stone, and a head 		 * in a pack will stay as head, so is the set of missing 		 * objects.  If the repo has been reorganized and we 		 * are missing some packs available back then, we have 		 * already discarded the info read from the file, so 		 * we will find (old_num< 0) in that case. 		 */
if|if
condition|(
literal|0
operator|<=
name|info
index|[
name|i
index|]
operator|->
name|old_num
condition|)
continue|continue;
name|find_pack_info_one
argument_list|(
name|i
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|update_info_packs
specifier|static
name|int
name|update_info_packs
parameter_list|(
name|int
name|force
parameter_list|)
block|{
name|char
name|infofile
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|char
name|name
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|namelen
decl_stmt|;
name|FILE
modifier|*
name|fp
decl_stmt|;
name|namelen
operator|=
name|sprintf
argument_list|(
name|infofile
argument_list|,
literal|"%s/info/packs"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|name
argument_list|,
name|infofile
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|name
operator|+
name|namelen
argument_list|,
literal|"+"
argument_list|)
expr_stmt|;
name|init_pack_info
argument_list|(
name|infofile
argument_list|,
name|force
argument_list|)
expr_stmt|;
name|find_pack_info
argument_list|()
expr_stmt|;
name|safe_create_leading_directories
argument_list|(
name|name
argument_list|)
expr_stmt|;
name|fp
operator|=
name|fopen
argument_list|(
name|name
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|fp
condition|)
return|return
name|error
argument_list|(
literal|"cannot open %s"
argument_list|,
name|name
argument_list|)
return|;
name|write_pack_info_file
argument_list|(
name|fp
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|fp
argument_list|)
expr_stmt|;
name|rename
argument_list|(
name|name
argument_list|,
name|infofile
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/* public */
end_comment

begin_function
DECL|function|update_server_info
name|int
name|update_server_info
parameter_list|(
name|int
name|force
parameter_list|)
block|{
comment|/* We would add more dumb-server support files later, 	 * including index of available pack files and their 	 * intended audiences. 	 */
name|int
name|errs
init|=
literal|0
decl_stmt|;
name|errs
operator|=
name|errs
operator||
name|update_info_refs
argument_list|(
name|force
argument_list|)
expr_stmt|;
name|errs
operator|=
name|errs
operator||
name|update_info_packs
argument_list|(
name|force
argument_list|)
expr_stmt|;
comment|/* remove leftover rev-cache file if there is any */
name|unlink
argument_list|(
name|git_path
argument_list|(
literal|"info/rev-cache"
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|errs
return|;
block|}
end_function

end_unit

