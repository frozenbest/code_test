/*
* 分割平衡字符串
*/
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <stdlib.h>
#include <sys/timeb.h>
#include <algorithm>

using namespace std;

#define PI 3.1415926535

//struct timeb
//  {
//   time_t time;        /* Seconds since epoch, as from `time'.  */
//   unsigned short int millitm; /* Additional milliseconds.  */
//   short int timezone;     /* Minutes west of GMT.  */
//   short int dstflag;      /* Nonzero if Daylight Savings Time used.  */
// };
// /* Fill in TIMEBUF with information about the current time.  */
// extern int ftime (struct timeb *__timebuf);

long long int getsystime()
{
    struct timeb tb;
    ftime(&tb);
    return tb.time*1000+tb.millitm;
}

int SplitVector(const string &str, char sep, vector<int>& array)
{
    vector<string> config_list;
    size_t pos = 0;
    size_t pre_pos = 0;
    do{
        pos = str.find_first_of(sep, pre_pos);
        config_list.push_back(str.substr(pre_pos, pos-pre_pos));
        pre_pos = pos + 1;
    }while(string::npos != pos);
    for(size_t i = 0; i < config_list.size(); ++i)
    {
        int value = atoi( config_list[i].c_str() );
        array.push_back(value);
        std::cout<<" value= "<<value<<"\n";
    }
    std::cout<<" finish. "<<"\n";
    return 0;
}

int printVector(const vector<int>& array)
{
    for(size_t i = 0; i < array.size(); ++i)
    {
        std::cout<<__FUNCTION__<<__LINE__<<" value: "<<array[i]<<"\n";
    }

    std::cout<<__FUNCTION__<<__LINE__<<" size: "<<array.size()<<"\n";
}


vector<int> smalllestK(vector<int>& array, int k)
{
    vector<int> out_array;
    if(k >= array.size())
    {
        out_array = array;
    }
    else
    {
        std::sort(array.begin(), array.end());
        for(size_t i = 0; i < k; ++i)
        {
            out_array.push_back(array[i]);
        }
    }

    return out_array;
}

/* 二分 */
int middle_search(vector<int>& nums, int target)
{
    int low = 0;
    int high = nums.size() - 1;
    int middle = 0;
    while(low <= high) {
        middle = (high - low)/2 + low;
        int num = nums[middle];
        if(num == target)
        {
            return middle;
        }
        else if( num > target)
        {
            high = middle - 1;
        }
        else if( num < target)
        {
            low = middle + 1;
        }

    }

    return -1;
}



// 输入 "RLRRLLRLRL"
// 输出 4
// 解释：s 可以分割为 "RL"、"RRLL"、"RL"、"RL" ，每个子字符串中都包含相同数量的 'L' 和 'R' 。

int balancedStringSplit(string s) {
    int low = 0;
    int high = s.size()-1;
    int result = 0;
    while(low <= high) {
        int l_num = 0, r_num = 0;
        for(int i = low; i <= high; i++)
        {
            if(s[i] == 'L')
            {
                l_num++;
            }
            if(s[i] == 'R')
            {
                r_num++;
            }
            if(l_num == r_num && 0 != l_num)
            {
                result++;
                low = i+1;
                break;
            }
        }
    }

    return result;
}



// 输入 最多选择项目个数  k = 2, 资本 w = 0, 纯利润 profits = {1,2,3},  所需资本capital = {0,1,1}
// 输出 4 可获得最多资本
// 注意 选择k个不同的项目

int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
    int n = profits.size();
    if(k > n)
        k = n;

    int curr = 0; // 游标
    priority_queue<int,vector<int>, less<int> > pq; // 大顶堆
    vector<pair<int, int> > choose; // 全部项目
    for(int i = 0; i < n; ++i)
    {
        choose.push_back({capital[i], profits[i]} );
    }
    sort(choose.begin(), choose.end()); // 按成本 升序排列
    
    for(int s = 0; s < k; ++s) // 选择k个项目
    {
        while(curr < n && choose[curr].first <= w) {
            pq.push(choose[curr].second); // 纯利润 插入队尾并排序
            curr++; // 确保只遍历一次
        }

        if(!pq.empty()) {
            w += pq.top();
            pq.pop(); // 确保同一项目只会选择一次
        } else {
            break;
        }
    }
    return w; // 返回总资产
}


int main(int argc, char* argv[])
{
    int k = 10;
    int w = 0;
    vector<int> profits = {1,2,3};
    vector<int> capital = {0,1,2};
    
    std::cout<<" ======================"<<"\n"<<"\n";

    int result = findMaximizedCapital(k, w, profits, capital);

    std::cout<<" result: "<<result<<"\n";

}

/*
*  g++ splitbalancestring.cpp -o splitbalancestring -std=c++11
*/

