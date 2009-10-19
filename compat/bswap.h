begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Let's make sure we always have a sane definition for ntohl()/htonl().  * Some libraries define those as a function call, just to perform byte  * shifting, bringing significant overhead to what should be a simple  * operation.  */
end_comment

begin_comment
comment|/*  * Default version that the compiler ought to optimize properly with  * constant values.  */
end_comment

begin_function
DECL|function|default_swab32
specifier|static
specifier|inline
name|uint32_t
name|default_swab32
parameter_list|(
name|uint32_t
name|val
parameter_list|)
block|{
return|return
operator|(
operator|(
operator|(
name|val
operator|&
literal|0xff000000
operator|)
operator|>>
literal|24
operator|)
operator||
operator|(
operator|(
name|val
operator|&
literal|0x00ff0000
operator|)
operator|>>
literal|8
operator|)
operator||
operator|(
operator|(
name|val
operator|&
literal|0x0000ff00
operator|)
operator|<<
literal|8
operator|)
operator||
operator|(
operator|(
name|val
operator|&
literal|0x000000ff
operator|)
operator|<<
literal|24
operator|)
operator|)
return|;
block|}
end_function

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__GNUC__
argument_list|)
operator|&&
operator|(
name|defined
argument_list|(
name|__i386__
argument_list|)
operator|||
name|defined
argument_list|(
name|__x86_64__
argument_list|)
operator|)
end_if

begin_define
DECL|macro|bswap32
define|#
directive|define
name|bswap32
parameter_list|(
name|x
parameter_list|)
value|({ \ 	uint32_t __res; \ 	if (__builtin_constant_p(x)) { \ 		__res = default_swab32(x); \ 	} else { \ 		__asm__("bswap %0" : "=r" (__res) : "0" (x)); \ 	} \ 	__res; })
end_define

begin_elif
elif|#
directive|elif
name|defined
argument_list|(
name|_MSC_VER
argument_list|)
operator|&&
operator|(
name|defined
argument_list|(
name|_M_IX86
argument_list|)
operator|||
name|defined
argument_list|(
name|_M_X64
argument_list|)
operator|)
end_elif

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_define
DECL|macro|bswap32
define|#
directive|define
name|bswap32
parameter_list|(
name|x
parameter_list|)
value|_byteswap_ulong(x)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|bswap32
end_ifdef

begin_undef
DECL|macro|ntohl
undef|#
directive|undef
name|ntohl
end_undef

begin_undef
DECL|macro|htonl
undef|#
directive|undef
name|htonl
end_undef

begin_define
DECL|macro|ntohl
define|#
directive|define
name|ntohl
parameter_list|(
name|x
parameter_list|)
value|bswap32(x)
end_define

begin_define
DECL|macro|htonl
define|#
directive|define
name|htonl
parameter_list|(
name|x
parameter_list|)
value|bswap32(x)
end_define

begin_endif
endif|#
directive|endif
end_endif

end_unit

