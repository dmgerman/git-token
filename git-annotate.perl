#!/usr/bin/perl
# Copyright 2006, Ryan Anderson <ryan@michonline.com>
#
# GPL v2 (See COPYING)
#
# This file is licensed under the GPL v2, or a later version
# at the discretion of Linus Torvalds.

use warnings;
use strict;
use Getopt::Long;
use POSIX qw(strftime gmtime);
use File::Basename qw(basename dirname);

sub usage() {
	print STDERR "Usage: ${\basename $0} [-s] [-S revs-file] file [ revision ]
	-l, --long
			Show long rev (Defaults off)
	-t, --time
			Show raw timestamp (Defaults off)
	-r, --rename
			Follow renames (Defaults on).
	-S, --rev-file revs-file
			Use revs from revs-file instead of calling git-rev-list
	-h, --help
			This message.
";

	exit(1);
}

our ($help, $longrev, $rename, $rawtime, $starting_rev, $rev_file) = (0, 0, 1);

my $rc = GetOptions(	"long|l" => \$longrev,
			"time|t" => \$rawtime,
			"help|h" => \$help,
			"rename|r" => \$rename,
			"rev-file|S=s" => \$rev_file);
if (!$rc or $help or !@ARGV) {
	usage();
}

my $filename = shift @ARGV;
if (@ARGV) {
	$starting_rev = shift @ARGV;
}

my @stack = (
	{
		'rev' => defined $starting_rev ? $starting_rev : "HEAD",
		'filename' => $filename,
	},
);

our @filelines = ();

if (defined $starting_rev) {
	@filelines = git_cat_file($starting_rev, $filename);
} else {
	open(F,"<",$filename)
		or die "Failed to open filename: $!";

	while(<F>) {
		chomp;
		push @filelines, $_;
	}
	close(F);

}

our %revs;
our @revqueue;
our $head;

my $revsprocessed = 0;
while (my $bound = pop @stack) {
	my @revisions = git_rev_list($bound->{'rev'}, $bound->{'filename'});
	foreach my $revinst (@revisions) {
		my ($rev, @parents) = @$revinst;
		$head ||= $rev;

		if (!defined($rev)) {
			$rev = "";
		}
		$revs{$rev}{'filename'} = $bound->{'filename'};
		if (scalar @parents > 0) {
			$revs{$rev}{'parents'} = \@parents;
			next;
		}

		if (!$rename) {
			next;
		}

		my $newbound = find_parent_renames($rev, $bound->{'filename'});
		if ( exists $newbound->{'filename'} && $newbound->{'filename'} ne $bound->{'filename'}) {
			push @stack, $newbound;
			$revs{$rev}{'parents'} = [$newbound->{'rev'}];
		}
	}
}
push @revqueue, $head;
init_claim( defined $starting_rev ? $head : 'dirty');
unless (defined $starting_rev) {
	my $diff = open_pipe("git","diff","HEAD", "--",$filename)
		or die "Failed to call git diff to check for dirty state: $!";

	_git_diff_parse($diff, [$head], "dirty", (
				'author' => gitvar_name("GIT_AUTHOR_IDENT"),
				'author_date' => sprintf("%s +0000",time()),
				)
			);
	close($diff);
}
handle_rev();


my $i = 0;
foreach my $l (@filelines) {
	my ($output, $rev, $committer, $date);
	if (ref $l eq 'ARRAY') {
		($output, $rev, $committer, $date) = @$l;
		if (!$longrev && length($rev) > 8) {
			$rev = substr($rev,0,8);
		}
	} else {
		$output = $l;
		($rev, $committer, $date) = ('unknown', 'unknown', 'unknown');
	}

	printf("%s\t(%10s\t%10s\t%d)%s\n", $rev, $committer,
		format_date($date), ++$i, $output);
}

sub init_claim {
	my ($rev) = @_;
	for (my $i = 0; $i < @filelines; $i++) {
		$filelines[$i] = [ $filelines[$i], '', '', '', 1];
			# line,
			# rev,
			# author,
			# date,
			# 1 <-- belongs to the original file.
	}
	$revs{$rev}{'lines'} = \@filelines;
}


sub handle_rev {
	my $i = 0;
	my %seen;
	while (my $rev = shift @revqueue) {
		next if $seen{$rev}++;

		my %revinfo = git_commit_info($rev);

		if (exists $revs{$rev}{parents} &&
		    scalar @{$revs{$rev}{parents}} != 0) {

			git_diff_parse($revs{$rev}{'parents'}, $rev, %revinfo);
			push @revqueue, @{$revs{$rev}{'parents'}};

		} else {
			# We must be at the initial rev here, so claim everything that is left.
			for (my $i = 0; $i < @{$revs{$rev}{lines}}; $i++) {
				if (ref ${$revs{$rev}{lines}}[$i] eq '' || ${$revs{$rev}{lines}}[$i][1] eq '') {
					claim_line($i, $rev, $revs{$rev}{lines}, %revinfo);
				}
			}
		}
	}
}


sub git_rev_list {
	my ($rev, $file) = @_;

	my $revlist;
	if ($rev_file) {
		open($revlist, '<' . $rev_file)
		    or die "Failed to open $rev_file : $!";
	} else {
		$revlist = open_pipe("git-rev-list","--parents","--remove-empty",$rev,"--",$file)
			or die "Failed to exec git-rev-list: $!";
	}

	my @revs;
	while(my $line = <$revlist>) {
		chomp $line;
		my ($rev, @parents) = split /\s+/, $line;
		push @revs, [ $rev, @parents ];
	}
	close($revlist);

	printf("0 revs found for rev %s (%s)\n", $rev, $file) if (@revs == 0);
	return @revs;
}

sub find_parent_renames {
	my ($rev, $file) = @_;

	my $patch = open_pipe("git-diff-tree", "-M50", "-r","--name-status", "-z","$rev")
		or die "Failed to exec git-diff: $!";

	local $/ = "\0";
	my %bound;
	my $junk = <$patch>;
	while (my $change = <$patch>) {
		chomp $change;
		my $filename = <$patch>;
		if (!defined $filename) {
			next;
		}
		chomp $filename;

		if ($change =~ m/^[AMD]$/ ) {
			next;
		} elsif ($change =~ m/^R/ ) {
			my $oldfilename = $filename;
			$filename = <$patch>;
			chomp $filename;
			if ( $file eq $filename ) {
				my $parent = git_find_parent($rev, $oldfilename);
				@bound{'rev','filename'} = ($parent, $oldfilename);
				last;
			}
		}
	}
	close($patch);

	return \%bound;
}


sub git_find_parent {
	my ($rev, $filename) = @_;

	my $revparent = open_pipe("git-rev-list","--remove-empty", "--parents","--max-count=1","$rev","--",$filename)
		or die "Failed to open git-rev-list to find a single parent: $!";

	my $parentline = <$revparent>;
	chomp $parentline;
	my ($revfound,$parent) = split m/\s+/, $parentline;

	close($revparent);

	return $parent;
}


# Get a diff between the current revision and a parent.
# Record the commit information that results.
sub git_diff_parse {
	my ($parents, $rev, %revinfo) = @_;

	my @filenames = ( $revs{$rev}{'filename'} );
	foreach my $parent (@$parents) {
		push @filenames, $revs{$parent}{'filename'};
	}

	my $diff = open_pipe("git-diff-tree","-M","-p","-c",$rev,"--",
				@filenames )
		or die "Failed to call git-diff for annotation: $!";

	_git_diff_parse($diff, $parents, $rev, %revinfo);

	close($diff);
}

sub _git_diff_parse {
	my ($diff, $parents, $rev, %revinfo) = @_;

	my $ri = 0;

	my $slines = $revs{$rev}{'lines'};
	my (%plines, %pi);

	my $gotheader = 0;
	my ($remstart);
	my $parent_count = @$parents;

	my $diff_header_regexp = "^@";
	$diff_header_regexp .= "@" x @$parents;
	$diff_header_regexp .= ' -\d+,\d+' x @$parents;
	$diff_header_regexp .= ' \+(\d+),\d+';

	my %claim_regexps;
	my $allparentplus = '^' . '\\+' x @$parents . '(.*)$';

	{
		my $i = 0;
		foreach my $parent (@$parents) {

			$pi{$parent} = 0;
			my $r = '^' . '.' x @$parents . '(.*)$';
			my $p = $r;
			substr($p,$i+1, 1) = '\\+';

			my $m = $r;
			substr($m,$i+1, 1) = '-';

			$claim_regexps{$parent}{plus} = $p;
			$claim_regexps{$parent}{minus} = $m;

			$plines{$parent} = [];

			$i++;
		}
	}

	DIFF:
	while(<$diff>) {
		chomp;
		if (m/$diff_header_regexp/) {
			$remstart = $1 - 1;
			# (0-based arrays)

			$gotheader = 1;

			foreach my $parent (@$parents) {
				for (my $i = $ri; $i < $remstart; $i++) {
					$plines{$parent}[$pi{$parent}++] = $slines->[$i];
				}
			}
			$ri = $remstart;

			next DIFF;

		} elsif (!$gotheader) {
			# Skip over the leadin.
			next DIFF;
		}

		if (m/^\\/) {
			;
			# Skip \No newline at end of file.
			# But this can be internationalized, so only look
			# for an initial \

		} else {
			my %claims = ();
			my $negclaim = 0;
			my $allclaimed = 0;
			my $line;

			if (m/$allparentplus/) {
				claim_line($ri, $rev, $slines, %revinfo);
				$allclaimed = 1;

			}

			PARENT:
			foreach my $parent (keys %claim_regexps) {
				my $m = $claim_regexps{$parent}{minus};
				my $p = $claim_regexps{$parent}{plus};

				if (m/$m/) {
					$line = $1;
					$plines{$parent}[$pi{$parent}++] = [ $line, '', '', '', 0 ];
					$negclaim++;

				} elsif (m/$p/) {
					$line = $1;
					if (get_line($slines, $ri) eq $line) {
						# Found a match, claim
						$claims{$parent}++;

					} else {
						die sprintf("Sync error: %d\n|%s\n|%s\n%s => %s\n",
								$ri, $line,
								get_line($slines, $ri),
								$rev, $parent);
					}
				}
			}

			if (%claims) {
				foreach my $parent (@$parents) {
					next if $claims{$parent} || $allclaimed;
					$plines{$parent}[$pi{$parent}++] = $slines->[$ri];
					    #[ $line, '', '', '', 0 ];
				}
				$ri++;

			} elsif ($negclaim) {
				next DIFF;

			} else {
				if (substr($_,scalar @$parents) ne get_line($slines,$ri) ) {
				        foreach my $parent (@$parents) {
						printf("parent %s is on line %d\n", $parent, $pi{$parent});
					}

					die sprintf("Line %d, does not match:\n|%s|\n|%s|\n%s\n",
						    $ri,
						substr($_,scalar @$parents),
						get_line($slines,$ri), $rev);
				}
				foreach my $parent (@$parents) {
					$plines{$parent}[$pi{$parent}++] = $slines->[$ri];
				}
				$ri++;
			}
		}
	}

	for (my $i = $ri; $i < @{$slines} ; $i++) {
		foreach my $parent (@$parents) {
			push @{$plines{$parent}}, $slines->[$ri];
		}
		$ri++;
	}

	foreach my $parent (@$parents) {
		$revs{$parent}{lines} = $plines{$parent};
	}

	return;
}

sub get_line {
	my ($lines, $index) = @_;

	return ref $lines->[$index] ne '' ? $lines->[$index][0] : $lines->[$index];
}

sub git_cat_file {
	my ($rev, $filename) = @_;
	return () unless defined $rev && defined $filename;

	my $blob = git_ls_tree($rev, $filename);
	die "Failed to find a blob for $filename in rev $rev\n" if !defined $blob;

	my $catfile = open_pipe("git","cat-file", "blob", $blob)
		or die "Failed to git-cat-file blob $blob (rev $rev, file $filename): " . $!;

	my @lines;
	while(<$catfile>) {
		chomp;
		push @lines, $_;
	}
	close($catfile);

	return @lines;
}

sub git_ls_tree {
	my ($rev, $filename) = @_;

	my $lstree = open_pipe("git","ls-tree",$rev,$filename)
		or die "Failed to call git ls-tree: $!";

	my ($mode, $type, $blob, $tfilename);
	while(<$lstree>) {
		chomp;
		($mode, $type, $blob, $tfilename) = split(/\s+/, $_, 4);
		last if ($tfilename eq $filename);
	}
	close($lstree);

	return $blob if ($tfilename eq $filename);
	die "git-ls-tree failed to find blob for $filename";

}



sub claim_line {
	my ($floffset, $rev, $lines, %revinfo) = @_;
	my $oline = get_line($lines, $floffset);
	@{$lines->[$floffset]} = ( $oline, $rev,
		$revinfo{'author'}, $revinfo{'author_date'} );
	#printf("Claiming line %d with rev %s: '%s'\n",
	#		$floffset, $rev, $oline) if 1;
}

sub git_commit_info {
	my ($rev) = @_;
	my $commit = open_pipe("git-cat-file", "commit", $rev)
		or die "Failed to call git-cat-file: $!";

	my %info;
	while(<$commit>) {
		chomp;
		last if (length $_ == 0);

		if (m/^author (.*) <(.*)> (.*)$/) {
			$info{'author'} = $1;
			$info{'author_email'} = $2;
			$info{'author_date'} = $3;
		} elsif (m/^committer (.*) <(.*)> (.*)$/) {
			$info{'committer'} = $1;
			$info{'committer_email'} = $2;
			$info{'committer_date'} = $3;
		}
	}
	close($commit);

	return %info;
}

sub format_date {
	if ($rawtime) {
		return $_[0];
	}
	my ($timestamp, $timezone) = split(' ', $_[0]);
	my $minutes = abs($timezone);
	$minutes = int($minutes / 100) * 60 + ($minutes % 100);
	if ($timezone < 0) {
	    $minutes = -$minutes;
	}
	my $t = $timestamp + $minutes * 60;
	return strftime("%Y-%m-%d %H:%M:%S " . $timezone, gmtime($t));
}

# Copied from git-send-email.perl - We need a Git.pm module..
sub gitvar {
    my ($var) = @_;
    my $fh;
    my $pid = open($fh, '-|');
    die "$!" unless defined $pid;
    if (!$pid) {
	exec('git-var', $var) or die "$!";
    }
    my ($val) = <$fh>;
    close $fh or die "$!";
    chomp($val);
    return $val;
}

sub gitvar_name {
    my ($name) = @_;
    my $val = gitvar($name);
    my @field = split(/\s+/, $val);
    return join(' ', @field[0...(@field-4)]);
}

sub open_pipe {
	if ($^O eq '##INSERT_ACTIVESTATE_STRING_HERE##') {
		return open_pipe_activestate(@_);
	} else {
		return open_pipe_normal(@_);
	}
}

sub open_pipe_activestate {
	tie *fh, "Git::ActiveStatePipe", @_;
	return *fh;
}

sub open_pipe_normal {
	my (@execlist) = @_;

	my $pid = open my $kid, "-|";
	defined $pid or die "Cannot fork: $!";

	unless ($pid) {
		exec @execlist;
		die "Cannot exec @execlist: $!";
	}

	return $kid;
}

package Git::ActiveStatePipe;
use strict;

sub TIEHANDLE {
	my ($class, @params) = @_;
	my $cmdline = join " ", @params;
	my  @data = qx{$cmdline};
	bless { i => 0, data => \@data }, $class;
}

sub READLINE {
	my $self = shift;
	if ($self->{i} >= scalar @{$self->{data}}) {
		return undef;
	}
	return $self->{'data'}->[ $self->{i}++ ];
}

sub CLOSE {
	my $self = shift;
	delete $self->{data};
	delete $self->{i};
}

sub EOF {
	my $self = shift;
	return ($self->{i} >= scalar @{$self->{data}});
}
