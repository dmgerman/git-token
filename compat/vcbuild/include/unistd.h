begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_UNISTD_
end_ifndef

begin_define
DECL|macro|_UNISTD_
define|#
directive|define
name|_UNISTD_
end_define

begin_comment
comment|/* Win32 define for porting git*/
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_MODE_T_
end_ifndef

begin_define
DECL|macro|_MODE_T_
define|#
directive|define
name|_MODE_T_
end_define

begin_typedef
DECL|typedef|_mode_t
typedef|typedef
name|unsigned
name|short
name|_mode_t
typedef|;
end_typedef

begin_ifndef
ifndef|#
directive|ifndef
name|_NO_OLDNAMES
end_ifndef

begin_typedef
DECL|typedef|mode_t
typedef|typedef
name|_mode_t
name|mode_t
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Not _MODE_T_ */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_SSIZE_T_
end_ifndef

begin_define
DECL|macro|_SSIZE_T_
define|#
directive|define
name|_SSIZE_T_
end_define

begin_typedef
DECL|typedef|_ssize_t
typedef|typedef
name|long
name|_ssize_t
typedef|;
end_typedef

begin_ifndef
ifndef|#
directive|ifndef
name|_OFF_T_
end_ifndef

begin_define
DECL|macro|_OFF_T_
define|#
directive|define
name|_OFF_T_
end_define

begin_typedef
DECL|typedef|_off_t
typedef|typedef
name|long
name|_off_t
typedef|;
end_typedef

begin_ifndef
ifndef|#
directive|ifndef
name|_NO_OLDNAMES
end_ifndef

begin_typedef
DECL|typedef|off_t
typedef|typedef
name|_off_t
name|off_t
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Not _OFF_T_ */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NO_OLDNAMES
end_ifndef

begin_typedef
DECL|typedef|ssize_t
typedef|typedef
name|_ssize_t
name|ssize_t
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Not _SSIZE_T_ */
end_comment

begin_typedef
DECL|typedef|int8_t
typedef|typedef
name|signed
name|char
name|int8_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|uint8_t
typedef|typedef
name|unsigned
name|char
name|uint8_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|int16_t
typedef|typedef
name|short
name|int16_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|uint16_t
typedef|typedef
name|unsigned
name|short
name|uint16_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|int32_t
typedef|typedef
name|int
name|int32_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|uint32_t
typedef|typedef
name|unsigned
name|uint32_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|int64_t
typedef|typedef
name|long
name|long
name|int64_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|uint64_t
typedef|typedef
name|unsigned
name|long
name|long
name|uint64_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|intmax_t
typedef|typedef
name|long
name|long
name|intmax_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|uintmax_t
typedef|typedef
name|unsigned
name|long
name|long
name|uintmax_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|off64_t
typedef|typedef
name|int64_t
name|off64_t
typedef|;
end_typedef

begin_define
DECL|macro|INTMAX_MIN
define|#
directive|define
name|INTMAX_MIN
value|_I64_MIN
end_define

begin_define
DECL|macro|INTMAX_MAX
define|#
directive|define
name|INTMAX_MAX
value|_I64_MAX
end_define

begin_define
DECL|macro|UINTMAX_MAX
define|#
directive|define
name|UINTMAX_MAX
value|_UI64_MAX
end_define

begin_define
DECL|macro|STDOUT_FILENO
define|#
directive|define
name|STDOUT_FILENO
value|1
end_define

begin_define
DECL|macro|STDERR_FILENO
define|#
directive|define
name|STDERR_FILENO
value|2
end_define

begin_comment
comment|/* Some defines for _access nAccessMode (MS doesn't define them, but  * it doesn't seem to hurt to add them). */
end_comment

begin_define
DECL|macro|F_OK
define|#
directive|define
name|F_OK
value|0
end_define

begin_comment
DECL|macro|F_OK
comment|/* Check for file existence */
end_comment

begin_comment
comment|/* Well maybe it does hurt.  On newer versions of MSVCRT, an access mode    of 1 causes invalid parameter error. */
end_comment

begin_define
DECL|macro|X_OK
define|#
directive|define
name|X_OK
value|0
end_define

begin_comment
DECL|macro|X_OK
comment|/* MS access() doesn't check for execute permission. */
end_comment

begin_define
DECL|macro|W_OK
define|#
directive|define
name|W_OK
value|2
end_define

begin_comment
DECL|macro|W_OK
comment|/* Check for write permission */
end_comment

begin_define
DECL|macro|R_OK
define|#
directive|define
name|R_OK
value|4
end_define

begin_comment
DECL|macro|R_OK
comment|/* Check for read permission */
end_comment

begin_define
DECL|macro|_S_IFIFO
define|#
directive|define
name|_S_IFIFO
value|0x1000
end_define

begin_comment
DECL|macro|_S_IFIFO
comment|/* FIFO */
end_comment

begin_define
DECL|macro|_S_IFCHR
define|#
directive|define
name|_S_IFCHR
value|0x2000
end_define

begin_comment
DECL|macro|_S_IFCHR
comment|/* Character */
end_comment

begin_define
DECL|macro|_S_IFBLK
define|#
directive|define
name|_S_IFBLK
value|0x3000
end_define

begin_comment
DECL|macro|_S_IFBLK
comment|/* Block: Is this ever set under w32? */
end_comment

begin_define
DECL|macro|_S_IFDIR
define|#
directive|define
name|_S_IFDIR
value|0x4000
end_define

begin_comment
DECL|macro|_S_IFDIR
comment|/* Directory */
end_comment

begin_define
DECL|macro|_S_IFREG
define|#
directive|define
name|_S_IFREG
value|0x8000
end_define

begin_comment
DECL|macro|_S_IFREG
comment|/* Regular */
end_comment

begin_define
DECL|macro|_S_IFMT
define|#
directive|define
name|_S_IFMT
value|0xF000
end_define

begin_comment
DECL|macro|_S_IFMT
comment|/* File type mask */
end_comment

begin_define
DECL|macro|_S_IXUSR
define|#
directive|define
name|_S_IXUSR
value|_S_IEXEC
end_define

begin_define
DECL|macro|_S_IWUSR
define|#
directive|define
name|_S_IWUSR
value|_S_IWRITE
end_define

begin_define
DECL|macro|_S_IRUSR
define|#
directive|define
name|_S_IRUSR
value|_S_IREAD
end_define

begin_define
DECL|macro|_S_ISDIR
define|#
directive|define
name|_S_ISDIR
parameter_list|(
name|m
parameter_list|)
value|(((m)& _S_IFMT) == _S_IFDIR)
end_define

begin_define
DECL|macro|S_IFIFO
define|#
directive|define
name|S_IFIFO
value|_S_IFIFO
end_define

begin_define
DECL|macro|S_IFCHR
define|#
directive|define
name|S_IFCHR
value|_S_IFCHR
end_define

begin_define
DECL|macro|S_IFBLK
define|#
directive|define
name|S_IFBLK
value|_S_IFBLK
end_define

begin_define
DECL|macro|S_IFDIR
define|#
directive|define
name|S_IFDIR
value|_S_IFDIR
end_define

begin_define
DECL|macro|S_IFREG
define|#
directive|define
name|S_IFREG
value|_S_IFREG
end_define

begin_define
DECL|macro|S_IFMT
define|#
directive|define
name|S_IFMT
value|_S_IFMT
end_define

begin_define
DECL|macro|S_IEXEC
define|#
directive|define
name|S_IEXEC
value|_S_IEXEC
end_define

begin_define
DECL|macro|S_IWRITE
define|#
directive|define
name|S_IWRITE
value|_S_IWRITE
end_define

begin_define
DECL|macro|S_IREAD
define|#
directive|define
name|S_IREAD
value|_S_IREAD
end_define

begin_define
DECL|macro|S_IRWXU
define|#
directive|define
name|S_IRWXU
value|_S_IRWXU
end_define

begin_define
DECL|macro|S_IXUSR
define|#
directive|define
name|S_IXUSR
value|_S_IXUSR
end_define

begin_define
DECL|macro|S_IWUSR
define|#
directive|define
name|S_IWUSR
value|_S_IWUSR
end_define

begin_define
DECL|macro|S_IRUSR
define|#
directive|define
name|S_IRUSR
value|_S_IRUSR
end_define

begin_define
DECL|macro|S_ISDIR
define|#
directive|define
name|S_ISDIR
parameter_list|(
name|m
parameter_list|)
value|(((m)& S_IFMT) == S_IFDIR)
end_define

begin_define
DECL|macro|S_ISREG
define|#
directive|define
name|S_ISREG
parameter_list|(
name|m
parameter_list|)
value|(((m)& S_IFMT) == S_IFREG)
end_define

begin_define
DECL|macro|S_ISFIFO
define|#
directive|define
name|S_ISFIFO
parameter_list|(
name|m
parameter_list|)
value|(((m)& S_IFMT) == S_IFIFO)
end_define

begin_endif
endif|#
directive|endif
end_endif

end_unit

