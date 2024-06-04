#!/usr/bin/env python3

import cgi
import cgitb
#import pyperclip

#line = line.replace(os.linesep, "\n")

# エラーメッセージをブラウザに表示するためにCGIデバッグを有効にします
cgitb.enable()

# HTTPヘッダーを出力します
print("Content-Type: text/html\r")
print("\r")

# フォームデータを取得します
form = cgi.FieldStorage()

# フォームデータからパラメータ 'a' の値を取得します
a_value = form.getvalue('a', 'デフォルト値')  # 第二引数はパラメータが存在しない場合のデフォルト値

# HTMLレスポンスを出力します
print("<html>")
print("<head><title>CGI Form Handling</title></head>")
print("<body>")
print(f"<h1>Received data</h1>")
print(f"<p>a: {a_value}</p>")
print("</body>")
print("</html>")

