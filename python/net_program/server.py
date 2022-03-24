#!/usr/bin/env python
#coding=utf-8

import sys
reload(sys)
sys.setdefaultencoding('utf8')

import socket


# simple tcp conn manage
class SimpleTcpServer:
    """docstring for ConnManager"""
    def __init__(self, host, port, conn_num):
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)#建立一个服务端
        self.server.bind(host, port) # 监听端口
        self.server.listen(conn_num) # 开始监听 标识可以使用n个链接
        self.conn_list = []

    def accept(conn)
        


while True:
    conn,addr = server.accept() #等待链接
    print(conn, addr)
    while True:
        data = conn.recv(1024)#接收数据
        print('receive:',data.decode())#打印接收到的数据
        conn.send(data.upper()) # 然后再发送数据
    conn.close()

