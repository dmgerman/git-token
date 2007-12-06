# The default target of this Makefile is...
all::

# Define V=1 to have a more verbose compile.
#
# Define NO_OPENSSL environment variable if you do not have OpenSSL.
# This also implies MOZILLA_SHA1.
#
# Define NO_CURL if you do not have curl installed.  git-http-pull and
# git-http-push are not built, and you cannot use http:// and https://
# transports.
#
# Define CURLDIR=/foo/bar if your curl header and library files are in
# /foo/bar/include and /foo/bar/lib directories.
#
# Define NO_EXPAT if you do not have expat installed.  git-http-push is
# not built, and you cannot push using http:// and https:// transports.
#
# Define NO_D_INO_IN_DIRENT if you don't have d_ino in your struct dirent.
#
# Define NO_D_TYPE_IN_DIRENT if your platform defines DT_UNKNOWN but lacks
# d_type in struct dirent (latest Cygwin -- will be fixed soonish).
#
# Define NO_C99_FORMAT if your formatted IO functions (printf/scanf et.al.)
# do not support the 'size specifiers' introduced by C99, namely ll, hh,
# j, z, t. (representing long long int, char, intmax_t, size_t, ptrdiff_t).
# some C compilers supported these specifiers prior to C99 as an extension.
#
# Define NO_STRCASESTR if you don't have strcasestr.
#
# Define NO_MEMMEM if you don't have memmem.
#
# Define NO_STRLCPY if you don't have strlcpy.
#
# Define NO_STRTOUMAX if you don't have strtoumax in the C library.
# If your compiler also does not support long long or does not have
# strtoull, define NO_STRTOULL.
#
# Define NO_SETENV if you don't have setenv in the C library.
#
# Define NO_MKDTEMP if you don't have mkdtemp in the C library.
#
# Define NO_SYMLINK_HEAD if you never want .git/HEAD to be a symbolic link.
# Enable it on Windows.  By default, symrefs are still used.
#
# Define NO_SVN_TESTS if you want to skip time-consuming SVN interoperability
# tests.  These tests take up a significant amount of the total test time
# but are not needed unless you plan to talk to SVN repos.
#
# Define NO_FINK if you are building on Darwin/Mac OS X, have Fink
# installed in /sw, but don't want GIT to link against any libraries
# installed there.  If defined you may specify your own (or Fink's)
# include directories and library directories by defining CFLAGS
# and LDFLAGS appropriately.
#
# Define NO_DARWIN_PORTS if you are building on Darwin/Mac OS X,
# have DarwinPorts installed in /opt/local, but don't want GIT to
# link against any libraries installed there.  If defined you may
# specify your own (or DarwinPort's) include directories and
# library directories by defining CFLAGS and LDFLAGS appropriately.
#
# Define PPC_SHA1 environment variable when running make to make use of
# a bundled SHA1 routine optimized for PowerPC.
#
# Define ARM_SHA1 environment variable when running make to make use of
# a bundled SHA1 routine optimized for ARM.
#
# Define MOZILLA_SHA1 environment variable when running make to make use of
# a bundled SHA1 routine coming from Mozilla. It is GPL'd and should be fast
# on non-x86 architectures (e.g. PowerPC), while the OpenSSL version (default
# choice) has very fast version optimized for i586.
#
# Define NEEDS_SSL_WITH_CRYPTO if you need -lcrypto with -lssl (Darwin).
#
# Define NEEDS_LIBICONV if linking with libc is not enough (Darwin).
#
# Define NEEDS_SOCKET if linking with libc is not enough (SunOS,
# Patrick Mauritz).
#
# Define NO_MMAP if you want to avoid mmap.
#
# Define NO_PREAD if you have a problem with pread() system call (e.g.
# cygwin.dll before v1.5.22).
#
# Define NO_FAST_WORKING_DIRECTORY if accessing objects in pack files is
# generally faster on your platform than accessing the working directory.
#
# Define NO_TRUSTABLE_FILEMODE if your filesystem may claim to support
# the executable mode bit, but doesn't really do so.
#
# Define NO_IPV6 if you lack IPv6 support and getaddrinfo().
#
# Define NO_SOCKADDR_STORAGE if your platform does not have struct
# sockaddr_storage.
#
# Define NO_ICONV if your libc does not properly support iconv.
#
# Define OLD_ICONV if your library has an old iconv(), where the second
# (input buffer pointer) parameter is declared with type (const char **).
#
# Define NO_DEFLATE_BOUND if your zlib does not have deflateBound.
#
# Define NO_R_TO_GCC_LINKER if your gcc does not like "-R/path/lib"
# that tells runtime paths to dynamic libraries;
# "-Wl,-rpath=/path/lib" is used instead.
#
# Define USE_NSEC below if you want git to care about sub-second file mtimes
# and ctimes. Note that you need recent glibc (at least 2.2.4) for this, and
# it will BREAK YOUR LOCAL DIFFS! show-diff and anything using it will likely
# randomly break unless your underlying filesystem supports those sub-second
# times (my ext3 doesn't).
#
# Define USE_STDEV below if you want git to care about the underlying device
# change being considered an inode change from the update-index perspective.
#
# Define ASCIIDOC8 if you want to format documentation with AsciiDoc 8
#
# Define DOCBOOK_XSL_172 if you want to format man pages with DocBook XSL v1.72.
#
# Define NO_PERL_MAKEMAKER if you cannot use Makefiles generated by perl's
# MakeMaker (e.g. using ActiveState under Cygwin).
#
# Define NO_TCLTK if you do not want Tcl/Tk GUI.
#
# The TCL_PATH variable governs the location of the Tcl interpreter
# used to optimize git-gui for your system.  Only used if NO_TCLTK
# is not set.  Defaults to the bare 'tclsh'.
#
# The TCLTK_PATH variable governs the location of the Tcl/Tk interpreter.
# If not set it defaults to the bare 'wish'. If it is set to the empty
# string then NO_TCLTK will be forced (this is used by configure script).
#
# Define THREADED_DELTA_SEARCH if you have pthreads and wish to exploit
# parallel delta searching when packing objects.
#

GIT-VERSION-FILE: .FORCE-GIT-VERSION-FILE
	@$(SHELL_PATH) ./GIT-VERSION-GEN
-include GIT-VERSION-FILE

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
uname_M := $(shell sh -c 'uname -m 2>/dev/null || echo not')
uname_O := $(shell sh -c 'uname -o 2>/dev/null || echo not')
uname_R := $(shell sh -c 'uname -r 2>/dev/null || echo not')
uname_P := $(shell sh -c 'uname -p 2>/dev/null || echo not')

# CFLAGS and LDFLAGS are for the users to override from the command line.

CFLAGS = -g -O2 -Wall
LDFLAGS =
ALL_CFLAGS = $(CFLAGS)
ALL_LDFLAGS = $(LDFLAGS)
STRIP ?= strip

prefix = $(HOME)
bindir = $(prefix)/bin
mandir = $(prefix)/share/man
gitexecdir = $(bindir)
sharedir = $(prefix)/share
template_dir = $(sharedir)/git-core/templates
ifeq ($(prefix),/usr)
sysconfdir = /etc
else
sysconfdir = $(prefix)/etc
endif
lib = lib
ETC_GITCONFIG = $(sysconfdir)/gitconfig
# DESTDIR=

# default configuration for gitweb
GITWEB_CONFIG = gitweb_config.perl
GITWEB_HOME_LINK_STR = projects
GITWEB_SITENAME =
GITWEB_PROJECTROOT = /pub/git
GITWEB_PROJECT_MAXDEPTH = 2007
GITWEB_EXPORT_OK =
GITWEB_STRICT_EXPORT =
GITWEB_BASE_URL =
GITWEB_LIST =
GITWEB_HOMETEXT = indextext.html
GITWEB_CSS = gitweb.css
GITWEB_LOGO = git-logo.png
GITWEB_FAVICON = git-favicon.png
GITWEB_SITE_HEADER =
GITWEB_SITE_FOOTER =

export prefix bindir gitexecdir sharedir template_dir sysconfdir

CC = gcc
AR = ar
RM = rm -f
TAR = tar
FIND = find
INSTALL = install
RPMBUILD = rpmbuild
TCL_PATH = tclsh
TCLTK_PATH = wish

export TCL_PATH TCLTK_PATH

# sparse is architecture-neutral, which means that we need to tell it
# explicitly what architecture to check for. Fix this up for yours..
SPARSE_FLAGS = -D__BIG_ENDIAN__ -D__powerpc__



### --- END CONFIGURATION SECTION ---

# Those must not be GNU-specific; they are shared with perl/ which may
# be built by a different compiler. (Note that this is an artifact now
# but it still might be nice to keep that distinction.)
BASIC_CFLAGS =
BASIC_LDFLAGS =

SCRIPT_SH = \
	git-bisect.sh git-checkout.sh \
	git-clone.sh \
	git-merge-one-file.sh git-mergetool.sh git-parse-remote.sh \
	git-pull.sh git-rebase.sh git-rebase--interactive.sh \
	git-repack.sh git-request-pull.sh \
	git-sh-setup.sh \
	git-am.sh \
	git-merge.sh git-merge-stupid.sh git-merge-octopus.sh \
	git-merge-resolve.sh \
	git-lost-found.sh git-quiltimport.sh git-submodule.sh \
	git-filter-branch.sh \
	git-stash.sh

SCRIPT_PERL = \
	git-add--interactive.perl \
	git-archimport.perl git-cvsimport.perl git-relink.perl \
	git-cvsserver.perl git-remote.perl git-cvsexportcommit.perl \
	git-send-email.perl git-svn.perl

SCRIPTS = $(patsubst %.sh,%,$(SCRIPT_SH)) \
	  $(patsubst %.perl,%,$(SCRIPT_PERL)) \
	  git-instaweb

# ... and all the rest that could be moved out of bindir to gitexecdir
PROGRAMS = \
	git-fetch-pack$X \
	git-hash-object$X git-index-pack$X \
	git-fast-import$X \
	git-daemon$X \
	git-merge-index$X git-mktag$X git-mktree$X git-patch-id$X \
	git-receive-pack$X \
	git-send-pack$X git-shell$X \
	git-show-index$X \
	git-unpack-file$X \
	git-update-server-info$X \
	git-upload-pack$X \
	git-pack-redundant$X git-var$X \
	git-merge-tree$X git-imap-send$X \
	git-merge-recursive$X \
	$(EXTRA_PROGRAMS)

# Empty...
EXTRA_PROGRAMS =

BUILT_INS = \
	git-format-patch$X git-show$X git-whatchanged$X git-cherry$X \
	git-get-tar-commit-id$X git-init$X git-repo-config$X \
	git-fsck-objects$X git-cherry-pick$X git-peek-remote$X git-status$X \
	$(patsubst builtin-%.o,git-%$X,$(BUILTIN_OBJS))

# what 'all' will build and 'install' will install, in gitexecdir
ALL_PROGRAMS = $(PROGRAMS) $(SCRIPTS)

ALL_PROGRAMS += git-merge-subtree$X

# what 'all' will build but not install in gitexecdir
OTHER_PROGRAMS = git$X gitweb/gitweb.cgi

# Set paths to tools early so that they can be used for version tests.
ifndef SHELL_PATH
	SHELL_PATH = /bin/sh
endif
ifndef PERL_PATH
	PERL_PATH = /usr/bin/perl
endif

export PERL_PATH

LIB_FILE=libgit.a
XDIFF_LIB=xdiff/lib.a

LIB_H = \
	archive.h blob.h cache.h cache-tree.h commit.h csum-file.h delta.h grep.h \
	diff.h object.h pack.h pkt-line.h quote.h refs.h list-objects.h sideband.h \
	run-command.h strbuf.h tag.h tree.h git-compat-util.h revision.h \
	tree-walk.h log-tree.h dir.h path-list.h unpack-trees.h builtin.h \
	utf8.h reflog-walk.h patch-ids.h attr.h decorate.h progress.h \
	mailmap.h remote.h parse-options.h transport.h diffcore.h hash.h

DIFF_OBJS = \
	diff.o diff-lib.o diffcore-break.o diffcore-order.o \
	diffcore-pickaxe.o diffcore-rename.o tree-diff.o combine-diff.o \
	diffcore-delta.o log-tree.o

LIB_OBJS = \
	blob.o commit.o connect.o csum-file.o cache-tree.o base85.o \
	date.o diff-delta.o entry.o exec_cmd.o ident.o \
	pretty.o interpolate.o hash.o \
	lockfile.o \
	patch-ids.o \
	object.o pack-check.o pack-write.o patch-delta.o path.o pkt-line.o \
	sideband.o reachable.o reflog-walk.o \
	quote.o read-cache.o refs.o run-command.o dir.o object-refs.o \
	server-info.o setup.o sha1_file.o sha1_name.o strbuf.o \
	tag.o tree.o usage.o config.o environment.o ctype.o copy.o \
	revision.o pager.o tree-walk.o xdiff-interface.o \
	write_or_die.o trace.o list-objects.o grep.o match-trees.o \
	alloc.o merge-file.o path-list.o help.o unpack-trees.o $(DIFF_OBJS) \
	color.o wt-status.o archive-zip.o archive-tar.o shallow.o utf8.o \
	convert.o attr.o decorate.o progress.o mailmap.o symlinks.o remote.o \
	transport.o bundle.o walker.o parse-options.o

BUILTIN_OBJS = \
	builtin-add.o \
	builtin-annotate.o \
	builtin-apply.o \
	builtin-archive.o \
	builtin-blame.o \
	builtin-branch.o \
	builtin-bundle.o \
	builtin-cat-file.o \
	builtin-check-attr.o \
	builtin-checkout-index.o \
	builtin-check-ref-format.o \
	builtin-clean.o \
	builtin-commit.o \
	builtin-commit-tree.o \
	builtin-count-objects.o \
	builtin-describe.o \
	builtin-diff.o \
	builtin-diff-files.o \
	builtin-diff-index.o \
	builtin-diff-tree.o \
	builtin-fast-export.o \
	builtin-fetch.o \
	builtin-fetch-pack.o \
	builtin-fetch--tool.o \
	builtin-fmt-merge-msg.o \
	builtin-for-each-ref.o \
	builtin-fsck.o \
	builtin-gc.o \
	builtin-grep.o \
	builtin-init-db.o \
	builtin-log.o \
	builtin-ls-files.o \
	builtin-ls-tree.o \
	builtin-ls-remote.o \
	builtin-mailinfo.o \
	builtin-mailsplit.o \
	builtin-merge-base.o \
	builtin-merge-file.o \
	builtin-merge-ours.o \
	builtin-mv.o \
	builtin-name-rev.o \
	builtin-pack-objects.o \
	builtin-prune.o \
	builtin-prune-packed.o \
	builtin-push.o \
	builtin-read-tree.o \
	builtin-reflog.o \
	builtin-send-pack.o \
	builtin-config.o \
	builtin-rerere.o \
	builtin-reset.o \
	builtin-rev-list.o \
	builtin-rev-parse.o \
	builtin-revert.o \
	builtin-rm.o \
	builtin-shortlog.o \
	builtin-show-branch.o \
	builtin-stripspace.o \
	builtin-symbolic-ref.o \
	builtin-tag.o \
	builtin-tar-tree.o \
	builtin-unpack-objects.o \
	builtin-update-index.o \
	builtin-update-ref.o \
	builtin-upload-archive.o \
	builtin-verify-pack.o \
	builtin-verify-tag.o \
	builtin-write-tree.o \
	builtin-show-ref.o \
	builtin-pack-refs.o

GITLIBS = $(LIB_FILE) $(XDIFF_LIB)
EXTLIBS =

#
# Platform specific tweaks
#

# We choose to avoid "if .. else if .. else .. endif endif"
# because maintaining the nesting to match is a pain.  If
# we had "elif" things would have been much nicer...

ifeq ($(uname_S),Linux)
	NO_STRLCPY = YesPlease
endif
ifeq ($(uname_S),GNU/kFreeBSD)
	NO_STRLCPY = YesPlease
endif
ifeq ($(uname_S),Darwin)
	NEEDS_SSL_WITH_CRYPTO = YesPlease
	NEEDS_LIBICONV = YesPlease
	ifneq ($(shell expr "$(uname_R)" : '9\.'),2)
		OLD_ICONV = UnfortunatelyYes
	endif
	NO_STRLCPY = YesPlease
	NO_MEMMEM = YesPlease
endif
ifeq ($(uname_S),SunOS)
	NEEDS_SOCKET = YesPlease
	NEEDS_NSL = YesPlease
	SHELL_PATH = /bin/bash
	NO_STRCASESTR = YesPlease
	NO_MEMMEM = YesPlease
	NO_HSTRERROR = YesPlease
	NO_MKDTEMP = YesPlease
	ifeq ($(uname_R),5.8)
		NEEDS_LIBICONV = YesPlease
		NO_UNSETENV = YesPlease
		NO_SETENV = YesPlease
		NO_C99_FORMAT = YesPlease
		NO_STRTOUMAX = YesPlease
	endif
	ifeq ($(uname_R),5.9)
		NO_UNSETENV = YesPlease
		NO_SETENV = YesPlease
		NO_C99_FORMAT = YesPlease
		NO_STRTOUMAX = YesPlease
	endif
	INSTALL = ginstall
	TAR = gtar
	BASIC_CFLAGS += -D__EXTENSIONS__
endif
ifeq ($(uname_O),Cygwin)
	NO_D_TYPE_IN_DIRENT = YesPlease
	NO_D_INO_IN_DIRENT = YesPlease
	NO_STRCASESTR = YesPlease
	NO_MEMMEM = YesPlease
	NO_SYMLINK_HEAD = YesPlease
	NEEDS_LIBICONV = YesPlease
	NO_FAST_WORKING_DIRECTORY = UnfortunatelyYes
	NO_TRUSTABLE_FILEMODE = UnfortunatelyYes
	OLD_ICONV = UnfortunatelyYes
	# There are conflicting reports about this.
	# On some boxes NO_MMAP is needed, and not so elsewhere.
	# Try commenting this out if you suspect MMAP is more efficient
	NO_MMAP = YesPlease
	NO_IPV6 = YesPlease
	X = .exe
endif
ifeq ($(uname_S),FreeBSD)
	NEEDS_LIBICONV = YesPlease
	NO_MEMMEM = YesPlease
	BASIC_CFLAGS += -I/usr/local/include
	BASIC_LDFLAGS += -L/usr/local/lib
endif
ifeq ($(uname_S),OpenBSD)
	NO_STRCASESTR = YesPlease
	NO_MEMMEM = YesPlease
	NEEDS_LIBICONV = YesPlease
	BASIC_CFLAGS += -I/usr/local/include
	BASIC_LDFLAGS += -L/usr/local/lib
endif
ifeq ($(uname_S),NetBSD)
	ifeq ($(shell expr "$(uname_R)" : '[01]\.'),2)
		NEEDS_LIBICONV = YesPlease
	endif
	BASIC_CFLAGS += -I/usr/pkg/include
	BASIC_LDFLAGS += -L/usr/pkg/lib
	ALL_LDFLAGS += -Wl,-rpath,/usr/pkg/lib
endif
ifeq ($(uname_S),AIX)
	NO_STRCASESTR=YesPlease
	NO_MEMMEM = YesPlease
	NO_STRLCPY = YesPlease
	NEEDS_LIBICONV=YesPlease
endif
ifeq ($(uname_S),GNU)
	# GNU/Hurd
	NO_STRLCPY=YesPlease
endif
ifeq ($(uname_S),IRIX64)
	NO_IPV6=YesPlease
	NO_SETENV=YesPlease
	NO_STRCASESTR=YesPlease
	NO_MEMMEM = YesPlease
	NO_STRLCPY = YesPlease
	NO_SOCKADDR_STORAGE=YesPlease
	SHELL_PATH=/usr/gnu/bin/bash
	BASIC_CFLAGS += -DPATH_MAX=1024
	# for now, build 32-bit version
	BASIC_LDFLAGS += -L/usr/lib32
endif
ifneq (,$(findstring arm,$(uname_M)))
	ARM_SHA1 = YesPlease
endif

-include config.mak.autogen
-include config.mak

ifeq ($(uname_S),Darwin)
	ifndef NO_FINK
		ifeq ($(shell test -d /sw/lib && echo y),y)
			BASIC_CFLAGS += -I/sw/include
			BASIC_LDFLAGS += -L/sw/lib
		endif
	endif
	ifndef NO_DARWIN_PORTS
		ifeq ($(shell test -d /opt/local/lib && echo y),y)
			BASIC_CFLAGS += -I/opt/local/include
			BASIC_LDFLAGS += -L/opt/local/lib
		endif
	endif
endif

ifdef NO_R_TO_GCC_LINKER
	# Some gcc does not accept and pass -R to the linker to specify
	# the runtime dynamic library path.
	CC_LD_DYNPATH = -Wl,-rpath=
else
	CC_LD_DYNPATH = -R
endif

ifdef NO_CURL
	BASIC_CFLAGS += -DNO_CURL
else
	ifdef CURLDIR
		# Try "-Wl,-rpath=$(CURLDIR)/$(lib)" in such a case.
		BASIC_CFLAGS += -I$(CURLDIR)/include
		CURL_LIBCURL = -L$(CURLDIR)/$(lib) $(CC_LD_DYNPATH)$(CURLDIR)/$(lib) -lcurl
	else
		CURL_LIBCURL = -lcurl
	endif
	BUILTIN_OBJS += builtin-http-fetch.o
	EXTLIBS += $(CURL_LIBCURL)
	LIB_OBJS += http.o http-walker.o
	curl_check := $(shell (echo 070908; curl-config --vernum) | sort -r | sed -ne 2p)
	ifeq "$(curl_check)" "070908"
		ifndef NO_EXPAT
			PROGRAMS += git-http-push$X
		endif
	endif
	ifndef NO_EXPAT
		EXPAT_LIBEXPAT = -lexpat
	endif
endif

ifdef ZLIB_PATH
	BASIC_CFLAGS += -I$(ZLIB_PATH)/include
	EXTLIBS += -L$(ZLIB_PATH)/$(lib) $(CC_LD_DYNPATH)$(ZLIB_PATH)/$(lib)
endif
EXTLIBS += -lz

ifndef NO_OPENSSL
	OPENSSL_LIBSSL = -lssl
	ifdef OPENSSLDIR
		BASIC_CFLAGS += -I$(OPENSSLDIR)/include
		OPENSSL_LINK = -L$(OPENSSLDIR)/$(lib) $(CC_LD_DYNPATH)$(OPENSSLDIR)/$(lib)
	else
		OPENSSL_LINK =
	endif
else
	BASIC_CFLAGS += -DNO_OPENSSL
	MOZILLA_SHA1 = 1
	OPENSSL_LIBSSL =
endif
ifdef NEEDS_SSL_WITH_CRYPTO
	LIB_4_CRYPTO = $(OPENSSL_LINK) -lcrypto -lssl
else
	LIB_4_CRYPTO = $(OPENSSL_LINK) -lcrypto
endif
ifdef NEEDS_LIBICONV
	ifdef ICONVDIR
		BASIC_CFLAGS += -I$(ICONVDIR)/include
		ICONV_LINK = -L$(ICONVDIR)/$(lib) $(CC_LD_DYNPATH)$(ICONVDIR)/$(lib)
	else
		ICONV_LINK =
	endif
	EXTLIBS += $(ICONV_LINK) -liconv
endif
ifdef NEEDS_SOCKET
	EXTLIBS += -lsocket
endif
ifdef NEEDS_NSL
	EXTLIBS += -lnsl
endif
ifdef NO_D_TYPE_IN_DIRENT
	BASIC_CFLAGS += -DNO_D_TYPE_IN_DIRENT
endif
ifdef NO_D_INO_IN_DIRENT
	BASIC_CFLAGS += -DNO_D_INO_IN_DIRENT
endif
ifdef NO_C99_FORMAT
	BASIC_CFLAGS += -DNO_C99_FORMAT
endif
ifdef NO_SYMLINK_HEAD
	BASIC_CFLAGS += -DNO_SYMLINK_HEAD
endif
ifdef NO_STRCASESTR
	COMPAT_CFLAGS += -DNO_STRCASESTR
	COMPAT_OBJS += compat/strcasestr.o
endif
ifdef NO_STRLCPY
	COMPAT_CFLAGS += -DNO_STRLCPY
	COMPAT_OBJS += compat/strlcpy.o
endif
ifdef NO_STRTOUMAX
	COMPAT_CFLAGS += -DNO_STRTOUMAX
	COMPAT_OBJS += compat/strtoumax.o
endif
ifdef NO_STRTOULL
	COMPAT_CFLAGS += -DNO_STRTOULL
endif
ifdef NO_SETENV
	COMPAT_CFLAGS += -DNO_SETENV
	COMPAT_OBJS += compat/setenv.o
endif
ifdef NO_MKDTEMP
	COMPAT_CFLAGS += -DNO_MKDTEMP
	COMPAT_OBJS += compat/mkdtemp.o
endif
ifdef NO_UNSETENV
	COMPAT_CFLAGS += -DNO_UNSETENV
	COMPAT_OBJS += compat/unsetenv.o
endif
ifdef NO_MMAP
	COMPAT_CFLAGS += -DNO_MMAP
	COMPAT_OBJS += compat/mmap.o
endif
ifdef NO_PREAD
	COMPAT_CFLAGS += -DNO_PREAD
	COMPAT_OBJS += compat/pread.o
endif
ifdef NO_FAST_WORKING_DIRECTORY
	BASIC_CFLAGS += -DNO_FAST_WORKING_DIRECTORY
endif
ifdef NO_TRUSTABLE_FILEMODE
	BASIC_CFLAGS += -DNO_TRUSTABLE_FILEMODE
endif
ifdef NO_IPV6
	BASIC_CFLAGS += -DNO_IPV6
endif
ifdef NO_SOCKADDR_STORAGE
ifdef NO_IPV6
	BASIC_CFLAGS += -Dsockaddr_storage=sockaddr_in
else
	BASIC_CFLAGS += -Dsockaddr_storage=sockaddr_in6
endif
endif
ifdef NO_INET_NTOP
	LIB_OBJS += compat/inet_ntop.o
endif
ifdef NO_INET_PTON
	LIB_OBJS += compat/inet_pton.o
endif

ifdef NO_ICONV
	BASIC_CFLAGS += -DNO_ICONV
endif

ifdef OLD_ICONV
	BASIC_CFLAGS += -DOLD_ICONV
endif

ifdef NO_DEFLATE_BOUND
	BASIC_CFLAGS += -DNO_DEFLATE_BOUND
endif

ifdef PPC_SHA1
	SHA1_HEADER = "ppc/sha1.h"
	LIB_OBJS += ppc/sha1.o ppc/sha1ppc.o
else
ifdef ARM_SHA1
	SHA1_HEADER = "arm/sha1.h"
	LIB_OBJS += arm/sha1.o arm/sha1_arm.o
else
ifdef MOZILLA_SHA1
	SHA1_HEADER = "mozilla-sha1/sha1.h"
	LIB_OBJS += mozilla-sha1/sha1.o
else
	SHA1_HEADER = <openssl/sha.h>
	EXTLIBS += $(LIB_4_CRYPTO)
endif
endif
endif
ifdef NO_PERL_MAKEMAKER
	export NO_PERL_MAKEMAKER
endif
ifdef NO_HSTRERROR
	COMPAT_CFLAGS += -DNO_HSTRERROR
	COMPAT_OBJS += compat/hstrerror.o
endif
ifdef NO_MEMMEM
	COMPAT_CFLAGS += -DNO_MEMMEM
	COMPAT_OBJS += compat/memmem.o
endif

ifdef THREADED_DELTA_SEARCH
	BASIC_CFLAGS += -DTHREADED_DELTA_SEARCH
	EXTLIBS += -lpthread
endif

ifeq ($(TCLTK_PATH),)
NO_TCLTK=NoThanks
endif

QUIET_SUBDIR0  = +$(MAKE) -C # space to separate -C and subdir
QUIET_SUBDIR1  =

ifneq ($(findstring $(MAKEFLAGS),w),w)
PRINT_DIR = --no-print-directory
else # "make -w"
NO_SUBDIR = :
endif

ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
	QUIET_CC       = @echo '   ' CC $@;
	QUIET_AR       = @echo '   ' AR $@;
	QUIET_LINK     = @echo '   ' LINK $@;
	QUIET_BUILT_IN = @echo '   ' BUILTIN $@;
	QUIET_GEN      = @echo '   ' GEN $@;
	QUIET_SUBDIR0  = +@subdir=
	QUIET_SUBDIR1  = ;$(NO_SUBDIR) echo '   ' SUBDIR $$subdir; \
			 $(MAKE) $(PRINT_DIR) -C $$subdir
	export V
	export QUIET_GEN
	export QUIET_BUILT_IN
endif
endif

ifdef ASCIIDOC8
	export ASCIIDOC8
endif

# Shell quote (do not use $(call) to accommodate ancient setups);

SHA1_HEADER_SQ = $(subst ','\'',$(SHA1_HEADER))
ETC_GITCONFIG_SQ = $(subst ','\'',$(ETC_GITCONFIG))

DESTDIR_SQ = $(subst ','\'',$(DESTDIR))
bindir_SQ = $(subst ','\'',$(bindir))
mandir_SQ = $(subst ','\'',$(mandir))
gitexecdir_SQ = $(subst ','\'',$(gitexecdir))
template_dir_SQ = $(subst ','\'',$(template_dir))
prefix_SQ = $(subst ','\'',$(prefix))

SHELL_PATH_SQ = $(subst ','\'',$(SHELL_PATH))
PERL_PATH_SQ = $(subst ','\'',$(PERL_PATH))
TCLTK_PATH_SQ = $(subst ','\'',$(TCLTK_PATH))

LIBS = $(GITLIBS) $(EXTLIBS)

BASIC_CFLAGS += -DSHA1_HEADER='$(SHA1_HEADER_SQ)' \
	$(COMPAT_CFLAGS)
LIB_OBJS += $(COMPAT_OBJS)

ALL_CFLAGS += $(BASIC_CFLAGS)
ALL_LDFLAGS += $(BASIC_LDFLAGS)

export TAR INSTALL DESTDIR SHELL_PATH


### Build rules

all:: $(ALL_PROGRAMS) $(BUILT_INS) $(OTHER_PROGRAMS)
ifneq (,$X)
	$(foreach p,$(patsubst %$X,%,$(filter %$X,$(ALL_PROGRAMS) $(BUILT_INS) git$X)), $(RM) '$p';)
endif

all::
ifndef NO_TCLTK
	$(QUIET_SUBDIR0)git-gui $(QUIET_SUBDIR1) all
	$(QUIET_SUBDIR0)gitk-git $(QUIET_SUBDIR1) all
endif
	$(QUIET_SUBDIR0)perl $(QUIET_SUBDIR1) PERL_PATH='$(PERL_PATH_SQ)' prefix='$(prefix_SQ)' all
	$(QUIET_SUBDIR0)templates $(QUIET_SUBDIR1)

strip: $(PROGRAMS) git$X
	$(STRIP) $(STRIP_OPTS) $(PROGRAMS) git$X

git.o: git.c common-cmds.h GIT-CFLAGS
	$(QUIET_CC)$(CC) -DGIT_VERSION='"$(GIT_VERSION)"' \
		$(ALL_CFLAGS) -c $(filter %.c,$^)

git$X: git.o $(BUILTIN_OBJS) $(GITLIBS)
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) -o $@ git.o \
		$(BUILTIN_OBJS) $(ALL_LDFLAGS) $(LIBS)

help.o: help.c common-cmds.h GIT-CFLAGS
	$(QUIET_CC)$(CC) -o $*.o -c $(ALL_CFLAGS) '-DGIT_MAN_PATH="$(mandir_SQ)"' $<

git-merge-subtree$X: git-merge-recursive$X
	$(QUIET_BUILT_IN)$(RM) $@ && ln git-merge-recursive$X $@

$(BUILT_INS): git$X
	$(QUIET_BUILT_IN)$(RM) $@ && ln git$X $@

common-cmds.h: ./generate-cmdlist.sh command-list.txt

common-cmds.h: $(wildcard Documentation/git-*.txt)
	$(QUIET_GEN)./generate-cmdlist.sh > $@+ && mv $@+ $@

$(patsubst %.sh,%,$(SCRIPT_SH)) : % : %.sh
	$(QUIET_GEN)$(RM) $@ $@+ && \
	sed -e '1s|#!.*/sh|#!$(SHELL_PATH_SQ)|' \
	    -e 's|@@PERL@@|$(PERL_PATH_SQ)|g' \
	    -e 's/@@GIT_VERSION@@/$(GIT_VERSION)/g' \
	    -e 's/@@NO_CURL@@/$(NO_CURL)/g' \
	    $@.sh >$@+ && \
	chmod +x $@+ && \
	mv $@+ $@

$(patsubst %.perl,%,$(SCRIPT_PERL)): perl/perl.mak

perl/perl.mak: GIT-CFLAGS perl/Makefile perl/Makefile.PL
	$(QUIET_SUBDIR0)perl $(QUIET_SUBDIR1) PERL_PATH='$(PERL_PATH_SQ)' prefix='$(prefix_SQ)' $(@F)

$(patsubst %.perl,%,$(SCRIPT_PERL)): % : %.perl
	$(QUIET_GEN)$(RM) $@ $@+ && \
	INSTLIBDIR=`MAKEFLAGS= $(MAKE) -C perl -s --no-print-directory instlibdir` && \
	sed -e '1{' \
	    -e '	s|#!.*perl|#!$(PERL_PATH_SQ)|' \
	    -e '	h' \
	    -e '	s=.*=use lib (split(/:/, $$ENV{GITPERLLIB} || "@@INSTLIBDIR@@"));=' \
	    -e '	H' \
	    -e '	x' \
	    -e '}' \
	    -e 's|@@INSTLIBDIR@@|'"$$INSTLIBDIR"'|g' \
	    -e 's/@@GIT_VERSION@@/$(GIT_VERSION)/g' \
	    $@.perl >$@+ && \
	chmod +x $@+ && \
	mv $@+ $@

gitweb/gitweb.cgi: gitweb/gitweb.perl
	$(QUIET_GEN)$(RM) $@ $@+ && \
	sed -e '1s|#!.*perl|#!$(PERL_PATH_SQ)|' \
	    -e 's|++GIT_VERSION++|$(GIT_VERSION)|g' \
	    -e 's|++GIT_BINDIR++|$(bindir)|g' \
	    -e 's|++GITWEB_CONFIG++|$(GITWEB_CONFIG)|g' \
	    -e 's|++GITWEB_HOME_LINK_STR++|$(GITWEB_HOME_LINK_STR)|g' \
	    -e 's|++GITWEB_SITENAME++|$(GITWEB_SITENAME)|g' \
	    -e 's|++GITWEB_PROJECTROOT++|$(GITWEB_PROJECTROOT)|g' \
	    -e 's|"++GITWEB_PROJECT_MAXDEPTH++"|$(GITWEB_PROJECT_MAXDEPTH)|g' \
	    -e 's|++GITWEB_EXPORT_OK++|$(GITWEB_EXPORT_OK)|g' \
	    -e 's|++GITWEB_STRICT_EXPORT++|$(GITWEB_STRICT_EXPORT)|g' \
	    -e 's|++GITWEB_BASE_URL++|$(GITWEB_BASE_URL)|g' \
	    -e 's|++GITWEB_LIST++|$(GITWEB_LIST)|g' \
	    -e 's|++GITWEB_HOMETEXT++|$(GITWEB_HOMETEXT)|g' \
	    -e 's|++GITWEB_CSS++|$(GITWEB_CSS)|g' \
	    -e 's|++GITWEB_LOGO++|$(GITWEB_LOGO)|g' \
	    -e 's|++GITWEB_FAVICON++|$(GITWEB_FAVICON)|g' \
	    -e 's|++GITWEB_SITE_HEADER++|$(GITWEB_SITE_HEADER)|g' \
	    -e 's|++GITWEB_SITE_FOOTER++|$(GITWEB_SITE_FOOTER)|g' \
	    $< >$@+ && \
	chmod +x $@+ && \
	mv $@+ $@

git-instaweb: git-instaweb.sh gitweb/gitweb.cgi gitweb/gitweb.css
	$(QUIET_GEN)$(RM) $@ $@+ && \
	sed -e '1s|#!.*/sh|#!$(SHELL_PATH_SQ)|' \
	    -e 's/@@GIT_VERSION@@/$(GIT_VERSION)/g' \
	    -e 's/@@NO_CURL@@/$(NO_CURL)/g' \
	    -e '/@@GITWEB_CGI@@/r gitweb/gitweb.cgi' \
	    -e '/@@GITWEB_CGI@@/d' \
	    -e '/@@GITWEB_CSS@@/r gitweb/gitweb.css' \
	    -e '/@@GITWEB_CSS@@/d' \
	    $@.sh > $@+ && \
	chmod +x $@+ && \
	mv $@+ $@

configure: configure.ac
	$(QUIET_GEN)$(RM) $@ $<+ && \
	sed -e 's/@@GIT_VERSION@@/$(GIT_VERSION)/g' \
	    $< > $<+ && \
	autoconf -o $@ $<+ && \
	$(RM) $<+

# These can record GIT_VERSION
git.o git.spec \
	$(patsubst %.sh,%,$(SCRIPT_SH)) \
	$(patsubst %.perl,%,$(SCRIPT_PERL)) \
	: GIT-VERSION-FILE

%.o: %.c GIT-CFLAGS
	$(QUIET_CC)$(CC) -o $*.o -c $(ALL_CFLAGS) $<
%.s: %.c GIT-CFLAGS
	$(QUIET_CC)$(CC) -S $(ALL_CFLAGS) $<
%.o: %.S
	$(QUIET_CC)$(CC) -o $*.o -c $(ALL_CFLAGS) $<

exec_cmd.o: exec_cmd.c GIT-CFLAGS
	$(QUIET_CC)$(CC) -o $*.o -c $(ALL_CFLAGS) '-DGIT_EXEC_PATH="$(gitexecdir_SQ)"' $<
builtin-init-db.o: builtin-init-db.c GIT-CFLAGS
	$(QUIET_CC)$(CC) -o $*.o -c $(ALL_CFLAGS) -DDEFAULT_GIT_TEMPLATE_DIR='"$(template_dir_SQ)"' $<

config.o: config.c GIT-CFLAGS
	$(QUIET_CC)$(CC) -o $*.o -c $(ALL_CFLAGS) -DETC_GITCONFIG='"$(ETC_GITCONFIG_SQ)"' $<

http.o: http.c GIT-CFLAGS
	$(QUIET_CC)$(CC) -o $*.o -c $(ALL_CFLAGS) -DGIT_USER_AGENT='"git/$(GIT_VERSION)"' $<

ifdef NO_EXPAT
http-walker.o: http-walker.c http.h GIT-CFLAGS
	$(QUIET_CC)$(CC) -o $*.o -c $(ALL_CFLAGS) -DNO_EXPAT $<
endif

git-%$X: %.o $(GITLIBS)
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) -o $@ $(ALL_LDFLAGS) $(filter %.o,$^) $(LIBS)

git-imap-send$X: imap-send.o $(LIB_FILE)

http.o http-walker.o http-push.o: http.h

git-http-push$X: revision.o http.o http-push.o $(GITLIBS)
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) -o $@ $(ALL_LDFLAGS) $(filter %.o,$^) \
		$(LIBS) $(CURL_LIBCURL) $(EXPAT_LIBEXPAT)

$(LIB_OBJS) $(BUILTIN_OBJS): $(LIB_H)
$(patsubst git-%$X,%.o,$(PROGRAMS)): $(LIB_H) $(wildcard */*.h)
builtin-revert.o builtin-runstatus.o wt-status.o: wt-status.h

$(LIB_FILE): $(LIB_OBJS)
	$(QUIET_AR)$(RM) $@ && $(AR) rcs $@ $(LIB_OBJS)

XDIFF_OBJS=xdiff/xdiffi.o xdiff/xprepare.o xdiff/xutils.o xdiff/xemit.o \
	xdiff/xmerge.o
$(XDIFF_OBJS): xdiff/xinclude.h xdiff/xmacros.h xdiff/xdiff.h xdiff/xtypes.h \
	xdiff/xutils.h xdiff/xprepare.h xdiff/xdiffi.h xdiff/xemit.h

$(XDIFF_LIB): $(XDIFF_OBJS)
	$(QUIET_AR)$(RM) $@ && $(AR) rcs $@ $(XDIFF_OBJS)


doc:
	$(MAKE) -C Documentation all

info:
	$(MAKE) -C Documentation info

TAGS:
	$(RM) TAGS
	$(FIND) . -name '*.[hcS]' -print | xargs etags -a

tags:
	$(RM) tags
	$(FIND) . -name '*.[hcS]' -print | xargs ctags -a

cscope:
	$(RM) cscope*
	$(FIND) . -name '*.[hcS]' -print | xargs cscope -b

### Detect prefix changes
TRACK_CFLAGS = $(subst ','\'',$(ALL_CFLAGS)):\
             $(bindir_SQ):$(gitexecdir_SQ):$(template_dir_SQ):$(prefix_SQ)

GIT-CFLAGS: .FORCE-GIT-CFLAGS
	@FLAGS='$(TRACK_CFLAGS)'; \
	    if test x"$$FLAGS" != x"`cat GIT-CFLAGS 2>/dev/null`" ; then \
		echo 1>&2 "    * new build flags or prefix"; \
		echo "$$FLAGS" >GIT-CFLAGS; \
            fi

### Detect Tck/Tk interpreter path changes
ifndef NO_TCLTK
TRACK_VARS = $(subst ','\'',-DTCLTK_PATH='$(TCLTK_PATH_SQ)')

GIT-GUI-VARS: .FORCE-GIT-GUI-VARS
	@VARS='$(TRACK_VARS)'; \
	    if test x"$$VARS" != x"`cat $@ 2>/dev/null`" ; then \
		echo 1>&2 "    * new Tcl/Tk interpreter location"; \
		echo "$$VARS" >$@; \
            fi

.PHONY: .FORCE-GIT-GUI-VARS
endif

### Testing rules

TEST_PROGRAMS = test-chmtime$X test-genrandom$X test-date$X test-delta$X test-sha1$X test-match-trees$X test-absolute-path$X test-parse-options$X

all:: $(TEST_PROGRAMS)

# GNU make supports exporting all variables by "export" without parameters.
# However, the environment gets quite big, and some programs have problems
# with that.

export NO_SVN_TESTS

test: all
	$(MAKE) -C t/ all

test-date$X: date.o ctype.o

test-delta$X: diff-delta.o patch-delta.o

test-parse-options$X: parse-options.o

.PRECIOUS: $(patsubst test-%$X,test-%.o,$(TEST_PROGRAMS))

test-%$X: test-%.o $(GITLIBS)
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) -o $@ $(ALL_LDFLAGS) $(filter %.o,$^) $(LIBS)

check-sha1:: test-sha1$X
	./test-sha1.sh

check: common-cmds.h
	for i in *.c; do sparse $(ALL_CFLAGS) $(SPARSE_FLAGS) $$i || exit; done

remove-dashes:
	./fixup-builtins $(BUILT_INS)

### Installation rules

install: all
	$(INSTALL) -d -m 755 '$(DESTDIR_SQ)$(bindir_SQ)'
	$(INSTALL) -d -m 755 '$(DESTDIR_SQ)$(gitexecdir_SQ)'
	$(INSTALL) $(ALL_PROGRAMS) '$(DESTDIR_SQ)$(gitexecdir_SQ)'
	$(INSTALL) git$X '$(DESTDIR_SQ)$(bindir_SQ)'
	$(MAKE) -C templates DESTDIR='$(DESTDIR_SQ)' install
	$(MAKE) -C perl prefix='$(prefix_SQ)' install
ifndef NO_TCLTK
	$(MAKE) -C gitk-git install
	$(MAKE) -C git-gui install
endif
	if test 'z$(bindir_SQ)' != 'z$(gitexecdir_SQ)'; \
	then \
		ln -f '$(DESTDIR_SQ)$(bindir_SQ)/git$X' \
			'$(DESTDIR_SQ)$(gitexecdir_SQ)/git$X' || \
		cp '$(DESTDIR_SQ)$(bindir_SQ)/git$X' \
			'$(DESTDIR_SQ)$(gitexecdir_SQ)/git$X'; \
	fi
	$(foreach p,$(BUILT_INS), $(RM) '$(DESTDIR_SQ)$(gitexecdir_SQ)/$p' && ln '$(DESTDIR_SQ)$(gitexecdir_SQ)/git$X' '$(DESTDIR_SQ)$(gitexecdir_SQ)/$p' ;)
ifneq (,$X)
	$(foreach p,$(patsubst %$X,%,$(filter %$X,$(ALL_PROGRAMS) $(BUILT_INS) git$X)), $(RM) '$(DESTDIR_SQ)$(gitexecdir_SQ)/$p';)
endif

install-doc:
	$(MAKE) -C Documentation install

install-info:
	$(MAKE) -C Documentation install-info

quick-install-doc:
	$(MAKE) -C Documentation quick-install



### Maintainer's dist rules

git.spec: git.spec.in
	sed -e 's/@@VERSION@@/$(GIT_VERSION)/g' < $< > $@+
	mv $@+ $@

GIT_TARNAME=git-$(GIT_VERSION)
dist: git.spec git-archive configure
	./git-archive --format=tar \
		--prefix=$(GIT_TARNAME)/ HEAD^{tree} > $(GIT_TARNAME).tar
	@mkdir -p $(GIT_TARNAME)
	@cp git.spec configure $(GIT_TARNAME)
	@echo $(GIT_VERSION) > $(GIT_TARNAME)/version
	@$(MAKE) -C git-gui TARDIR=../$(GIT_TARNAME)/git-gui dist-version
	$(TAR) rf $(GIT_TARNAME).tar \
		$(GIT_TARNAME)/git.spec \
		$(GIT_TARNAME)/configure \
		$(GIT_TARNAME)/version \
		$(GIT_TARNAME)/git-gui/version
	@$(RM) -r $(GIT_TARNAME)
	gzip -f -9 $(GIT_TARNAME).tar

rpm: dist
	$(RPMBUILD) -ta $(GIT_TARNAME).tar.gz

htmldocs = git-htmldocs-$(GIT_VERSION)
manpages = git-manpages-$(GIT_VERSION)
dist-doc:
	$(RM) -r .doc-tmp-dir
	mkdir .doc-tmp-dir
	$(MAKE) -C Documentation WEBDOC_DEST=../.doc-tmp-dir install-webdoc
	cd .doc-tmp-dir && $(TAR) cf ../$(htmldocs).tar .
	gzip -n -9 -f $(htmldocs).tar
	:
	$(RM) -r .doc-tmp-dir
	mkdir -p .doc-tmp-dir/man1 .doc-tmp-dir/man5 .doc-tmp-dir/man7
	$(MAKE) -C Documentation DESTDIR=./ \
		man1dir=../.doc-tmp-dir/man1 \
		man5dir=../.doc-tmp-dir/man5 \
		man7dir=../.doc-tmp-dir/man7 \
		install
	cd .doc-tmp-dir && $(TAR) cf ../$(manpages).tar .
	gzip -n -9 -f $(manpages).tar
	$(RM) -r .doc-tmp-dir

### Cleaning rules

distclean: clean
	$(RM) configure

clean:
	$(RM) *.o mozilla-sha1/*.o arm/*.o ppc/*.o compat/*.o xdiff/*.o \
		$(LIB_FILE) $(XDIFF_LIB)
	$(RM) $(ALL_PROGRAMS) $(BUILT_INS) git$X
	$(RM) $(TEST_PROGRAMS)
	$(RM) *.spec *.pyc *.pyo */*.pyc */*.pyo common-cmds.h TAGS tags cscope*
	$(RM) -r autom4te.cache
	$(RM) config.log config.mak.autogen config.mak.append config.status config.cache
	$(RM) -r $(GIT_TARNAME) .doc-tmp-dir
	$(RM) $(GIT_TARNAME).tar.gz git-core_$(GIT_VERSION)-*.tar.gz
	$(RM) $(htmldocs).tar.gz $(manpages).tar.gz
	$(RM) gitweb/gitweb.cgi
	$(MAKE) -C Documentation/ clean
	$(MAKE) -C perl clean
	$(MAKE) -C templates/ clean
	$(MAKE) -C t/ clean
ifndef NO_TCLTK
	$(MAKE) -C gitk-git clean
	$(MAKE) -C git-gui clean
endif
	$(RM) GIT-VERSION-FILE GIT-CFLAGS GIT-GUI-VARS

.PHONY: all install clean strip
.PHONY: .FORCE-GIT-VERSION-FILE TAGS tags cscope .FORCE-GIT-CFLAGS

### Check documentation
#
check-docs::
	@(for v in $(ALL_PROGRAMS) $(BUILT_INS) git gitk; \
	do \
		case "$$v" in \
		git-merge-octopus | git-merge-ours | git-merge-recursive | \
		git-merge-resolve | git-merge-stupid | git-merge-subtree | \
		git-add--interactive | git-fsck-objects | git-init-db | \
		git-rebase--interactive | \
		git-repo-config | git-fetch--tool ) continue ;; \
		esac ; \
		test -f "Documentation/$$v.txt" || \
		echo "no doc: $$v"; \
		sed -e '/^#/d' command-list.txt | \
		grep -q "^$$v[ 	]" || \
		case "$$v" in \
		git) ;; \
		*) echo "no link: $$v";; \
		esac ; \
	done; \
	( \
		sed -e '/^#/d' \
		    -e 's/[ 	].*//' \
		    -e 's/^/listed /' command-list.txt; \
		ls -1 Documentation/git*txt | \
		sed -e 's|Documentation/|documented |' \
		    -e 's/\.txt//'; \
	) | while read how cmd; \
	do \
		case "$$how,$$cmd" in \
		*,git-citool | \
		*,git-gui | \
		*,git-help | \
		documented,gitattributes | \
		documented,gitignore | \
		documented,gitmodules | \
		documented,git-tools | \
		sentinel,not,matching,is,ok ) continue ;; \
		esac; \
		case " $(ALL_PROGRAMS) $(BUILT_INS) git gitk " in \
		*" $$cmd "*)	;; \
		*) echo "removed but $$how: $$cmd" ;; \
		esac; \
	done ) | sort

### Make sure built-ins do not have dups and listed in git.c
#
check-builtins::
	./check-builtins.sh
