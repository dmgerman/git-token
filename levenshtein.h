begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|LEVENSHTEIN_H
end_ifndef

begin_define
DECL|macro|LEVENSHTEIN_H
define|#
directive|define
name|LEVENSHTEIN_H
end_define

begin_function_decl
name|int
name|levenshtein
parameter_list|(
specifier|const
name|char
modifier|*
name|string1
parameter_list|,
specifier|const
name|char
modifier|*
name|string2
parameter_list|,
name|int
name|swap_penalty
parameter_list|,
name|int
name|substition_penalty
parameter_list|,
name|int
name|insertion_penalty
parameter_list|,
name|int
name|deletion_penalty
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

