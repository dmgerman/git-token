begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|COMMIT_H
end_ifndef

begin_define
DECL|macro|COMMIT_H
define|#
directive|define
name|COMMIT_H
end_define

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_include
include|#
directive|include
file|"decorate.h"
end_include

begin_struct
DECL|struct|commit_list
struct|struct
name|commit_list
block|{
DECL|member|item
name|struct
name|commit
modifier|*
name|item
decl_stmt|;
DECL|member|next
name|struct
name|commit_list
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|commit
struct|struct
name|commit
block|{
DECL|member|object
name|struct
name|object
name|object
decl_stmt|;
DECL|member|util
name|void
modifier|*
name|util
decl_stmt|;
DECL|member|indegree
name|unsigned
name|int
name|indegree
decl_stmt|;
DECL|member|date
name|unsigned
name|long
name|date
decl_stmt|;
DECL|member|parents
name|struct
name|commit_list
modifier|*
name|parents
decl_stmt|;
DECL|member|tree
name|struct
name|tree
modifier|*
name|tree
decl_stmt|;
DECL|member|buffer
name|char
modifier|*
name|buffer
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
specifier|extern
name|int
name|save_commit_buffer
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
specifier|const
name|char
modifier|*
name|commit_type
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* While we can decorate any object with a name, it's only used for commits.. */
end_comment

begin_decl_stmt
specifier|extern
name|struct
name|decoration
name|name_decoration
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|name_decoration
struct|struct
name|name_decoration
block|{
DECL|member|next
name|struct
name|name_decoration
modifier|*
name|next
decl_stmt|;
DECL|member|type
name|int
name|type
decl_stmt|;
DECL|member|name
name|char
name|name
index|[
literal|1
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|commit
modifier|*
name|lookup_commit
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|commit
modifier|*
name|lookup_commit_reference
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|commit
modifier|*
name|lookup_commit_reference_gently
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|quiet
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|commit
modifier|*
name|lookup_commit_reference_by_name
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_commit_buffer
parameter_list|(
name|struct
name|commit
modifier|*
name|item
parameter_list|,
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|item
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Find beginning and length of commit subject. */
end_comment

begin_function_decl
name|int
name|find_commit_subject
parameter_list|(
specifier|const
name|char
modifier|*
name|commit_buffer
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|subject
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|commit_list
modifier|*
name|commit_list_insert
parameter_list|(
name|struct
name|commit
modifier|*
name|item
parameter_list|,
name|struct
name|commit_list
modifier|*
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|unsigned
name|commit_list_count
parameter_list|(
specifier|const
name|struct
name|commit_list
modifier|*
name|l
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|commit_list
modifier|*
name|commit_list_insert_by_date
parameter_list|(
name|struct
name|commit
modifier|*
name|item
parameter_list|,
name|struct
name|commit_list
modifier|*
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|commit_list_sort_by_date
parameter_list|(
name|struct
name|commit_list
modifier|*
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|free_commit_list
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Commit formats */
end_comment

begin_enum
DECL|enum|cmit_fmt
enum|enum
name|cmit_fmt
block|{
DECL|enumerator|CMIT_FMT_RAW
name|CMIT_FMT_RAW
block|,
DECL|enumerator|CMIT_FMT_MEDIUM
name|CMIT_FMT_MEDIUM
block|,
DECL|enumerator|CMIT_FMT_DEFAULT
name|CMIT_FMT_DEFAULT
init|=
name|CMIT_FMT_MEDIUM
block|,
DECL|enumerator|CMIT_FMT_SHORT
name|CMIT_FMT_SHORT
block|,
DECL|enumerator|CMIT_FMT_FULL
name|CMIT_FMT_FULL
block|,
DECL|enumerator|CMIT_FMT_FULLER
name|CMIT_FMT_FULLER
block|,
DECL|enumerator|CMIT_FMT_ONELINE
name|CMIT_FMT_ONELINE
block|,
DECL|enumerator|CMIT_FMT_EMAIL
name|CMIT_FMT_EMAIL
block|,
DECL|enumerator|CMIT_FMT_USERFORMAT
name|CMIT_FMT_USERFORMAT
block|,
DECL|enumerator|CMIT_FMT_UNSPECIFIED
name|CMIT_FMT_UNSPECIFIED
block|}
enum|;
end_enum

begin_struct
DECL|struct|pretty_print_context
struct|struct
name|pretty_print_context
block|{
DECL|member|fmt
name|enum
name|cmit_fmt
name|fmt
decl_stmt|;
DECL|member|abbrev
name|int
name|abbrev
decl_stmt|;
DECL|member|subject
specifier|const
name|char
modifier|*
name|subject
decl_stmt|;
DECL|member|after_subject
specifier|const
name|char
modifier|*
name|after_subject
decl_stmt|;
DECL|member|date_mode
name|enum
name|date_mode
name|date_mode
decl_stmt|;
DECL|member|need_8bit_cte
name|int
name|need_8bit_cte
decl_stmt|;
DECL|member|show_notes
name|int
name|show_notes
decl_stmt|;
DECL|member|reflog_info
name|struct
name|reflog_walk_info
modifier|*
name|reflog_info
decl_stmt|;
DECL|member|output_encoding
specifier|const
name|char
modifier|*
name|output_encoding
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|userformat_want
struct|struct
name|userformat_want
block|{
DECL|member|notes
name|unsigned
name|notes
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|int
name|has_non_ascii
parameter_list|(
specifier|const
name|char
modifier|*
name|text
parameter_list|)
function_decl|;
end_function_decl

begin_struct_decl
struct_decl|struct
name|rev_info
struct_decl|;
end_struct_decl

begin_comment
comment|/* in revision.h, it circularly uses enum cmit_fmt */
end_comment

begin_function_decl
specifier|extern
name|char
modifier|*
name|logmsg_reencode
parameter_list|(
specifier|const
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
specifier|const
name|char
modifier|*
name|output_encoding
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|reencode_commit_message
parameter_list|(
specifier|const
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|encoding_p
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|get_commit_format
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|struct
name|rev_info
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|userformat_find_requirements
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
name|struct
name|userformat_want
modifier|*
name|w
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|format_commit_message
parameter_list|(
specifier|const
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|struct
name|pretty_print_context
modifier|*
name|context
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|pretty_print_commit
parameter_list|(
specifier|const
name|struct
name|pretty_print_context
modifier|*
name|pp
parameter_list|,
specifier|const
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|pp_commit_easy
parameter_list|(
name|enum
name|cmit_fmt
name|fmt
parameter_list|,
specifier|const
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|pp_user_info
parameter_list|(
specifier|const
name|struct
name|pretty_print_context
modifier|*
name|pp
parameter_list|,
specifier|const
name|char
modifier|*
name|what
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|line
parameter_list|,
specifier|const
name|char
modifier|*
name|encoding
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|pp_title_line
parameter_list|(
specifier|const
name|struct
name|pretty_print_context
modifier|*
name|pp
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|msg_p
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|encoding
parameter_list|,
name|int
name|need_8bit_cte
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|pp_remainder
parameter_list|(
specifier|const
name|struct
name|pretty_print_context
modifier|*
name|pp
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|msg_p
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|int
name|indent
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Removes the first commit from a list sorted by date, and adds all  * of its parents.  **/
end_comment

begin_function_decl
name|struct
name|commit
modifier|*
name|pop_most_recent_commit
parameter_list|(
name|struct
name|commit_list
modifier|*
modifier|*
name|list
parameter_list|,
name|unsigned
name|int
name|mark
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|commit
modifier|*
name|pop_commit
parameter_list|(
name|struct
name|commit_list
modifier|*
modifier|*
name|stack
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|clear_commit_marks
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|unsigned
name|int
name|mark
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Performs an in-place topological sort of list supplied.  *  *   invariant of resulting list is:  *      a reachable from b => ord(b)< ord(a)  *   in addition, when lifo == 0, commits on parallel tracks are  *   sorted in the dates order.  */
end_comment

begin_function_decl
name|void
name|sort_in_topological_order
parameter_list|(
name|struct
name|commit_list
modifier|*
modifier|*
name|list
parameter_list|,
name|int
name|lifo
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|commit_graft
struct|struct
name|commit_graft
block|{
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|nr_parent
name|int
name|nr_parent
decl_stmt|;
comment|/*< 0 if shallow commit */
DECL|member|parent
name|unsigned
name|char
name|parent
index|[
name|FLEX_ARRAY
index|]
index|[
literal|20
index|]
decl_stmt|;
comment|/* more */
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|commit_graft
modifier|*
name|read_graft_line
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|int
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|register_commit_graft
parameter_list|(
name|struct
name|commit_graft
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|commit_graft
modifier|*
name|lookup_commit_graft
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|const
name|unsigned
name|char
modifier|*
name|lookup_replace_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|struct
name|commit_list
modifier|*
name|get_merge_bases
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
parameter_list|,
name|int
name|cleanup
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|struct
name|commit_list
modifier|*
name|get_merge_bases_many
parameter_list|(
name|struct
name|commit
modifier|*
name|one
parameter_list|,
name|int
name|n
parameter_list|,
name|struct
name|commit
modifier|*
modifier|*
name|twos
parameter_list|,
name|int
name|cleanup
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|struct
name|commit_list
modifier|*
name|get_octopus_merge_bases
parameter_list|(
name|struct
name|commit_list
modifier|*
name|in
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|register_shallow
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|unregister_shallow
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|write_shallow_commits
parameter_list|(
name|struct
name|strbuf
modifier|*
name|out
parameter_list|,
name|int
name|use_pack_protocol
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|is_repository_shallow
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|struct
name|commit_list
modifier|*
name|get_shallow_commits
parameter_list|(
name|struct
name|object_array
modifier|*
name|heads
parameter_list|,
name|int
name|depth
parameter_list|,
name|int
name|shallow_flag
parameter_list|,
name|int
name|not_shallow_flag
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|is_descendant_of
parameter_list|(
name|struct
name|commit
modifier|*
parameter_list|,
name|struct
name|commit_list
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|in_merge_bases
parameter_list|(
name|struct
name|commit
modifier|*
parameter_list|,
name|struct
name|commit
modifier|*
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|interactive_add
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
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|run_add_interactive
parameter_list|(
specifier|const
name|char
modifier|*
name|revision
parameter_list|,
specifier|const
name|char
modifier|*
name|patch_mode
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|single_parent
specifier|static
specifier|inline
name|int
name|single_parent
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
return|return
name|commit
operator|->
name|parents
operator|&&
operator|!
name|commit
operator|->
name|parents
operator|->
name|next
return|;
block|}
end_function

begin_function_decl
name|struct
name|commit_list
modifier|*
name|reduce_heads
parameter_list|(
name|struct
name|commit_list
modifier|*
name|heads
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|commit_tree
parameter_list|(
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
name|unsigned
name|char
modifier|*
name|tree
parameter_list|,
name|struct
name|commit_list
modifier|*
name|parents
parameter_list|,
name|unsigned
name|char
modifier|*
name|ret
parameter_list|,
specifier|const
name|char
modifier|*
name|author
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* COMMIT_H */
end_comment

end_unit

