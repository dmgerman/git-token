begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|GRAPH_H
end_ifndef

begin_define
DECL|macro|GRAPH_H
define|#
directive|define
name|GRAPH_H
end_define

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_comment
comment|/* A graph is a pointer to this opaque structure */
end_comment

begin_struct_decl
struct_decl|struct
name|git_graph
struct_decl|;
end_struct_decl

begin_comment
comment|/*  * Called to setup global display of line_prefix diff option.  *  * Passed a diff_options structure which indicates the line_prefix and the  * file to output the prefix to. This is sort of a hack used so that the  * line_prefix will be honored by all flows which also honor "--graph"  * regardless of whether a graph has actually been setup. The normal graph  * flow will honor the exact diff_options passed, but a NULL graph will cause  * display of a line_prefix to stdout.  */
end_comment

begin_function_decl
name|void
name|graph_setup_line_prefix
parameter_list|(
name|struct
name|diff_options
modifier|*
name|diffopt
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Set up a custom scheme for column colors.  *  * The default column color scheme inserts ANSI color escapes to colorize  * the graph. The various color escapes are stored in an array of strings  * where each entry corresponds to a color, except for the last entry,  * which denotes the escape for resetting the color back to the default.  * When generating the graph, strings from this array are inserted before  * and after the various column characters.  *  * This function allows you to enable a custom array of color escapes.  * The 'colors_max' argument is the index of the last "reset" entry.  *  * This functions must be called BEFORE graph_init() is called.  *  * NOTE: This function isn't used in Git outside graph.c but it is used  * by CGit (http://git.zx2c4.com/cgit/) to use HTML for colors.  */
end_comment

begin_function_decl
name|void
name|graph_set_column_colors
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|colors
parameter_list|,
name|unsigned
name|short
name|colors_max
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Create a new struct git_graph.  */
end_comment

begin_function_decl
name|struct
name|git_graph
modifier|*
name|graph_init
parameter_list|(
name|struct
name|rev_info
modifier|*
name|opt
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Update a git_graph with a new commit.  * This will cause the graph to begin outputting lines for the new commit  * the next time graph_next_line() is called.  *  * If graph_update() is called before graph_is_commit_finished() returns 1,  * the next call to graph_next_line() will output an ellipsis ("...")  * to indicate that a portion of the graph is missing.  */
end_comment

begin_function_decl
name|void
name|graph_update
parameter_list|(
name|struct
name|git_graph
modifier|*
name|graph
parameter_list|,
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Determine if a graph has finished outputting lines for the current  * commit.  *  * Returns 1 if graph_next_line() needs to be called again before  * graph_update() should be called.  Returns 0 if no more lines are needed  * for this commit.  If 0 is returned, graph_next_line() may still be  * called without calling graph_update(), and it will merely output  * appropriate "vertical padding" in the graph.  */
end_comment

begin_function_decl
name|int
name|graph_is_commit_finished
parameter_list|(
name|struct
name|git_graph
specifier|const
modifier|*
name|graph
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Output the next line for a graph.  * This formats the next graph line into the specified strbuf.  It is not  * terminated with a newline.  *  * Returns 1 if the line includes the current commit, and 0 otherwise.  * graph_next_line() will return 1 exactly once for each time  * graph_update() is called.  *  * NOTE: This function isn't used in Git outside graph.c but it is used  * by CGit (http://git.zx2c4.com/cgit/) to wrap HTML around graph lines.  */
end_comment

begin_function_decl
name|int
name|graph_next_line
parameter_list|(
name|struct
name|git_graph
modifier|*
name|graph
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Return current width of the graph in on-screen characters.  */
end_comment

begin_function_decl
name|int
name|graph_width
parameter_list|(
name|struct
name|git_graph
modifier|*
name|graph
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * graph_show_*: helper functions for printing to stdout  */
end_comment

begin_comment
comment|/*  * If the graph is non-NULL, print the history graph to stdout,  * up to and including the line containing this commit.  * Does not print a terminating newline on the last line.  */
end_comment

begin_function_decl
name|void
name|graph_show_commit
parameter_list|(
name|struct
name|git_graph
modifier|*
name|graph
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * If the graph is non-NULL, print one line of the history graph to stdout.  * Does not print a terminating newline on the last line.  */
end_comment

begin_function_decl
name|void
name|graph_show_oneline
parameter_list|(
name|struct
name|git_graph
modifier|*
name|graph
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * If the graph is non-NULL, print one line of vertical graph padding to  * stdout.  Does not print a terminating newline on the last line.  */
end_comment

begin_function_decl
name|void
name|graph_show_padding
parameter_list|(
name|struct
name|git_graph
modifier|*
name|graph
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * If the graph is non-NULL, print the rest of the history graph for this  * commit to stdout.  Does not print a terminating newline on the last line.  */
end_comment

begin_function_decl
name|int
name|graph_show_remainder
parameter_list|(
name|struct
name|git_graph
modifier|*
name|graph
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Print a commit message strbuf and the remainder of the graph to stdout.  *  * This is similar to graph_show_strbuf(), but it always prints the  * remainder of the graph.  *  * If the strbuf ends with a newline, the output printed by  * graph_show_commit_msg() will end with a newline.  If the strbuf is  * missing a terminating newline (including if it is empty), the output  * printed by graph_show_commit_msg() will also be missing a terminating  * newline.  *  * Note that unlike some other graph display functions, you must pass the file  * handle directly. It is assumed that this is the same file handle as the  * file specified by the graph diff options. This is necessary so that  * graph_show_commit_msg can be called even with a NULL graph.  */
end_comment

begin_function_decl
name|void
name|graph_show_commit_msg
parameter_list|(
name|struct
name|git_graph
modifier|*
name|graph
parameter_list|,
name|FILE
modifier|*
name|file
parameter_list|,
name|struct
name|strbuf
specifier|const
modifier|*
name|sb
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* GRAPH_H */
end_comment

end_unit

