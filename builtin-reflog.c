begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"tree-walk.h"
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
file|"reachable.h"
end_include

begin_comment
comment|/*  * reflog expire  */
end_comment

begin_decl_stmt
DECL|variable|reflog_expire_usage
specifier|static
specifier|const
name|char
name|reflog_expire_usage
index|[]
init|=
literal|"git-reflog expire [--verbose] [--dry-run] [--stale-fix] [--expire=<time>] [--expire-unreachable=<time>] [--all]<refs>..."
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|default_reflog_expire
specifier|static
name|unsigned
name|long
name|default_reflog_expire
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|default_reflog_expire_unreachable
specifier|static
name|unsigned
name|long
name|default_reflog_expire_unreachable
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|cmd_reflog_expire_cb
struct|struct
name|cmd_reflog_expire_cb
block|{
DECL|member|revs
name|struct
name|rev_info
name|revs
decl_stmt|;
DECL|member|dry_run
name|int
name|dry_run
decl_stmt|;
DECL|member|stalefix
name|int
name|stalefix
decl_stmt|;
DECL|member|verbose
name|int
name|verbose
decl_stmt|;
DECL|member|expire_total
name|unsigned
name|long
name|expire_total
decl_stmt|;
DECL|member|expire_unreachable
name|unsigned
name|long
name|expire_unreachable
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|expire_reflog_cb
struct|struct
name|expire_reflog_cb
block|{
DECL|member|newlog
name|FILE
modifier|*
name|newlog
decl_stmt|;
DECL|member|ref
specifier|const
name|char
modifier|*
name|ref
decl_stmt|;
DECL|member|ref_commit
name|struct
name|commit
modifier|*
name|ref_commit
decl_stmt|;
DECL|member|cmd
name|struct
name|cmd_reflog_expire_cb
modifier|*
name|cmd
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|INCOMPLETE
define|#
directive|define
name|INCOMPLETE
value|(1u<<10)
end_define

begin_define
DECL|macro|STUDYING
define|#
directive|define
name|STUDYING
value|(1u<<11)
end_define

begin_function
DECL|function|tree_is_complete
specifier|static
name|int
name|tree_is_complete
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
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
name|complete
decl_stmt|;
name|struct
name|tree
modifier|*
name|tree
decl_stmt|;
name|tree
operator|=
name|lookup_tree
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|tree
operator|->
name|object
operator|.
name|flags
operator|&
name|SEEN
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|tree
operator|->
name|object
operator|.
name|flags
operator|&
name|INCOMPLETE
condition|)
return|return
literal|0
return|;
name|desc
operator|.
name|buf
operator|=
name|tree
operator|->
name|buffer
expr_stmt|;
name|desc
operator|.
name|size
operator|=
name|tree
operator|->
name|size
expr_stmt|;
if|if
condition|(
operator|!
name|desc
operator|.
name|buf
condition|)
block|{
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|void
modifier|*
name|data
init|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|desc
operator|.
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|data
condition|)
block|{
name|tree
operator|->
name|object
operator|.
name|flags
operator||=
name|INCOMPLETE
expr_stmt|;
return|return
literal|0
return|;
block|}
name|desc
operator|.
name|buf
operator|=
name|data
expr_stmt|;
name|tree
operator|->
name|buffer
operator|=
name|data
expr_stmt|;
block|}
name|complete
operator|=
literal|1
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
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
operator|||
operator|(
name|S_ISDIR
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
operator|&&
operator|!
name|tree_is_complete
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
operator|)
condition|)
block|{
name|tree
operator|->
name|object
operator|.
name|flags
operator||=
name|INCOMPLETE
expr_stmt|;
name|complete
operator|=
literal|0
expr_stmt|;
block|}
block|}
name|free
argument_list|(
name|tree
operator|->
name|buffer
argument_list|)
expr_stmt|;
name|tree
operator|->
name|buffer
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|complete
condition|)
name|tree
operator|->
name|object
operator|.
name|flags
operator||=
name|SEEN
expr_stmt|;
return|return
name|complete
return|;
block|}
end_function

begin_function
DECL|function|commit_is_complete
specifier|static
name|int
name|commit_is_complete
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
name|struct
name|object_array
name|study
decl_stmt|;
name|struct
name|object_array
name|found
decl_stmt|;
name|int
name|is_incomplete
init|=
literal|0
decl_stmt|;
name|int
name|i
decl_stmt|;
comment|/* early return */
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|SEEN
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|INCOMPLETE
condition|)
return|return
literal|0
return|;
comment|/* 	 * Find all commits that are reachable and are not marked as 	 * SEEN.  Then make sure the trees and blobs contained are 	 * complete.  After that, mark these commits also as SEEN. 	 * If some of the objects that are needed to complete this 	 * commit are missing, mark this commit as INCOMPLETE. 	 */
name|memset
argument_list|(
operator|&
name|study
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|study
argument_list|)
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|found
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|found
argument_list|)
argument_list|)
expr_stmt|;
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
name|study
argument_list|)
expr_stmt|;
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
name|found
argument_list|)
expr_stmt|;
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|STUDYING
expr_stmt|;
while|while
condition|(
name|study
operator|.
name|nr
condition|)
block|{
name|struct
name|commit
modifier|*
name|c
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|parent
decl_stmt|;
name|c
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|study
operator|.
name|objects
index|[
operator|--
name|study
operator|.
name|nr
index|]
operator|.
name|item
expr_stmt|;
if|if
condition|(
operator|!
name|c
operator|->
name|object
operator|.
name|parsed
operator|&&
operator|!
name|parse_object
argument_list|(
name|c
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
name|c
operator|->
name|object
operator|.
name|flags
operator||=
name|INCOMPLETE
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|object
operator|.
name|flags
operator|&
name|INCOMPLETE
condition|)
block|{
name|is_incomplete
operator|=
literal|1
expr_stmt|;
break|break;
block|}
elseif|else
if|if
condition|(
name|c
operator|->
name|object
operator|.
name|flags
operator|&
name|SEEN
condition|)
continue|continue;
for|for
control|(
name|parent
operator|=
name|c
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
block|{
name|struct
name|commit
modifier|*
name|p
init|=
name|parent
operator|->
name|item
decl_stmt|;
if|if
condition|(
name|p
operator|->
name|object
operator|.
name|flags
operator|&
name|STUDYING
condition|)
continue|continue;
name|p
operator|->
name|object
operator|.
name|flags
operator||=
name|STUDYING
expr_stmt|;
name|add_object_array
argument_list|(
operator|&
name|p
operator|->
name|object
argument_list|,
name|NULL
argument_list|,
operator|&
name|study
argument_list|)
expr_stmt|;
name|add_object_array
argument_list|(
operator|&
name|p
operator|->
name|object
argument_list|,
name|NULL
argument_list|,
operator|&
name|found
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|is_incomplete
condition|)
block|{
comment|/* 		 * make sure all commits in "found" array have all the 		 * necessary objects. 		 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|found
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|commit
modifier|*
name|c
init|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|found
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|item
decl_stmt|;
if|if
condition|(
operator|!
name|tree_is_complete
argument_list|(
name|c
operator|->
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
block|{
name|is_incomplete
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|object
operator|.
name|flags
operator||=
name|INCOMPLETE
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|is_incomplete
condition|)
block|{
comment|/* mark all found commits as complete, iow SEEN */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|found
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|found
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|item
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
block|}
block|}
comment|/* clear flags from the objects we traversed */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|found
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|found
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|item
operator|->
name|flags
operator|&=
operator|~
name|STUDYING
expr_stmt|;
if|if
condition|(
name|is_incomplete
condition|)
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|INCOMPLETE
expr_stmt|;
else|else
block|{
comment|/* 		 * If we come here, we have (1) traversed the ancestry chain 		 * from the "commit" until we reach SEEN commits (which are 		 * known to be complete), and (2) made sure that the commits 		 * encountered during the above traversal refer to trees that 		 * are complete.  Which means that we know *all* the commits 		 * we have seen during this process are complete. 		 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|found
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|found
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|item
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
block|}
comment|/* free object arrays */
name|free
argument_list|(
name|study
operator|.
name|objects
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|found
operator|.
name|objects
argument_list|)
expr_stmt|;
return|return
operator|!
name|is_incomplete
return|;
block|}
end_function

begin_function
DECL|function|keep_entry
specifier|static
name|int
name|keep_entry
parameter_list|(
name|struct
name|commit
modifier|*
modifier|*
name|it
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
if|if
condition|(
name|is_null_sha1
argument_list|(
name|sha1
argument_list|)
condition|)
return|return
literal|1
return|;
name|commit
operator|=
name|lookup_commit_reference_gently
argument_list|(
name|sha1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
condition|)
return|return
literal|0
return|;
comment|/* 	 * Make sure everything in this commit exists. 	 * 	 * We have walked all the objects reachable from the refs 	 * and cache earlier.  The commits reachable by this commit 	 * must meet SEEN commits -- and then we should mark them as 	 * SEEN as well. 	 */
if|if
condition|(
operator|!
name|commit_is_complete
argument_list|(
name|commit
argument_list|)
condition|)
return|return
literal|0
return|;
operator|*
name|it
operator|=
name|commit
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|expire_reflog_ent
specifier|static
name|int
name|expire_reflog_ent
parameter_list|(
name|unsigned
name|char
modifier|*
name|osha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|nsha1
parameter_list|,
specifier|const
name|char
modifier|*
name|email
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|,
name|int
name|tz
parameter_list|,
specifier|const
name|char
modifier|*
name|message
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|struct
name|expire_reflog_cb
modifier|*
name|cb
init|=
name|cb_data
decl_stmt|;
name|struct
name|commit
modifier|*
name|old
decl_stmt|,
modifier|*
name|new
decl_stmt|;
if|if
condition|(
name|timestamp
operator|<
name|cb
operator|->
name|cmd
operator|->
name|expire_total
condition|)
goto|goto
name|prune
goto|;
name|old
operator|=
name|new
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|cb
operator|->
name|cmd
operator|->
name|stalefix
operator|&&
operator|(
operator|!
name|keep_entry
argument_list|(
operator|&
name|old
argument_list|,
name|osha1
argument_list|)
operator|||
operator|!
name|keep_entry
argument_list|(
operator|&
name|new
argument_list|,
name|nsha1
argument_list|)
operator|)
condition|)
goto|goto
name|prune
goto|;
if|if
condition|(
name|timestamp
operator|<
name|cb
operator|->
name|cmd
operator|->
name|expire_unreachable
condition|)
block|{
if|if
condition|(
operator|!
name|cb
operator|->
name|ref_commit
condition|)
goto|goto
name|prune
goto|;
if|if
condition|(
operator|!
name|old
operator|&&
operator|!
name|is_null_sha1
argument_list|(
name|osha1
argument_list|)
condition|)
name|old
operator|=
name|lookup_commit_reference_gently
argument_list|(
name|osha1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|new
operator|&&
operator|!
name|is_null_sha1
argument_list|(
name|nsha1
argument_list|)
condition|)
name|new
operator|=
name|lookup_commit_reference_gently
argument_list|(
name|nsha1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|old
operator|&&
operator|!
name|in_merge_bases
argument_list|(
name|old
argument_list|,
name|cb
operator|->
name|ref_commit
argument_list|)
operator|)
operator|||
operator|(
name|new
operator|&&
operator|!
name|in_merge_bases
argument_list|(
name|new
argument_list|,
name|cb
operator|->
name|ref_commit
argument_list|)
operator|)
condition|)
goto|goto
name|prune
goto|;
block|}
if|if
condition|(
name|cb
operator|->
name|newlog
condition|)
block|{
name|char
name|sign
init|=
operator|(
name|tz
operator|<
literal|0
operator|)
condition|?
literal|'-'
else|:
literal|'+'
decl_stmt|;
name|int
name|zone
init|=
operator|(
name|tz
operator|<
literal|0
operator|)
condition|?
operator|(
operator|-
name|tz
operator|)
else|:
name|tz
decl_stmt|;
name|fprintf
argument_list|(
name|cb
operator|->
name|newlog
argument_list|,
literal|"%s %s %s %lu %c%04d\t%s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|osha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|nsha1
argument_list|)
argument_list|,
name|email
argument_list|,
name|timestamp
argument_list|,
name|sign
argument_list|,
name|zone
argument_list|,
name|message
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|cb
operator|->
name|cmd
operator|->
name|verbose
condition|)
name|printf
argument_list|(
literal|"keep %s"
argument_list|,
name|message
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
name|prune
label|:
if|if
condition|(
operator|!
name|cb
operator|->
name|newlog
operator|||
name|cb
operator|->
name|cmd
operator|->
name|verbose
condition|)
name|printf
argument_list|(
literal|"%sprune %s"
argument_list|,
name|cb
operator|->
name|newlog
condition|?
literal|""
else|:
literal|"would "
argument_list|,
name|message
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|expire_reflog
specifier|static
name|int
name|expire_reflog
parameter_list|(
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
parameter_list|,
name|int
name|unused
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|struct
name|cmd_reflog_expire_cb
modifier|*
name|cmd
init|=
name|cb_data
decl_stmt|;
name|struct
name|expire_reflog_cb
name|cb
decl_stmt|;
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
name|char
modifier|*
name|newlog_path
init|=
name|NULL
decl_stmt|;
name|int
name|status
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
name|ref
argument_list|,
literal|"refs/"
argument_list|,
literal|5
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"not a ref '%s'"
argument_list|,
name|ref
argument_list|)
return|;
name|memset
argument_list|(
operator|&
name|cb
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|cb
argument_list|)
argument_list|)
expr_stmt|;
comment|/* we take the lock for the ref itself to prevent it from 	 * getting updated. 	 */
name|lock
operator|=
name|lock_ref_sha1
argument_list|(
name|ref
operator|+
literal|5
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lock
condition|)
return|return
name|error
argument_list|(
literal|"cannot lock ref '%s'"
argument_list|,
name|ref
argument_list|)
return|;
if|if
condition|(
operator|!
name|file_exists
argument_list|(
name|lock
operator|->
name|log_file
argument_list|)
condition|)
goto|goto
name|finish
goto|;
if|if
condition|(
operator|!
name|cmd
operator|->
name|dry_run
condition|)
block|{
name|newlog_path
operator|=
name|xstrdup
argument_list|(
name|git_path
argument_list|(
literal|"logs/%s.lock"
argument_list|,
name|ref
argument_list|)
argument_list|)
expr_stmt|;
name|cb
operator|.
name|newlog
operator|=
name|fopen
argument_list|(
name|newlog_path
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
block|}
name|cb
operator|.
name|ref_commit
operator|=
name|lookup_commit_reference_gently
argument_list|(
name|sha1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|cb
operator|.
name|ref
operator|=
name|ref
expr_stmt|;
name|cb
operator|.
name|cmd
operator|=
name|cmd
expr_stmt|;
name|for_each_reflog_ent
argument_list|(
name|ref
argument_list|,
name|expire_reflog_ent
argument_list|,
operator|&
name|cb
argument_list|)
expr_stmt|;
name|finish
label|:
if|if
condition|(
name|cb
operator|.
name|newlog
condition|)
block|{
if|if
condition|(
name|fclose
argument_list|(
name|cb
operator|.
name|newlog
argument_list|)
condition|)
name|status
operator||=
name|error
argument_list|(
literal|"%s: %s"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|,
name|newlog_path
argument_list|)
expr_stmt|;
if|if
condition|(
name|rename
argument_list|(
name|newlog_path
argument_list|,
name|lock
operator|->
name|log_file
argument_list|)
condition|)
block|{
name|status
operator||=
name|error
argument_list|(
literal|"cannot rename %s to %s"
argument_list|,
name|newlog_path
argument_list|,
name|lock
operator|->
name|log_file
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|newlog_path
argument_list|)
expr_stmt|;
block|}
block|}
name|free
argument_list|(
name|newlog_path
argument_list|)
expr_stmt|;
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
return|return
name|status
return|;
block|}
end_function

begin_function
DECL|function|reflog_expire_config
specifier|static
name|int
name|reflog_expire_config
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
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"gc.reflogexpire"
argument_list|)
condition|)
name|default_reflog_expire
operator|=
name|approxidate
argument_list|(
name|value
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"gc.reflogexpireunreachable"
argument_list|)
condition|)
name|default_reflog_expire_unreachable
operator|=
name|approxidate
argument_list|(
name|value
argument_list|)
expr_stmt|;
else|else
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_reflog_expire
specifier|static
name|int
name|cmd_reflog_expire
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
name|cmd_reflog_expire_cb
name|cb
decl_stmt|;
name|unsigned
name|long
name|now
init|=
name|time
argument_list|(
name|NULL
argument_list|)
decl_stmt|;
name|int
name|i
decl_stmt|,
name|status
decl_stmt|,
name|do_all
decl_stmt|;
name|git_config
argument_list|(
name|reflog_expire_config
argument_list|)
expr_stmt|;
name|save_commit_buffer
operator|=
literal|0
expr_stmt|;
name|do_all
operator|=
name|status
operator|=
literal|0
expr_stmt|;
name|memset
argument_list|(
operator|&
name|cb
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|cb
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|default_reflog_expire_unreachable
condition|)
name|default_reflog_expire_unreachable
operator|=
name|now
operator|-
literal|30
operator|*
literal|24
operator|*
literal|3600
expr_stmt|;
if|if
condition|(
operator|!
name|default_reflog_expire
condition|)
name|default_reflog_expire
operator|=
name|now
operator|-
literal|90
operator|*
literal|24
operator|*
literal|3600
expr_stmt|;
name|cb
operator|.
name|expire_total
operator|=
name|default_reflog_expire
expr_stmt|;
name|cb
operator|.
name|expire_unreachable
operator|=
name|default_reflog_expire_unreachable
expr_stmt|;
comment|/* 	 * We can trust the commits and objects reachable from refs 	 * even in older repository.  We cannot trust what's reachable 	 * from reflog if the repository was pruned with older git. 	 */
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
literal|"--dry-run"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-n"
argument_list|)
condition|)
name|cb
operator|.
name|dry_run
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--expire="
argument_list|,
literal|9
argument_list|)
condition|)
name|cb
operator|.
name|expire_total
operator|=
name|approxidate
argument_list|(
name|arg
operator|+
literal|9
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--expire-unreachable="
argument_list|,
literal|21
argument_list|)
condition|)
name|cb
operator|.
name|expire_unreachable
operator|=
name|approxidate
argument_list|(
name|arg
operator|+
literal|21
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--stale-fix"
argument_list|)
condition|)
name|cb
operator|.
name|stalefix
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--all"
argument_list|)
condition|)
name|do_all
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--verbose"
argument_list|)
condition|)
name|cb
operator|.
name|verbose
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--"
argument_list|)
condition|)
block|{
name|i
operator|++
expr_stmt|;
break|break;
block|}
elseif|else
if|if
condition|(
name|arg
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
name|usage
argument_list|(
name|reflog_expire_usage
argument_list|)
expr_stmt|;
else|else
break|break;
block|}
if|if
condition|(
name|cb
operator|.
name|stalefix
condition|)
block|{
name|init_revisions
argument_list|(
operator|&
name|cb
operator|.
name|revs
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
if|if
condition|(
name|cb
operator|.
name|verbose
condition|)
name|printf
argument_list|(
literal|"Marking reachable objects..."
argument_list|)
expr_stmt|;
name|mark_reachable_objects
argument_list|(
operator|&
name|cb
operator|.
name|revs
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|cb
operator|.
name|verbose
condition|)
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|do_all
condition|)
name|status
operator||=
name|for_each_ref
argument_list|(
name|expire_reflog
argument_list|,
operator|&
name|cb
argument_list|)
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|argc
condition|)
block|{
specifier|const
name|char
modifier|*
name|ref
init|=
name|argv
index|[
name|i
operator|++
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|resolve_ref
argument_list|(
name|ref
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
name|status
operator||=
name|error
argument_list|(
literal|"%s points nowhere!"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|status
operator||=
name|expire_reflog
argument_list|(
name|ref
argument_list|,
name|sha1
argument_list|,
literal|0
argument_list|,
operator|&
name|cb
argument_list|)
expr_stmt|;
block|}
return|return
name|status
return|;
block|}
end_function

begin_comment
comment|/*  * main "reflog"  */
end_comment

begin_decl_stmt
DECL|variable|reflog_usage
specifier|static
specifier|const
name|char
name|reflog_usage
index|[]
init|=
literal|"git-reflog (expire | ...)"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_reflog
name|int
name|cmd_reflog
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
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
name|usage
argument_list|(
name|reflog_usage
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"expire"
argument_list|)
condition|)
return|return
name|cmd_reflog_expire
argument_list|(
name|argc
operator|-
literal|1
argument_list|,
name|argv
operator|+
literal|1
argument_list|,
name|prefix
argument_list|)
return|;
else|else
name|usage
argument_list|(
name|reflog_usage
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

