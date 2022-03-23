/*
* Template 前端控制模式
* 
* 使用场景: 
* 优点: 1.封装不变部分,扩展可变部分 2.提取公共代码，便于维护 3.行为由父类控制,子类实现
* 缺点: 每一个不同的实现都需要一个子类实现,导致类的个数增加,系统结构过于复杂
*/

#include <thread>
#include <memory>
#include <iostream>
#include <vector>

using namespace std;

class View{
public:
	virtual void show() {}
};

class HomeView:public View{
public:
	virtual void show() {
		std::cout<<" HomeView show() \n";
	}
};

class StudentView:public View{
	virtual void show() {
		std::cout<<" StudentView show() \n";
	}
};

// 调度器
class Dispatcher{
public:
	Dispatcher() {
		home_view = new HomeView();
		student_view = new StudentView();
	}

public:
	void dispatch(string request) {
		if(request == "home")
			home_view->show();
		else if(request == "student")
			student_view->show();
	}

private:
	View* home_view;
	View* student_view;
};


// 前端控制器
class FrontController{
public:
	FrontController() {
		dispatcher = new Dispatcher();
	}
public:
	void dispatchRequest(string request) {
		// 记录每一个请求
		trackRequest(request);
		// 身份验证
		if(isAuthenticUser()){
			dispatcher->dispatch(request);
		}
	}
private:
	bool isAuthenticUser() {
		return true;
	}
	void trackRequest(string request) {
		std::cout<<" TRACE: request= "<<request<<"\n";
	}

private:
	Dispatcher* dispatcher;
};


// 主函数入口
int main(int argc, char** argv) {

    FrontController* front = new FrontController();

    front->dispatchRequest("home");

    front->dispatchRequest("student");

}

