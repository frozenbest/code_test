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
#include <numeric>
#include <unordered_map>
#include <string.h>
#include <tuple>
#include <bitset>
#include <ctype.h> // isupper 是否为大写英文字母 islower 是否为小写英文字母
#include <algorithm> // all_of any_of none_of 

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

int printVector(const vector<string>& array)
{
    for(size_t i = 0; i < array.size(); ++i)
    {
        std::cout<<__FUNCTION__<<__LINE__<<" value: "<<array[i]<<"\n";
    }

    std::cout<<__FUNCTION__<<__LINE__<<" size: "<<array.size()<<"\n";
}



// 文本左右对齐
// 输入 words = {"This", "is", "an", "example", "of", "text", "justification."}            maxWidth = 16
// 输出
// {
//     "This    is    an",
//     "example  of text",
//     "justification.  "
// }

string blank(int n) {
    return string(n, ' ');
}

// [left,right) 之间 左闭右开 用 sep 填充, 并返回结果
string join(vector<string>& words, int left, int right, string sep) {
    string s = words[left];
    for(int i = left + 1; i < right; ++i) {
        s += sep + words[i];
    }
    return s;
}

string point2str(const vector<int>& point) {
    char str[128];
    snprintf(str, 127, "(%d,%d)", point[0], point[1]);
    return str; 
}

int distance(const vector<int>& point1, const vector<int>& point2) {
    double dis_x = abs(point1[0] - point2[0]);
    double dis_y = abs(point1[1] - point2[1]);
    double dis = pow(dis_x, 2) + pow(dis_y,2);
    std::cout<<" point1: "<<point2str(point1)<<" point2: "<<point2str(point2)<<" dis: "<<dis<<"\n";
    return dis;
}



int dis[4][2] = {{1,0},{-1,0},{0,1},{0,-1}}; // 上下左右 四个方向


void bit_set_test()
{
    bitset<4> set1;
    std::cout<<" set1 = "<<set1<<"\n";

    string s = "101001";
    bitset<8> set2(s);
    std::cout<<" set2 = "<<set2<<"\n";
    std::cout<<" set2 count = "<<set2.count()<<" size= "<<set2.size()<<" set[2]= "<<set2[2]<<"\n";
    set2.set(2);
    std::cout<<" set2 = "<<set2<<" set[2]= "<<set2[2]<<"\n";
    s = set2.to_string(); // bitset转string
    unsigned int set2_value = set2.to_ulong(); // bitset 转 unsigned long类型 
    std::cout<<" set2 to string s= "<<s<<" set2_value= "<<set2_value<<"\n";

    int v = 5;
    bitset<8> set3(5);
    std::cout<<" set3 = "<<set3<<"\n";


}

void all_of_test()
{    
    string s1 = "ABCDEFG";
    string s2 = "Abcdefg";

    bool ans1 = std::all_of(std::begin(s1), std::end(s1), ::isupper);
    std::cout<<" all_of s1= "<<s1<<" ans1= "<<ans1<<"\n";

    bool ans2 = std::all_of(std::begin(s2), std::end(s2), ::isupper);
    std::cout<<" all_of s2= "<<s2<<" ans2= "<<ans2<<"\n";

    bool ans3 = std::any_of(std::begin(s2), std::end(s2), ::islower);
    std::cout<<" any_of s2= "<<s2<<" ans3= "<<ans3<<"\n";

    bool ans4 = std::none_of(std::begin(s1), std::end(s1), ::islower);
    std::cout<<" none_of s1= "<<s1<<" ans4= "<<ans4<<"\n";

    string s3 = "AABCDEFGA";
    char the_char = 'A';
    int ans5 = std::count(std::begin(s3), std::end(s3), the_char);
    std::cout<<" count s3= "<<s3<<" the_char= "<<the_char<<" ans5= "<<ans5<<"\n"; // 返回等于the_char的个数

    char max_char{'B'};
    int ans6 = std::count_if(std::begin(s3), std::end(s3), [max_char] (int ch) {return ch > max_char;} ); // 返回大于max_char的个数
    std::cout<<" count_if s3= "<<s3<<" max_char= "<<max_char<<" ans6= "<<ans6<<"\n";


}



int main(int argc, char* argv[])
{
    std::cout<<" ======================"<<"\n"<<"\n";

    // bit_set_test();
    all_of_test();

}

/*
*  g++ getSum20210926.cpp -o getSum20210926 -std=c++0x
*/

