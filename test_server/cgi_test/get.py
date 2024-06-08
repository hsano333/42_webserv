#!/usr/bin/python3
import os

#a = input()
#b = int(input())
#print(f'{splited_values=}')

import cgi
#a = 'abcdefghijklmnopqrstuvwxyz'
#query = (os.environ['QUERY_STRING'])
#splited_values = query.split('&')
#form = cgi.FieldStorage()
#a = form.getfirst('a')
#b = int(form.getfirst('b'))
#a= (os.environ['HOME'])
# size 100
a='abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX'
b = 30
cur_path = os.getcwd()
print("Content-Type:text/plain")
#print(f"Content-length:{len(a) * b}")
print("Status:200 OK\r")
print("\r")
print(a*b)
print(f'{cur_path=}')

