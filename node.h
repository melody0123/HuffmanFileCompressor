#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

using namespace std;

//先声明
template <class char_type, class weight_type>
class HuffNode // huffman 树的节点的抽象类
{
public:
    virtual weight_type get_weight() = 0; //返回节点的权值
    virtual bool is_leaf() = 0; //判断是不是叶子节点
    virtual HuffNode<char_type, weight_type>* get_left() = 0; //返回当前节点的指向左孩子节点的指针
    virtual HuffNode<char_type, weight_type>* get_right() = 0; //返回当前节点的指向右孩子节点的指针
    virtual void set_left(HuffNode<char_type, weight_type> *child) = 0; //设置当前节点的左孩子节点
    virtual void set_right(HuffNode<char_type, weight_type> *child) = 0; //设置当前节点的右孩子节点
};

template <class char_type, class weight_type>
class HuffLeafNode: public HuffNode<char_type, weight_type> //huffman 树的叶子节点
{
public:
    HuffLeafNode(const char_type &ch, const weight_type &w); //构造函数
    virtual ~HuffLeafNode(){} //析构函数
    char_type get_char(); //返回叶节点的字符
    weight_type get_weight(); //返回节点的权值
    bool is_leaf(); //判断是否为叶节点
    HuffNode<char_type, weight_type>* get_left(); //返回当前节点的指向左孩子节点的指针
    HuffNode<char_type, weight_type>* get_right(); //返回当前节点的指向右孩子节点的指针
    void set_left(HuffNode<char_type, weight_type> *child){} //设置当前节点的左孩子节点
    void set_right(HuffNode<char_type, weight_type> *child){} //设置当前节点的右孩子节点

private:
    char_type cha; //叶节点存储的字符
    weight_type weight; //叶节点的权重
};

template<class char_type, class weight_type>
class HuffInNode: public HuffNode<char_type, weight_type>
{
public:
    HuffInNode(const weight_type &w,
               HuffNode<char_type, weight_type> *lc = nullptr, HuffNode<char_type, weight_type> *rc = nullptr); //构造函数
    virtual ~HuffInNode(){} //析构函数
    weight_type get_weight(); //返回该节点权重
    bool is_leaf(); //判断是否为叶节点
    HuffNode<char_type, weight_type>* get_left(); //返回当前节点的指向左孩子节点的指针
    HuffNode<char_type, weight_type>* get_right(); //返回当前节点的指向右孩子节点的指针
    void set_left(HuffNode<char_type, weight_type> *child); //设置当前节点的左孩子节点
    void set_right(HuffNode<char_type, weight_type> *child); //设置当前节点的右孩子节点

private:
    HuffNode<char_type, weight_type> *l_child; //指向左孩子的指针
    HuffNode<char_type, weight_type> *r_child; //指向右孩子的指针
    weight_type weight; //当前节点的权重
};
//后定义
//HuffLeafNode 实现
template<class char_type, class weight_type>
HuffLeafNode<char_type, weight_type>::HuffLeafNode(const char_type &ch, const weight_type &w)
{
    weight = w;
    cha = ch;
}

template<class char_type, class weight_type>
char_type HuffLeafNode<char_type, weight_type>::get_char()
{
    return cha;
}

template<class char_type, class weight_type>
weight_type HuffLeafNode<char_type, weight_type>::get_weight()
{
    return weight;
}

template<class char_type, class weight_type>
bool HuffLeafNode<char_type, weight_type>::is_leaf()
{
    return true;
}

template<class char_type, class weight_type>
HuffNode<char_type, weight_type>* HuffLeafNode<char_type, weight_type>::get_left()
{
    return nullptr;
}

template<class char_type, class weight_type>
HuffNode<char_type, weight_type>* HuffLeafNode<char_type, weight_type>::get_right()
{
    return nullptr;
}

//HuffInNode 实现
template<class char_type, class weight_type>
HuffInNode<char_type, weight_type>::HuffInNode(const weight_type &w,
               HuffNode<char_type, weight_type> *lc, HuffNode<char_type, weight_type> *rc)
{
    weight = w;
    l_child = lc;
    r_child = rc;
}

template<class char_type, class weight_type>
weight_type HuffInNode<char_type, weight_type>::get_weight()
{
    return weight;
}

template<class char_type, class weight_type>
bool HuffInNode<char_type, weight_type>::is_leaf()
{
    return false;
}

template<class char_type, class weight_type>
HuffNode<char_type, weight_type>* HuffInNode<char_type, weight_type>::get_left()
{
    return l_child;
}

template<class char_type, class weight_type>
HuffNode<char_type, weight_type>* HuffInNode<char_type, weight_type>::get_right()
{
    return r_child;
}

template<class char_type, class weight_type>
void HuffInNode<char_type, weight_type>::set_left(HuffNode<char_type, weight_type> *child)
{
    l_child = child;
    return;
}

template<class char_type, class weight_type>
void HuffInNode<char_type, weight_type>::set_right(HuffNode<char_type, weight_type> *child)
{
    r_child = child;
    return;
}

#endif // NODE_H_INCLUDED
