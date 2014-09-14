begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2011, Google Inc.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|BULK_CHECKIN_H
end_ifndef

begin_define
DECL|macro|BULK_CHECKIN_H
define|#
directive|define
name|BULK_CHECKIN_H
end_define

begin_function_decl
specifier|extern
name|int
name|index_bulk_checkin
parameter_list|(
name|unsigned
name|char
name|sha1
index|[]
parameter_list|,
name|int
name|fd
parameter_list|,
name|size_t
name|size
parameter_list|,
name|enum
name|object_type
name|type
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|plug_bulk_checkin
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|unplug_bulk_checkin
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

