begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Sane locale-independent, ASCII ctype.  *  * No surprises, and works with signed and unsigned chars.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_enum
enum|enum
block|{
DECL|enumerator|S
name|S
init|=
name|GIT_SPACE
block|,
DECL|enumerator|A
name|A
init|=
name|GIT_ALPHA
block|,
DECL|enumerator|D
name|D
init|=
name|GIT_DIGIT
block|,
DECL|enumerator|G
name|G
init|=
name|GIT_GLOB_SPECIAL
block|,
comment|/* *, ?, [, \\ */
DECL|enumerator|R
name|R
init|=
name|GIT_REGEX_SPECIAL
block|,
comment|/* $, (, ), +, ., ^, {, | */
block|}
enum|;
end_enum

begin_decl_stmt
DECL|variable|sane_ctype
name|unsigned
name|char
name|sane_ctype
index|[
literal|256
index|]
init|=
block|{
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
name|S
block|,
name|S
block|,
literal|0
block|,
literal|0
block|,
name|S
block|,
literal|0
block|,
literal|0
block|,
comment|/*   0.. 15 */
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
comment|/*  16.. 31 */
name|S
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
name|R
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
name|R
block|,
name|R
block|,
name|G
block|,
name|R
block|,
literal|0
block|,
literal|0
block|,
name|R
block|,
literal|0
block|,
comment|/*  32.. 47 */
name|D
block|,
name|D
block|,
name|D
block|,
name|D
block|,
name|D
block|,
name|D
block|,
name|D
block|,
name|D
block|,
name|D
block|,
name|D
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
name|G
block|,
comment|/*  48.. 63 */
literal|0
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
comment|/*  64.. 79 */
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|G
block|,
name|G
block|,
literal|0
block|,
name|R
block|,
literal|0
block|,
comment|/*  80.. 95 */
literal|0
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
comment|/*  96..111 */
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|A
block|,
name|R
block|,
name|R
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|,
comment|/* 112..127 */
comment|/* Nothing in the 128.. range */
block|}
decl_stmt|;
end_decl_stmt

end_unit

