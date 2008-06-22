begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * "git fast-export" builtin command  *  * Copyright (C) 2007 Johannes E. Schindelin  */
end_comment

begin_include
include|#
directive|include
file|"builtin.h"
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
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_include
include|#
directive|include
file|"log-tree.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"decorate.h"
end_include

begin_include
include|#
directive|include
file|"path-list.h"
end_include

begin_include
include|#
directive|include
file|"utf8.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_decl_stmt
DECL|variable|fast_export_usage
specifier|static
specifier|const
name|char
modifier|*
name|fast_export_usage
index|[]
init|=
block|{
literal|"git-fast-export [rev-list-opts]"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|progress
specifier|static
name|int
name|progress
decl_stmt|;
end_decl_stmt

begin_enum
DECL|enumerator|VERBATIM
DECL|enumerator|WARN
DECL|enumerator|STRIP
DECL|enumerator|ABORT
DECL|variable|signed_tag_mode
specifier|static
enum|enum
block|{
name|VERBATIM
block|,
name|WARN
block|,
name|STRIP
block|,
name|ABORT
block|}
name|signed_tag_mode
init|=
name|ABORT
enum|;
end_enum

begin_function
DECL|function|parse_opt_signed_tag_mode
specifier|static
name|int
name|parse_opt_signed_tag_mode
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
if|if
condition|(
name|unset
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"abort"
argument_list|)
condition|)
name|signed_tag_mode
operator|=
name|ABORT
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"verbatim"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"ignore"
argument_list|)
condition|)
name|signed_tag_mode
operator|=
name|VERBATIM
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"warn"
argument_list|)
condition|)
name|signed_tag_mode
operator|=
name|WARN
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"strip"
argument_list|)
condition|)
name|signed_tag_mode
operator|=
name|STRIP
expr_stmt|;
else|else
return|return
name|error
argument_list|(
literal|"Unknown signed-tag mode: %s"
argument_list|,
name|arg
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|idnums
specifier|static
name|struct
name|decoration
name|idnums
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|last_idnum
specifier|static
name|uint32_t
name|last_idnum
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|has_unshown_parent
specifier|static
name|int
name|has_unshown_parent
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|parent
decl_stmt|;
for|for
control|(
name|parent
operator|=
name|commit
operator|->
name|parents
init|;
name|parent
condition|;
name|parent
operator|=
name|parent
operator|->
name|next
control|)
if|if
condition|(
operator|!
operator|(
name|parent
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&
name|SHOWN
operator|)
operator|&&
operator|!
operator|(
name|parent
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&
name|UNINTERESTING
operator|)
condition|)
return|return
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/* Since intptr_t is C99, we do not use it here */
end_comment

begin_function
DECL|function|mark_to_ptr
specifier|static
specifier|inline
name|uint32_t
modifier|*
name|mark_to_ptr
parameter_list|(
name|uint32_t
name|mark
parameter_list|)
block|{
return|return
operator|(
operator|(
name|uint32_t
operator|*
operator|)
name|NULL
operator|)
operator|+
name|mark
return|;
block|}
end_function

begin_function
DECL|function|ptr_to_mark
specifier|static
specifier|inline
name|uint32_t
name|ptr_to_mark
parameter_list|(
name|void
modifier|*
name|mark
parameter_list|)
block|{
return|return
operator|(
name|uint32_t
operator|*
operator|)
name|mark
operator|-
operator|(
name|uint32_t
operator|*
operator|)
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|mark_object
specifier|static
specifier|inline
name|void
name|mark_object
parameter_list|(
name|struct
name|object
modifier|*
name|object
parameter_list|,
name|uint32_t
name|mark
parameter_list|)
block|{
name|add_decoration
argument_list|(
operator|&
name|idnums
argument_list|,
name|object
argument_list|,
name|mark_to_ptr
argument_list|(
name|mark
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|mark_next_object
specifier|static
specifier|inline
name|void
name|mark_next_object
parameter_list|(
name|struct
name|object
modifier|*
name|object
parameter_list|)
block|{
name|mark_object
argument_list|(
name|object
argument_list|,
operator|++
name|last_idnum
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_object_mark
specifier|static
name|int
name|get_object_mark
parameter_list|(
name|struct
name|object
modifier|*
name|object
parameter_list|)
block|{
name|void
modifier|*
name|decoration
init|=
name|lookup_decoration
argument_list|(
operator|&
name|idnums
argument_list|,
name|object
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|decoration
condition|)
return|return
literal|0
return|;
return|return
name|ptr_to_mark
argument_list|(
name|decoration
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|show_progress
specifier|static
name|void
name|show_progress
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|int
name|counter
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|progress
condition|)
return|return;
if|if
condition|(
operator|(
operator|++
name|counter
operator|%
name|progress
operator|)
operator|==
literal|0
condition|)
name|printf
argument_list|(
literal|"progress %d objects\n"
argument_list|,
name|counter
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|handle_object
specifier|static
name|void
name|handle_object
parameter_list|(
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
decl_stmt|;
name|struct
name|object
modifier|*
name|object
decl_stmt|;
if|if
condition|(
name|is_null_sha1
argument_list|(
name|sha1
argument_list|)
condition|)
return|return;
name|object
operator|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|object
condition|)
name|die
argument_list|(
literal|"Could not read blob %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|object
operator|->
name|flags
operator|&
name|SHOWN
condition|)
return|return;
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
name|die
argument_list|(
literal|"Could not read blob %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|mark_next_object
argument_list|(
name|object
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"blob\nmark :%d\ndata %lu\n"
argument_list|,
name|last_idnum
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|size
operator|&&
name|fwrite
argument_list|(
name|buf
argument_list|,
name|size
argument_list|,
literal|1
argument_list|,
name|stdout
argument_list|)
operator|!=
literal|1
condition|)
name|die
argument_list|(
literal|"Could not write blob %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
name|show_progress
argument_list|()
expr_stmt|;
name|object
operator|->
name|flags
operator||=
name|SHOWN
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_filemodify
specifier|static
name|void
name|show_filemodify
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
name|q
parameter_list|,
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
name|void
modifier|*
name|data
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
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|diff_filespec
modifier|*
name|spec
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
operator|->
name|two
decl_stmt|;
if|if
condition|(
name|is_null_sha1
argument_list|(
name|spec
operator|->
name|sha1
argument_list|)
condition|)
name|printf
argument_list|(
literal|"D %s\n"
argument_list|,
name|spec
operator|->
name|path
argument_list|)
expr_stmt|;
else|else
block|{
name|struct
name|object
modifier|*
name|object
init|=
name|lookup_object
argument_list|(
name|spec
operator|->
name|sha1
argument_list|)
decl_stmt|;
name|printf
argument_list|(
literal|"M %06o :%d %s\n"
argument_list|,
name|spec
operator|->
name|mode
argument_list|,
name|get_object_mark
argument_list|(
name|object
argument_list|)
argument_list|,
name|spec
operator|->
name|path
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|find_encoding
specifier|static
specifier|const
name|char
modifier|*
name|find_encoding
parameter_list|(
specifier|const
name|char
modifier|*
name|begin
parameter_list|,
specifier|const
name|char
modifier|*
name|end
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|needle
init|=
literal|"\nencoding "
decl_stmt|;
name|char
modifier|*
name|bol
decl_stmt|,
modifier|*
name|eol
decl_stmt|;
name|bol
operator|=
name|memmem
argument_list|(
name|begin
argument_list|,
name|end
condition|?
name|end
operator|-
name|begin
else|:
name|strlen
argument_list|(
name|begin
argument_list|)
argument_list|,
name|needle
argument_list|,
name|strlen
argument_list|(
name|needle
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|bol
condition|)
return|return
name|git_commit_encoding
return|;
name|bol
operator|+=
name|strlen
argument_list|(
name|needle
argument_list|)
expr_stmt|;
name|eol
operator|=
name|strchrnul
argument_list|(
name|bol
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
operator|*
name|eol
operator|=
literal|'\0'
expr_stmt|;
return|return
name|bol
return|;
block|}
end_function

begin_function
DECL|function|handle_commit
specifier|static
name|void
name|handle_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|)
block|{
name|int
name|saved_output_format
init|=
name|rev
operator|->
name|diffopt
operator|.
name|output_format
decl_stmt|;
specifier|const
name|char
modifier|*
name|author
decl_stmt|,
modifier|*
name|author_end
decl_stmt|,
modifier|*
name|committer
decl_stmt|,
modifier|*
name|committer_end
decl_stmt|;
specifier|const
name|char
modifier|*
name|encoding
decl_stmt|,
modifier|*
name|message
decl_stmt|;
name|char
modifier|*
name|reencoded
init|=
name|NULL
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|p
decl_stmt|;
name|int
name|i
decl_stmt|;
name|rev
operator|->
name|diffopt
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_CALLBACK
expr_stmt|;
name|parse_commit
argument_list|(
name|commit
argument_list|)
expr_stmt|;
name|author
operator|=
name|strstr
argument_list|(
name|commit
operator|->
name|buffer
argument_list|,
literal|"\nauthor "
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|author
condition|)
name|die
argument_list|(
literal|"Could not find author in commit %s"
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
name|author
operator|++
expr_stmt|;
name|author_end
operator|=
name|strchrnul
argument_list|(
name|author
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|committer
operator|=
name|strstr
argument_list|(
name|author_end
argument_list|,
literal|"\ncommitter "
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|committer
condition|)
name|die
argument_list|(
literal|"Could not find committer in commit %s"
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
name|committer
operator|++
expr_stmt|;
name|committer_end
operator|=
name|strchrnul
argument_list|(
name|committer
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|message
operator|=
name|strstr
argument_list|(
name|committer_end
argument_list|,
literal|"\n\n"
argument_list|)
expr_stmt|;
name|encoding
operator|=
name|find_encoding
argument_list|(
name|committer_end
argument_list|,
name|message
argument_list|)
expr_stmt|;
if|if
condition|(
name|message
condition|)
name|message
operator|+=
literal|2
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|parents
condition|)
block|{
name|parse_commit
argument_list|(
name|commit
operator|->
name|parents
operator|->
name|item
argument_list|)
expr_stmt|;
name|diff_tree_sha1
argument_list|(
name|commit
operator|->
name|parents
operator|->
name|item
operator|->
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|commit
operator|->
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|,
literal|""
argument_list|,
operator|&
name|rev
operator|->
name|diffopt
argument_list|)
expr_stmt|;
block|}
else|else
name|diff_root_tree_sha1
argument_list|(
name|commit
operator|->
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|,
literal|""
argument_list|,
operator|&
name|rev
operator|->
name|diffopt
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
name|diff_queued_diff
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|handle_object
argument_list|(
name|diff_queued_diff
operator|.
name|queue
index|[
name|i
index|]
operator|->
name|two
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|mark_next_object
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_encoding_utf8
argument_list|(
name|encoding
argument_list|)
condition|)
name|reencoded
operator|=
name|reencode_string
argument_list|(
name|message
argument_list|,
literal|"UTF-8"
argument_list|,
name|encoding
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
operator|->
name|parents
condition|)
name|printf
argument_list|(
literal|"reset %s\n"
argument_list|,
operator|(
specifier|const
name|char
operator|*
operator|)
name|commit
operator|->
name|util
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"commit %s\nmark :%d\n%.*s\n%.*s\ndata %u\n%s"
argument_list|,
operator|(
specifier|const
name|char
operator|*
operator|)
name|commit
operator|->
name|util
argument_list|,
name|last_idnum
argument_list|,
call|(
name|int
call|)
argument_list|(
name|author_end
operator|-
name|author
argument_list|)
argument_list|,
name|author
argument_list|,
call|(
name|int
call|)
argument_list|(
name|committer_end
operator|-
name|committer
argument_list|)
argument_list|,
name|committer
argument_list|,
call|(
name|unsigned
call|)
argument_list|(
name|reencoded
condition|?
name|strlen
argument_list|(
name|reencoded
argument_list|)
else|:
name|message
condition|?
name|strlen
argument_list|(
name|message
argument_list|)
else|:
literal|0
argument_list|)
argument_list|,
name|reencoded
condition|?
name|reencoded
else|:
name|message
condition|?
name|message
else|:
literal|""
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|reencoded
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
name|commit
operator|->
name|parents
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
name|mark
init|=
name|get_object_mark
argument_list|(
operator|&
name|p
operator|->
name|item
operator|->
name|object
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|mark
condition|)
continue|continue;
if|if
condition|(
name|i
operator|==
literal|0
condition|)
name|printf
argument_list|(
literal|"from :%d\n"
argument_list|,
name|mark
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"merge :%d\n"
argument_list|,
name|mark
argument_list|)
expr_stmt|;
name|i
operator|++
expr_stmt|;
block|}
name|log_tree_diff_flush
argument_list|(
name|rev
argument_list|)
expr_stmt|;
name|rev
operator|->
name|diffopt
operator|.
name|output_format
operator|=
name|saved_output_format
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
name|show_progress
argument_list|()
expr_stmt|;
block|}
end_function

begin_function
DECL|function|handle_tail
specifier|static
name|void
name|handle_tail
parameter_list|(
name|struct
name|object_array
modifier|*
name|commits
parameter_list|,
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
while|while
condition|(
name|commits
operator|->
name|nr
condition|)
block|{
name|commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|commits
operator|->
name|objects
index|[
name|commits
operator|->
name|nr
operator|-
literal|1
index|]
operator|.
name|item
expr_stmt|;
if|if
condition|(
name|has_unshown_parent
argument_list|(
name|commit
argument_list|)
condition|)
return|return;
name|handle_commit
argument_list|(
name|commit
argument_list|,
name|revs
argument_list|)
expr_stmt|;
name|commits
operator|->
name|nr
operator|--
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|handle_tag
specifier|static
name|void
name|handle_tag
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|struct
name|tag
modifier|*
name|tag
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
decl_stmt|;
specifier|const
name|char
modifier|*
name|tagger
decl_stmt|,
modifier|*
name|tagger_end
decl_stmt|,
modifier|*
name|message
decl_stmt|;
name|size_t
name|message_size
init|=
literal|0
decl_stmt|;
name|buf
operator|=
name|read_sha1_file
argument_list|(
name|tag
operator|->
name|object
operator|.
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
name|die
argument_list|(
literal|"Could not read tag %s"
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
name|message
operator|=
name|memmem
argument_list|(
name|buf
argument_list|,
name|size
argument_list|,
literal|"\n\n"
argument_list|,
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|message
condition|)
block|{
name|message
operator|+=
literal|2
expr_stmt|;
name|message_size
operator|=
name|strlen
argument_list|(
name|message
argument_list|)
expr_stmt|;
block|}
name|tagger
operator|=
name|memmem
argument_list|(
name|buf
argument_list|,
name|message
condition|?
name|message
operator|-
name|buf
else|:
name|size
argument_list|,
literal|"\ntagger "
argument_list|,
literal|8
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tagger
condition|)
name|die
argument_list|(
literal|"No tagger for tag %s"
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
name|tagger
operator|++
expr_stmt|;
name|tagger_end
operator|=
name|strchrnul
argument_list|(
name|tagger
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
comment|/* handle signed tags */
if|if
condition|(
name|message
condition|)
block|{
specifier|const
name|char
modifier|*
name|signature
init|=
name|strstr
argument_list|(
name|message
argument_list|,
literal|"\n-----BEGIN PGP SIGNATURE-----\n"
argument_list|)
decl_stmt|;
if|if
condition|(
name|signature
condition|)
switch|switch
condition|(
name|signed_tag_mode
condition|)
block|{
case|case
name|ABORT
case|:
name|die
argument_list|(
literal|"Encountered signed tag %s; use "
literal|"--signed-tag=<mode> to handle it."
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
case|case
name|WARN
case|:
name|warning
argument_list|(
literal|"Exporting signed tag %s"
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
comment|/* fallthru */
case|case
name|VERBATIM
case|:
break|break;
case|case
name|STRIP
case|:
name|message_size
operator|=
name|signature
operator|+
literal|1
operator|-
name|message
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|name
argument_list|,
literal|"refs/tags/"
argument_list|)
condition|)
name|name
operator|+=
literal|10
expr_stmt|;
name|printf
argument_list|(
literal|"tag %s\nfrom :%d\n%.*s\ndata %d\n%.*s\n"
argument_list|,
name|name
argument_list|,
name|get_object_mark
argument_list|(
name|tag
operator|->
name|tagged
argument_list|)
argument_list|,
call|(
name|int
call|)
argument_list|(
name|tagger_end
operator|-
name|tagger
argument_list|)
argument_list|,
name|tagger
argument_list|,
operator|(
name|int
operator|)
name|message_size
argument_list|,
operator|(
name|int
operator|)
name|message_size
argument_list|,
name|message
condition|?
name|message
else|:
literal|""
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_tags_and_duplicates
specifier|static
name|void
name|get_tags_and_duplicates
parameter_list|(
name|struct
name|object_array
modifier|*
name|pending
parameter_list|,
name|struct
name|path_list
modifier|*
name|extra_refs
parameter_list|)
block|{
name|struct
name|tag
modifier|*
name|tag
decl_stmt|;
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
name|pending
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|object_array_entry
modifier|*
name|e
init|=
name|pending
operator|->
name|objects
operator|+
name|i
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
init|=
name|commit
decl_stmt|;
name|char
modifier|*
name|full_name
decl_stmt|;
if|if
condition|(
name|dwim_ref
argument_list|(
name|e
operator|->
name|name
argument_list|,
name|strlen
argument_list|(
name|e
operator|->
name|name
argument_list|)
argument_list|,
name|sha1
argument_list|,
operator|&
name|full_name
argument_list|)
operator|!=
literal|1
condition|)
continue|continue;
switch|switch
condition|(
name|e
operator|->
name|item
operator|->
name|type
condition|)
block|{
case|case
name|OBJ_COMMIT
case|:
name|commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|e
operator|->
name|item
expr_stmt|;
break|break;
case|case
name|OBJ_TAG
case|:
name|tag
operator|=
operator|(
expr|struct
name|tag
operator|*
operator|)
name|e
operator|->
name|item
expr_stmt|;
while|while
condition|(
name|tag
operator|&&
name|tag
operator|->
name|object
operator|.
name|type
operator|==
name|OBJ_TAG
condition|)
block|{
name|path_list_insert
argument_list|(
name|full_name
argument_list|,
name|extra_refs
argument_list|)
operator|->
name|util
operator|=
name|tag
expr_stmt|;
name|tag
operator|=
operator|(
expr|struct
name|tag
operator|*
operator|)
name|tag
operator|->
name|tagged
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|tag
condition|)
name|die
argument_list|(
literal|"Tag %s points nowhere?"
argument_list|,
name|e
operator|->
name|name
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|tag
operator|->
name|object
operator|.
name|type
condition|)
block|{
case|case
name|OBJ_COMMIT
case|:
name|commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|tag
expr_stmt|;
break|break;
case|case
name|OBJ_BLOB
case|:
name|handle_object
argument_list|(
name|tag
operator|->
name|object
operator|.
name|sha1
argument_list|)
expr_stmt|;
continue|continue;
block|}
break|break;
default|default:
name|die
argument_list|(
literal|"Unexpected object of type %s"
argument_list|,
name|typename
argument_list|(
name|e
operator|->
name|item
operator|->
name|type
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|commit
operator|->
name|util
condition|)
comment|/* more than one name for the same object */
name|path_list_insert
argument_list|(
name|full_name
argument_list|,
name|extra_refs
argument_list|)
operator|->
name|util
operator|=
name|commit
expr_stmt|;
else|else
name|commit
operator|->
name|util
operator|=
name|full_name
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|handle_tags_and_duplicates
specifier|static
name|void
name|handle_tags_and_duplicates
parameter_list|(
name|struct
name|path_list
modifier|*
name|extra_refs
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
name|extra_refs
operator|->
name|nr
operator|-
literal|1
init|;
name|i
operator|>=
literal|0
condition|;
name|i
operator|--
control|)
block|{
specifier|const
name|char
modifier|*
name|name
init|=
name|extra_refs
operator|->
name|items
index|[
name|i
index|]
operator|.
name|path
decl_stmt|;
name|struct
name|object
modifier|*
name|object
init|=
name|extra_refs
operator|->
name|items
index|[
name|i
index|]
operator|.
name|util
decl_stmt|;
switch|switch
condition|(
name|object
operator|->
name|type
condition|)
block|{
case|case
name|OBJ_TAG
case|:
name|handle_tag
argument_list|(
name|name
argument_list|,
operator|(
expr|struct
name|tag
operator|*
operator|)
name|object
argument_list|)
expr_stmt|;
break|break;
case|case
name|OBJ_COMMIT
case|:
comment|/* create refs pointing to already seen commits */
name|commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|object
expr_stmt|;
name|printf
argument_list|(
literal|"reset %s\nfrom :%d\n\n"
argument_list|,
name|name
argument_list|,
name|get_object_mark
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|)
argument_list|)
expr_stmt|;
name|show_progress
argument_list|()
expr_stmt|;
break|break;
block|}
block|}
block|}
end_function

begin_function
DECL|function|export_marks
specifier|static
name|void
name|export_marks
parameter_list|(
name|char
modifier|*
name|file
parameter_list|)
block|{
name|unsigned
name|int
name|i
decl_stmt|;
name|uint32_t
name|mark
decl_stmt|;
name|struct
name|object_decoration
modifier|*
name|deco
init|=
name|idnums
operator|.
name|hash
decl_stmt|;
name|FILE
modifier|*
name|f
decl_stmt|;
name|f
operator|=
name|fopen
argument_list|(
name|file
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|f
condition|)
name|error
argument_list|(
literal|"Unable to open marks file %s for writing"
argument_list|,
name|file
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
name|idnums
operator|.
name|size
condition|;
operator|++
name|i
control|)
block|{
name|deco
operator|++
expr_stmt|;
if|if
condition|(
name|deco
operator|&&
name|deco
operator|->
name|base
operator|&&
name|deco
operator|->
name|base
operator|->
name|type
operator|==
literal|1
condition|)
block|{
name|mark
operator|=
name|ptr_to_mark
argument_list|(
name|deco
operator|->
name|decoration
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|f
argument_list|,
literal|":%u %s\n"
argument_list|,
name|mark
argument_list|,
name|sha1_to_hex
argument_list|(
name|deco
operator|->
name|base
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|ferror
argument_list|(
name|f
argument_list|)
operator|||
name|fclose
argument_list|(
name|f
argument_list|)
condition|)
name|error
argument_list|(
literal|"Unable to write marks file %s."
argument_list|,
name|file
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|import_marks
specifier|static
name|void
name|import_marks
parameter_list|(
name|char
modifier|*
name|input_file
parameter_list|)
block|{
name|char
name|line
index|[
literal|512
index|]
decl_stmt|;
name|FILE
modifier|*
name|f
init|=
name|fopen
argument_list|(
name|input_file
argument_list|,
literal|"r"
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|f
condition|)
name|die
argument_list|(
literal|"cannot read %s: %s"
argument_list|,
name|input_file
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
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
name|f
argument_list|)
condition|)
block|{
name|uint32_t
name|mark
decl_stmt|;
name|char
modifier|*
name|line_end
decl_stmt|,
modifier|*
name|mark_end
decl_stmt|;
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
name|object
decl_stmt|;
name|line_end
operator|=
name|strchr
argument_list|(
name|line
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
name|line
index|[
literal|0
index|]
operator|!=
literal|':'
operator|||
operator|!
name|line_end
condition|)
name|die
argument_list|(
literal|"corrupt mark line: %s"
argument_list|,
name|line
argument_list|)
expr_stmt|;
operator|*
name|line_end
operator|=
literal|0
expr_stmt|;
name|mark
operator|=
name|strtoumax
argument_list|(
name|line
operator|+
literal|1
argument_list|,
operator|&
name|mark_end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|mark
operator|||
name|mark_end
operator|==
name|line
operator|+
literal|1
operator|||
operator|*
name|mark_end
operator|!=
literal|' '
operator|||
name|get_sha1
argument_list|(
name|mark_end
operator|+
literal|1
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt mark line: %s"
argument_list|,
name|line
argument_list|)
expr_stmt|;
name|object
operator|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|object
condition|)
name|die
argument_list|(
literal|"Could not read blob %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|object
operator|->
name|flags
operator|&
name|SHOWN
condition|)
name|error
argument_list|(
literal|"Object %s already has a mark"
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|mark_object
argument_list|(
name|object
argument_list|,
name|mark
argument_list|)
expr_stmt|;
if|if
condition|(
name|last_idnum
operator|<
name|mark
condition|)
name|last_idnum
operator|=
name|mark
expr_stmt|;
name|object
operator|->
name|flags
operator||=
name|SHOWN
expr_stmt|;
block|}
name|fclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_fast_export
name|int
name|cmd_fast_export
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
name|rev_info
name|revs
decl_stmt|;
name|struct
name|object_array
name|commits
init|=
block|{
literal|0
block|,
literal|0
block|,
name|NULL
block|}
decl_stmt|;
name|struct
name|path_list
name|extra_refs
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|char
modifier|*
name|export_filename
init|=
name|NULL
decl_stmt|,
modifier|*
name|import_filename
init|=
name|NULL
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_INTEGER
argument_list|(
literal|0
argument_list|,
literal|"progress"
argument_list|,
operator|&
name|progress
argument_list|,
literal|"show progress after<n> objects"
argument_list|)
block|,
name|OPT_CALLBACK
argument_list|(
literal|0
argument_list|,
literal|"signed-tags"
argument_list|,
operator|&
name|signed_tag_mode
argument_list|,
literal|"mode"
argument_list|,
literal|"select handling of signed tags"
argument_list|,
name|parse_opt_signed_tag_mode
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"export-marks"
argument_list|,
operator|&
name|export_filename
argument_list|,
literal|"FILE"
argument_list|,
literal|"Dump marks to this file"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"import-marks"
argument_list|,
operator|&
name|import_filename
argument_list|,
literal|"FILE"
argument_list|,
literal|"Import marks from this file"
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
comment|/* we handle encodings */
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|init_revisions
argument_list|(
operator|&
name|revs
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|argc
operator|=
name|setup_revisions
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
operator|&
name|revs
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|options
argument_list|,
name|fast_export_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|>
literal|1
condition|)
name|usage_with_options
argument_list|(
name|fast_export_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|import_filename
condition|)
name|import_marks
argument_list|(
name|import_filename
argument_list|)
expr_stmt|;
name|get_tags_and_duplicates
argument_list|(
operator|&
name|revs
operator|.
name|pending
argument_list|,
operator|&
name|extra_refs
argument_list|)
expr_stmt|;
if|if
condition|(
name|prepare_revision_walk
argument_list|(
operator|&
name|revs
argument_list|)
condition|)
name|die
argument_list|(
literal|"revision walk setup failed"
argument_list|)
expr_stmt|;
name|revs
operator|.
name|diffopt
operator|.
name|format_callback
operator|=
name|show_filemodify
expr_stmt|;
name|DIFF_OPT_SET
argument_list|(
operator|&
name|revs
operator|.
name|diffopt
argument_list|,
name|RECURSIVE
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|commit
operator|=
name|get_revision
argument_list|(
operator|&
name|revs
argument_list|)
operator|)
condition|)
block|{
if|if
condition|(
name|has_unshown_parent
argument_list|(
name|commit
argument_list|)
condition|)
block|{
name|struct
name|commit_list
modifier|*
name|parent
init|=
name|commit
operator|->
name|parents
decl_stmt|;
name|add_object_array
argument_list|(
operator|&
name|commit
operator|->
name|object
argument_list|,
name|NULL
argument_list|,
operator|&
name|commits
argument_list|)
expr_stmt|;
for|for
control|(
init|;
name|parent
condition|;
name|parent
operator|=
name|parent
operator|->
name|next
control|)
if|if
condition|(
operator|!
name|parent
operator|->
name|item
operator|->
name|util
condition|)
name|parent
operator|->
name|item
operator|->
name|util
operator|=
name|commit
operator|->
name|util
expr_stmt|;
block|}
else|else
block|{
name|handle_commit
argument_list|(
name|commit
argument_list|,
operator|&
name|revs
argument_list|)
expr_stmt|;
name|handle_tail
argument_list|(
operator|&
name|commits
argument_list|,
operator|&
name|revs
argument_list|)
expr_stmt|;
block|}
block|}
name|handle_tags_and_duplicates
argument_list|(
operator|&
name|extra_refs
argument_list|)
expr_stmt|;
if|if
condition|(
name|export_filename
condition|)
name|export_marks
argument_list|(
name|export_filename
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

