/*
* Bridge 桥接模式
* 
* 使用场景: 
* 优点: 
* 缺点: 
*/

#include <thread>
#include <memory>
#include <iostream>
#include <vector>

using namespace std;

// 接口类
class DrawAPI{
public:
	virtual void drawCircle(int r, int x, int y) {}
};

class RedCircle:public DrawAPI{
public:
	void drawCircle(int r, int x, int y) {
		std::cout<<" Drawing Circle [ color: red, radius: "<< r << ", x: "<<x<<", y: "<<y<<" ] "<<"\n";
	}
};

class BlueCircle:public DrawAPI{
public:
	void drawCircle(int r, int x, int y) {
		std::cout<<" Drawing Circle [ color: blue, radius: "<< r << ", x: "<<x<<", y: "<<y<<" ] "<<"\n";
	}
};


// 产品类
class Shape{
public:
	Shape(DrawAPI draw_api) {
		this->draw_api = draw_api;
		std:cout<<" Shape init. "<<"\n";
	}
public:
	virtual ~Shape() {
		std:cout<<" Shape destory. "<<"\n";
	}

public:
	virtual void draw() = 0;

protected:
	DrawAPI draw_api;
};

class Circle:public Shape
{
public:
	Circle(int r, int x, int y, DrawAPI draw_api):Shape(draw_api) {
		this->r = r;
		this->x = x;
		this->y = y;
		std:cout<<" Circle init. "<<"\n";
	}
	virtual ~Circle() {
		std:cout<<" Circle destory. "<<"\n";
	}
public:
	void draw() {
		draw_api.drawCircle(r, x, y);
	}
private:
	int r; // 半径
	int x;
	int y;
	
};

// 主函数入口
int main() {

	RedCircle red_circle;
	BlueCircle blue_circle;

	Circle circle_1(8, 0, 0, red_circle);
	Circle circle_2(4, 2, 2, blue_circle);

	circle_1.draw();
	circle_2.draw();

}

