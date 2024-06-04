#!/usr/bin/perl

use CGI;
use utf8;
use Encode;
my $q = new CGI;

my $remote_addre = $ENV{PATH_INFO};
my $user_name = $ENV{REMOTE_USER};
my $remote_addre = $ENV{PATH_INFO};
my $filename = $remote_addre.'/auth_basic';
#read( STDIN, my $data, $ENV{CONTENT_LENGTH} );
$user = $q->param("user");
$password= $q->param("password");

$user_len = length($user);
$pass_len = length($password);

if($user_len <= 1 || $user_len > 10){
    print "status: 400\r\n";
    print $q->header("text/html; charset=UTF-8");
    print "Error:[$user] is too large or small<br>";
}

if($pass_len <= 3 || $pass_len > 16){
    print "status: 400\r\n";
    print $q->header("text/html; charset=UTF-8");
    print "Error:[$password] is too large or small<br>";
}

open my $FH, '<', $filename or  die("Error:can't open file");
for my $line (<$FH>) {
    chomp $line;
    my @field = split /:/, $line;    # カンマ区切りで配列に格納
    my $record = \@field;            # 配列リファレンスにする
    push @records, $record;   # 配列リファレンスを格納用の配列に入れる
    if($field[0] eq $user){
        print "status: 400\r\n";
        print $q->header("text/html; charset=UTF-8");
        print "Error:[$user] is used<br>";
        exit;

    }
}
close $FH;

open my $FHW, '>>', $filename or  die("Error:can't open file");
print $FHW "\n".$user.":".$password;

close $FHW;
print $q->header("text/html; charset=UTF-8");

print "<br>$user is registered<br>";
print "<a href=\"/cgi/library.pl/db\">図書館へ</a>";
#print "\n";
#print $password;
#print "\nfilename:";
#print $filename;
#print "\nfile:";

