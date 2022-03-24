
// redis zset 实现
#ifndef __ZSET__H__
#define __ZSET__H__


#include <map>
#include "zskiplist.h"

template<class T>
class CZset
{
private:
    ZSkipList<T> skpl; // 跳跃表存储
    std::map<T, long long int> MemScoreMap; // T 如果是自定义结构 需要重载 < 操作符
public:
    zAdd(const T& mem, const long long int & score)
    {
        
    }

}



#endif // __ZSET_H__