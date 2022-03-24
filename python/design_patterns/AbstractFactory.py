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

logging.basicConfig(filename='AbsFactory.log', format=log_format, level=logging.INFO, filemode='a')

""" shape 相关 """
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

""" Color 相关 """

class Color:
	def __init__(self):
		logging.info("Color init")

	def fill(self):
		logging.info(" fill color ")

class Red(Color):
	"""docstring for Red"""
	def __init__(self):
		logging.info(" Red init ")

	def fill(self):
		logging.info(" fill red ")

class Green(Color):
	def __init__(self):
		logging.info(" Green init ")

	def fill(self):
		logging.info(" fill green ")

class Blue(Color):
	def __init__(self):
		logging.info(" Blue init ")

	def fill(self):
		logging.info(" fill blue ")


""" factory 相关 """
class AbstraceFactory:
	"""docstring for ClassName"""
	def __init__(self):
		logging.info(" AbstraceFactory init ")

	def get_shape(shape):
		logging.info(" AbstraceFactory get_shape() ")

	def get_color(color):
		logging.info(" AbstraceFactory get_color() ")


class ShapeFactory(AbstraceFactory):
    def __init__(self):
        super(ShapeFactory, self).__init__()
        logging.info(" ShapeFactory init ")	

    def get_shape(self, type):
    	if type == "circle":
    		return Circle()
    	elif type == "square":
    		return Square()
    	elif type == "rectangle":
    		return Rectangle()
    	else:
    		return Shape()

    def get_color(self):
    	pass

class ColorFactory(AbstraceFactory):
	def __init__(self):
		super(ColorFactory, self).__init__()
		logging.info(" ColorFactory init ")

	def get_shape(self):
		pass

	def get_color(self, color):
		if color == "Red":
			return Red()
		elif color == "Green":
			return Green()
		elif color == "Blue":
			return Blue()
		else:
			return Color()

class FactoryProducer:
	def __init__(self):
		logging.info(" FactoryProducer init ")

	def get_factory(self, type):
		if type == "shape":
			return ShapeFactory()
		elif type == "color":
			return ColorFactory()
		else:
			logging.warning(" factory type error. ")


def main():

    producer = FactoryProducer()

    shape_factory = producer.get_factory("shape")
    color_factory = producer.get_factory("color")

    shape1 = shape_factory.get_shape("circle")
    shape1.draw()

    shape2 = shape_factory.get_shape("square")
    shape2.draw()

    shape3 = shape_factory.get_shape("rectangle")
    shape3.draw()

    shape4 = shape_factory.get_shape("shape")
    shape4.draw()

    color1 = color_factory.get_color("Red")
    color1.fill()

    color2 = color_factory.get_color("Green")
    color2.fill()

    color3 = color_factory.get_color("Blue")
    color3.fill()


if __name__ == '__main__':
    main()