begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PACK_REFS_H
end_ifndef

begin_define
DECL|macro|PACK_REFS_H
define|#
directive|define
name|PACK_REFS_H
end_define

begin_comment
comment|/*  * Flags for controlling behaviour of pack_refs()  * PACK_REFS_PRUNE: Prune loose refs after packing  * PACK_REFS_ALL:   Pack _all_ refs, not just tags and already packed refs  */
end_comment

begin_define
DECL|macro|PACK_REFS_PRUNE
define|#
directive|define
name|PACK_REFS_PRUNE
value|0x0001
end_define

begin_define
DECL|macro|PACK_REFS_ALL
define|#
directive|define
name|PACK_REFS_ALL
value|0x0002
end_define

begin_comment
comment|/*  * Write a packed-refs file for the current repository.  * flags: Combination of the above PACK_REFS_* flags.  */
end_comment

begin_function_decl
name|int
name|pack_refs
parameter_list|(
name|unsigned
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PACK_REFS_H */
end_comment

end_unit

