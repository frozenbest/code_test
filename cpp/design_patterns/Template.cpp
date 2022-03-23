/*
* Template 模板模式
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

class Game{
public:
	// 初始化游戏
	virtual void initialize() {}
	// 开始游戏
	virtual void startPlay() {}
	// 结束游戏
	virtual void endPlay() {}

public:
	virtual void play() final {

		initialize();

		startPlay();

		endPlay();
	}

};

class Cricket:public Game{
public:
	virtual void initialize() {
		std::cout<<" Cricket Game Finished! "<<"\n";
	}
	virtual void startPlay() {
		std::cout<<" Cricket Game Initialized! Start playing. "<<"\n";
	}
	virtual void endPlay() {
		std::cout<<" Cricket Game Started. Enjoy the game! "<<"\n";
	}
};

class Football:public Game{
public:
	virtual void initialize() {
		std::cout<<" Football Game Finished! "<<"\n";
	}
	virtual void startPlay() {
		std::cout<<" Football Game Initialized! Start playing. "<<"\n";
	}
	virtual void endPlay() {
		std::cout<<" Football Game Started. Enjoy the game! "<<"\n";
	}
};



// 主函数入口
int main(int argc, char** argv) {

	Game* game = new Cricket();
	game->play();
	
	game = new Football();
	game->play();

}

