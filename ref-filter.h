begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|REF_FILTER_H
end_ifndef

begin_define
DECL|macro|REF_FILTER_H
define|#
directive|define
name|REF_FILTER_H
end_define

begin_include
include|#
directive|include
file|"sha1-array.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_comment
comment|/* Quoting styles */
end_comment

begin_define
DECL|macro|QUOTE_NONE
define|#
directive|define
name|QUOTE_NONE
value|0
end_define

begin_define
DECL|macro|QUOTE_SHELL
define|#
directive|define
name|QUOTE_SHELL
value|1
end_define

begin_define
DECL|macro|QUOTE_PERL
define|#
directive|define
name|QUOTE_PERL
value|2
end_define

begin_define
DECL|macro|QUOTE_PYTHON
define|#
directive|define
name|QUOTE_PYTHON
value|4
end_define

begin_define
DECL|macro|QUOTE_TCL
define|#
directive|define
name|QUOTE_TCL
value|8
end_define

begin_define
DECL|macro|FILTER_REFS_INCLUDE_BROKEN
define|#
directive|define
name|FILTER_REFS_INCLUDE_BROKEN
value|0x0001
end_define

begin_define
DECL|macro|FILTER_REFS_TAGS
define|#
directive|define
name|FILTER_REFS_TAGS
value|0x0002
end_define

begin_define
DECL|macro|FILTER_REFS_BRANCHES
define|#
directive|define
name|FILTER_REFS_BRANCHES
value|0x0004
end_define

begin_define
DECL|macro|FILTER_REFS_REMOTES
define|#
directive|define
name|FILTER_REFS_REMOTES
value|0x0008
end_define

begin_define
DECL|macro|FILTER_REFS_OTHERS
define|#
directive|define
name|FILTER_REFS_OTHERS
value|0x0010
end_define

begin_define
DECL|macro|FILTER_REFS_ALL
define|#
directive|define
name|FILTER_REFS_ALL
value|(FILTER_REFS_TAGS | FILTER_REFS_BRANCHES | \ 				    FILTER_REFS_REMOTES | FILTER_REFS_OTHERS)
end_define

begin_define
DECL|macro|FILTER_REFS_DETACHED_HEAD
define|#
directive|define
name|FILTER_REFS_DETACHED_HEAD
value|0x0020
end_define

begin_define
DECL|macro|FILTER_REFS_KIND_MASK
define|#
directive|define
name|FILTER_REFS_KIND_MASK
value|(FILTER_REFS_ALL | FILTER_REFS_DETACHED_HEAD)
end_define

begin_struct_decl
struct_decl|struct
name|atom_value
struct_decl|;
end_struct_decl

begin_struct
DECL|struct|ref_sorting
struct|struct
name|ref_sorting
block|{
DECL|member|next
name|struct
name|ref_sorting
modifier|*
name|next
decl_stmt|;
DECL|member|atom
name|int
name|atom
decl_stmt|;
comment|/* index into used_atom array (internal) */
DECL|member|reverse
name|unsigned
name|reverse
range|:
literal|1
decl_stmt|,
DECL|member|version
name|version
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|ref_array_item
struct|struct
name|ref_array_item
block|{
DECL|member|objectname
name|unsigned
name|char
name|objectname
index|[
literal|20
index|]
decl_stmt|;
DECL|member|flag
name|int
name|flag
decl_stmt|;
DECL|member|kind
name|unsigned
name|int
name|kind
decl_stmt|;
DECL|member|ignore
name|int
name|ignore
range|:
literal|1
decl_stmt|;
comment|/* To be removed in the next patch */
DECL|member|symref
specifier|const
name|char
modifier|*
name|symref
decl_stmt|;
DECL|member|commit
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
DECL|member|value
name|struct
name|atom_value
modifier|*
name|value
decl_stmt|;
DECL|member|refname
name|char
name|refname
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|ref_array
struct|struct
name|ref_array
block|{
DECL|member|nr
DECL|member|alloc
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
DECL|member|items
name|struct
name|ref_array_item
modifier|*
modifier|*
name|items
decl_stmt|;
DECL|member|revs
name|struct
name|rev_info
modifier|*
name|revs
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|ref_filter
struct|struct
name|ref_filter
block|{
DECL|member|name_patterns
specifier|const
name|char
modifier|*
modifier|*
name|name_patterns
decl_stmt|;
DECL|member|points_at
name|struct
name|sha1_array
name|points_at
decl_stmt|;
DECL|member|with_commit
name|struct
name|commit_list
modifier|*
name|with_commit
decl_stmt|;
enum|enum
block|{
DECL|enumerator|REF_FILTER_MERGED_NONE
name|REF_FILTER_MERGED_NONE
init|=
literal|0
block|,
DECL|enumerator|REF_FILTER_MERGED_INCLUDE
name|REF_FILTER_MERGED_INCLUDE
block|,
DECL|enumerator|REF_FILTER_MERGED_OMIT
name|REF_FILTER_MERGED_OMIT
DECL|member|merge
block|}
name|merge
enum|;
DECL|member|merge_commit
name|struct
name|commit
modifier|*
name|merge_commit
decl_stmt|;
DECL|member|with_commit_tag_algo
name|unsigned
name|int
name|with_commit_tag_algo
range|:
literal|1
decl_stmt|,
DECL|member|match_as_path
name|match_as_path
range|:
literal|1
decl_stmt|,
DECL|member|detached
name|detached
range|:
literal|1
decl_stmt|;
DECL|member|kind
name|unsigned
name|int
name|kind
decl_stmt|,
DECL|member|lines
name|lines
decl_stmt|;
DECL|member|abbrev
name|int
name|abbrev
decl_stmt|,
DECL|member|verbose
name|verbose
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|ref_filter_cbdata
struct|struct
name|ref_filter_cbdata
block|{
DECL|member|array
name|struct
name|ref_array
modifier|*
name|array
decl_stmt|;
DECL|member|filter
name|struct
name|ref_filter
modifier|*
name|filter
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  Macros for checking --merged and --no-merged options */
end_comment

begin_define
DECL|macro|_OPT_MERGED_NO_MERGED
define|#
directive|define
name|_OPT_MERGED_NO_MERGED
parameter_list|(
name|option
parameter_list|,
name|filter
parameter_list|,
name|h
parameter_list|)
define|\
value|{ OPTION_CALLBACK, 0, option, (filter), N_("commit"), (h), \ 	  PARSE_OPT_LASTARG_DEFAULT | PARSE_OPT_NONEG, \ 	  parse_opt_merge_filter, (intptr_t) "HEAD" \ 	}
end_define

begin_define
DECL|macro|OPT_MERGED
define|#
directive|define
name|OPT_MERGED
parameter_list|(
name|f
parameter_list|,
name|h
parameter_list|)
value|_OPT_MERGED_NO_MERGED("merged", f, h)
end_define

begin_define
DECL|macro|OPT_NO_MERGED
define|#
directive|define
name|OPT_NO_MERGED
parameter_list|(
name|f
parameter_list|,
name|h
parameter_list|)
value|_OPT_MERGED_NO_MERGED("no-merged", f, h)
end_define

begin_comment
comment|/*  * API for filtering a set of refs. Based on the type of refs the user  * has requested, we iterate through those refs and apply filters  * as per the given ref_filter structure and finally store the  * filtered refs in the ref_array structure.  */
end_comment

begin_function_decl
name|int
name|filter_refs
parameter_list|(
name|struct
name|ref_array
modifier|*
name|array
parameter_list|,
name|struct
name|ref_filter
modifier|*
name|filter
parameter_list|,
name|unsigned
name|int
name|type
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  Clear all memory allocated to ref_array */
end_comment

begin_function_decl
name|void
name|ref_array_clear
parameter_list|(
name|struct
name|ref_array
modifier|*
name|array
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  Parse format string and sort specifiers */
end_comment

begin_function_decl
name|int
name|parse_ref_filter_atom
parameter_list|(
specifier|const
name|char
modifier|*
name|atom
parameter_list|,
specifier|const
name|char
modifier|*
name|ep
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  Used to verify if the given format is correct and to parse out the used atoms */
end_comment

begin_function_decl
name|int
name|verify_ref_format
parameter_list|(
specifier|const
name|char
modifier|*
name|format
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  Sort the given ref_array as per the ref_sorting provided */
end_comment

begin_function_decl
name|void
name|ref_array_sort
parameter_list|(
name|struct
name|ref_sorting
modifier|*
name|sort
parameter_list|,
name|struct
name|ref_array
modifier|*
name|array
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  Print the ref using the given format and quote_style */
end_comment

begin_function_decl
name|void
name|show_ref_array_item
parameter_list|(
name|struct
name|ref_array_item
modifier|*
name|info
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
name|int
name|quote_style
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  Callback function for parsing the sort option */
end_comment

begin_function_decl
name|int
name|parse_opt_ref_sorting
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
function_decl|;
end_function_decl

begin_comment
comment|/*  Default sort option based on refname */
end_comment

begin_function_decl
name|struct
name|ref_sorting
modifier|*
name|ref_default_sorting
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  Function to parse --merged and --no-merged options */
end_comment

begin_function_decl
name|int
name|parse_opt_merge_filter
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
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  REF_FILTER_H  */
end_comment

end_unit

