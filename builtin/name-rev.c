begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"sha1-lookup.h"
end_include

begin_define
DECL|macro|CUTOFF_DATE_SLOP
define|#
directive|define
name|CUTOFF_DATE_SLOP
value|86400
end_define

begin_comment
DECL|macro|CUTOFF_DATE_SLOP
comment|/* one day */
end_comment

begin_typedef
DECL|struct|rev_name
typedef|typedef
struct|struct
name|rev_name
block|{
DECL|member|tip_name
specifier|const
name|char
modifier|*
name|tip_name
decl_stmt|;
DECL|member|generation
name|int
name|generation
decl_stmt|;
DECL|member|distance
name|int
name|distance
decl_stmt|;
DECL|typedef|rev_name
block|}
name|rev_name
typedef|;
end_typedef

begin_decl_stmt
DECL|variable|cutoff
specifier|static
name|long
name|cutoff
init|=
name|LONG_MAX
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* How many generations are maximally preferred over _one_ merge traversal? */
end_comment

begin_define
DECL|macro|MERGE_TRAVERSAL_WEIGHT
define|#
directive|define
name|MERGE_TRAVERSAL_WEIGHT
value|65535
end_define

begin_function
DECL|function|name_rev
specifier|static
name|void
name|name_rev
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
specifier|const
name|char
modifier|*
name|tip_name
parameter_list|,
name|int
name|generation
parameter_list|,
name|int
name|distance
parameter_list|,
name|int
name|deref
parameter_list|)
block|{
name|struct
name|rev_name
modifier|*
name|name
init|=
operator|(
expr|struct
name|rev_name
operator|*
operator|)
name|commit
operator|->
name|util
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|parents
decl_stmt|;
name|int
name|parent_number
init|=
literal|1
decl_stmt|;
name|parse_commit
argument_list|(
name|commit
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|date
operator|<
name|cutoff
condition|)
return|return;
if|if
condition|(
name|deref
condition|)
block|{
name|tip_name
operator|=
name|xstrfmt
argument_list|(
literal|"%s^0"
argument_list|,
name|tip_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|generation
condition|)
name|die
argument_list|(
literal|"generation: %d, but deref?"
argument_list|,
name|generation
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|name
operator|==
name|NULL
condition|)
block|{
name|name
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
name|rev_name
argument_list|)
argument_list|)
expr_stmt|;
name|commit
operator|->
name|util
operator|=
name|name
expr_stmt|;
goto|goto
name|copy_data
goto|;
block|}
elseif|else
if|if
condition|(
name|name
operator|->
name|distance
operator|>
name|distance
condition|)
block|{
name|copy_data
label|:
name|name
operator|->
name|tip_name
operator|=
name|tip_name
expr_stmt|;
name|name
operator|->
name|generation
operator|=
name|generation
expr_stmt|;
name|name
operator|->
name|distance
operator|=
name|distance
expr_stmt|;
block|}
else|else
return|return;
for|for
control|(
name|parents
operator|=
name|commit
operator|->
name|parents
init|;
name|parents
condition|;
name|parents
operator|=
name|parents
operator|->
name|next
operator|,
name|parent_number
operator|++
control|)
block|{
if|if
condition|(
name|parent_number
operator|>
literal|1
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|tip_name
argument_list|)
decl_stmt|;
name|char
modifier|*
name|new_name
init|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|1
operator|+
name|decimal_length
argument_list|(
name|generation
argument_list|)
operator|+
comment|/* ~<n> */
literal|1
operator|+
literal|2
operator|+
comment|/* ^NN */
literal|1
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|>
literal|2
operator|&&
operator|!
name|strcmp
argument_list|(
name|tip_name
operator|+
name|len
operator|-
literal|2
argument_list|,
literal|"^0"
argument_list|)
condition|)
name|len
operator|-=
literal|2
expr_stmt|;
if|if
condition|(
name|generation
operator|>
literal|0
condition|)
name|sprintf
argument_list|(
name|new_name
argument_list|,
literal|"%.*s~%d^%d"
argument_list|,
name|len
argument_list|,
name|tip_name
argument_list|,
name|generation
argument_list|,
name|parent_number
argument_list|)
expr_stmt|;
else|else
name|sprintf
argument_list|(
name|new_name
argument_list|,
literal|"%.*s^%d"
argument_list|,
name|len
argument_list|,
name|tip_name
argument_list|,
name|parent_number
argument_list|)
expr_stmt|;
name|name_rev
argument_list|(
name|parents
operator|->
name|item
argument_list|,
name|new_name
argument_list|,
literal|0
argument_list|,
name|distance
operator|+
name|MERGE_TRAVERSAL_WEIGHT
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|name_rev
argument_list|(
name|parents
operator|->
name|item
argument_list|,
name|tip_name
argument_list|,
name|generation
operator|+
literal|1
argument_list|,
name|distance
operator|+
literal|1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|subpath_matches
specifier|static
name|int
name|subpath_matches
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|filter
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|subpath
init|=
name|path
decl_stmt|;
while|while
condition|(
name|subpath
condition|)
block|{
if|if
condition|(
operator|!
name|wildmatch
argument_list|(
name|filter
argument_list|,
name|subpath
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
condition|)
return|return
name|subpath
operator|-
name|path
return|;
name|subpath
operator|=
name|strchr
argument_list|(
name|subpath
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
name|subpath
condition|)
name|subpath
operator|++
expr_stmt|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|name_ref_abbrev
specifier|static
specifier|const
name|char
modifier|*
name|name_ref_abbrev
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|int
name|shorten_unambiguous
parameter_list|)
block|{
if|if
condition|(
name|shorten_unambiguous
condition|)
name|refname
operator|=
name|shorten_unambiguous_ref
argument_list|(
name|refname
argument_list|,
literal|0
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|starts_with
argument_list|(
name|refname
argument_list|,
literal|"refs/heads/"
argument_list|)
condition|)
name|refname
operator|=
name|refname
operator|+
literal|11
expr_stmt|;
elseif|else
if|if
condition|(
name|starts_with
argument_list|(
name|refname
argument_list|,
literal|"refs/"
argument_list|)
condition|)
name|refname
operator|=
name|refname
operator|+
literal|5
expr_stmt|;
return|return
name|refname
return|;
block|}
end_function

begin_struct
DECL|struct|name_ref_data
struct|struct
name|name_ref_data
block|{
DECL|member|tags_only
name|int
name|tags_only
decl_stmt|;
DECL|member|name_only
name|int
name|name_only
decl_stmt|;
DECL|member|ref_filter
specifier|const
name|char
modifier|*
name|ref_filter
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|tip_table
specifier|static
struct|struct
name|tip_table
block|{
DECL|struct|tip_table_entry
struct|struct
name|tip_table_entry
block|{
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|refname
specifier|const
name|char
modifier|*
name|refname
decl_stmt|;
DECL|member|table
block|}
modifier|*
name|table
struct|;
DECL|member|nr
name|int
name|nr
decl_stmt|;
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|sorted
name|int
name|sorted
decl_stmt|;
DECL|variable|tip_table
block|}
name|tip_table
struct|;
end_struct

begin_function
DECL|function|add_to_tip_table
specifier|static
name|void
name|add_to_tip_table
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
name|refname
parameter_list|,
name|int
name|shorten_unambiguous
parameter_list|)
block|{
name|refname
operator|=
name|name_ref_abbrev
argument_list|(
name|refname
argument_list|,
name|shorten_unambiguous
argument_list|)
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|tip_table
operator|.
name|table
argument_list|,
name|tip_table
operator|.
name|nr
operator|+
literal|1
argument_list|,
name|tip_table
operator|.
name|alloc
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|tip_table
operator|.
name|table
index|[
name|tip_table
operator|.
name|nr
index|]
operator|.
name|sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|tip_table
operator|.
name|table
index|[
name|tip_table
operator|.
name|nr
index|]
operator|.
name|refname
operator|=
name|xstrdup
argument_list|(
name|refname
argument_list|)
expr_stmt|;
name|tip_table
operator|.
name|nr
operator|++
expr_stmt|;
name|tip_table
operator|.
name|sorted
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|tipcmp
specifier|static
name|int
name|tipcmp
parameter_list|(
specifier|const
name|void
modifier|*
name|a_
parameter_list|,
specifier|const
name|void
modifier|*
name|b_
parameter_list|)
block|{
specifier|const
name|struct
name|tip_table_entry
modifier|*
name|a
init|=
name|a_
decl_stmt|,
modifier|*
name|b
init|=
name|b_
decl_stmt|;
return|return
name|hashcmp
argument_list|(
name|a
operator|->
name|sha1
argument_list|,
name|b
operator|->
name|sha1
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|name_ref
specifier|static
name|int
name|name_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|flags
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|struct
name|name_ref_data
modifier|*
name|data
init|=
name|cb_data
decl_stmt|;
name|int
name|can_abbreviate_output
init|=
name|data
operator|->
name|tags_only
operator|&&
name|data
operator|->
name|name_only
decl_stmt|;
name|int
name|deref
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|data
operator|->
name|tags_only
operator|&&
operator|!
name|starts_with
argument_list|(
name|path
argument_list|,
literal|"refs/tags/"
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|data
operator|->
name|ref_filter
condition|)
block|{
switch|switch
condition|(
name|subpath_matches
argument_list|(
name|path
argument_list|,
name|data
operator|->
name|ref_filter
argument_list|)
condition|)
block|{
case|case
operator|-
literal|1
case|:
comment|/* did not match */
return|return
literal|0
return|;
case|case
literal|0
case|:
comment|/* matched fully */
break|break;
default|default:
comment|/* matched subpath */
name|can_abbreviate_output
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
name|add_to_tip_table
argument_list|(
name|sha1
argument_list|,
name|path
argument_list|,
name|can_abbreviate_output
argument_list|)
expr_stmt|;
while|while
condition|(
name|o
operator|&&
name|o
operator|->
name|type
operator|==
name|OBJ_TAG
condition|)
block|{
name|struct
name|tag
modifier|*
name|t
init|=
operator|(
expr|struct
name|tag
operator|*
operator|)
name|o
decl_stmt|;
if|if
condition|(
operator|!
name|t
operator|->
name|tagged
condition|)
break|break;
comment|/* broken repository */
name|o
operator|=
name|parse_object
argument_list|(
name|t
operator|->
name|tagged
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|deref
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|o
operator|&&
name|o
operator|->
name|type
operator|==
name|OBJ_COMMIT
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o
decl_stmt|;
name|path
operator|=
name|name_ref_abbrev
argument_list|(
name|path
argument_list|,
name|can_abbreviate_output
argument_list|)
expr_stmt|;
name|name_rev
argument_list|(
name|commit
argument_list|,
name|xstrdup
argument_list|(
name|path
argument_list|)
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|,
name|deref
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|nth_tip_table_ent
specifier|static
specifier|const
name|unsigned
name|char
modifier|*
name|nth_tip_table_ent
parameter_list|(
name|size_t
name|ix
parameter_list|,
name|void
modifier|*
name|table_
parameter_list|)
block|{
name|struct
name|tip_table_entry
modifier|*
name|table
init|=
name|table_
decl_stmt|;
return|return
name|table
index|[
name|ix
index|]
operator|.
name|sha1
return|;
block|}
end_function

begin_function
DECL|function|get_exact_ref_match
specifier|static
specifier|const
name|char
modifier|*
name|get_exact_ref_match
parameter_list|(
specifier|const
name|struct
name|object
modifier|*
name|o
parameter_list|)
block|{
name|int
name|found
decl_stmt|;
if|if
condition|(
operator|!
name|tip_table
operator|.
name|table
operator|||
operator|!
name|tip_table
operator|.
name|nr
condition|)
return|return
name|NULL
return|;
if|if
condition|(
operator|!
name|tip_table
operator|.
name|sorted
condition|)
block|{
name|qsort
argument_list|(
name|tip_table
operator|.
name|table
argument_list|,
name|tip_table
operator|.
name|nr
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|tip_table
operator|.
name|table
argument_list|)
argument_list|,
name|tipcmp
argument_list|)
expr_stmt|;
name|tip_table
operator|.
name|sorted
operator|=
literal|1
expr_stmt|;
block|}
name|found
operator|=
name|sha1_pos
argument_list|(
name|o
operator|->
name|sha1
argument_list|,
name|tip_table
operator|.
name|table
argument_list|,
name|tip_table
operator|.
name|nr
argument_list|,
name|nth_tip_table_ent
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|found
condition|)
return|return
name|tip_table
operator|.
name|table
index|[
name|found
index|]
operator|.
name|refname
return|;
return|return
name|NULL
return|;
block|}
end_function

begin_comment
comment|/* returns a static buffer */
end_comment

begin_function
DECL|function|get_rev_name
specifier|static
specifier|const
name|char
modifier|*
name|get_rev_name
parameter_list|(
specifier|const
name|struct
name|object
modifier|*
name|o
parameter_list|)
block|{
specifier|static
name|char
name|buffer
index|[
literal|1024
index|]
decl_stmt|;
name|struct
name|rev_name
modifier|*
name|n
decl_stmt|;
name|struct
name|commit
modifier|*
name|c
decl_stmt|;
if|if
condition|(
name|o
operator|->
name|type
operator|!=
name|OBJ_COMMIT
condition|)
return|return
name|get_exact_ref_match
argument_list|(
name|o
argument_list|)
return|;
name|c
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o
expr_stmt|;
name|n
operator|=
name|c
operator|->
name|util
expr_stmt|;
if|if
condition|(
operator|!
name|n
condition|)
return|return
name|NULL
return|;
if|if
condition|(
operator|!
name|n
operator|->
name|generation
condition|)
return|return
name|n
operator|->
name|tip_name
return|;
else|else
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|n
operator|->
name|tip_name
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|>
literal|2
operator|&&
operator|!
name|strcmp
argument_list|(
name|n
operator|->
name|tip_name
operator|+
name|len
operator|-
literal|2
argument_list|,
literal|"^0"
argument_list|)
condition|)
name|len
operator|-=
literal|2
expr_stmt|;
name|snprintf
argument_list|(
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|,
literal|"%.*s~%d"
argument_list|,
name|len
argument_list|,
name|n
operator|->
name|tip_name
argument_list|,
name|n
operator|->
name|generation
argument_list|)
expr_stmt|;
return|return
name|buffer
return|;
block|}
block|}
end_function

begin_function
DECL|function|show_name
specifier|static
name|void
name|show_name
parameter_list|(
specifier|const
name|struct
name|object
modifier|*
name|obj
parameter_list|,
specifier|const
name|char
modifier|*
name|caller_name
parameter_list|,
name|int
name|always
parameter_list|,
name|int
name|allow_undefined
parameter_list|,
name|int
name|name_only
parameter_list|)
block|{
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
init|=
name|obj
operator|->
name|sha1
decl_stmt|;
if|if
condition|(
operator|!
name|name_only
condition|)
name|printf
argument_list|(
literal|"%s "
argument_list|,
name|caller_name
condition|?
name|caller_name
else|:
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|name
operator|=
name|get_rev_name
argument_list|(
name|obj
argument_list|)
expr_stmt|;
if|if
condition|(
name|name
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|name
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|allow_undefined
condition|)
name|printf
argument_list|(
literal|"undefined\n"
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|always
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
literal|"cannot describe '%s'"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|name_rev_usage
specifier|static
name|char
specifier|const
modifier|*
specifier|const
name|name_rev_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git name-rev [<options>]<commit>..."
argument_list|)
block|,
name|N_
argument_list|(
literal|"git name-rev [<options>] --all"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git name-rev [<options>] --stdin"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|name_rev_line
specifier|static
name|void
name|name_rev_line
parameter_list|(
name|char
modifier|*
name|p
parameter_list|,
name|struct
name|name_ref_data
modifier|*
name|data
parameter_list|)
block|{
name|int
name|forty
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|p_start
decl_stmt|;
for|for
control|(
name|p_start
operator|=
name|p
init|;
operator|*
name|p
condition|;
name|p
operator|++
control|)
block|{
DECL|macro|ishex
define|#
directive|define
name|ishex
parameter_list|(
name|x
parameter_list|)
value|(isdigit((x)) || ((x)>= 'a'&& (x)<= 'f'))
if|if
condition|(
operator|!
name|ishex
argument_list|(
operator|*
name|p
argument_list|)
condition|)
name|forty
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
operator|++
name|forty
operator|==
literal|40
operator|&&
operator|!
name|ishex
argument_list|(
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
argument_list|)
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|40
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|name
init|=
name|NULL
decl_stmt|;
name|char
name|c
init|=
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
decl_stmt|;
name|int
name|p_len
init|=
name|p
operator|-
name|p_start
operator|+
literal|1
decl_stmt|;
name|forty
operator|=
literal|0
expr_stmt|;
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
name|get_sha1
argument_list|(
name|p
operator|-
literal|39
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
name|lookup_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|o
condition|)
name|name
operator|=
name|get_rev_name
argument_list|(
name|o
argument_list|)
expr_stmt|;
block|}
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|=
name|c
expr_stmt|;
if|if
condition|(
operator|!
name|name
condition|)
continue|continue;
if|if
condition|(
name|data
operator|->
name|name_only
condition|)
name|printf
argument_list|(
literal|"%.*s%s"
argument_list|,
name|p_len
operator|-
literal|40
argument_list|,
name|p_start
argument_list|,
name|name
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%.*s (%s)"
argument_list|,
name|p_len
argument_list|,
name|p_start
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|p_start
operator|=
name|p
operator|+
literal|1
expr_stmt|;
block|}
block|}
comment|/* flush */
if|if
condition|(
name|p_start
operator|!=
name|p
condition|)
name|fwrite
argument_list|(
name|p_start
argument_list|,
name|p
operator|-
name|p_start
argument_list|,
literal|1
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_name_rev
name|int
name|cmd_name_rev
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
name|object_array
name|revs
init|=
name|OBJECT_ARRAY_INIT
decl_stmt|;
name|int
name|all
init|=
literal|0
decl_stmt|,
name|transform_stdin
init|=
literal|0
decl_stmt|,
name|allow_undefined
init|=
literal|1
decl_stmt|,
name|always
init|=
literal|0
decl_stmt|,
name|peel_tag
init|=
literal|0
decl_stmt|;
name|struct
name|name_ref_data
name|data
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
name|option
name|opts
index|[]
init|=
block|{
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"name-only"
argument_list|,
operator|&
name|data
operator|.
name|name_only
argument_list|,
name|N_
argument_list|(
literal|"print only names (no SHA-1)"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"tags"
argument_list|,
operator|&
name|data
operator|.
name|tags_only
argument_list|,
name|N_
argument_list|(
literal|"only use tags to name the commits"
argument_list|)
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"refs"
argument_list|,
operator|&
name|data
operator|.
name|ref_filter
argument_list|,
name|N_
argument_list|(
literal|"pattern"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"only use refs matching<pattern>"
argument_list|)
argument_list|)
block|,
name|OPT_GROUP
argument_list|(
literal|""
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"all"
argument_list|,
operator|&
name|all
argument_list|,
name|N_
argument_list|(
literal|"list all commits reachable from all refs"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"stdin"
argument_list|,
operator|&
name|transform_stdin
argument_list|,
name|N_
argument_list|(
literal|"read from stdin"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"undefined"
argument_list|,
operator|&
name|allow_undefined
argument_list|,
name|N_
argument_list|(
literal|"allow to print `undefined` names (default)"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"always"
argument_list|,
operator|&
name|always
argument_list|,
name|N_
argument_list|(
literal|"show abbreviated commit object as fallback"
argument_list|)
argument_list|)
block|,
block|{
comment|/* A Hidden OPT_BOOL */
name|OPTION_SET_INT
block|,
literal|0
block|,
literal|"peel-tag"
block|,
operator|&
name|peel_tag
block|,
name|NULL
block|,
name|N_
argument_list|(
literal|"dereference tags in the input (internal use)"
argument_list|)
block|,
name|PARSE_OPT_NOARG
operator||
name|PARSE_OPT_HIDDEN
block|,
name|NULL
block|,
literal|1
block|, 		}
block|,
name|OPT_END
argument_list|()
block|, 	}
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
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
name|prefix
argument_list|,
name|opts
argument_list|,
name|name_rev_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|all
operator|+
name|transform_stdin
operator|+
operator|!
operator|!
name|argc
operator|>
literal|1
condition|)
block|{
name|error
argument_list|(
literal|"Specify either a list, or --all, not both!"
argument_list|)
expr_stmt|;
name|usage_with_options
argument_list|(
name|name_rev_usage
argument_list|,
name|opts
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|all
operator|||
name|transform_stdin
condition|)
name|cutoff
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
name|argc
condition|;
name|argc
operator|--
operator|,
name|argv
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
name|struct
name|object
modifier|*
name|object
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
operator|*
name|argv
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Could not get sha1 for %s. Skipping.\n"
argument_list|,
operator|*
name|argv
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|commit
operator|=
name|NULL
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
name|object
condition|)
block|{
name|struct
name|object
modifier|*
name|peeled
init|=
name|deref_tag
argument_list|(
name|object
argument_list|,
operator|*
name|argv
argument_list|,
literal|0
argument_list|)
decl_stmt|;
if|if
condition|(
name|peeled
operator|&&
name|peeled
operator|->
name|type
operator|==
name|OBJ_COMMIT
condition|)
name|commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|peeled
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|object
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Could not get object for %s. Skipping.\n"
argument_list|,
operator|*
name|argv
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|commit
condition|)
block|{
if|if
condition|(
name|cutoff
operator|>
name|commit
operator|->
name|date
condition|)
name|cutoff
operator|=
name|commit
operator|->
name|date
expr_stmt|;
block|}
if|if
condition|(
name|peel_tag
condition|)
block|{
if|if
condition|(
operator|!
name|commit
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Could not get commit for %s. Skipping.\n"
argument_list|,
operator|*
name|argv
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|object
operator|=
operator|(
expr|struct
name|object
operator|*
operator|)
name|commit
expr_stmt|;
block|}
name|add_object_array
argument_list|(
name|object
argument_list|,
operator|*
name|argv
argument_list|,
operator|&
name|revs
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|cutoff
condition|)
name|cutoff
operator|=
name|cutoff
operator|-
name|CUTOFF_DATE_SLOP
expr_stmt|;
name|for_each_ref
argument_list|(
name|name_ref
argument_list|,
operator|&
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|transform_stdin
condition|)
block|{
name|char
name|buffer
index|[
literal|2048
index|]
decl_stmt|;
while|while
condition|(
operator|!
name|feof
argument_list|(
name|stdin
argument_list|)
condition|)
block|{
name|char
modifier|*
name|p
init|=
name|fgets
argument_list|(
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|,
name|stdin
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|p
condition|)
break|break;
name|name_rev_line
argument_list|(
name|p
argument_list|,
operator|&
name|data
argument_list|)
expr_stmt|;
block|}
block|}
elseif|else
if|if
condition|(
name|all
condition|)
block|{
name|int
name|i
decl_stmt|,
name|max
decl_stmt|;
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
operator|||
name|obj
operator|->
name|type
operator|!=
name|OBJ_COMMIT
condition|)
continue|continue;
name|show_name
argument_list|(
name|obj
argument_list|,
name|NULL
argument_list|,
name|always
argument_list|,
name|allow_undefined
argument_list|,
name|data
operator|.
name|name_only
argument_list|)
expr_stmt|;
block|}
block|}
else|else
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
name|revs
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|show_name
argument_list|(
name|revs
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|item
argument_list|,
name|revs
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|name
argument_list|,
name|always
argument_list|,
name|allow_undefined
argument_list|,
name|data
operator|.
name|name_only
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

