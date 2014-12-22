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

begin_define
DECL|macro|APPEND_SIGNOFF_DEDUP
define|#
directive|define
name|APPEND_SIGNOFF_DEDUP
value|(1u<< 0)
end_define

begin_enum
DECL|enum|replay_action
enum|enum
name|replay_action
block|{
DECL|enumerator|REPLAY_REVERT
name|REPLAY_REVERT
block|,
DECL|enumerator|REPLAY_PICK
name|REPLAY_PICK
block|}
enum|;
end_enum

begin_enum
DECL|enum|replay_subcommand
enum|enum
name|replay_subcommand
block|{
DECL|enumerator|REPLAY_NONE
name|REPLAY_NONE
block|,
DECL|enumerator|REPLAY_REMOVE_STATE
name|REPLAY_REMOVE_STATE
block|,
DECL|enumerator|REPLAY_CONTINUE
name|REPLAY_CONTINUE
block|,
DECL|enumerator|REPLAY_ROLLBACK
name|REPLAY_ROLLBACK
block|}
enum|;
end_enum

begin_struct
DECL|struct|replay_opts
struct|struct
name|replay_opts
block|{
DECL|member|action
name|enum
name|replay_action
name|action
decl_stmt|;
DECL|member|subcommand
name|enum
name|replay_subcommand
name|subcommand
decl_stmt|;
comment|/* Boolean options */
DECL|member|edit
name|int
name|edit
decl_stmt|;
DECL|member|record_origin
name|int
name|record_origin
decl_stmt|;
DECL|member|no_commit
name|int
name|no_commit
decl_stmt|;
DECL|member|signoff
name|int
name|signoff
decl_stmt|;
DECL|member|allow_ff
name|int
name|allow_ff
decl_stmt|;
DECL|member|allow_rerere_auto
name|int
name|allow_rerere_auto
decl_stmt|;
DECL|member|allow_empty
name|int
name|allow_empty
decl_stmt|;
DECL|member|allow_empty_message
name|int
name|allow_empty_message
decl_stmt|;
DECL|member|keep_redundant_commits
name|int
name|keep_redundant_commits
decl_stmt|;
DECL|member|mainline
name|int
name|mainline
decl_stmt|;
DECL|member|gpg_sign
specifier|const
name|char
modifier|*
name|gpg_sign
decl_stmt|;
comment|/* Merge strategy */
DECL|member|strategy
specifier|const
name|char
modifier|*
name|strategy
decl_stmt|;
DECL|member|xopts
specifier|const
name|char
modifier|*
modifier|*
name|xopts
decl_stmt|;
DECL|member|xopts_nr
DECL|member|xopts_alloc
name|size_t
name|xopts_nr
decl_stmt|,
name|xopts_alloc
decl_stmt|;
comment|/* Only used by REPLAY_NONE */
DECL|member|revs
name|struct
name|rev_info
modifier|*
name|revs
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|sequencer_pick_revisions
parameter_list|(
name|struct
name|replay_opts
modifier|*
name|opts
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
specifier|const
name|char
name|sign_off_header
index|[]
decl_stmt|;
end_decl_stmt

begin_function_decl
name|void
name|append_signoff
parameter_list|(
name|struct
name|strbuf
modifier|*
name|msgbuf
parameter_list|,
name|int
name|ignore_footer
parameter_list|,
name|unsigned
name|flag
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|append_conflicts_hint
parameter_list|(
name|struct
name|strbuf
modifier|*
name|msgbuf
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

