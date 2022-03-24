// 红黑树实现
#ifndef __RB_TREE_H__
#define __RB_TREE_H__


#include <iostream>
#include <vector>
#include <stack> // size() empty() top() pop() push()
#include <boost/lexical_cast.hpp>

using namespace std;

enum RBTColor{
    RED = 1,
    BLACK,
};

template<class T>
class RBTNode
{
public:
    /* 构造 */
    RBTNode(T value, RBTColor c, RBTNode* p, RBTNode* l, RBTNode* r):key(value),color(c),parent(p),left(l),right(r) {}
    /* 析构 */
    ~ RBTNode() {}

public:
    T        key;    // 键值
    RBTColor color;  // 颜色
    RBTNode* parent; // 父节点
    RBTNode* left;   // 左孩子
    RBTNode* right;  // 又孩子
};


template<class T>
class RBTree
{
public:
    RBTree();
    ~RBTree();

public:
    // 将结点(key为节点键值)插入到红黑树中
    void insert(T value);

    void remove(T key);

private:
    // 左旋
    void leftRotate(RBTNode<T>* &root, RBTNode<T>* x);
    // 右旋
    void rightRotate(RBTNode<T>* &root, RBTNode<T>* y);


    RBTNode<T>* search(RBTNode<T>* root, T key) const;
    // 插入函数
    void insert(RBTNode<T>* &root, RBTNode<T>* node); // 这里根结点使用引用 因为有可能修改根结点的值
    // 插入修正函数
    void insertFixUp(RBTNode<T>* &root, RBTNode<T>* node);

    void remove(RBTNode<T>* &root, RBTNode<T>* node);

    // 删除修正函数
    void removeFixUp(RBTNode<T>* &root, RBTNode<T>* node, RBTNode<T>* parent);

private:
    void preOrder(RBTNode<T>* tree) const;

    void inOrder(RBTNode<T>* tree) const;

    void postOrder(RBTNode<T>* tree) const;

    // 非递归实现 前序遍历  注意防止爆栈
    void iterativePreOrder(RBTNode<T>* node);

    // 非递归实现 中序遍历
    void iterativeInOrder(RBTNode<T>* node); 

    // 非递归实现 后序遍历
    void iterativePostOrder(RBTNode<T>* node);

    // 非递归实现
    RBTNode<T>* iterativeSearch(RBTNode<T>* node, T key);

private:
    RBTNode<T>* root; // 根结点

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

// 左旋
template<class T>
void RBTree<T>::leftRotate(RBTNode<T>* &root, RBTNode<T>* x)
{
    // 设置 y 为x的右节点
    RBTNode<T>* y = x->right;
    // 修改 x的 右节点
    x->right = y->left;
    if(y->left != NULL)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == NULL) // 如果x是根结点
    {
        root = y; 
    }
    else
    {
        if(x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// 右旋
template<class T>
void RBTree<T>::rightRotate(RBTNode<T>* &root, RBTNode<T>* y)
{
    // 设置y 为 x 的 左节点
    RBTNode<T>* x = y->left;
    y->left = x->right;
    if(x->right != NULL)
    {
        x->right->parent = y;
    }
    // 将 “y的父亲” 设为 “x的父亲”
    x->parent = y->parent;
    if(y->parent == NULL) // y是根结点
    {
        root = x;
    }
    else
    {
        if(y->parent->left == y)
            y->parent->left = x; // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
        else
            y->parent->right = x;
    }
    // 将 “y” 设为 “x的右孩子”
    x->right = y;
    // 将 “y的父节点” 设为 “x”
    y->parent = x;

}


// 插入  insert(key)的作用是将"key"添加到红黑树中
template<class T>
void RBTree<T>::insert(T value)
{
    RBTNode<T>* new_node = NULL; 
    new_node = new RBTNode<T>(value, RED, NULL, NULL, NULL);
    if(new_node == NULL)
        return;
    insert(root, new_node);

}

/*
*红黑树的特性：
*(1) 每个节点或者是黑色，或者是红色。
*(2) 根节点是黑色。
*(3) 每个叶子节点是黑色。 [注意：这里叶子节点，是指为空的叶子节点！]
*(4) 如果一个节点是红色的，则它的子节点必须是黑色的。
*(5) 从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。
*/
// 插入 insert(root, node)的作用是将"node"节点插入到红黑树中。其中，root是根，node是被插入节点
template<class T>
void RBTree<T>::insert(RBTNode<T>* &root, RBTNode<T>* node)
{
    RBTNode<T>* y = NULL; // 最终的父节点
    RBTNode<T>* x = root;

    // 1. 将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
    if(x != NULL)
    {
        y = x;
        if(node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    // 父节点
    node->parent = y;
    if(y != NULL)
    {
        if(node->key < y->key)
            y->left = node;
        else
            y->right = node;
    }
    else // 空的红黑树
    {
        root = node;
    }
    // 2. 设置结点颜色为红色
    rb_set_red(node);

    // 3.重新修正为一棵 二叉查找树
    insertFixUp(root, node);
}

// 插入修正函数
template<class T>
void RBTree<T>::insertFixUp(RBTNode<T>* &root, RBTNode<T>* node)
{
/*
 * 红黑树插入修正函数
 *
 * 在向红黑树中插入节点之后(失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 插入的结点        // 对应《算法导论》中的z
 */
    RBTNode<T> *parent, *gparent;

    // 若“父节点存在，并且父节点的颜色是红色”
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        //若“父节点”是“祖父节点的左孩子”
        if (parent == gparent->left)
        {
            // Case 1条件：叔叔节点是红色
            {
                RBTNode<T> *uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点是右孩子
            if (parent->right == node)
            {
                RBTNode<T> *tmp;
                leftRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是左孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rightRotate(root, gparent);
        } 
        else//若“z的父节点”是“z的祖父节点的右孩子”
        {
            // Case 1条件：叔叔节点是红色
            {
                RBTNode<T> *uncle = gparent->left;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点是左孩子
            if (parent->left == node)
            {
                RBTNode<T> *tmp;
                rightRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是右孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            leftRotate(root, gparent);
        }
    }

    // 将根节点设为黑色
    rb_set_black(root);
}

// 删除结点
template<class T>
void RBTree<T>::remove(T key)
{
    RBTNode<T>* node = NULL;
    node = search(root, key);
    if(node != NULL)
        remove(root, node);
}

// 删除结点
template<class T>
void RBTree<T>::remove(RBTNode<T>* &root, RBTNode<T>* node)
{
    RBTNode<T>* child, parent;
    RBTColor color;

    // 1.被删除的所有节点都不为空
    if(node->left != NULL && node->right != NULL)
    {
        RBTNode<T>* replace = node;
        // 寻找替代节点
        replace = node->right;
        if(replace->left != NULL)
            replace = replace->left;

        // 修改父节点到替代节点
        if(rb_parent(node))
        {
            if(rb_parent(node)->left == node )
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        }
        else // node是父节点
            root = node;

        // 修改子节点到替代节点
        child = replace->right;
        parent = rb_parent(replace);
        color = replace->color;
        if(parent == node)
        {
            parent = replace;
        }
        else
        {
            if(child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->parent, replace);

        }

        // 修改替代节点
        replace->parent = node->parent;
        replace->left = node->left;
        replace->color = node->color;
        node->left->parent = replace;

        if(color == BLACK)
            removeFixUp(root, child, parent);
        delete node;
        return;
    }

    if(node->left != NULL)
        child = node->left;
    else
        child = node->right;
    parent = node->parent;
    color = node->color;
    if(parent)
    {
        if(parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root = child;

    if(color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}

// 删除修正函数
template<class T>
void RBTree<T>::removeFixUp(RBTNode<T>* &root, RBTNode<T>* node, RBTNode<T>* parent)
{

}

template<class T>
RBTNode<T>* RBTree<T>::search(RBTNode<T>* node, T key) const
{
    if(node == NULL || node->key == key)
        return node;
    if( key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

// 非递归实现
template<class T>
RBTNode<T>* RBTree<T>::iterativeSearch(RBTNode<T>* x, T key)
{
    while(x && x->key != key)
    {
        if(key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

// 前序遍历
template<class T>
void RBTree<T>::preOrder(RBTNode<T>* tree) const
{
    std::vector<T> sum_vec;
    if(tree)
    {
        sum_vec.push_back(tree->key);
        preOrder(tree->left);
        preOrder(tree->right);
    }

}

// 中序遍历
template<class T>
void RBTree<T>::inOrder(RBTNode<T>* tree) const
{
    std::vector<T> sum_vec;
    if(tree)
    {
        inOrder(tree->left);
        sum_vec.push_back(tree->key);
        inOrder(tree->right);
    }
}

// 后序遍历
template<class T>
void RBTree<T>::postOrder(RBTNode<T>* tree) const
{
    std::vector<T> sum_vec;
    if(tree)
    {
        postOrder(tree->left);
        postOrder(tree->right);
        sum_vec.push_back(tree->key);
    }

}

// 非递归实现 前序遍历  注意防止爆栈  ulimit -s 8192KB
template<class T>
void RBTree<T>::iterativePreOrder(RBTNode<T>* node)
{
    std::vector<T> sum_vec;
    if(node == NULL)
        return;
    std::stack<RBTNode<T>*> sort_stack;
    sort_stack.push(node);
    while(!sort_stack.empty())
    {
        RBTNode<T>* top = sort_stack.top();
        sum_vec.push_back(top->key);
        sort_stack.pop();
        // 栈 是 先进后出的 所以先把右节点压入栈
        if(top->right != NULL)
            sort_stack.push(top->right);

        if(top->left != NULL)
            sort_stack.push(top->left);
    }

}

// 非递归实现 中序遍历
template<class T>
void RBTree<T>::iterativeInOrder(RBTNode<T>* node)
{
    std::vector<T> sum_vec;
    if(node == NULL)
        return;

    std::stack<RBTNode<T>*> sort_stack;

    if(!sort_stack.empty() || node != NULL)
    {
        sort_stack.push(node);
        node = node->left;
    }
    else
    {
        node = sort_stack.pop();
        sum_vec.push_back(node->key);
        node = node->right;   
    }

}

// 非递归实现 后序遍历
template<class T>
void RBTree<T>::iterativePostOrder(RBTNode<T>* node)
{
    std::vector<T> sum_vec;

    // 后序遍历 使用两个栈实现
    std::stack<RBTNode<T>*> stack_1;
    std::stack<RBTNode<T>*> stack_2; // 保存结果
    stack_1.push(node);
    while(!stack_1.empty()) {
        RBTNode<T>* tmp = stack_1.pop();
        stack_2.push(tmp);

        if(tmp->left != NULL) {
            stack_1.push(tmp->left);
        }
        if(tmp->right != NULL) {
            stack_1.push(tmp->right);
        }
    }

    while(!stack_2.empty()) {
        RBTNode<T>* tmp = stack_2.pop();
        sum_vec.push_back(tmp->key);
    }

}


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