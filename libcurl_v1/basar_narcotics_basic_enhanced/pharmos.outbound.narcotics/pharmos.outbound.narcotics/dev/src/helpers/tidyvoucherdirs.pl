#!/usr/bin/perl
use strict;

use warnings;

use Archive::Zip qw( :ERROR_CODES :CONSTANTS ); #install from cpan

use File::Copy; #built in


#function: 	given to directories ([source] & [dest]) this scrip traverses [source] and all it's subdirctories and does two things
#			if a filename containing '[name].pdf[suffix]' is found, it is renamed to '[name.pdf]' and put in a subfolder of [dest], 
#			if a filename matching '[number](_[digit(s)]).zip[suffix]'
#			is found it is unpacked to a subfolder of [dest] named '[number]'

#params:	[source] [dest]

#externals:	need Archive::Zip from CPAN


if ($#ARGV + 1 < 2 || $#ARGV + 1 > 2)
{
	printUsage();
	exit;
}

my $srcDir = $ARGV[0];

my $destDir = $ARGV[1];

#some sanitychecks
checkSrcDirExists($srcDir);

#check if exists $destDir and create if not
checkAndCreateDestDir($destDir);

print "all set up\n";


traverseDirectories($destDir, $srcDir);


sub traverseDirectories
{
	my ($destDir,@dirs) = @_;
	
	my %seen;
	
	
	while(my $pwd = shift(@dirs))
	{
		
		print "checking $pwd\n";
		
		opendir(DIR, $pwd) or die "can't open dir $pwd: $!\n";
		my @files = readdir(DIR);
		close DIR;
		foreach my $file (@files)
		{
			print "checking $pwd/$file\n";
			
			#check is directory, move in @dirs		
			if (-d "$pwd/$file" and ($file !~ /^\.\.?$/) and !$seen{"$pwd/$file"} )
			{
				$seen{"$pwd/$file"} = 1;
				
				print "adding $pwd/$file to list of directories\n";
				
				push @dirs, "$pwd/$file";
			}
			#next if ($file !~ /\.pdf/i or $file !~ /\.zip/i);
			
			if ($file =~ /\.pdf/i)
			{
				print "found pdf: $file\n";
				
				my $fileTidied = tidyFileName($file);
				
				my $destSubDir = checkAndCreateDestSubDir($pwd, $destDir);
				
				copy ("$pwd/$file", "$destDir/$destSubDir/$fileTidied" ) or die "can't copy file $pwd/$file to $destDir/$destSubDir/$fileTidied: $!\n";
			}
			
			if ($file =~ /.zip/i)
			{
				print "found zip: $file\n";
				extractTo("$pwd/$file", $destDir);
			}
		}
	}
}

sub checkAndCreateDestSubDir
{
	my ($dir, $destdir) = @_;
	
	$dir =~ /(\d{10})$/;
	
	$dir = $1;
	
	if (!-d "$destDir/$dir" )
	{
		mkdir "$destDir/$dir" or die "cound not create dir: $destDir/$dir: $!\n";
	}
	
	return $dir;	
}

sub extractTo
{
	my ($zipFileName,$destDir) = @_;
	
	my $zip = Archive::Zip->new();
	
   unless ( $zip->read( $zipFileName ) == AZ_OK )
   {
       die "error reading zipfile: $zipFileName: $!\n";
   }
   
   $zip->extractTree('', "$destDir/");	
	 
}

sub tidyFileName
{
	my $fileName = shift(@_);
	
	if ($fileName !~ /\.pdf$/)
	{
		$fileName =~ /(\d{12}\.pdf)*/;
		
		print "file $fileName tidied to $1\n";
		
		$fileName = $1;
	}
	
	return $fileName;
}

sub printUsage
{
	print "usage: $0 [source] [dest] with [source]: folder containing data to tidy up and [dest]: name of folder of tidied data\nNote: [dest] is created if not present";
}


sub checkSrcDirExists
{
	my $srcDir = shift(@_);
	
	if(! -d $srcDir)
	{
		die "[source] is not a directory\n";
	}
	
}

sub checkAndCreateDestDir
{
	my $destDir = shift(@_);
	
	if (! -d $destDir)
	{
		mkdir($destDir) or die "Could not create $destDir: $!\n";
	}	

}