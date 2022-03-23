/*
* 测试 三角函数
*/
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sys/timeb.h>

using namespace std;

#define PI 3.1415926535

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

int compareVersion(string version1, string version2) {
    vector<int> vec1, vec2;
    SplitVector(version1, '.', vec1);
    SplitVector(version2, '.', vec2);

    printVector(vec1);
    printVector(vec2);

    if(vec1.size() < vec2.size())
    {
        for(int i=0; i < vec2.size()-vec1.size(); ++i)
        {
            std::cout<<"FUNCTION "<<__FUNCTION__<<__LINE__<<" i= "<<i<<"\n";
            vec1.push_back(0);
        }
        std::cout<<"FUNCTION "<<__FUNCTION__<<__LINE__<<"\n";
    }
    else if(vec2.size() < vec1.size())
    {
        int n = vec1.size()-vec2.size();
        for(int i=0; i < n; ++i)
        {
            std::cout<<"FUNCTION "<<__FUNCTION__<<__LINE__<<" i= "<<i<<"n= "<<n<<"\n";
            vec2.push_back(0);
        }
        std::cout<<"FUNCTION "<<__FUNCTION__<<__LINE__<<"\n";
    }
    
    printVector(vec1);
    printVector(vec2);

    for(int i = 0; i < vec1.size(); ++i)
    {
        std::cout<<"FUNCTION "<<__FUNCTION__<<__LINE__<<" vec1= "<<vec1[i]<<" vec2= "<<vec2[i]<<"\n";
        if(vec1[i] < vec2[i])
        {
            return -1;
        }
        if(vec2[i] < vec1[i])
        {
            return 1;
        }
    }
    return 0;
}


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

int fortest(int n)
{
    long long int time1 = getsystime();
    for(int i = 0; i < n; ++i)
    {
        std::cout<<"FUNCTION "<<__FUNCTION__<<__LINE__<<" i= "<<i<<"n= "<<n<<"\n";
    }
    long long int time2 = getsystime();
    for(int i = 0; i < n; i++)
    {
        std::cout<<"FUNCTION "<<__FUNCTION__<<__LINE__<<" i= "<<i<<"n= "<<n<<"\n";
    }
    long long int time3 = getsystime();

    std::cout<<" time1= "<<time1<<" time2= "<<time2<<" time3= "<<time3<<"\n";
    std::cout<<" interval1= "<<time2-time1<<" interval2= "<<time3-time2<<"\n";

}


int main(int argc, char* argv[])
{
    fortest(10000);

    string version1 = "1.0.1";
    string version2 = "1";
    // int ret = compareVersion(version1,version2);
    // std::cout<<" result= "<<ret<<"\n";


}


// 1 120:150   4 30:60
// 2 210:240   3 300:330

/*
*  g++ math.cpp -o math -std=c++11
*  ./math 120
*  1 4
*  2 3
*/

