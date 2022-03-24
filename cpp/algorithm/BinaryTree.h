// 二叉树实现
#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__


#include <iostream>
#include <vector>
#include <stack> // size() empty() top() pop() push()
#include <boost/lexical_cast.hpp>

using namespace std;

template<class T>
class BinTNode
{
public:
    /* 构造 */
    BinTNode(T value, BinTNode* p, BinTNode* l, BinTNode* r):key(value),parent(p),left(l),right(r) {}
    /* 析构 */
    ~ BinTNode() {}

public:
    T        key;    // 键值
    BinTNode* parent; // 父节点
    BinTNode* left;   // 左孩子
    BinTNode* right;  // 又孩子
};


template<class T>
class BinaryTree
{
public:
    BinaryTree();
    ~BinaryTree();

public:
    // 将结点(key为节点键值)插入到红黑树中
    void insert(T value);

    void remove(T key);

private:
    // 左旋
    void leftRotate(BinTNode<T>* &root, BinTNode<T>* x);
    // 右旋
    void rightRotate(BinTNode<T>* &root, BinTNode<T>* y);


    BinTNode<T>* search(BinTNode<T>* root, T key) const;
    // 插入函数
    void insert(BinTNode<T>* &root, BinTNode<T>* node); // 这里根结点使用引用 因为有可能修改根结点的值
    // 插入修正函数
    void insertFixUp(BinTNode<T>* &root, BinTNode<T>* node);

    void remove(BinTNode<T>* &root, BinTNode<T>* node);

    // 删除修正函数
    void removeFixUp(BinTNode<T>* &root, BinTNode<T>* node, RBTNoBinTNodede<T>* parent);

private:
    void preOrder(BinTNode<T>* tree) const;

    void inOrder(BinTNode<T>* tree) const;

    void postOrder(BinTNode<T>* tree) const;

    // 非递归实现 前序遍历  注意防止爆栈
    void iterativePreOrder(BinTNode<T>* node);

    // 非递归实现 中序遍历
    void iterativeInOrder(BinTNode<T>* node); 

    // 非递归实现 后序遍历
    void iterativePostOrder(BinTNode<T>* node);

    // 非递归实现
    BinTNode<T>* iterativeSearch(BinTNode<T>* node, T key);

private:
    BinTNode<T>* root; // 根结点

// 宏定义函数 
#define rb_parent(r)\
    (r)->parent
#define rb_set_parent(r, p)\
    (r)->parent = (p)
#define rb_color(r)\
    (r)->color
#define rb_is_red(r)\
    (r)->color == RED
#define rb_is_black(r)\
    (r)->color == BLACK
#define rb_set_red(r)\
    (r)->color = RED
#define rb_set_black(r)\
    (r)->color = BLACK

};




// vec打印函数
template<class T>
static std::string vector_dump(std::vector<T>& vec)
{
    std::string info;
    for(typename std::vector<T>::iterator it = vec.begin(); it != vec.end(); ++vec)
    {
        info += boost::lexical_cast<std::string>(*it) + ",";
    }
    return info;
}

#endif //__RB_TREE_H__

// https://www.cnblogs.com/alantu2018/p/8462017.html
// https://blog.csdn.net/li1914309758/article/details/80997342