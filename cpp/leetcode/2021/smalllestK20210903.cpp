/*
* 找出数组中最小的k个数  以任意顺序返回K个数
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


int main(int argc, char* argv[])
{
    vector<int> in_array = {1,3,5,7,2,4,6,8};
    int k = 4;

    printVector(in_array);
    std::cout<<" before array. k= "<<k<<"\n";

    vector<int> out_array = smalllestK(in_array,k);

    std::cout<<" result: "<<"\n";
    printVector(out_array);

}


// 1 120:150   4 30:60
// 2 210:240   3 300:330

/*
*  g++ math.cpp -o math -std=c++11
*  ./math 120
*  1 4
*  2 3
*/

