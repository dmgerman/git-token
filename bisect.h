begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|BISECT_H
end_ifndef

begin_define
DECL|macro|BISECT_H
define|#
directive|define
name|BISECT_H
end_define

begin_function_decl
specifier|extern
name|struct
name|commit_list
modifier|*
name|find_bisection
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|,
name|int
modifier|*
name|reaches
parameter_list|,
name|int
modifier|*
name|all
parameter_list|,
name|int
name|find_all
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|struct
name|commit_list
modifier|*
name|filter_skipped
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|,
name|struct
name|commit_list
modifier|*
modifier|*
name|tried
parameter_list|,
name|int
name|show_all
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* show_bisect_vars flags */
end_comment

begin_define
DECL|macro|BISECT_SHOW_ALL
define|#
directive|define
name|BISECT_SHOW_ALL
value|(1<<0)
end_define

begin_define
DECL|macro|BISECT_SHOW_TRIED
define|#
directive|define
name|BISECT_SHOW_TRIED
value|(1<<1)
end_define

begin_define
DECL|macro|BISECT_SHOW_STRINGED
define|#
directive|define
name|BISECT_SHOW_STRINGED
value|(1<<2)
end_define

begin_comment
comment|/*  * The flag BISECT_SHOW_ALL should not be set if this function is called  * from outside "builtin-rev-list.c" as otherwise it would use  * static "revs" from this file.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|show_bisect_vars
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|int
name|reaches
parameter_list|,
name|int
name|all
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|bisect_next_vars
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

