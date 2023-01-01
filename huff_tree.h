#ifndef HUFF_TREE_H_INCLUDED
#define HUFF_TREE_H_INCLUDED

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include "node.h"

using namespace std;

unsigned char char_idx(const char &ch)
{
    return (unsigned char)ch;
}

template<class char_type, class weight_type>
struct HuffTreeAssist //用于在建树时协助比较两棵树的权重大小
{
    HuffNode<char_type, weight_type> *root;
};

//重载比较运算符
template<class char_type, class weight_type>
bool operator<(const HuffTreeAssist<char_type, weight_type> &first,
               const HuffTreeAssist<char_type, weight_type> &second)
{
    return first.root->get_weight() < second.root->get_weight();
}

template<class char_type, class weight_type>
bool operator>(const HuffTreeAssist<char_type, weight_type> &first,
               const HuffTreeAssist<char_type, weight_type> &second)
{
    return first.root->get_weight() > second.root->get_weight();
}

template<class char_type, class weight_type>
bool operator<=(const HuffTreeAssist<char_type, weight_type> &first,
               const HuffTreeAssist<char_type, weight_type> &second)
{
    return first.root->get_weight() <= second.root->get_weight();
}

template<class char_type, class weight_type>
bool operator>=(const HuffTreeAssist<char_type, weight_type> &first,
               const HuffTreeAssist<char_type, weight_type> &second)
{
    return first.root->get_weight() >= second.root->get_weight();
}

//哈夫曼树类
template<class char_type, class weight_type>
class HuffmanTree
{
public:
    HuffmanTree(char_type ch[], weight_type w[], int n); //由字符序列，对应的权重，字符种类数，字符->编码函数指针构造哈夫曼树
    virtual ~HuffmanTree(); //析构函数
    string encode(char_type ch); //求一个字符的编码
    void decode(unsigned char code, char_type res[], int *cur_pos); //把一串编码转换成字符序列，链表的每一个节点是一个字符

private:
    HuffNode<char_type, weight_type> *root; //树的根节点指针
    string code_arr[256]; //一个 string 的数组，下标是对应字符的ascii码，string 里保存的是这个字符的编码
    HuffNode<char_type, weight_type> *cur_ptr; //译码时从根节点到叶节点路径的当前指针
    int num; //叶节点个数
    //unsigned int (*char_idx)(const char_type &); //字符->编码函数的指针

    void make_code_table(HuffNode<char_type, weight_type> *r, char code[], int len = 0); //辅助函数，填 code_arr
    void clear(HuffNode<char_type, weight_type> *r); //辅助函数，释放以 r 为根的树
};

//先实现辅助函数
template<class char_type, class weight_type>
void HuffmanTree<char_type, weight_type>::clear(HuffNode<char_type, weight_type> *r)
{
    if(r != nullptr)
    {
        HuffNode<char_type, weight_type> *t1 = r->get_left();
        HuffNode<char_type, weight_type> *t2 = r->get_right();
        delete r;
        clear(t1);
        clear(t2);
    }
    else
        return;
}

template<class char_type, class weight_type>
void HuffmanTree<char_type, weight_type>::make_code_table(HuffNode<char_type, weight_type> *r, char code[], int len)
{
    HuffNode<char_type, weight_type> *cur = r;
    if(cur->is_leaf())
    {
        //int a = 0;
        char_type ch = ((HuffLeafNode<char_type, weight_type> *)cur)->get_char(); //获取叶节点的字符
        code[len] = '\0';
        string code_str(code); //类型转换
        //strcpy(code_arr[char_idx(ch)], code); //把编码存入表中
        code_arr[char_idx(ch)] = code_str;
        return;
    }
    else
    {
        code[len] = '0'; //往左走
        make_code_table(r->get_left(), code, len + 1);

        code[len] = '1'; //往右走
        make_code_table(r->get_right(), code, len + 1);
    }
}

//再实现成员函数
template<class char_type, class weight_type>
HuffmanTree<char_type, weight_type>::HuffmanTree(char_type ch[], weight_type w[], int n)
{
    //初始化可以初始化的数据成员
    //char_idx = ch_idx; //函数指针赋值
    num = n;
    //code_arr = new string[num];

    priority_queue<HuffTreeAssist<char_type, weight_type>, vector<HuffTreeAssist<char_type, weight_type> >, greater<HuffTreeAssist<char_type, weight_type> > > min_heap; //这个小根堆里用于存储二叉树，用于构造哈夫曼树
    int cnt = 0;
    for(int i = 0; i < num; i++) //一开始，所有叶子节点都是一棵树，然后插进 min_heap 里
    {
        HuffTreeAssist<char_type, weight_type> tmp;
        if((tmp.root = new HuffLeafNode<char_type, weight_type>(ch[i], w[i])) == nullptr)
        {
            printf("内存分配出错！\n");
            exit(1);
        }
        if (w[i])
        {
            min_heap.push(tmp);
            cnt++;
        }
    }

    for(int i = 0; i < cnt - 1; i++) //建立哈夫曼树
    {
        HuffTreeAssist<char_type, weight_type> t, t1, t2; //临时变量
        t1 = min_heap.top();
        min_heap.pop();
        t2 = min_heap.top();
        min_heap.pop();
        if((t.root = new HuffInNode<char_type, weight_type>(t1.root->get_weight() + t2.root->get_weight(), t1.root, t2.root)) == nullptr) //合并
        {
            printf("内存分配出错！\n");
            exit(1);
        }
        min_heap.push(t); //重新入队
    }
    HuffTreeAssist<char_type, weight_type> tmp;
    tmp = min_heap.top();
    min_heap.pop(); //取出哈夫曼树

    root = tmp.root;
    cur_ptr = root; //剩余数据成员赋值

    char *code;
    if((code = new char[num]) == nullptr)
    {
        printf("内存分配出错！\n");
        exit(1);
    }
    make_code_table(root, code); //生成编码表
    delete []code;
}

template<class char_type, class weight_type>
HuffmanTree<char_type, weight_type>::~HuffmanTree()
{
    clear(root);
}

template<class char_type, class weight_type>
string HuffmanTree<char_type, weight_type>::encode(char_type ch)
{
    return code_arr[char_idx(ch)];
}

template<class char_type, class weight_type>
void HuffmanTree<char_type, weight_type>::decode(unsigned char code, char_type res[], int *cur_pos)
{
    for(int i = 0; i < 8; i++)
    {
        //逐个处理每位编码
        if (code < 128) //往左走
        {
            cur_ptr = cur_ptr->get_left();
            code = code << 1;
        }
           
        else //往右走
        {
            cur_ptr = cur_ptr->get_right();
            code = code << 1;
        }

       if(cur_ptr->is_leaf())
       {
           //当前节点为叶节点
           res[(*cur_pos)++] = ((HuffLeafNode<char_type, weight_type> *)cur_ptr)->get_char(); //把当前字符存入链表
           cur_ptr = root;
       }
    }
    return;
}


#endif // HUFF_TREE_H_INCLUDED
