/*
* Builder 建造者模式
* 使用多个简单的对象 一步一步构建成一个复杂的对象
* 使用场景: 一些基本部件不会变,组合经常变化
* 优点: 1.建造者独立、易扩展 2. 便于控制细节风险
* 缺点: 1.产品必须有共同点、范围有限制 2.如内部变化复杂，会有很多建造类
*/

#include <thread>
#include <memory>
#include <iostream>
#include <vector>

using namespace std;

/* 接口类 */
// 包装
class Packing{
public:
	virtual std::string pack(void) = 0;
};

// 纸盒
class Wrapper:public Packing // 继承接口类
{
public:
	std::string pack(void) {
		// std::cout<<" Wrapper::pack() "<<"\n";
		return "Wrapper";
	}
};

// 瓶子
class Bottle:public Packing // 继承接口类
{
public:
	std::string pack(void) {
		// std::cout<<" Bottle::pack() "<<"\n";
		return "Bottle";
	}
};


/* 抽象类 */
// ITEM 基类
class Item
{
public:
	Item() {}
	virtual ~Item() {}
public:
	virtual std::string name() = 0;
    virtual Packing* packing() = 0;
    virtual double price() = 0;
};

typedef std::shared_ptr<Item> auto_item_ptr;

// 抽象类
class Burger:public Item
{
public:
	Burger() {}
	virtual ~Burger() {}
public:
	virtual std::string name() = 0;
	Packing* packing() {
		return new Wrapper();
	}
	virtual double price() = 0;

};

// 抽象类
class ColdDrink:public Item
{
public:
	ColdDrink() {}
	virtual ~ColdDrink() {}
public:
	virtual std::string name() = 0;
	Packing* packing() {
		return new Bottle();
	}
	virtual double price() = 0;
};


/* 实体类 */
// 鸡肉汉堡
class ChickenBurger:public Burger
{
public:
	ChickenBurger() {}
	~ChickenBurger() {}
public:
	std::string name() {
		return "ChickenBurger";
	}
	double price() {
		return 20.5;
	}
};

// 可乐
class Coke:public ColdDrink
{
public:
	Coke() {}
	~Coke() {}
public:
	std::string name() {
		return "Coke";
	}
	double price() {
		return 9.5;
	}
};

// 套餐
class Meal {
public:
	Meal() {}
	~Meal() {}
public:
	// 添加 
	void addItem(auto_item_ptr item) {
		items_vec.push_back(item);
	}
	// 价格
	double getCost() {
		double cost = 0;
		for(std::vector<auto_item_ptr>::iterator it = items_vec.begin(); it != items_vec.end(); ++it)
		{
			cost += (*it)->price();
		}
		return cost;
	}
	// 
	void showItems() {
		std::string detail;
		for(std::vector<auto_item_ptr>::iterator it = items_vec.begin(); it != items_vec.end(); ++it)
		{
			std::cout<<" Item: "<<(*it)->name()<<"\n";
            std::cout<<" Packing: "<<(*it)->packing()->pack()<<"\n";
            std::cout<<" Price: "<<(*it)->price()<<"\n";
            std::cout<<"\n";
		}
	}

private:
	std::vector<auto_item_ptr> items_vec;
};

class MealBuilder {
// 使用默认 构造 析构函数
public:
    // 可以再来个蔬菜套餐
    // Meal* prepareVegMeal()

	// 无蔬菜套餐
	Meal* prepareNonVegMeal() {
		Meal* meal = new Meal();
		meal->addItem( std::shared_ptr<Item>( new ChickenBurger() ) );
		meal->addItem( std::shared_ptr<Item>( new Coke() ) );
		std::cout<<" tmp meal "<<meal<<"\n";
		return meal;
	}
};

// 主函数入口
int main() {
	MealBuilder meal_builder;

	Meal*  nonveg_meal = meal_builder.prepareNonVegMeal();
	std::cout<<" nonveg_meal "<<nonveg_meal<<"\n";
	nonveg_meal->showItems();

}

