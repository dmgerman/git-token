begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"tree-walk.h"
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

begin_include
include|#
directive|include
file|"fsck.h"
end_include

begin_function
DECL|function|fsck_walk_tree
specifier|static
name|int
name|fsck_walk_tree
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
parameter_list|,
name|fsck_walk_func
name|walk
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|tree_desc
name|desc
decl_stmt|;
name|struct
name|name_entry
name|entry
decl_stmt|;
name|int
name|res
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|parse_tree
argument_list|(
name|tree
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|init_tree_desc
argument_list|(
operator|&
name|desc
argument_list|,
name|tree
operator|->
name|buffer
argument_list|,
name|tree
operator|->
name|size
argument_list|)
expr_stmt|;
while|while
condition|(
name|tree_entry
argument_list|(
operator|&
name|desc
argument_list|,
operator|&
name|entry
argument_list|)
condition|)
block|{
name|int
name|result
decl_stmt|;
if|if
condition|(
name|S_ISGITLINK
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
name|result
operator|=
name|walk
argument_list|(
operator|&
name|lookup_tree
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
operator|->
name|object
argument_list|,
name|OBJ_TREE
argument_list|,
name|data
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|S_ISREG
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
operator|||
name|S_ISLNK
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
name|result
operator|=
name|walk
argument_list|(
operator|&
name|lookup_blob
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
operator|->
name|object
argument_list|,
name|OBJ_BLOB
argument_list|,
name|data
argument_list|)
expr_stmt|;
else|else
block|{
name|result
operator|=
name|error
argument_list|(
literal|"in tree %s: entry %s has bad mode %.6o\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|,
name|entry
operator|.
name|path
argument_list|,
name|entry
operator|.
name|mode
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|result
operator|<
literal|0
condition|)
return|return
name|result
return|;
if|if
condition|(
operator|!
name|res
condition|)
name|res
operator|=
name|result
expr_stmt|;
block|}
return|return
name|res
return|;
block|}
end_function

begin_function
DECL|function|fsck_walk_commit
specifier|static
name|int
name|fsck_walk_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|fsck_walk_func
name|walk
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|parents
decl_stmt|;
name|int
name|res
decl_stmt|;
name|int
name|result
decl_stmt|;
if|if
condition|(
name|parse_commit
argument_list|(
name|commit
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|result
operator|=
name|walk
argument_list|(
operator|(
expr|struct
name|object
operator|*
operator|)
name|commit
operator|->
name|tree
argument_list|,
name|OBJ_TREE
argument_list|,
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|result
operator|<
literal|0
condition|)
return|return
name|result
return|;
name|res
operator|=
name|result
expr_stmt|;
name|parents
operator|=
name|commit
operator|->
name|parents
expr_stmt|;
while|while
condition|(
name|parents
condition|)
block|{
name|result
operator|=
name|walk
argument_list|(
operator|(
expr|struct
name|object
operator|*
operator|)
name|parents
operator|->
name|item
argument_list|,
name|OBJ_COMMIT
argument_list|,
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|result
operator|<
literal|0
condition|)
return|return
name|result
return|;
if|if
condition|(
operator|!
name|res
condition|)
name|res
operator|=
name|result
expr_stmt|;
name|parents
operator|=
name|parents
operator|->
name|next
expr_stmt|;
block|}
return|return
name|res
return|;
block|}
end_function

begin_function
DECL|function|fsck_walk_tag
specifier|static
name|int
name|fsck_walk_tag
parameter_list|(
name|struct
name|tag
modifier|*
name|tag
parameter_list|,
name|fsck_walk_func
name|walk
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
if|if
condition|(
name|parse_tag
argument_list|(
name|tag
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
return|return
name|walk
argument_list|(
name|tag
operator|->
name|tagged
argument_list|,
name|OBJ_ANY
argument_list|,
name|data
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|fsck_walk
name|int
name|fsck_walk
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|fsck_walk_func
name|walk
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
if|if
condition|(
operator|!
name|obj
condition|)
return|return
operator|-
literal|1
return|;
switch|switch
condition|(
name|obj
operator|->
name|type
condition|)
block|{
case|case
name|OBJ_BLOB
case|:
return|return
literal|0
return|;
case|case
name|OBJ_TREE
case|:
return|return
name|fsck_walk_tree
argument_list|(
operator|(
expr|struct
name|tree
operator|*
operator|)
name|obj
argument_list|,
name|walk
argument_list|,
name|data
argument_list|)
return|;
case|case
name|OBJ_COMMIT
case|:
return|return
name|fsck_walk_commit
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|obj
argument_list|,
name|walk
argument_list|,
name|data
argument_list|)
return|;
case|case
name|OBJ_TAG
case|:
return|return
name|fsck_walk_tag
argument_list|(
operator|(
expr|struct
name|tag
operator|*
operator|)
name|obj
argument_list|,
name|walk
argument_list|,
name|data
argument_list|)
return|;
default|default:
name|error
argument_list|(
literal|"Unknown object type for %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
block|}
end_function

begin_comment
comment|/*  * The entries in a tree are ordered in the _path_ order,  * which means that a directory entry is ordered by adding  * a slash to the end of it.  *  * So a directory called "a" is ordered _after_ a file  * called "a.c", because "a/" sorts after "a.c".  */
end_comment

begin_define
DECL|macro|TREE_UNORDERED
define|#
directive|define
name|TREE_UNORDERED
value|(-1)
end_define

begin_define
DECL|macro|TREE_HAS_DUPS
define|#
directive|define
name|TREE_HAS_DUPS
value|(-2)
end_define

begin_function
DECL|function|verify_ordered
specifier|static
name|int
name|verify_ordered
parameter_list|(
name|unsigned
name|mode1
parameter_list|,
specifier|const
name|char
modifier|*
name|name1
parameter_list|,
name|unsigned
name|mode2
parameter_list|,
specifier|const
name|char
modifier|*
name|name2
parameter_list|)
block|{
name|int
name|len1
init|=
name|strlen
argument_list|(
name|name1
argument_list|)
decl_stmt|;
name|int
name|len2
init|=
name|strlen
argument_list|(
name|name2
argument_list|)
decl_stmt|;
name|int
name|len
init|=
name|len1
operator|<
name|len2
condition|?
name|len1
else|:
name|len2
decl_stmt|;
name|unsigned
name|char
name|c1
decl_stmt|,
name|c2
decl_stmt|;
name|int
name|cmp
decl_stmt|;
name|cmp
operator|=
name|memcmp
argument_list|(
name|name1
argument_list|,
name|name2
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
return|return
name|TREE_UNORDERED
return|;
comment|/* 	 * Ok, the first<len> characters are the same. 	 * Now we need to order the next one, but turn 	 * a '\0' into a '/' for a directory entry. 	 */
name|c1
operator|=
name|name1
index|[
name|len
index|]
expr_stmt|;
name|c2
operator|=
name|name2
index|[
name|len
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|c1
operator|&&
operator|!
name|c2
condition|)
comment|/* 		 * git-write-tree used to write out a nonsense tree that has 		 * entries with the same name, one blob and one tree.  Make 		 * sure we do not have duplicate entries. 		 */
return|return
name|TREE_HAS_DUPS
return|;
if|if
condition|(
operator|!
name|c1
operator|&&
name|S_ISDIR
argument_list|(
name|mode1
argument_list|)
condition|)
name|c1
operator|=
literal|'/'
expr_stmt|;
if|if
condition|(
operator|!
name|c2
operator|&&
name|S_ISDIR
argument_list|(
name|mode2
argument_list|)
condition|)
name|c2
operator|=
literal|'/'
expr_stmt|;
return|return
name|c1
operator|<
name|c2
condition|?
literal|0
else|:
name|TREE_UNORDERED
return|;
block|}
end_function

begin_function
DECL|function|fsck_tree
specifier|static
name|int
name|fsck_tree
parameter_list|(
name|struct
name|tree
modifier|*
name|item
parameter_list|,
name|int
name|strict
parameter_list|,
name|fsck_error
name|error_func
parameter_list|)
block|{
name|int
name|retval
decl_stmt|;
name|int
name|has_full_path
init|=
literal|0
decl_stmt|;
name|int
name|has_empty_name
init|=
literal|0
decl_stmt|;
name|int
name|has_zero_pad
init|=
literal|0
decl_stmt|;
name|int
name|has_bad_modes
init|=
literal|0
decl_stmt|;
name|int
name|has_dup_entries
init|=
literal|0
decl_stmt|;
name|int
name|not_properly_sorted
init|=
literal|0
decl_stmt|;
name|struct
name|tree_desc
name|desc
decl_stmt|;
name|unsigned
name|o_mode
decl_stmt|;
specifier|const
name|char
modifier|*
name|o_name
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|o_sha1
decl_stmt|;
name|init_tree_desc
argument_list|(
operator|&
name|desc
argument_list|,
name|item
operator|->
name|buffer
argument_list|,
name|item
operator|->
name|size
argument_list|)
expr_stmt|;
name|o_mode
operator|=
literal|0
expr_stmt|;
name|o_name
operator|=
name|NULL
expr_stmt|;
name|o_sha1
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
operator|!
name|desc
operator|.
name|size
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"empty tree"
argument_list|)
return|;
while|while
condition|(
name|desc
operator|.
name|size
condition|)
block|{
name|unsigned
name|mode
decl_stmt|;
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|sha1
operator|=
name|tree_entry_extract
argument_list|(
operator|&
name|desc
argument_list|,
operator|&
name|name
argument_list|,
operator|&
name|mode
argument_list|)
expr_stmt|;
if|if
condition|(
name|strchr
argument_list|(
name|name
argument_list|,
literal|'/'
argument_list|)
condition|)
name|has_full_path
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|name
condition|)
name|has_empty_name
operator|=
literal|1
expr_stmt|;
name|has_zero_pad
operator||=
operator|*
operator|(
name|char
operator|*
operator|)
name|desc
operator|.
name|buffer
operator|==
literal|'0'
expr_stmt|;
name|update_tree_entry
argument_list|(
operator|&
name|desc
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|mode
condition|)
block|{
comment|/* 		 * Standard modes.. 		 */
case|case
name|S_IFREG
operator||
literal|0755
case|:
case|case
name|S_IFREG
operator||
literal|0644
case|:
case|case
name|S_IFLNK
case|:
case|case
name|S_IFDIR
case|:
case|case
name|S_IFGITLINK
case|:
break|break;
comment|/* 		 * This is nonstandard, but we had a few of these 		 * early on when we honored the full set of mode 		 * bits.. 		 */
case|case
name|S_IFREG
operator||
literal|0664
case|:
if|if
condition|(
operator|!
name|strict
condition|)
break|break;
default|default:
name|has_bad_modes
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|o_name
condition|)
block|{
switch|switch
condition|(
name|verify_ordered
argument_list|(
name|o_mode
argument_list|,
name|o_name
argument_list|,
name|mode
argument_list|,
name|name
argument_list|)
condition|)
block|{
case|case
name|TREE_UNORDERED
case|:
name|not_properly_sorted
operator|=
literal|1
expr_stmt|;
break|break;
case|case
name|TREE_HAS_DUPS
case|:
name|has_dup_entries
operator|=
literal|1
expr_stmt|;
break|break;
default|default:
break|break;
block|}
block|}
name|o_mode
operator|=
name|mode
expr_stmt|;
name|o_name
operator|=
name|name
expr_stmt|;
name|o_sha1
operator|=
name|sha1
expr_stmt|;
block|}
name|retval
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|has_full_path
condition|)
name|retval
operator|+=
name|error_func
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
name|FSCK_WARN
argument_list|,
literal|"contains full pathnames"
argument_list|)
expr_stmt|;
if|if
condition|(
name|has_empty_name
condition|)
name|retval
operator|+=
name|error_func
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
name|FSCK_WARN
argument_list|,
literal|"contains empty pathname"
argument_list|)
expr_stmt|;
if|if
condition|(
name|has_zero_pad
condition|)
name|retval
operator|+=
name|error_func
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
name|FSCK_WARN
argument_list|,
literal|"contains zero-padded file modes"
argument_list|)
expr_stmt|;
if|if
condition|(
name|has_bad_modes
condition|)
name|retval
operator|+=
name|error_func
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
name|FSCK_WARN
argument_list|,
literal|"contains bad file modes"
argument_list|)
expr_stmt|;
if|if
condition|(
name|has_dup_entries
condition|)
name|retval
operator|+=
name|error_func
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"contains duplicate file entries"
argument_list|)
expr_stmt|;
if|if
condition|(
name|not_properly_sorted
condition|)
name|retval
operator|+=
name|error_func
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"not properly sorted"
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|fsck_commit
specifier|static
name|int
name|fsck_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|fsck_error
name|error_func
parameter_list|)
block|{
name|char
modifier|*
name|buffer
init|=
name|commit
operator|->
name|buffer
decl_stmt|;
name|unsigned
name|char
name|tree_sha1
index|[
literal|20
index|]
decl_stmt|,
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|commit_graft
modifier|*
name|graft
decl_stmt|;
name|int
name|parents
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|commit
operator|->
name|date
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"invalid author/committer line"
argument_list|)
return|;
if|if
condition|(
name|memcmp
argument_list|(
name|buffer
argument_list|,
literal|"tree "
argument_list|,
literal|5
argument_list|)
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"invalid format - expected 'tree' line"
argument_list|)
return|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|buffer
operator|+
literal|5
argument_list|,
name|tree_sha1
argument_list|)
operator|||
name|buffer
index|[
literal|45
index|]
operator|!=
literal|'\n'
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"invalid 'tree' line format - bad sha1"
argument_list|)
return|;
name|buffer
operator|+=
literal|46
expr_stmt|;
while|while
condition|(
operator|!
name|memcmp
argument_list|(
name|buffer
argument_list|,
literal|"parent "
argument_list|,
literal|7
argument_list|)
condition|)
block|{
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|buffer
operator|+
literal|7
argument_list|,
name|sha1
argument_list|)
operator|||
name|buffer
index|[
literal|47
index|]
operator|!=
literal|'\n'
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"invalid 'parent' line format - bad sha1"
argument_list|)
return|;
name|buffer
operator|+=
literal|48
expr_stmt|;
name|parents
operator|++
expr_stmt|;
block|}
name|graft
operator|=
name|lookup_commit_graft
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|graft
condition|)
block|{
name|struct
name|commit_list
modifier|*
name|p
init|=
name|commit
operator|->
name|parents
decl_stmt|;
name|parents
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|p
condition|)
block|{
name|p
operator|=
name|p
operator|->
name|next
expr_stmt|;
name|parents
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|graft
operator|->
name|nr_parent
operator|==
operator|-
literal|1
operator|&&
operator|!
name|parents
condition|)
empty_stmt|;
comment|/* shallow commit */
elseif|else
if|if
condition|(
name|graft
operator|->
name|nr_parent
operator|!=
name|parents
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"graft objects missing"
argument_list|)
return|;
block|}
else|else
block|{
name|struct
name|commit_list
modifier|*
name|p
init|=
name|commit
operator|->
name|parents
decl_stmt|;
while|while
condition|(
name|p
operator|&&
name|parents
condition|)
block|{
name|p
operator|=
name|p
operator|->
name|next
expr_stmt|;
name|parents
operator|--
expr_stmt|;
block|}
if|if
condition|(
name|p
operator|||
name|parents
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"parent objects missing"
argument_list|)
return|;
block|}
if|if
condition|(
name|memcmp
argument_list|(
name|buffer
argument_list|,
literal|"author "
argument_list|,
literal|7
argument_list|)
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"invalid format - expected 'author' line"
argument_list|)
return|;
if|if
condition|(
operator|!
name|commit
operator|->
name|tree
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"could not load commit's tree %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|tree_sha1
argument_list|)
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fsck_tag
specifier|static
name|int
name|fsck_tag
parameter_list|(
name|struct
name|tag
modifier|*
name|tag
parameter_list|,
name|fsck_error
name|error_func
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|tagged
init|=
name|tag
operator|->
name|tagged
decl_stmt|;
if|if
condition|(
operator|!
name|tagged
condition|)
return|return
name|error_func
argument_list|(
operator|&
name|tag
operator|->
name|object
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"could not load tagged object"
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fsck_object
name|int
name|fsck_object
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|int
name|strict
parameter_list|,
name|fsck_error
name|error_func
parameter_list|)
block|{
if|if
condition|(
operator|!
name|obj
condition|)
return|return
name|error_func
argument_list|(
name|obj
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"no valid object to fsck"
argument_list|)
return|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_BLOB
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_TREE
condition|)
return|return
name|fsck_tree
argument_list|(
operator|(
expr|struct
name|tree
operator|*
operator|)
name|obj
argument_list|,
name|strict
argument_list|,
name|error_func
argument_list|)
return|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_COMMIT
condition|)
return|return
name|fsck_commit
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|obj
argument_list|,
name|error_func
argument_list|)
return|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_TAG
condition|)
return|return
name|fsck_tag
argument_list|(
operator|(
expr|struct
name|tag
operator|*
operator|)
name|obj
argument_list|,
name|error_func
argument_list|)
return|;
return|return
name|error_func
argument_list|(
name|obj
argument_list|,
name|FSCK_ERROR
argument_list|,
literal|"unknown type '%d' (internal fsck error)"
argument_list|,
name|obj
operator|->
name|type
argument_list|)
return|;
block|}
end_function

end_unit

