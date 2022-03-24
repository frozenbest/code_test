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

string blank(int n) {
    return string(n, ' ');
}

string point2str(const vector<int>& point) {
    char str[128];
    snprintf(str, 127, "(%d,%d)", point[0], point[1]);
    return str; 
}

// 字典数
struct TrieNode{
    bool is_end;
    unordered_map<char, TrieNode*> children;
    TrieNode() {
        is_end = false;
        children.clear();
    }
};

void insert(TrieNode* root,const string& word) {
    TrieNode* node = root;
    for(char c : word) {
        if(!node->children.count(c)) {
            node->children[c] = new TrieNode();
        }
        node = node->children[c];
    }
    node->is_end = true; // word 尾结点
}

class WordDictionary{
public:
    WordDictionary() {
        trie = new TrieNode();
    }
    ~WordDictionary() {}

    void addWord(string word) {
        insert(trie, word);
    }

    bool search(string word) {
        return dfs(word, 0, trie);
    }

    bool dfs(const string& word, int index, TrieNode* node) {
        if(index == word.size() ) { // 最多递归word的size次
            return node->is_end;
        }

        char c = word[index];
        if(c != '.') {
            if(node->children.count(c) > 0 && dfs(word, index+1, node->children[c]) ) {
                return true;
            }
        } else {
            for(int i = 0; i < 26; i++) {
                c = 'a' + i;
                if(node->children.count(c) > 0 && dfs(word, index+1, node->children[c]) ) {
                    return true;
                }
            }
        }

        return false;
    }
private:
    TrieNode* trie;
};



int main(int argc, char* argv[])
{
    std::cout<<" ======================"<<"\n"<<"\n";

    // string num = argv[1];
    // int target = atoi(argv[2]);

    // std::cout<<" num= "<<num<<" target= "<<target<<"\n";

    // vector<string> ans = addOperators(num, target);

    // printVector(ans);
    char a = 'a';
    char b = 'a' + 1;
    std::cout<<" a= "<<a<<" b= "<<b<<"\n";

}

/*
*  g++ getSum20210926.cpp -o getSum20210926 -std=c++0x
*/

