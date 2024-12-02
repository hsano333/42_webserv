# 42_webserv
42の課題であるwebservのリポジトリです。


# Installation

C++98でコンパイルが必要になります。
また、Pythonとperlがない環境なら事前にインストールしてください
また、perlのCGIライブラリのために、libcgi-session-perlもインストールの必要があります(Ubuntuなら以下でインストールできます)
```bash
apt install libcgi-session-perl
```


# Usage
ルートディレクトリでmakeを実行すればwebservという名前の実行ファイルが作成されます。
webservを実行すればサーバーが立ち上がります。引数でconfigファイルを指定することもできますが、指定がない場合は、[webserv.conf]がデフォルトで指定されます。
```
make
./webserv webserv.conf
```

webserv.conf内でpythonでperlの実行ファイルを指定する必要があります。
どちらも/usr/bin/ディレクトリ内にあることが前提ですが、異なる場合はwebserv.confファイルを変更してください。

デフォルトのwebserv.confをそのまま利用するなら、サーバーが立ち上がり次第、http://localhost:11111 にブラウザでアクセスしてください。テスト用のページが表示されます。


