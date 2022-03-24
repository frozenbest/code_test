#!/usr/bin/env python
#coding=utf-8

import os
import sys
import re

# os.popen() 返回的是file read的对象 可以读出执行的内容
# os.system() 命令执行结果的返回值


def writefile():
    fo = open("test_file.txt","w")
    print "文件名: ", fo.name
    fo.write("test write file success!")

def filter_file():
    s = []
    cmd = "ls -ltr|grep 'txt'"
    file_set = os.popen(cmd).readlines() # 所有匹配的文件名
    for item in file_set:
        item = item.replace("\n", "")
        arr = item.split(' ')
        file_name = arr[len(arr) - 1]
        #print ("filter_file_name: %s"%file_name)
        if re.search('test',file_name): # 过滤掉不需要的
            #print file_name
            s.append(file_name)
    return s

# 修改读出来的字符串
def replace_world():
    arr = filter_file()
    for file in arr:
        fr = open(file)
        content = fr.read()
        print content

        print "---turn---"
        content, n = re.subn("test", "exec", content)
        print content
        print n

#修改文件里 匹配到的字符串
def replace_file_world():
    arr = filter_file()
    for file_name in arr:
        fr = open(file_name)
        content = fr.read()
        print content

        print "---turn---"
        content, n = re.subn("test", "exec", content)
        fo = file(file_name,"w+")
        fo.write(content)
        fo.close()



def main():
    #writefile()
    #filter_file()
    #replace_world()
    replace_file_world()
if __name__ == '__main__':
    main()








#示例test文件

#cat test.txt 

#test test test 111
#test test 222
#test 333
#444

#执行后
#cat test.txt
 
#exec exec exec 111
#exec exec 222
#exec 333
#444

























