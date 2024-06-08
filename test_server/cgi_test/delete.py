#!/usr/bin/python3

import os
import sys
import cgi
print("Content-Type: text/html\r")
print("Status:200 OK\r")
print("\r")


def make_file(file_path, mode):
    try:
        fout = open(file_path, mode = "w")
        fout.write("test")
        fout.close()
        os.chmod(file_path, mode)
    except Exception as ee:
        pass
        #print(str(ee))

#os.chdir('..')
path_info = os.getenv("PATH_INFO")
path_info = str(path_info)
mode = 0o666
delte_file1 = os.path.join(path_info ,"a")
delte_file2 = os.path.join(path_info ,"b")
delte_file3 = os.path.join(path_info ,"c")
delte_file4 = os.path.join(path_info ,"d")
delte_file5 = os.path.join(path_info ,"e")
delte_file6 = os.path.join(path_info ,"ff")
make_file(delte_file1, mode)
make_file(delte_file2, mode)
make_file(delte_file3, mode)
make_file(delte_file4, mode)
make_file(delte_file5, mode)
make_file(delte_file6, 0o000)

upload_dir = path_info
files = os.listdir(upload_dir)

print(f'{path_info=}')
print("<html>")
print("<head>")
print("<title>DeleteTEST</title>")
print("<meta charset=\"UTF-8\">")
print("<script>")
print("function ClickDelete(){")
#print("return 0;")
#print("preventDefault();")
print('let select = document.querySelector(\'[name="filename"]\').value;')
#print('test = document.getElementById("delete_form").action;')
#print('test2 = document.getElementById("select_file").value;')
#print('document.getElementById("delete_form").action = test + "/" + select;')
print('url = "/delete_files/" + select;');
#print('console.log(url);');

print('const xhr = new XMLHttpRequest();')
print('xhr.open("DELETE", url);')
print('xhr.send();')
print('xhr.onload = () => {')

print('if(xhr.status == 200){ alert(select + "を削除しました");}')
print('else{ alert(select + "の削除に失敗しました");}')
print('};')


#print('alert(select + "を削除しました");')
      
print("}")
print("</script>")
print("</head>")
print("<body>")
print("<h1>Delete Method Test</h1>")
print(f"<div>対象ディレクトリ:{upload_dir}</div>")

print('<select  name="filename"  style="min-width:200px">')
for file in files:
    print(f'<option value="{file}" >{file}</option>')
print('</select>')

print(f'<button id="delete_form"  onclick=ClickDelete() ">')
print("削除</button>")
#print('<INPUT type="submit" value="remove">')
#print('<input type="hidden" name="_method" value="DELETE">')
#print('</button>')
print('	<BR><A HREF=/>return</A>')
print("</body>")
print("</html>")
