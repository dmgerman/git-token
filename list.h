begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002 Free Software Foundation, Inc.  * (originally part of the GNU C Library and Userspace RCU)  * Contributed by Ulrich Drepper<drepper@redhat.com>, 2002.  *  * Copyright (C) 2009 Pierre-Marc Fournier  * Conversion to RCU list.  * Copyright (C) 2010 Mathieu Desnoyers<mathieu.desnoyers@efficios.com>  *  * This library is free software; you can redistribute it and/or  * modify it under the terms of the GNU Lesser General Public  * License as published by the Free Software Foundation; either  * version 2.1 of the License, or (at your option) any later version.  *  * This library is distributed in the hope that it will be useful,  * but WITHOUT ANY WARRANTY; without even the implied warranty of  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  * Lesser General Public License for more details.  *  * You should have received a copy of the GNU Lesser General Public  * License along with this library; if not, see  *<http://www.gnu.org/licenses/>.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|LIST_H
end_ifndef

begin_define
DECL|macro|LIST_H
define|#
directive|define
name|LIST_H
value|1
end_define

begin_comment
comment|/*  * The definitions of this file are adopted from those which can be  * found in the Linux kernel headers to enable people familiar with the  * latter find their way in these sources as well.  */
end_comment

begin_comment
comment|/* Basic type for the double-link list. */
end_comment

begin_struct
DECL|struct|list_head
struct|struct
name|list_head
block|{
DECL|member|next
DECL|member|prev
name|struct
name|list_head
modifier|*
name|next
decl_stmt|,
modifier|*
name|prev
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* avoid conflicts with BSD-only sys/queue.h */
end_comment

begin_undef
DECL|macro|LIST_HEAD
undef|#
directive|undef
name|LIST_HEAD
end_undef

begin_comment
comment|/* Define a variable with the head and tail of the list. */
end_comment

begin_define
DECL|macro|LIST_HEAD
define|#
directive|define
name|LIST_HEAD
parameter_list|(
name|name
parameter_list|)
define|\
value|struct list_head name = {&(name),&(name) }
end_define

begin_comment
comment|/* Initialize a new list head. */
end_comment

begin_define
DECL|macro|INIT_LIST_HEAD
define|#
directive|define
name|INIT_LIST_HEAD
parameter_list|(
name|ptr
parameter_list|)
define|\
value|(ptr)->next = (ptr)->prev = (ptr)
end_define

begin_define
DECL|macro|LIST_HEAD_INIT
define|#
directive|define
name|LIST_HEAD_INIT
parameter_list|(
name|name
parameter_list|)
value|{&(name),&(name) }
end_define

begin_comment
comment|/* Add new element at the head of the list. */
end_comment

begin_function
DECL|function|list_add
specifier|static
specifier|inline
name|void
name|list_add
parameter_list|(
name|struct
name|list_head
modifier|*
name|newp
parameter_list|,
name|struct
name|list_head
modifier|*
name|head
parameter_list|)
block|{
name|head
operator|->
name|next
operator|->
name|prev
operator|=
name|newp
expr_stmt|;
name|newp
operator|->
name|next
operator|=
name|head
operator|->
name|next
expr_stmt|;
name|newp
operator|->
name|prev
operator|=
name|head
expr_stmt|;
name|head
operator|->
name|next
operator|=
name|newp
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Add new element at the tail of the list. */
end_comment

begin_function
DECL|function|list_add_tail
specifier|static
specifier|inline
name|void
name|list_add_tail
parameter_list|(
name|struct
name|list_head
modifier|*
name|newp
parameter_list|,
name|struct
name|list_head
modifier|*
name|head
parameter_list|)
block|{
name|head
operator|->
name|prev
operator|->
name|next
operator|=
name|newp
expr_stmt|;
name|newp
operator|->
name|next
operator|=
name|head
expr_stmt|;
name|newp
operator|->
name|prev
operator|=
name|head
operator|->
name|prev
expr_stmt|;
name|head
operator|->
name|prev
operator|=
name|newp
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Remove element from list. */
end_comment

begin_function
DECL|function|__list_del
specifier|static
specifier|inline
name|void
name|__list_del
parameter_list|(
name|struct
name|list_head
modifier|*
name|prev
parameter_list|,
name|struct
name|list_head
modifier|*
name|next
parameter_list|)
block|{
name|next
operator|->
name|prev
operator|=
name|prev
expr_stmt|;
name|prev
operator|->
name|next
operator|=
name|next
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Remove element from list. */
end_comment

begin_function
DECL|function|list_del
specifier|static
specifier|inline
name|void
name|list_del
parameter_list|(
name|struct
name|list_head
modifier|*
name|elem
parameter_list|)
block|{
name|__list_del
argument_list|(
name|elem
operator|->
name|prev
argument_list|,
name|elem
operator|->
name|next
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Remove element from list, initializing the element's list pointers. */
end_comment

begin_function
DECL|function|list_del_init
specifier|static
specifier|inline
name|void
name|list_del_init
parameter_list|(
name|struct
name|list_head
modifier|*
name|elem
parameter_list|)
block|{
name|list_del
argument_list|(
name|elem
argument_list|)
expr_stmt|;
name|INIT_LIST_HEAD
argument_list|(
name|elem
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Delete from list, add to another list as head. */
end_comment

begin_function
DECL|function|list_move
specifier|static
specifier|inline
name|void
name|list_move
parameter_list|(
name|struct
name|list_head
modifier|*
name|elem
parameter_list|,
name|struct
name|list_head
modifier|*
name|head
parameter_list|)
block|{
name|__list_del
argument_list|(
name|elem
operator|->
name|prev
argument_list|,
name|elem
operator|->
name|next
argument_list|)
expr_stmt|;
name|list_add
argument_list|(
name|elem
argument_list|,
name|head
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Replace an old entry. */
end_comment

begin_function
DECL|function|list_replace
specifier|static
specifier|inline
name|void
name|list_replace
parameter_list|(
name|struct
name|list_head
modifier|*
name|old
parameter_list|,
name|struct
name|list_head
modifier|*
name|newp
parameter_list|)
block|{
name|newp
operator|->
name|next
operator|=
name|old
operator|->
name|next
expr_stmt|;
name|newp
operator|->
name|prev
operator|=
name|old
operator|->
name|prev
expr_stmt|;
name|newp
operator|->
name|prev
operator|->
name|next
operator|=
name|newp
expr_stmt|;
name|newp
operator|->
name|next
operator|->
name|prev
operator|=
name|newp
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Join two lists. */
end_comment

begin_function
DECL|function|list_splice
specifier|static
specifier|inline
name|void
name|list_splice
parameter_list|(
name|struct
name|list_head
modifier|*
name|add
parameter_list|,
name|struct
name|list_head
modifier|*
name|head
parameter_list|)
block|{
comment|/* Do nothing if the list which gets added is empty. */
if|if
condition|(
name|add
operator|!=
name|add
operator|->
name|next
condition|)
block|{
name|add
operator|->
name|next
operator|->
name|prev
operator|=
name|head
expr_stmt|;
name|add
operator|->
name|prev
operator|->
name|next
operator|=
name|head
operator|->
name|next
expr_stmt|;
name|head
operator|->
name|next
operator|->
name|prev
operator|=
name|add
operator|->
name|prev
expr_stmt|;
name|head
operator|->
name|next
operator|=
name|add
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/* Get typed element from list at a given position. */
end_comment

begin_define
DECL|macro|list_entry
define|#
directive|define
name|list_entry
parameter_list|(
name|ptr
parameter_list|,
name|type
parameter_list|,
name|member
parameter_list|)
define|\
value|((type *) ((char *) (ptr) - offsetof(type, member)))
end_define

begin_comment
comment|/* Get first entry from a list. */
end_comment

begin_define
DECL|macro|list_first_entry
define|#
directive|define
name|list_first_entry
parameter_list|(
name|ptr
parameter_list|,
name|type
parameter_list|,
name|member
parameter_list|)
define|\
value|list_entry((ptr)->next, type, member)
end_define

begin_comment
comment|/* Iterate forward over the elements of the list. */
end_comment

begin_define
DECL|macro|list_for_each
define|#
directive|define
name|list_for_each
parameter_list|(
name|pos
parameter_list|,
name|head
parameter_list|)
define|\
value|for (pos = (head)->next; pos != (head); pos = pos->next)
end_define

begin_comment
comment|/*  * Iterate forward over the elements list. The list elements can be  * removed from the list while doing this.  */
end_comment

begin_define
DECL|macro|list_for_each_safe
define|#
directive|define
name|list_for_each_safe
parameter_list|(
name|pos
parameter_list|,
name|p
parameter_list|,
name|head
parameter_list|)
define|\
value|for (pos = (head)->next, p = pos->next; \ 		pos != (head); \ 		pos = p, p = pos->next)
end_define

begin_comment
comment|/* Iterate backward over the elements of the list. */
end_comment

begin_define
DECL|macro|list_for_each_prev
define|#
directive|define
name|list_for_each_prev
parameter_list|(
name|pos
parameter_list|,
name|head
parameter_list|)
define|\
value|for (pos = (head)->prev; pos != (head); pos = pos->prev)
end_define

begin_comment
comment|/*  * Iterate backwards over the elements list. The list elements can be  * removed from the list while doing this.  */
end_comment

begin_define
DECL|macro|list_for_each_prev_safe
define|#
directive|define
name|list_for_each_prev_safe
parameter_list|(
name|pos
parameter_list|,
name|p
parameter_list|,
name|head
parameter_list|)
define|\
value|for (pos = (head)->prev, p = pos->prev; \ 		pos != (head); \ 		pos = p, p = pos->prev)
end_define

begin_function
DECL|function|list_empty
specifier|static
specifier|inline
name|int
name|list_empty
parameter_list|(
name|struct
name|list_head
modifier|*
name|head
parameter_list|)
block|{
return|return
name|head
operator|==
name|head
operator|->
name|next
return|;
block|}
end_function

begin_function
DECL|function|list_replace_init
specifier|static
specifier|inline
name|void
name|list_replace_init
parameter_list|(
name|struct
name|list_head
modifier|*
name|old
parameter_list|,
name|struct
name|list_head
modifier|*
name|newp
parameter_list|)
block|{
name|struct
name|list_head
modifier|*
name|head
init|=
name|old
operator|->
name|next
decl_stmt|;
name|list_del
argument_list|(
name|old
argument_list|)
expr_stmt|;
name|list_add_tail
argument_list|(
name|newp
argument_list|,
name|head
argument_list|)
expr_stmt|;
name|INIT_LIST_HEAD
argument_list|(
name|old
argument_list|)
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* LIST_H */
end_comment

end_unit

