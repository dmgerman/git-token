begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<sys/types.h>
end_include

begin_include
include|#
directive|include
file|<dirent.h>
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

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"pack.h"
end_include

begin_include
include|#
directive|include
file|"cache-tree.h"
end_include

begin_include
include|#
directive|include
file|"tree-walk.h"
end_include

begin_define
DECL|macro|REACHABLE
define|#
directive|define
name|REACHABLE
value|0x0001
end_define

begin_define
DECL|macro|SEEN
define|#
directive|define
name|SEEN
value|0x0002
end_define

begin_decl_stmt
DECL|variable|show_root
specifier|static
name|int
name|show_root
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_tags
specifier|static
name|int
name|show_tags
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_unreachable
specifier|static
name|int
name|show_unreachable
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_full
specifier|static
name|int
name|check_full
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_strict
specifier|static
name|int
name|check_strict
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|keep_cache_objects
specifier|static
name|int
name|keep_cache_objects
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|head_sha1
specifier|static
name|unsigned
name|char
name|head_sha1
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_ifdef
ifdef|#
directive|ifdef
name|NO_D_INO_IN_DIRENT
end_ifdef

begin_define
DECL|macro|SORT_DIRENT
define|#
directive|define
name|SORT_DIRENT
value|0
end_define

begin_define
DECL|macro|DIRENT_SORT_HINT
define|#
directive|define
name|DIRENT_SORT_HINT
parameter_list|(
name|de
parameter_list|)
value|0
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|SORT_DIRENT
define|#
directive|define
name|SORT_DIRENT
value|1
end_define

begin_define
DECL|macro|DIRENT_SORT_HINT
define|#
directive|define
name|DIRENT_SORT_HINT
parameter_list|(
name|de
parameter_list|)
value|((de)->d_ino)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|objreport
specifier|static
name|void
name|objreport
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
specifier|const
name|char
modifier|*
name|severity
parameter_list|,
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s in %s %s: "
argument_list|,
name|severity
argument_list|,
name|typename
argument_list|(
name|obj
operator|->
name|type
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|vfprintf
argument_list|(
name|stderr
argument_list|,
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
literal|"\n"
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|objerror
specifier|static
name|int
name|objerror
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|params
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|err
argument_list|)
expr_stmt|;
name|objreport
argument_list|(
name|obj
argument_list|,
literal|"error"
argument_list|,
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|objwarning
specifier|static
name|int
name|objwarning
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|params
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|err
argument_list|)
expr_stmt|;
name|objreport
argument_list|(
name|obj
argument_list|,
literal|"warning"
argument_list|,
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|check_connectivity
specifier|static
name|void
name|check_connectivity
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|max
decl_stmt|;
comment|/* Look up all the requirements, warn about missing objects.. */
name|max
operator|=
name|get_max_object_index
argument_list|()
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|max
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|struct
name|object_refs
modifier|*
name|refs
decl_stmt|;
name|struct
name|object
modifier|*
name|obj
init|=
name|get_indexed_object
argument_list|(
name|i
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
continue|continue;
if|if
condition|(
operator|!
name|obj
operator|->
name|parsed
condition|)
block|{
if|if
condition|(
name|has_sha1_file
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
condition|)
empty_stmt|;
comment|/* it is in pack */
else|else
name|printf
argument_list|(
literal|"missing %s %s\n"
argument_list|,
name|typename
argument_list|(
name|obj
operator|->
name|type
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|refs
operator|=
name|lookup_object_refs
argument_list|(
name|obj
argument_list|)
expr_stmt|;
if|if
condition|(
name|refs
condition|)
block|{
name|unsigned
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
block|{
name|struct
name|object
modifier|*
name|ref
init|=
name|refs
operator|->
name|ref
index|[
name|j
index|]
decl_stmt|;
if|if
condition|(
name|ref
operator|->
name|parsed
operator|||
operator|(
name|has_sha1_file
argument_list|(
name|ref
operator|->
name|sha1
argument_list|)
operator|)
condition|)
continue|continue;
name|printf
argument_list|(
literal|"broken link from %7s %s\n"
argument_list|,
name|typename
argument_list|(
name|obj
operator|->
name|type
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"              to %7s %s\n"
argument_list|,
name|typename
argument_list|(
name|ref
operator|->
name|type
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|show_unreachable
operator|&&
operator|!
operator|(
name|obj
operator|->
name|flags
operator|&
name|REACHABLE
operator|)
condition|)
block|{
name|printf
argument_list|(
literal|"unreachable %s %s\n"
argument_list|,
name|typename
argument_list|(
name|obj
operator|->
name|type
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|obj
operator|->
name|used
condition|)
block|{
name|printf
argument_list|(
literal|"dangling %s %s\n"
argument_list|,
name|typename
argument_list|(
name|obj
operator|->
name|type
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
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
name|desc
operator|.
name|buf
operator|=
name|item
operator|->
name|buffer
expr_stmt|;
name|desc
operator|.
name|size
operator|=
name|item
operator|->
name|size
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
name|has_zero_pad
operator||=
operator|*
operator|(
name|char
operator|*
operator|)
name|desc
operator|.
name|buf
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
name|check_strict
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
name|free
argument_list|(
name|item
operator|->
name|buffer
argument_list|)
expr_stmt|;
name|item
operator|->
name|buffer
operator|=
name|NULL
expr_stmt|;
name|retval
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|has_full_path
condition|)
block|{
name|objwarning
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
literal|"contains full pathnames"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|has_zero_pad
condition|)
block|{
name|objwarning
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
literal|"contains zero-padded file modes"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|has_bad_modes
condition|)
block|{
name|objwarning
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
literal|"contains bad file modes"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|has_dup_entries
condition|)
block|{
name|retval
operator|=
name|objerror
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
literal|"contains duplicate file entries"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|not_properly_sorted
condition|)
block|{
name|retval
operator|=
name|objerror
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
literal|"not properly sorted"
argument_list|)
expr_stmt|;
block|}
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
name|objerror
argument_list|(
operator|&
name|commit
operator|->
name|object
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
name|objerror
argument_list|(
operator|&
name|commit
operator|->
name|object
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
name|objerror
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
literal|"invalid 'parent' line format - bad sha1"
argument_list|)
return|;
name|buffer
operator|+=
literal|48
expr_stmt|;
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
name|objerror
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
literal|"invalid format - expected 'author' line"
argument_list|)
return|;
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
if|if
condition|(
operator|!
name|commit
operator|->
name|tree
condition|)
return|return
name|objerror
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
literal|"could not load commit's tree %s"
argument_list|,
name|tree_sha1
argument_list|)
return|;
if|if
condition|(
operator|!
name|commit
operator|->
name|parents
operator|&&
name|show_root
condition|)
name|printf
argument_list|(
literal|"root %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
operator|->
name|date
condition|)
name|printf
argument_list|(
literal|"bad commit date in %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit
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
block|{
return|return
name|objerror
argument_list|(
operator|&
name|tag
operator|->
name|object
argument_list|,
literal|"could not load tagged object"
argument_list|)
return|;
block|}
if|if
condition|(
operator|!
name|show_tags
condition|)
return|return
literal|0
return|;
name|printf
argument_list|(
literal|"tagged %s %s"
argument_list|,
name|typename
argument_list|(
name|tagged
operator|->
name|type
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|tagged
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|" (%s) in %s\n"
argument_list|,
name|tag
operator|->
name|tag
argument_list|,
name|sha1_to_hex
argument_list|(
name|tag
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

begin_function
DECL|function|fsck_sha1
specifier|static
name|int
name|fsck_sha1
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
init|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
return|return
name|error
argument_list|(
literal|"%s: object not found"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
return|;
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
name|SEEN
condition|)
return|return
literal|0
return|;
name|obj
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
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
argument_list|)
return|;
comment|/* By now, parse_object() would've returned NULL instead. */
return|return
name|objerror
argument_list|(
name|obj
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

begin_comment
comment|/*  * This is the sorting chunk size: make it reasonably  * big so that we can sort well..  */
end_comment

begin_define
DECL|macro|MAX_SHA1_ENTRIES
define|#
directive|define
name|MAX_SHA1_ENTRIES
value|(1024)
end_define

begin_struct
DECL|struct|sha1_entry
struct|struct
name|sha1_entry
block|{
DECL|member|ino
name|unsigned
name|long
name|ino
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
specifier|static
struct|struct
block|{
DECL|member|nr
name|unsigned
name|long
name|nr
decl_stmt|;
DECL|member|entry
name|struct
name|sha1_entry
modifier|*
name|entry
index|[
name|MAX_SHA1_ENTRIES
index|]
decl_stmt|;
DECL|variable|sha1_list
block|}
name|sha1_list
struct|;
end_struct

begin_function
DECL|function|ino_compare
specifier|static
name|int
name|ino_compare
parameter_list|(
specifier|const
name|void
modifier|*
name|_a
parameter_list|,
specifier|const
name|void
modifier|*
name|_b
parameter_list|)
block|{
specifier|const
name|struct
name|sha1_entry
modifier|*
name|a
init|=
name|_a
decl_stmt|,
modifier|*
name|b
init|=
name|_b
decl_stmt|;
name|unsigned
name|long
name|ino1
init|=
name|a
operator|->
name|ino
decl_stmt|,
name|ino2
init|=
name|b
operator|->
name|ino
decl_stmt|;
return|return
name|ino1
operator|<
name|ino2
condition|?
operator|-
literal|1
else|:
name|ino1
operator|>
name|ino2
condition|?
literal|1
else|:
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fsck_sha1_list
specifier|static
name|void
name|fsck_sha1_list
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|nr
init|=
name|sha1_list
operator|.
name|nr
decl_stmt|;
if|if
condition|(
name|SORT_DIRENT
condition|)
name|qsort
argument_list|(
name|sha1_list
operator|.
name|entry
argument_list|,
name|nr
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|sha1_entry
operator|*
argument_list|)
argument_list|,
name|ino_compare
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
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|sha1_entry
modifier|*
name|entry
init|=
name|sha1_list
operator|.
name|entry
index|[
name|i
index|]
decl_stmt|;
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|entry
operator|->
name|sha1
decl_stmt|;
name|sha1_list
operator|.
name|entry
index|[
name|i
index|]
operator|=
name|NULL
expr_stmt|;
name|fsck_sha1
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|entry
argument_list|)
expr_stmt|;
block|}
name|sha1_list
operator|.
name|nr
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_sha1_list
specifier|static
name|void
name|add_sha1_list
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|long
name|ino
parameter_list|)
block|{
name|struct
name|sha1_entry
modifier|*
name|entry
init|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|entry
argument_list|)
argument_list|)
decl_stmt|;
name|int
name|nr
decl_stmt|;
name|entry
operator|->
name|ino
operator|=
name|ino
expr_stmt|;
name|hashcpy
argument_list|(
name|entry
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|nr
operator|=
name|sha1_list
operator|.
name|nr
expr_stmt|;
if|if
condition|(
name|nr
operator|==
name|MAX_SHA1_ENTRIES
condition|)
block|{
name|fsck_sha1_list
argument_list|()
expr_stmt|;
name|nr
operator|=
literal|0
expr_stmt|;
block|}
name|sha1_list
operator|.
name|entry
index|[
name|nr
index|]
operator|=
name|entry
expr_stmt|;
name|sha1_list
operator|.
name|nr
operator|=
operator|++
name|nr
expr_stmt|;
block|}
end_function

begin_function
DECL|function|fsck_dir
specifier|static
name|void
name|fsck_dir
parameter_list|(
name|int
name|i
parameter_list|,
name|char
modifier|*
name|path
parameter_list|)
block|{
name|DIR
modifier|*
name|dir
init|=
name|opendir
argument_list|(
name|path
argument_list|)
decl_stmt|;
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
if|if
condition|(
operator|!
name|dir
condition|)
return|return;
while|while
condition|(
operator|(
name|de
operator|=
name|readdir
argument_list|(
name|dir
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|char
name|name
index|[
literal|100
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
decl_stmt|;
switch|switch
condition|(
name|len
condition|)
block|{
case|case
literal|2
case|:
if|if
condition|(
name|de
operator|->
name|d_name
index|[
literal|1
index|]
operator|!=
literal|'.'
condition|)
break|break;
case|case
literal|1
case|:
if|if
condition|(
name|de
operator|->
name|d_name
index|[
literal|0
index|]
operator|!=
literal|'.'
condition|)
break|break;
continue|continue;
case|case
literal|38
case|:
name|sprintf
argument_list|(
name|name
argument_list|,
literal|"%02x"
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|name
operator|+
literal|2
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
break|break;
name|add_sha1_list
argument_list|(
name|sha1
argument_list|,
name|DIRENT_SORT_HINT
argument_list|(
name|de
argument_list|)
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"bad sha1 file: %s/%s\n"
argument_list|,
name|path
argument_list|,
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
block|}
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|default_refs
specifier|static
name|int
name|default_refs
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|fsck_handle_ref
specifier|static
name|int
name|fsck_handle_ref
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
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
decl_stmt|;
name|obj
operator|=
name|lookup_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
block|{
if|if
condition|(
name|has_sha1_file
argument_list|(
name|sha1
argument_list|)
condition|)
block|{
name|default_refs
operator|++
expr_stmt|;
return|return
literal|0
return|;
comment|/* it is in a pack */
block|}
name|error
argument_list|(
literal|"%s: invalid sha1 pointer %s"
argument_list|,
name|refname
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
comment|/* We'll continue with the rest despite the error.. */
return|return
literal|0
return|;
block|}
name|default_refs
operator|++
expr_stmt|;
name|obj
operator|->
name|used
operator|=
literal|1
expr_stmt|;
name|mark_reachable
argument_list|(
name|obj
argument_list|,
name|REACHABLE
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|get_default_heads
specifier|static
name|void
name|get_default_heads
parameter_list|(
name|void
parameter_list|)
block|{
name|for_each_ref
argument_list|(
name|fsck_handle_ref
argument_list|)
expr_stmt|;
comment|/* 	 * Not having any default heads isn't really fatal, but 	 * it does mean that "--unreachable" no longer makes any 	 * sense (since in this case everything will obviously 	 * be unreachable by definition. 	 * 	 * Showing dangling objects is valid, though (as those 	 * dangling objects are likely lost heads). 	 * 	 * So we just print a warning about it, and clear the 	 * "show_unreachable" flag. 	 */
if|if
condition|(
operator|!
name|default_refs
condition|)
block|{
name|error
argument_list|(
literal|"No default references"
argument_list|)
expr_stmt|;
name|show_unreachable
operator|=
literal|0
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|fsck_object_dir
specifier|static
name|void
name|fsck_object_dir
parameter_list|(
specifier|const
name|char
modifier|*
name|path
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
literal|256
condition|;
name|i
operator|++
control|)
block|{
specifier|static
name|char
name|dir
index|[
literal|4096
index|]
decl_stmt|;
name|sprintf
argument_list|(
name|dir
argument_list|,
literal|"%s/%02x"
argument_list|,
name|path
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|fsck_dir
argument_list|(
name|i
argument_list|,
name|dir
argument_list|)
expr_stmt|;
block|}
name|fsck_sha1_list
argument_list|()
expr_stmt|;
block|}
end_function

begin_function
DECL|function|fsck_head_link
specifier|static
name|int
name|fsck_head_link
parameter_list|(
name|void
parameter_list|)
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
name|head_points_at
init|=
name|resolve_ref
argument_list|(
literal|"HEAD"
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|head_points_at
condition|)
return|return
name|error
argument_list|(
literal|"HEAD is not a symbolic ref"
argument_list|)
return|;
if|if
condition|(
name|strncmp
argument_list|(
name|head_points_at
argument_list|,
literal|"refs/heads/"
argument_list|,
literal|11
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"HEAD points to something strange (%s)"
argument_list|,
name|head_points_at
argument_list|)
return|;
if|if
condition|(
name|is_null_sha1
argument_list|(
name|sha1
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"HEAD: not a valid git pointer"
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fsck_cache_tree
specifier|static
name|int
name|fsck_cache_tree
parameter_list|(
name|struct
name|cache_tree
modifier|*
name|it
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|err
init|=
literal|0
decl_stmt|;
if|if
condition|(
literal|0
operator|<=
name|it
operator|->
name|entry_count
condition|)
block|{
name|struct
name|object
modifier|*
name|obj
init|=
name|parse_object
argument_list|(
name|it
operator|->
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
block|{
name|error
argument_list|(
literal|"%s: invalid sha1 pointer in cache-tree"
argument_list|,
name|sha1_to_hex
argument_list|(
name|it
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
name|mark_reachable
argument_list|(
name|obj
argument_list|,
name|REACHABLE
argument_list|)
expr_stmt|;
name|obj
operator|->
name|used
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|obj
operator|->
name|type
operator|!=
name|OBJ_TREE
condition|)
name|err
operator||=
name|objerror
argument_list|(
name|obj
argument_list|,
literal|"non-tree in cache-tree"
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
name|it
operator|->
name|subtree_nr
condition|;
name|i
operator|++
control|)
name|err
operator||=
name|fsck_cache_tree
argument_list|(
name|it
operator|->
name|down
index|[
name|i
index|]
operator|->
name|cache_tree
argument_list|)
expr_stmt|;
return|return
name|err
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
name|int
name|i
decl_stmt|,
name|heads
decl_stmt|;
name|track_object_refs
operator|=
literal|1
expr_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--unreachable"
argument_list|)
condition|)
block|{
name|show_unreachable
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--tags"
argument_list|)
condition|)
block|{
name|show_tags
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--root"
argument_list|)
condition|)
block|{
name|show_root
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--cache"
argument_list|)
condition|)
block|{
name|keep_cache_objects
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--full"
argument_list|)
condition|)
block|{
name|check_full
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--strict"
argument_list|)
condition|)
block|{
name|check_strict
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
name|usage
argument_list|(
literal|"git-fsck-objects [--tags] [--root] [[--unreachable] [--cache] [--full] [--strict]<head-sha1>*]"
argument_list|)
expr_stmt|;
block|}
name|fsck_head_link
argument_list|()
expr_stmt|;
name|fsck_object_dir
argument_list|(
name|get_object_directory
argument_list|()
argument_list|)
expr_stmt|;
if|if
condition|(
name|check_full
condition|)
block|{
name|struct
name|alternate_object_database
modifier|*
name|alt
decl_stmt|;
name|struct
name|packed_git
modifier|*
name|p
decl_stmt|;
name|prepare_alt_odb
argument_list|()
expr_stmt|;
for|for
control|(
name|alt
operator|=
name|alt_odb_list
init|;
name|alt
condition|;
name|alt
operator|=
name|alt
operator|->
name|next
control|)
block|{
name|char
name|namebuf
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|namelen
init|=
name|alt
operator|->
name|name
operator|-
name|alt
operator|->
name|base
decl_stmt|;
name|memcpy
argument_list|(
name|namebuf
argument_list|,
name|alt
operator|->
name|base
argument_list|,
name|namelen
argument_list|)
expr_stmt|;
name|namebuf
index|[
name|namelen
operator|-
literal|1
index|]
operator|=
literal|0
expr_stmt|;
name|fsck_object_dir
argument_list|(
name|namebuf
argument_list|)
expr_stmt|;
block|}
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
comment|/* verify gives error messages itself */
name|verify_pack
argument_list|(
name|p
argument_list|,
literal|0
argument_list|)
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
name|int
name|num
init|=
name|num_packed_objects
argument_list|(
name|p
argument_list|)
decl_stmt|;
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
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|nth_packed_object_sha1
argument_list|(
name|p
argument_list|,
name|i
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|fsck_sha1
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
block|}
block|}
block|}
name|heads
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
continue|continue;
if|if
condition|(
operator|!
name|get_sha1
argument_list|(
name|arg
argument_list|,
name|head_sha1
argument_list|)
condition|)
block|{
name|struct
name|object
modifier|*
name|obj
init|=
name|lookup_object
argument_list|(
name|head_sha1
argument_list|)
decl_stmt|;
comment|/* Error is printed by lookup_object(). */
if|if
condition|(
operator|!
name|obj
condition|)
continue|continue;
name|obj
operator|->
name|used
operator|=
literal|1
expr_stmt|;
name|mark_reachable
argument_list|(
name|obj
argument_list|,
name|REACHABLE
argument_list|)
expr_stmt|;
name|heads
operator|++
expr_stmt|;
continue|continue;
block|}
name|error
argument_list|(
literal|"invalid parameter: expected sha1, got '%s'"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * If we've not been given any explicit head information, do the 	 * default ones from .git/refs. We also consider the index file 	 * in this case (ie this implies --cache). 	 */
if|if
condition|(
operator|!
name|heads
condition|)
block|{
name|get_default_heads
argument_list|()
expr_stmt|;
name|keep_cache_objects
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|keep_cache_objects
condition|)
block|{
name|int
name|i
decl_stmt|;
name|read_cache
argument_list|()
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|active_nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|blob
modifier|*
name|blob
init|=
name|lookup_blob
argument_list|(
name|active_cache
index|[
name|i
index|]
operator|->
name|sha1
argument_list|)
decl_stmt|;
name|struct
name|object
modifier|*
name|obj
decl_stmt|;
if|if
condition|(
operator|!
name|blob
condition|)
continue|continue;
name|obj
operator|=
operator|&
name|blob
operator|->
name|object
expr_stmt|;
name|obj
operator|->
name|used
operator|=
literal|1
expr_stmt|;
name|mark_reachable
argument_list|(
name|obj
argument_list|,
name|REACHABLE
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|active_cache_tree
condition|)
name|fsck_cache_tree
argument_list|(
name|active_cache_tree
argument_list|)
expr_stmt|;
block|}
name|check_connectivity
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

