#!/usr/bin/env python
#coding=utf-8
import os, sys, time
from optparse import OptionParser
from kafka import KafkaProducer
def on_send_success(record_metadata):
    print("success")
def on_send_error(excp):
    print(excp)
def main():
    parser = OptionParser()
    parser.add_option("-s", "--host", dest="host", help="server host")
    parser.add_option("-t", "--topic", dest="topic", help="topic of message")
    parser.add_option("-m", "--message", dest="message", help="message value")
    (options, args) = parser.parse_args()
     
    if options.host is None or options.topic is None:
        parser.print_help()
        exit()
    brokers = options.host.split(",")
    producer = KafkaProducer(bootstrap_servers=brokers)
    if options.message is not None:
        producer.send(options.topic,bytes(options.message,encoding = "utf8")).add_callback(on_send_success).add_errback(on_send_error)
    elif args[0] is not None:
        producer.send(options.topic,bytes(args[0],encoding = "utf8")).add_callback(on_send_success).add_errback(on_send_error)
    else:
        print("miss message contect");
        
    producer.flush()
if __name__ == '__main__':
    main()