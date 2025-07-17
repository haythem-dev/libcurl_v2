#!/usr/bin/perl


# function: this script checks all *.pdf files (contribution vouchers) in a given directory if their Abgabedatum (formated as DDMMYY)
# is before a given date (formated as DDMMYY)
# if a matching file is found, it's moved to another directory
# also, because there's no quick way to connect to cpan using solaris, this script generates another script to move found files out
# of their directory

use strict;

use warnings;

use CAM::PDF;
use CAM::PDF::PageText;
use File::Copy;

#params: foldername startdate (format TTMMJJ)


my @filesToMove;

my $dir;
my $startdate;

if ($#ARGV == 1)
{
	($dir, $startdate) = @ARGV;
}
else
{
	printUsage();
	exit 0;
#	$dir = '3003080035';

#	$startdate = 310712;
	#todo: usage
}

my $logFileName = "log_$dir.log";

my $backupDirectory = "save_$dir";

open my $log_fh, '>', $logFileName or die ("could not create logfile: $!"); 

print $log_fh "Starting $dir\n"; 

opendir( DIR, $dir ) or die $!;

while ( my $file = readdir(DIR) ) {

	# We only want files
	next unless ( -f "$dir/$file" );

	# Use a regular expression to find files ending in .txt
	next unless ( $file =~ m/\.pdf$/ );

	if (checkFileToRename($startdate, "$dir/$file", $log_fh))
	{
		push(@filesToMove, $file);
	}
}

closedir(DIR);

#create $backupDirectory

unless(-d $backupDirectory)
{
	mkdir($backupDirectory, 0777);
}

#create move script

open my $move_fh, '>', "move_$dir.sh";

print $move_fh "#!/bin/sh\n";

print $move_fh "mkdir ../save/save_$dir\n";

foreach(@filesToMove)
{
	move("$dir/$_", "$backupDirectory/$_") or die ("cound not move file $dir/$_");
	print $log_fh "moved file $dir/$_ to $backupDirectory/$_\n";
	
	print $move_fh "mv $dir/$_ ../save/save_$dir/\n";
	
}

print $log_fh "finished $dir";
close $move_fh;
close $log_fh;

exit 0;

#for each filename, do:
sub checkFileToRename
{
	
	my ($startdate, $filename, $log_fh) = @_;
	
	my $startDay = substr($startdate, 0, 2);
	my $startMonth = substr($startdate, 2, 2);
	my $startYear = substr($startdate, 4, 2);
	
	my $pdf          = CAM::PDF->new($filename);
	my $pageone_tree = $pdf->getPageContentTree(1);
	my $pdf_content  = CAM::PDF::PageText->render($pageone_tree);
	
	if ( $pdf_content =~ /Abgabedatum \(TTMMJJ\)\n(\d{2})(\d{2})(\d{2})/ ) {
		# $1 days, $2 months, $3 year
		# startYear is at least 12 for 2012
		if ( $3 < $startYear )
		{	
			print $log_fh "$filename has date $1$2$3, startdate is $startdate - moving required\n";
			return 1;
		}
		elsif ( $2 < $startMonth  ) #here $3 is at least even or greater than $startYear
		{	
			print $log_fh "$filename has date $1$2$3, startdate is $startdate - moving required\n";
			return 1;
		}
		elsif ($1 < $startDay && !($2 > $startMonth))
		{
			print $log_fh "$filename has date $1$2$3, startdate is $startdate - moving required\n";
			return 1;
		}
		else
		{
			#log: $filename has date $1$2$3, startdate is $startdate - no action required
			print $log_fh "$filename has date $1$2$3, startdate is $startdate - no action required\n";
		}
	}
	else {
	
		print $log_fh "WARN: $filename could not be matched, manual action is required\n";
		print STDERR "WARN: $filename could not be matched, manual action is required\n";
	}
	
	return 0;
	
}

sub printUsage
{
	print "usage: $0 <foldername containg contribution vouchers> <startdate (format DDMMYY)>";
}


