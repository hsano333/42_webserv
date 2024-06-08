#!/usr/bin/python3

import os
import sys
import cgi
print("Content-Type: text/html\r")
print("Status:200 OK\r")
print("\r")

path_info = os.getenv("PATH_INFO")
print(f'{path_info=}')
#os.chdir('..')
upload_dir = path_info
files = os.listdir(upload_dir)

def delete_file(item):
	filename = os.path.join(upload_dir, item.value)
	os.remove(filename)
	if os.path.exists(filename):
		print(f'failed to delete [{item.value}]')
	else:
		print(f'delete [{item.value}]')
	print(f'<BR><A HREF=/>return</A>')

form = cgi.FieldStorage()
if "filename" in form:
	filename = form["filename"]
	if isinstance(filename, list):
		for file in filename:
			delete_file(file)
	else:
		delete_file(filename)
