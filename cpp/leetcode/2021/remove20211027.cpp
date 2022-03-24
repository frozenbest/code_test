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

bool contain(string a, string b) {
    int s = 0;
    int n = b.size();
    int m = a.size();
    if(m < n)
        return false;

    for(int i = 0; i < m; ++i) {
        if(a[i] == b[s]) {
            s++;
            if(s == n) {
                return true;
            }
        }
    }
    return false;
}

int dis[4][2] = {{1,0},{-1,0},{0,1},{0,-1}}; // 上下左右 四个方向


// 删除无效的括号
set<string> unique; // 对结果去重

// s: 原字符串 t 取出括号后的 i 索引 l 需要删除的左括号 r 需要删除的后括号 lcnt 保留左括号的数量  rcnt保留右括号的数量
void dfs(const string& s, string& t, int i, int l, int r, int lcnt, int rcnt) {
    // 结束条件
    if(i == s.length()) {
        if(l == 0 && r == 0) unique.insert(t); // 需要删除的左右括号都删除完毕
        return;
    }
    char ch = s[i];

    // 跳过左、右括号(删除括号操作)
    if(ch == '(' && l > 0)
        dfs(s, t, i+1, l-1, r, lcnt, rcnt);
    if(ch == ')' && r > 0)
        dfs(s, t, i+1, l, r-1, lcnt, rcnt);

    // 更新字符串t 入栈
    t.push_back(ch);
    if(ch == '(') {
        dfs(s, t, i+1, l, r, lcnt+1, rcnt);
    } else if(ch == ')') {
        if(rcnt < lcnt) {
            dfs(s, t, i+1, l, r, lcnt, rcnt+1);
        }
        // rcnt >= lcnt时  有括号多 铁定需要删除 剪支
    } else {
        dfs(s, t, i+1, l, r, lcnt, rcnt);
    }
    // 复原 出栈
    t.pop_back();
}

// 301. 删除多余括号  困难
vector<string> removeInvalidParentheses(string s) {
    int l = 0, r = 0, n = s.length(); 
    // 先遍历一遍 找出左右括号需要删除的数量
    for(int i = 0; i < n; ++i) {
        if(s[i] == '(') {
            l++;
        } else if(s[i] == ')') {
            if(l <= 0) {
                r++; // 此时多的右括号 一定是匹配不掉的 需要删除
            } else {
                l--;
            }
        }
    }
    string t = ""; // 临时变量
    dfs(s, t, 0, l, r, 0, 0);

    vector<string> ans;
    for(auto & s : unique) {
        ans.emplace_back(s);
    }
    return ans;
}


int main(int argc, char* argv[])
{
    std::cout<<" ======================"<<"\n"<<"\n";

    string s = argv[1];

    std::cout<<" s= "<<s<<"\n";

    vector<string> ans = removeInvalidParentheses(s);

    printVector(ans);

}

/*
*  g++ getSum20210926.cpp -o getSum20210926 -std=c++0x
*/

