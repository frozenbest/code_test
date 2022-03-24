// 跳跃表实现

/* 跳跃表组成
* 1. 表头（head）：负责维护跳跃表的节点指针。
* 2. 跳跃表节点：保存着元素值，以及多个层。
* 3. 层：保存着指向其他元素的指针。高层的指针越过的元素数量大于等于低层的指针，为了提高查找的效率，程序总是从高层先开始访问，然后随着元素值范围的缩小，慢慢降低层次。
* 4. 表尾：全部由 NULL 组成，表示跳跃表的末尾。
*/
#include <iostream>

#ifndef __ZSKIP_LIST_H__
#define __ZSKIP_LIST_H__

namespace zsl_ns{

// #define NULL        (0)

#define ZSKIPLIST_P 0.25     // 随机层数概率
#define AUTO_SAFE_DELETE(p) if(p) {delete p; p=NULL;} else {p = NULL;}

static const int MAX_LEVEL = 32; // 头节点默认层数为32  跳跃层数初始化为1

template<class T>
struct ZSkipListNode {
    T        member; // member 对象
    double   score; // 分值
    struct   ZSkipListNode* backward; // 后退指针
    // 层
    typedef struct ZSkipListLevel {
        struct ZSkipListNode* forward; // 前进指针
        unsigned int span; // 这个层跨越的节点数量
    public:
        ZSkipListLevel() {
            forward = NULL;
            span = 0;
        }
        ~ZSkipListLevel() {
            AUTO_SAFE_DELETE(forward);
            span = 0;
        }
    } ZSkipListLevel;

    ZSkipListLevel level[MAX_LEVEL];

public:
    // 构造函数
    ZSkipListNode() {
        backward = NULL;
        score = 0;
    }
    // 析构函数
    ~ZSkipListNode() {
        score = 0;
        AUTO_SAFE_DELETE(backward);
    }

};

template<class T>
class ZSkipList {
public:
    // 构造函数
    ZSkipList();
    // 析构函数
    ~ZSkipList();

public:
    // 插入节点
    ZSkipListNode<T>* insert(T m, double s);

    // 删除节点
    void zsDeleteNode(ZSkipListNode<T>* x, ZSkipListNode<T>** update);

    bool zsDelete(T m, double s);

    unsigned int zsDeleteRangeByRank(unsigned int start, unsigned int end);

public: // 查询
    // 取指定节点的排名
    unsigned int zsGetRank(T m, double s);
    /* 根据排名 取指定 */
    ZSkipListNode<T>* zsGetElementByRank(unsigned int rank);

    ZSkipListNode<T>* search(T m, double s);

private:
    inline int random_level() const {
        int level = 1;
        while(random()&0xFFFF < ZSKIPLIST_P*0xFFFF)
            level++;
        return level < MAX_LEVEL ?  level : MAX_LEVEL;
    }

private:
    // 头节点, 尾节点
    struct ZSkipListNode<T> *header, *tail;
    // 节点数量
    unsigned long length;
    // 目前表内节点最大层数
    int level;

};

template<class T>
ZSkipList<T>::ZSkipList()
{
    header = new ZSkipListNode<T>();
    tail = NULL;
    length = 0;
    level = 1;
}

template<class T>
ZSkipList<T>::~ZSkipList()
{
    AUTO_SAFE_DELETE(header);
    AUTO_SAFE_DELETE(tail);
    length = 0;
    level = 0;
}

// 插入
template<class T>
ZSkipListNode<T>* ZSkipList<T>::insert(T m, double s)
{
    ZSkipListNode<T>* update[MAX_LEVEL]; // 记录每一层位于插入节点的前一个节点
    unsigned int rank[MAX_LEVEL]; //每一层 位于插入节点的前一个节点的排名

    ZSkipListNode<T>* x = this->header;
    // 最高层开始查找
    for(int i = this->level - 1; i >=0 ; i--)
    {
        rank[i] = i == this->level-1 ? 0: rank[i+1];

        while(x->forward && 
            (x->forward->score < s || (s == x->forward->score && x->forward->member < m ) ) 
            ) 
        {
            rank[i] += x->level[i].span; // 记录跨域多少个节点
            x = x->level[i].forward; // 继续查找下一个节点
        }
        // 每一层位于插入节点的前一个节点
        update[i] = x;
    }

    int level = this->random_level(); // 随机一个层
    // 增加新的层
    if(level > this->level) {
        for(int i = this->level; i < level; i++) {
            rank[i] = 0;
            update[i] = this->header; // 这一层只有头节点
            // 未添加新节点之前 需要更新的节点 跨越的节点数目 length ??
            update[i]->level[i].span = this->length;
        }

        this->level = level;// 更新层数
    }

    // 增加新的节点
    ZSkipListNode<T>* node = new ZSkipListNode<T>;
    node->member = m;
    node->score = s;

    for(int i = 0; i < level; i++) {
        // 跳跃表的每一层 都改变其指向
        node->level[i].forward = update[i]->level[i]->forward;
        update[i]->level[i]->forward = node;
        // 改变span 值
        node->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]) ;
        update[i]->level[i].span = rank[0] - rank[i] + 1;
    }

    // 如果有更高层 更新更高层的span
    for(int i = level; i < this->level; i++)
    {
        update[i]->level[i].span++;
    }

    node->backward = (update[0] == this->header) ? NULL : update[0];

    if(node->level[0].forward)
        node->level[0].forward->backward = node;
    else
        this->tail = node;

    this->length++;
    // 返回新插入的节点
    return node;
}

// 删除节点
template<class T>
void ZSkipList<T>::zsDeleteNode(ZSkipListNode<T>* x, ZSkipListNode<T>** update)
{
    for(int i = 0; i < this->level; ++i)
    {
        if(update[i]->forward == x)
        {
            update[i]->level[i].forward = x->level[i].forward;
            update[i]->level[i].span += x->level[i].span - 1;
        }
        else
        {
            update[i]->level[i].span--;
        }
    }
    if(x->level[0].forward)
    {
        x->level[0].forward->backward = x->backward; //  修改下一个节点的 后置节点
    }
    else
    {
        this->tail = x->backward;
    }
    // 跳跃表的层数处理, 如果表头层级的前项指针为空,说明这一层已经没有元素,层数要减一
    while(this->level > 1 && this->header->level[this->level-1].forward == NULL)
    {
        this->level--;
    }
    length--; // 链表长度 自减一
}

template<class T>
bool ZSkipList<T>::zsDelete(T m, double s)
{
    ZSkipListNode<T>*  update[MAX_LEVEL];
    ZSkipListNode<T>*  x = this->header;
    // 遍历所有层 记录删除节点后需要被修改的节点 到update数组
    for(int i = this->level-1; i >= 0; i--)
    {
        while(x->level[i].forward && (x->level[i].forward->score < s || (x->level[i].forward->score == s && x->level[i].forward->member < m ) ) )
        {
            x = x->level[i].forward;
        }
        update[i] = x;
    }
    // 因为 多个不同的member可能有相同的score 所以要确保 m 和 s 都匹配时 才删除
    x = x->level[0].forward;
    while(x && x->member == m && x->score == s)
    {
        zsDeleteNode(x, update);
        AUTO_SAFE_DELETE(x);
        return true; // 找到指定member 并且删除成功
    }
    return false; // 没找到 删除失败
}

// 删除指定排名范围内的节点
/*
* start 删除的起始 排名
* end   删除的终止 排名
* 返回删除的节点个数
*/
template<class T>
unsigned int ZSkipList<T>::zsDeleteRangeByRank(unsigned int start, unsigned int end)
{
    if(start > end)
        return 0;

    ZSkipListNode<T>* update[MAX_LEVEL];
    ZSkipListNode<T>* x;
    unsigned int traversed = 0, removed = 0;
    x = this->header;

    for(int i = this->level-1; i >= 0; i--)
    {
        while(x->level[i].forward && (traversed + x->level[i].span ) < start )
        {
            x = x->level[i].forward;
            traversed += x->level[i].span;
        }
        update[i] = x;
    }

    // 节点的排名肯定大于等于start
    traversed++;
    x = x->level[0].forward;
    while(x && traversed <= end) {
        ZSkipListNode<T>* next = x->level[0].forward;
        zsDeleteNode(x, update);
        AUTO_SAFE_DELETE(x);
        traversed++;
        removed++;
        x = next;
    }
    return removed;
}

// 取指定节点的排名
template<class T>
unsigned int ZSkipList<T>::zsGetRank(T m, double s)
{
    ZSkipListNode<T>* x;
    unsigned int rank = 0;

    x = this->header;
    for(int i = this->level - 1; i >= 0; i--)
    {
        while(x->level[i].forward && (x->level[i].score < s || (x->level[i].forward->score == s && x->level[i].forward->member < m ) ) )
        {
            x = x->level[i].forward;
            rank += x->level[i].span ;
        }
        if(x->member && x->member == m)
        {
            return rank;
        }
    }
    return 0;
}

/* 根据排名 取指定 */
template<class T>
ZSkipListNode<T>* ZSkipList<T>::zsGetElementByRank(unsigned int rank)
{
    ZSkipListNode<T>* x;
    x = this->header;
    unsigned int traversed = 0;

    for(int i = this->level - 1; i >= 0; i--)
    {
        while(x->level[i].forward && (traversed + x->level[i].span <= rank ) )
        {
            traversed += x->level[i].span;
            x = x->level[i].forward;
        }
        if(traversed == rank)
        {
            return x;
        }
    }
    return NULL;
}

template<class T>
ZSkipListNode<T>* ZSkipList<T>::search(T m, double s)
{
    ZSkipListNode<T>* x;

    x = this->header;
    for(int i = this->level - 1; i >= 0; i--)
    {
        while(x->level[i].forward && (x->level[i].score < s || (x->level[i].forward->score == s && x->level[i].forward->member < m ) ) )
        {
            x = x->level[i].forward;
        }
    }
    x = x->level[0].forward;
    if(x && x->score == s)
    {
        return x;
    }

    return 0;
}


}; // namespace zsl_ns

#endif // __ZSKIP_LIST_H__

/*
int main(){
    zsl_ns::ZSkipList<std::string> zsl;
    std::cout<<"struct size= "<<sizeof(zsl)<<"\n";
}
*/




// https://www.cnblogs.com/thrillerz/p/4505550.html