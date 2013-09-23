begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2011 John Szakmeister<john@szakmeister.net>  *               2012 Philipp A. Hartmann<pah@qo.cx>  *  *  This program is free software; you can redistribute it and/or modify  *  it under the terms of the GNU General Public License as published by  *  the Free Software Foundation; either version 2 of the License, or  *  (at your option) any later version.  *  *  This program is distributed in the hope that it will be useful,  *  but WITHOUT ANY WARRANTY; without even the implied warranty of  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  *  GNU General Public License for more details.  *  *  You should have received a copy of the GNU General Public License  *  along with this program; if not, write to the Free Software  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  */
end_comment

begin_comment
comment|/*  * Credits:  * - GNOME Keyring API handling originally written by John Szakmeister  * - ported to credential helper API by Philipp A. Hartmann  */
end_comment

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_include
include|#
directive|include
file|<glib.h>
end_include

begin_include
include|#
directive|include
file|<gnome-keyring.h>
end_include

begin_include
include|#
directive|include
file|<gnome-keyring-memory.h>
end_include

begin_comment
comment|/*  * This credential struct and API is simplified from git's credential.{h,c}  */
end_comment

begin_struct
DECL|struct|credential
struct|struct
name|credential
block|{
DECL|member|protocol
name|char
modifier|*
name|protocol
decl_stmt|;
DECL|member|host
name|char
modifier|*
name|host
decl_stmt|;
DECL|member|port
name|unsigned
name|short
name|port
decl_stmt|;
DECL|member|path
name|char
modifier|*
name|path
decl_stmt|;
DECL|member|username
name|char
modifier|*
name|username
decl_stmt|;
DECL|member|password
name|char
modifier|*
name|password
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|CREDENTIAL_INIT
define|#
directive|define
name|CREDENTIAL_INIT
define|\
value|{ NULL,NULL,0,NULL,NULL,NULL }
end_define

begin_typedef
DECL|typedef|credential_op_cb
typedef|typedef
name|int
function_decl|(
modifier|*
name|credential_op_cb
function_decl|)
parameter_list|(
name|struct
name|credential
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|credential_operation
struct|struct
name|credential_operation
block|{
DECL|member|name
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|op
name|credential_op_cb
name|op
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|CREDENTIAL_OP_END
define|#
directive|define
name|CREDENTIAL_OP_END
define|\
value|{ NULL,NULL }
end_define

begin_comment
comment|/* ----------------- GNOME Keyring functions ----------------- */
end_comment

begin_comment
comment|/* create a special keyring option string, if path is given */
end_comment

begin_function
DECL|function|keyring_object
specifier|static
name|char
modifier|*
name|keyring_object
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
if|if
condition|(
operator|!
name|c
operator|->
name|path
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|c
operator|->
name|port
condition|)
return|return
name|g_strdup_printf
argument_list|(
literal|"%s:%hd/%s"
argument_list|,
name|c
operator|->
name|host
argument_list|,
name|c
operator|->
name|port
argument_list|,
name|c
operator|->
name|path
argument_list|)
return|;
return|return
name|g_strdup_printf
argument_list|(
literal|"%s/%s"
argument_list|,
name|c
operator|->
name|host
argument_list|,
name|c
operator|->
name|path
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|keyring_get
specifier|static
name|int
name|keyring_get
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|char
modifier|*
name|object
init|=
name|NULL
decl_stmt|;
name|GList
modifier|*
name|entries
decl_stmt|;
name|GnomeKeyringNetworkPasswordData
modifier|*
name|password_data
decl_stmt|;
name|GnomeKeyringResult
name|result
decl_stmt|;
if|if
condition|(
operator|!
name|c
operator|->
name|protocol
operator|||
operator|!
operator|(
name|c
operator|->
name|host
operator|||
name|c
operator|->
name|path
operator|)
condition|)
return|return
name|EXIT_FAILURE
return|;
name|object
operator|=
name|keyring_object
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|result
operator|=
name|gnome_keyring_find_network_password_sync
argument_list|(
name|c
operator|->
name|username
argument_list|,
name|NULL
comment|/* domain */
argument_list|,
name|c
operator|->
name|host
argument_list|,
name|object
argument_list|,
name|c
operator|->
name|protocol
argument_list|,
name|NULL
comment|/* authtype */
argument_list|,
name|c
operator|->
name|port
argument_list|,
operator|&
name|entries
argument_list|)
expr_stmt|;
name|g_free
argument_list|(
name|object
argument_list|)
expr_stmt|;
if|if
condition|(
name|result
operator|==
name|GNOME_KEYRING_RESULT_NO_MATCH
condition|)
return|return
name|EXIT_SUCCESS
return|;
if|if
condition|(
name|result
operator|==
name|GNOME_KEYRING_RESULT_CANCELLED
condition|)
return|return
name|EXIT_SUCCESS
return|;
if|if
condition|(
name|result
operator|!=
name|GNOME_KEYRING_RESULT_OK
condition|)
block|{
name|g_critical
argument_list|(
literal|"%s"
argument_list|,
name|gnome_keyring_result_to_message
argument_list|(
name|result
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|EXIT_FAILURE
return|;
block|}
comment|/* pick the first one from the list */
name|password_data
operator|=
operator|(
name|GnomeKeyringNetworkPasswordData
operator|*
operator|)
name|entries
operator|->
name|data
expr_stmt|;
name|gnome_keyring_memory_free
argument_list|(
name|c
operator|->
name|password
argument_list|)
expr_stmt|;
name|c
operator|->
name|password
operator|=
name|gnome_keyring_memory_strdup
argument_list|(
name|password_data
operator|->
name|password
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|c
operator|->
name|username
condition|)
name|c
operator|->
name|username
operator|=
name|g_strdup
argument_list|(
name|password_data
operator|->
name|user
argument_list|)
expr_stmt|;
name|gnome_keyring_network_password_list_free
argument_list|(
name|entries
argument_list|)
expr_stmt|;
return|return
name|EXIT_SUCCESS
return|;
block|}
end_function

begin_function
DECL|function|keyring_store
specifier|static
name|int
name|keyring_store
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|guint32
name|item_id
decl_stmt|;
name|char
modifier|*
name|object
init|=
name|NULL
decl_stmt|;
comment|/* 	 * Sanity check that what we are storing is actually sensible. 	 * In particular, we can't make a URL without a protocol field. 	 * Without either a host or pathname (depending on the scheme), 	 * we have no primary key. And without a username and password, 	 * we are not actually storing a credential. 	 */
if|if
condition|(
operator|!
name|c
operator|->
name|protocol
operator|||
operator|!
operator|(
name|c
operator|->
name|host
operator|||
name|c
operator|->
name|path
operator|)
operator|||
operator|!
name|c
operator|->
name|username
operator|||
operator|!
name|c
operator|->
name|password
condition|)
return|return
name|EXIT_FAILURE
return|;
name|object
operator|=
name|keyring_object
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|gnome_keyring_set_network_password_sync
argument_list|(
name|GNOME_KEYRING_DEFAULT
argument_list|,
name|c
operator|->
name|username
argument_list|,
name|NULL
comment|/* domain */
argument_list|,
name|c
operator|->
name|host
argument_list|,
name|object
argument_list|,
name|c
operator|->
name|protocol
argument_list|,
name|NULL
comment|/* authtype */
argument_list|,
name|c
operator|->
name|port
argument_list|,
name|c
operator|->
name|password
argument_list|,
operator|&
name|item_id
argument_list|)
expr_stmt|;
name|g_free
argument_list|(
name|object
argument_list|)
expr_stmt|;
return|return
name|EXIT_SUCCESS
return|;
block|}
end_function

begin_function
DECL|function|keyring_erase
specifier|static
name|int
name|keyring_erase
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|char
modifier|*
name|object
init|=
name|NULL
decl_stmt|;
name|GList
modifier|*
name|entries
decl_stmt|;
name|GnomeKeyringNetworkPasswordData
modifier|*
name|password_data
decl_stmt|;
name|GnomeKeyringResult
name|result
decl_stmt|;
comment|/* 	 * Sanity check that we actually have something to match 	 * against. The input we get is a restrictive pattern, 	 * so technically a blank credential means "erase everything". 	 * But it is too easy to accidentally send this, since it is equivalent 	 * to empty input. So explicitly disallow it, and require that the 	 * pattern have some actual content to match. 	 */
if|if
condition|(
operator|!
name|c
operator|->
name|protocol
operator|&&
operator|!
name|c
operator|->
name|host
operator|&&
operator|!
name|c
operator|->
name|path
operator|&&
operator|!
name|c
operator|->
name|username
condition|)
return|return
name|EXIT_FAILURE
return|;
name|object
operator|=
name|keyring_object
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|result
operator|=
name|gnome_keyring_find_network_password_sync
argument_list|(
name|c
operator|->
name|username
argument_list|,
name|NULL
comment|/* domain */
argument_list|,
name|c
operator|->
name|host
argument_list|,
name|object
argument_list|,
name|c
operator|->
name|protocol
argument_list|,
name|NULL
comment|/* authtype */
argument_list|,
name|c
operator|->
name|port
argument_list|,
operator|&
name|entries
argument_list|)
expr_stmt|;
name|g_free
argument_list|(
name|object
argument_list|)
expr_stmt|;
if|if
condition|(
name|result
operator|==
name|GNOME_KEYRING_RESULT_NO_MATCH
condition|)
return|return
name|EXIT_SUCCESS
return|;
if|if
condition|(
name|result
operator|==
name|GNOME_KEYRING_RESULT_CANCELLED
condition|)
return|return
name|EXIT_SUCCESS
return|;
if|if
condition|(
name|result
operator|!=
name|GNOME_KEYRING_RESULT_OK
condition|)
block|{
name|g_critical
argument_list|(
literal|"%s"
argument_list|,
name|gnome_keyring_result_to_message
argument_list|(
name|result
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|EXIT_FAILURE
return|;
block|}
comment|/* pick the first one from the list (delete all matches?) */
name|password_data
operator|=
operator|(
name|GnomeKeyringNetworkPasswordData
operator|*
operator|)
name|entries
operator|->
name|data
expr_stmt|;
name|result
operator|=
name|gnome_keyring_item_delete_sync
argument_list|(
name|password_data
operator|->
name|keyring
argument_list|,
name|password_data
operator|->
name|item_id
argument_list|)
expr_stmt|;
name|gnome_keyring_network_password_list_free
argument_list|(
name|entries
argument_list|)
expr_stmt|;
if|if
condition|(
name|result
operator|!=
name|GNOME_KEYRING_RESULT_OK
condition|)
block|{
name|g_critical
argument_list|(
literal|"%s"
argument_list|,
name|gnome_keyring_result_to_message
argument_list|(
name|result
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|EXIT_FAILURE
return|;
block|}
return|return
name|EXIT_SUCCESS
return|;
block|}
end_function

begin_comment
comment|/*  * Table with helper operation callbacks, used by generic  * credential helper main function.  */
end_comment

begin_decl_stmt
DECL|variable|credential_helper_ops
specifier|static
name|struct
name|credential_operation
specifier|const
name|credential_helper_ops
index|[]
init|=
block|{
block|{
literal|"get"
block|,
name|keyring_get
block|}
block|,
block|{
literal|"store"
block|,
name|keyring_store
block|}
block|,
block|{
literal|"erase"
block|,
name|keyring_erase
block|}
block|,
name|CREDENTIAL_OP_END
block|}
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* ------------------ credential functions ------------------ */
end_comment

begin_function
DECL|function|credential_init
specifier|static
name|void
name|credential_init
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|memset
argument_list|(
name|c
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|c
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_clear
specifier|static
name|void
name|credential_clear
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|g_free
argument_list|(
name|c
operator|->
name|protocol
argument_list|)
expr_stmt|;
name|g_free
argument_list|(
name|c
operator|->
name|host
argument_list|)
expr_stmt|;
name|g_free
argument_list|(
name|c
operator|->
name|path
argument_list|)
expr_stmt|;
name|g_free
argument_list|(
name|c
operator|->
name|username
argument_list|)
expr_stmt|;
name|gnome_keyring_memory_free
argument_list|(
name|c
operator|->
name|password
argument_list|)
expr_stmt|;
name|credential_init
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_read
specifier|static
name|int
name|credential_read
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|char
modifier|*
name|buf
decl_stmt|;
name|size_t
name|line_len
decl_stmt|;
name|char
modifier|*
name|key
decl_stmt|;
name|char
modifier|*
name|value
decl_stmt|;
name|key
operator|=
name|buf
operator|=
name|gnome_keyring_memory_alloc
argument_list|(
literal|1024
argument_list|)
expr_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|buf
argument_list|,
literal|1024
argument_list|,
name|stdin
argument_list|)
condition|)
block|{
name|line_len
operator|=
name|strlen
argument_list|(
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|line_len
operator|&&
name|buf
index|[
name|line_len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|buf
index|[
operator|--
name|line_len
index|]
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
operator|!
name|line_len
condition|)
break|break;
name|value
operator|=
name|strchr
argument_list|(
name|buf
argument_list|,
literal|'='
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|value
condition|)
block|{
name|g_warning
argument_list|(
literal|"invalid credential line: %s"
argument_list|,
name|key
argument_list|)
expr_stmt|;
name|gnome_keyring_memory_free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
operator|*
name|value
operator|++
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"protocol"
argument_list|)
condition|)
block|{
name|g_free
argument_list|(
name|c
operator|->
name|protocol
argument_list|)
expr_stmt|;
name|c
operator|->
name|protocol
operator|=
name|g_strdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"host"
argument_list|)
condition|)
block|{
name|g_free
argument_list|(
name|c
operator|->
name|host
argument_list|)
expr_stmt|;
name|c
operator|->
name|host
operator|=
name|g_strdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
name|value
operator|=
name|strrchr
argument_list|(
name|c
operator|->
name|host
argument_list|,
literal|':'
argument_list|)
expr_stmt|;
if|if
condition|(
name|value
condition|)
block|{
operator|*
name|value
operator|++
operator|=
literal|'\0'
expr_stmt|;
name|c
operator|->
name|port
operator|=
name|atoi
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"path"
argument_list|)
condition|)
block|{
name|g_free
argument_list|(
name|c
operator|->
name|path
argument_list|)
expr_stmt|;
name|c
operator|->
name|path
operator|=
name|g_strdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"username"
argument_list|)
condition|)
block|{
name|g_free
argument_list|(
name|c
operator|->
name|username
argument_list|)
expr_stmt|;
name|c
operator|->
name|username
operator|=
name|g_strdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"password"
argument_list|)
condition|)
block|{
name|gnome_keyring_memory_free
argument_list|(
name|c
operator|->
name|password
argument_list|)
expr_stmt|;
name|c
operator|->
name|password
operator|=
name|gnome_keyring_memory_strdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
while|while
condition|(
operator|*
name|value
condition|)
operator|*
name|value
operator|++
operator|=
literal|'\0'
expr_stmt|;
block|}
comment|/* 		 * Ignore other lines; we don't know what they mean, but 		 * this future-proofs us when later versions of git do 		 * learn new lines, and the helpers are updated to match. 		 */
block|}
name|gnome_keyring_memory_free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|credential_write_item
specifier|static
name|void
name|credential_write_item
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|key
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
if|if
condition|(
operator|!
name|value
condition|)
return|return;
name|fprintf
argument_list|(
name|fp
argument_list|,
literal|"%s=%s\n"
argument_list|,
name|key
argument_list|,
name|value
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_write
specifier|static
name|void
name|credential_write
parameter_list|(
specifier|const
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
comment|/* only write username/password, if set */
name|credential_write_item
argument_list|(
name|stdout
argument_list|,
literal|"username"
argument_list|,
name|c
operator|->
name|username
argument_list|)
expr_stmt|;
name|credential_write_item
argument_list|(
name|stdout
argument_list|,
literal|"password"
argument_list|,
name|c
operator|->
name|password
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|usage
specifier|static
name|void
name|usage
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|struct
name|credential_operation
specifier|const
modifier|*
name|try_op
init|=
name|credential_helper_ops
decl_stmt|;
specifier|const
name|char
modifier|*
name|basename
init|=
name|strrchr
argument_list|(
name|name
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
name|basename
operator|=
operator|(
name|basename
operator|)
condition|?
name|basename
operator|+
literal|1
else|:
name|name
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"usage: %s<"
argument_list|,
name|basename
argument_list|)
expr_stmt|;
while|while
condition|(
name|try_op
operator|->
name|name
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s"
argument_list|,
operator|(
name|try_op
operator|++
operator|)
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|try_op
operator|->
name|name
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s"
argument_list|,
literal|"|"
argument_list|)
expr_stmt|;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s"
argument_list|,
literal|">\n"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
name|argv
index|[]
parameter_list|)
block|{
name|int
name|ret
init|=
name|EXIT_SUCCESS
decl_stmt|;
name|struct
name|credential_operation
specifier|const
modifier|*
name|try_op
init|=
name|credential_helper_ops
decl_stmt|;
name|struct
name|credential
name|cred
init|=
name|CREDENTIAL_INIT
decl_stmt|;
if|if
condition|(
operator|!
name|argv
index|[
literal|1
index|]
condition|)
block|{
name|usage
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|exit
argument_list|(
name|EXIT_FAILURE
argument_list|)
expr_stmt|;
block|}
name|g_set_application_name
argument_list|(
literal|"Git Credential Helper"
argument_list|)
expr_stmt|;
comment|/* lookup operation callback */
while|while
condition|(
name|try_op
operator|->
name|name
operator|&&
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|try_op
operator|->
name|name
argument_list|)
condition|)
name|try_op
operator|++
expr_stmt|;
comment|/* unsupported operation given -- ignore silently */
if|if
condition|(
operator|!
name|try_op
operator|->
name|name
operator|||
operator|!
name|try_op
operator|->
name|op
condition|)
goto|goto
name|out
goto|;
name|ret
operator|=
name|credential_read
argument_list|(
operator|&
name|cred
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
goto|goto
name|out
goto|;
comment|/* perform credential operation */
name|ret
operator|=
call|(
modifier|*
name|try_op
operator|->
name|op
call|)
argument_list|(
operator|&
name|cred
argument_list|)
expr_stmt|;
name|credential_write
argument_list|(
operator|&
name|cred
argument_list|)
expr_stmt|;
name|out
label|:
name|credential_clear
argument_list|(
operator|&
name|cred
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

end_unit

