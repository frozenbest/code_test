/*
* Iterator 迭代器模式
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

template<class T>
class Iterator{
public:
	virtual bool hasNext() {}
	virtual T next() {}
};



// 
template<class Iterator>
struct iterator_traits
{
	typedef typename Iterator::difference_type    difference_type;
	typedef typename Iterator::value_type         value_type;
	typedef typename Iterator::pointer            pointer; // 指针
	typedef typename Iterator::reference          reference; // 引用
	typedef typename Iterator::iterator_category  iterator_category;
};


//
template<class T>
struct iterator_traits<T*> // 特化模板 
{
	typedef ptrdiff_t difference_type; // ptrdiff_t 保存两个指针减法操作的结果
	typedef T         value_type;
	typedef T*        pointer;
	typedef T&        reference;
	typedef random_access_iterator_tag iterator_category;
};


// 主函数入口
int main(int argc, char* argv[]) {

	std::cout<<" nonveg_meal "<<nonveg_meal<<"\n";


}

