#!/usr/bin/env python
#coding=utf-8

import os
import sys
import re
import redis
import logging
import time
import json

log_format = "%(asctime)s - %(levelname)s - %(message)s"

logging.basicConfig(filename='pop_clear_trade_list.log', format=log_format, level=logging.INFO, filemode='a')

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


class ClearTradeDate:
    pop_key = "wk:test" # 类变量
    proxy_conn = redis.StrictRedis(host="10.101.171.238",port=65005); # codis前端代理地址

    def __init__(self):  # 构造函数
        # 类实例变量
        self.proxy_conn = redis.StrictRedis(host="10.101.171.238",port=65005); # codis前端代理地址
        self.addr_list = ["10.101.63.205:7002", "10.101.63.206:7002", "10.101.63.204:7003"]; # codis 后端从 地址
        self.base_function = BaseFunction()
        print ("proxy_conn: %s, addr_list: %s"%(self.proxy_conn,self.addr_list))
        logging.warning("proxy_conn: %s, addr_list: %s"%(self.proxy_conn,self.addr_list))
    
    def create_json_obj(self):
        json_obj = {}
        json_obj["map_id"] = 43
        json_obj["version"] = 680
        json_str = json.dumps(json_obj) # json对象转为字符串
        print ("json_obj: %s,  json_str: %s"%(json_obj,json_str))
        return json_str

    def parse_json_obj(self, json_str):
        map_id = 0
        version = 0 
        try :
            json_obj = json.loads(json_str)
        except:
            logging.info("invaild data: %s"%json_str)
        else:

            if json_obj.items() :
                for k,v in json_obj.items():
                    if k == "map_id" :
                        map_id = v
                    elif k == "version" :
                        version = v
        return map_id, version

    def pop_list(self):
        # pop_key = "wk:test"
        logging.warning("begin pop list!, key= %s"%ClearTradeDate.pop_key)
        while(1):
        
            bytes_data = self.proxy_conn.rpop(self.__class__.pop_key)
            if bytes_data is None :
                time.sleep(1)
                logging.info(bytes_data)
            
            else : 
                map_id,version = self.parse_json_obj(bytes_data)
                if map_id != 0 and version != 0 :
                    logging.warning("correct data, map static resource refresh, map_id: %d, version: %d"%(map_id,version))
                else:
                    logging.info("invaild data, map_id: %d, version: %d, source_data: %s"%(map_id,version,bytes_data)) 

    # 类方法
    @classmethod
    def lpush_list(cls):
        json_str = cls.create_json_obj(cls) #类方法 内部 调用 类实例方法
        cls.proxy_conn.lpush(ClearTradeDate.pop_key, json_str)


def main():

    clear_test = ClearTradeDate()
    ClearTradeDate.lpush_list() # 类 调用 类方法
    clear_test.pop_list() # 类对象 调用 类实例方法


if __name__ == '__main__':
    main()