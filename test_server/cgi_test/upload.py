#!/usr/bin/python2

import os
import sys
import cgi

print("Content-Type: text/html\r")
print("Status:200 OK\r")
print("\r")

#exit(1);
#os.chdir('..')
path_info = os.getenv("PATH_INFO")
path_info = str(path_info)
#print(f"path_info1={path_info}")
upload_dir = path_info
#os.mkdir(path_info)
def save_file(item):
    #print(f'upload file:{item}<BR>')
    #print(f'upload file({item[0]})<BR>')
    #print(f'upload file:{item[1]}<BR>')
    #tmpfilename = item.filename
    tmpfilename = item['filename']
    #tmpfilename = item['filename']
    #print(f'({tmpfilename=})<BR>')
    filename = os.path.join(upload_dir, item.filename)
    #print(f'upload file({filename=})<BR>')
    i = 0
    while(os.path.exists(filename)):
        i = i + 1
        tmp = tmpfilename.replace(".", "_" + str(i) + ".")
        filename = os.path.join(upload_dir, tmp)
    try:
        fout = open(filename, mode = "wb")
        chunk = item.file.read(1024)
        while (chunk):
            print("write")
            fout.write(chunk)
            chunk = item.file.read(1024)
        fout.close()
        #print(f'write end({filename=})<BR>')
        os.chmod(filename, 0o666)
    except Exception as ee:
        #pass
        print(str(ee))



print('form1=')
form = cgi.FieldStorage()
print('No.21 form')
ret = form.getvalue('text_name')
print('No.3 form=')


#print(f'{ret=}')
if "filename" in form:
    files= form["filename"]
    print('test No.1')
    if isinstance(files, list):
        print('test No.2')
        for file in files:
            print('test No.3')
            #print(f'{file}')
            save_file(file)
    else:
        print('test No.4')
        #print(f'{files=}')
        save_file(files)



print("<html>")
print("<head>")
print("<title>DeleteTEST</title>")
print("<meta charset=\"UTF-8\">")
print("</head>")
print("<body>")
print("<h1>Upload Test</h1>")
#print(f"path_info={path_info}")

print('    <BR><A HREF=/>return</A>')
print("</body>")
print("</html>")

