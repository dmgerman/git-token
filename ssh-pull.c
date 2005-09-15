begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_define
DECL|macro|COUNTERPART_ENV_NAME
define|#
directive|define
name|COUNTERPART_ENV_NAME
value|"GIT_SSH_PUSH"
end_define

begin_define
DECL|macro|COUNTERPART_PROGRAM_NAME
define|#
directive|define
name|COUNTERPART_PROGRAM_NAME
value|"git-ssh-push"
end_define

begin_define
DECL|macro|MY_PROGRAM_NAME
define|#
directive|define
name|MY_PROGRAM_NAME
value|"git-ssh-pull"
end_define

begin_include
include|#
directive|include
file|"ssh-fetch.c"
end_include

end_unit

