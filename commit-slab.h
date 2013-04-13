begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|COMMIT_SLAB_H
end_ifndef

begin_define
DECL|macro|COMMIT_SLAB_H
define|#
directive|define
name|COMMIT_SLAB_H
end_define

begin_comment
comment|/*  * define_commit_slab(slabname, elemtype) creates boilerplate code to define  * a new struct (struct slabname) that is used to associate a piece of data  * of elemtype to commits, and a few functions to use that struct.  *  * After including this header file, using:  *  * define_commit_slab(indegee, int);  *  * will let you call the following functions:  *  * - int *indegree_at(struct indegree *, struct commit *);  *  *   This function locates the data associated with the given commit in  *   the indegree slab, and returns the pointer to it.  *  * - void init_indegree(struct indegree *);  *   void init_indegree_with_stride(struct indegree *, int);  *  *   Initializes the indegree slab that associates an array of integers  *   to each commit. 'stride' specifies how big each array is.  The slab  *   that id initialied by the variant without "_with_stride" associates  *   each commit with an array of one integer.  */
end_comment

begin_comment
comment|/* allocate ~512kB at once, allowing for malloc overhead */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|COMMIT_SLAB_SIZE
end_ifndef

begin_define
DECL|macro|COMMIT_SLAB_SIZE
define|#
directive|define
name|COMMIT_SLAB_SIZE
value|(512*1024-32)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|define_commit_slab
define|#
directive|define
name|define_commit_slab
parameter_list|(
name|slabname
parameter_list|,
name|elemtype
parameter_list|)
define|\ 									\
value|struct slabname {							\ 	unsigned slab_size;						\ 	unsigned stride;						\ 	unsigned slab_count;						\ 	elemtype **slab;						\ };									\ static int stat_ ##slabname## realloc;					\ 									\ static void init_ ##slabname## _with_stride(struct slabname *s,		\ 					    unsigned stride)		\ {									\ 	unsigned int elem_size;						\ 	if (!stride)							\ 		stride = 1;						\ 	s->stride = stride;						\ 	elem_size = sizeof(struct slabname) * stride;			\ 	s->slab_size = COMMIT_SLAB_SIZE / elem_size;			\ 	s->slab_count = 0;						\ 	s->slab = NULL;							\ }									\ 									\ static void init_ ##slabname(struct slabname *s)			\ {									\ 	init_ ##slabname## _with_stride(s, 1);				\ }									\ 									\ static void clear_ ##slabname(struct slabname *s)			\ {									\ 	int i;								\ 	for (i = 0; i< s->slab_count; i++)				\ 		free(s->slab[i]);					\ 	s->slab_count = 0;						\ 	free(s->slab);							\ 	s->slab = NULL;							\ }									\ 									\ static elemtype *slabname## _at(struct slabname *s,			\ 				const struct commit *c)			\ {									\ 	int nth_slab, nth_slot, ix;					\ 									\ 	ix = c->index * s->stride;					\ 	nth_slab = ix / s->slab_size;					\ 	nth_slot = ix % s->slab_size;					\ 									\ 	if (s->slab_count<= nth_slab) {				\ 		int i;							\ 		s->slab = xrealloc(s->slab,				\ 				   (nth_slab + 1) * sizeof(s->slab));	\ 		stat_ ##slabname## realloc++;				\ 		for (i = s->slab_count; i<= nth_slab; i++)		\ 			s->slab[i] = NULL;				\ 		s->slab_count = nth_slab + 1;				\ 	}								\ 	if (!s->slab[nth_slab])						\ 		s->slab[nth_slab] = xcalloc(s->slab_size,		\ 					    sizeof(**s->slab));		\ 	return&s->slab[nth_slab][nth_slot];				\ }									\ 									\ static int stat_ ##slabname## realloc
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* COMMIT_SLAB_H */
end_comment

end_unit

