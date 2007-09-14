begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|TRANSPORT_H
end_ifndef

begin_define
DECL|macro|TRANSPORT_H
define|#
directive|define
name|TRANSPORT_H
end_define

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"remote.h"
end_include

begin_struct
DECL|struct|transport
struct|struct
name|transport
block|{
DECL|member|verbose
name|unsigned
name|verbose
range|:
literal|1
decl_stmt|;
DECL|member|fetch
name|unsigned
name|fetch
range|:
literal|1
decl_stmt|;
DECL|member|remote
name|struct
name|remote
modifier|*
name|remote
decl_stmt|;
DECL|member|url
specifier|const
name|char
modifier|*
name|url
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|remote_refs
name|struct
name|ref
modifier|*
name|remote_refs
decl_stmt|;
DECL|member|ops
specifier|const
name|struct
name|transport_ops
modifier|*
name|ops
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|TRANSPORT_PUSH_ALL
define|#
directive|define
name|TRANSPORT_PUSH_ALL
value|1
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_FORCE
define|#
directive|define
name|TRANSPORT_PUSH_FORCE
value|2
end_define

begin_struct
DECL|struct|transport_ops
struct|struct
name|transport_ops
block|{
comment|/** 	 * Returns 0 if successful, positive if the option is not 	 * recognized or is inapplicable, and negative if the option 	 * is applicable but the value is invalid. 	 **/
DECL|member|set_option
name|int
function_decl|(
modifier|*
name|set_option
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
DECL|member|get_refs_list
name|struct
name|ref
modifier|*
function_decl|(
modifier|*
name|get_refs_list
function_decl|)
parameter_list|(
specifier|const
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
function_decl|;
DECL|member|fetch
name|int
function_decl|(
modifier|*
name|fetch
function_decl|)
parameter_list|(
specifier|const
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|refs_nr
parameter_list|,
name|struct
name|ref
modifier|*
modifier|*
name|refs
parameter_list|)
function_decl|;
DECL|member|push
name|int
function_decl|(
modifier|*
name|push
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
parameter_list|,
name|int
name|refspec_nr
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspec
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
DECL|member|disconnect
name|int
function_decl|(
modifier|*
name|disconnect
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
parameter_list|)
function_decl|;
block|}
struct|;
end_struct

begin_comment
comment|/* Returns a transport suitable for the url */
end_comment

begin_function_decl
name|struct
name|transport
modifier|*
name|transport_get
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
specifier|const
name|char
modifier|*
name|url
parameter_list|,
name|int
name|fetch
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Transport options which apply to git:// and scp-style URLs */
end_comment

begin_comment
comment|/* The program to use on the remote side to send a pack */
end_comment

begin_define
DECL|macro|TRANS_OPT_UPLOADPACK
define|#
directive|define
name|TRANS_OPT_UPLOADPACK
value|"uploadpack"
end_define

begin_comment
comment|/* The program to use on the remote side to receive a pack */
end_comment

begin_define
DECL|macro|TRANS_OPT_RECEIVEPACK
define|#
directive|define
name|TRANS_OPT_RECEIVEPACK
value|"receivepack"
end_define

begin_comment
comment|/* Transfer the data as a thin pack if not null */
end_comment

begin_define
DECL|macro|TRANS_OPT_THIN
define|#
directive|define
name|TRANS_OPT_THIN
value|"thin"
end_define

begin_comment
comment|/* Keep the pack that was transferred if not null */
end_comment

begin_define
DECL|macro|TRANS_OPT_KEEP
define|#
directive|define
name|TRANS_OPT_KEEP
value|"keep"
end_define

begin_comment
comment|/* Unpack the objects if fewer than this number of objects are fetched */
end_comment

begin_define
DECL|macro|TRANS_OPT_UNPACKLIMIT
define|#
directive|define
name|TRANS_OPT_UNPACKLIMIT
value|"unpacklimit"
end_define

begin_comment
comment|/* Limit the depth of the fetch if not null */
end_comment

begin_define
DECL|macro|TRANS_OPT_DEPTH
define|#
directive|define
name|TRANS_OPT_DEPTH
value|"depth"
end_define

begin_comment
comment|/**  * Returns 0 if the option was used, non-zero otherwise. Prints a  * message to stderr if the option is not used.  **/
end_comment

begin_function_decl
name|int
name|transport_set_option
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|transport_push
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
parameter_list|,
name|int
name|refspec_nr
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspec
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|ref
modifier|*
name|transport_get_remote_refs
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|transport_fetch_refs
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|struct
name|ref
modifier|*
name|refs
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|transport_disconnect
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

