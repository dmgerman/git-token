begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|<time.h>
end_include

begin_include
include|#
directive|include
file|<sys/time.h>
end_include

begin_include
include|#
directive|include
file|"cache.h"
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
comment|/*  * The "tz" thing is passed in as this strange "decimal parse of tz"  * thing, which means that tz -0100 is passed in as the integer -100,  * even though it means "sixty minutes off"  */
end_comment

begin_function
DECL|function|show_date
specifier|const
name|char
modifier|*
name|show_date
parameter_list|(
name|unsigned
name|long
name|time
parameter_list|,
name|int
name|tz
parameter_list|)
block|{
name|struct
name|tm
modifier|*
name|tm
decl_stmt|;
name|time_t
name|t
decl_stmt|;
specifier|static
name|char
name|timebuf
index|[
literal|200
index|]
decl_stmt|;
name|int
name|minutes
decl_stmt|;
name|minutes
operator|=
name|tz
operator|<
literal|0
condition|?
operator|-
name|tz
else|:
name|tz
expr_stmt|;
name|minutes
operator|=
operator|(
name|minutes
operator|/
literal|100
operator|)
operator|*
literal|60
operator|+
operator|(
name|minutes
operator|%
literal|100
operator|)
expr_stmt|;
name|minutes
operator|=
name|tz
operator|<
literal|0
condition|?
operator|-
name|minutes
else|:
name|minutes
expr_stmt|;
name|t
operator|=
name|time
operator|+
name|minutes
operator|*
literal|60
expr_stmt|;
name|tm
operator|=
name|gmtime
argument_list|(
operator|&
name|t
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tm
condition|)
return|return
name|NULL
return|;
name|sprintf
argument_list|(
name|timebuf
argument_list|,
literal|"%.3s %.3s %d %02d:%02d:%02d %d %+05d"
argument_list|,
name|weekday_names
index|[
name|tm
operator|->
name|tm_wday
index|]
argument_list|,
name|month_names
index|[
name|tm
operator|->
name|tm_mon
index|]
argument_list|,
name|tm
operator|->
name|tm_mday
argument_list|,
name|tm
operator|->
name|tm_hour
argument_list|,
name|tm
operator|->
name|tm_min
argument_list|,
name|tm
operator|->
name|tm_sec
argument_list|,
name|tm
operator|->
name|tm_year
operator|+
literal|1900
argument_list|,
name|tz
argument_list|)
expr_stmt|;
return|return
name|timebuf
return|;
block|}
end_function

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
DECL|member|dst
name|int
name|dst
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
block|,
literal|0
block|, }
block|,
comment|/* International Date Line West */
block|{
literal|"NT"
block|,
operator|-
literal|11
block|,
literal|0
block|, }
block|,
comment|/* Nome */
block|{
literal|"CAT"
block|,
operator|-
literal|10
block|,
literal|0
block|, }
block|,
comment|/* Central Alaska */
block|{
literal|"HST"
block|,
operator|-
literal|10
block|,
literal|0
block|, }
block|,
comment|/* Hawaii Standard */
block|{
literal|"HDT"
block|,
operator|-
literal|10
block|,
literal|1
block|, }
block|,
comment|/* Hawaii Daylight */
block|{
literal|"YST"
block|,
operator|-
literal|9
block|,
literal|0
block|, }
block|,
comment|/* Yukon Standard */
block|{
literal|"YDT"
block|,
operator|-
literal|9
block|,
literal|1
block|, }
block|,
comment|/* Yukon Daylight */
block|{
literal|"PST"
block|,
operator|-
literal|8
block|,
literal|0
block|, }
block|,
comment|/* Pacific Standard */
block|{
literal|"PDT"
block|,
operator|-
literal|8
block|,
literal|1
block|, }
block|,
comment|/* Pacific Daylight */
block|{
literal|"MST"
block|,
operator|-
literal|7
block|,
literal|0
block|, }
block|,
comment|/* Mountain Standard */
block|{
literal|"MDT"
block|,
operator|-
literal|7
block|,
literal|1
block|, }
block|,
comment|/* Mountain Daylight */
block|{
literal|"CST"
block|,
operator|-
literal|6
block|,
literal|0
block|, }
block|,
comment|/* Central Standard */
block|{
literal|"CDT"
block|,
operator|-
literal|6
block|,
literal|1
block|, }
block|,
comment|/* Central Daylight */
block|{
literal|"EST"
block|,
operator|-
literal|5
block|,
literal|0
block|, }
block|,
comment|/* Eastern Standard */
block|{
literal|"EDT"
block|,
operator|-
literal|5
block|,
literal|1
block|, }
block|,
comment|/* Eastern Daylight */
block|{
literal|"AST"
block|,
operator|-
literal|3
block|,
literal|0
block|, }
block|,
comment|/* Atlantic Standard */
block|{
literal|"ADT"
block|,
operator|-
literal|3
block|,
literal|1
block|, }
block|,
comment|/* Atlantic Daylight */
block|{
literal|"WAT"
block|,
operator|-
literal|1
block|,
literal|0
block|, }
block|,
comment|/* West Africa */
block|{
literal|"GMT"
block|,
literal|0
block|,
literal|0
block|, }
block|,
comment|/* Greenwich Mean */
block|{
literal|"UTC"
block|,
literal|0
block|,
literal|0
block|, }
block|,
comment|/* Universal (Coordinated) */
block|{
literal|"WET"
block|,
literal|0
block|,
literal|0
block|, }
block|,
comment|/* Western European */
block|{
literal|"BST"
block|,
literal|0
block|,
literal|1
block|, }
block|,
comment|/* British Summer */
block|{
literal|"CET"
block|,
operator|+
literal|1
block|,
literal|0
block|, }
block|,
comment|/* Central European */
block|{
literal|"MET"
block|,
operator|+
literal|1
block|,
literal|0
block|, }
block|,
comment|/* Middle European */
block|{
literal|"MEWT"
block|,
operator|+
literal|1
block|,
literal|0
block|, }
block|,
comment|/* Middle European Winter */
block|{
literal|"MEST"
block|,
operator|+
literal|1
block|,
literal|1
block|, }
block|,
comment|/* Middle European Summer */
block|{
literal|"CEST"
block|,
operator|+
literal|1
block|,
literal|1
block|, }
block|,
comment|/* Central European Summer */
block|{
literal|"MESZ"
block|,
operator|+
literal|1
block|,
literal|1
block|, }
block|,
comment|/* Middle European Summer */
block|{
literal|"FWT"
block|,
operator|+
literal|1
block|,
literal|0
block|, }
block|,
comment|/* French Winter */
block|{
literal|"FST"
block|,
operator|+
literal|1
block|,
literal|1
block|, }
block|,
comment|/* French Summer */
block|{
literal|"EET"
block|,
operator|+
literal|2
block|,
literal|0
block|, }
block|,
comment|/* Eastern Europe, USSR Zone 1 */
block|{
literal|"EEST"
block|,
operator|+
literal|2
block|,
literal|1
block|, }
block|,
comment|/* Eastern European Daylight */
block|{
literal|"WAST"
block|,
operator|+
literal|7
block|,
literal|0
block|, }
block|,
comment|/* West Australian Standard */
block|{
literal|"WADT"
block|,
operator|+
literal|7
block|,
literal|1
block|, }
block|,
comment|/* West Australian Daylight */
block|{
literal|"CCT"
block|,
operator|+
literal|8
block|,
literal|0
block|, }
block|,
comment|/* China Coast, USSR Zone 7 */
block|{
literal|"JST"
block|,
operator|+
literal|9
block|,
literal|0
block|, }
block|,
comment|/* Japan Standard, USSR Zone 8 */
block|{
literal|"EAST"
block|,
operator|+
literal|10
block|,
literal|0
block|, }
block|,
comment|/* Eastern Australian Standard */
block|{
literal|"EADT"
block|,
operator|+
literal|10
block|,
literal|1
block|, }
block|,
comment|/* Eastern Australian Daylight */
block|{
literal|"GST"
block|,
operator|+
literal|10
block|,
literal|0
block|, }
block|,
comment|/* Guam Standard, USSR Zone 9 */
block|{
literal|"NZT"
block|,
operator|+
literal|11
block|,
literal|0
block|, }
block|,
comment|/* New Zealand */
block|{
literal|"NZST"
block|,
operator|+
literal|11
block|,
literal|0
block|, }
block|,
comment|/* New Zealand Standard */
block|{
literal|"NZDT"
block|,
operator|+
literal|11
block|,
literal|1
block|, }
block|,
comment|/* New Zealand Daylight */
block|{
literal|"IDLE"
block|,
operator|+
literal|12
block|,
literal|0
block|, }
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

begin_function
DECL|function|skip_alpha
specifier|static
name|int
name|skip_alpha
parameter_list|(
specifier|const
name|char
modifier|*
name|date
parameter_list|)
block|{
name|int
name|i
init|=
literal|0
decl_stmt|;
do|do
block|{
name|i
operator|++
expr_stmt|;
block|}
do|while
condition|(
name|isalpha
argument_list|(
name|date
index|[
name|i
index|]
argument_list|)
condition|)
do|;
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
name|int
name|off
init|=
name|timezone_names
index|[
name|i
index|]
operator|.
name|offset
decl_stmt|;
comment|/* This is bogus, but we like summer */
name|off
operator|+=
name|timezone_names
index|[
name|i
index|]
operator|.
name|dst
expr_stmt|;
comment|/* Only use the tz name offset if we don't have anything better */
if|if
condition|(
operator|*
name|offset
operator|==
operator|-
literal|1
condition|)
operator|*
name|offset
operator|=
literal|60
operator|*
name|off
expr_stmt|;
return|return
name|match
return|;
block|}
block|}
if|if
condition|(
name|match_string
argument_list|(
name|date
argument_list|,
literal|"PM"
argument_list|)
operator|==
literal|2
condition|)
block|{
if|if
condition|(
name|tm
operator|->
name|tm_hour
operator|>
literal|0
operator|&&
name|tm
operator|->
name|tm_hour
operator|<
literal|12
condition|)
name|tm
operator|->
name|tm_hour
operator|+=
literal|12
expr_stmt|;
return|return
literal|2
return|;
block|}
comment|/* BAD CRAP */
return|return
name|skip_alpha
argument_list|(
name|date
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|is_date
specifier|static
name|int
name|is_date
parameter_list|(
name|int
name|year
parameter_list|,
name|int
name|month
parameter_list|,
name|int
name|day
parameter_list|,
name|struct
name|tm
modifier|*
name|tm
parameter_list|)
block|{
if|if
condition|(
name|month
operator|>
literal|0
operator|&&
name|month
operator|<
literal|13
operator|&&
name|day
operator|>
literal|0
operator|&&
name|day
operator|<
literal|32
condition|)
block|{
if|if
condition|(
name|year
operator|==
operator|-
literal|1
condition|)
block|{
name|tm
operator|->
name|tm_mon
operator|=
name|month
operator|-
literal|1
expr_stmt|;
name|tm
operator|->
name|tm_mday
operator|=
name|day
expr_stmt|;
return|return
literal|1
return|;
block|}
if|if
condition|(
name|year
operator|>=
literal|1970
operator|&&
name|year
operator|<
literal|2100
condition|)
block|{
name|year
operator|-=
literal|1900
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|year
operator|>
literal|70
operator|&&
name|year
operator|<
literal|100
condition|)
block|{
comment|/* ok */
block|}
elseif|else
if|if
condition|(
name|year
operator|<
literal|38
condition|)
block|{
name|year
operator|+=
literal|100
expr_stmt|;
block|}
else|else
return|return
literal|0
return|;
name|tm
operator|->
name|tm_mon
operator|=
name|month
operator|-
literal|1
expr_stmt|;
name|tm
operator|->
name|tm_mday
operator|=
name|day
expr_stmt|;
name|tm
operator|->
name|tm_year
operator|=
name|year
expr_stmt|;
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|match_multi_number
specifier|static
name|int
name|match_multi_number
parameter_list|(
name|unsigned
name|long
name|num
parameter_list|,
name|char
name|c
parameter_list|,
specifier|const
name|char
modifier|*
name|date
parameter_list|,
name|char
modifier|*
name|end
parameter_list|,
name|struct
name|tm
modifier|*
name|tm
parameter_list|)
block|{
name|long
name|num2
decl_stmt|,
name|num3
decl_stmt|;
name|num2
operator|=
name|strtol
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
name|num3
operator|=
operator|-
literal|1
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
name|num3
operator|=
name|strtol
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
comment|/* Time? Date? */
switch|switch
condition|(
name|c
condition|)
block|{
case|case
literal|':'
case|:
if|if
condition|(
name|num3
operator|<
literal|0
condition|)
name|num3
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|num
operator|<
literal|25
operator|&&
name|num2
operator|>=
literal|0
operator|&&
name|num2
operator|<
literal|60
operator|&&
name|num3
operator|>=
literal|0
operator|&&
name|num3
operator|<=
literal|60
condition|)
block|{
name|tm
operator|->
name|tm_hour
operator|=
name|num
expr_stmt|;
name|tm
operator|->
name|tm_min
operator|=
name|num2
expr_stmt|;
name|tm
operator|->
name|tm_sec
operator|=
name|num3
expr_stmt|;
break|break;
block|}
return|return
literal|0
return|;
case|case
literal|'-'
case|:
case|case
literal|'/'
case|:
if|if
condition|(
name|num
operator|>
literal|70
condition|)
block|{
comment|/* yyyy-mm-dd? */
if|if
condition|(
name|is_date
argument_list|(
name|num
argument_list|,
name|num2
argument_list|,
name|num3
argument_list|,
name|tm
argument_list|)
condition|)
break|break;
comment|/* yyyy-dd-mm? */
if|if
condition|(
name|is_date
argument_list|(
name|num
argument_list|,
name|num3
argument_list|,
name|num2
argument_list|,
name|tm
argument_list|)
condition|)
break|break;
block|}
comment|/* mm/dd/yy ? */
if|if
condition|(
name|is_date
argument_list|(
name|num3
argument_list|,
name|num2
argument_list|,
name|num
argument_list|,
name|tm
argument_list|)
condition|)
break|break;
comment|/* dd/mm/yy ? */
if|if
condition|(
name|is_date
argument_list|(
name|num3
argument_list|,
name|num
argument_list|,
name|num2
argument_list|,
name|tm
argument_list|)
condition|)
break|break;
return|return
literal|0
return|;
block|}
return|return
name|end
operator|-
name|date
return|;
block|}
end_function

begin_comment
comment|/*  * We've seen a digit. Time? Year? Date?   */
end_comment

begin_function
DECL|function|match_digit
specifier|static
name|int
name|match_digit
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
parameter_list|,
name|int
modifier|*
name|tm_gmt
parameter_list|)
block|{
name|int
name|n
decl_stmt|;
name|char
modifier|*
name|end
decl_stmt|;
name|unsigned
name|long
name|num
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
comment|/* 	 * Seconds since 1970? We trigger on that for anything after Jan 1, 2000 	 */
if|if
condition|(
name|num
operator|>
literal|946684800
condition|)
block|{
name|time_t
name|time
init|=
name|num
decl_stmt|;
if|if
condition|(
name|gmtime_r
argument_list|(
operator|&
name|time
argument_list|,
name|tm
argument_list|)
condition|)
block|{
operator|*
name|tm_gmt
operator|=
literal|1
expr_stmt|;
return|return
name|end
operator|-
name|date
return|;
block|}
block|}
comment|/* 	 * Check for special formats: num[:-/]num[same]num 	 */
switch|switch
condition|(
operator|*
name|end
condition|)
block|{
case|case
literal|':'
case|:
case|case
literal|'/'
case|:
case|case
literal|'-'
case|:
if|if
condition|(
name|isdigit
argument_list|(
name|end
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
name|int
name|match
init|=
name|match_multi_number
argument_list|(
name|num
argument_list|,
operator|*
name|end
argument_list|,
name|date
argument_list|,
name|end
argument_list|,
name|tm
argument_list|)
decl_stmt|;
if|if
condition|(
name|match
condition|)
return|return
name|match
return|;
block|}
block|}
comment|/* 	 * None of the special formats? Try to guess what 	 * the number meant. We use the number of digits 	 * to make a more educated guess.. 	 */
name|n
operator|=
literal|0
expr_stmt|;
do|do
block|{
name|n
operator|++
expr_stmt|;
block|}
do|while
condition|(
name|isdigit
argument_list|(
name|date
index|[
name|n
index|]
argument_list|)
condition|)
do|;
comment|/* Four-digit year or a timezone? */
if|if
condition|(
name|n
operator|==
literal|4
condition|)
block|{
if|if
condition|(
name|num
operator|<=
literal|1200
operator|&&
operator|*
name|offset
operator|==
operator|-
literal|1
condition|)
block|{
name|unsigned
name|int
name|minutes
init|=
name|num
operator|%
literal|100
decl_stmt|;
name|unsigned
name|int
name|hours
init|=
name|num
operator|/
literal|100
decl_stmt|;
operator|*
name|offset
operator|=
name|hours
operator|*
literal|60
operator|+
name|minutes
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|num
operator|>
literal|1900
operator|&&
name|num
operator|<
literal|2100
condition|)
name|tm
operator|->
name|tm_year
operator|=
name|num
operator|-
literal|1900
expr_stmt|;
return|return
name|n
return|;
block|}
comment|/* 	 * NOTE! We will give precedence to day-of-month over month or 	 * year numebers in the 1-12 range. So 05 is always "mday 5", 	 * unless we already have a mday.. 	 * 	 * IOW, 01 Apr 05 parses as "April 1st, 2005". 	 */
if|if
condition|(
name|num
operator|>
literal|0
operator|&&
name|num
operator|<
literal|32
operator|&&
name|tm
operator|->
name|tm_mday
operator|<
literal|0
condition|)
block|{
name|tm
operator|->
name|tm_mday
operator|=
name|num
expr_stmt|;
return|return
name|n
return|;
block|}
comment|/* Two-digit year? */
if|if
condition|(
name|n
operator|==
literal|2
operator|&&
name|tm
operator|->
name|tm_year
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|num
operator|<
literal|10
operator|&&
name|tm
operator|->
name|tm_mday
operator|>=
literal|0
condition|)
block|{
name|tm
operator|->
name|tm_year
operator|=
name|num
operator|+
literal|100
expr_stmt|;
return|return
name|n
return|;
block|}
if|if
condition|(
name|num
operator|>=
literal|70
condition|)
block|{
name|tm
operator|->
name|tm_year
operator|=
name|num
expr_stmt|;
return|return
name|n
return|;
block|}
block|}
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
block|}
elseif|else
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
block|}
elseif|else
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
block|}
elseif|else
if|if
condition|(
name|num
operator|>
literal|0
operator|&&
name|num
operator|<
literal|13
condition|)
block|{
name|tm
operator|->
name|tm_mon
operator|=
name|num
operator|-
literal|1
expr_stmt|;
block|}
return|return
name|n
return|;
block|}
end_function

begin_function
DECL|function|match_tz
specifier|static
name|int
name|match_tz
parameter_list|(
specifier|const
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
name|int
name|n
init|=
name|end
operator|-
name|date
operator|-
literal|1
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
comment|/* 	 * Don't accept any random crap.. At least 3 digits, and 	 * a valid minute. We might want to check that the minutes 	 * are divisible by 30 or something too. 	 */
if|if
condition|(
name|min
operator|<
literal|60
operator|&&
name|n
operator|>
literal|2
condition|)
block|{
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
block|}
return|return
name|end
operator|-
name|date
return|;
block|}
end_function

begin_function
DECL|function|date_string
specifier|static
name|int
name|date_string
parameter_list|(
name|unsigned
name|long
name|date
parameter_list|,
name|int
name|offset
parameter_list|,
name|char
modifier|*
name|buf
parameter_list|,
name|int
name|len
parameter_list|)
block|{
name|int
name|sign
init|=
literal|'+'
decl_stmt|;
if|if
condition|(
name|offset
operator|<
literal|0
condition|)
block|{
name|offset
operator|=
operator|-
name|offset
expr_stmt|;
name|sign
operator|=
literal|'-'
expr_stmt|;
block|}
return|return
name|snprintf
argument_list|(
name|buf
argument_list|,
name|len
argument_list|,
literal|"%lu %c%02d%02d"
argument_list|,
name|date
argument_list|,
name|sign
argument_list|,
name|offset
operator|/
literal|60
argument_list|,
name|offset
operator|%
literal|60
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* Gr. strptime is crap for this; it doesn't have a way to require RFC2822    (i.e. English) day/month names, and it doesn't work correctly with %z. */
end_comment

begin_function
DECL|function|parse_date
name|int
name|parse_date
parameter_list|(
specifier|const
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
decl_stmt|,
name|tm_gmt
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
name|tm_gmt
operator|=
literal|0
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
argument_list|,
operator|&
name|tm_gmt
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
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|tm_gmt
condition|)
name|then
operator|-=
name|offset
operator|*
literal|60
expr_stmt|;
return|return
name|date_string
argument_list|(
name|then
argument_list|,
name|offset
argument_list|,
name|result
argument_list|,
name|maxlen
argument_list|)
return|;
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
name|date_string
argument_list|(
name|now
argument_list|,
name|offset
argument_list|,
name|buf
argument_list|,
name|bufsize
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|update_tm
specifier|static
name|void
name|update_tm
parameter_list|(
name|struct
name|tm
modifier|*
name|tm
parameter_list|,
name|unsigned
name|long
name|sec
parameter_list|)
block|{
name|time_t
name|n
init|=
name|mktime
argument_list|(
name|tm
argument_list|)
operator|-
name|sec
decl_stmt|;
name|localtime_r
argument_list|(
operator|&
name|n
argument_list|,
name|tm
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|number_name
specifier|static
specifier|const
name|char
modifier|*
name|number_name
index|[]
init|=
block|{
literal|"zero"
block|,
literal|"one"
block|,
literal|"two"
block|,
literal|"three"
block|,
literal|"four"
block|,
literal|"five"
block|,
literal|"six"
block|,
literal|"seven"
block|,
literal|"eight"
block|,
literal|"nine"
block|,
literal|"ten"
block|, }
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|typelen
specifier|static
struct|struct
name|typelen
block|{
DECL|member|type
specifier|const
name|char
modifier|*
name|type
decl_stmt|;
DECL|member|length
name|int
name|length
decl_stmt|;
DECL|variable|typelen
block|}
name|typelen
index|[]
init|=
block|{
block|{
literal|"seconds"
block|,
literal|1
block|}
block|,
block|{
literal|"minutes"
block|,
literal|60
block|}
block|,
block|{
literal|"hours"
block|,
literal|60
operator|*
literal|60
block|}
block|,
block|{
literal|"days"
block|,
literal|24
operator|*
literal|60
operator|*
literal|60
block|}
block|,
block|{
literal|"weeks"
block|,
literal|7
operator|*
literal|24
operator|*
literal|60
operator|*
literal|60
block|}
block|,
block|{
name|NULL
block|}
block|}
struct|;
end_struct

begin_function
DECL|function|approxidate_alpha
specifier|static
specifier|const
name|char
modifier|*
name|approxidate_alpha
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
name|num
parameter_list|)
block|{
name|struct
name|typelen
modifier|*
name|tl
decl_stmt|;
specifier|const
name|char
modifier|*
name|end
init|=
name|date
decl_stmt|;
name|int
name|n
init|=
literal|1
decl_stmt|,
name|i
decl_stmt|;
while|while
condition|(
name|isalpha
argument_list|(
operator|*
operator|++
name|end
argument_list|)
condition|)
name|n
operator|++
expr_stmt|;
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
name|end
return|;
block|}
block|}
if|if
condition|(
name|match_string
argument_list|(
name|date
argument_list|,
literal|"yesterday"
argument_list|)
operator|>
literal|8
condition|)
block|{
name|update_tm
argument_list|(
name|tm
argument_list|,
literal|24
operator|*
literal|60
operator|*
literal|60
argument_list|)
expr_stmt|;
return|return
name|end
return|;
block|}
if|if
condition|(
operator|!
operator|*
name|num
condition|)
block|{
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
literal|11
condition|;
name|i
operator|++
control|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|number_name
index|[
name|i
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
name|match_string
argument_list|(
name|date
argument_list|,
name|number_name
index|[
name|i
index|]
argument_list|)
operator|==
name|len
condition|)
block|{
operator|*
name|num
operator|=
name|i
expr_stmt|;
return|return
name|end
return|;
block|}
block|}
if|if
condition|(
name|match_string
argument_list|(
name|date
argument_list|,
literal|"last"
argument_list|)
operator|==
literal|4
condition|)
operator|*
name|num
operator|=
literal|1
expr_stmt|;
return|return
name|end
return|;
block|}
name|tl
operator|=
name|typelen
expr_stmt|;
while|while
condition|(
name|tl
operator|->
name|type
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|tl
operator|->
name|type
argument_list|)
decl_stmt|;
if|if
condition|(
name|match_string
argument_list|(
name|date
argument_list|,
name|tl
operator|->
name|type
argument_list|)
operator|>=
name|len
operator|-
literal|1
condition|)
block|{
name|update_tm
argument_list|(
name|tm
argument_list|,
name|tl
operator|->
name|length
operator|*
operator|*
name|num
argument_list|)
expr_stmt|;
operator|*
name|num
operator|=
literal|0
expr_stmt|;
return|return
name|end
return|;
block|}
name|tl
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|match_string
argument_list|(
name|date
argument_list|,
literal|"months"
argument_list|)
operator|>=
literal|5
condition|)
block|{
name|int
name|n
init|=
name|tm
operator|->
name|tm_mon
operator|-
operator|*
name|num
decl_stmt|;
operator|*
name|num
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|n
operator|<
literal|0
condition|)
block|{
name|n
operator|+=
literal|12
expr_stmt|;
name|tm
operator|->
name|tm_year
operator|--
expr_stmt|;
block|}
name|tm
operator|->
name|tm_mon
operator|=
name|n
expr_stmt|;
return|return
name|end
return|;
block|}
if|if
condition|(
name|match_string
argument_list|(
name|date
argument_list|,
literal|"years"
argument_list|)
operator|>=
literal|4
condition|)
block|{
name|tm
operator|->
name|tm_year
operator|-=
operator|*
name|num
expr_stmt|;
operator|*
name|num
operator|=
literal|0
expr_stmt|;
return|return
name|end
return|;
block|}
return|return
name|end
return|;
block|}
end_function

begin_function
DECL|function|approxidate
name|unsigned
name|long
name|approxidate
parameter_list|(
specifier|const
name|char
modifier|*
name|date
parameter_list|)
block|{
name|int
name|number
init|=
literal|0
decl_stmt|;
name|struct
name|tm
name|tm
decl_stmt|,
name|now
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
name|char
name|buffer
index|[
literal|50
index|]
decl_stmt|;
if|if
condition|(
name|parse_date
argument_list|(
name|date
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|)
operator|>
literal|0
condition|)
return|return
name|strtoul
argument_list|(
name|buffer
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
return|;
name|gettimeofday
argument_list|(
operator|&
name|tv
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|localtime_r
argument_list|(
operator|&
name|tv
operator|.
name|tv_sec
argument_list|,
operator|&
name|tm
argument_list|)
expr_stmt|;
name|now
operator|=
name|tm
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|unsigned
name|char
name|c
init|=
operator|*
name|date
decl_stmt|;
if|if
condition|(
operator|!
name|c
condition|)
break|break;
name|date
operator|++
expr_stmt|;
if|if
condition|(
name|isdigit
argument_list|(
name|c
argument_list|)
condition|)
block|{
name|char
modifier|*
name|end
decl_stmt|;
name|number
operator|=
name|strtoul
argument_list|(
name|date
operator|-
literal|1
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|date
operator|=
name|end
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|isalpha
argument_list|(
name|c
argument_list|)
condition|)
name|date
operator|=
name|approxidate_alpha
argument_list|(
name|date
operator|-
literal|1
argument_list|,
operator|&
name|tm
argument_list|,
operator|&
name|number
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|number
operator|>
literal|0
operator|&&
name|number
operator|<
literal|32
condition|)
name|tm
operator|.
name|tm_mday
operator|=
name|number
expr_stmt|;
if|if
condition|(
name|tm
operator|.
name|tm_mon
operator|>
name|now
operator|.
name|tm_mon
condition|)
name|tm
operator|.
name|tm_year
operator|--
expr_stmt|;
return|return
name|mktime
argument_list|(
operator|&
name|tm
argument_list|)
return|;
block|}
end_function

end_unit

