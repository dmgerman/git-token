begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git replace"  *  * Copyright (c) 2008 Christian Couder<chriscool@tuxfamily.org>  *  * Based on builtin/tag.c by Kristian HÃ¸gsberg<krh@redhat.com>  * and Carlos Rica<jasampler@gmail.com> that was itself based on  * git-tag.sh and mktag.c by Linus Torvalds.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_decl_stmt
DECL|variable|git_replace_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|git_replace_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git replace [-f]<object><replacement>"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git replace -d<object>..."
argument_list|)
block|,
name|N_
argument_list|(
literal|"git replace [--format=<format>] [-l [<pattern>]]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_enum
DECL|enum|replace_format
enum|enum
name|replace_format
block|{
DECL|enumerator|REPLACE_FORMAT_SHORT
name|REPLACE_FORMAT_SHORT
block|,
DECL|enumerator|REPLACE_FORMAT_MEDIUM
name|REPLACE_FORMAT_MEDIUM
block|,
DECL|enumerator|REPLACE_FORMAT_LONG
name|REPLACE_FORMAT_LONG
block|}
enum|;
end_enum

begin_struct
DECL|struct|show_data
struct|struct
name|show_data
block|{
DECL|member|pattern
specifier|const
name|char
modifier|*
name|pattern
decl_stmt|;
DECL|member|format
name|enum
name|replace_format
name|format
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|show_reference
specifier|static
name|int
name|show_reference
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
name|struct
name|show_data
modifier|*
name|data
init|=
name|cb_data
decl_stmt|;
if|if
condition|(
operator|!
name|wildmatch
argument_list|(
name|data
operator|->
name|pattern
argument_list|,
name|refname
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
if|if
condition|(
name|data
operator|->
name|format
operator|==
name|REPLACE_FORMAT_SHORT
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|data
operator|->
name|format
operator|==
name|REPLACE_FORMAT_MEDIUM
condition|)
name|printf
argument_list|(
literal|"%s -> %s\n"
argument_list|,
name|refname
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
else|else
block|{
comment|/* data->format == REPLACE_FORMAT_LONG */
name|unsigned
name|char
name|object
index|[
literal|20
index|]
decl_stmt|;
name|enum
name|object_type
name|obj_type
decl_stmt|,
name|repl_type
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|refname
argument_list|,
name|object
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"Failed to resolve '%s' as a valid ref."
argument_list|,
name|refname
argument_list|)
return|;
name|obj_type
operator|=
name|sha1_object_info
argument_list|(
name|object
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|repl_type
operator|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s (%s) -> %s (%s)\n"
argument_list|,
name|refname
argument_list|,
name|typename
argument_list|(
name|obj_type
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|typename
argument_list|(
name|repl_type
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|list_replace_refs
specifier|static
name|int
name|list_replace_refs
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|)
block|{
name|struct
name|show_data
name|data
decl_stmt|;
if|if
condition|(
name|pattern
operator|==
name|NULL
condition|)
name|pattern
operator|=
literal|"*"
expr_stmt|;
name|data
operator|.
name|pattern
operator|=
name|pattern
expr_stmt|;
if|if
condition|(
name|format
operator|==
name|NULL
operator|||
operator|*
name|format
operator|==
literal|'\0'
operator|||
operator|!
name|strcmp
argument_list|(
name|format
argument_list|,
literal|"short"
argument_list|)
condition|)
name|data
operator|.
name|format
operator|=
name|REPLACE_FORMAT_SHORT
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|format
argument_list|,
literal|"medium"
argument_list|)
condition|)
name|data
operator|.
name|format
operator|=
name|REPLACE_FORMAT_MEDIUM
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|format
argument_list|,
literal|"long"
argument_list|)
condition|)
name|data
operator|.
name|format
operator|=
name|REPLACE_FORMAT_LONG
expr_stmt|;
else|else
name|die
argument_list|(
literal|"invalid replace format '%s'\n"
literal|"valid formats are 'short', 'medium' and 'long'\n"
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|for_each_replace_ref
argument_list|(
name|show_reference
argument_list|,
operator|(
name|void
operator|*
operator|)
operator|&
name|data
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_typedef
DECL|typedef|each_replace_name_fn
typedef|typedef
name|int
function_decl|(
modifier|*
name|each_replace_name_fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_typedef

begin_function
DECL|function|for_each_replace_name
specifier|static
name|int
name|for_each_replace_name
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|each_replace_name_fn
name|fn
parameter_list|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|p
decl_stmt|,
modifier|*
name|full_hex
decl_stmt|;
name|char
name|ref
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|had_error
init|=
literal|0
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
for|for
control|(
name|p
operator|=
name|argv
init|;
operator|*
name|p
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
name|get_sha1
argument_list|(
operator|*
name|p
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"Failed to resolve '%s' as a valid ref."
argument_list|,
operator|*
name|p
argument_list|)
expr_stmt|;
name|had_error
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|full_hex
operator|=
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|snprintf
argument_list|(
name|ref
argument_list|,
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
argument_list|,
literal|"refs/replace/%s"
argument_list|,
name|full_hex
argument_list|)
expr_stmt|;
comment|/* read_ref() may reuse the buffer */
name|full_hex
operator|=
name|ref
operator|+
name|strlen
argument_list|(
literal|"refs/replace/"
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_ref
argument_list|(
name|ref
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"replace ref '%s' not found."
argument_list|,
name|full_hex
argument_list|)
expr_stmt|;
name|had_error
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|fn
argument_list|(
name|full_hex
argument_list|,
name|ref
argument_list|,
name|sha1
argument_list|)
condition|)
name|had_error
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|had_error
return|;
block|}
end_function

begin_function
DECL|function|delete_replace_ref
specifier|static
name|int
name|delete_replace_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
name|delete_ref
argument_list|(
name|ref
argument_list|,
name|sha1
argument_list|,
literal|0
argument_list|)
condition|)
return|return
literal|1
return|;
name|printf
argument_list|(
literal|"Deleted replace ref '%s'\n"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|check_ref_valid
specifier|static
name|void
name|check_ref_valid
parameter_list|(
name|unsigned
name|char
name|object
index|[
literal|20
index|]
parameter_list|,
name|unsigned
name|char
name|prev
index|[
literal|20
index|]
parameter_list|,
name|char
modifier|*
name|ref
parameter_list|,
name|int
name|ref_size
parameter_list|,
name|int
name|force
parameter_list|)
block|{
if|if
condition|(
name|snprintf
argument_list|(
name|ref
argument_list|,
name|ref_size
argument_list|,
literal|"refs/replace/%s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|object
argument_list|)
argument_list|)
operator|>
name|ref_size
operator|-
literal|1
condition|)
name|die
argument_list|(
literal|"replace ref name too long: %.*s..."
argument_list|,
literal|50
argument_list|,
name|ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|check_refname_format
argument_list|(
name|ref
argument_list|,
literal|0
argument_list|)
condition|)
name|die
argument_list|(
literal|"'%s' is not a valid ref name."
argument_list|,
name|ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_ref
argument_list|(
name|ref
argument_list|,
name|prev
argument_list|)
condition|)
name|hashclr
argument_list|(
name|prev
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|force
condition|)
name|die
argument_list|(
literal|"replace ref '%s' already exists"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|replace_object_sha1
specifier|static
name|int
name|replace_object_sha1
parameter_list|(
specifier|const
name|char
modifier|*
name|object_ref
parameter_list|,
name|unsigned
name|char
name|object
index|[
literal|20
index|]
parameter_list|,
specifier|const
name|char
modifier|*
name|replace_ref
parameter_list|,
name|unsigned
name|char
name|repl
index|[
literal|20
index|]
parameter_list|,
name|int
name|force
parameter_list|)
block|{
name|unsigned
name|char
name|prev
index|[
literal|20
index|]
decl_stmt|;
name|enum
name|object_type
name|obj_type
decl_stmt|,
name|repl_type
decl_stmt|;
name|char
name|ref
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
name|obj_type
operator|=
name|sha1_object_info
argument_list|(
name|object
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|repl_type
operator|=
name|sha1_object_info
argument_list|(
name|repl
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|force
operator|&&
name|obj_type
operator|!=
name|repl_type
condition|)
name|die
argument_list|(
literal|"Objects must be of the same type.\n"
literal|"'%s' points to a replaced object of type '%s'\n"
literal|"while '%s' points to a replacement object of type '%s'."
argument_list|,
name|object_ref
argument_list|,
name|typename
argument_list|(
name|obj_type
argument_list|)
argument_list|,
name|replace_ref
argument_list|,
name|typename
argument_list|(
name|repl_type
argument_list|)
argument_list|)
expr_stmt|;
name|check_ref_valid
argument_list|(
name|object
argument_list|,
name|prev
argument_list|,
name|ref
argument_list|,
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
argument_list|,
name|force
argument_list|)
expr_stmt|;
name|lock
operator|=
name|lock_any_ref_for_update
argument_list|(
name|ref
argument_list|,
name|prev
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lock
condition|)
name|die
argument_list|(
literal|"%s: cannot lock the ref"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|write_ref_sha1
argument_list|(
name|lock
argument_list|,
name|repl
argument_list|,
name|NULL
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"%s: cannot update the ref"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|replace_object
specifier|static
name|int
name|replace_object
parameter_list|(
specifier|const
name|char
modifier|*
name|object_ref
parameter_list|,
specifier|const
name|char
modifier|*
name|replace_ref
parameter_list|,
name|int
name|force
parameter_list|)
block|{
name|unsigned
name|char
name|object
index|[
literal|20
index|]
decl_stmt|,
name|repl
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|object_ref
argument_list|,
name|object
argument_list|)
condition|)
name|die
argument_list|(
literal|"Failed to resolve '%s' as a valid ref."
argument_list|,
name|object_ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|replace_ref
argument_list|,
name|repl
argument_list|)
condition|)
name|die
argument_list|(
literal|"Failed to resolve '%s' as a valid ref."
argument_list|,
name|replace_ref
argument_list|)
expr_stmt|;
return|return
name|replace_object_sha1
argument_list|(
name|object_ref
argument_list|,
name|object
argument_list|,
name|replace_ref
argument_list|,
name|repl
argument_list|,
name|force
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Write the contents of the object named by "sha1" to the file "filename",  * pretty-printed for human editing based on its type.  */
end_comment

begin_function
DECL|function|export_object
specifier|static
name|void
name|export_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|argv
index|[]
init|=
block|{
literal|"--no-replace-objects"
block|,
literal|"cat-file"
block|,
literal|"-p"
block|,
name|NULL
block|,
name|NULL
block|}
decl_stmt|;
name|struct
name|child_process
name|cmd
init|=
block|{
name|argv
block|}
decl_stmt|;
name|int
name|fd
decl_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|filename
argument_list|,
name|O_WRONLY
operator||
name|O_CREAT
operator||
name|O_TRUNC
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to open %s for writing"
argument_list|,
name|filename
argument_list|)
expr_stmt|;
name|argv
index|[
literal|3
index|]
operator|=
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|cmd
operator|.
name|git_cmd
operator|=
literal|1
expr_stmt|;
name|cmd
operator|.
name|out
operator|=
name|fd
expr_stmt|;
if|if
condition|(
name|run_command
argument_list|(
operator|&
name|cmd
argument_list|)
condition|)
name|die
argument_list|(
literal|"cat-file reported failure"
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * Read a previously-exported (and possibly edited) object back from "filename",  * interpreting it as "type", and writing the result to the object database.  * The sha1 of the written object is returned via sha1.  */
end_comment

begin_function
DECL|function|import_object
specifier|static
name|void
name|import_object
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|enum
name|object_type
name|type
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|filename
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to open %s for reading"
argument_list|,
name|filename
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|==
name|OBJ_TREE
condition|)
block|{
specifier|const
name|char
modifier|*
name|argv
index|[]
init|=
block|{
literal|"mktree"
block|,
name|NULL
block|}
decl_stmt|;
name|struct
name|child_process
name|cmd
init|=
block|{
name|argv
block|}
decl_stmt|;
name|struct
name|strbuf
name|result
init|=
name|STRBUF_INIT
decl_stmt|;
name|cmd
operator|.
name|argv
operator|=
name|argv
expr_stmt|;
name|cmd
operator|.
name|git_cmd
operator|=
literal|1
expr_stmt|;
name|cmd
operator|.
name|in
operator|=
name|fd
expr_stmt|;
name|cmd
operator|.
name|out
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|cmd
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to spawn mktree"
argument_list|)
expr_stmt|;
if|if
condition|(
name|strbuf_read
argument_list|(
operator|&
name|result
argument_list|,
name|cmd
operator|.
name|out
argument_list|,
literal|41
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to read from mktree"
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|cmd
operator|.
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|finish_command
argument_list|(
operator|&
name|cmd
argument_list|)
condition|)
name|die
argument_list|(
literal|"mktree reported failure"
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|result
operator|.
name|buf
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"mktree did not return an object name"
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|result
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|int
name|flags
init|=
name|HASH_FORMAT_CHECK
operator||
name|HASH_WRITE_OBJECT
decl_stmt|;
if|if
condition|(
name|fstat
argument_list|(
name|fd
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to fstat %s"
argument_list|,
name|filename
argument_list|)
expr_stmt|;
if|if
condition|(
name|index_fd
argument_list|(
name|sha1
argument_list|,
name|fd
argument_list|,
operator|&
name|st
argument_list|,
name|type
argument_list|,
name|NULL
argument_list|,
name|flags
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to write object to database"
argument_list|)
expr_stmt|;
comment|/* index_fd close()s fd for us */
block|}
comment|/* 	 * No need to close(fd) here; both run-command and index-fd 	 * will have done it for us. 	 */
block|}
end_function

begin_function
DECL|function|edit_and_replace
specifier|static
name|int
name|edit_and_replace
parameter_list|(
specifier|const
name|char
modifier|*
name|object_ref
parameter_list|,
name|int
name|force
parameter_list|)
block|{
name|char
modifier|*
name|tmpfile
init|=
name|git_pathdup
argument_list|(
literal|"REPLACE_EDITOBJ"
argument_list|)
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|unsigned
name|char
name|old
index|[
literal|20
index|]
decl_stmt|,
name|new
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|object_ref
argument_list|,
name|old
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Not a valid object name: '%s'"
argument_list|,
name|object_ref
argument_list|)
expr_stmt|;
name|type
operator|=
name|sha1_object_info
argument_list|(
name|old
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to get object type for %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|old
argument_list|)
argument_list|)
expr_stmt|;
name|export_object
argument_list|(
name|old
argument_list|,
name|tmpfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|launch_editor
argument_list|(
name|tmpfile
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"editing object file failed"
argument_list|)
expr_stmt|;
name|import_object
argument_list|(
name|new
argument_list|,
name|type
argument_list|,
name|tmpfile
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tmpfile
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|old
argument_list|,
name|new
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"new object is the same as the old one: '%s'"
argument_list|,
name|sha1_to_hex
argument_list|(
name|old
argument_list|)
argument_list|)
return|;
return|return
name|replace_object_sha1
argument_list|(
name|object_ref
argument_list|,
name|old
argument_list|,
literal|"replacement"
argument_list|,
name|new
argument_list|,
name|force
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cmd_replace
name|int
name|cmd_replace
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|int
name|force
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|format
init|=
name|NULL
decl_stmt|;
enum|enum
block|{
name|MODE_UNSPECIFIED
init|=
literal|0
block|,
name|MODE_LIST
block|,
name|MODE_DELETE
block|,
name|MODE_EDIT
block|,
name|MODE_REPLACE
block|}
name|cmdmode
init|=
name|MODE_UNSPECIFIED
enum|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_CMDMODE
argument_list|(
literal|'l'
argument_list|,
literal|"list"
argument_list|,
operator|&
name|cmdmode
argument_list|,
name|N_
argument_list|(
literal|"list replace refs"
argument_list|)
argument_list|,
name|MODE_LIST
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|'d'
argument_list|,
literal|"delete"
argument_list|,
operator|&
name|cmdmode
argument_list|,
name|N_
argument_list|(
literal|"delete replace refs"
argument_list|)
argument_list|,
name|MODE_DELETE
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|'e'
argument_list|,
literal|"edit"
argument_list|,
operator|&
name|cmdmode
argument_list|,
name|N_
argument_list|(
literal|"edit existing object"
argument_list|)
argument_list|,
name|MODE_EDIT
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|'f'
argument_list|,
literal|"force"
argument_list|,
operator|&
name|force
argument_list|,
name|N_
argument_list|(
literal|"replace the ref if it exists"
argument_list|)
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"format"
argument_list|,
operator|&
name|format
argument_list|,
name|N_
argument_list|(
literal|"format"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"use this format"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|check_replace_refs
operator|=
literal|0
expr_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
name|options
argument_list|,
name|git_replace_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cmdmode
condition|)
name|cmdmode
operator|=
name|argc
condition|?
name|MODE_REPLACE
else|:
name|MODE_LIST
expr_stmt|;
if|if
condition|(
name|format
operator|&&
name|cmdmode
operator|!=
name|MODE_LIST
condition|)
name|usage_msg_opt
argument_list|(
literal|"--format cannot be used when not listing"
argument_list|,
name|git_replace_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|force
operator|&&
name|cmdmode
operator|!=
name|MODE_REPLACE
operator|&&
name|cmdmode
operator|!=
name|MODE_EDIT
condition|)
name|usage_msg_opt
argument_list|(
literal|"-f only makes sense when writing a replacement"
argument_list|,
name|git_replace_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|cmdmode
condition|)
block|{
case|case
name|MODE_DELETE
case|:
if|if
condition|(
name|argc
operator|<
literal|1
condition|)
name|usage_msg_opt
argument_list|(
literal|"-d needs at least one argument"
argument_list|,
name|git_replace_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
return|return
name|for_each_replace_name
argument_list|(
name|argv
argument_list|,
name|delete_replace_ref
argument_list|)
return|;
case|case
name|MODE_REPLACE
case|:
if|if
condition|(
name|argc
operator|!=
literal|2
condition|)
name|usage_msg_opt
argument_list|(
literal|"bad number of arguments"
argument_list|,
name|git_replace_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
return|return
name|replace_object
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|,
name|force
argument_list|)
return|;
case|case
name|MODE_EDIT
case|:
if|if
condition|(
name|argc
operator|!=
literal|1
condition|)
name|usage_msg_opt
argument_list|(
literal|"-e needs exactly one argument"
argument_list|,
name|git_replace_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
return|return
name|edit_and_replace
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
name|force
argument_list|)
return|;
case|case
name|MODE_LIST
case|:
if|if
condition|(
name|argc
operator|>
literal|1
condition|)
name|usage_msg_opt
argument_list|(
literal|"only one pattern can be given with -l"
argument_list|,
name|git_replace_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
return|return
name|list_replace_refs
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
name|format
argument_list|)
return|;
default|default:
name|die
argument_list|(
literal|"BUG: invalid cmdmode %d"
argument_list|,
operator|(
name|int
operator|)
name|cmdmode
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

