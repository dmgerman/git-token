begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|DIR_ITERATOR_H
end_ifndef

begin_define
DECL|macro|DIR_ITERATOR_H
define|#
directive|define
name|DIR_ITERATOR_H
end_define

begin_comment
comment|/*  * Iterate over a directory tree.  *  * Iterate over a directory tree, recursively, including paths of all  * types and hidden paths. Skip "." and ".." entries and don't follow  * symlinks except for the original path.  *  * Every time dir_iterator_advance() is called, update the members of  * the dir_iterator structure to reflect the next path in the  * iteration. The order that paths are iterated over within a  * directory is undefined, but directory paths are always iterated  * over before the subdirectory contents.  *  * A typical iteration looks like this:  *  *     int ok;  *     struct iterator *iter = dir_iterator_begin(path);  *  *     while ((ok = dir_iterator_advance(iter)) == ITER_OK) {  *             if (want_to_stop_iteration()) {  *                     ok = dir_iterator_abort(iter);  *                     break;  *             }  *  *             // Access information about the current path:  *             if (S_ISDIR(iter->st.st_mode))  *                     printf("%s is a directory\n", iter->relative_path);  *     }  *  *     if (ok != ITER_DONE)  *             handle_error();  *  * Callers are allowed to modify iter->path while they are working,  * but they must restore it to its original contents before calling  * dir_iterator_advance() again.  */
end_comment

begin_struct
DECL|struct|dir_iterator
struct|struct
name|dir_iterator
block|{
comment|/* The current path: */
DECL|member|path
name|struct
name|strbuf
name|path
decl_stmt|;
comment|/* 	 * The current path relative to the starting path. This part 	 * of the path always uses "/" characters to separate path 	 * components: 	 */
DECL|member|relative_path
specifier|const
name|char
modifier|*
name|relative_path
decl_stmt|;
comment|/* The current basename: */
DECL|member|basename
specifier|const
name|char
modifier|*
name|basename
decl_stmt|;
comment|/* The result of calling lstat() on path: */
DECL|member|st
name|struct
name|stat
name|st
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * Start a directory iteration over path. Return a dir_iterator that  * holds the internal state of the iteration.  *  * The iteration includes all paths under path, not including path  * itself and not including "." or ".." entries.  *  * path is the starting directory. An internal copy will be made.  */
end_comment

begin_function_decl
name|struct
name|dir_iterator
modifier|*
name|dir_iterator_begin
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Advance the iterator to the first or next item and return ITER_OK.  * If the iteration is exhausted, free the dir_iterator and any  * resources associated with it and return ITER_DONE. On error, free  * dir_iterator and associated resources and return ITER_ERROR. It is  * a bug to use iterator or call this function again after it has  * returned ITER_DONE or ITER_ERROR.  */
end_comment

begin_function_decl
name|int
name|dir_iterator_advance
parameter_list|(
name|struct
name|dir_iterator
modifier|*
name|iterator
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * End the iteration before it has been exhausted. Free the  * dir_iterator and any associated resources and return ITER_DONE. On  * error, free the dir_iterator and return ITER_ERROR.  */
end_comment

begin_function_decl
name|int
name|dir_iterator_abort
parameter_list|(
name|struct
name|dir_iterator
modifier|*
name|iterator
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

