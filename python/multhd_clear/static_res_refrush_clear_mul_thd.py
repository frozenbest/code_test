#!/usr/bin/env python
#coding=utf-8

import os
import sys
import re
import redis
import logging
import time
import json
import threading

# 日志
log_format = "%(asctime)s - %(levelname)s - %(message)s"  
logging.basicConfig(filename='static_res_refrush_clear.log', format=log_format, level=logging.INFO, filemode='a')


class PopClearCacheObj:
    pop_key = "clear:trade:searoute:position" # 类变量

    def __init__(self):  # 构造函数
        # 类实例变量
        self.name = ""
        self.proxy_conn = redis.StrictRedis(host="10.103.63.239",port=65005) # codis前端代理地址
        self.addr_list = [] # codis 后端从 地址 ["10.103.63.205:7002", "10.103.63.206:7002", "10.103.63.204:7003"]
        self.pop_conn = redis.StrictRedis(host="10.101.171.238",port=65005) # codis前端代理地址
        #logging.warning("proxy_conn: %s, addr_list: %s"%(self.proxy_conn,self.addr_list))

    def __del__(self): # 析构函数
        print (" %s  ~del~"%self.name)
        logging.info(" %s  ~del~"%self.name)
    
    def create_json_obj(self):
        json_obj = {}
        json_obj["map_id"] = 245
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
                    #print ("k: %s, v: %s"%(k,v))
        # print ("map_id: %s, version: %s"%(map_id,version))
        return map_id, version

    def pop_list(self):
        logging.warning("begin pop list!, key= %s  pop_address: %s",self.__class__.pop_key, self.pop_conn)
        while(1):
        
            bytes_data = self.pop_conn.rpop(self.__class__.pop_key);
            if bytes_data is None :
                time.sleep(1)
                logging.info(bytes_data)
            
            else : 
                #logging.warning(bytes_data)
                map_id,version = self.parse_json_obj(bytes_data)
                if map_id != 0 and version != 0 :
                    logging.warning("correct data, map static resource refresh, map_id: %d, version: %d"%(map_id,version))
                    # self.scan_trade_island_hash(map_id)
                    # self.scan_pearl_door_hash(map_id)
                    # self.scan_trade_key()
                else:
                    logging.info("invaild data, map_id: %d, version: %d, source_data: %s"%(map_id,version,bytes_data)) 


    """ 清除 指定地图 家族锅盖 到 贸易岛 hash  """
    def scan_trade_island_hash(self,map_id):

        logging.warning(" begin clean trade island hash. ")
        for addr in self.addr_list:
            host, port = addr.split(":")
            conn = redis.Redis(host=host, port=port)
            for key in conn.scan_iter(count=2000, match="trade:island:wld:%s:family:element:[0-9]*:[0-9]*"%map_id):
                logging.warning(key)
                datas = self.proxy_conn.hgetall(key)
                for k,v in datas.items():
                    logging.info(k)
                    logging.info(v)
                #self.proxy_conn.delete(key)    

    """ 清除 指定地图 家族锅盖 到 珍珠矿门 hash """
    def scan_pearl_door_hash(self,map_id):

        logging.warning(" begin clean pearl door hash. ")
        for addr in self.addr_list:
            host, port = addr.split(":")
            conn = redis.Redis(host=host, port=port)
            for key in conn.scan_iter(count=2000, match="pearl:door:wld:%s:family:element:[0-9]*:[0-9]*"%map_id):
                logging.warning(key)
                datas = self.proxy_conn.hgetall(key)
                for k,v in datas.items():
                    logging.info('field= %s, value= %s', k ,v)
                #self.proxy_conn.delete(key)

    """ 清除 贸易岛 航线 起止点 坐标 string """
    def scan_trade_key(self):
    
        logging.warning(" begin clean trade sea route pos string. ")
        for addr in self.addr_list:
            host, port = addr.split(":")
            conn = redis.Redis(host=host, port=port)
            for key in conn.scan_iter(count=2000, match="user:*:task:*:searoute:pos:*"):
                logging.warning(key)
                #self.proxy_conn.delete(key)


""" 静态资源清除缓存管理 """
class PopManager():

    """docstring for test_manager"""
    def __init__(self):
        self.cfg_mgr = []
        self.group_obj_mgr = []  #分组对象 数组 
        self.thread_mgr = [] # 线程 数组

    def __del__(self):
        self.release()
        print ("PopManager ~del~")
        logging.info("PopManager ~del~")

    def run(self):

        for item in self.cfg_mgr :
            obj = PopClearCacheObj()
            obj.name = item["name"];
            obj.proxy_conn = redis.StrictRedis(host=item["proxy_host"],port=item["proxy_port"]); # codis前端代理地址
            for addr in item["addr_list"]:
                obj.addr_list.append(addr) # codis 后端从 地址
            obj.pop_conn = redis.StrictRedis(host=item["pop_host"],port=item["pop_port"],db=item["pop_db"]); # codis前端代理地址
            self.group_obj_mgr.append(obj)

        # 创建线程对象
        logging.warning(" create thread obj. ")
        for obj in self.group_obj_mgr :
            pop_thd = threading.Thread(target=obj.pop_list)
            self.thread_mgr.append(pop_thd)
            #logging.warning(" create thread obj. %s",obj.pop_conn)

        # 启动线程
        logging.warning(" start run thread. ")
        for thd in self.thread_mgr :
            thd.start()
            #logging.warning(" start thread obj. %s",obj.pop_conn)


    def release(self):
        # 停止线程
        for thd in self.thread_mgr:
            thd.join()

        # 删除对象
        for obj in self.group_obj_mgr:
            del obj

        # 清空数组
        self.group_obj_mgr.clear()
        self.thread_mgr.clear()
        self.cfg_mgr.clear()


""" conf.json 解析 """
class Config():
    
    def __init__(self):
        self.con_file_name = "conf.json" # 配置文件名  类实例变量


    def parse_json_config(self):
        config_mgr = [] # 配置数组
        fr = open(self.con_file_name)
        content = fr.read()
        print (content)

        try :
            json_obj = json.loads(content)
        except:
            logging.info(" parse conf.json fail, content: %s"%content)
        else:

            for k,v in json_obj.items():
                if k == "grp_cfg" :
                    config_mgr = v

        return config_mgr


def main():

    #thread = threading.Thread(target=lpush_list,kwargs={"thread_id":1,"thread_name":"test_1"})
    #thread = threading.Thread(target=lpush_list,args=(1,"test_1"))
    #thread.start()

    pop_manager = PopManager()

    config = Config()
    pop_manager.cfg_mgr = config.parse_json_config()

    pop_manager.run()

    print (" start pop success !")
    logging.warning(" start pop success !")


if __name__ == '__main__':
    main()




"""
思路：
    多线程 pop 队列  每组一个线程  也可以每组多个线程
    队列 取到 静态资源刷新通知  扫redis的key 清除对应地图的数据

TODO:
    开一个线程 动态 读取配置文件  
"""