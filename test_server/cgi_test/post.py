#!/usr/bin/python3

import os
import cgi
import cgitb
#cgitb.enable()

import time
form = cgi.FieldStorage()

#form = cgi.FieldStorage()

#a=form.getfirst('a')
#a = form.getvalue('a', 'NG')
    #print('<p>%s: %s</p>' % (key, value))

#a = input()
a = form.getvalue('a', 'test')
b = int(form.getvalue('b', '1'))
#a="abcdefg"

if len(a)*b > 100000000:
    print("Content-Type:text/plain")
    #print(f"Content-length:{len(a) * b}")
    print("Status:200 OK\r")
    print("\r")
    print("error")

else:
    print("Content-Type:text/plain")
    #print(f"Content-length:{len(a) * b}")
    print("Status:200 OK\r")
    print("\r")
    print(a*b)
#a='abc'
#b = 2
#a = 'abcdefghijklmnopqrstuvwxyz'
#b = 51
