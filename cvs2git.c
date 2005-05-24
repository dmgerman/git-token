begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * cvs2git  *  * Copyright (C) Linus Torvalds 2005  */
end_comment

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<ctype.h>
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
file|<unistd.h>
end_include

begin_decl_stmt
DECL|variable|verbose
specifier|static
name|int
name|verbose
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * This is a really stupid program that takes cvsps output, and  * generates a a long _shell_script_ that will create the GIT archive  * from it.   *  * You've been warned. I told you it was stupid.  *  * NOTE NOTE NOTE! In order to do branches correctly, this needs  * the fixed cvsps that has the "Ancestor branch" tag output.  * Hopefully David Mansfield will update his distribution soon  * enough (he's the one who wrote the patch, so at least we don't  * have to figt maintainer issues ;)  */
end_comment

begin_enum
DECL|enum|state
enum|enum
name|state
block|{
DECL|enumerator|Header
name|Header
block|,
DECL|enumerator|Log
name|Log
block|,
DECL|enumerator|Members
name|Members
block|}
enum|;
end_enum

begin_decl_stmt
DECL|variable|rcsdir
specifier|static
name|char
modifier|*
name|rcsdir
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|date
specifier|static
name|char
name|date
index|[
literal|100
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|author
specifier|static
name|char
name|author
index|[
literal|100
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|branch
specifier|static
name|char
name|branch
index|[
literal|100
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ancestor
specifier|static
name|char
name|ancestor
index|[
literal|100
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tag
specifier|static
name|char
name|tag
index|[
literal|100
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|log
specifier|static
name|char
name|log
index|[
literal|32768
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|loglen
specifier|static
name|int
name|loglen
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|initial_commit
specifier|static
name|int
name|initial_commit
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|lookup_author
specifier|static
name|void
name|lookup_author
parameter_list|(
name|char
modifier|*
name|n
parameter_list|,
name|char
modifier|*
modifier|*
name|name
parameter_list|,
name|char
modifier|*
modifier|*
name|email
parameter_list|)
block|{
comment|/* 	 * FIXME!!! I'm lazy and stupid. 	 * 	 * This could be something like 	 * 	 *	printf("lookup_author '%s'\n", n); 	 *	*name = "$author_name"; 	 *	*email = "$author_email"; 	 * 	 * and that would allow the script to do its own 	 * lookups at run-time. 	 */
operator|*
name|name
operator|=
name|n
expr_stmt|;
operator|*
name|email
operator|=
name|n
expr_stmt|;
block|}
end_function

begin_function
DECL|function|prepare_commit
specifier|static
name|void
name|prepare_commit
parameter_list|(
name|void
parameter_list|)
block|{
name|char
modifier|*
name|author_name
decl_stmt|,
modifier|*
name|author_email
decl_stmt|;
name|char
modifier|*
name|src_branch
decl_stmt|;
name|lookup_author
argument_list|(
name|author
argument_list|,
operator|&
name|author_name
argument_list|,
operator|&
name|author_email
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"export GIT_COMMITTER_NAME=%s\n"
argument_list|,
name|author_name
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"export GIT_COMMITTER_EMAIL=%s\n"
argument_list|,
name|author_email
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"export GIT_AUTHOR_NAME=%s\n"
argument_list|,
name|author_name
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"export GIT_AUTHOR_EMAIL=%s\n"
argument_list|,
name|author_email
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"export GIT_AUTHOR_DATE='%s'\n"
argument_list|,
name|date
argument_list|)
expr_stmt|;
if|if
condition|(
name|initial_commit
condition|)
return|return;
name|src_branch
operator|=
operator|*
name|ancestor
condition|?
name|ancestor
else|:
name|branch
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|src_branch
argument_list|,
literal|"HEAD"
argument_list|)
condition|)
name|src_branch
operator|=
literal|"master"
expr_stmt|;
name|printf
argument_list|(
literal|"ln -sf refs/heads/'%s' .git/HEAD\n"
argument_list|,
name|src_branch
argument_list|)
expr_stmt|;
comment|/* 	 * Even if cvsps claims an ancestor, we'll let the new 	 * branch name take precedence if it already exists 	 */
if|if
condition|(
operator|*
name|ancestor
condition|)
block|{
name|src_branch
operator|=
name|branch
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|src_branch
argument_list|,
literal|"HEAD"
argument_list|)
condition|)
name|src_branch
operator|=
literal|"master"
expr_stmt|;
name|printf
argument_list|(
literal|"[ -e .git/refs/heads/'%s' ]&& ln -sf refs/heads/'%s' .git/HEAD\n"
argument_list|,
name|src_branch
argument_list|,
name|src_branch
argument_list|)
expr_stmt|;
block|}
name|printf
argument_list|(
literal|"git-read-tree -m HEAD || exit 1\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"git-checkout-cache -f -u -a\n"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|commit
specifier|static
name|void
name|commit
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|cmit_parent
init|=
name|initial_commit
condition|?
literal|""
else|:
literal|"-p HEAD"
decl_stmt|;
specifier|const
name|char
modifier|*
name|dst_branch
decl_stmt|;
name|int
name|i
decl_stmt|;
name|printf
argument_list|(
literal|"tree=$(git-write-tree)\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"cat> .cmitmsg<<EOFMSG\n"
argument_list|)
expr_stmt|;
comment|/* Escape $ characters, and remove control characters */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|loglen
condition|;
name|i
operator|++
control|)
block|{
name|unsigned
name|char
name|c
init|=
name|log
index|[
name|i
index|]
decl_stmt|;
switch|switch
condition|(
name|c
condition|)
block|{
case|case
literal|'$'
case|:
name|putchar
argument_list|(
literal|'\\'
argument_list|)
expr_stmt|;
break|break;
case|case
literal|0
operator|...
literal|31
case|:
if|if
condition|(
name|c
operator|==
literal|'\n'
operator|||
name|c
operator|==
literal|'\t'
condition|)
break|break;
case|case
literal|128
operator|...
literal|159
case|:
continue|continue;
block|}
name|putchar
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
name|printf
argument_list|(
literal|"\nEOFMSG\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"commit=$(cat .cmitmsg | git-commit-tree $tree %s)\n"
argument_list|,
name|cmit_parent
argument_list|)
expr_stmt|;
name|dst_branch
operator|=
name|branch
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|dst_branch
argument_list|,
literal|"HEAD"
argument_list|)
condition|)
name|dst_branch
operator|=
literal|"master"
expr_stmt|;
name|printf
argument_list|(
literal|"echo $commit> .git/refs/heads/'%s'\n"
argument_list|,
name|dst_branch
argument_list|)
expr_stmt|;
operator|*
name|date
operator|=
literal|0
expr_stmt|;
operator|*
name|author
operator|=
literal|0
expr_stmt|;
operator|*
name|branch
operator|=
literal|0
expr_stmt|;
operator|*
name|ancestor
operator|=
literal|0
expr_stmt|;
operator|*
name|tag
operator|=
literal|0
expr_stmt|;
name|loglen
operator|=
literal|0
expr_stmt|;
name|initial_commit
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_rcs_name
specifier|static
name|void
name|get_rcs_name
parameter_list|(
name|char
modifier|*
name|rcspathname
parameter_list|,
name|char
modifier|*
name|name
parameter_list|,
name|char
modifier|*
name|dir
parameter_list|)
block|{
name|sprintf
argument_list|(
name|rcspathname
argument_list|,
literal|"%s/%s,v"
argument_list|,
name|rcsdir
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|access
argument_list|(
name|rcspathname
argument_list|,
name|R_OK
argument_list|)
condition|)
return|return;
name|sprintf
argument_list|(
name|rcspathname
argument_list|,
literal|"%s/Attic/%s,v"
argument_list|,
name|rcsdir
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|access
argument_list|(
name|rcspathname
argument_list|,
name|R_OK
argument_list|)
condition|)
return|return;
if|if
condition|(
name|dir
condition|)
block|{
name|sprintf
argument_list|(
name|rcspathname
argument_list|,
literal|"%s/%.*s/Attic/%s,v"
argument_list|,
name|rcsdir
argument_list|,
call|(
name|int
call|)
argument_list|(
name|dir
operator|-
name|name
argument_list|)
argument_list|,
name|name
argument_list|,
name|dir
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|access
argument_list|(
name|rcspathname
argument_list|,
name|R_OK
argument_list|)
condition|)
return|return;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Unable to find RCS file for %s\n"
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|update_file
specifier|static
name|void
name|update_file
parameter_list|(
name|char
modifier|*
name|line
parameter_list|)
block|{
specifier|static
name|char
name|rcspathname
index|[
literal|4096
index|]
decl_stmt|;
name|char
modifier|*
name|name
decl_stmt|,
modifier|*
name|version
decl_stmt|;
name|char
modifier|*
name|dir
decl_stmt|;
while|while
condition|(
name|isspace
argument_list|(
operator|*
name|line
argument_list|)
condition|)
name|line
operator|++
expr_stmt|;
name|name
operator|=
name|line
expr_stmt|;
name|line
operator|=
name|strchr
argument_list|(
name|line
argument_list|,
literal|':'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|line
condition|)
return|return;
operator|*
name|line
operator|++
operator|=
literal|0
expr_stmt|;
name|line
operator|=
name|strchr
argument_list|(
name|line
argument_list|,
literal|'>'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|line
condition|)
return|return;
operator|*
name|line
operator|++
operator|=
literal|0
expr_stmt|;
name|version
operator|=
name|line
expr_stmt|;
name|line
operator|=
name|strchr
argument_list|(
name|line
argument_list|,
literal|'('
argument_list|)
expr_stmt|;
if|if
condition|(
name|line
condition|)
block|{
comment|/* "(DEAD)" */
name|printf
argument_list|(
literal|"git-update-cache --force-remove '%s'\n"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return;
block|}
name|dir
operator|=
name|strrchr
argument_list|(
name|name
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
name|dir
condition|)
name|printf
argument_list|(
literal|"mkdir -p %.*s\n"
argument_list|,
call|(
name|int
call|)
argument_list|(
name|dir
operator|-
name|name
argument_list|)
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|get_rcs_name
argument_list|(
name|rcspathname
argument_list|,
name|name
argument_list|,
name|dir
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"co -q -p -r%s '%s'> '%s'\n"
argument_list|,
name|version
argument_list|,
name|rcspathname
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"git-update-cache --add -- '%s'\n"
argument_list|,
name|name
argument_list|)
expr_stmt|;
block|}
end_function

begin_struct
DECL|struct|hdrentry
struct|struct
name|hdrentry
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|dest
name|char
modifier|*
name|dest
decl_stmt|;
DECL|variable|hdrs
block|}
name|hdrs
index|[]
init|=
block|{
block|{
literal|"Date:"
block|,
name|date
block|}
block|,
block|{
literal|"Author:"
block|,
name|author
block|}
block|,
block|{
literal|"Branch:"
block|,
name|branch
block|}
block|,
block|{
literal|"Ancestor branch:"
block|,
name|ancestor
block|}
block|,
block|{
literal|"Tag:"
block|,
name|tag
block|}
block|,
block|{
literal|"Log:"
block|,
name|NULL
block|}
block|,
block|{
name|NULL
block|,
name|NULL
block|}
block|}
struct|;
end_struct

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
modifier|*
name|argv
parameter_list|)
block|{
specifier|static
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|enum
name|state
name|state
init|=
name|Header
decl_stmt|;
name|rcsdir
operator|=
name|getenv
argument_list|(
literal|"RCSDIR"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|rcsdir
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"I need an $RCSDIR\n"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|printf
argument_list|(
literal|"[ -d .git ]&& exit 1\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"git-init-db\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"mkdir -p .git/refs/heads\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"mkdir -p .git/refs/tags\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"ln -sf refs/heads/master .git/HEAD\n"
argument_list|)
expr_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|line
argument_list|,
sizeof|sizeof
argument_list|(
name|line
argument_list|)
argument_list|,
name|stdin
argument_list|)
operator|!=
name|NULL
condition|)
block|{
name|int
name|linelen
init|=
name|strlen
argument_list|(
name|line
argument_list|)
decl_stmt|;
while|while
condition|(
name|linelen
operator|&&
name|isspace
argument_list|(
name|line
index|[
name|linelen
operator|-
literal|1
index|]
argument_list|)
condition|)
name|line
index|[
operator|--
name|linelen
index|]
operator|=
literal|0
expr_stmt|;
switch|switch
condition|(
name|state
condition|)
block|{
name|struct
name|hdrentry
modifier|*
name|entry
decl_stmt|;
case|case
name|Header
case|:
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"# H: %s\n"
argument_list|,
name|line
argument_list|)
expr_stmt|;
for|for
control|(
name|entry
operator|=
name|hdrs
init|;
name|entry
operator|->
name|name
condition|;
name|entry
operator|++
control|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|entry
operator|->
name|name
argument_list|)
decl_stmt|;
name|char
modifier|*
name|val
decl_stmt|;
if|if
condition|(
name|memcmp
argument_list|(
name|entry
operator|->
name|name
argument_list|,
name|line
argument_list|,
name|len
argument_list|)
condition|)
continue|continue;
if|if
condition|(
operator|!
name|entry
operator|->
name|dest
condition|)
block|{
name|state
operator|=
name|Log
expr_stmt|;
break|break;
block|}
name|val
operator|=
name|line
operator|+
name|len
expr_stmt|;
name|linelen
operator|-=
name|len
expr_stmt|;
while|while
condition|(
name|isspace
argument_list|(
operator|*
name|val
argument_list|)
condition|)
block|{
name|val
operator|++
expr_stmt|;
name|linelen
operator|--
expr_stmt|;
block|}
name|memcpy
argument_list|(
name|entry
operator|->
name|dest
argument_list|,
name|val
argument_list|,
name|linelen
operator|+
literal|1
argument_list|)
expr_stmt|;
break|break;
block|}
continue|continue;
case|case
name|Log
case|:
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"# L: %s\n"
argument_list|,
name|line
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|line
argument_list|,
literal|"Members:"
argument_list|)
condition|)
block|{
while|while
condition|(
name|loglen
operator|&&
name|isspace
argument_list|(
name|log
index|[
name|loglen
operator|-
literal|1
index|]
argument_list|)
condition|)
name|log
index|[
operator|--
name|loglen
index|]
operator|=
literal|0
expr_stmt|;
name|prepare_commit
argument_list|()
expr_stmt|;
name|state
operator|=
name|Members
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|loglen
operator|+
name|linelen
operator|+
literal|5
operator|>
sizeof|sizeof
argument_list|(
name|log
argument_list|)
condition|)
continue|continue;
name|memcpy
argument_list|(
name|log
operator|+
name|loglen
argument_list|,
name|line
argument_list|,
name|linelen
argument_list|)
expr_stmt|;
name|loglen
operator|+=
name|linelen
expr_stmt|;
name|log
index|[
name|loglen
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
continue|continue;
case|case
name|Members
case|:
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"# M: %s\n"
argument_list|,
name|line
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|linelen
condition|)
block|{
name|commit
argument_list|()
expr_stmt|;
name|state
operator|=
name|Header
expr_stmt|;
continue|continue;
block|}
name|update_file
argument_list|(
name|line
argument_list|)
expr_stmt|;
continue|continue;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

