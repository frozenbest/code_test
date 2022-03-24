#!/usr/bin/env python
#coding=utf-8

import os
import sys
import re
#import redis
#import logging
import time
import json

#log_format = "%(asctime)s - %(levelname)s - %(message)s"

#logging.basicConfig(filename='test.log', format=log_format, level=logging.INFO, filemode='a')

class BaseFunction():
    """基础功能函数"""
    def __init__(self):
        pass

    def getType(self, variate):
        """ isinstance() 判断一个对象是否是一个已知类型 """
        type = None
        if isinstance(variate,int) :
            type = "int"
        elif isinstance(variate,str) :
            type = "str"
        elif isinstance(variate,float) :
            type = "float"
        elif isinstance(variate,list) :
            type = "list"
        elif isinstance(variate,tuple) :
            type = "tuple"
        elif isinstance(variate,dict) :
            type = "dict"
        elif isinstance(variate,set) :
            type = "set"

        return type
