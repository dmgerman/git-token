begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* This file has been copied from commit e7ac713d^ in the GNU grep git  * repository. A few small changes have been made to adapt the code to  * Git.  */
end_comment

begin_comment
comment|/* kwset.h - header declaring the keyword set library.    Copyright (C) 1989, 1998, 2005 Free Software Foundation, Inc.     This program is free software; you can redistribute it and/or modify    it under the terms of the GNU General Public License as published by    the Free Software Foundation; either version 2, or (at your option)    any later version.     This program is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    GNU General Public License for more details.     You should have received a copy of the GNU General Public License    along with this program; if not, write to the Free Software    Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA    02110-1301, USA.  */
end_comment

begin_comment
comment|/* Written August 1989 by Mike Haertel.    The author may be reached (Email) at the address mike@ai.mit.edu,    or (US mail) as Mike Haertel c/o Free Software Foundation. */
end_comment

begin_struct
DECL|struct|kwsmatch
struct|struct
name|kwsmatch
block|{
DECL|member|index
name|int
name|index
decl_stmt|;
comment|/* Index number of matching keyword. */
DECL|member|offset
name|size_t
name|offset
index|[
literal|1
index|]
decl_stmt|;
comment|/* Offset of each submatch. */
DECL|member|size
name|size_t
name|size
index|[
literal|1
index|]
decl_stmt|;
comment|/* Length of each submatch. */
block|}
struct|;
end_struct

begin_struct_decl
struct_decl|struct
name|kwset_t
struct_decl|;
end_struct_decl

begin_typedef
DECL|typedef|kwset_t
typedef|typedef
name|struct
name|kwset_t
modifier|*
name|kwset_t
typedef|;
end_typedef

begin_comment
comment|/* Return an opaque pointer to a newly allocated keyword set, or NULL    if enough memory cannot be obtained.  The argument if non-NULL    specifies a table of character translations to be applied to all    pattern and search text. */
end_comment

begin_function_decl
specifier|extern
name|kwset_t
name|kwsalloc
parameter_list|(
name|unsigned
name|char
specifier|const
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Incrementally extend the keyword set to include the given string.    Return NULL for success, or an error message.  Remember an index    number for each keyword included in the set. */
end_comment

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|kwsincr
parameter_list|(
name|kwset_t
parameter_list|,
name|char
specifier|const
modifier|*
parameter_list|,
name|size_t
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* When the keyword set has been completely built, prepare it for    use.  Return NULL for success, or an error message. */
end_comment

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|kwsprep
parameter_list|(
name|kwset_t
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Search through the given buffer for a member of the keyword set.    Return a pointer to the leftmost longest match found, or NULL if    no match is found.  If foundlen is non-NULL, store the length of    the matching substring in the integer it points to.  Similarly,    if foundindex is non-NULL, store the index of the particular    keyword found therein. */
end_comment

begin_function_decl
specifier|extern
name|size_t
name|kwsexec
parameter_list|(
name|kwset_t
parameter_list|,
name|char
specifier|const
modifier|*
parameter_list|,
name|size_t
parameter_list|,
name|struct
name|kwsmatch
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Deallocate the given keyword set and all its associated storage. */
end_comment

begin_function_decl
specifier|extern
name|void
name|kwsfree
parameter_list|(
name|kwset_t
parameter_list|)
function_decl|;
end_function_decl

end_unit

