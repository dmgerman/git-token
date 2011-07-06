begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Licensed under a two-clause BSD-style license.  * See LICENSE for details.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|OBJ_POOL_H_
end_ifndef

begin_define
DECL|macro|OBJ_POOL_H_
define|#
directive|define
name|OBJ_POOL_H_
end_define

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_define
DECL|macro|MAYBE_UNUSED
define|#
directive|define
name|MAYBE_UNUSED
value|__attribute__((__unused__))
end_define

begin_define
DECL|macro|obj_pool_gen
define|#
directive|define
name|obj_pool_gen
parameter_list|(
name|pre
parameter_list|,
name|obj_t
parameter_list|,
name|initial_capacity
parameter_list|)
define|\
value|static struct { \ 	uint32_t committed; \ 	uint32_t size; \ 	uint32_t capacity; \ 	obj_t *base; \ } pre##_pool = {0, 0, 0, NULL}; \ static MAYBE_UNUSED uint32_t pre##_alloc(uint32_t count) \ { \ 	uint32_t offset; \ 	if (pre##_pool.size + count> pre##_pool.capacity) { \ 		while (pre##_pool.size + count> pre##_pool.capacity) \ 			if (pre##_pool.capacity) \ 				pre##_pool.capacity *= 2; \ 			else \ 				pre##_pool.capacity = initial_capacity; \ 		pre##_pool.base = realloc(pre##_pool.base, \ 					pre##_pool.capacity * sizeof(obj_t)); \ 	} \ 	offset = pre##_pool.size; \ 	pre##_pool.size += count; \ 	return offset; \ } \ static MAYBE_UNUSED void pre##_free(uint32_t count) \ { \ 	pre##_pool.size -= count; \ } \ static MAYBE_UNUSED uint32_t pre##_offset(obj_t *obj) \ { \ 	return obj == NULL ? ~0 : obj - pre##_pool.base; \ } \ static MAYBE_UNUSED obj_t *pre##_pointer(uint32_t offset) \ { \ 	return offset>= pre##_pool.size ? NULL :&pre##_pool.base[offset]; \ } \ static MAYBE_UNUSED void pre##_commit(void) \ { \ 	pre##_pool.committed = pre##_pool.size; \ } \ static MAYBE_UNUSED void pre##_reset(void) \ { \ 	free(pre##_pool.base); \ 	pre##_pool.base = NULL; \ 	pre##_pool.size = 0; \ 	pre##_pool.capacity = 0; \ 	pre##_pool.committed = 0; \ }
end_define

begin_endif
endif|#
directive|endif
end_endif

end_unit

