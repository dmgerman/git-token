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
parameter_list|,
name|int
modifier|*
name|count
parameter_list|,
name|int
modifier|*
name|skipped_first
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|BISECT_SHOW_ALL
define|#
directive|define
name|BISECT_SHOW_ALL
value|(1<<0)
end_define

begin_define
DECL|macro|REV_LIST_QUIET
define|#
directive|define
name|REV_LIST_QUIET
value|(1<<1)
end_define

begin_struct
DECL|struct|rev_list_info
struct|struct
name|rev_list_info
block|{
DECL|member|revs
name|struct
name|rev_info
modifier|*
name|revs
decl_stmt|;
DECL|member|flags
name|int
name|flags
decl_stmt|;
DECL|member|show_timestamp
name|int
name|show_timestamp
decl_stmt|;
DECL|member|hdr_termination
name|int
name|hdr_termination
decl_stmt|;
DECL|member|header_prefix
specifier|const
name|char
modifier|*
name|header_prefix
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|int
name|bisect_next_all
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|no_checkout
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|estimate_bisect_steps
parameter_list|(
name|int
name|all
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|read_bisect_terms
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|bad
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|good
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

