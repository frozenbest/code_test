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

logging.basicConfig(filename='Factory.log', format=log_format, level=logging.INFO, filemode='a')

class Shape:
	def __init__(self):
		logging.info("Shape init")

	def draw(self):
		logging.info("This is a Shape")


class Circle(Shape):
	def __init__(self):
		logging.info("Circle init")

	def draw(self):
		logging.info("This is a Circle")

class Square(Shape):
	def __init__(self):
		logging.info("Square init")

	def draw(self):
		logging.info("This is a Square")

class Rectangle(Shape):
	"""docstring for Rectangle"""
	def __init__(self):
		super(Rectangle, self).__init__() # 调用父类构造函数
		logging.info("Rectangle init")
		#self.arg = arg

	def draw(self):
		logging.info("This is a Rectangle")

class Factory():
	"""docstring for Factory"""
	def __init__(self):
		logging.info("Factory init")

	def get_shape(self, type):
		if type == "circle":
			return Circle()
		elif type == "square":
			return Square()
		elif type == "rectangle":
			return Rectangle()
		else:
			return Shape()



def main():

    factory = Factory()

    shape1 = factory.get_shape("circle")
    shape1.draw()

    shape2 = factory.get_shape("square")
    shape2.draw()

    shape3 = factory.get_shape("rectangle")
    shape3.draw()

    shape4 = factory.get_shape("shape")
    shape4.draw()


if __name__ == '__main__':
    main()