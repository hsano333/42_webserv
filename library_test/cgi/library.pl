#!/usr/bin/perl

use CGI;
use utf8;
use Encode;
use CGI::Cookie;

my $q = new CGI;
#my @param = $q->param();
my $page = int($q->param( 'page' ));
my $cookie_books = $q->cookie("books");


my @cookie_array = split /-/, $cookie_books;

my $books = "NG";


#&getCookie();
#$books = $COOKIE{'books'};
#if ($books eq "") {
#$books = 1;
#}

#my %cookies = fetch CGI::Cookie;
#my %cookies = CGI::Cookie->fetch;
#$old_books = $cookies{'books'}->value;
my $domain = $ENV{SERVER_NAME};
my $port = $ENV{SERVER_PORT};
#

#my $old_books = "1234";

#$cookie = new CGI::Cookie(-name=>'books',
#    -value=>$old_books,
#    -expires =>  '+1D',
#    -domain  =>  $domain,
#);


#my $name = $q->param("a");

#open(DATAFILE, "< books.txt")) or die("error :$!");
my $remote_addre = $ENV{PATH_INFO};
my $path = $ENV{PATH_INFO}."../cgi/";
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

my $cookie = $q->cookie(
    -name  => 'books',
    #-value => 'ABC',
    -value => $cookie_books,
    -expires =>  '+1d',
    -domain  =>  "",
    #-path => "/"
);


#print $cookie;
print $q->header(
    -type => "text/html; charset=UTF-8",
    -cookie => $cookie
    #-cookie => $cookie,
);
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
    <script>

        function getCookie(name) {
            let cookieName = name + "=";
            let decodedCookie = decodeURIComponent(document.cookie);
            let cookies = decodedCookie.split(';');

            for (let i = 0; i < cookies.length; i++) {
                let cookie = cookies[i].trim();
                if (cookie.indexOf(cookieName) === 0) {
                    return cookie.substring(cookieName.length, cookie.length);
                }
              }
              return "";
        }


        function saveCookie(id) {


            var old = new Date('1999-12-31T23:59:59Z'); 
            let path = "path=/"
            let old_value = getCookie("books");
            document.cookie = "books=; expires=" + old.toUTCString() + "; "+ path;

            console.log("old:" + old_value);


            let one = 1; 
            let nowdate = new Date();
            nowdate.setTime(nowdate.getTime() + 24*60*60*1000);
            let expire = nowdate.toGMTString() + "; "; //GMT形式に変換して変数kigendateに格納
            let value = "books=" + old_value + "-" + id + "; ";


            console.log(id);
            document.cookie = value + expire + path;
            console.log(value + expire + path);
            console.log(document.cookie);
            window.location.reload();
        }
    </script>
</head>
<body>
    <div class="header">
        <a href="/cgi/mypage.pl/db" class="mypage-link">マイページ</a>
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

$page = $page -1;
if($page <= 0){
    $page = 0;
}
#for my $record (@records) {
#for my $record (@records) {
for ($count = $page*30; $count < ($page+1)*30; $count++ ){
    my @record = @{$records[$count]};

    print "<figure>";
    print "    <img src=\"/img/@record[2].jpg\" alt=\"Image @record[0]\" width=\"300\">";
    print "    <figcaption>@record[3]</figcaption>";

    #my $tmp = "OK" if grep { $_ eq @record[0] } @cookie_array;
    #if(@record[0] == 5){
    if (grep { int($_) == int(@record[0]) } @cookie_array){
    #if ($tmp eq "OK"){
        print "    <div class=\"cart\" id=\"@record[0]\"></button>";
    }elsif(int(@record[1]) >= 1){
        print "    <button class=\"check-out\" id=\"@record[0]\" onclick=\"saveCookie(\'@record[0]\')\"   ></button>";
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
print $books;
#print $page;


print $path;
print "   \r\n domaien:";
print $domain;
print "\nbooks:";
print $cookie_books;
print "\ntest cookie No.1";
print @cookie_array[1];
print @cookie_array[2];
print @cookie_array[3];
#%cookies = fetch CGI::Cookie;
#%cookies = CGI::Cookie->fetch;
#$id = $cookies{'books'}->value;
#my $test = $q->cookie('books');


#print $%cookies{'books'}->value;
foreach (keys %cookies) {
    print "test cookie";
    if($cookies{$_} eq "books"){
        $books = "OK";
    }
   do_something($cookies{$_});
   print $cookies{$_};
   print $_;
}


