/*
* 升序数组 和 目标值 target    搜索数组中的target,存在返回下标
*/
#include <iostream>
#include <vector>
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


int main(int argc, char* argv[])
{
    vector<int> array = {-1,0,3,5,9,12};
    int target = atoi(argv[1]);

    printVector(array);
    std::cout<<" before search. target= "<<target<<"\n";
    std::cout<<" ======================"<<"\n"<<"\n";

    int result = middle_search(array,target);

    std::cout<<" result: "<<result<<"\n";

}



/*
*  g++ math.cpp -o math -std=c++11
*/

