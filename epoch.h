begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|EPOCH_H
end_ifndef

begin_define
DECL|macro|EPOCH_H
define|#
directive|define
name|EPOCH_H
end_define

begin_comment
comment|// return codes for emitter_func
end_comment

begin_define
DECL|macro|STOP
define|#
directive|define
name|STOP
value|0
end_define

begin_define
DECL|macro|CONTINUE
define|#
directive|define
name|CONTINUE
value|1
end_define

begin_define
DECL|macro|DO
define|#
directive|define
name|DO
value|2
end_define

begin_typedef
DECL|typedef|emitter_func
typedef|typedef
name|int
function_decl|(
modifier|*
name|emitter_func
function_decl|)
parameter_list|(
name|struct
name|commit
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|int
name|sort_list_in_merge_order
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|,
name|emitter_func
name|emitter
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Low bits are used by rev-list */
end_comment

begin_define
DECL|macro|UNINTERESTING
define|#
directive|define
name|UNINTERESTING
value|(1u<<10)
end_define

begin_define
DECL|macro|BOUNDARY
define|#
directive|define
name|BOUNDARY
value|(1u<<11)
end_define

begin_define
DECL|macro|VISITED
define|#
directive|define
name|VISITED
value|(1u<<12)
end_define

begin_define
DECL|macro|DISCONTINUITY
define|#
directive|define
name|DISCONTINUITY
value|(1u<<13)
end_define

begin_define
DECL|macro|LAST_EPOCH_FLAG
define|#
directive|define
name|LAST_EPOCH_FLAG
value|(1u<<14)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* EPOCH_H */
end_comment

end_unit

