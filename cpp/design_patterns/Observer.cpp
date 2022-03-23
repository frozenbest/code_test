/*
* Observer 观察者模式
* 对象间存在一对多关系时 使用观察者模式 一个对象被修改，会自动通知依赖塔的对象. 行为型模式
* 使用场景: 一个目标对象变化,所有观察者都将得到广播通知
* 优点: 1.观察者和被观察者是抽象耦合的 2.建立一套触发机制
* 缺点: 1.观察者太多,通知所有观察者耗时太长 2.观察者不知道被观察目标怎么发生变化,只知道目标发生变化
*/
/* 前置声明
* 前置声明的类 不能定义对象   可以定义指向这个类型的指针和引用 
* 前置声明的类是不完全的类 不能操作成员或成员函数
*/

#include <thread>
#include <memory>
#include <iostream>
#include <vector>

using namespace std;

class Observer; // 前置声明 
class Subject{ // 被观察者
public:
    Subject();
    ~Subject();
public:
	inline int getState() {
		return state;
	}
	void setState(int state);
	void attach(Observer* observer);
	void notifyAllObservers();

private:
	std::vector<Observer*> observers;
	int                    state;
};

class Observer{ // 观察者
public:
	virtual void update() {
		std::cout<<" Observer::update() \n";
	}

protected:
	Subject* subject;
};

class BinaryObserver:public Observer{
public:
	BinaryObserver(Subject* s) {
		this->subject = s;
		this->subject->attach(this);
		std::cout<<" BinaryObserver::BinaryObserver "<<"\n";
	}
	~BinaryObserver() {
		std::cout<<" BinaryObserver::~BinaryObserver "<<"\n";
	}
public:
	void update() {
		std::cout<<" BinaryObserver::update()  state: "<<subject->getState()<<" \n ";
	}

};

class OctalObserver:public Observer{
public:
	OctalObserver(Subject* s) {
		this->subject = s;
		this->subject->attach(this);
		std::cout<<" OctalObserver::OctalObserver "<<"\n";
	}
	~OctalObserver() {
		std::cout<<" OctalObserver::~OctalObserver "<<"\n";
	}
public:
	void update() {
		std::cout<<" OctalObserver::update()  state: "<<subject->getState()<<" \n ";
	}

};

class HexaObserver:public Observer{
public:
	HexaObserver(Subject* s) {
		this->subject = s;
		this->subject->attach(this);
		std::cout<<" HexaObserver::HexaObserver "<<"\n";
	}
	~HexaObserver() {
		std::cout<<" HexaObserver::~HexaObserver "<<"\n";
	}
public:
	void update() {
		std::cout<<" HexaObserver::update()  state: "<<subject->getState()<<" \n ";
	}

};


Subject::Subject() {
	std::cout<<" Subject::Subject "<<"\n";
	observers.clear();
	state = 0;
}
Subject::~Subject() {
	std::cout<<" Subject::~Subject "<<"\n";
	for(auto it = observers.begin(); it != observers.end(); )
	{
		Observer* obp = *it;
		if(obp) {
			delete obp;
			obp = NULL;
		}
		it = observers.erase(it);
	}
}

void Subject::setState(int state) {
	this->state = state;
	notifyAllObservers();
}
void Subject::attach(Observer* observer) {
	observers.push_back(observer);
}
void Subject::notifyAllObservers() {
	for(auto it = observers.begin(); it != observers.end(); ++it)
	{
		(*it)->update();
	}
}


// 主函数入口
int main(int argc, char* argv[]) {

    Subject subject;
    BinaryObserver* bop = new BinaryObserver(&subject);
    OctalObserver*  oop = new OctalObserver(&subject);
    HexaObserver*   hop = new HexaObserver(&subject);

    std::cout<<" First state change: 15 "<<"\n";
    subject.setState(15);
    std::cout<<" Second state change: 10 "<<"\n";
    subject.setState(10);


}

