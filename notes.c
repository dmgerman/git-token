begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"notes.h"
end_include

begin_include
include|#
directive|include
file|"utf8.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_include
include|#
directive|include
file|"tree-walk.h"
end_include

begin_comment
comment|/*  * Use a non-balancing simple 16-tree structure with struct int_node as  * internal nodes, and struct leaf_node as leaf nodes. Each int_node has a  * 16-array of pointers to its children.  * The bottom 2 bits of each pointer is used to identify the pointer type  * - ptr& 3 == 0 - NULL pointer, assert(ptr == NULL)  * - ptr& 3 == 1 - pointer to next internal node - cast to struct int_node *  * - ptr& 3 == 2 - pointer to note entry - cast to struct leaf_node *  * - ptr& 3 == 3 - pointer to subtree entry - cast to struct leaf_node *  *  * The root node is a statically allocated struct int_node.  */
end_comment

begin_struct
DECL|struct|int_node
struct|struct
name|int_node
block|{
DECL|member|a
name|void
modifier|*
name|a
index|[
literal|16
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * Leaf nodes come in two variants, note entries and subtree entries,  * distinguished by the LSb of the leaf node pointer (see above).  * As a note entry, the key is the SHA1 of the referenced object, and the  * value is the SHA1 of the note object.  * As a subtree entry, the key is the prefix SHA1 (w/trailing NULs) of the  * referenced object, using the last byte of the key to store the length of  * the prefix. The value is the SHA1 of the tree object containing the notes  * subtree.  */
end_comment

begin_struct
DECL|struct|leaf_node
struct|struct
name|leaf_node
block|{
DECL|member|key_sha1
name|unsigned
name|char
name|key_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|val_sha1
name|unsigned
name|char
name|val_sha1
index|[
literal|20
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|PTR_TYPE_NULL
define|#
directive|define
name|PTR_TYPE_NULL
value|0
end_define

begin_define
DECL|macro|PTR_TYPE_INTERNAL
define|#
directive|define
name|PTR_TYPE_INTERNAL
value|1
end_define

begin_define
DECL|macro|PTR_TYPE_NOTE
define|#
directive|define
name|PTR_TYPE_NOTE
value|2
end_define

begin_define
DECL|macro|PTR_TYPE_SUBTREE
define|#
directive|define
name|PTR_TYPE_SUBTREE
value|3
end_define

begin_define
DECL|macro|GET_PTR_TYPE
define|#
directive|define
name|GET_PTR_TYPE
parameter_list|(
name|ptr
parameter_list|)
value|((uintptr_t) (ptr)& 3)
end_define

begin_define
DECL|macro|CLR_PTR_TYPE
define|#
directive|define
name|CLR_PTR_TYPE
parameter_list|(
name|ptr
parameter_list|)
value|((void *) ((uintptr_t) (ptr)& ~3))
end_define

begin_define
DECL|macro|SET_PTR_TYPE
define|#
directive|define
name|SET_PTR_TYPE
parameter_list|(
name|ptr
parameter_list|,
name|type
parameter_list|)
value|((void *) ((uintptr_t) (ptr) | (type)))
end_define

begin_define
DECL|macro|GET_NIBBLE
define|#
directive|define
name|GET_NIBBLE
parameter_list|(
name|n
parameter_list|,
name|sha1
parameter_list|)
value|(((sha1[n>> 1])>> ((~n& 0x01)<< 2))& 0x0f)
end_define

begin_define
DECL|macro|SUBTREE_SHA1_PREFIXCMP
define|#
directive|define
name|SUBTREE_SHA1_PREFIXCMP
parameter_list|(
name|key_sha1
parameter_list|,
name|subtree_sha1
parameter_list|)
define|\
value|(memcmp(key_sha1, subtree_sha1, subtree_sha1[19]))
end_define

begin_decl_stmt
DECL|variable|root_node
specifier|static
name|struct
name|int_node
name|root_node
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|initialized
specifier|static
name|int
name|initialized
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|static
name|void
name|load_subtree
parameter_list|(
name|struct
name|leaf_node
modifier|*
name|subtree
parameter_list|,
name|struct
name|int_node
modifier|*
name|node
parameter_list|,
name|unsigned
name|int
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Search the tree until the appropriate location for the given key is found:  * 1. Start at the root node, with n = 0  * 2. If a[0] at the current level is a matching subtree entry, unpack that  *    subtree entry and remove it; restart search at the current level.  * 3. Use the nth nibble of the key as an index into a:  *    - If a[n] is an int_node, recurse from #2 into that node and increment n  *    - If a matching subtree entry, unpack that subtree entry (and remove it);  *      restart search at the current level.  *    - Otherwise, we have found one of the following:  *      - a subtree entry which does not match the key  *      - a note entry which may or may not match the key  *      - an unused leaf node (NULL)  *      In any case, set *tree and *n, and return pointer to the tree location.  */
end_comment

begin_function
DECL|function|note_tree_search
specifier|static
name|void
modifier|*
modifier|*
name|note_tree_search
parameter_list|(
name|struct
name|int_node
modifier|*
modifier|*
name|tree
parameter_list|,
name|unsigned
name|char
modifier|*
name|n
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|key_sha1
parameter_list|)
block|{
name|struct
name|leaf_node
modifier|*
name|l
decl_stmt|;
name|unsigned
name|char
name|i
decl_stmt|;
name|void
modifier|*
name|p
init|=
operator|(
operator|*
name|tree
operator|)
operator|->
name|a
index|[
literal|0
index|]
decl_stmt|;
if|if
condition|(
name|GET_PTR_TYPE
argument_list|(
name|p
argument_list|)
operator|==
name|PTR_TYPE_SUBTREE
condition|)
block|{
name|l
operator|=
operator|(
expr|struct
name|leaf_node
operator|*
operator|)
name|CLR_PTR_TYPE
argument_list|(
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|SUBTREE_SHA1_PREFIXCMP
argument_list|(
name|key_sha1
argument_list|,
name|l
operator|->
name|key_sha1
argument_list|)
condition|)
block|{
comment|/* unpack tree and resume search */
operator|(
operator|*
name|tree
operator|)
operator|->
name|a
index|[
literal|0
index|]
operator|=
name|NULL
expr_stmt|;
name|load_subtree
argument_list|(
name|l
argument_list|,
operator|*
name|tree
argument_list|,
operator|*
name|n
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|l
argument_list|)
expr_stmt|;
return|return
name|note_tree_search
argument_list|(
name|tree
argument_list|,
name|n
argument_list|,
name|key_sha1
argument_list|)
return|;
block|}
block|}
name|i
operator|=
name|GET_NIBBLE
argument_list|(
operator|*
name|n
argument_list|,
name|key_sha1
argument_list|)
expr_stmt|;
name|p
operator|=
operator|(
operator|*
name|tree
operator|)
operator|->
name|a
index|[
name|i
index|]
expr_stmt|;
switch|switch
condition|(
name|GET_PTR_TYPE
argument_list|(
name|p
argument_list|)
condition|)
block|{
case|case
name|PTR_TYPE_INTERNAL
case|:
operator|*
name|tree
operator|=
name|CLR_PTR_TYPE
argument_list|(
name|p
argument_list|)
expr_stmt|;
operator|(
operator|*
name|n
operator|)
operator|++
expr_stmt|;
return|return
name|note_tree_search
argument_list|(
name|tree
argument_list|,
name|n
argument_list|,
name|key_sha1
argument_list|)
return|;
case|case
name|PTR_TYPE_SUBTREE
case|:
name|l
operator|=
operator|(
expr|struct
name|leaf_node
operator|*
operator|)
name|CLR_PTR_TYPE
argument_list|(
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|SUBTREE_SHA1_PREFIXCMP
argument_list|(
name|key_sha1
argument_list|,
name|l
operator|->
name|key_sha1
argument_list|)
condition|)
block|{
comment|/* unpack tree and resume search */
operator|(
operator|*
name|tree
operator|)
operator|->
name|a
index|[
name|i
index|]
operator|=
name|NULL
expr_stmt|;
name|load_subtree
argument_list|(
name|l
argument_list|,
operator|*
name|tree
argument_list|,
operator|*
name|n
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|l
argument_list|)
expr_stmt|;
return|return
name|note_tree_search
argument_list|(
name|tree
argument_list|,
name|n
argument_list|,
name|key_sha1
argument_list|)
return|;
block|}
comment|/* fall through */
default|default:
return|return
operator|&
operator|(
operator|(
operator|*
name|tree
operator|)
operator|->
name|a
index|[
name|i
index|]
operator|)
return|;
block|}
block|}
end_function

begin_comment
comment|/*  * To find a leaf_node:  * Search to the tree location appropriate for the given key:  * If a note entry with matching key, return the note entry, else return NULL.  */
end_comment

begin_function
DECL|function|note_tree_find
specifier|static
name|struct
name|leaf_node
modifier|*
name|note_tree_find
parameter_list|(
name|struct
name|int_node
modifier|*
name|tree
parameter_list|,
name|unsigned
name|char
name|n
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|key_sha1
parameter_list|)
block|{
name|void
modifier|*
modifier|*
name|p
init|=
name|note_tree_search
argument_list|(
operator|&
name|tree
argument_list|,
operator|&
name|n
argument_list|,
name|key_sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|GET_PTR_TYPE
argument_list|(
operator|*
name|p
argument_list|)
operator|==
name|PTR_TYPE_NOTE
condition|)
block|{
name|struct
name|leaf_node
modifier|*
name|l
init|=
operator|(
expr|struct
name|leaf_node
operator|*
operator|)
name|CLR_PTR_TYPE
argument_list|(
operator|*
name|p
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|key_sha1
argument_list|,
name|l
operator|->
name|key_sha1
argument_list|)
condition|)
return|return
name|l
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_comment
comment|/* Create a new blob object by concatenating the two given blob objects */
end_comment

begin_function
DECL|function|concatenate_notes
specifier|static
name|int
name|concatenate_notes
parameter_list|(
name|unsigned
name|char
modifier|*
name|cur_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|)
block|{
name|char
modifier|*
name|cur_msg
decl_stmt|,
modifier|*
name|new_msg
decl_stmt|,
modifier|*
name|buf
decl_stmt|;
name|unsigned
name|long
name|cur_len
decl_stmt|,
name|new_len
decl_stmt|,
name|buf_len
decl_stmt|;
name|enum
name|object_type
name|cur_type
decl_stmt|,
name|new_type
decl_stmt|;
name|int
name|ret
decl_stmt|;
comment|/* read in both note blob objects */
name|new_msg
operator|=
name|read_sha1_file
argument_list|(
name|new_sha1
argument_list|,
operator|&
name|new_type
argument_list|,
operator|&
name|new_len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|new_msg
operator|||
operator|!
name|new_len
operator|||
name|new_type
operator|!=
name|OBJ_BLOB
condition|)
block|{
name|free
argument_list|(
name|new_msg
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|cur_msg
operator|=
name|read_sha1_file
argument_list|(
name|cur_sha1
argument_list|,
operator|&
name|cur_type
argument_list|,
operator|&
name|cur_len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cur_msg
operator|||
operator|!
name|cur_len
operator|||
name|cur_type
operator|!=
name|OBJ_BLOB
condition|)
block|{
name|free
argument_list|(
name|cur_msg
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|new_msg
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|cur_sha1
argument_list|,
name|new_sha1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
comment|/* we will separate the notes by a newline anyway */
if|if
condition|(
name|cur_msg
index|[
name|cur_len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|cur_len
operator|--
expr_stmt|;
comment|/* concatenate cur_msg and new_msg into buf */
name|buf_len
operator|=
name|cur_len
operator|+
literal|1
operator|+
name|new_len
expr_stmt|;
name|buf
operator|=
operator|(
name|char
operator|*
operator|)
name|xmalloc
argument_list|(
name|buf_len
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|buf
argument_list|,
name|cur_msg
argument_list|,
name|cur_len
argument_list|)
expr_stmt|;
name|buf
index|[
name|cur_len
index|]
operator|=
literal|'\n'
expr_stmt|;
name|memcpy
argument_list|(
name|buf
operator|+
name|cur_len
operator|+
literal|1
argument_list|,
name|new_msg
argument_list|,
name|new_len
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|cur_msg
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|new_msg
argument_list|)
expr_stmt|;
comment|/* create a new blob object from buf */
name|ret
operator|=
name|write_sha1_file
argument_list|(
name|buf
argument_list|,
name|buf_len
argument_list|,
literal|"blob"
argument_list|,
name|cur_sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_comment
comment|/*  * To insert a leaf_node:  * Search to the tree location appropriate for the given leaf_node's key:  * - If location is unused (NULL), store the tweaked pointer directly there  * - If location holds a note entry that matches the note-to-be-inserted, then  *   concatenate the two notes.  * - If location holds a note entry that matches the subtree-to-be-inserted,  *   then unpack the subtree-to-be-inserted into the location.  * - If location holds a matching subtree entry, unpack the subtree at that  *   location, and restart the insert operation from that level.  * - Else, create a new int_node, holding both the node-at-location and the  *   node-to-be-inserted, and store the new int_node into the location.  */
end_comment

begin_function
DECL|function|note_tree_insert
specifier|static
name|void
name|note_tree_insert
parameter_list|(
name|struct
name|int_node
modifier|*
name|tree
parameter_list|,
name|unsigned
name|char
name|n
parameter_list|,
name|struct
name|leaf_node
modifier|*
name|entry
parameter_list|,
name|unsigned
name|char
name|type
parameter_list|)
block|{
name|struct
name|int_node
modifier|*
name|new_node
decl_stmt|;
name|struct
name|leaf_node
modifier|*
name|l
decl_stmt|;
name|void
modifier|*
modifier|*
name|p
init|=
name|note_tree_search
argument_list|(
operator|&
name|tree
argument_list|,
operator|&
name|n
argument_list|,
name|entry
operator|->
name|key_sha1
argument_list|)
decl_stmt|;
name|assert
argument_list|(
name|GET_PTR_TYPE
argument_list|(
name|entry
argument_list|)
operator|==
literal|0
argument_list|)
expr_stmt|;
comment|/* no type bits set */
name|l
operator|=
operator|(
expr|struct
name|leaf_node
operator|*
operator|)
name|CLR_PTR_TYPE
argument_list|(
operator|*
name|p
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|GET_PTR_TYPE
argument_list|(
operator|*
name|p
argument_list|)
condition|)
block|{
case|case
name|PTR_TYPE_NULL
case|:
name|assert
argument_list|(
operator|!
operator|*
name|p
argument_list|)
expr_stmt|;
operator|*
name|p
operator|=
name|SET_PTR_TYPE
argument_list|(
name|entry
argument_list|,
name|type
argument_list|)
expr_stmt|;
return|return;
case|case
name|PTR_TYPE_NOTE
case|:
switch|switch
condition|(
name|type
condition|)
block|{
case|case
name|PTR_TYPE_NOTE
case|:
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|l
operator|->
name|key_sha1
argument_list|,
name|entry
operator|->
name|key_sha1
argument_list|)
condition|)
block|{
comment|/* skip concatenation if l == entry */
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|l
operator|->
name|val_sha1
argument_list|,
name|entry
operator|->
name|val_sha1
argument_list|)
condition|)
return|return;
if|if
condition|(
name|concatenate_notes
argument_list|(
name|l
operator|->
name|val_sha1
argument_list|,
name|entry
operator|->
name|val_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"failed to concatenate note %s "
literal|"into note %s for object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|val_sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|l
operator|->
name|val_sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|l
operator|->
name|key_sha1
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|entry
argument_list|)
expr_stmt|;
return|return;
block|}
break|break;
case|case
name|PTR_TYPE_SUBTREE
case|:
if|if
condition|(
operator|!
name|SUBTREE_SHA1_PREFIXCMP
argument_list|(
name|l
operator|->
name|key_sha1
argument_list|,
name|entry
operator|->
name|key_sha1
argument_list|)
condition|)
block|{
comment|/* unpack 'entry' */
name|load_subtree
argument_list|(
name|entry
argument_list|,
name|tree
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|entry
argument_list|)
expr_stmt|;
return|return;
block|}
break|break;
block|}
break|break;
case|case
name|PTR_TYPE_SUBTREE
case|:
if|if
condition|(
operator|!
name|SUBTREE_SHA1_PREFIXCMP
argument_list|(
name|entry
operator|->
name|key_sha1
argument_list|,
name|l
operator|->
name|key_sha1
argument_list|)
condition|)
block|{
comment|/* unpack 'l' and restart insert */
operator|*
name|p
operator|=
name|NULL
expr_stmt|;
name|load_subtree
argument_list|(
name|l
argument_list|,
name|tree
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|l
argument_list|)
expr_stmt|;
name|note_tree_insert
argument_list|(
name|tree
argument_list|,
name|n
argument_list|,
name|entry
argument_list|,
name|type
argument_list|)
expr_stmt|;
return|return;
block|}
break|break;
block|}
comment|/* non-matching leaf_node */
name|assert
argument_list|(
name|GET_PTR_TYPE
argument_list|(
operator|*
name|p
argument_list|)
operator|==
name|PTR_TYPE_NOTE
operator|||
name|GET_PTR_TYPE
argument_list|(
operator|*
name|p
argument_list|)
operator|==
name|PTR_TYPE_SUBTREE
argument_list|)
expr_stmt|;
name|new_node
operator|=
operator|(
expr|struct
name|int_node
operator|*
operator|)
name|xcalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|int_node
argument_list|)
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|note_tree_insert
argument_list|(
name|new_node
argument_list|,
name|n
operator|+
literal|1
argument_list|,
name|l
argument_list|,
name|GET_PTR_TYPE
argument_list|(
operator|*
name|p
argument_list|)
argument_list|)
expr_stmt|;
operator|*
name|p
operator|=
name|SET_PTR_TYPE
argument_list|(
name|new_node
argument_list|,
name|PTR_TYPE_INTERNAL
argument_list|)
expr_stmt|;
name|note_tree_insert
argument_list|(
name|new_node
argument_list|,
name|n
operator|+
literal|1
argument_list|,
name|entry
argument_list|,
name|type
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Free the entire notes data contained in the given tree */
end_comment

begin_function
DECL|function|note_tree_free
specifier|static
name|void
name|note_tree_free
parameter_list|(
name|struct
name|int_node
modifier|*
name|tree
parameter_list|)
block|{
name|unsigned
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
literal|16
condition|;
name|i
operator|++
control|)
block|{
name|void
modifier|*
name|p
init|=
name|tree
operator|->
name|a
index|[
name|i
index|]
decl_stmt|;
switch|switch
condition|(
name|GET_PTR_TYPE
argument_list|(
name|p
argument_list|)
condition|)
block|{
case|case
name|PTR_TYPE_INTERNAL
case|:
name|note_tree_free
argument_list|(
name|CLR_PTR_TYPE
argument_list|(
name|p
argument_list|)
argument_list|)
expr_stmt|;
comment|/* fall through */
case|case
name|PTR_TYPE_NOTE
case|:
case|case
name|PTR_TYPE_SUBTREE
case|:
name|free
argument_list|(
name|CLR_PTR_TYPE
argument_list|(
name|p
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_comment
comment|/*  * Convert a partial SHA1 hex string to the corresponding partial SHA1 value.  * - hex      - Partial SHA1 segment in ASCII hex format  * - hex_len  - Length of above segment. Must be multiple of 2 between 0 and 40  * - sha1     - Partial SHA1 value is written here  * - sha1_len - Max #bytes to store in sha1, Must be>= hex_len / 2, and< 20  * Returns -1 on error (invalid arguments or invalid SHA1 (not in hex format)).  * Otherwise, returns number of bytes written to sha1 (i.e. hex_len / 2).  * Pads sha1 with NULs up to sha1_len (not included in returned length).  */
end_comment

begin_function
DECL|function|get_sha1_hex_segment
specifier|static
name|int
name|get_sha1_hex_segment
parameter_list|(
specifier|const
name|char
modifier|*
name|hex
parameter_list|,
name|unsigned
name|int
name|hex_len
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|int
name|sha1_len
parameter_list|)
block|{
name|unsigned
name|int
name|i
decl_stmt|,
name|len
init|=
name|hex_len
operator|>>
literal|1
decl_stmt|;
if|if
condition|(
name|hex_len
operator|%
literal|2
operator|!=
literal|0
operator|||
name|len
operator|>
name|sha1_len
condition|)
return|return
operator|-
literal|1
return|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|len
condition|;
name|i
operator|++
control|)
block|{
name|unsigned
name|int
name|val
init|=
operator|(
name|hexval
argument_list|(
name|hex
index|[
literal|0
index|]
argument_list|)
operator|<<
literal|4
operator|)
operator||
name|hexval
argument_list|(
name|hex
index|[
literal|1
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
name|val
operator|&
operator|~
literal|0xff
condition|)
return|return
operator|-
literal|1
return|;
operator|*
name|sha1
operator|++
operator|=
name|val
expr_stmt|;
name|hex
operator|+=
literal|2
expr_stmt|;
block|}
for|for
control|(
init|;
name|i
operator|<
name|sha1_len
condition|;
name|i
operator|++
control|)
operator|*
name|sha1
operator|++
operator|=
literal|0
expr_stmt|;
return|return
name|len
return|;
block|}
end_function

begin_function
DECL|function|load_subtree
specifier|static
name|void
name|load_subtree
parameter_list|(
name|struct
name|leaf_node
modifier|*
name|subtree
parameter_list|,
name|struct
name|int_node
modifier|*
name|node
parameter_list|,
name|unsigned
name|int
name|n
parameter_list|)
block|{
name|unsigned
name|char
name|object_sha1
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|int
name|prefix_len
decl_stmt|;
name|void
modifier|*
name|buf
decl_stmt|;
name|struct
name|tree_desc
name|desc
decl_stmt|;
name|struct
name|name_entry
name|entry
decl_stmt|;
name|buf
operator|=
name|fill_tree_descriptor
argument_list|(
operator|&
name|desc
argument_list|,
name|subtree
operator|->
name|val_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
name|die
argument_list|(
literal|"Could not read %s for notes-index"
argument_list|,
name|sha1_to_hex
argument_list|(
name|subtree
operator|->
name|val_sha1
argument_list|)
argument_list|)
expr_stmt|;
name|prefix_len
operator|=
name|subtree
operator|->
name|key_sha1
index|[
literal|19
index|]
expr_stmt|;
name|assert
argument_list|(
name|prefix_len
operator|*
literal|2
operator|>=
name|n
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|object_sha1
argument_list|,
name|subtree
operator|->
name|key_sha1
argument_list|,
name|prefix_len
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
name|len
init|=
name|get_sha1_hex_segment
argument_list|(
name|entry
operator|.
name|path
argument_list|,
name|strlen
argument_list|(
name|entry
operator|.
name|path
argument_list|)
argument_list|,
name|object_sha1
operator|+
name|prefix_len
argument_list|,
literal|20
operator|-
name|prefix_len
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|<
literal|0
condition|)
continue|continue;
comment|/* entry.path is not a SHA1 sum. Skip */
name|len
operator|+=
name|prefix_len
expr_stmt|;
comment|/* 		 * If object SHA1 is complete (len == 20), assume note object 		 * If object SHA1 is incomplete (len< 20), assume note subtree 		 */
if|if
condition|(
name|len
operator|<=
literal|20
condition|)
block|{
name|unsigned
name|char
name|type
init|=
name|PTR_TYPE_NOTE
decl_stmt|;
name|struct
name|leaf_node
modifier|*
name|l
init|=
operator|(
expr|struct
name|leaf_node
operator|*
operator|)
name|xcalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|leaf_node
argument_list|)
argument_list|,
literal|1
argument_list|)
decl_stmt|;
name|hashcpy
argument_list|(
name|l
operator|->
name|key_sha1
argument_list|,
name|object_sha1
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|l
operator|->
name|val_sha1
argument_list|,
name|entry
operator|.
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|<
literal|20
condition|)
block|{
if|if
condition|(
operator|!
name|S_ISDIR
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
continue|continue;
comment|/* entry cannot be subtree */
name|l
operator|->
name|key_sha1
index|[
literal|19
index|]
operator|=
operator|(
name|unsigned
name|char
operator|)
name|len
expr_stmt|;
name|type
operator|=
name|PTR_TYPE_SUBTREE
expr_stmt|;
block|}
name|note_tree_insert
argument_list|(
name|node
argument_list|,
name|n
argument_list|,
name|l
argument_list|,
name|type
argument_list|)
expr_stmt|;
block|}
block|}
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|initialize_notes
specifier|static
name|void
name|initialize_notes
parameter_list|(
specifier|const
name|char
modifier|*
name|notes_ref_name
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|,
name|object_sha1
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|mode
decl_stmt|;
name|struct
name|leaf_node
name|root_tree
decl_stmt|;
if|if
condition|(
operator|!
name|notes_ref_name
operator|||
name|read_ref
argument_list|(
name|notes_ref_name
argument_list|,
name|object_sha1
argument_list|)
operator|||
name|get_tree_entry
argument_list|(
name|object_sha1
argument_list|,
literal|""
argument_list|,
name|sha1
argument_list|,
operator|&
name|mode
argument_list|)
condition|)
return|return;
name|hashclr
argument_list|(
name|root_tree
operator|.
name|key_sha1
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|root_tree
operator|.
name|val_sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|load_subtree
argument_list|(
operator|&
name|root_tree
argument_list|,
operator|&
name|root_node
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|lookup_notes
specifier|static
name|unsigned
name|char
modifier|*
name|lookup_notes
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|object_sha1
parameter_list|)
block|{
name|struct
name|leaf_node
modifier|*
name|found
init|=
name|note_tree_find
argument_list|(
operator|&
name|root_node
argument_list|,
literal|0
argument_list|,
name|object_sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|found
condition|)
return|return
name|found
operator|->
name|val_sha1
return|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|free_notes
name|void
name|free_notes
parameter_list|(
name|void
parameter_list|)
block|{
name|note_tree_free
argument_list|(
operator|&
name|root_node
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|root_node
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|int_node
argument_list|)
argument_list|)
expr_stmt|;
name|initialized
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|format_note
name|void
name|format_note
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|object_sha1
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|output_encoding
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
specifier|static
specifier|const
name|char
name|utf8
index|[]
init|=
literal|"utf-8"
decl_stmt|;
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|char
modifier|*
name|msg
decl_stmt|,
modifier|*
name|msg_p
decl_stmt|;
name|unsigned
name|long
name|linelen
decl_stmt|,
name|msglen
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
if|if
condition|(
operator|!
name|initialized
condition|)
block|{
specifier|const
name|char
modifier|*
name|env
init|=
name|getenv
argument_list|(
name|GIT_NOTES_REF_ENVIRONMENT
argument_list|)
decl_stmt|;
if|if
condition|(
name|env
condition|)
name|notes_ref_name
operator|=
name|getenv
argument_list|(
name|GIT_NOTES_REF_ENVIRONMENT
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|notes_ref_name
condition|)
name|notes_ref_name
operator|=
name|GIT_NOTES_DEFAULT_REF
expr_stmt|;
name|initialize_notes
argument_list|(
name|notes_ref_name
argument_list|)
expr_stmt|;
name|initialized
operator|=
literal|1
expr_stmt|;
block|}
name|sha1
operator|=
name|lookup_notes
argument_list|(
name|object_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sha1
condition|)
return|return;
if|if
condition|(
operator|!
operator|(
name|msg
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
operator|&
name|type
argument_list|,
operator|&
name|msglen
argument_list|)
operator|)
operator|||
operator|!
name|msglen
operator|||
name|type
operator|!=
name|OBJ_BLOB
condition|)
block|{
name|free
argument_list|(
name|msg
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|output_encoding
operator|&&
operator|*
name|output_encoding
operator|&&
name|strcmp
argument_list|(
name|utf8
argument_list|,
name|output_encoding
argument_list|)
condition|)
block|{
name|char
modifier|*
name|reencoded
init|=
name|reencode_string
argument_list|(
name|msg
argument_list|,
name|output_encoding
argument_list|,
name|utf8
argument_list|)
decl_stmt|;
if|if
condition|(
name|reencoded
condition|)
block|{
name|free
argument_list|(
name|msg
argument_list|)
expr_stmt|;
name|msg
operator|=
name|reencoded
expr_stmt|;
name|msglen
operator|=
name|strlen
argument_list|(
name|msg
argument_list|)
expr_stmt|;
block|}
block|}
comment|/* we will end the annotation by a newline anyway */
if|if
condition|(
name|msglen
operator|&&
name|msg
index|[
name|msglen
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|msglen
operator|--
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|NOTES_SHOW_HEADER
condition|)
name|strbuf_addstr
argument_list|(
name|sb
argument_list|,
literal|"\nNotes:\n"
argument_list|)
expr_stmt|;
for|for
control|(
name|msg_p
operator|=
name|msg
init|;
name|msg_p
operator|<
name|msg
operator|+
name|msglen
condition|;
name|msg_p
operator|+=
name|linelen
operator|+
literal|1
control|)
block|{
name|linelen
operator|=
name|strchrnul
argument_list|(
name|msg_p
argument_list|,
literal|'\n'
argument_list|)
operator|-
name|msg_p
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|NOTES_INDENT
condition|)
name|strbuf_addstr
argument_list|(
name|sb
argument_list|,
literal|"    "
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
name|sb
argument_list|,
name|msg_p
argument_list|,
name|linelen
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
name|sb
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|msg
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

