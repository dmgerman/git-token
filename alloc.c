begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * alloc.c  - specialized allocator for internal objects  *  * Copyright (C) 2006 Linus Torvalds  *  * The standard malloc/free wastes too much space for objects, partly because  * it maintains all the allocation infrastructure (which isn't needed, since  * we never free an object descriptor anyway), but even more because it ends  * up with maximal alignment because it doesn't know what the object alignment  * for the new allocation is.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_define
DECL|macro|BLOCKING
define|#
directive|define
name|BLOCKING
value|1024
end_define

begin_define
DECL|macro|DEFINE_ALLOCATOR
define|#
directive|define
name|DEFINE_ALLOCATOR
parameter_list|(
name|name
parameter_list|,
name|type
parameter_list|)
define|\
value|static unsigned int name##_allocs;				\ void *alloc_##name##_node(void)					\ {								\ 	static int nr;						\ 	static type *block;					\ 	void *ret;						\ 								\ 	if (!nr) {						\ 		nr = BLOCKING;					\ 		block = xmalloc(BLOCKING * sizeof(type));	\ 	}							\ 	nr--;							\ 	name##_allocs++;					\ 	ret = block++;						\ 	memset(ret, 0, sizeof(type));				\ 	return ret;						\ }
end_define

begin_union
DECL|union|any_object
union|union
name|any_object
block|{
DECL|member|object
name|struct
name|object
name|object
decl_stmt|;
DECL|member|blob
name|struct
name|blob
name|blob
decl_stmt|;
DECL|member|tree
name|struct
name|tree
name|tree
decl_stmt|;
DECL|member|commit
name|struct
name|commit
name|commit
decl_stmt|;
DECL|member|tag
name|struct
name|tag
name|tag
decl_stmt|;
block|}
union|;
end_union

begin_macro
name|DEFINE_ALLOCATOR
argument_list|(
argument|blob
argument_list|,
argument|struct blob
argument_list|)
end_macro

begin_macro
name|DEFINE_ALLOCATOR
argument_list|(
argument|tree
argument_list|,
argument|struct tree
argument_list|)
end_macro

begin_macro
name|DEFINE_ALLOCATOR
argument_list|(
argument|commit
argument_list|,
argument|struct commit
argument_list|)
end_macro

begin_macro
name|DEFINE_ALLOCATOR
argument_list|(
argument|tag
argument_list|,
argument|struct tag
argument_list|)
end_macro

begin_macro
name|DEFINE_ALLOCATOR
argument_list|(
argument|object
argument_list|,
argument|union any_object
argument_list|)
end_macro

begin_ifdef
ifdef|#
directive|ifdef
name|NO_C99_FORMAT
end_ifdef

begin_define
DECL|macro|SZ_FMT
define|#
directive|define
name|SZ_FMT
value|"%u"
end_define

begin_else
else|#
directive|else
end_else

begin_define
define|#
directive|define
name|SZ_FMT
value|"%zu"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|report
specifier|static
name|void
name|report
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|int
name|count
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%10s: %8u ("
name|SZ_FMT
literal|" kB)\n"
argument_list|,
name|name
argument_list|,
name|count
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
end_function

begin_undef
DECL|macro|SZ_FMT
undef|#
directive|undef
name|SZ_FMT
end_undef

begin_define
DECL|macro|REPORT
define|#
directive|define
name|REPORT
parameter_list|(
name|name
parameter_list|)
define|\
value|report(#name, name##_allocs, name##_allocs*sizeof(struct name)>> 10)
end_define

begin_function
DECL|function|alloc_report
name|void
name|alloc_report
parameter_list|(
name|void
parameter_list|)
block|{
name|REPORT
argument_list|(
name|blob
argument_list|)
expr_stmt|;
name|REPORT
argument_list|(
name|tree
argument_list|)
expr_stmt|;
name|REPORT
argument_list|(
name|commit
argument_list|)
expr_stmt|;
name|REPORT
argument_list|(
name|tag
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

