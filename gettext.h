begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2010-2011 Ãvar ArnfjÃ¶rÃ° Bjarmason  *  * This is a skeleton no-op implementation of gettext for Git.  * You can replace it with something that uses libintl.h and wraps  * gettext() to try out the translations.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|GETTEXT_H
end_ifndef

begin_define
DECL|macro|GETTEXT_H
define|#
directive|define
name|GETTEXT_H
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|_
end_ifdef

begin_error
error|#
directive|error
literal|"namespace conflict: '_' is pre-defined?"
end_error

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|FORMAT_PRESERVING
define|#
directive|define
name|FORMAT_PRESERVING
parameter_list|(
name|n
parameter_list|)
value|__attribute__((format_arg(n)))
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|GETTEXT_POISON
end_ifdef

begin_define
DECL|macro|use_gettext_poison
define|#
directive|define
name|use_gettext_poison
parameter_list|()
value|1
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|use_gettext_poison
define|#
directive|define
name|use_gettext_poison
parameter_list|()
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_expr_stmt
DECL|function|_
specifier|static
specifier|inline
name|FORMAT_PRESERVING
argument_list|(
literal|1
argument_list|)
specifier|const
name|char
operator|*
name|_
argument_list|(
argument|const char *msgid
argument_list|)
block|{
return|return
name|use_gettext_poison
argument_list|()
operator|?
literal|"# GETTEXT POISON #"
operator|:
name|msgid
return|;
block|}
end_expr_stmt

begin_comment
comment|/* Mark msgid for translation but do not translate it. */
end_comment

begin_define
DECL|macro|N_
define|#
directive|define
name|N_
parameter_list|(
name|msgid
parameter_list|)
value|(msgid)
end_define

begin_endif
endif|#
directive|endif
end_endif

end_unit

