begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"split-index.h"
end_include

begin_include
include|#
directive|include
file|"ewah/ewok.h"
end_include

begin_function
DECL|function|show_bit
specifier|static
name|void
name|show_bit
parameter_list|(
name|size_t
name|pos
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|printf
argument_list|(
literal|" %d"
argument_list|,
operator|(
name|int
operator|)
name|pos
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_main
name|int
name|cmd_main
parameter_list|(
name|int
name|ac
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
name|struct
name|split_index
modifier|*
name|si
decl_stmt|;
name|int
name|i
decl_stmt|;
name|do_read_index
argument_list|(
operator|&
name|the_index
argument_list|,
name|av
index|[
literal|1
index|]
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"own %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|the_index
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|si
operator|=
name|the_index
operator|.
name|split_index
expr_stmt|;
if|if
condition|(
operator|!
name|si
condition|)
block|{
name|printf
argument_list|(
literal|"not a split index\n"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|printf
argument_list|(
literal|"base %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|si
operator|->
name|base_sha1
argument_list|)
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|the_index
operator|.
name|cache_nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|the_index
operator|.
name|cache
index|[
name|i
index|]
decl_stmt|;
name|printf
argument_list|(
literal|"%06o %s %d\t%s\n"
argument_list|,
name|ce
operator|->
name|ce_mode
argument_list|,
name|sha1_to_hex
argument_list|(
name|ce
operator|->
name|sha1
argument_list|)
argument_list|,
name|ce_stage
argument_list|(
name|ce
argument_list|)
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
name|printf
argument_list|(
literal|"replacements:"
argument_list|)
expr_stmt|;
if|if
condition|(
name|si
operator|->
name|replace_bitmap
condition|)
name|ewah_each_bit
argument_list|(
name|si
operator|->
name|replace_bitmap
argument_list|,
name|show_bit
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\ndeletions:"
argument_list|)
expr_stmt|;
if|if
condition|(
name|si
operator|->
name|delete_bitmap
condition|)
name|ewah_each_bit
argument_list|(
name|si
operator|->
name|delete_bitmap
argument_list|,
name|show_bit
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

