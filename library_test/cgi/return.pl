#!/usr/bin/perl

use CGI;
use utf8;
use Encode;
my $q = new CGI;

my $remote_addre = $ENV{PATH_INFO};
my $user_name = $ENV{REMOTE_USER};
my $remote_addre = $ENV{PATH_INFO};
#read( STDIN, my $data, $ENV{CONTENT_LENGTH} );
$user = $q->param("user");

$user_len = length($user);

if($user_len <= 1 || $user_len > 10){
    print "status: 400\r\n";
    print $q->header("text/html; charset=UTF-8");
    print "Error:[$user] is too large or small<br>";
}

my $file_line = "";
my $myfile = $remote_addre.'/'.$user.".txt";
if(-e $myfile){
    open my $FHU, '<', $myfile;

    for my $line (<$FHU>) {
        chomp $line;
        $file_line = $file_line.$line;
        }
    close $FHU;
}
my @rent_array = split /-/, $file_line;
@rent_array = grep $_ ne " ", @rent_array;

my $bookfile= $remote_addre.'/books.txt';
open my $FHB, '<', $bookfile;

for my $line (<$FHB>) {
    #print $line;
    chomp $line;
    my @field = split /,/, $line;    # カンマ区切りで配列に格納
    my $record = \@field;            # 配列リファレンスにする
    push @records, $record;   # 配列リファレンスを格納用の配列に入れる
}
close $FHB;

for ($count = 0; $count < 150; $count++ ){
    my @record = @{$records[$count]};

    if (grep { int($_) == int(@record[0]) } @rent_array){
        $records[$count][1] = 1;
    }
}


print $q->header("text/html; charset=UTF-8");
my $html = << "EOF_HTML";
<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
    <div>返却しました:@rent
    <div>返却しました:@rent_array
    </div>
    <div class="header">
        <a href="/cgi/mypage.pl/db" class="mypage-link">マイページ</a>に戻る
    </div>
</body>
</html>
EOF_HTML
print $html;

open my $FHB, '>', $bookfile;
for ($count = 0; $count < 150; $count++ ){
    my @record = @{$records[$count]};

    print $FHB @record[0].",".@record[1].",".@record[2].",".@record[3].",".@record[4].","."\n";
}
close $FHB;

my $filename = $remote_addre.'/'.$user.".txt";
open my $FHW, '>', $filename or die("Error:can't open file");
    print $FHW "";
close $FHW;

