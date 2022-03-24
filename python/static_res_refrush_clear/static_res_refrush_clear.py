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
import ctypes
import inspect # 收集python对象信息 获取 类或函数的参数的信息，源码，解析堆栈，对对对象进行类型检查
import setting
from logging.handlers import RotatingFileHandler

# 日志
#log_format = "%(asctime)s - %(levelname)s - %(message)s"  
#logging.basicConfig(filename='static_res_refrush_clear.log', format=log_format, level=logging.INFO, filemode='a')


class PopClearCacheObj:
    pop_key = "clear:trade:searoute:position" # 类变量
    write_back_key = "group:%s:resource:progress:v:%s"

    def __init__(self):  # 构造函数
        # 类实例变量
        self.name = ""
        self.proxy_conn = redis.StrictRedis(host="10.103.63.239",port=65005) # codis前端代理地址
        self.addr_list = [] # codis 后端从 地址 ["10.103.63.205:7002", "10.103.63.206:7002", "10.103.63.204:7003"]
        self.pop_conn = redis.StrictRedis(host="10.101.171.238",port=65005) # pop队列地址
        self.result_conn = redis.StrictRedis(host="10.101.171.238",port=65005) # 回写结果地址

        #logging.warning("proxy_conn: %s, addr_list: %s"%(self.proxy_conn,self.addr_list))

    def __del__(self): # 析构函数
        # print (" %s  ~del~"%self.name)
        logging.info(" %s  ~del~"%self.name)
    
    def create_json_obj(self):
        json_obj = {}
        json_obj["map_id"] = 245
        json_obj["version"] = 680
        json_str = json.dumps(json_obj) # json对象转为字符串
        print ("json_obj: %s,  json_str: %s"%(json_obj,json_str))
        return json_str

    def parse_json_obj(self, json_str):
        map_id = []
        version = 0 
        now_version = 0
        group_id = 0
        refresh_type = 0
        try :
            json_obj = json.loads(json_str) 
            # "{\"group_id\":\"3\",\"map_id\":[117,139,140,141,142,143,147],\"refresh_type\":\"10006\",\"old_version\":\"0.4.427\",\"now_version\":\"0.4.427\",\"status\":\"1\",\"start_time\":\"1609300609\",\"end_time\":\"0\"}"
        except:
            logging.info("invaild data: %s"%json_str)
        else:
            logging.info("receive clear data: %s"%json_str)
            if json_obj.items() :
                for k,v in json_obj.items():
                    if k == "map_id" :
                        map_id = v
                    elif k == "now_version" :
                        now_version = v
                        version1, version2, version = now_version.split(".")
                    elif k == "group_id" :
                        group_id = v
                    elif k == "refresh_type" :
                        refresh_type = v

                    #print ("k: %s, v: %s"%(k,v))
        # print ("map_id: %s, version: %s"%(map_id,version))
        return map_id, version, now_version, group_id, refresh_type

    def pop_list(self):
        logging.warning("begin pop list!, key= %s  pop_address: %s",self.__class__.pop_key, self.pop_conn)
        while(1):
        
            bytes_data = self.pop_conn.rpop(self.__class__.pop_key);
            if bytes_data is None :
                time.sleep(1)
                logging.info(bytes_data)
            
            else : 
                #logging.warning(bytes_data)
                map_id_list = []
                map_id_list,version,now_version,group_id,refresh_type = self.parse_json_obj(bytes_data)
                if len(map_id_list) > 0 and version != 0 :
                    logging.warning("correct data, map static resource refresh, map_id_list_size: %d, now_version: %s, group_id: %s"%(len(map_id_list),now_version,group_id))
                    self.scan_trade_key()
                    for map_id in map_id_list:
                        logging.warning("correct data, map static resource refresh, map_id: %s, version: %s"%(map_id,version))
                        self.scan_trade_island_hash(map_id)
                        self.scan_pearl_door_hash(map_id)
                    # 全部地图清除成功 回写结果
                    self.write_back_result(group_id, now_version, map_id_list, refresh_type)
                else:
                    logging.info("invaild data, map_id_list: %s, version: %s, source_data: %s"%(map_id_list,version,bytes_data)) 


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
                self.proxy_conn.delete(key)    

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
                self.proxy_conn.delete(key)

    """ 清除 贸易岛 航线 起止点 坐标 string """
    def scan_trade_key(self):
    
        logging.warning(" begin clean trade sea route pos string. ")
        for addr in self.addr_list:
            host, port = addr.split(":")
            conn = redis.Redis(host=host, port=port)
            for key in conn.scan_iter(count=2000, match="user:*:task:*:searoute:pos:*"):
                logging.warning(key)
                self.proxy_conn.delete(key)

    """ 回写 清除资源 结果 """
    def write_back_result(self,group_id,version,map_id_list,refresh_type):
        """
        {
            "server_name":"gold_resource",
            "status":100,   //200 成功  
            "desc":"xxx"
            "other_data":"[{"sss":"ssss"}]"
            "detail":[
                {
                    "field":"xxx1",
                    "status":200,
                    "desc":"成功"
                },
                {
                    "field":"xxx2",
                    "status":101,
                    "desc":"xxx更新失败"
                }
            ]
        }"""
        result = {}
        result["server_name"] = "clear_trade_sea_route_pos_script"
        result["status"] = 200 # 200 成功
        result["desc"] = "成功"
        result["server_id"] = refresh_type
        result["detail"] = []
        for map_id in map_id_list:
            detail = {}
            detail["field"] = map_id
            detail["status"] = 200
            detail["desc"] = "成功"
            result["detail"].append(detail)

        key = (self.__class__.write_back_key%(group_id,version))

        result_str = json.dumps(result)
        self.result_conn.lpush(key, result_str)
        logging.warning("write_back_result, key:%s, str:%s"%(key, result_str))




""" 静态资源清除缓存管理 """
class PopManager():

    """docstring for test_manager"""
    def __init__(self):
        self.cfg_mgr = []
        self.group_obj_mgr = []  #分组对象 数组 
        self.thread_mgr = [] # 线程 数组

    def __del__(self):
        self.release()
        #print ("PopManager ~del~")
        logging.info("PopManager ~del~")

    def run(self):

        for item in self.cfg_mgr :
            obj = PopClearCacheObj()
            obj.name = item["name"];
            obj.proxy_conn = redis.StrictRedis(host=item["proxy_host"],port=item["proxy_port"]); # codis前端代理地址
            for addr in item["addr_list"]:
                obj.addr_list.append(addr) # codis 后端从 地址
            obj.pop_conn = redis.StrictRedis(host=item["pop_host"],port=item["pop_port"],db=item["pop_db"]); # pop刷资源通知地址
            obj.result_conn = redis.StrictRedis(host=item["result_host"],port=item["result_port"]); # 回写结果队列地址地址
            self.group_obj_mgr.append(obj)

        # 创建线程对象
        logging.info(" create thread obj. ")
        for obj in self.group_obj_mgr :
            pop_thd = threading.Thread(target=obj.pop_list)
            self.thread_mgr.append(pop_thd)
            #logging.warning(" create thread obj. %s",obj.pop_conn)

        # 启动线程
        logging.info(" start run thread. ")
        for thd in self.thread_mgr :
            thd.start()
            #logging.warning(" start thread obj. %s",obj.pop_conn)


    """ 释放对象 结束线程 """
    def release(self):
        logging.warning("PopManager begin release")
        # 停止线程
        for thd in self.thread_mgr:
            # thd.join()
            stop_thread(thd)
        # logging.info("all pop thread stop end.")

        # 删除对象
        for obj in self.group_obj_mgr:
            del obj
        # logging.info("all pop obj del end.")

        # 清空数组
        self.group_obj_mgr.clear()
        self.thread_mgr.clear()
        self.cfg_mgr.clear()
        logging.warning("PopManager release end.")


""" conf.json 解析 """
class Config():
    
    def __init__(self):
        self.con_file_name = "conf.json" # 配置文件名  类实例变量


    def parse_json_config(self):
        config_mgr = [] # 配置列表
        fr = open(self.con_file_name)
        content = fr.read()
        # print (content)

        try :
            json_obj = json.loads(content)
        except:
            logging.info(" parse conf.json fail, content: %s"%content)
        else:

            for k,v in json_obj.items():
                if k == "grp_cfg" :
                    config_mgr = v

        return config_mgr


    def compare_config(self, cfg_1, cfg_2):
        change = 0
        if len(cfg_1) != len(cfg_2) :
            change = 1
        else:
            for index in range(0, len(cfg_1) ):
                if set(cfg_1[index].keys()) != set(cfg_2[index].keys()):
                    change = 1
                    break
                #if set(cfg_1[index].values()) != set(cfg_2[index].values()):
                    #change = 1
                    #break
        return change


""" 抛出异常，结束子线程 """
def _async_raise(tid, exctype):
    """raises the exception, performs cleanup if needed"""
    tid = ctypes.c_long(tid)
    if not inspect.isclass(exctype):
        exctype = type(exctype)
    res = ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, ctypes.py_object(exctype))
    if res == 0:
        raise ValueError("invalid thread id")
    elif res != 1:
        # """if it returns a number greater than one, you're in trouble,
        # and you should call it again with exc=NULL to revert the effect"""
        ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, None)
        raise SystemError("PyThreadState_SetAsyncExc failed")
 
""" 结束线程 """
def stop_thread(thread):
    _async_raise(thread.ident, SystemExit)


""" 每隔1s检查配置文件 """
def check_thread(pop_manager, config):

    while (1):
        cfg_tuple = config.parse_json_config()
        change = config.compare_config(cfg_tuple, pop_manager.cfg_mgr)

        if change:
            pop_manager.release() # 释放资源
            pop_manager.cfg_mgr = cfg_tuple # 更新配置信息
            pop_manager.run()
            # print (" start pop success !")
            logging.warning(" start pop success !")
        #else:
            #print (" config file no change. \n")
            #logging.info(" config file no change. ")

        time.sleep(1) # 1s检查 1次 配置文件 是否有变化

# 初始化日志
def int_log(log_name):
    handler = RotatingFileHandler(log_name, maxBytes=1024 * 1024 * 1024, backupCount=9999)
    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
    handler.setFormatter(formatter)
    logging.getLogger('').setLevel(logging.INFO)
    logging.getLogger('').addHandler(handler)

def main():

    #thread = threading.Thread(target=lpush_list,kwargs={"thread_id":1,"thread_name":"test_1"})
    #thread = threading.Thread(target=lpush_list,args=(1,"test_1"))
    #thread.start()

    pop_manager = PopManager()
    config = Config()

    check_thd = threading.Thread(target=check_thread, args=(pop_manager,config) )
    logging.warning(" check thread begin! ")
    check_thd.start()




if __name__ == '__main__':
    int_log(setting.LOG_PATH)
    main()




"""
思路：
    多线程 pop 队列  每组一个线程  也可以每组多个线程
    队列 取到 静态资源刷新通知  扫redis的key 清除对应地图的数据

    动态 读取配置文件 版本 
"""