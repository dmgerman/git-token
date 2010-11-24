begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SYSLOG_H
end_ifndef

begin_define
DECL|macro|SYSLOG_H
define|#
directive|define
name|SYSLOG_H
end_define

begin_define
DECL|macro|LOG_PID
define|#
directive|define
name|LOG_PID
value|0x01
end_define

begin_define
DECL|macro|LOG_EMERG
define|#
directive|define
name|LOG_EMERG
value|0
end_define

begin_define
DECL|macro|LOG_ALERT
define|#
directive|define
name|LOG_ALERT
value|1
end_define

begin_define
DECL|macro|LOG_CRIT
define|#
directive|define
name|LOG_CRIT
value|2
end_define

begin_define
DECL|macro|LOG_ERR
define|#
directive|define
name|LOG_ERR
value|3
end_define

begin_define
DECL|macro|LOG_WARNING
define|#
directive|define
name|LOG_WARNING
value|4
end_define

begin_define
DECL|macro|LOG_NOTICE
define|#
directive|define
name|LOG_NOTICE
value|5
end_define

begin_define
DECL|macro|LOG_INFO
define|#
directive|define
name|LOG_INFO
value|6
end_define

begin_define
DECL|macro|LOG_DEBUG
define|#
directive|define
name|LOG_DEBUG
value|7
end_define

begin_define
DECL|macro|LOG_DAEMON
define|#
directive|define
name|LOG_DAEMON
value|(3<<3)
end_define

begin_function_decl
name|void
name|openlog
parameter_list|(
specifier|const
name|char
modifier|*
name|ident
parameter_list|,
name|int
name|logopt
parameter_list|,
name|int
name|facility
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|syslog
parameter_list|(
name|int
name|priority
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* SYSLOG_H */
end_comment

end_unit

