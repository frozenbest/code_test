/*
* 抽象工厂 exercise
*/
#include <memory>
#include <iostream>

using namespace std;

// 产品类
class Shape{
public:
	Shape() {
		std:cout<<" Shape init. "<<"\n";
	}
	virtual ~Shape() {
		std:cout<<" Shape destory. "<<"\n";
	}

public:
	void draw() const {
		std::cout<<" This is Shape. "<<"\n";
	}
};

class Circle:public Shape
{
public:
	Circle() {
		std:cout<<" Circle init. "<<"\n";
	}
	virtual ~Circle() {
		std:cout<<" Circle destory. "<<"\n";
	}
public:
	void draw() const {
		std::cout<<" This is Circle. "<<"\n";
	}
	
};

class Square:public Shape
{
public:
	Square() {
		std:cout<<" Square init. "<<"\n";
	}
	virtual ~Square() {
		std:cout<<" Square destory. "<<"\n";
	}
public:
	void draw() const {
		std::cout<<" This is Square. "<<"\n";
	}
	
};

class Rectangle:public Shape
{
public:
	Rectangle() {
		std:cout<<" Rectangle init. "<<"\n";
	}
	virtual ~Rectangle() {
		std:cout<<" Rectangle destory. "<<"\n";
	}
public:
	void draw() const {
		std::cout<<" This is Rectangle. "<<"\n";
	}
	
};

class Color{
public:
	Color() {
		std:cout<<" Color init. "<<"\n";
	}
	virtual ~Color() {
		std:cout<<" Color destory. "<<"\n";
	}

public:
	void fill() const {
		std::cout<<" Fill Color. "<<"\n";
	}
};

class Red:public Color
{
public:
	Red() {
		std:cout<<" Red init. "<<"\n";
	}
	virtual ~Red() {
		std:cout<<" Red destory. "<<"\n";
	}
public:
	void fill() const {
		std::cout<<" Fill Red. "<<"\n";
	}
	
};

class Green:public Color
{
public:
	Green() {
		std:cout<<" Green init. "<<"\n";
	}
	virtual ~Green() {
		std:cout<<" Green destory. "<<"\n";
	}
public:
	void fill() const {
		std::cout<<" Fill Green. "<<"\n";
	}
	
};

class Blue:public Color
{
public:
	Blue() {
		std:cout<<" Blue init. "<<"\n";
	}
	virtual ~Blue() {
		std:cout<<" Blue destory. "<<"\n";
	}
public:
	void fill() const {
		std::cout<<" Fill Blue. "<<"\n";
	}
	
};


// 抽象工厂
class AbsFactory
{
public:
	AbsFactory() {
		std:cout<<" AbsFactory init. "<<"\n";
	}
	virtual ~AbsFactory() {
		std:cout<<" AbsFactory destory. "<<"\n";
	}
public:
	Shape* getShape(const char* type) const {}

	Color* getColor(const char* color) const {}
	
};

// 形状工厂
class ShapeFactory:public AbsFactory
{
public:
	ShapeFactory() {
		std:cout<<" ShapeFactory init. "<<"\n";
	}
	virtual ~ShapeFactory() {
		std:cout<<" ShapeFactory destory. "<<"\n";
	}
public:
	Shape* getShape(const char* type) const {
		if(type == "Circle")
			return new Circle();
		else if(type == "Rectangle")
			return new Rectangle();
		else if(type == "Square")
			return new Square();
	}

	Color* getColor(const char* color) const {
		std:cout<<" empty. "<<"\n";
		return 0;
	}
	
};

// 颜色工厂
class ColorFactory:public AbsFactory
{
public:
	ColorFactory() {
		std:cout<<" ColorFactory init. "<<"\n";
	}
	virtual ~ColorFactory() {
		std:cout<<" ColorFactory destory. "<<"\n";
	}
public:
	Shape* getShape(const char* type) const {
		std:cout<<" empty. "<<"\n";
		return 0;
	}

	Color* getColor(const char* color) const {
		if(color == "Red")
			return new Red();
		else if(color == "Green")
			return new Green();
		else if(color == "Blue")
			return new Blue();
	}
	
};

// 工厂管理类
class FactoryManager
{
public:
	FactoryManager() {
		std:cout<<" FactoryManager init. "<<"\n";
	}
	~FactoryManager() {
		std:cout<<" FactoryManager destory. "<<"\n";
	}
public:
	AbsFactory* getFactory(const char* type) {
		if(type == "Shape")
			return new ShapeFactory();
		else if(type == "Color")
			return new ColorFactory();
	}
	
};

// 测试代码
int main() {

    FactoryManager manager;

    /* pointer */
    // shape
    AbsFactory* shape_factory = manager.getFactory("Shape");
    // color
    AbsFactory* color_factory = manager.getFactory("Color");
    // 父类裸指针转为子类指针 
    Circle* circle = dynamic_cast<Circle*>( shape_factory->getShape("Circle") );
    circle->draw();

    Red* red = dynamic_cast<Red*>( color_factory->getColor("Red") );
    red->fill();

    // 释放指针
    delete red;
    delete circle;
    delete shape_factory;
    delete color_factory;

}

