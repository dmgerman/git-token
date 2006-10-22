begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"path-list.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_decl_stmt
DECL|variable|shortlog_usage
specifier|static
specifier|const
name|char
name|shortlog_usage
index|[]
init|=
literal|"git-shortlog [-n] [-s] [<commit-id>... ]\n"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|compare_by_number
specifier|static
name|int
name|compare_by_number
parameter_list|(
specifier|const
name|void
modifier|*
name|a1
parameter_list|,
specifier|const
name|void
modifier|*
name|a2
parameter_list|)
block|{
specifier|const
name|struct
name|path_list_item
modifier|*
name|i1
init|=
name|a1
decl_stmt|,
modifier|*
name|i2
init|=
name|a2
decl_stmt|;
specifier|const
name|struct
name|path_list
modifier|*
name|l1
init|=
name|i1
operator|->
name|util
decl_stmt|,
modifier|*
name|l2
init|=
name|i2
operator|->
name|util
decl_stmt|;
if|if
condition|(
name|l1
operator|->
name|nr
operator|<
name|l2
operator|->
name|nr
condition|)
return|return
operator|-
literal|1
return|;
elseif|else
if|if
condition|(
name|l1
operator|->
name|nr
operator|==
name|l2
operator|->
name|nr
condition|)
return|return
literal|0
return|;
else|else
return|return
operator|+
literal|1
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|mailmap_list
specifier|static
name|struct
name|path_list_item
name|mailmap_list
index|[]
init|=
block|{
block|{
literal|"R.Marek@sh.cvut.cz"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Rudolf Marek"
block|}
block|,
block|{
literal|"Ralf.Wildenhues@gmx.de"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Ralf Wildenhues"
block|}
block|,
block|{
literal|"aherrman@de.ibm.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Andreas Herrmann"
block|}
block|,
block|{
literal|"akpm@osdl.org"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Andrew Morton"
block|}
block|,
block|{
literal|"andrew.vasquez@qlogic.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Andrew Vasquez"
block|}
block|,
block|{
literal|"aquynh@gmail.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Nguyen Anh Quynh"
block|}
block|,
block|{
literal|"axboe@suse.de"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Jens Axboe"
block|}
block|,
block|{
literal|"blaisorblade@yahoo.it"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Paolo 'Blaisorblade' Giarrusso"
block|}
block|,
block|{
literal|"bunk@stusta.de"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Adrian Bunk"
block|}
block|,
block|{
literal|"domen@coderock.org"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Domen Puncer"
block|}
block|,
block|{
literal|"dougg@torque.net"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Douglas Gilbert"
block|}
block|,
block|{
literal|"dwmw2@shinybook.infradead.org"
block|,
operator|(
name|void
operator|*
operator|)
literal|"David Woodhouse"
block|}
block|,
block|{
literal|"ecashin@coraid.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Ed L Cashin"
block|}
block|,
block|{
literal|"felix@derklecks.de"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Felix Moeller"
block|}
block|,
block|{
literal|"fzago@systemfabricworks.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Frank Zago"
block|}
block|,
block|{
literal|"gregkh@suse.de"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Greg Kroah-Hartman"
block|}
block|,
block|{
literal|"hch@lst.de"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Christoph Hellwig"
block|}
block|,
block|{
literal|"htejun@gmail.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Tejun Heo"
block|}
block|,
block|{
literal|"jejb@mulgrave.(none)"
block|,
operator|(
name|void
operator|*
operator|)
literal|"James Bottomley"
block|}
block|,
block|{
literal|"jejb@titanic.il.steeleye.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"James Bottomley"
block|}
block|,
block|{
literal|"jgarzik@pretzel.yyz.us"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Jeff Garzik"
block|}
block|,
block|{
literal|"johnpol@2ka.mipt.ru"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Evgeniy Polyakov"
block|}
block|,
block|{
literal|"kay.sievers@vrfy.org"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Kay Sievers"
block|}
block|,
block|{
literal|"minyard@acm.org"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Corey Minyard"
block|}
block|,
block|{
literal|"mshah@teja.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Mitesh shah"
block|}
block|,
block|{
literal|"pj@ludd.ltu.se"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Peter A Jonsson"
block|}
block|,
block|{
literal|"rmps@joel.ist.utl.pt"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Rui Saraiva"
block|}
block|,
block|{
literal|"santtu.hyrkko@gmail.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Santtu Hyrkk
literal|,Av
literal|(B"
block|}
block|,
block|{
literal|"simon@thekelleys.org.uk"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Simon Kelley"
block|}
block|,
block|{
literal|"ssant@in.ibm.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Sachin P Sant"
block|}
block|,
block|{
literal|"terra@gnome.org"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Morten Welinder"
block|}
block|,
block|{
literal|"tony.luck@intel.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Tony Luck"
block|}
block|,
block|{
literal|"welinder@anemone.rentec.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Morten Welinder"
block|}
block|,
block|{
literal|"welinder@darter.rentec.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Morten Welinder"
block|}
block|,
block|{
literal|"welinder@troll.com"
block|,
operator|(
name|void
operator|*
operator|)
literal|"Morten Welinder"
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|mailmap
specifier|static
name|struct
name|path_list
name|mailmap
init|=
block|{
name|mailmap_list
block|,
sizeof|sizeof
argument_list|(
name|mailmap_list
argument_list|)
operator|/
sizeof|sizeof
argument_list|(
expr|struct
name|path_list_item
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|map_email
specifier|static
name|int
name|map_email
parameter_list|(
name|char
modifier|*
name|email
parameter_list|,
name|char
modifier|*
name|name
parameter_list|,
name|int
name|maxlen
parameter_list|)
block|{
name|char
modifier|*
name|p
decl_stmt|;
name|struct
name|path_list_item
modifier|*
name|item
decl_stmt|;
comment|/* autocomplete common developers */
name|p
operator|=
name|strchr
argument_list|(
name|email
argument_list|,
literal|'>'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|p
condition|)
return|return
literal|0
return|;
operator|*
name|p
operator|=
literal|'\0'
expr_stmt|;
name|item
operator|=
name|path_list_lookup
argument_list|(
name|email
argument_list|,
operator|&
name|mailmap
argument_list|)
expr_stmt|;
if|if
condition|(
name|item
operator|!=
name|NULL
condition|)
block|{
specifier|const
name|char
modifier|*
name|realname
init|=
operator|(
specifier|const
name|char
operator|*
operator|)
name|item
operator|->
name|util
decl_stmt|;
name|strncpy
argument_list|(
name|name
argument_list|,
name|realname
argument_list|,
name|maxlen
argument_list|)
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
DECL|function|insert_author_oneline
specifier|static
name|void
name|insert_author_oneline
parameter_list|(
name|struct
name|path_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|author
parameter_list|,
name|int
name|authorlen
parameter_list|,
specifier|const
name|char
modifier|*
name|oneline
parameter_list|,
name|int
name|onelinelen
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|dot3
init|=
literal|"/pub/scm/linux/kernel/git/"
decl_stmt|;
name|char
modifier|*
name|buffer
decl_stmt|,
modifier|*
name|p
decl_stmt|;
name|struct
name|path_list_item
modifier|*
name|item
decl_stmt|;
name|struct
name|path_list
modifier|*
name|onelines
decl_stmt|;
while|while
condition|(
name|authorlen
operator|>
literal|0
operator|&&
name|isspace
argument_list|(
name|author
index|[
name|authorlen
operator|-
literal|1
index|]
argument_list|)
condition|)
name|authorlen
operator|--
expr_stmt|;
name|buffer
operator|=
name|xmalloc
argument_list|(
name|authorlen
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|buffer
argument_list|,
name|author
argument_list|,
name|authorlen
argument_list|)
expr_stmt|;
name|buffer
index|[
name|authorlen
index|]
operator|=
literal|'\0'
expr_stmt|;
name|item
operator|=
name|path_list_insert
argument_list|(
name|buffer
argument_list|,
name|list
argument_list|)
expr_stmt|;
if|if
condition|(
name|item
operator|->
name|util
operator|==
name|NULL
condition|)
name|item
operator|->
name|util
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|path_list
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|oneline
argument_list|,
literal|"[PATCH"
argument_list|,
literal|6
argument_list|)
condition|)
block|{
name|char
modifier|*
name|eob
init|=
name|strchr
argument_list|(
name|buffer
argument_list|,
literal|']'
argument_list|)
decl_stmt|;
while|while
condition|(
name|isspace
argument_list|(
name|eob
index|[
literal|1
index|]
argument_list|)
operator|&&
name|eob
index|[
literal|1
index|]
operator|!=
literal|'\n'
condition|)
name|eob
operator|++
expr_stmt|;
if|if
condition|(
name|eob
operator|-
name|oneline
operator|<
name|onelinelen
condition|)
block|{
name|onelinelen
operator|-=
name|eob
operator|-
name|oneline
expr_stmt|;
name|oneline
operator|=
name|eob
expr_stmt|;
block|}
block|}
while|while
condition|(
name|onelinelen
operator|>
literal|0
operator|&&
name|isspace
argument_list|(
name|oneline
index|[
literal|0
index|]
argument_list|)
condition|)
block|{
name|oneline
operator|++
expr_stmt|;
name|onelinelen
operator|--
expr_stmt|;
block|}
while|while
condition|(
name|onelinelen
operator|>
literal|0
operator|&&
name|isspace
argument_list|(
name|oneline
index|[
name|onelinelen
operator|-
literal|1
index|]
argument_list|)
condition|)
name|onelinelen
operator|--
expr_stmt|;
name|buffer
operator|=
name|xmalloc
argument_list|(
name|onelinelen
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|buffer
argument_list|,
name|oneline
argument_list|,
name|onelinelen
argument_list|)
expr_stmt|;
name|buffer
index|[
name|onelinelen
index|]
operator|=
literal|'\0'
expr_stmt|;
while|while
condition|(
operator|(
name|p
operator|=
name|strstr
argument_list|(
name|buffer
argument_list|,
name|dot3
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|memcpy
argument_list|(
name|p
argument_list|,
literal|"..."
argument_list|,
literal|3
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|p
operator|+
literal|2
argument_list|,
name|p
operator|+
sizeof|sizeof
argument_list|(
name|dot3
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
name|onelines
operator|=
name|item
operator|->
name|util
expr_stmt|;
if|if
condition|(
name|onelines
operator|->
name|nr
operator|>=
name|onelines
operator|->
name|alloc
condition|)
block|{
name|onelines
operator|->
name|alloc
operator|=
name|alloc_nr
argument_list|(
name|onelines
operator|->
name|nr
argument_list|)
expr_stmt|;
name|onelines
operator|->
name|items
operator|=
name|xrealloc
argument_list|(
name|onelines
operator|->
name|items
argument_list|,
name|onelines
operator|->
name|alloc
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|path_list_item
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|onelines
operator|->
name|items
index|[
name|onelines
operator|->
name|nr
index|]
operator|.
name|util
operator|=
name|NULL
expr_stmt|;
name|onelines
operator|->
name|items
index|[
name|onelines
operator|->
name|nr
operator|++
index|]
operator|.
name|path
operator|=
name|buffer
expr_stmt|;
block|}
end_function

begin_function
DECL|function|read_from_stdin
specifier|static
name|void
name|read_from_stdin
parameter_list|(
name|struct
name|path_list
modifier|*
name|list
parameter_list|)
block|{
name|char
name|buffer
index|[
literal|1024
index|]
decl_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|,
name|stdin
argument_list|)
operator|!=
name|NULL
condition|)
block|{
name|char
modifier|*
name|bob
decl_stmt|;
if|if
condition|(
operator|(
name|buffer
index|[
literal|0
index|]
operator|==
literal|'A'
operator|||
name|buffer
index|[
literal|0
index|]
operator|==
literal|'a'
operator|)
operator|&&
operator|!
name|strncmp
argument_list|(
name|buffer
operator|+
literal|1
argument_list|,
literal|"uthor: "
argument_list|,
literal|7
argument_list|)
operator|&&
operator|(
name|bob
operator|=
name|strchr
argument_list|(
name|buffer
operator|+
literal|7
argument_list|,
literal|'<'
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|char
name|buffer2
index|[
literal|1024
index|]
decl_stmt|,
name|offset
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|map_email
argument_list|(
name|bob
operator|+
literal|1
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|)
condition|)
name|bob
operator|=
name|buffer
operator|+
name|strlen
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
else|else
block|{
name|offset
operator|=
literal|8
expr_stmt|;
while|while
condition|(
name|isspace
argument_list|(
name|bob
index|[
operator|-
literal|1
index|]
argument_list|)
condition|)
name|bob
operator|--
expr_stmt|;
block|}
while|while
condition|(
name|fgets
argument_list|(
name|buffer2
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer2
argument_list|)
argument_list|,
name|stdin
argument_list|)
operator|&&
name|buffer2
index|[
literal|0
index|]
operator|!=
literal|'\n'
condition|)
empty_stmt|;
comment|/* chomp input */
if|if
condition|(
name|fgets
argument_list|(
name|buffer2
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer2
argument_list|)
argument_list|,
name|stdin
argument_list|)
condition|)
name|insert_author_oneline
argument_list|(
name|list
argument_list|,
name|buffer
operator|+
name|offset
argument_list|,
name|bob
operator|-
name|buffer
operator|-
name|offset
argument_list|,
name|buffer2
argument_list|,
name|strlen
argument_list|(
name|buffer2
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|get_from_rev
specifier|static
name|void
name|get_from_rev
parameter_list|(
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|,
name|struct
name|path_list
modifier|*
name|list
parameter_list|)
block|{
name|char
name|scratch
index|[
literal|1024
index|]
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|prepare_revision_walk
argument_list|(
name|rev
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|commit
operator|=
name|get_revision
argument_list|(
name|rev
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|char
modifier|*
name|author
init|=
name|NULL
decl_stmt|,
modifier|*
name|oneline
decl_stmt|,
modifier|*
name|buffer
decl_stmt|;
name|int
name|authorlen
init|=
name|authorlen
decl_stmt|,
name|onelinelen
decl_stmt|;
comment|/* get author and oneline */
for|for
control|(
name|buffer
operator|=
name|commit
operator|->
name|buffer
init|;
name|buffer
operator|&&
operator|*
name|buffer
operator|!=
literal|'\0'
operator|&&
operator|*
name|buffer
operator|!=
literal|'\n'
condition|;
control|)
block|{
name|char
modifier|*
name|eol
init|=
name|strchr
argument_list|(
name|buffer
argument_list|,
literal|'\n'
argument_list|)
decl_stmt|;
if|if
condition|(
name|eol
operator|==
name|NULL
condition|)
name|eol
operator|=
name|buffer
operator|+
name|strlen
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
else|else
name|eol
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|buffer
argument_list|,
literal|"author "
argument_list|,
literal|7
argument_list|)
condition|)
block|{
name|char
modifier|*
name|bracket
init|=
name|strchr
argument_list|(
name|buffer
argument_list|,
literal|'<'
argument_list|)
decl_stmt|;
if|if
condition|(
name|bracket
operator|==
name|NULL
operator|||
name|bracket
operator|>
name|eol
condition|)
name|die
argument_list|(
literal|"Invalid commit buffer: %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|map_email
argument_list|(
name|bracket
operator|+
literal|1
argument_list|,
name|scratch
argument_list|,
sizeof|sizeof
argument_list|(
name|scratch
argument_list|)
argument_list|)
condition|)
block|{
name|author
operator|=
name|scratch
expr_stmt|;
name|authorlen
operator|=
name|strlen
argument_list|(
name|scratch
argument_list|)
expr_stmt|;
block|}
else|else
block|{
while|while
condition|(
name|bracket
index|[
operator|-
literal|1
index|]
operator|==
literal|' '
condition|)
name|bracket
operator|--
expr_stmt|;
name|author
operator|=
name|buffer
operator|+
literal|7
expr_stmt|;
name|authorlen
operator|=
name|bracket
operator|-
name|buffer
operator|-
literal|7
expr_stmt|;
block|}
block|}
name|buffer
operator|=
name|eol
expr_stmt|;
block|}
if|if
condition|(
name|author
operator|==
name|NULL
condition|)
name|die
argument_list|(
literal|"Missing author: %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|buffer
operator|==
name|NULL
operator|||
operator|*
name|buffer
operator|==
literal|'\0'
condition|)
block|{
name|oneline
operator|=
literal|"<none>"
expr_stmt|;
name|onelinelen
operator|=
sizeof|sizeof
argument_list|(
name|oneline
argument_list|)
operator|+
literal|1
expr_stmt|;
block|}
else|else
block|{
name|char
modifier|*
name|eol
decl_stmt|;
name|oneline
operator|=
name|buffer
operator|+
literal|1
expr_stmt|;
name|eol
operator|=
name|strchr
argument_list|(
name|oneline
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
name|eol
operator|==
name|NULL
condition|)
name|onelinelen
operator|=
name|strlen
argument_list|(
name|oneline
argument_list|)
expr_stmt|;
else|else
name|onelinelen
operator|=
name|eol
operator|-
name|oneline
expr_stmt|;
block|}
name|insert_author_oneline
argument_list|(
name|list
argument_list|,
name|author
argument_list|,
name|authorlen
argument_list|,
name|oneline
argument_list|,
name|onelinelen
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|cmd_shortlog
name|int
name|cmd_shortlog
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|struct
name|rev_info
name|rev
decl_stmt|;
name|struct
name|path_list
name|list
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|1
block|}
decl_stmt|;
name|int
name|i
decl_stmt|,
name|j
decl_stmt|,
name|sort_by_number
init|=
literal|0
decl_stmt|,
name|summary
init|=
literal|0
decl_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|argc
operator|=
name|setup_revisions
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
operator|&
name|rev
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
while|while
condition|(
name|argc
operator|>
literal|1
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-n"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--numbered"
argument_list|)
condition|)
name|sort_by_number
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-s"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--summary"
argument_list|)
condition|)
name|summary
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-h"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--help"
argument_list|)
condition|)
name|usage
argument_list|(
name|shortlog_usage
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
literal|"unrecognized argument: %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
block|}
if|if
condition|(
name|rev
operator|.
name|pending
operator|.
name|nr
operator|==
literal|1
condition|)
name|die
argument_list|(
literal|"Need a range!"
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|rev
operator|.
name|pending
operator|.
name|nr
operator|==
literal|0
condition|)
name|read_from_stdin
argument_list|(
operator|&
name|list
argument_list|)
expr_stmt|;
else|else
name|get_from_rev
argument_list|(
operator|&
name|rev
argument_list|,
operator|&
name|list
argument_list|)
expr_stmt|;
if|if
condition|(
name|sort_by_number
condition|)
name|qsort
argument_list|(
name|list
operator|.
name|items
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|path_list_item
argument_list|)
argument_list|,
name|list
operator|.
name|nr
argument_list|,
name|compare_by_number
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
name|list
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|path_list
modifier|*
name|onelines
init|=
name|list
operator|.
name|items
index|[
name|i
index|]
operator|.
name|util
decl_stmt|;
name|printf
argument_list|(
literal|"%s (%d):\n"
argument_list|,
name|list
operator|.
name|items
index|[
name|i
index|]
operator|.
name|path
argument_list|,
name|onelines
operator|->
name|nr
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|summary
condition|)
block|{
for|for
control|(
name|j
operator|=
name|onelines
operator|->
name|nr
operator|-
literal|1
init|;
name|j
operator|>=
literal|0
condition|;
name|j
operator|--
control|)
name|printf
argument_list|(
literal|"      %s\n"
argument_list|,
name|onelines
operator|->
name|items
index|[
name|j
index|]
operator|.
name|path
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
block|}
name|onelines
operator|->
name|strdup_paths
operator|=
literal|1
expr_stmt|;
name|path_list_clear
argument_list|(
name|onelines
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|onelines
argument_list|)
expr_stmt|;
name|list
operator|.
name|items
index|[
name|i
index|]
operator|.
name|util
operator|=
name|NULL
expr_stmt|;
block|}
name|list
operator|.
name|strdup_paths
operator|=
literal|1
expr_stmt|;
name|path_list_clear
argument_list|(
operator|&
name|list
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

