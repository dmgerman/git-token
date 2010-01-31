begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<string.h>
end_include

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<netdb.h>
end_include

begin_function
DECL|function|githstrerror
specifier|const
name|char
modifier|*
name|githstrerror
parameter_list|(
name|int
name|err
parameter_list|)
block|{
specifier|static
name|char
name|buffer
index|[
literal|48
index|]
decl_stmt|;
switch|switch
condition|(
name|err
condition|)
block|{
case|case
name|HOST_NOT_FOUND
case|:
return|return
literal|"Authoritative answer: host not found"
return|;
case|case
name|NO_DATA
case|:
return|return
literal|"Valid name, no data record of requested type"
return|;
case|case
name|NO_RECOVERY
case|:
return|return
literal|"Non recoverable errors, FORMERR, REFUSED, NOTIMP"
return|;
case|case
name|TRY_AGAIN
case|:
return|return
literal|"Non-authoritative \"host not found\", or SERVERFAIL"
return|;
block|}
name|sprintf
argument_list|(
name|buffer
argument_list|,
literal|"Name resolution error %d"
argument_list|,
name|err
argument_list|)
expr_stmt|;
return|return
name|buffer
return|;
block|}
end_function

end_unit

