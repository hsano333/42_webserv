#!/usr/bin/perl

use CGI;
use utf8;
use Encode;
my $q = new CGI;

my $remote_addre = $ENV{PATH_INFO};
my $user_name = $ENV{REMOTE_USER};
my $cookie_books = $q->cookie("books");
my @cookie_array = split /-/, $cookie_books;

my $filename = $remote_addre.'/books.txt';
open my $FH, '<', $filename or die("Error:can't open file");

for my $line (<$FH>) {
    chomp $line;
    my @field = split /,/, $line;
    my $record = \@field;
    push @records, $record;
}
close $FH;

my $userfile = $remote_addre.'/'.$user.".txt";

if(-e $userfile){
    open my $FHW, '>', $userfile or die("Error:can't open file");
        print $FHW $rent;
    close $FHW;
}



my $file_line = "";
my $myfile = $remote_addre.'/'.$user_name.".txt";
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

print $q->header("text/html; charset=UTF-8");
my $html = << "EOF_HTML";
<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>図書館のマイページ</title>
    <link rel="stylesheet" href="styles.css">
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

        function clearCookie() {
        
            var old = new Date('1999-12-31T23:59:59Z');
            let path = "path=/"
            document.cookie = "books=; expires=" + old.toUTCString() + "; "+ path;
            window.location.reload();
        }



        function rentBooks(name){

            let old_value = getCookie("books");
            console.log("name:" + name);

            clearCookie();
        }

    </script>



</head>
<body>
    <div class="header">
        <h1>図書館のマイページ</h1>
        <a href="/cgi/library.pl/db" class="mypage-link">戻る</a>
    </div>
    <div class="container">
        <h2>ユーザー名: $user_name</h2>
        
        <h3>借りている本の一覧</h3>
        <form action="/cgi/return.pl/db" method="post">
            <input type="hidden" NAME="user" value="$user_name"  required>
            <input type="submit" NAME="submit" value="返却する"  onclick=\"clearCookie()\">
        </form>


        <ul>
EOF_HTML
print $html;

foreach my $i(0 .. $#rent_array){

    if($rent_array[$i] ne ""){
        my $index = int($rent_array[$i])-1;
        my @record = @{$records[$index]};
        #print "<li>$index</li>";
        print "<li>@record[3]</li>";
    }
}

#<li>本1: 例のタイトル1</li>
#<li>本2: 例のタイトル2</li>
#<li>本3: 例のタイトル3</li>
my $html2 = << "EOF_HTML";
        </ul>

        <h3>カートに入っている本の一覧</h3>
        <ul>
EOF_HTML
print $html2;

foreach my $i(0 .. $#cookie_array){
    if($cookie_array[$i] ne ""){
        my $index = int($cookie_array[$i])-1;
        my @record = @{$records[$index]};
        print "<li>@record[3]</li>";
    }
}
$file_line = $file_line.$cookie_books;
my $html3 = << "EOF_HTML";
        </ul>
        <div>
        <button onclick=\"clearCookie()\">カートを空にする</button>
        <form action="/cgi/rent.pl/db" method="post">
            <input type="hidden" NAME="user" value="$user_name"  required>
            <input type="hidden" NAME="rent" value="$file_line"  required>
            <input type="submit" NAME="submit" value="借りる"  onclick=\"clearCookie()\">
        </form>
        </div>
    </div>
</body>
</html>
EOF_HTML
print $html3;

