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
import base64
import StringIO

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

from thriftdef.ProtoService import ProtoRpcService
from thriftdef.ProtoService.ttypes import ProtoRequest
#import _thread
#import threading
log_format = "%(asctime)s - %(levelname)s - %(message)s"
logging.basicConfig(filename='sailor.log', format=log_format, level=logging.INFO, filemode='a')

sys.path.append("./protobuf_new/")
sys.path.append("./protobuf_new/paipai_world/common")
sys.path.append("./protobuf_new/paipai_world/server")

from protobuf_new.paipai_world.common.DrawCardSailor_pb2 import UserLuckyCardRouletteRQ
from protobuf_new.paipai_world.common.DrawCardSailor_pb2 import UserLuckyCardRouletteRS


from config.rpc_type import *

from dbconfig.rpc_proxy_config import *

from google.protobuf.text_format import MessageToString

from dbconfig.dbRedisConfig import redisValetData

lucky_ticket_queue = redis.StrictRedis(host="10.101.166.84",port=9011) # 加高级抽奖券 队列地址

sailor_svr_str_redis = redis.StrictRedis(host="10.101.166.240",port=16000) # 抽卡回写结果key 组6的


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
user_list = [218006923,218009424] # 先拿10个做测试
special_class = 5

UN_TCP_PACK_CLI_DRAW_SAILOR_CARD_RS = 10963 # 酒馆抽航海士应答包  chenhui 2019-12-17 11:59:03
UN_TCP_PACK_CLI_DRAW_SAILOR_CARD_RQ = 10962 # 酒馆抽航海士请求包  chenhui 2019-12-17 11:59:03
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
    def draw_update_special(self, quality, sailor_id, count):
        if quality_limit[quality] <= 0:
            return 0
        self.count += count
        self.draw_cache[quality].update(sailor_id, count)
        if self.draw_cache[quality].complete():
            detail_log = self.draw_cache[quality].log_debug()
            draw_log = self.log_debug()
            log = "%s %s"%(draw_log,detail_log)
            logging.warning(log)
            return 1
        return 2


def rpcDealTwowayMessage(type, msg):
    reply = UserLuckyCardRouletteRS()
    try:
        rq = ProtoRequest()
        rq.type = int(type)
        rq.content = msg.SerializePartialToString()  # Make socket
        rq.sharding_id = msg.user_id  # msg.user_id

        ip = "10.101.174.2"
        port = 7882

        #print ip
        #print port
        transport = TSocket.TSocket(ip, port)
        # Buffering is critical. Raw sockets are very slow
        transport = TTransport.TBufferedTransport(transport)
        # Wrap in a protocol
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        # Create a client to use the protocol encoder
        client = ProtoRpcService.Client(protocol)
        # Connect!
        transport.open()
        if transport.isOpen() is True:
            time_start = time.time()

            try:
                rs = client.dealTwowayMessage(rq)
            except:
                print 'InvalidOperation'

            time_end = time.time()
            #print str(time_start) + ":" + str(time_end)

            # Close!
            transport.close()

        if rs.type == UN_TCP_PACK_CLI_DRAW_SAILOR_CARD_RS:
            reply.ParseFromString(rs.content)


    except Thrift.TException, tx:
        print '%s' % (tx.message)
        return 0,None

    return rs.type, reply


# 10连抽
def drawSailorCard(userId, draw_type):
    print "-----------draw sailor card info------------"
    user_draw_sailor = UserDrawSailor(userId)
    user_count = 0
    while 1:
        request = UserLuckyCardRouletteRQ()
        request.user_id = int(userId)
        request.draw_type = draw_type
        request.is_continuity = int(1)
        request.is_free = 0
        request.draw_count = 10
        #print MessageToString(request)

        reply_type, reply = rpcDealTwowayMessage(UN_TCP_PACK_CLI_DRAW_SAILOR_CARD_RQ, request)
        if reply_type==0:
            print "timeout"
            continue
        user_count += 10
        #print reply_type
        #print MessageToString(reply)
        for result in reply.item_list:
            quality = result.drop_quality
            sailor_id = result.item_id
            count = result.count

            if(quality == special_class):
                res = user_draw_sailor.draw_update_special(quality, sailor_id, count)
                if res == 1:
                    logging.warning("complete: %s,%s"% (userId,user_count))
                    return 1
            else:
                user_draw_sailor.draw_update(quality, sailor_id, count)


# 批量用户抽
def muti_draw_sailor_card():
    user_min = user_list[0]
    user_max = user_list[1] + 1
    print("min:%d  max: %d"%(user_min,user_max-1))
    for user_id in range(user_min, user_max):
        #print(user_id)
        drawSailorCard(user_id, int(1)) # 目前只是啤酒
        time.sleep(0.01)


# 给某个yoghurt加啤酒券
def add_ppwd_lucky_ticket(userId):

    key = "ppwd:high:lucky:ticket:value:queue"

    result = {}
    result["user_id"] = int(userId)
    result["value"] = 10000 # 加8000个啤酒券
    result["type"] = 1
    result["time"] = int(time.time())
    result["order_id"] = int(time.time())*1000
    result["desc"] = "probability_ceshiceshi"
    result["session"] = "probability_ceshiceshi"
    result["value_type"] = 10
    result["peer_id"] = 0

    result_str = json.dumps(result)

    lucky_ticket_queue.lpush(key,result_str)

    log = "key: %s value: %s"%(key, result_str)
    logging.info(log)

# 批量加啤酒卷
def muti_add_all_user_ticket():
    user_min = user_list[0]
    user_max = user_list[1] + 1
    print("min:%d  max: %d"%(user_min,user_max-1))
    for user_id in range(user_min, user_max):
        print(user_id)
        add_ppwd_lucky_ticket(user_id)


#清除酒馆抽卡结果
def clear_sailor_svr_result(userId):
    key = "user:%d:draw:drop:sailor:hash"%userId
    sailor_svr_str_redis.delete(key)
    logging.info("delete svr result key: %s"%key)

#批量 清除酒馆抽卡结果
def muti_clear_sailor_svr_result():

    user_min = user_list[0]
    user_max = user_list[1] + 1
    print("muti clear min:%d  max: %d"%(user_min,user_max-1))
    for user_id in range(user_min, user_max):
        print("muti clear, user_id: %d "%user_id)
        clear_sailor_svr_result(user_id)


def test_single(userId, draw_type):
    #add_ppwd_lucky_ticket(userId)
    drawSailorCard(userId, draw_type)
    #clear_sailor_svr_result(userId)

def main():
    reload(sys)
    sys.setdefaultencoding("utf-8")

    draw_type = int(sys.argv[1])
    user_id = int(sys.argv[2])

    test_single(user_id, draw_type)
    #muti_add_all_user_ticket()
    #muti_draw_sailor_card()
    return


if __name__ == '__main__':
    main()

# python sailor_real.py 2 9003200100005495
# 调用 酒馆抽卡数据统计 概率