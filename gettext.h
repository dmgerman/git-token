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

begin_if
if|#
directive|if
name|defined
argument_list|(
name|_
argument_list|)
operator|||
name|defined
argument_list|(
name|Q_
argument_list|)
end_if

begin_error
error|#
directive|error
literal|"namespace conflict: '_' or 'Q_' is pre-defined?"
end_error

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|NO_GETTEXT
end_ifndef

begin_include
include|#
directive|include
file|<libintl.h>
end_include

begin_else
else|#
directive|else
end_else

begin_ifdef
ifdef|#
directive|ifdef
name|gettext
end_ifdef

begin_undef
DECL|macro|gettext
undef|#
directive|undef
name|gettext
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|gettext
define|#
directive|define
name|gettext
parameter_list|(
name|s
parameter_list|)
value|(s)
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|ngettext
end_ifdef

begin_undef
DECL|macro|ngettext
undef|#
directive|undef
name|ngettext
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|ngettext
define|#
directive|define
name|ngettext
parameter_list|(
name|s
parameter_list|,
name|p
parameter_list|,
name|n
parameter_list|)
value|((n == 1) ? (s) : (p))
end_define

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

begin_ifndef
ifndef|#
directive|ifndef
name|NO_GETTEXT
end_ifndef

begin_function_decl
specifier|extern
name|void
name|git_setup_gettext
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|gettext_width
parameter_list|(
specifier|const
name|char
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_else
else|#
directive|else
end_else

begin_function
DECL|function|git_setup_gettext
specifier|static
specifier|inline
name|void
name|git_setup_gettext
parameter_list|(
name|void
parameter_list|)
block|{ }
end_function

begin_function
DECL|function|gettext_width
specifier|static
specifier|inline
name|int
name|gettext_width
parameter_list|(
specifier|const
name|char
modifier|*
name|s
parameter_list|)
block|{
return|return
name|strlen
argument_list|(
name|s
argument_list|)
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|GETTEXT_POISON
end_ifdef

begin_function_decl
specifier|extern
name|int
name|use_gettext_poison
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

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
if|if
condition|(
operator|!
operator|*
name|msgid
condition|)
return|return
literal|""
return|;
end_expr_stmt

begin_return
return|return
name|use_gettext_poison
argument_list|()
condition|?
literal|"# GETTEXT POISON #"
else|:
name|gettext
argument_list|(
name|msgid
argument_list|)
return|;
end_return

begin_expr_stmt
unit|}  static
specifier|inline
name|FORMAT_PRESERVING
argument_list|(
literal|1
argument_list|)
name|FORMAT_PRESERVING
argument_list|(
literal|2
argument_list|)
DECL|function|Q_
specifier|const
name|char
operator|*
name|Q_
argument_list|(
argument|const char *msgid
argument_list|,
argument|const char *plu
argument_list|,
argument|unsigned long n
argument_list|)
block|{
if|if
condition|(
name|use_gettext_poison
argument_list|()
condition|)
return|return
literal|"# GETTEXT POISON #"
return|;
end_expr_stmt

begin_return
return|return
name|ngettext
argument_list|(
name|msgid
argument_list|,
name|plu
argument_list|,
name|n
argument_list|)
return|;
end_return

begin_comment
unit|}
comment|/* Mark msgid for translation but do not translate it. */
end_comment

begin_if
if|#
directive|if
operator|!
name|USE_PARENS_AROUND_GETTEXT_N
end_if

begin_define
DECL|macro|N_
define|#
directive|define
name|N_
parameter_list|(
name|msgid
parameter_list|)
value|msgid
end_define

begin_else
else|#
directive|else
end_else

begin_comment
comment|/*  * Strictly speaking, this will lead to invalid C when  * used this way:  *	static const char s[] = N_("FOO");  * which will expand to  *	static const char s[] = ("FOO");  * and in valid C, the initializer on the right hand side must  * be without the parentheses.  But many compilers do accept it  * as a language extension and it will allow us to catch mistakes  * like:  *	static const char *msgs[] = {  *		N_("one")  *		N_("two"),  *		N_("three"),  *		NULL  *	};  * (notice the missing comma on one of the lines) by forcing  * a compilation error, because parenthesised ("one") ("two")  * will not get silently turned into ("onetwo").  */
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

begin_function_decl
unit|const
name|char
modifier|*
name|get_preferred_languages
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|is_utf8_locale
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

