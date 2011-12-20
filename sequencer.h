begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SEQUENCER_H
end_ifndef

begin_define
DECL|macro|SEQUENCER_H
define|#
directive|define
name|SEQUENCER_H
end_define

begin_define
DECL|macro|SEQ_DIR
define|#
directive|define
name|SEQ_DIR
value|"sequencer"
end_define

begin_define
DECL|macro|SEQ_HEAD_FILE
define|#
directive|define
name|SEQ_HEAD_FILE
value|"sequencer/head"
end_define

begin_define
DECL|macro|SEQ_TODO_FILE
define|#
directive|define
name|SEQ_TODO_FILE
value|"sequencer/todo"
end_define

begin_define
DECL|macro|SEQ_OPTS_FILE
define|#
directive|define
name|SEQ_OPTS_FILE
value|"sequencer/opts"
end_define

begin_comment
comment|/* Removes SEQ_DIR. */
end_comment

begin_function_decl
specifier|extern
name|void
name|remove_sequencer_state
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

