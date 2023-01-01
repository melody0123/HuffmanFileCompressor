#ifndef LINK_LIST_H_INCLUDED
#define LINK_LIST_H_INCLUDED

#include <cstdio>

using namespace std;

template<class T>
struct node
{
    T data; //数据域
    node *next; //后继节点指针
};

template<class T>
class LinkList
{
public:
    LinkList(); //构造函数
    virtual ~LinkList(); //析构函数
    void push_back(T elem); //尾部插入
    LinkList<T>& operator=(LinkList<T> &elem); //重载赋值运算符
    node<T>* get_head(); //获取头节点指针

private:
    node<T> *head; //头节点指针
    node<T> *tail; //尾节点指针
    void init(); //辅助函数，初始化链表
    void clear(); //辅助函数，清空链表（包括头节点）
};

//实现
template<class T>
node<T>* LinkList<T>::get_head()
{
    return head;
}

template<class T>
LinkList<T>::LinkList()
{
    init();
}

template<class T>
LinkList<T>::~LinkList()
{
    clear();
}

template<class T>
void LinkList<T>::init()
{
    if((head = new node<T>) == nullptr)
    {
        printf("内存分配出错！\n");
        exit(1);
    }
    tail = head;
    head->next = nullptr;
    return;
}

template<class T>
void LinkList<T>::clear()
{
    node<T> *tmp;
    while(head != tail)
    {
        tmp = head;
        head = head->next;
        delete tmp;
        tmp = nullptr;
    }
    delete head;
    head = tail = nullptr;
    return;
}

template<class T>
void LinkList<T>::push_back(T elem)
{
    if((tail->next = new node<T>) == nullptr)
    {
        printf("内存分配出错！\n");
        exit(1);
    }
    tail = tail->next;
    tail->data = elem;
    tail->next = nullptr;
}

template<class T>
LinkList<T>& LinkList<T>::operator=(LinkList<T> &elem)
{
    if(this != &elem)
    {
        clear();
        init(); //先清空，然后初始化链表

        node<T> *tmp_src = elem->head; //数据源
        node<T> *tmp_dst = head;
        while(tmp_src->next != nullptr)
        {
            tmp_src = tmp_src->next;
            tmp_dst->next = new node<T>;
            tmp_dst = tmp_dst->next; //下一个节点非空了再移动指针
            tmp_dst->data = tmp_src->data;
            tmp_dst->next = nullptr;
        }
    }

    return *this;
}

#endif // LINK_LIST_H_INCLUDED
