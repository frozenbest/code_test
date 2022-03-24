#!/usr/bin/env python
#coding=utf-8

import os
import sys
import re
import redis
import logging
import time
import json
import random
#import _thread
#import threading
log_format = "%(asctime)s - %(levelname)s - %(message)s"
logging.basicConfig(filename='sailor.log', format=log_format, level=logging.INFO, filemode='a')



""" 酒馆抽卡相关概率和配置 开始 """
# 航海士id 列表
sailor_list = [34550101, 34550102, 34550103, 34550104, 34550105, 34550106, 34550107, 34550108, 
34550201, 34550202, 34550203, 34550204, 34550205, 34550206, 34550207, 34550208,
34550301, 34550302, 34550303, 34550304, 34550305, 34550306, 34550307, 34550308,
34550601, 34550602, 34550603, 34550604, 34550605, 34550606, 34550607, 34550608, 
34550901, 34550902, 34550903, 34550904, 34550905, 34550906, 34550907, 34550908]
# 航海士品质列表
quality_list = [2, 3, 4, 5]
quality_des = {2:"D级", 3:"C级", 4:"B级", 5:"A级"}
quality_limit = {2:-1, 3:-1, 4:-1, 5:20}
""" 单抽概率 """
single_quality_dict = {2:69, 3:20, 4:10, 5:1}
""" 10连抽概率 """
# 10连抽必出 5 级卡
continuous_quality_dict = {2:90, 3:9, 4:1} 

"""用户列表"""
user_list = [90010001,90010001] # 先拿10个做测试
""" 酒馆抽卡相关概率和配置 结束 """



""" 某用户 某品质 抽到的航海士 以及 对应的数量 """ 
class SailorCache:

    def __init__(self, quality):  # 构造函数
        # 类实例变量
        self.sum = 0 #总数
        self.quality = quality
        self.special_sailor_list = []
        self.curr_sailor_dict = {}
        for sailor_id in sailor_list:
            self.curr_sailor_dict[sailor_id] = 0
            self.special_sailor_list.append(sailor_id)
        self.full = False # 是否集齐该品质航海士

    def log_debug(self):
        log = " SailorCache quality: %d. des: %s draw_quality_sum: %d detail"%(self.quality, quality_des[self.quality], self.sum)
        #for k,v in self.curr_sailor_dict.items():
            #log += " sailor_id: %d, count: %d;"%(k,v)
        return log 

    # 更新抽卡记录
    def update(self, sailor_id, count):
    	self.curr_sailor_dict[sailor_id] += count
        self.sum += count
        if quality_limit[self.quality] > 0 and self.curr_sailor_dict[sailor_id] == quality_limit[self.quality]:
            self.special_sailor_list.remove(sailor_id)


        #self.log_debug()

    # 是否抽满所有卡
    def complete(self):
        count = 0
        for k,v in self.curr_sailor_dict.items():
            if v > 0:
                count += 1

        if count >= len(sailor_list) and bool(1-self.full) :
            self.full = True
            return True
        else:
            return False

    # 抽取有上限的卡
    def draw_limit_sailor(self):
        sailor_random = 0
        if len(self.special_sailor_list) > 0:
            sailor_random = get_random( 0, len(self.special_sailor_list)-1 ) # 随机一个航海士id索引
            sailor_id = self.special_sailor_list[sailor_random]
            logging.info("update limit sailor_id: %d"%sailor_id)
        else:
            sailor_random = get_random( 0, len(sailor_list)-1 ) # 随机一个航海士id索引
            sailor_id = sailor_list[sailor_random]
            logging.info("update normal sailor_id: %d"%sailor_id)
        return sailor_id



""" 某用户  <品质,SailorCache> """
class UserDrawSailor:
    """docstring for ClassName"""
    def __init__(self, user_id):
        self.user_id = user_id
        self.count = 0  #该用户抽卡次数
        self.draw_cache = {} # <品质,SailorCache>
        for quality in quality_list:
            sailor_cache = SailorCache(quality)
            self.draw_cache[quality] = sailor_cache
    
    # 日志打印
    def log_debug(self):
        log = " UserDrawSailor user_id: %d, total_count: %d"%(self.user_id, self.count)
        return log

    # 更新抽卡记录
    def draw_update(self, quality, sailor_id, count):
        self.count += count
        self.log_debug()
        self.draw_cache[quality].update(sailor_id, count)
        if self.draw_cache[quality].complete():
            detail_log = self.draw_cache[quality].log_debug()
            draw_log = self.log_debug()
            log = "%s %s"%(draw_log,detail_log)
            logging.warning(log)

    # 更新抽卡记录
    def draw_update_special(self, quality, count):
        if quality_limit[quality] <= 0:
            return
        self.count += count
        sailor_id = self.draw_cache[quality].draw_limit_sailor()
        self.draw_cache[quality].update(sailor_id, count)
        if self.draw_cache[quality].complete():
            detail_log = self.draw_cache[quality].log_debug()
            draw_log = self.log_debug()
            log = "%s %s"%(draw_log,detail_log)
            logging.warning(log)
        
# 生成随机数
def get_random(min, max):
    random_num = random.randint(min,max)
    return random_num

""" 连抽 """
# 一次10连抽
def draw_ten_continuous(user_draw_sailor):
    count = 1
    a_class_quality = 5 # A卡品质
    # 增加统计
    user_draw_sailor.draw_update_special(a_class_quality, count)

    for index in range(0,9): #循环9次 
        sailor_random = get_random( 0, len(sailor_list)-1 ) # 随机一个航海士id索引
        sailor_id = sailor_list[sailor_random]
        # 随机航海士品质 
        quality = 0
        quality_random = get_random(1, 100)
        quality_curr = 0
        for k,v in continuous_quality_dict.items():
            quality_curr += v
            if quality_curr >= quality_random:
                quality = k
                break
        log = " draw_ten_continuous(): quality: %d sailor_id: %d count: %d"%(quality,sailor_id,count)
        logging.info(log)
        # 增加统计
        user_draw_sailor.draw_update(quality, sailor_id, count)
# 连抽
def muti_user_draw_continuous(count):
    user_min = user_list[0]
    user_max = user_list[1] + 1
    print("min:%d  max: %d"%(user_min,user_max-1))
    for user_id in range(user_min, user_max):
        print(user_id)
        user_draw_sailor = UserDrawSailor(user_id)
        for index in range(0,count):
            draw_ten_continuous(user_draw_sailor)


""" 连抽 """


""" 单抽 """
# 单抽
def draw_single(user_draw_sailor):
    quality = 0 # 抽到的航海士品质
    sailor_id = 0 # 抽到的航海士id
    count = 1 # 抽到的航海士数量

    # 随机航海士id
    sailor_random = get_random( 0, len(sailor_list)-1 ) # 随机一个航海士id索引
    sailor_id = sailor_list[sailor_random]
    # 随机航海士品质 
    quality_random = get_random(1, 100)
    quality_curr = 0
    for k,v in single_quality_dict.items():
        quality_curr += v
        if quality_curr >= quality_random:
            quality = k
            break
    log = " draw_single(): quality: %d sailor_id: %d count: %d"%(quality,sailor_id,count)
    logging.info(log)
    # 增加统计
    user_draw_sailor.draw_update(quality, sailor_id, count)
    #return quality,sailor_id,count

def muti_user_draw_single(count):
    user_min = user_list[0]
    user_max = user_list[1] + 1
    print("min:%d  max: %d"%(user_min,user_max-1))
    for user_id in range(user_min, user_max):
        print(user_id)
        user_draw_sailor = UserDrawSailor(user_id)
        for index in range(0,count):
            draw_single(user_draw_sailor)

"""  单抽  """

# 酒馆抽卡概率仿真
def draw_sailor_official(mode,count):
    # mode = int(sys.argv[1]) # 模式 1:单抽 10:10连抽
    # count = int(sys.argv[2]) # 抽奖次数 
    if mode != 1 and mode != 10:
        print(" draw mode error!")
    if mode == 1:
        # print("--------单抽开始----------")
        logging.info("--------单抽开始----------")
        muti_user_draw_single(count)
        # print("========单抽结束==========")
        logging.info("========单抽结束==========")
    if mode == 10:
        # print("--------连抽开始----------")
        logging.info("--------连抽开始----------")
        muti_user_draw_continuous(count)
        # print("========连抽结束==========")
        logging.info("========连抽结束==========")


# 程序主函数
def main():

    mode = int(sys.argv[1]) # 模式 1:单抽 10:10连抽
    count = int(sys.argv[2]) # 抽奖次数 

    # 酒馆抽卡概率仿真
    draw_sailor_official(mode, count)


if __name__ == '__main__':
    main()



# /usr/local/svr_python/bin/python sailor_test.py 10 10000  # 10 抽奖模式:连抽 10000 抽一万次10连抽
# /usr/local/svr_python/bin/python sailor_test.py 1 10000   # 1  抽奖模式:单抽 10000 抽一万次单抽

# 模拟仿真 酒馆抽卡 概率
