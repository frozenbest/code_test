#!/usr/bin/env python
#coding=utf-8

import sys
reload(sys)
sys.setdefaultencoding('utf8')

import socket
#建立一个客户端
client = socket.socket(socket.AF_INET,socket.SOCK_STREAM) # 声明socket类型，同时生成链接对象
client.connect(('10.101.70.10',9090)) # 建立一个链接  链接到服务端

while True:
    sock, addr = client.accept()
    print("连接地址：",addr)
    msg = 'welcome!'
    client.send( msg.encode('utf-8') )
    data = client.recv(1024)
    print('recv:', data.decode())
client.close() # 关闭链接


