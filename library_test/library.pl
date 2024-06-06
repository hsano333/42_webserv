#!/usr/bin/perl

use CGI;
use utf8;
use Encode;
my $q = new CGI;
#my @param = $q->param();
my $page = int($q->param( 'page' ));
#my $name = $q->param("a");

#open(DATAFILE, "< books.txt")) or die("error :$!");
my $remote_addre = $ENV{PATH_INFO};
my $query = $ENV{QUERY_STRING};
my $user_name = $ENV{REMOTE_USER};
#print $remote_addre;
my $filename = $remote_addre.'/books.txt';
open my $FH, '<', $filename;

for my $line (<$FH>) {
    #print $line;
    chomp $line;
    my @field = split /,/, $line;    # カンマ区切りで配列に格納

    my $record = \@field;            # 配列リファレンスにする
    push @records, $record;   # 配列リファレンスを格納用の配列に入れる
}
close $FH;





#print $cgi->header(
#-type    => 'text/html',
#-charset => 'UTF-8'
#);

print $q->header("text/html; charset=UTF-8");
#print "charset: text/html\r\n\r\n";
#print "Content-type: text/html; charset=UTF-8\r\n\r\n";
#print "\n";
#print "text:html\n\n";
#
#
my $str = 'こんにちは';      # use utf8 がないので、この文字列は外部文字列と同じ
utf8::decode($str);     # 外部文字列を内部文字列にデコード (use utf8 がなくても利用できる)
#$str =~ tr/あ-ん/ア-ン/;    # ひらがなをカタカナに変換
#utf8::encode($str);     # 内部文字列を外部文字列にエンコード (use utf8 がなくても利用できる)
#print $str, "\n";       # カタカナに変換されずに「こんにちは」と出力される

my $html = << "EOF_HTML";
<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="/library.css">
</head>
<body>
    <div class="header">
        <a href="/mypage.html" class="mypage-link">マイページ</a>
    </div>

    <div class="pagination">
        <a href="/cgi/library.pl/db?page=1">1</a>
        <a href="/cgi/library.pl/db?page=2">2</a>
        <a href="/cgi/library.pl/db?page=3">3</a>
        <a href="/cgi/library.pl/db?page=4">4</a>
        <a href="/cgi/library.pl/db?page=5">5</a>
    </div>
EOF_HTML
print $html;
print $user_name;

$page = $page -1;
if($page <= 0){
    $page = 0;
}
#for my $record (@records) {
#for my $record (@records) {
for ($count = $page*30; $count < ($page+1)*30; $count++ ){
    my @record = @{$records[$count]};

    print "<figure>";
    print "    <img src=\"/img/@record[2].jpg\" alt=\"Image @record[0]\" width=\"200\">";
    print "    <figcaption>@record[3]</figcaption>";
    if(int(@record[1]) >= 1){
        print "    <button class=\"check-out\" id=\"@record[0]\"></button>";
    }else{
        print "    <div class=\"checked-out\"></button>";
    }
    print "</figure>";
}

#print $q->param( 'page' );
#print $page;

my $html2 = << "EOF_HTML";
</body>
</html>
EOF_HTML
print $html2;
#print $page;




