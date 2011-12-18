begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * SHA1 routine optimized to do word accesses rather than byte accesses,  * and to avoid unnecessary copies into the context array.  *  * This was initially based on the Mozilla SHA1 implementation, although  * none of the original Mozilla code remains.  */
end_comment

begin_comment
comment|/* this is only to get definitions for memcpy(), ntohl() and htonl() */
end_comment

begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"sha1.h"
end_include

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

begin_comment
comment|/*  * Force usage of rol or ror by selecting the one with the smaller constant.  * It _can_ generate slightly smaller code (a constant of 1 is special), but  * perhaps more importantly it's possibly faster on any uarch that does a  * rotate with a loop.  */
end_comment

begin_define
DECL|macro|SHA_ASM
define|#
directive|define
name|SHA_ASM
parameter_list|(
name|op
parameter_list|,
name|x
parameter_list|,
name|n
parameter_list|)
value|({ unsigned int __res; __asm__(op " %1,%0":"=r" (__res):"i" (n), "0" (x)); __res; })
end_define

begin_define
DECL|macro|SHA_ROL
define|#
directive|define
name|SHA_ROL
parameter_list|(
name|x
parameter_list|,
name|n
parameter_list|)
value|SHA_ASM("rol", x, n)
end_define

begin_define
DECL|macro|SHA_ROR
define|#
directive|define
name|SHA_ROR
parameter_list|(
name|x
parameter_list|,
name|n
parameter_list|)
value|SHA_ASM("ror", x, n)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|SHA_ROT
define|#
directive|define
name|SHA_ROT
parameter_list|(
name|X
parameter_list|,
name|l
parameter_list|,
name|r
parameter_list|)
value|(((X)<< (l)) | ((X)>> (r)))
end_define

begin_define
DECL|macro|SHA_ROL
define|#
directive|define
name|SHA_ROL
parameter_list|(
name|X
parameter_list|,
name|n
parameter_list|)
value|SHA_ROT(X,n,32-(n))
end_define

begin_define
DECL|macro|SHA_ROR
define|#
directive|define
name|SHA_ROR
parameter_list|(
name|X
parameter_list|,
name|n
parameter_list|)
value|SHA_ROT(X,32-(n),n)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * If you have 32 registers or more, the compiler can (and should)  * try to change the array[] accesses into registers. However, on  * machines with less than ~25 registers, that won't really work,  * and at least gcc will make an unholy mess of it.  *  * So to avoid that mess which just slows things down, we force  * the stores to memory to actually happen (we might be better off  * with a 'W(t)=(val);asm("":"+m" (W(t))' there instead, as  * suggested by Artur Skawina - that will also make gcc unable to  * try to do the silly "optimize away loads" part because it won't  * see what the value will be).  *  * Ben Herrenschmidt reports that on PPC, the C version comes close  * to the optimized asm with this (ie on PPC you don't want that  * 'volatile', since there are lots of registers).  *  * On ARM we get the best code generation by forcing a full memory barrier  * between each SHA_ROUND, otherwise gcc happily get wild with spilling and  * the stack frame size simply explode and performance goes down the drain.  */
end_comment

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__i386__
argument_list|)
operator|||
name|defined
argument_list|(
name|__x86_64__
argument_list|)
end_if

begin_define
DECL|macro|setW
define|#
directive|define
name|setW
parameter_list|(
name|x
parameter_list|,
name|val
parameter_list|)
value|(*(volatile unsigned int *)&W(x) = (val))
end_define

begin_elif
elif|#
directive|elif
name|defined
argument_list|(
name|__GNUC__
argument_list|)
operator|&&
name|defined
argument_list|(
name|__arm__
argument_list|)
end_elif

begin_define
DECL|macro|setW
define|#
directive|define
name|setW
parameter_list|(
name|x
parameter_list|,
name|val
parameter_list|)
value|do { W(x) = (val); __asm__("":::"memory"); } while (0)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|setW
define|#
directive|define
name|setW
parameter_list|(
name|x
parameter_list|,
name|val
parameter_list|)
value|(W(x) = (val))
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * Performance might be improved if the CPU architecture is OK with  * unaligned 32-bit loads and a fast ntohl() is available.  * Otherwise fall back to byte loads and shifts which is portable,  * and is faster on architectures with memory alignment issues.  */
end_comment

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__i386__
argument_list|)
operator|||
name|defined
argument_list|(
name|__x86_64__
argument_list|)
operator|||
expr|\
name|defined
argument_list|(
name|__ppc__
argument_list|)
operator|||
name|defined
argument_list|(
name|__ppc64__
argument_list|)
operator|||
expr|\
name|defined
argument_list|(
name|__powerpc__
argument_list|)
operator|||
name|defined
argument_list|(
name|__powerpc64__
argument_list|)
operator|||
expr|\
name|defined
argument_list|(
name|__s390__
argument_list|)
operator|||
name|defined
argument_list|(
name|__s390x__
argument_list|)
end_if

begin_define
DECL|macro|get_be32
define|#
directive|define
name|get_be32
parameter_list|(
name|p
parameter_list|)
value|ntohl(*(unsigned int *)(p))
end_define

begin_define
DECL|macro|put_be32
define|#
directive|define
name|put_be32
parameter_list|(
name|p
parameter_list|,
name|v
parameter_list|)
value|do { *(unsigned int *)(p) = htonl(v); } while (0)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|get_be32
define|#
directive|define
name|get_be32
parameter_list|(
name|p
parameter_list|)
value|( \ 	(*((unsigned char *)(p) + 0)<< 24) | \ 	(*((unsigned char *)(p) + 1)<< 16) | \ 	(*((unsigned char *)(p) + 2)<<  8) | \ 	(*((unsigned char *)(p) + 3)<<  0) )
end_define

begin_define
DECL|macro|put_be32
define|#
directive|define
name|put_be32
parameter_list|(
name|p
parameter_list|,
name|v
parameter_list|)
value|do { \ 	unsigned int __v = (v); \ 	*((unsigned char *)(p) + 0) = __v>> 24; \ 	*((unsigned char *)(p) + 1) = __v>> 16; \ 	*((unsigned char *)(p) + 2) = __v>>  8; \ 	*((unsigned char *)(p) + 3) = __v>>  0; } while (0)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* This "rolls" over the 512-bit array */
end_comment

begin_define
DECL|macro|W
define|#
directive|define
name|W
parameter_list|(
name|x
parameter_list|)
value|(array[(x)&15])
end_define

begin_comment
comment|/*  * Where do we get the source from? The first 16 iterations get it from  * the input data, the next mix it from the 512-bit array.  */
end_comment

begin_define
DECL|macro|SHA_SRC
define|#
directive|define
name|SHA_SRC
parameter_list|(
name|t
parameter_list|)
value|get_be32(data + t)
end_define

begin_define
DECL|macro|SHA_MIX
define|#
directive|define
name|SHA_MIX
parameter_list|(
name|t
parameter_list|)
value|SHA_ROL(W(t+13) ^ W(t+8) ^ W(t+2) ^ W(t), 1)
end_define

begin_define
DECL|macro|SHA_ROUND
define|#
directive|define
name|SHA_ROUND
parameter_list|(
name|t
parameter_list|,
name|input
parameter_list|,
name|fn
parameter_list|,
name|constant
parameter_list|,
name|A
parameter_list|,
name|B
parameter_list|,
name|C
parameter_list|,
name|D
parameter_list|,
name|E
parameter_list|)
value|do { \ 	unsigned int TEMP = input(t); setW(t, TEMP); \ 	E += TEMP + SHA_ROL(A,5) + (fn) + (constant); \ 	B = SHA_ROR(B, 2); } while (0)
end_define

begin_define
DECL|macro|T_0_15
define|#
directive|define
name|T_0_15
parameter_list|(
name|t
parameter_list|,
name|A
parameter_list|,
name|B
parameter_list|,
name|C
parameter_list|,
name|D
parameter_list|,
name|E
parameter_list|)
value|SHA_ROUND(t, SHA_SRC, (((C^D)&B)^D) , 0x5a827999, A, B, C, D, E )
end_define

begin_define
DECL|macro|T_16_19
define|#
directive|define
name|T_16_19
parameter_list|(
name|t
parameter_list|,
name|A
parameter_list|,
name|B
parameter_list|,
name|C
parameter_list|,
name|D
parameter_list|,
name|E
parameter_list|)
value|SHA_ROUND(t, SHA_MIX, (((C^D)&B)^D) , 0x5a827999, A, B, C, D, E )
end_define

begin_define
DECL|macro|T_20_39
define|#
directive|define
name|T_20_39
parameter_list|(
name|t
parameter_list|,
name|A
parameter_list|,
name|B
parameter_list|,
name|C
parameter_list|,
name|D
parameter_list|,
name|E
parameter_list|)
value|SHA_ROUND(t, SHA_MIX, (B^C^D) , 0x6ed9eba1, A, B, C, D, E )
end_define

begin_define
DECL|macro|T_40_59
define|#
directive|define
name|T_40_59
parameter_list|(
name|t
parameter_list|,
name|A
parameter_list|,
name|B
parameter_list|,
name|C
parameter_list|,
name|D
parameter_list|,
name|E
parameter_list|)
value|SHA_ROUND(t, SHA_MIX, ((B&C)+(D&(B^C))) , 0x8f1bbcdc, A, B, C, D, E )
end_define

begin_define
DECL|macro|T_60_79
define|#
directive|define
name|T_60_79
parameter_list|(
name|t
parameter_list|,
name|A
parameter_list|,
name|B
parameter_list|,
name|C
parameter_list|,
name|D
parameter_list|,
name|E
parameter_list|)
value|SHA_ROUND(t, SHA_MIX, (B^C^D) ,  0xca62c1d6, A, B, C, D, E )
end_define

begin_function
DECL|function|blk_SHA1_Block
specifier|static
name|void
name|blk_SHA1_Block
parameter_list|(
name|blk_SHA_CTX
modifier|*
name|ctx
parameter_list|,
specifier|const
name|unsigned
name|int
modifier|*
name|data
parameter_list|)
block|{
name|unsigned
name|int
name|A
decl_stmt|,
name|B
decl_stmt|,
name|C
decl_stmt|,
name|D
decl_stmt|,
name|E
decl_stmt|;
name|unsigned
name|int
name|array
index|[
literal|16
index|]
decl_stmt|;
name|A
operator|=
name|ctx
operator|->
name|H
index|[
literal|0
index|]
expr_stmt|;
name|B
operator|=
name|ctx
operator|->
name|H
index|[
literal|1
index|]
expr_stmt|;
name|C
operator|=
name|ctx
operator|->
name|H
index|[
literal|2
index|]
expr_stmt|;
name|D
operator|=
name|ctx
operator|->
name|H
index|[
literal|3
index|]
expr_stmt|;
name|E
operator|=
name|ctx
operator|->
name|H
index|[
literal|4
index|]
expr_stmt|;
comment|/* Round 1 - iterations 0-16 take their input from 'data' */
name|T_0_15
argument_list|(
literal|0
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|1
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|2
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|3
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|4
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|5
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|6
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|7
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|8
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|9
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|10
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|11
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|12
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|13
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|14
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_0_15
argument_list|(
literal|15
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
comment|/* Round 1 - tail. Input from 512-bit mixing array */
name|T_16_19
argument_list|(
literal|16
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_16_19
argument_list|(
literal|17
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_16_19
argument_list|(
literal|18
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_16_19
argument_list|(
literal|19
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
comment|/* Round 2 */
name|T_20_39
argument_list|(
literal|20
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|21
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|22
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|23
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|24
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|25
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|26
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|27
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|28
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|29
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|30
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|31
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|32
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|33
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|34
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|35
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|36
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|37
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|38
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_20_39
argument_list|(
literal|39
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
comment|/* Round 3 */
name|T_40_59
argument_list|(
literal|40
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|41
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|42
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|43
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|44
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|45
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|46
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|47
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|48
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|49
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|50
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|51
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|52
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|53
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|54
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|55
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|56
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|57
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|58
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_40_59
argument_list|(
literal|59
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
comment|/* Round 4 */
name|T_60_79
argument_list|(
literal|60
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|61
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|62
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|63
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|64
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|65
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|66
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|67
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|68
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|69
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|70
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|71
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|72
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|73
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|74
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|75
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|76
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|77
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|,
name|C
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|78
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|,
name|B
argument_list|)
expr_stmt|;
name|T_60_79
argument_list|(
literal|79
argument_list|,
name|B
argument_list|,
name|C
argument_list|,
name|D
argument_list|,
name|E
argument_list|,
name|A
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|H
index|[
literal|0
index|]
operator|+=
name|A
expr_stmt|;
name|ctx
operator|->
name|H
index|[
literal|1
index|]
operator|+=
name|B
expr_stmt|;
name|ctx
operator|->
name|H
index|[
literal|2
index|]
operator|+=
name|C
expr_stmt|;
name|ctx
operator|->
name|H
index|[
literal|3
index|]
operator|+=
name|D
expr_stmt|;
name|ctx
operator|->
name|H
index|[
literal|4
index|]
operator|+=
name|E
expr_stmt|;
block|}
end_function

begin_function
DECL|function|blk_SHA1_Init
name|void
name|blk_SHA1_Init
parameter_list|(
name|blk_SHA_CTX
modifier|*
name|ctx
parameter_list|)
block|{
name|ctx
operator|->
name|size
operator|=
literal|0
expr_stmt|;
comment|/* Initialize H with the magic constants (see FIPS180 for constants) */
name|ctx
operator|->
name|H
index|[
literal|0
index|]
operator|=
literal|0x67452301
expr_stmt|;
name|ctx
operator|->
name|H
index|[
literal|1
index|]
operator|=
literal|0xefcdab89
expr_stmt|;
name|ctx
operator|->
name|H
index|[
literal|2
index|]
operator|=
literal|0x98badcfe
expr_stmt|;
name|ctx
operator|->
name|H
index|[
literal|3
index|]
operator|=
literal|0x10325476
expr_stmt|;
name|ctx
operator|->
name|H
index|[
literal|4
index|]
operator|=
literal|0xc3d2e1f0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|blk_SHA1_Update
name|void
name|blk_SHA1_Update
parameter_list|(
name|blk_SHA_CTX
modifier|*
name|ctx
parameter_list|,
specifier|const
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|len
parameter_list|)
block|{
name|int
name|lenW
init|=
name|ctx
operator|->
name|size
operator|&
literal|63
decl_stmt|;
name|ctx
operator|->
name|size
operator|+=
name|len
expr_stmt|;
comment|/* Read the data into W and process blocks as they get full */
if|if
condition|(
name|lenW
condition|)
block|{
name|int
name|left
init|=
literal|64
operator|-
name|lenW
decl_stmt|;
if|if
condition|(
name|len
operator|<
name|left
condition|)
name|left
operator|=
name|len
expr_stmt|;
name|memcpy
argument_list|(
name|lenW
operator|+
operator|(
name|char
operator|*
operator|)
name|ctx
operator|->
name|W
argument_list|,
name|data
argument_list|,
name|left
argument_list|)
expr_stmt|;
name|lenW
operator|=
operator|(
name|lenW
operator|+
name|left
operator|)
operator|&
literal|63
expr_stmt|;
name|len
operator|-=
name|left
expr_stmt|;
name|data
operator|=
operator|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|data
operator|+
name|left
operator|)
expr_stmt|;
if|if
condition|(
name|lenW
condition|)
return|return;
name|blk_SHA1_Block
argument_list|(
name|ctx
argument_list|,
name|ctx
operator|->
name|W
argument_list|)
expr_stmt|;
block|}
while|while
condition|(
name|len
operator|>=
literal|64
condition|)
block|{
name|blk_SHA1_Block
argument_list|(
name|ctx
argument_list|,
name|data
argument_list|)
expr_stmt|;
name|data
operator|=
operator|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|data
operator|+
literal|64
operator|)
expr_stmt|;
name|len
operator|-=
literal|64
expr_stmt|;
block|}
if|if
condition|(
name|len
condition|)
name|memcpy
argument_list|(
name|ctx
operator|->
name|W
argument_list|,
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|blk_SHA1_Final
name|void
name|blk_SHA1_Final
parameter_list|(
name|unsigned
name|char
name|hashout
index|[
literal|20
index|]
parameter_list|,
name|blk_SHA_CTX
modifier|*
name|ctx
parameter_list|)
block|{
specifier|static
specifier|const
name|unsigned
name|char
name|pad
index|[
literal|64
index|]
init|=
block|{
literal|0x80
block|}
decl_stmt|;
name|unsigned
name|int
name|padlen
index|[
literal|2
index|]
decl_stmt|;
name|int
name|i
decl_stmt|;
comment|/* Pad with a binary 1 (ie 0x80), then zeroes, then length */
name|padlen
index|[
literal|0
index|]
operator|=
name|htonl
argument_list|(
name|ctx
operator|->
name|size
operator|>>
literal|29
argument_list|)
expr_stmt|;
name|padlen
index|[
literal|1
index|]
operator|=
name|htonl
argument_list|(
name|ctx
operator|->
name|size
operator|<<
literal|3
argument_list|)
expr_stmt|;
name|i
operator|=
name|ctx
operator|->
name|size
operator|&
literal|63
expr_stmt|;
name|blk_SHA1_Update
argument_list|(
name|ctx
argument_list|,
name|pad
argument_list|,
literal|1
operator|+
operator|(
literal|63
operator|&
operator|(
literal|55
operator|-
name|i
operator|)
operator|)
argument_list|)
expr_stmt|;
name|blk_SHA1_Update
argument_list|(
name|ctx
argument_list|,
name|padlen
argument_list|,
literal|8
argument_list|)
expr_stmt|;
comment|/* Output hash */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|5
condition|;
name|i
operator|++
control|)
name|put_be32
argument_list|(
name|hashout
operator|+
name|i
operator|*
literal|4
argument_list|,
name|ctx
operator|->
name|H
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

