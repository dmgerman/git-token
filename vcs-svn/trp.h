begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * C macro implementation of treaps.  *  * Usage:  *   #include<stdint.h>  *   #include "trp.h"  *   trp_gen(...)  *  * Licensed under a two-clause BSD-style license.  * See LICENSE for details.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|TRP_H_
end_ifndef

begin_define
DECL|macro|TRP_H_
define|#
directive|define
name|TRP_H_
end_define

begin_define
DECL|macro|MAYBE_UNUSED
define|#
directive|define
name|MAYBE_UNUSED
value|__attribute__((__unused__))
end_define

begin_comment
comment|/* Node structure. */
end_comment

begin_struct
DECL|struct|trp_node
struct|struct
name|trp_node
block|{
DECL|member|trpn_left
name|uint32_t
name|trpn_left
decl_stmt|;
DECL|member|trpn_right
name|uint32_t
name|trpn_right
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* Root structure. */
end_comment

begin_struct
DECL|struct|trp_root
struct|struct
name|trp_root
block|{
DECL|member|trp_root
name|uint32_t
name|trp_root
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* Pointer/Offset conversion. */
end_comment

begin_define
DECL|macro|trpn_pointer
define|#
directive|define
name|trpn_pointer
parameter_list|(
name|a_base
parameter_list|,
name|a_offset
parameter_list|)
value|(a_base##_pointer(a_offset))
end_define

begin_define
DECL|macro|trpn_offset
define|#
directive|define
name|trpn_offset
parameter_list|(
name|a_base
parameter_list|,
name|a_pointer
parameter_list|)
value|(a_base##_offset(a_pointer))
end_define

begin_define
DECL|macro|trpn_modify
define|#
directive|define
name|trpn_modify
parameter_list|(
name|a_base
parameter_list|,
name|a_offset
parameter_list|)
define|\
value|do { \ 		if ((a_offset)< a_base##_pool.committed) { \ 			uint32_t old_offset = (a_offset);\ 			(a_offset) = a_base##_alloc(1); \ 			*trpn_pointer(a_base, a_offset) = \ 				*trpn_pointer(a_base, old_offset); \ 		} \ 	} while (0)
end_define

begin_comment
comment|/* Left accessors. */
end_comment

begin_define
DECL|macro|trp_left_get
define|#
directive|define
name|trp_left_get
parameter_list|(
name|a_base
parameter_list|,
name|a_field
parameter_list|,
name|a_node
parameter_list|)
define|\
value|(trpn_pointer(a_base, a_node)->a_field.trpn_left)
end_define

begin_define
DECL|macro|trp_left_set
define|#
directive|define
name|trp_left_set
parameter_list|(
name|a_base
parameter_list|,
name|a_field
parameter_list|,
name|a_node
parameter_list|,
name|a_left
parameter_list|)
define|\
value|do { \ 		trpn_modify(a_base, a_node); \ 		trp_left_get(a_base, a_field, a_node) = (a_left); \ 	} while (0)
end_define

begin_comment
comment|/* Right accessors. */
end_comment

begin_define
DECL|macro|trp_right_get
define|#
directive|define
name|trp_right_get
parameter_list|(
name|a_base
parameter_list|,
name|a_field
parameter_list|,
name|a_node
parameter_list|)
define|\
value|(trpn_pointer(a_base, a_node)->a_field.trpn_right)
end_define

begin_define
DECL|macro|trp_right_set
define|#
directive|define
name|trp_right_set
parameter_list|(
name|a_base
parameter_list|,
name|a_field
parameter_list|,
name|a_node
parameter_list|,
name|a_right
parameter_list|)
define|\
value|do { \ 		trpn_modify(a_base, a_node); \ 		trp_right_get(a_base, a_field, a_node) = (a_right); \ 	} while (0)
end_define

begin_comment
comment|/*  * Fibonacci hash function.  * The multiplier is the nearest prime to (2^32 times (â5 - 1)/2).  * See Knuth Â§6.4: volume 3, 3rd ed, p518.  */
end_comment

begin_define
DECL|macro|trpn_hash
define|#
directive|define
name|trpn_hash
parameter_list|(
name|a_node
parameter_list|)
value|(uint32_t) (2654435761u * (a_node))
end_define

begin_comment
comment|/* Priority accessors. */
end_comment

begin_define
DECL|macro|trp_prio_get
define|#
directive|define
name|trp_prio_get
parameter_list|(
name|a_node
parameter_list|)
value|trpn_hash(a_node)
end_define

begin_comment
comment|/* Node initializer. */
end_comment

begin_define
DECL|macro|trp_node_new
define|#
directive|define
name|trp_node_new
parameter_list|(
name|a_base
parameter_list|,
name|a_field
parameter_list|,
name|a_node
parameter_list|)
define|\
value|do { \ 		trp_left_set(a_base, a_field, (a_node), ~0); \ 		trp_right_set(a_base, a_field, (a_node), ~0); \ 	} while (0)
end_define

begin_comment
comment|/* Internal utility macros. */
end_comment

begin_define
DECL|macro|trpn_first
define|#
directive|define
name|trpn_first
parameter_list|(
name|a_base
parameter_list|,
name|a_field
parameter_list|,
name|a_root
parameter_list|,
name|r_node
parameter_list|)
define|\
value|do { \ 		(r_node) = (a_root); \ 		if ((r_node) == ~0) \ 			return NULL; \ 		while (~trp_left_get(a_base, a_field, (r_node))) \ 			(r_node) = trp_left_get(a_base, a_field, (r_node)); \ 	} while (0)
end_define

begin_define
DECL|macro|trpn_rotate_left
define|#
directive|define
name|trpn_rotate_left
parameter_list|(
name|a_base
parameter_list|,
name|a_field
parameter_list|,
name|a_node
parameter_list|,
name|r_node
parameter_list|)
define|\
value|do { \ 		(r_node) = trp_right_get(a_base, a_field, (a_node)); \ 		trp_right_set(a_base, a_field, (a_node), \ 			trp_left_get(a_base, a_field, (r_node))); \ 		trp_left_set(a_base, a_field, (r_node), (a_node)); \ 	} while (0)
end_define

begin_define
DECL|macro|trpn_rotate_right
define|#
directive|define
name|trpn_rotate_right
parameter_list|(
name|a_base
parameter_list|,
name|a_field
parameter_list|,
name|a_node
parameter_list|,
name|r_node
parameter_list|)
define|\
value|do { \ 		(r_node) = trp_left_get(a_base, a_field, (a_node)); \ 		trp_left_set(a_base, a_field, (a_node), \ 			trp_right_get(a_base, a_field, (r_node))); \ 		trp_right_set(a_base, a_field, (r_node), (a_node)); \ 	} while (0)
end_define

begin_define
DECL|macro|trp_gen
define|#
directive|define
name|trp_gen
parameter_list|(
name|a_attr
parameter_list|,
name|a_pre
parameter_list|,
name|a_type
parameter_list|,
name|a_field
parameter_list|,
name|a_base
parameter_list|,
name|a_cmp
parameter_list|)
define|\
value|a_attr a_type MAYBE_UNUSED *a_pre##first(struct trp_root *treap) \ { \ 	uint32_t ret; \ 	trpn_first(a_base, a_field, treap->trp_root, ret); \ 	return trpn_pointer(a_base, ret); \ } \ a_attr a_type MAYBE_UNUSED *a_pre##next(struct trp_root *treap, a_type *node) \ { \ 	uint32_t ret; \ 	uint32_t offset = trpn_offset(a_base, node); \ 	if (~trp_right_get(a_base, a_field, offset)) { \ 		trpn_first(a_base, a_field, \ 			trp_right_get(a_base, a_field, offset), ret); \ 	} else { \ 		uint32_t tnode = treap->trp_root; \ 		ret = ~0; \ 		while (1) { \ 			int cmp = (a_cmp)(trpn_pointer(a_base, offset), \ 				trpn_pointer(a_base, tnode)); \ 			if (cmp< 0) { \ 				ret = tnode; \ 				tnode = trp_left_get(a_base, a_field, tnode); \ 			} else if (cmp> 0) { \ 				tnode = trp_right_get(a_base, a_field, tnode); \ 			} else { \ 				break; \ 			} \ 		} \ 	} \ 	return trpn_pointer(a_base, ret); \ } \ a_attr a_type MAYBE_UNUSED *a_pre##search(struct trp_root *treap, a_type *key) \ { \ 	int cmp; \ 	uint32_t ret = treap->trp_root; \ 	while (~ret&& (cmp = (a_cmp)(key, trpn_pointer(a_base, ret)))) { \ 		if (cmp< 0) { \ 			ret = trp_left_get(a_base, a_field, ret); \ 		} else { \ 			ret = trp_right_get(a_base, a_field, ret); \ 		} \ 	} \ 	return trpn_pointer(a_base, ret); \ } \ a_attr a_type MAYBE_UNUSED *a_pre##nsearch(struct trp_root *treap, a_type *key) \ { \ 	int cmp; \ 	uint32_t ret = treap->trp_root; \ 	while (~ret&& (cmp = (a_cmp)(key, trpn_pointer(a_base, ret)))) { \ 		if (cmp< 0) { \ 			if (!~trp_left_get(a_base, a_field, ret)) \ 				break; \ 			ret = trp_left_get(a_base, a_field, ret); \ 		} else { \ 			ret = trp_right_get(a_base, a_field, ret); \ 		} \ 	} \ 	return trpn_pointer(a_base, ret); \ } \ a_attr uint32_t MAYBE_UNUSED a_pre##insert_recurse(uint32_t cur_node, uint32_t ins_node) \ { \ 	if (cur_node == ~0) { \ 		return ins_node; \ 	} else { \ 		uint32_t ret; \ 		int cmp = (a_cmp)(trpn_pointer(a_base, ins_node), \ 					trpn_pointer(a_base, cur_node)); \ 		if (cmp< 0) { \ 			uint32_t left = a_pre##insert_recurse( \ 				trp_left_get(a_base, a_field, cur_node), ins_node); \ 			trp_left_set(a_base, a_field, cur_node, left); \ 			if (trp_prio_get(left)< trp_prio_get(cur_node)) \ 				trpn_rotate_right(a_base, a_field, cur_node, ret); \ 			else \ 				ret = cur_node; \ 		} else { \ 			uint32_t right = a_pre##insert_recurse( \ 				trp_right_get(a_base, a_field, cur_node), ins_node); \ 			trp_right_set(a_base, a_field, cur_node, right); \ 			if (trp_prio_get(right)< trp_prio_get(cur_node)) \ 				trpn_rotate_left(a_base, a_field, cur_node, ret); \ 			else \ 				ret = cur_node; \ 		} \ 		return ret; \ 	} \ } \ a_attr void MAYBE_UNUSED a_pre##insert(struct trp_root *treap, a_type *node) \ { \ 	uint32_t offset = trpn_offset(a_base, node); \ 	trp_node_new(a_base, a_field, offset); \ 	treap->trp_root = a_pre##insert_recurse(treap->trp_root, offset); \ } \ a_attr uint32_t MAYBE_UNUSED a_pre##remove_recurse(uint32_t cur_node, uint32_t rem_node) \ { \ 	int cmp = a_cmp(trpn_pointer(a_base, rem_node), \ 			trpn_pointer(a_base, cur_node)); \ 	if (cmp == 0) { \ 		uint32_t ret; \ 		uint32_t left = trp_left_get(a_base, a_field, cur_node); \ 		uint32_t right = trp_right_get(a_base, a_field, cur_node); \ 		if (left == ~0) { \ 			if (right == ~0) \ 				return ~0; \ 		} else if (right == ~0 || trp_prio_get(left)< trp_prio_get(right)) { \ 			trpn_rotate_right(a_base, a_field, cur_node, ret); \ 			right = a_pre##remove_recurse(cur_node, rem_node); \ 			trp_right_set(a_base, a_field, ret, right); \ 			return ret; \ 		} \ 		trpn_rotate_left(a_base, a_field, cur_node, ret); \ 		left = a_pre##remove_recurse(cur_node, rem_node); \ 		trp_left_set(a_base, a_field, ret, left); \ 		return ret; \ 	} else if (cmp< 0) { \ 		uint32_t left = a_pre##remove_recurse( \ 			trp_left_get(a_base, a_field, cur_node), rem_node); \ 		trp_left_set(a_base, a_field, cur_node, left); \ 		return cur_node; \ 	} else { \ 		uint32_t right = a_pre##remove_recurse( \ 			trp_right_get(a_base, a_field, cur_node), rem_node); \ 		trp_right_set(a_base, a_field, cur_node, right); \ 		return cur_node; \ 	} \ } \ a_attr void MAYBE_UNUSED a_pre##remove(struct trp_root *treap, a_type *node) \ { \ 	treap->trp_root = a_pre##remove_recurse(treap->trp_root, \ 		trpn_offset(a_base, node)); \ } \  #endif
end_define

end_unit

