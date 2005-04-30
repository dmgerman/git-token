begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_include
include|#
directive|include
file|<ctype.h>
end_include

begin_include
include|#
directive|include
file|<time.h>
end_include

begin_function
DECL|function|my_mktime
specifier|static
name|time_t
name|my_mktime
parameter_list|(
name|struct
name|tm
modifier|*
name|tm
parameter_list|)
block|{
specifier|static
specifier|const
name|int
name|mdays
index|[]
init|=
block|{
literal|0
block|,
literal|31
block|,
literal|59
block|,
literal|90
block|,
literal|120
block|,
literal|151
block|,
literal|181
block|,
literal|212
block|,
literal|243
block|,
literal|273
block|,
literal|304
block|,
literal|334
block|}
decl_stmt|;
name|int
name|year
init|=
name|tm
operator|->
name|tm_year
operator|-
literal|70
decl_stmt|;
name|int
name|month
init|=
name|tm
operator|->
name|tm_mon
decl_stmt|;
name|int
name|day
init|=
name|tm
operator|->
name|tm_mday
decl_stmt|;
if|if
condition|(
name|year
operator|<
literal|0
operator|||
name|year
operator|>
literal|129
condition|)
comment|/* algo only works for 1970-2099 */
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|month
operator|<
literal|0
operator|||
name|month
operator|>
literal|11
condition|)
comment|/* array bounds */
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|month
operator|<
literal|2
operator|||
operator|(
name|year
operator|+
literal|2
operator|)
operator|%
literal|4
condition|)
name|day
operator|--
expr_stmt|;
return|return
operator|(
name|year
operator|*
literal|365
operator|+
operator|(
name|year
operator|+
literal|1
operator|)
operator|/
literal|4
operator|+
name|mdays
index|[
name|month
index|]
operator|+
name|day
operator|)
operator|*
literal|24
operator|*
literal|60
operator|*
literal|60UL
operator|+
name|tm
operator|->
name|tm_hour
operator|*
literal|60
operator|*
literal|60
operator|+
name|tm
operator|->
name|tm_min
operator|*
literal|60
operator|+
name|tm
operator|->
name|tm_sec
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|month_names
specifier|static
specifier|const
name|char
modifier|*
name|month_names
index|[]
init|=
block|{
literal|"January"
block|,
literal|"February"
block|,
literal|"March"
block|,
literal|"April"
block|,
literal|"May"
block|,
literal|"June"
block|,
literal|"July"
block|,
literal|"August"
block|,
literal|"September"
block|,
literal|"October"
block|,
literal|"November"
block|,
literal|"December"
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|weekday_names
specifier|static
specifier|const
name|char
modifier|*
name|weekday_names
index|[]
init|=
block|{
literal|"Sunday"
block|,
literal|"Monday"
block|,
literal|"Tuesday"
block|,
literal|"Wednesday"
block|,
literal|"Thursday"
block|,
literal|"Friday"
block|,
literal|"Saturday"
block|}
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Check these. And note how it doesn't do the summer-time conversion.  *  * In my world, it's always summer, and things are probably a bit off  * in other ways too.  */
end_comment

begin_struct
specifier|static
specifier|const
struct|struct
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|offset
name|int
name|offset
decl_stmt|;
DECL|variable|timezone_names
block|}
name|timezone_names
index|[]
init|=
block|{
block|{
literal|"IDLW"
block|,
operator|-
literal|12
block|}
block|,
comment|/* International Date Line West */
block|{
literal|"NT"
block|,
operator|-
literal|11
block|}
block|,
comment|/* Nome */
block|{
literal|"CAT"
block|,
operator|-
literal|10
block|}
block|,
comment|/* Central Alaska */
block|{
literal|"HST"
block|,
operator|-
literal|10
block|}
block|,
comment|/* Hawaii Standard */
block|{
literal|"HDT"
block|,
operator|-
literal|9
block|}
block|,
comment|/* Hawaii Daylight */
block|{
literal|"YDT"
block|,
operator|-
literal|8
block|}
block|,
comment|/* Yukon Daylight */
block|{
literal|"YST"
block|,
operator|-
literal|9
block|}
block|,
comment|/* Yukon Standard */
block|{
literal|"PST"
block|,
operator|-
literal|8
block|}
block|,
comment|/* Pacific Standard */
block|{
literal|"PDT"
block|,
operator|-
literal|7
block|}
block|,
comment|/* Pacific Daylight */
block|{
literal|"MST"
block|,
operator|-
literal|7
block|}
block|,
comment|/* Mountain Standard */
block|{
literal|"MDT"
block|,
operator|-
literal|6
block|}
block|,
comment|/* Mountain Daylight */
block|{
literal|"CST"
block|,
operator|-
literal|6
block|}
block|,
comment|/* Central Standard */
block|{
literal|"CDT"
block|,
operator|-
literal|5
block|}
block|,
comment|/* Central Daylight */
block|{
literal|"EST"
block|,
operator|-
literal|5
block|}
block|,
comment|/* Eastern Standard */
block|{
literal|"EDT"
block|,
operator|-
literal|4
block|}
block|,
comment|/* Eastern Daylight */
block|{
literal|"AST"
block|,
operator|-
literal|3
block|}
block|,
comment|/* Atlantic Standard */
block|{
literal|"ADT"
block|,
operator|-
literal|2
block|}
block|,
comment|/* Atlantic Daylight */
block|{
literal|"WAT"
block|,
operator|-
literal|1
block|}
block|,
comment|/* West Africa */
block|{
literal|"GMT"
block|,
literal|0
block|}
block|,
comment|/* Greenwich Mean */
block|{
literal|"UTC"
block|,
literal|0
block|}
block|,
comment|/* Universal (Coordinated) */
block|{
literal|"WET"
block|,
literal|0
block|}
block|,
comment|/* Western European */
block|{
literal|"BST"
block|,
literal|0
block|}
block|,
comment|/* British Summer */
block|{
literal|"CET"
block|,
operator|+
literal|1
block|}
block|,
comment|/* Central European */
block|{
literal|"MET"
block|,
operator|+
literal|1
block|}
block|,
comment|/* Middle European */
block|{
literal|"MEWT"
block|,
operator|+
literal|1
block|}
block|,
comment|/* Middle European Winter */
block|{
literal|"MEST"
block|,
operator|+
literal|2
block|}
block|,
comment|/* Middle European Summer */
block|{
literal|"CEST"
block|,
operator|+
literal|2
block|}
block|,
comment|/* Central European Summer */
block|{
literal|"MESZ"
block|,
operator|+
literal|1
block|}
block|,
comment|/* Middle European Summer */
block|{
literal|"FWT"
block|,
operator|+
literal|1
block|}
block|,
comment|/* French Winter */
block|{
literal|"FST"
block|,
operator|+
literal|2
block|}
block|,
comment|/* French Summer */
block|{
literal|"EET"
block|,
operator|+
literal|2
block|}
block|,
comment|/* Eastern Europe, USSR Zone 1 */
block|{
literal|"WAST"
block|,
operator|+
literal|7
block|}
block|,
comment|/* West Australian Standard */
block|{
literal|"WADT"
block|,
operator|+
literal|8
block|}
block|,
comment|/* West Australian Daylight */
block|{
literal|"CCT"
block|,
operator|+
literal|8
block|}
block|,
comment|/* China Coast, USSR Zone 7 */
block|{
literal|"JST"
block|,
operator|+
literal|9
block|}
block|,
comment|/* Japan Standard, USSR Zone 8 */
block|{
literal|"EAST"
block|,
operator|+
literal|10
block|}
block|,
comment|/* Eastern Australian Standard */
block|{
literal|"EADT"
block|,
operator|+
literal|11
block|}
block|,
comment|/* Eastern Australian Daylight */
block|{
literal|"GST"
block|,
operator|+
literal|10
block|}
block|,
comment|/* Guam Standard, USSR Zone 9 */
block|{
literal|"NZT"
block|,
operator|+
literal|11
block|}
block|,
comment|/* New Zealand */
block|{
literal|"NZST"
block|,
operator|+
literal|11
block|}
block|,
comment|/* New Zealand Standard */
block|{
literal|"NZDT"
block|,
operator|+
literal|12
block|}
block|,
comment|/* New Zealand Daylight */
block|{
literal|"IDLE"
block|,
operator|+
literal|12
block|}
block|,
comment|/* International Date Line East */
block|}
struct|;
end_struct

begin_define
DECL|macro|NR_TZ
define|#
directive|define
name|NR_TZ
value|(sizeof(timezone_names) / sizeof(timezone_names[0]))
end_define

begin_function
DECL|function|match_string
specifier|static
name|int
name|match_string
parameter_list|(
specifier|const
name|char
modifier|*
name|date
parameter_list|,
specifier|const
name|char
modifier|*
name|str
parameter_list|)
block|{
name|int
name|i
init|=
literal|0
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
operator|*
name|date
condition|;
name|date
operator|++
operator|,
name|str
operator|++
operator|,
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|date
operator|==
operator|*
name|str
condition|)
continue|continue;
if|if
condition|(
name|toupper
argument_list|(
operator|*
name|date
argument_list|)
operator|==
name|toupper
argument_list|(
operator|*
name|str
argument_list|)
condition|)
continue|continue;
if|if
condition|(
operator|!
name|isalnum
argument_list|(
operator|*
name|date
argument_list|)
condition|)
break|break;
return|return
literal|0
return|;
block|}
return|return
name|i
return|;
block|}
end_function

begin_comment
comment|/* * Parse month, weekday, or timezone name */
end_comment

begin_function
DECL|function|match_alpha
specifier|static
name|int
name|match_alpha
parameter_list|(
specifier|const
name|char
modifier|*
name|date
parameter_list|,
name|struct
name|tm
modifier|*
name|tm
parameter_list|,
name|int
modifier|*
name|offset
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|12
condition|;
name|i
operator|++
control|)
block|{
name|int
name|match
init|=
name|match_string
argument_list|(
name|date
argument_list|,
name|month_names
index|[
name|i
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
name|match
operator|>=
literal|3
condition|)
block|{
name|tm
operator|->
name|tm_mon
operator|=
name|i
expr_stmt|;
return|return
name|match
return|;
block|}
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|7
condition|;
name|i
operator|++
control|)
block|{
name|int
name|match
init|=
name|match_string
argument_list|(
name|date
argument_list|,
name|weekday_names
index|[
name|i
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
name|match
operator|>=
literal|3
condition|)
block|{
name|tm
operator|->
name|tm_wday
operator|=
name|i
expr_stmt|;
return|return
name|match
return|;
block|}
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|NR_TZ
condition|;
name|i
operator|++
control|)
block|{
name|int
name|match
init|=
name|match_string
argument_list|(
name|date
argument_list|,
name|timezone_names
index|[
name|i
index|]
operator|.
name|name
argument_list|)
decl_stmt|;
if|if
condition|(
name|match
operator|>=
literal|3
condition|)
block|{
operator|*
name|offset
operator|=
literal|60
operator|*
name|timezone_names
index|[
name|i
index|]
operator|.
name|offset
expr_stmt|;
return|return
name|match
return|;
block|}
block|}
comment|/* BAD CRAP */
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|match_digit
specifier|static
name|int
name|match_digit
parameter_list|(
name|char
modifier|*
name|date
parameter_list|,
name|struct
name|tm
modifier|*
name|tm
parameter_list|,
name|int
modifier|*
name|offset
parameter_list|)
block|{
name|char
modifier|*
name|end
decl_stmt|,
name|c
decl_stmt|;
name|unsigned
name|long
name|num
decl_stmt|,
name|num2
decl_stmt|,
name|num3
decl_stmt|;
name|num
operator|=
name|strtoul
argument_list|(
name|date
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
comment|/* Time? num:num[:num] */
if|if
condition|(
name|num
operator|<
literal|24
operator|&&
name|end
index|[
literal|0
index|]
operator|==
literal|':'
operator|&&
name|isdigit
argument_list|(
name|end
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
name|tm
operator|->
name|tm_hour
operator|=
name|num
expr_stmt|;
name|num
operator|=
name|strtoul
argument_list|(
name|end
operator|+
literal|1
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|num
operator|<
literal|60
condition|)
block|{
name|tm
operator|->
name|tm_min
operator|=
name|num
expr_stmt|;
if|if
condition|(
name|end
index|[
literal|0
index|]
operator|==
literal|':'
operator|&&
name|isdigit
argument_list|(
name|end
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
name|num
operator|=
name|strtoul
argument_list|(
name|end
operator|+
literal|1
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|num
operator|<
literal|61
condition|)
name|tm
operator|->
name|tm_sec
operator|=
name|num
expr_stmt|;
block|}
block|}
return|return
name|end
operator|-
name|date
return|;
block|}
comment|/* Year? Day of month? Numeric date-string?*/
name|c
operator|=
operator|*
name|end
expr_stmt|;
switch|switch
condition|(
name|c
condition|)
block|{
default|default:
if|if
condition|(
name|num
operator|>
literal|0
operator|&&
name|num
operator|<
literal|32
condition|)
block|{
name|tm
operator|->
name|tm_mday
operator|=
name|num
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|num
operator|>
literal|1900
condition|)
block|{
name|tm
operator|->
name|tm_year
operator|=
name|num
operator|-
literal|1900
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|num
operator|>
literal|70
condition|)
block|{
name|tm
operator|->
name|tm_year
operator|=
name|num
expr_stmt|;
break|break;
block|}
break|break;
case|case
literal|'-'
case|:
case|case
literal|'/'
case|:
if|if
condition|(
name|num
operator|&&
name|num
operator|<
literal|32
operator|&&
name|isdigit
argument_list|(
name|end
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
name|num2
operator|=
name|strtoul
argument_list|(
name|end
operator|+
literal|1
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|num2
operator|||
name|num2
operator|>
literal|31
condition|)
break|break;
if|if
condition|(
name|num
operator|>
literal|12
condition|)
block|{
if|if
condition|(
name|num2
operator|>
literal|12
condition|)
break|break;
name|num3
operator|=
name|num
expr_stmt|;
name|num
operator|=
name|num2
expr_stmt|;
name|num2
operator|=
name|num3
expr_stmt|;
block|}
name|tm
operator|->
name|tm_mon
operator|=
name|num
operator|-
literal|1
expr_stmt|;
name|tm
operator|->
name|tm_mday
operator|=
name|num2
expr_stmt|;
if|if
condition|(
operator|*
name|end
operator|==
name|c
operator|&&
name|isdigit
argument_list|(
name|end
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
name|num3
operator|=
name|strtoul
argument_list|(
name|end
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|num3
operator|>
literal|1900
condition|)
name|num3
operator|-=
literal|1900
expr_stmt|;
name|tm
operator|->
name|tm_year
operator|=
name|num3
expr_stmt|;
block|}
break|break;
block|}
block|}
return|return
name|end
operator|-
name|date
return|;
block|}
end_function

begin_function
DECL|function|match_tz
specifier|static
name|int
name|match_tz
parameter_list|(
name|char
modifier|*
name|date
parameter_list|,
name|int
modifier|*
name|offp
parameter_list|)
block|{
name|char
modifier|*
name|end
decl_stmt|;
name|int
name|offset
init|=
name|strtoul
argument_list|(
name|date
operator|+
literal|1
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
decl_stmt|;
name|int
name|min
decl_stmt|,
name|hour
decl_stmt|;
name|min
operator|=
name|offset
operator|%
literal|100
expr_stmt|;
name|hour
operator|=
name|offset
operator|/
literal|100
expr_stmt|;
name|offset
operator|=
name|hour
operator|*
literal|60
operator|+
name|min
expr_stmt|;
if|if
condition|(
operator|*
name|date
operator|==
literal|'-'
condition|)
name|offset
operator|=
operator|-
name|offset
expr_stmt|;
operator|*
name|offp
operator|=
name|offset
expr_stmt|;
return|return
name|end
operator|-
name|date
return|;
block|}
end_function

begin_comment
comment|/* Gr. strptime is crap for this; it doesn't have a way to require RFC2822    (i.e. English) day/month names, and it doesn't work correctly with %z. */
end_comment

begin_function
DECL|function|parse_date
name|void
name|parse_date
parameter_list|(
name|char
modifier|*
name|date
parameter_list|,
name|char
modifier|*
name|result
parameter_list|,
name|int
name|maxlen
parameter_list|)
block|{
name|struct
name|tm
name|tm
decl_stmt|;
name|int
name|offset
decl_stmt|;
name|time_t
name|then
decl_stmt|;
name|memset
argument_list|(
operator|&
name|tm
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|tm
argument_list|)
argument_list|)
expr_stmt|;
name|tm
operator|.
name|tm_year
operator|=
operator|-
literal|1
expr_stmt|;
name|tm
operator|.
name|tm_mon
operator|=
operator|-
literal|1
expr_stmt|;
name|tm
operator|.
name|tm_mday
operator|=
operator|-
literal|1
expr_stmt|;
name|tm
operator|.
name|tm_isdst
operator|=
operator|-
literal|1
expr_stmt|;
name|offset
operator|=
operator|-
literal|1
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|match
init|=
literal|0
decl_stmt|;
name|unsigned
name|char
name|c
init|=
operator|*
name|date
decl_stmt|;
comment|/* Stop at end of string or newline */
if|if
condition|(
operator|!
name|c
operator|||
name|c
operator|==
literal|'\n'
condition|)
break|break;
if|if
condition|(
name|isalpha
argument_list|(
name|c
argument_list|)
condition|)
name|match
operator|=
name|match_alpha
argument_list|(
name|date
argument_list|,
operator|&
name|tm
argument_list|,
operator|&
name|offset
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|isdigit
argument_list|(
name|c
argument_list|)
condition|)
name|match
operator|=
name|match_digit
argument_list|(
name|date
argument_list|,
operator|&
name|tm
argument_list|,
operator|&
name|offset
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|(
name|c
operator|==
literal|'-'
operator|||
name|c
operator|==
literal|'+'
operator|)
operator|&&
name|isdigit
argument_list|(
name|date
index|[
literal|1
index|]
argument_list|)
condition|)
name|match
operator|=
name|match_tz
argument_list|(
name|date
argument_list|,
operator|&
name|offset
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|match
condition|)
block|{
comment|/* BAD CRAP */
name|match
operator|=
literal|1
expr_stmt|;
block|}
name|date
operator|+=
name|match
expr_stmt|;
block|}
comment|/* mktime uses local timezone */
name|then
operator|=
name|my_mktime
argument_list|(
operator|&
name|tm
argument_list|)
expr_stmt|;
if|if
condition|(
name|offset
operator|==
operator|-
literal|1
condition|)
name|offset
operator|=
operator|(
name|then
operator|-
name|mktime
argument_list|(
operator|&
name|tm
argument_list|)
operator|)
operator|/
literal|60
expr_stmt|;
if|if
condition|(
name|then
operator|==
operator|-
literal|1
condition|)
return|return;
name|then
operator|-=
name|offset
operator|*
literal|60
expr_stmt|;
name|snprintf
argument_list|(
name|result
argument_list|,
name|maxlen
argument_list|,
literal|"%lu %+03d%02d"
argument_list|,
name|then
argument_list|,
name|offset
operator|/
literal|60
argument_list|,
name|offset
operator|%
literal|60
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|datestamp
name|void
name|datestamp
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|int
name|bufsize
parameter_list|)
block|{
name|time_t
name|now
decl_stmt|;
name|int
name|offset
decl_stmt|;
name|time
argument_list|(
operator|&
name|now
argument_list|)
expr_stmt|;
name|offset
operator|=
name|my_mktime
argument_list|(
name|localtime
argument_list|(
operator|&
name|now
argument_list|)
argument_list|)
operator|-
name|now
expr_stmt|;
name|offset
operator|/=
literal|60
expr_stmt|;
name|snprintf
argument_list|(
name|buf
argument_list|,
name|bufsize
argument_list|,
literal|"%lu %+05d"
argument_list|,
name|now
argument_list|,
name|offset
operator|/
literal|60
operator|*
literal|100
operator|+
name|offset
operator|%
literal|60
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

