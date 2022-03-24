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

int printVector(const vector<string>& array)
{
    for(size_t i = 0; i < array.size(); ++i)
    {
        std::cout<<__FUNCTION__<<__LINE__<<" value: "<<array[i]<<"\n";
    }

    std::cout<<__FUNCTION__<<__LINE__<<" size: "<<array.size()<<"\n";
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

vector<string> fullJustify(vector<string>& words, int maxWidth) {
    vector<string> ans; // 返回值
    int n = words.size();
    int right = 0;// 左右双游标 找区间
    while(true) {
        int len = 0;
        int left = right;
        // 找出 不超过 maxWidth 最多单词  注意单词之间至少有一个空格
        while(right < n && len + words[right].size() + right - left <= maxWidth) {
            len += words[right++].size(); // 后置++
        }
        // 最后一行 左对齐
        if(right == n) {
            string s = join(words, left, n, " ");
            ans.emplace_back( s + blank(maxWidth-s.size()) ); // 末尾用空格填充
            return ans;
        }

        int words_count = right - left; // 区间内单词个数
        int blank_count = maxWidth - len; // 区间内 空格个数
        // 只有一个单词 左对齐
        if(words_count == 1) {
            ans.emplace_back(words[left] + blank(blank_count) );
            continue;
        }

        int avg_blank = blank_count / (words_count-1); // 平均每两个单词之间的空格数
        int extra_blank = blank_count % (words_count-1); // 多出来的 从左到右 平均到每一个单词之前

        string s1 = join(words, left, left+extra_blank+1, blank(avg_blank+1) ); // extra_blank拥有多余空格的单词个数 平均每一个 都加一
        string s2 = join(words, left+extra_blank+1, right, blank(avg_blank) );
        ans.emplace_back(s1 + blank(avg_blank) + s2);
    }
}



int main(int argc, char* argv[])
{
    vector<string> words = {"This", "is", "an", "example", "of", "text", "justification."};
    int maxWidth = 16;
    printVector(words);
    std::cout<<" maxWidth= "<<maxWidth<<"\n";
    std::cout<<" ======================"<<"\n"<<"\n";
    vector<string> result = fullJustify(words, maxWidth);

    printVector(result);

}

/*
*  g++ splitbalancestring.cpp -o splitbalancestring -std=c++11
*/

