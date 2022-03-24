
#include <iostream>
#include <stdlib.h>
// 双向链表 实现
#ifndef __LINK__H__
#define __LINK__H__


typedef struct Node{
	Node* prev;
	int   data;
	Node* next;
}Node;

class Link{
public:
	Link(int t); // 构造
	~Link(); // 析构

public:
	// 添加
	int add(int data, int index);
    // 删除
	int del(int data);

    void show();

private:
	Node* head;
};


Link::Link(int t)
{
#ifdef __TEST_CODE__
    std::cout<<" ~Link构造~ \n";
#endif
    head = (Node*)malloc(sizeof(Node));
    head->data = t;
    head->prev = 0;
    head->next = 0;

}

Link::~Link()
{
#ifdef __TEST_CODE__
	std::cout<<" ~Link析构~ \n";
#endif
}


int Link::add(const int data, int index)
{

}


int Link::del(const int data)
{

}

void Link::show()
{
	Node* temp = head;
	while(temp)
	{
		if(temp->next == 0)
			std::cout<<temp->data<<"\n";
		else
			std::cout<<"->"<<temp->data;
		temp = temp->next;
	}
}


#endif // __LINK_H__



int main(int argc, char** argv)
{
	Link* link = new Link(2);
	link->show();
}