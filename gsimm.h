begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|GSIMM_H
end_ifndef

begin_define
DECL|macro|GSIMM_H
define|#
directive|define
name|GSIMM_H
end_define

begin_comment
comment|/* Length of file message digest (MD) in bytes. Longer MD's are    better, but increase processing time for diminishing returns.    Must be multiple of NUM_HASHES_PER_CHAR / 8, and at least 24    for good results */
end_comment

begin_define
DECL|macro|MD_LENGTH
define|#
directive|define
name|MD_LENGTH
value|32
end_define

begin_define
DECL|macro|MD_BITS
define|#
directive|define
name|MD_BITS
value|(MD_LENGTH * 8)
end_define

begin_comment
comment|/* The MIN_FILE_SIZE indicates the absolute minimal file size that    can be processed. As indicated above, the first and last    RABIN_WINDOW_SIZE - 1 bytes are skipped.    In order to get at least an average of 12 samples    per bit in the final message digest, require at least 3 * MD_LENGTH    complete windows in the file.  */
end_comment

begin_define
DECL|macro|GB_SIMM_MIN_FILE_SIZE
define|#
directive|define
name|GB_SIMM_MIN_FILE_SIZE
value|(3 * MD_LENGTH + 2 * (RABIN_WINDOW_SIZE - 1))
end_define

begin_comment
comment|/* Limit matching algorithm to files less than 256 MB, so we can use    32 bit integers everywhere without fear of overflow. For larger    files we should add logic to mmap the file by piece and accumulate    the frequency counts. */
end_comment

begin_define
DECL|macro|GB_SIMM_MAX_FILE_SIZE
define|#
directive|define
name|GB_SIMM_MAX_FILE_SIZE
value|(256*1024*1024 - 1)
end_define

begin_function_decl
name|void
name|gb_simm_process
parameter_list|(
name|u_char
modifier|*
name|data
parameter_list|,
name|unsigned
name|len
parameter_list|,
name|u_char
modifier|*
name|md
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|double
name|gb_simm_score
parameter_list|(
name|u_char
modifier|*
name|l
parameter_list|,
name|u_char
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

