begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"sigchain.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_define
DECL|macro|X
define|#
directive|define
name|X
parameter_list|(
name|f
parameter_list|)
define|\
value|static void f(int sig) { \ 	puts(#f); \ 	fflush(stdout); \ 	sigchain_pop(sig); \ 	raise(sig); \ }
end_define

begin_macro
DECL|function|X
name|X
argument_list|(
argument|one
argument_list|)
end_macro

begin_macro
name|X
argument_list|(
argument|two
argument_list|)
end_macro

begin_macro
name|X
argument_list|(
argument|three
argument_list|)
end_macro

begin_undef
DECL|macro|X
undef|#
directive|undef
name|X
end_undef

begin_function
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|sigchain_push
argument_list|(
name|SIGTERM
argument_list|,
name|one
argument_list|)
expr_stmt|;
name|sigchain_push
argument_list|(
name|SIGTERM
argument_list|,
name|two
argument_list|)
expr_stmt|;
name|sigchain_push
argument_list|(
name|SIGTERM
argument_list|,
name|three
argument_list|)
expr_stmt|;
name|raise
argument_list|(
name|SIGTERM
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

