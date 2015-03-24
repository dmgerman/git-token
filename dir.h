begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|DIR_H
end_ifndef

begin_define
DECL|macro|DIR_H
define|#
directive|define
name|DIR_H
end_define

begin_comment
comment|/* See Documentation/technical/api-directory-listing.txt */
end_comment

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_struct
DECL|struct|dir_entry
struct|struct
name|dir_entry
block|{
DECL|member|len
name|unsigned
name|int
name|len
decl_stmt|;
DECL|member|name
name|char
name|name
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
comment|/* more */
block|}
struct|;
end_struct

begin_define
DECL|macro|EXC_FLAG_NODIR
define|#
directive|define
name|EXC_FLAG_NODIR
value|1
end_define

begin_define
DECL|macro|EXC_FLAG_ENDSWITH
define|#
directive|define
name|EXC_FLAG_ENDSWITH
value|4
end_define

begin_define
DECL|macro|EXC_FLAG_MUSTBEDIR
define|#
directive|define
name|EXC_FLAG_MUSTBEDIR
value|8
end_define

begin_define
DECL|macro|EXC_FLAG_NEGATIVE
define|#
directive|define
name|EXC_FLAG_NEGATIVE
value|16
end_define

begin_comment
comment|/*  * Each excludes file will be parsed into a fresh exclude_list which  * is appended to the relevant exclude_list_group (either EXC_DIRS or  * EXC_FILE).  An exclude_list within the EXC_CMDL exclude_list_group  * can also be used to represent the list of --exclude values passed  * via CLI args.  */
end_comment

begin_struct
DECL|struct|exclude_list
struct|struct
name|exclude_list
block|{
DECL|member|nr
name|int
name|nr
decl_stmt|;
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
comment|/* remember pointer to exclude file contents so we can free() */
DECL|member|filebuf
name|char
modifier|*
name|filebuf
decl_stmt|;
comment|/* origin of list, e.g. path to filename, or descriptive string */
DECL|member|src
specifier|const
name|char
modifier|*
name|src
decl_stmt|;
DECL|struct|exclude
struct|struct
name|exclude
block|{
comment|/* 		 * This allows callers of last_exclude_matching() etc. 		 * to determine the origin of the matching pattern. 		 */
DECL|member|el
name|struct
name|exclude_list
modifier|*
name|el
decl_stmt|;
DECL|member|pattern
specifier|const
name|char
modifier|*
name|pattern
decl_stmt|;
DECL|member|patternlen
name|int
name|patternlen
decl_stmt|;
DECL|member|nowildcardlen
name|int
name|nowildcardlen
decl_stmt|;
DECL|member|base
specifier|const
name|char
modifier|*
name|base
decl_stmt|;
DECL|member|baselen
name|int
name|baselen
decl_stmt|;
DECL|member|flags
name|int
name|flags
decl_stmt|;
comment|/* 		 * Counting starts from 1 for line numbers in ignore files, 		 * and from -1 decrementing for patterns from CLI args. 		 */
DECL|member|srcpos
name|int
name|srcpos
decl_stmt|;
DECL|member|excludes
block|}
modifier|*
modifier|*
name|excludes
struct|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * The contents of the per-directory exclude files are lazily read on  * demand and then cached in memory, one per exclude_stack struct, in  * order to avoid opening and parsing each one every time that  * directory is traversed.  */
end_comment

begin_struct
DECL|struct|exclude_stack
struct|struct
name|exclude_stack
block|{
DECL|member|prev
name|struct
name|exclude_stack
modifier|*
name|prev
decl_stmt|;
comment|/* the struct exclude_stack for the parent directory */
DECL|member|baselen
name|int
name|baselen
decl_stmt|;
DECL|member|exclude_ix
name|int
name|exclude_ix
decl_stmt|;
comment|/* index of exclude_list within EXC_DIRS exclude_list_group */
block|}
struct|;
end_struct

begin_struct
DECL|struct|exclude_list_group
struct|struct
name|exclude_list_group
block|{
DECL|member|nr
DECL|member|alloc
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
DECL|member|el
name|struct
name|exclude_list
modifier|*
name|el
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|dir_struct
struct|struct
name|dir_struct
block|{
DECL|member|nr
DECL|member|alloc
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
DECL|member|ignored_nr
DECL|member|ignored_alloc
name|int
name|ignored_nr
decl_stmt|,
name|ignored_alloc
decl_stmt|;
enum|enum
block|{
DECL|enumerator|DIR_SHOW_IGNORED
name|DIR_SHOW_IGNORED
init|=
literal|1
operator|<<
literal|0
block|,
DECL|enumerator|DIR_SHOW_OTHER_DIRECTORIES
name|DIR_SHOW_OTHER_DIRECTORIES
init|=
literal|1
operator|<<
literal|1
block|,
DECL|enumerator|DIR_HIDE_EMPTY_DIRECTORIES
name|DIR_HIDE_EMPTY_DIRECTORIES
init|=
literal|1
operator|<<
literal|2
block|,
DECL|enumerator|DIR_NO_GITLINKS
name|DIR_NO_GITLINKS
init|=
literal|1
operator|<<
literal|3
block|,
DECL|enumerator|DIR_COLLECT_IGNORED
name|DIR_COLLECT_IGNORED
init|=
literal|1
operator|<<
literal|4
block|,
DECL|enumerator|DIR_SHOW_IGNORED_TOO
name|DIR_SHOW_IGNORED_TOO
init|=
literal|1
operator|<<
literal|5
block|,
DECL|enumerator|DIR_COLLECT_KILLED_ONLY
name|DIR_COLLECT_KILLED_ONLY
init|=
literal|1
operator|<<
literal|6
DECL|member|flags
block|}
name|flags
enum|;
DECL|member|entries
name|struct
name|dir_entry
modifier|*
modifier|*
name|entries
decl_stmt|;
DECL|member|ignored
name|struct
name|dir_entry
modifier|*
modifier|*
name|ignored
decl_stmt|;
comment|/* Exclude info */
DECL|member|exclude_per_dir
specifier|const
name|char
modifier|*
name|exclude_per_dir
decl_stmt|;
comment|/* 	 * We maintain three groups of exclude pattern lists: 	 * 	 * EXC_CMDL lists patterns explicitly given on the command line. 	 * EXC_DIRS lists patterns obtained from per-directory ignore files. 	 * EXC_FILE lists patterns from fallback ignore files, e.g. 	 *   - .git/info/exclude 	 *   - core.excludesfile 	 * 	 * Each group contains multiple exclude lists, a single list 	 * per source. 	 */
DECL|macro|EXC_CMDL
define|#
directive|define
name|EXC_CMDL
value|0
DECL|macro|EXC_DIRS
define|#
directive|define
name|EXC_DIRS
value|1
DECL|macro|EXC_FILE
define|#
directive|define
name|EXC_FILE
value|2
DECL|member|exclude_list_group
name|struct
name|exclude_list_group
name|exclude_list_group
index|[
literal|3
index|]
decl_stmt|;
comment|/* 	 * Temporary variables which are used during loading of the 	 * per-directory exclude lists. 	 * 	 * exclude_stack points to the top of the exclude_stack, and 	 * basebuf contains the full path to the current 	 * (sub)directory in the traversal. Exclude points to the 	 * matching exclude struct if the directory is excluded. 	 */
DECL|member|exclude_stack
name|struct
name|exclude_stack
modifier|*
name|exclude_stack
decl_stmt|;
DECL|member|exclude
name|struct
name|exclude
modifier|*
name|exclude
decl_stmt|;
DECL|member|basebuf
name|char
name|basebuf
index|[
name|PATH_MAX
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * The ordering of these constants is significant, with  * higher-numbered match types signifying "closer" (i.e. more  * specific) matches which will override lower-numbered match types  * when populating the seen[] array.  */
end_comment

begin_define
DECL|macro|MATCHED_RECURSIVELY
define|#
directive|define
name|MATCHED_RECURSIVELY
value|1
end_define

begin_define
DECL|macro|MATCHED_FNMATCH
define|#
directive|define
name|MATCHED_FNMATCH
value|2
end_define

begin_define
DECL|macro|MATCHED_EXACTLY
define|#
directive|define
name|MATCHED_EXACTLY
value|3
end_define

begin_function_decl
specifier|extern
name|int
name|simple_length
parameter_list|(
specifier|const
name|char
modifier|*
name|match
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|no_wildcard
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|common_prefix
parameter_list|(
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|match_pathspec
parameter_list|(
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|namelen
parameter_list|,
name|int
name|prefix
parameter_list|,
name|char
modifier|*
name|seen
parameter_list|,
name|int
name|is_dir
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|report_path_error
parameter_list|(
specifier|const
name|char
modifier|*
name|ps_matched
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
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
name|within_depth
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|namelen
parameter_list|,
name|int
name|depth
parameter_list|,
name|int
name|max_depth
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|fill_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|read_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|len
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|is_excluded_from_list
parameter_list|(
specifier|const
name|char
modifier|*
name|pathname
parameter_list|,
name|int
name|pathlen
parameter_list|,
specifier|const
name|char
modifier|*
name|basename
parameter_list|,
name|int
modifier|*
name|dtype
parameter_list|,
name|struct
name|exclude_list
modifier|*
name|el
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|dir_entry
modifier|*
name|dir_add_ignored
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|pathname
parameter_list|,
name|int
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * these implement the matching logic for dir.c:excluded_from_list and  * attr.c:path_matches()  */
end_comment

begin_function_decl
specifier|extern
name|int
name|match_basename
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|,
name|int
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|match_pathname
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|,
name|int
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|struct
name|exclude
modifier|*
name|last_exclude_matching
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
modifier|*
name|dtype
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|is_excluded
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
modifier|*
name|dtype
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|struct
name|exclude_list
modifier|*
name|add_exclude_list
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
name|int
name|group_type
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|add_excludes_from_file_to_list
parameter_list|(
specifier|const
name|char
modifier|*
name|fname
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
name|struct
name|exclude_list
modifier|*
name|el
parameter_list|,
name|int
name|check_index
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|add_excludes_from_file
parameter_list|(
name|struct
name|dir_struct
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|fname
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|parse_exclude_pattern
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|string
parameter_list|,
name|int
modifier|*
name|patternlen
parameter_list|,
name|int
modifier|*
name|flags
parameter_list|,
name|int
modifier|*
name|nowildcardlen
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|add_exclude
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
name|struct
name|exclude_list
modifier|*
name|el
parameter_list|,
name|int
name|srcpos
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|clear_exclude_list
parameter_list|(
name|struct
name|exclude_list
modifier|*
name|el
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|clear_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|file_exists
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|is_inside_dir
parameter_list|(
specifier|const
name|char
modifier|*
name|dir
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|dir_inside_of
parameter_list|(
specifier|const
name|char
modifier|*
name|subdir
parameter_list|,
specifier|const
name|char
modifier|*
name|dir
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|is_dot_or_dotdot
specifier|static
specifier|inline
name|int
name|is_dot_or_dotdot
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
return|return
operator|(
name|name
index|[
literal|0
index|]
operator|==
literal|'.'
operator|&&
operator|(
name|name
index|[
literal|1
index|]
operator|==
literal|'\0'
operator|||
operator|(
name|name
index|[
literal|1
index|]
operator|==
literal|'.'
operator|&&
name|name
index|[
literal|2
index|]
operator|==
literal|'\0'
operator|)
operator|)
operator|)
return|;
block|}
end_function

begin_function_decl
specifier|extern
name|int
name|is_empty_dir
parameter_list|(
specifier|const
name|char
modifier|*
name|dir
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|setup_standard_excludes
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|REMOVE_DIR_EMPTY_ONLY
define|#
directive|define
name|REMOVE_DIR_EMPTY_ONLY
value|01
end_define

begin_define
DECL|macro|REMOVE_DIR_KEEP_NESTED_GIT
define|#
directive|define
name|REMOVE_DIR_KEEP_NESTED_GIT
value|02
end_define

begin_define
DECL|macro|REMOVE_DIR_KEEP_TOPLEVEL
define|#
directive|define
name|REMOVE_DIR_KEEP_TOPLEVEL
value|04
end_define

begin_function_decl
specifier|extern
name|int
name|remove_dir_recursively
parameter_list|(
name|struct
name|strbuf
modifier|*
name|path
parameter_list|,
name|int
name|flag
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* tries to remove the path with empty directories along it, ignores ENOENT */
end_comment

begin_function_decl
specifier|extern
name|int
name|remove_path
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|strcmp_icase
parameter_list|(
specifier|const
name|char
modifier|*
name|a
parameter_list|,
specifier|const
name|char
modifier|*
name|b
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|strncmp_icase
parameter_list|(
specifier|const
name|char
modifier|*
name|a
parameter_list|,
specifier|const
name|char
modifier|*
name|b
parameter_list|,
name|size_t
name|count
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|fnmatch_icase
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * The prefix part of pattern must not contains wildcards.  */
end_comment

begin_struct_decl
struct_decl|struct
name|pathspec_item
struct_decl|;
end_struct_decl

begin_function_decl
specifier|extern
name|int
name|git_fnmatch
parameter_list|(
specifier|const
name|struct
name|pathspec_item
modifier|*
name|item
parameter_list|,
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|int
name|prefix
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ce_path_match
specifier|static
specifier|inline
name|int
name|ce_path_match
parameter_list|(
specifier|const
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|,
name|char
modifier|*
name|seen
parameter_list|)
block|{
return|return
name|match_pathspec
argument_list|(
name|pathspec
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
argument_list|,
literal|0
argument_list|,
name|seen
argument_list|,
name|S_ISDIR
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
operator|||
name|S_ISGITLINK
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|dir_path_match
specifier|static
specifier|inline
name|int
name|dir_path_match
parameter_list|(
specifier|const
name|struct
name|dir_entry
modifier|*
name|ent
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|,
name|int
name|prefix
parameter_list|,
name|char
modifier|*
name|seen
parameter_list|)
block|{
name|int
name|has_trailing_dir
init|=
name|ent
operator|->
name|len
operator|&&
name|ent
operator|->
name|name
index|[
name|ent
operator|->
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
decl_stmt|;
name|int
name|len
init|=
name|has_trailing_dir
condition|?
name|ent
operator|->
name|len
operator|-
literal|1
else|:
name|ent
operator|->
name|len
decl_stmt|;
return|return
name|match_pathspec
argument_list|(
name|pathspec
argument_list|,
name|ent
operator|->
name|name
argument_list|,
name|len
argument_list|,
name|prefix
argument_list|,
name|seen
argument_list|,
name|has_trailing_dir
argument_list|)
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

