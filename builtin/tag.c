begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git tag"  *  * Copyright (c) 2007 Kristian HÃ¸gsberg<krh@redhat.com>,  *                    Carlos Rica<jasampler@gmail.com>  * Based on git-tag.sh and mktag.c by Linus Torvalds.  */
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
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
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
file|"gpg-interface.h"
end_include

begin_include
include|#
directive|include
file|"sha1-array.h"
end_include

begin_include
include|#
directive|include
file|"column.h"
end_include

begin_include
include|#
directive|include
file|"ref-filter.h"
end_include

begin_decl_stmt
DECL|variable|git_tag_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|git_tag_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git tag [-a | -s | -u<key-id>] [-f] [-m<msg> | -F<file>]<tagname> [<head>]"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git tag -d<tagname>..."
argument_list|)
block|,
name|N_
argument_list|(
literal|"git tag -l [-n[<num>]] [--contains<commit>] [--points-at<object>]"
literal|"\n\t\t[<pattern>...]"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git tag -v<tagname>..."
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|colopts
specifier|static
name|unsigned
name|int
name|colopts
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|list_tags
specifier|static
name|int
name|list_tags
parameter_list|(
name|struct
name|ref_filter
modifier|*
name|filter
parameter_list|,
name|struct
name|ref_sorting
modifier|*
name|sorting
parameter_list|)
block|{
name|struct
name|ref_array
name|array
decl_stmt|;
name|char
modifier|*
name|format
decl_stmt|,
modifier|*
name|to_free
init|=
name|NULL
decl_stmt|;
name|int
name|i
decl_stmt|;
name|memset
argument_list|(
operator|&
name|array
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|array
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|filter
operator|->
name|lines
operator|==
operator|-
literal|1
condition|)
name|filter
operator|->
name|lines
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|filter
operator|->
name|lines
condition|)
block|{
name|to_free
operator|=
name|xstrfmt
argument_list|(
literal|"%s %%(contents:lines=%d)"
argument_list|,
literal|"%(align:15)%(refname:short)%(end)"
argument_list|,
name|filter
operator|->
name|lines
argument_list|)
expr_stmt|;
name|format
operator|=
name|to_free
expr_stmt|;
block|}
else|else
name|format
operator|=
literal|"%(refname:short)"
expr_stmt|;
name|verify_ref_format
argument_list|(
name|format
argument_list|)
expr_stmt|;
name|filter_refs
argument_list|(
operator|&
name|array
argument_list|,
name|filter
argument_list|,
name|FILTER_REFS_TAGS
argument_list|)
expr_stmt|;
name|ref_array_sort
argument_list|(
name|sorting
argument_list|,
operator|&
name|array
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
name|array
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|show_ref_array_item
argument_list|(
name|array
operator|.
name|items
index|[
name|i
index|]
argument_list|,
name|format
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ref_array_clear
argument_list|(
operator|&
name|array
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|to_free
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_typedef
DECL|typedef|each_tag_name_fn
typedef|typedef
name|int
function_decl|(
modifier|*
name|each_tag_name_fn
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
DECL|function|for_each_tag_name
specifier|static
name|int
name|for_each_tag_name
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|each_tag_name_fn
name|fn
parameter_list|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|p
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
name|snprintf
argument_list|(
name|ref
argument_list|,
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
argument_list|,
literal|"refs/tags/%s"
argument_list|,
operator|*
name|p
argument_list|)
operator|>=
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
condition|)
block|{
name|error
argument_list|(
name|_
argument_list|(
literal|"tag name too long: %.*s..."
argument_list|)
argument_list|,
literal|50
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
name|_
argument_list|(
literal|"tag '%s' not found."
argument_list|)
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
if|if
condition|(
name|fn
argument_list|(
operator|*
name|p
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
DECL|function|delete_tag
specifier|static
name|int
name|delete_tag
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
name|_
argument_list|(
literal|"Deleted tag '%s' (was %s)\n"
argument_list|)
argument_list|,
name|name
argument_list|,
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|verify_tag
specifier|static
name|int
name|verify_tag
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
specifier|const
name|char
modifier|*
name|argv_verify_tag
index|[]
init|=
block|{
literal|"verify-tag"
block|,
literal|"-v"
block|,
literal|"SHA1_HEX"
block|,
name|NULL
block|}
decl_stmt|;
name|argv_verify_tag
index|[
literal|2
index|]
operator|=
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|run_command_v_opt
argument_list|(
name|argv_verify_tag
argument_list|,
name|RUN_GIT_CMD
argument_list|)
condition|)
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"could not verify the tag '%s'"
argument_list|)
argument_list|,
name|name
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|do_sign
specifier|static
name|int
name|do_sign
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buffer
parameter_list|)
block|{
return|return
name|sign_buffer
argument_list|(
name|buffer
argument_list|,
name|buffer
argument_list|,
name|get_signing_key
argument_list|()
argument_list|)
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|tag_template
specifier|static
specifier|const
name|char
name|tag_template
index|[]
init|=
name|N_
argument_list|(
literal|"\nWrite a message for tag:\n  %s\n"
literal|"Lines starting with '%c' will be ignored.\n"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tag_template_nocleanup
specifier|static
specifier|const
name|char
name|tag_template_nocleanup
index|[]
init|=
name|N_
argument_list|(
literal|"\nWrite a message for tag:\n  %s\n"
literal|"Lines starting with '%c' will be kept; you may remove them"
literal|" yourself if you want to.\n"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Parse arg given and add it the ref_sorting array */
end_comment

begin_function
DECL|function|parse_sorting_string
specifier|static
name|int
name|parse_sorting_string
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|struct
name|ref_sorting
modifier|*
modifier|*
name|sorting_tail
parameter_list|)
block|{
name|struct
name|ref_sorting
modifier|*
name|s
decl_stmt|;
name|int
name|len
decl_stmt|;
name|s
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|s
argument_list|)
argument_list|)
expr_stmt|;
name|s
operator|->
name|next
operator|=
operator|*
name|sorting_tail
expr_stmt|;
operator|*
name|sorting_tail
operator|=
name|s
expr_stmt|;
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
block|{
name|s
operator|->
name|reverse
operator|=
literal|1
expr_stmt|;
name|arg
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|skip_prefix
argument_list|(
name|arg
argument_list|,
literal|"version:"
argument_list|,
operator|&
name|arg
argument_list|)
operator|||
name|skip_prefix
argument_list|(
name|arg
argument_list|,
literal|"v:"
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
name|s
operator|->
name|version
operator|=
literal|1
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|arg
argument_list|)
expr_stmt|;
name|s
operator|->
name|atom
operator|=
name|parse_ref_filter_atom
argument_list|(
name|arg
argument_list|,
name|arg
operator|+
name|len
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|git_tag_config
specifier|static
name|int
name|git_tag_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
block|{
name|int
name|status
decl_stmt|;
name|struct
name|ref_sorting
modifier|*
modifier|*
name|sorting_tail
init|=
operator|(
expr|struct
name|ref_sorting
operator|*
operator|*
operator|)
name|cb
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"tag.sort"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|value
condition|)
return|return
name|config_error_nonbool
argument_list|(
name|var
argument_list|)
return|;
name|parse_sorting_string
argument_list|(
name|value
argument_list|,
name|sorting_tail
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|status
operator|=
name|git_gpg_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
name|cb
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
condition|)
return|return
name|status
return|;
if|if
condition|(
name|starts_with
argument_list|(
name|var
argument_list|,
literal|"column."
argument_list|)
condition|)
return|return
name|git_column_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
literal|"tag"
argument_list|,
operator|&
name|colopts
argument_list|)
return|;
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
name|cb
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|write_tag_body
specifier|static
name|void
name|write_tag_body
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|unsigned
name|long
name|size
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|,
modifier|*
name|sp
decl_stmt|;
name|buf
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
operator|&
name|type
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
return|return;
comment|/* skip header */
name|sp
operator|=
name|strstr
argument_list|(
name|buf
argument_list|,
literal|"\n\n"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sp
operator|||
operator|!
name|size
operator|||
name|type
operator|!=
name|OBJ_TAG
condition|)
block|{
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return;
block|}
name|sp
operator|+=
literal|2
expr_stmt|;
comment|/* skip the 2 LFs */
name|write_or_die
argument_list|(
name|fd
argument_list|,
name|sp
argument_list|,
name|parse_signature
argument_list|(
name|sp
argument_list|,
name|buf
operator|+
name|size
operator|-
name|sp
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|build_tag_object
specifier|static
name|int
name|build_tag_object
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|int
name|sign
parameter_list|,
name|unsigned
name|char
modifier|*
name|result
parameter_list|)
block|{
if|if
condition|(
name|sign
operator|&&
name|do_sign
argument_list|(
name|buf
argument_list|)
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"unable to sign the tag"
argument_list|)
argument_list|)
return|;
if|if
condition|(
name|write_sha1_file
argument_list|(
name|buf
operator|->
name|buf
argument_list|,
name|buf
operator|->
name|len
argument_list|,
name|tag_type
argument_list|,
name|result
argument_list|)
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"unable to write tag file"
argument_list|)
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_struct
DECL|struct|create_tag_options
struct|struct
name|create_tag_options
block|{
DECL|member|message_given
name|unsigned
name|int
name|message_given
range|:
literal|1
decl_stmt|;
DECL|member|sign
name|unsigned
name|int
name|sign
decl_stmt|;
enum|enum
block|{
DECL|enumerator|CLEANUP_NONE
name|CLEANUP_NONE
block|,
DECL|enumerator|CLEANUP_SPACE
name|CLEANUP_SPACE
block|,
DECL|enumerator|CLEANUP_ALL
name|CLEANUP_ALL
DECL|member|cleanup_mode
block|}
name|cleanup_mode
enum|;
block|}
struct|;
end_struct

begin_function
DECL|function|create_tag
specifier|static
name|void
name|create_tag
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|object
parameter_list|,
specifier|const
name|char
modifier|*
name|tag
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|struct
name|create_tag_options
modifier|*
name|opt
parameter_list|,
name|unsigned
name|char
modifier|*
name|prev
parameter_list|,
name|unsigned
name|char
modifier|*
name|result
parameter_list|)
block|{
name|enum
name|object_type
name|type
decl_stmt|;
name|char
name|header_buf
index|[
literal|1024
index|]
decl_stmt|;
name|int
name|header_len
decl_stmt|;
name|char
modifier|*
name|path
init|=
name|NULL
decl_stmt|;
name|type
operator|=
name|sha1_object_info
argument_list|(
name|object
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|<=
name|OBJ_NONE
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"bad object type."
argument_list|)
argument_list|)
expr_stmt|;
name|header_len
operator|=
name|snprintf
argument_list|(
name|header_buf
argument_list|,
sizeof|sizeof
argument_list|(
name|header_buf
argument_list|)
argument_list|,
literal|"object %s\n"
literal|"type %s\n"
literal|"tag %s\n"
literal|"tagger %s\n\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|object
argument_list|)
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|,
name|tag
argument_list|,
name|git_committer_info
argument_list|(
name|IDENT_STRICT
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|header_len
operator|>
sizeof|sizeof
argument_list|(
name|header_buf
argument_list|)
operator|-
literal|1
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"tag header too big."
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|opt
operator|->
name|message_given
condition|)
block|{
name|int
name|fd
decl_stmt|;
comment|/* write the template message before editing: */
name|path
operator|=
name|git_pathdup
argument_list|(
literal|"TAG_EDITMSG"
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|path
argument_list|,
name|O_CREAT
operator||
name|O_TRUNC
operator||
name|O_WRONLY
argument_list|,
literal|0600
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
name|_
argument_list|(
literal|"could not create file '%s'"
argument_list|)
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_null_sha1
argument_list|(
name|prev
argument_list|)
condition|)
block|{
name|write_tag_body
argument_list|(
name|fd
argument_list|,
name|prev
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|strbuf_addch
argument_list|(
operator|&
name|buf
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt
operator|->
name|cleanup_mode
operator|==
name|CLEANUP_ALL
condition|)
name|strbuf_commented_addf
argument_list|(
operator|&
name|buf
argument_list|,
name|_
argument_list|(
name|tag_template
argument_list|)
argument_list|,
name|tag
argument_list|,
name|comment_line_char
argument_list|)
expr_stmt|;
else|else
name|strbuf_commented_addf
argument_list|(
operator|&
name|buf
argument_list|,
name|_
argument_list|(
name|tag_template_nocleanup
argument_list|)
argument_list|,
name|tag
argument_list|,
name|comment_line_char
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
name|fd
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|buf
operator|.
name|len
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|launch_editor
argument_list|(
name|path
argument_list|,
name|buf
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
name|_
argument_list|(
literal|"Please supply the message using either -m or -F option.\n"
argument_list|)
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|opt
operator|->
name|cleanup_mode
operator|!=
name|CLEANUP_NONE
condition|)
name|stripspace
argument_list|(
name|buf
argument_list|,
name|opt
operator|->
name|cleanup_mode
operator|==
name|CLEANUP_ALL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|opt
operator|->
name|message_given
operator|&&
operator|!
name|buf
operator|->
name|len
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"no tag message?"
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_insert
argument_list|(
name|buf
argument_list|,
literal|0
argument_list|,
name|header_buf
argument_list|,
name|header_len
argument_list|)
expr_stmt|;
if|if
condition|(
name|build_tag_object
argument_list|(
name|buf
argument_list|,
name|opt
operator|->
name|sign
argument_list|,
name|result
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|path
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
name|_
argument_list|(
literal|"The tag message has been left in %s\n"
argument_list|)
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|path
condition|)
block|{
name|unlink_or_warn
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|path
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_struct
DECL|struct|msg_arg
struct|struct
name|msg_arg
block|{
DECL|member|given
name|int
name|given
decl_stmt|;
DECL|member|buf
name|struct
name|strbuf
name|buf
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|parse_msg_arg
specifier|static
name|int
name|parse_msg_arg
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
block|{
name|struct
name|msg_arg
modifier|*
name|msg
init|=
name|opt
operator|->
name|value
decl_stmt|;
if|if
condition|(
operator|!
name|arg
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|msg
operator|->
name|buf
operator|.
name|len
condition|)
name|strbuf_addstr
argument_list|(
operator|&
operator|(
name|msg
operator|->
name|buf
operator|)
argument_list|,
literal|"\n\n"
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
operator|(
name|msg
operator|->
name|buf
operator|)
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|msg
operator|->
name|given
operator|=
literal|1
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|strbuf_check_tag_ref
specifier|static
name|int
name|strbuf_check_tag_ref
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
if|if
condition|(
name|name
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
return|return
operator|-
literal|1
return|;
name|strbuf_reset
argument_list|(
name|sb
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
name|sb
argument_list|,
literal|"refs/tags/%s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
name|check_refname_format
argument_list|(
name|sb
operator|->
name|buf
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cmd_tag
name|int
name|cmd_tag
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
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|ref
init|=
name|STRBUF_INIT
decl_stmt|;
name|unsigned
name|char
name|object
index|[
literal|20
index|]
decl_stmt|,
name|prev
index|[
literal|20
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|object_ref
decl_stmt|,
modifier|*
name|tag
decl_stmt|;
name|struct
name|create_tag_options
name|opt
decl_stmt|;
name|char
modifier|*
name|cleanup_arg
init|=
name|NULL
decl_stmt|;
name|int
name|create_reflog
init|=
literal|0
decl_stmt|;
name|int
name|annotate
init|=
literal|0
decl_stmt|,
name|force
init|=
literal|0
decl_stmt|;
name|int
name|cmdmode
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|msgfile
init|=
name|NULL
decl_stmt|,
modifier|*
name|keyid
init|=
name|NULL
decl_stmt|;
name|struct
name|msg_arg
name|msg
init|=
block|{
literal|0
block|,
name|STRBUF_INIT
block|}
decl_stmt|;
name|struct
name|ref_transaction
modifier|*
name|transaction
decl_stmt|;
name|struct
name|strbuf
name|err
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|ref_filter
name|filter
decl_stmt|;
specifier|static
name|struct
name|ref_sorting
modifier|*
name|sorting
init|=
name|NULL
decl_stmt|,
modifier|*
modifier|*
name|sorting_tail
init|=
operator|&
name|sorting
decl_stmt|;
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
literal|"list tag names"
argument_list|)
argument_list|,
literal|'l'
argument_list|)
block|,
block|{
name|OPTION_INTEGER
block|,
literal|'n'
block|,
name|NULL
block|,
operator|&
name|filter
operator|.
name|lines
block|,
name|N_
argument_list|(
literal|"n"
argument_list|)
block|,
name|N_
argument_list|(
literal|"print<n> lines of each tag message"
argument_list|)
block|,
name|PARSE_OPT_OPTARG
block|,
name|NULL
block|,
literal|1
block|}
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
literal|"delete tags"
argument_list|)
argument_list|,
literal|'d'
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|'v'
argument_list|,
literal|"verify"
argument_list|,
operator|&
name|cmdmode
argument_list|,
name|N_
argument_list|(
literal|"verify tags"
argument_list|)
argument_list|,
literal|'v'
argument_list|)
block|,
name|OPT_GROUP
argument_list|(
name|N_
argument_list|(
literal|"Tag creation options"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|'a'
argument_list|,
literal|"annotate"
argument_list|,
operator|&
name|annotate
argument_list|,
name|N_
argument_list|(
literal|"annotated tag, needs a message"
argument_list|)
argument_list|)
block|,
name|OPT_CALLBACK
argument_list|(
literal|'m'
argument_list|,
literal|"message"
argument_list|,
operator|&
name|msg
argument_list|,
name|N_
argument_list|(
literal|"message"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"tag message"
argument_list|)
argument_list|,
name|parse_msg_arg
argument_list|)
block|,
name|OPT_FILENAME
argument_list|(
literal|'F'
argument_list|,
literal|"file"
argument_list|,
operator|&
name|msgfile
argument_list|,
name|N_
argument_list|(
literal|"read message from file"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|'s'
argument_list|,
literal|"sign"
argument_list|,
operator|&
name|opt
operator|.
name|sign
argument_list|,
name|N_
argument_list|(
literal|"annotated and GPG-signed tag"
argument_list|)
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"cleanup"
argument_list|,
operator|&
name|cleanup_arg
argument_list|,
name|N_
argument_list|(
literal|"mode"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"how to strip spaces and #comments from message"
argument_list|)
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|'u'
argument_list|,
literal|"local-user"
argument_list|,
operator|&
name|keyid
argument_list|,
name|N_
argument_list|(
literal|"key-id"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"use another key to sign the tag"
argument_list|)
argument_list|)
block|,
name|OPT__FORCE
argument_list|(
operator|&
name|force
argument_list|,
name|N_
argument_list|(
literal|"replace the tag if exists"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"create-reflog"
argument_list|,
operator|&
name|create_reflog
argument_list|,
name|N_
argument_list|(
literal|"create_reflog"
argument_list|)
argument_list|)
block|,
name|OPT_GROUP
argument_list|(
name|N_
argument_list|(
literal|"Tag listing options"
argument_list|)
argument_list|)
block|,
name|OPT_COLUMN
argument_list|(
literal|0
argument_list|,
literal|"column"
argument_list|,
operator|&
name|colopts
argument_list|,
name|N_
argument_list|(
literal|"show tag list in columns"
argument_list|)
argument_list|)
block|,
name|OPT_CONTAINS
argument_list|(
operator|&
name|filter
operator|.
name|with_commit
argument_list|,
name|N_
argument_list|(
literal|"print only tags that contain the commit"
argument_list|)
argument_list|)
block|,
name|OPT_WITH
argument_list|(
operator|&
name|filter
operator|.
name|with_commit
argument_list|,
name|N_
argument_list|(
literal|"print only tags that contain the commit"
argument_list|)
argument_list|)
block|,
name|OPT_CALLBACK
argument_list|(
literal|0
argument_list|,
literal|"sort"
argument_list|,
name|sorting_tail
argument_list|,
name|N_
argument_list|(
literal|"key"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"field name to sort on"
argument_list|)
argument_list|,
operator|&
name|parse_opt_ref_sorting
argument_list|)
block|,
block|{
name|OPTION_CALLBACK
block|,
literal|0
block|,
literal|"points-at"
block|,
operator|&
name|filter
operator|.
name|points_at
block|,
name|N_
argument_list|(
literal|"object"
argument_list|)
block|,
name|N_
argument_list|(
literal|"print only tags of the object"
argument_list|)
block|,
literal|0
block|,
name|parse_opt_object_name
block|}
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|git_config
argument_list|(
name|git_tag_config
argument_list|,
name|sorting_tail
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|opt
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|opt
argument_list|)
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|filter
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|filter
argument_list|)
argument_list|)
expr_stmt|;
name|filter
operator|.
name|lines
operator|=
operator|-
literal|1
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
name|git_tag_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|keyid
condition|)
block|{
name|opt
operator|.
name|sign
operator|=
literal|1
expr_stmt|;
name|set_signing_key
argument_list|(
name|keyid
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|opt
operator|.
name|sign
condition|)
name|annotate
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|argc
operator|==
literal|0
operator|&&
operator|!
name|cmdmode
condition|)
name|cmdmode
operator|=
literal|'l'
expr_stmt|;
if|if
condition|(
operator|(
name|annotate
operator|||
name|msg
operator|.
name|given
operator|||
name|msgfile
operator|||
name|force
operator|)
operator|&&
operator|(
name|cmdmode
operator|!=
literal|0
operator|)
condition|)
name|usage_with_options
argument_list|(
name|git_tag_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|finalize_colopts
argument_list|(
operator|&
name|colopts
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmdmode
operator|==
literal|'l'
operator|&&
name|filter
operator|.
name|lines
operator|!=
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|explicitly_enable_column
argument_list|(
name|colopts
argument_list|)
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"--column and -n are incompatible"
argument_list|)
argument_list|)
expr_stmt|;
name|colopts
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|sorting
condition|)
name|sorting
operator|=
name|ref_default_sorting
argument_list|()
expr_stmt|;
if|if
condition|(
name|cmdmode
operator|==
literal|'l'
condition|)
block|{
name|int
name|ret
decl_stmt|;
if|if
condition|(
name|column_active
argument_list|(
name|colopts
argument_list|)
condition|)
block|{
name|struct
name|column_options
name|copts
decl_stmt|;
name|memset
argument_list|(
operator|&
name|copts
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|copts
argument_list|)
argument_list|)
expr_stmt|;
name|copts
operator|.
name|padding
operator|=
literal|2
expr_stmt|;
name|run_column_filter
argument_list|(
name|colopts
argument_list|,
operator|&
name|copts
argument_list|)
expr_stmt|;
block|}
name|filter
operator|.
name|name_patterns
operator|=
name|argv
expr_stmt|;
name|ret
operator|=
name|list_tags
argument_list|(
operator|&
name|filter
argument_list|,
name|sorting
argument_list|)
expr_stmt|;
if|if
condition|(
name|column_active
argument_list|(
name|colopts
argument_list|)
condition|)
name|stop_column_filter
argument_list|()
expr_stmt|;
return|return
name|ret
return|;
block|}
if|if
condition|(
name|filter
operator|.
name|lines
operator|!=
operator|-
literal|1
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"-n option is only allowed with -l."
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|filter
operator|.
name|with_commit
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"--contains option is only allowed with -l."
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|filter
operator|.
name|points_at
operator|.
name|nr
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"--points-at option is only allowed with -l."
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmdmode
operator|==
literal|'d'
condition|)
return|return
name|for_each_tag_name
argument_list|(
name|argv
argument_list|,
name|delete_tag
argument_list|)
return|;
if|if
condition|(
name|cmdmode
operator|==
literal|'v'
condition|)
return|return
name|for_each_tag_name
argument_list|(
name|argv
argument_list|,
name|verify_tag
argument_list|)
return|;
if|if
condition|(
name|msg
operator|.
name|given
operator|||
name|msgfile
condition|)
block|{
if|if
condition|(
name|msg
operator|.
name|given
operator|&&
name|msgfile
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"only one -F or -m option is allowed."
argument_list|)
argument_list|)
expr_stmt|;
name|annotate
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|msg
operator|.
name|given
condition|)
name|strbuf_addbuf
argument_list|(
operator|&
name|buf
argument_list|,
operator|&
operator|(
name|msg
operator|.
name|buf
operator|)
argument_list|)
expr_stmt|;
else|else
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|msgfile
argument_list|,
literal|"-"
argument_list|)
condition|)
block|{
if|if
condition|(
name|strbuf_read
argument_list|(
operator|&
name|buf
argument_list|,
literal|0
argument_list|,
literal|1024
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
name|_
argument_list|(
literal|"cannot read '%s'"
argument_list|)
argument_list|,
name|msgfile
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|strbuf_read_file
argument_list|(
operator|&
name|buf
argument_list|,
name|msgfile
argument_list|,
literal|1024
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
name|_
argument_list|(
literal|"could not open or read '%s'"
argument_list|)
argument_list|,
name|msgfile
argument_list|)
expr_stmt|;
block|}
block|}
block|}
name|tag
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|object_ref
operator|=
name|argc
operator|==
literal|2
condition|?
name|argv
index|[
literal|1
index|]
else|:
literal|"HEAD"
expr_stmt|;
if|if
condition|(
name|argc
operator|>
literal|2
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"too many params"
argument_list|)
argument_list|)
expr_stmt|;
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
name|_
argument_list|(
literal|"Failed to resolve '%s' as a valid ref."
argument_list|)
argument_list|,
name|object_ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|strbuf_check_tag_ref
argument_list|(
operator|&
name|ref
argument_list|,
name|tag
argument_list|)
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"'%s' is not a valid tag name."
argument_list|)
argument_list|,
name|tag
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_ref
argument_list|(
name|ref
operator|.
name|buf
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
name|_
argument_list|(
literal|"tag '%s' already exists"
argument_list|)
argument_list|,
name|tag
argument_list|)
expr_stmt|;
name|opt
operator|.
name|message_given
operator|=
name|msg
operator|.
name|given
operator|||
name|msgfile
expr_stmt|;
if|if
condition|(
operator|!
name|cleanup_arg
operator|||
operator|!
name|strcmp
argument_list|(
name|cleanup_arg
argument_list|,
literal|"strip"
argument_list|)
condition|)
name|opt
operator|.
name|cleanup_mode
operator|=
name|CLEANUP_ALL
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cleanup_arg
argument_list|,
literal|"verbatim"
argument_list|)
condition|)
name|opt
operator|.
name|cleanup_mode
operator|=
name|CLEANUP_NONE
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cleanup_arg
argument_list|,
literal|"whitespace"
argument_list|)
condition|)
name|opt
operator|.
name|cleanup_mode
operator|=
name|CLEANUP_SPACE
expr_stmt|;
else|else
name|die
argument_list|(
name|_
argument_list|(
literal|"Invalid cleanup mode %s"
argument_list|)
argument_list|,
name|cleanup_arg
argument_list|)
expr_stmt|;
if|if
condition|(
name|annotate
condition|)
name|create_tag
argument_list|(
name|object
argument_list|,
name|tag
argument_list|,
operator|&
name|buf
argument_list|,
operator|&
name|opt
argument_list|,
name|prev
argument_list|,
name|object
argument_list|)
expr_stmt|;
name|transaction
operator|=
name|ref_transaction_begin
argument_list|(
operator|&
name|err
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|transaction
operator|||
name|ref_transaction_update
argument_list|(
name|transaction
argument_list|,
name|ref
operator|.
name|buf
argument_list|,
name|object
argument_list|,
name|prev
argument_list|,
name|create_reflog
condition|?
name|REF_FORCE_CREATE_REFLOG
else|:
literal|0
argument_list|,
name|NULL
argument_list|,
operator|&
name|err
argument_list|)
operator|||
name|ref_transaction_commit
argument_list|(
name|transaction
argument_list|,
operator|&
name|err
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s"
argument_list|,
name|err
operator|.
name|buf
argument_list|)
expr_stmt|;
name|ref_transaction_free
argument_list|(
name|transaction
argument_list|)
expr_stmt|;
if|if
condition|(
name|force
operator|&&
operator|!
name|is_null_sha1
argument_list|(
name|prev
argument_list|)
operator|&&
name|hashcmp
argument_list|(
name|prev
argument_list|,
name|object
argument_list|)
condition|)
name|printf
argument_list|(
name|_
argument_list|(
literal|"Updated tag '%s' (was %s)\n"
argument_list|)
argument_list|,
name|tag
argument_list|,
name|find_unique_abbrev
argument_list|(
name|prev
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|err
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|ref
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

