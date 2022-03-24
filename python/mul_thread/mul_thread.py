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

import Queue

class ThreadManager(Thread):
    def __init__(self, work_queue):
        Thread.__init__(self)
        self.work_queue = work_queue
        self.daemon = True

    def run(self):
        while 1:
            target, args = self.work_queue.get()
            target(*args)
            self.work_queue.task_done()

class ThreadPoolManager():
    def __init__(self, thread_num):
        self.work_queue = Queue()
        self.thread_num = thread_num
        self.__init_threading_pool(self.thread_num)

    def __init_threading_pool(self, thread_num):
        for i in range(thread_num):
            thread = ThreadManager(self.work_queue)
            thread.start()

    def add_job(self, func, *args):
        self.work_queue.put((func, args))


class ThreadPool():
    def __init__(self,max_thread=20):
        self.queue = Queue.Queue(max_thread)
        for i in xrange(max_thread):
            self.queue.put(threading.Thread)

    def get_thread(self):
        return self.queue.get()

    def add_thread(self):
        self.queue.put(threading.Thread)



""" 静态资源清除缓存管理 """
class PopManager():

    """docstring for test_manager"""
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