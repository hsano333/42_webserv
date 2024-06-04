#!/usr/bin/perl

open(DATAFILE, "< books.txt") or die("error :$!");


my @books;

while (my $line = <DATAFILE>){
  chomp($line);
  #print "$line\n";
  my @strlist = split(/,/, $line);

  my $cnt = @strlist;
  print "$cnt\n";
  push @books, @strlist;
}


foreach my $parts(@books){
	#print "$parts\n";
}

my $cnt = @books;
print "$cnt\n";
print "end\n";
