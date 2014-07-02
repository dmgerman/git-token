begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"hashmap.h"
end_include

begin_struct
DECL|struct|test_entry
struct|struct
name|test_entry
block|{
DECL|member|ent
name|struct
name|hashmap_entry
name|ent
decl_stmt|;
comment|/* key and value as two \0-terminated strings */
DECL|member|key
name|char
name|key
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|get_value
specifier|static
specifier|const
name|char
modifier|*
name|get_value
parameter_list|(
specifier|const
name|struct
name|test_entry
modifier|*
name|e
parameter_list|)
block|{
return|return
name|e
operator|->
name|key
operator|+
name|strlen
argument_list|(
name|e
operator|->
name|key
argument_list|)
operator|+
literal|1
return|;
block|}
end_function

begin_function
DECL|function|test_entry_cmp
specifier|static
name|int
name|test_entry_cmp
parameter_list|(
specifier|const
name|struct
name|test_entry
modifier|*
name|e1
parameter_list|,
specifier|const
name|struct
name|test_entry
modifier|*
name|e2
parameter_list|,
specifier|const
name|char
modifier|*
name|key
parameter_list|)
block|{
return|return
name|strcmp
argument_list|(
name|e1
operator|->
name|key
argument_list|,
name|key
condition|?
name|key
else|:
name|e2
operator|->
name|key
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|test_entry_cmp_icase
specifier|static
name|int
name|test_entry_cmp_icase
parameter_list|(
specifier|const
name|struct
name|test_entry
modifier|*
name|e1
parameter_list|,
specifier|const
name|struct
name|test_entry
modifier|*
name|e2
parameter_list|,
specifier|const
name|char
modifier|*
name|key
parameter_list|)
block|{
return|return
name|strcasecmp
argument_list|(
name|e1
operator|->
name|key
argument_list|,
name|key
condition|?
name|key
else|:
name|e2
operator|->
name|key
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|alloc_test_entry
specifier|static
name|struct
name|test_entry
modifier|*
name|alloc_test_entry
parameter_list|(
name|int
name|hash
parameter_list|,
name|char
modifier|*
name|key
parameter_list|,
name|int
name|klen
parameter_list|,
name|char
modifier|*
name|value
parameter_list|,
name|int
name|vlen
parameter_list|)
block|{
name|struct
name|test_entry
modifier|*
name|entry
init|=
name|malloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|test_entry
argument_list|)
operator|+
name|klen
operator|+
name|vlen
operator|+
literal|2
argument_list|)
decl_stmt|;
name|hashmap_entry_init
argument_list|(
name|entry
argument_list|,
name|hash
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|entry
operator|->
name|key
argument_list|,
name|key
argument_list|,
name|klen
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|entry
operator|->
name|key
operator|+
name|klen
operator|+
literal|1
argument_list|,
name|value
argument_list|,
name|vlen
operator|+
literal|1
argument_list|)
expr_stmt|;
return|return
name|entry
return|;
block|}
end_function

begin_define
DECL|macro|HASH_METHOD_FNV
define|#
directive|define
name|HASH_METHOD_FNV
value|0
end_define

begin_define
DECL|macro|HASH_METHOD_I
define|#
directive|define
name|HASH_METHOD_I
value|1
end_define

begin_define
DECL|macro|HASH_METHOD_IDIV10
define|#
directive|define
name|HASH_METHOD_IDIV10
value|2
end_define

begin_define
DECL|macro|HASH_METHOD_0
define|#
directive|define
name|HASH_METHOD_0
value|3
end_define

begin_define
DECL|macro|HASH_METHOD_X2
define|#
directive|define
name|HASH_METHOD_X2
value|4
end_define

begin_define
DECL|macro|TEST_SPARSE
define|#
directive|define
name|TEST_SPARSE
value|8
end_define

begin_define
DECL|macro|TEST_ADD
define|#
directive|define
name|TEST_ADD
value|16
end_define

begin_define
DECL|macro|TEST_SIZE
define|#
directive|define
name|TEST_SIZE
value|100000
end_define

begin_function
DECL|function|hash
specifier|static
name|unsigned
name|int
name|hash
parameter_list|(
name|unsigned
name|int
name|method
parameter_list|,
name|unsigned
name|int
name|i
parameter_list|,
specifier|const
name|char
modifier|*
name|key
parameter_list|)
block|{
name|unsigned
name|int
name|hash
decl_stmt|;
switch|switch
condition|(
name|method
operator|&
literal|3
condition|)
block|{
case|case
name|HASH_METHOD_FNV
case|:
name|hash
operator|=
name|strhash
argument_list|(
name|key
argument_list|)
expr_stmt|;
break|break;
case|case
name|HASH_METHOD_I
case|:
name|hash
operator|=
name|i
expr_stmt|;
break|break;
case|case
name|HASH_METHOD_IDIV10
case|:
name|hash
operator|=
name|i
operator|/
literal|10
expr_stmt|;
break|break;
case|case
name|HASH_METHOD_0
case|:
name|hash
operator|=
literal|0
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|method
operator|&
name|HASH_METHOD_X2
condition|)
name|hash
operator|=
literal|2
operator|*
name|hash
expr_stmt|;
return|return
name|hash
return|;
block|}
end_function

begin_comment
comment|/*  * Test performance of hashmap.[ch]  * Usage: time echo "perfhashmap method rounds" | test-hashmap  */
end_comment

begin_function
DECL|function|perf_hashmap
specifier|static
name|void
name|perf_hashmap
parameter_list|(
name|unsigned
name|int
name|method
parameter_list|,
name|unsigned
name|int
name|rounds
parameter_list|)
block|{
name|struct
name|hashmap
name|map
decl_stmt|;
name|char
name|buf
index|[
literal|16
index|]
decl_stmt|;
name|struct
name|test_entry
modifier|*
modifier|*
name|entries
decl_stmt|;
name|unsigned
name|int
modifier|*
name|hashes
decl_stmt|;
name|unsigned
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
name|entries
operator|=
name|malloc
argument_list|(
name|TEST_SIZE
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|test_entry
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|hashes
operator|=
name|malloc
argument_list|(
name|TEST_SIZE
operator|*
sizeof|sizeof
argument_list|(
name|int
argument_list|)
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
name|TEST_SIZE
condition|;
name|i
operator|++
control|)
block|{
name|snprintf
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
literal|"%i"
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|entries
index|[
name|i
index|]
operator|=
name|alloc_test_entry
argument_list|(
literal|0
argument_list|,
name|buf
argument_list|,
name|strlen
argument_list|(
name|buf
argument_list|)
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|hashes
index|[
name|i
index|]
operator|=
name|hash
argument_list|(
name|method
argument_list|,
name|i
argument_list|,
name|entries
index|[
name|i
index|]
operator|->
name|key
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|method
operator|&
name|TEST_ADD
condition|)
block|{
comment|/* test adding to the map */
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|rounds
condition|;
name|j
operator|++
control|)
block|{
name|hashmap_init
argument_list|(
operator|&
name|map
argument_list|,
operator|(
name|hashmap_cmp_fn
operator|)
name|test_entry_cmp
argument_list|,
literal|0
argument_list|)
expr_stmt|;
comment|/* add entries */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|TEST_SIZE
condition|;
name|i
operator|++
control|)
block|{
name|hashmap_entry_init
argument_list|(
name|entries
index|[
name|i
index|]
argument_list|,
name|hashes
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|hashmap_add
argument_list|(
operator|&
name|map
argument_list|,
name|entries
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
name|hashmap_free
argument_list|(
operator|&
name|map
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
comment|/* test map lookups */
name|hashmap_init
argument_list|(
operator|&
name|map
argument_list|,
operator|(
name|hashmap_cmp_fn
operator|)
name|test_entry_cmp
argument_list|,
literal|0
argument_list|)
expr_stmt|;
comment|/* fill the map (sparsely if specified) */
name|j
operator|=
operator|(
name|method
operator|&
name|TEST_SPARSE
operator|)
condition|?
name|TEST_SIZE
operator|/
literal|10
else|:
name|TEST_SIZE
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|j
condition|;
name|i
operator|++
control|)
block|{
name|hashmap_entry_init
argument_list|(
name|entries
index|[
name|i
index|]
argument_list|,
name|hashes
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|hashmap_add
argument_list|(
operator|&
name|map
argument_list|,
name|entries
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|rounds
condition|;
name|j
operator|++
control|)
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|TEST_SIZE
condition|;
name|i
operator|++
control|)
block|{
name|hashmap_get_from_hash
argument_list|(
operator|&
name|map
argument_list|,
name|hashes
index|[
name|i
index|]
argument_list|,
name|entries
index|[
name|i
index|]
operator|->
name|key
argument_list|)
expr_stmt|;
block|}
block|}
name|hashmap_free
argument_list|(
operator|&
name|map
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_define
DECL|macro|DELIM
define|#
directive|define
name|DELIM
value|" \t\r\n"
end_define

begin_comment
comment|/*  * Read stdin line by line and print result of commands to stdout:  *  * hash key -> strhash(key) memhash(key) strihash(key) memihash(key)  * put key value -> NULL / old value  * get key -> NULL / value  * remove key -> NULL / old value  * iterate -> key1 value1\nkey2 value2\n...  * size -> tablesize numentries  *  * perfhashmap method rounds -> test hashmap.[ch] performance  */
end_comment

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
name|char
name|line
index|[
literal|1024
index|]
decl_stmt|;
name|struct
name|hashmap
name|map
decl_stmt|;
name|int
name|icase
decl_stmt|;
comment|/* init hash map */
name|icase
operator|=
name|argc
operator|>
literal|1
operator|&&
operator|!
name|strcmp
argument_list|(
literal|"ignorecase"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|hashmap_init
argument_list|(
operator|&
name|map
argument_list|,
call|(
name|hashmap_cmp_fn
call|)
argument_list|(
name|icase
condition|?
name|test_entry_cmp_icase
else|:
name|test_entry_cmp
argument_list|)
argument_list|,
literal|0
argument_list|)
expr_stmt|;
comment|/* process commands from stdin */
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
name|stdin
argument_list|)
condition|)
block|{
name|char
modifier|*
name|cmd
decl_stmt|,
modifier|*
name|p1
init|=
name|NULL
decl_stmt|,
modifier|*
name|p2
init|=
name|NULL
decl_stmt|;
name|int
name|l1
init|=
literal|0
decl_stmt|,
name|l2
init|=
literal|0
decl_stmt|,
name|hash
init|=
literal|0
decl_stmt|;
name|struct
name|test_entry
modifier|*
name|entry
decl_stmt|;
comment|/* break line into command and up to two parameters */
name|cmd
operator|=
name|strtok
argument_list|(
name|line
argument_list|,
name|DELIM
argument_list|)
expr_stmt|;
comment|/* ignore empty lines */
if|if
condition|(
operator|!
name|cmd
operator|||
operator|*
name|cmd
operator|==
literal|'#'
condition|)
continue|continue;
name|p1
operator|=
name|strtok
argument_list|(
name|NULL
argument_list|,
name|DELIM
argument_list|)
expr_stmt|;
if|if
condition|(
name|p1
condition|)
block|{
name|l1
operator|=
name|strlen
argument_list|(
name|p1
argument_list|)
expr_stmt|;
name|hash
operator|=
name|icase
condition|?
name|strihash
argument_list|(
name|p1
argument_list|)
else|:
name|strhash
argument_list|(
name|p1
argument_list|)
expr_stmt|;
name|p2
operator|=
name|strtok
argument_list|(
name|NULL
argument_list|,
name|DELIM
argument_list|)
expr_stmt|;
if|if
condition|(
name|p2
condition|)
name|l2
operator|=
name|strlen
argument_list|(
name|p2
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"hash"
argument_list|,
name|cmd
argument_list|)
operator|&&
name|l1
condition|)
block|{
comment|/* print results of different hash functions */
name|printf
argument_list|(
literal|"%u %u %u %u\n"
argument_list|,
name|strhash
argument_list|(
name|p1
argument_list|)
argument_list|,
name|memhash
argument_list|(
name|p1
argument_list|,
name|l1
argument_list|)
argument_list|,
name|strihash
argument_list|(
name|p1
argument_list|)
argument_list|,
name|memihash
argument_list|(
name|p1
argument_list|,
name|l1
argument_list|)
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"add"
argument_list|,
name|cmd
argument_list|)
operator|&&
name|l1
operator|&&
name|l2
condition|)
block|{
comment|/* create entry with key = p1, value = p2 */
name|entry
operator|=
name|alloc_test_entry
argument_list|(
name|hash
argument_list|,
name|p1
argument_list|,
name|l1
argument_list|,
name|p2
argument_list|,
name|l2
argument_list|)
expr_stmt|;
comment|/* add to hashmap */
name|hashmap_add
argument_list|(
operator|&
name|map
argument_list|,
name|entry
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"put"
argument_list|,
name|cmd
argument_list|)
operator|&&
name|l1
operator|&&
name|l2
condition|)
block|{
comment|/* create entry with key = p1, value = p2 */
name|entry
operator|=
name|alloc_test_entry
argument_list|(
name|hash
argument_list|,
name|p1
argument_list|,
name|l1
argument_list|,
name|p2
argument_list|,
name|l2
argument_list|)
expr_stmt|;
comment|/* add / replace entry */
name|entry
operator|=
name|hashmap_put
argument_list|(
operator|&
name|map
argument_list|,
name|entry
argument_list|)
expr_stmt|;
comment|/* print and free replaced entry, if any */
name|puts
argument_list|(
name|entry
condition|?
name|get_value
argument_list|(
name|entry
argument_list|)
else|:
literal|"NULL"
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|entry
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"get"
argument_list|,
name|cmd
argument_list|)
operator|&&
name|l1
condition|)
block|{
comment|/* lookup entry in hashmap */
name|entry
operator|=
name|hashmap_get_from_hash
argument_list|(
operator|&
name|map
argument_list|,
name|hash
argument_list|,
name|p1
argument_list|)
expr_stmt|;
comment|/* print result */
if|if
condition|(
operator|!
name|entry
condition|)
name|puts
argument_list|(
literal|"NULL"
argument_list|)
expr_stmt|;
while|while
condition|(
name|entry
condition|)
block|{
name|puts
argument_list|(
name|get_value
argument_list|(
name|entry
argument_list|)
argument_list|)
expr_stmt|;
name|entry
operator|=
name|hashmap_get_next
argument_list|(
operator|&
name|map
argument_list|,
name|entry
argument_list|)
expr_stmt|;
block|}
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"remove"
argument_list|,
name|cmd
argument_list|)
operator|&&
name|l1
condition|)
block|{
comment|/* setup static key */
name|struct
name|hashmap_entry
name|key
decl_stmt|;
name|hashmap_entry_init
argument_list|(
operator|&
name|key
argument_list|,
name|hash
argument_list|)
expr_stmt|;
comment|/* remove entry from hashmap */
name|entry
operator|=
name|hashmap_remove
argument_list|(
operator|&
name|map
argument_list|,
operator|&
name|key
argument_list|,
name|p1
argument_list|)
expr_stmt|;
comment|/* print result and free entry*/
name|puts
argument_list|(
name|entry
condition|?
name|get_value
argument_list|(
name|entry
argument_list|)
else|:
literal|"NULL"
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|entry
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"iterate"
argument_list|,
name|cmd
argument_list|)
condition|)
block|{
name|struct
name|hashmap_iter
name|iter
decl_stmt|;
name|hashmap_iter_init
argument_list|(
operator|&
name|map
argument_list|,
operator|&
name|iter
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|entry
operator|=
name|hashmap_iter_next
argument_list|(
operator|&
name|iter
argument_list|)
operator|)
condition|)
name|printf
argument_list|(
literal|"%s %s\n"
argument_list|,
name|entry
operator|->
name|key
argument_list|,
name|get_value
argument_list|(
name|entry
argument_list|)
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"size"
argument_list|,
name|cmd
argument_list|)
condition|)
block|{
comment|/* print table sizes */
name|printf
argument_list|(
literal|"%u %u\n"
argument_list|,
name|map
operator|.
name|tablesize
argument_list|,
name|map
operator|.
name|size
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"perfhashmap"
argument_list|,
name|cmd
argument_list|)
operator|&&
name|l1
operator|&&
name|l2
condition|)
block|{
name|perf_hashmap
argument_list|(
name|atoi
argument_list|(
name|p1
argument_list|)
argument_list|,
name|atoi
argument_list|(
name|p2
argument_list|)
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"Unknown command %s\n"
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
block|}
block|}
name|hashmap_free
argument_list|(
operator|&
name|map
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

