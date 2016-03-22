//
//  main.cpp
//  straightforward
//
//  Created by 王曼晨 on 15/12/23.
//  Copyright © 2015年 王曼晨. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#define HASH_LEN 40000 //定义散列表大小
#define STACK_LEN 100 //定义栈大小
#define MAX 14 //最大匹配字数
using namespace std;
ofstream fout("/Users/Iris/Desktop/straightforward/sftest.txt");
typedef struct hash_node      //散列表下拉链表结果
{
    char *data;
    struct hash_node *next;
}HASH_NODE;
typedef struct           //散列表数据结构
{
    int len;
    HASH_NODE *data;
}HASH;
typedef struct      //栈数据结构
{
    int len;
    char *data[STACK_LEN];
}STACK;
unsigned int hash_fun(char *key)  //哈希函数，计算哈希值
{
    unsigned int seed=131;
    unsigned int hash=0;
    while(*key)
    {
        hash=hash*seed+*key;
        ++key;
    }
    return hash&0x7FFFFFFF;
}
void hash_init(HASH *hash)    //初始化散列表
{
    int i;
    for(i=0;i != HASH_LEN;++i)   //所有表中数据为空
        hash[i].len=0;
}
void hash_list_insert(HASH_NODE *data,char *key)      //冲突时拉下链表
{
    HASH_NODE *temp;
    int flag=0;
    while(data->next != NULL)
    {
        if(strcmp(data->data,key) != 0)
            data=data->next;
        else
        {
            flag=1;
            break;
        }
    }
    if((strcmp(data->data,key) != 0)&&flag==0){
    temp=(HASH_NODE *)malloc(sizeof(HASH_NODE));
    temp->data=key;
    temp->next=NULL;
    data->next=temp;
    }
}
void hash_insert(HASH *hash,char *key)     //插入数据
{
    int h;
    h=hash_fun(key)%HASH_LEN;
    //cout<<key<<endl;
    if(hash[h].len > 0)      //如果当前表中有数据则插入链表
        hash_list_insert(hash[h].data,key);
    else                     //否则直接插入
    {
        hash[h].data=(HASH_NODE *)malloc(sizeof(HASH_NODE));
        hash[h].data->data=key;
        hash[h].data->next=NULL;
    }
    ++hash[h].len;    //当前表中数据值加1
}
char *hash_node_key(HASH_NODE *node,char *key)     //从该表地址中进行搜索
{
    while(node)
    {
        if(strcmp(node->data,key) == 0)
            return node->data;
        node=node->next;
    }
    return NULL;
}
char *hash_get(HASH *hash,char *key)       //从散列表查找
{
    int h;
    h=hash_fun(key)%HASH_LEN;
    if(hash[h].len == 0)
        return NULL;
    return hash_node_key(hash[h].data,key);
}
int hash_node_in(HASH_NODE *node,char *key)     //判断数据是否在该表中
{
    while(node)
    {
        if(strcmp(node->data,key) == 0)
            return 1;
        node=node->next;
    }
    return 0;
}
int hash_in(HASH *hash,char *key)      //从表中搜索关键词,如若存在则返回1
{
    int h;
    
    h=hash_fun(key)%HASH_LEN;
    if(hash[h].len == 0)
        return 0;
    return hash_node_in(hash[h].data,key);
}
void load_data(HASH *hash)        //加载词典数据并存入散列表中
{
    string buf="";
    string line="";
    //int cnt=0;
    ifstream fin("/Users/Iris/Desktop/straightforward/train.txt");
    if(!fin)
        cout<<"open train file error"<<endl;
    while(getline(fin,line))
    {
        istringstream strstm(line);
        //cout<<cnt++<<endl;
        while(strstm>>buf)
        {
            char* mystr = new char[buf.length()+1];
            strcpy(mystr,buf.c_str());
            hash_insert(hash,mystr);     //插入数据
        }
    }
}
void hash_list_print(HASH_NODE *data)
{
    while(data != NULL)
    {
        fout<<data->data<<" ";
        data=data->next;
    }
    fout<<endl;
}
void hash_print(HASH *hash)
{
    for(int i=0;i != HASH_LEN;++i)
    {
        if(hash[i].len != 0)
            hash_list_print(hash[i].data);
    }
}
void stack_init(STACK *stack)
{
    int i;
    stack->len=0;                 //栈数据置零
    for(i=0;i != STACK_LEN;++i)
        stack->data[i]=NULL;
}
int stack_push(STACK *stack,char *data)    //压入一个数据到栈中
{
    if(stack->len >= STACK_LEN)
        return 0;
    stack->data[stack->len]=data;
    ++stack->len;
    return stack->len;
}
char *stack_pop(STACK *stack)    //从栈中弹出一个数据
{
    if(stack->len <= 0)
        return NULL;
    --stack->len;
    return stack->data[stack->len];
}
int for_match(HASH *hash,STACK *stack,char *data,int *index)
{
    int len=strlen(data);
    while(len)
    {
        if(hash_in(hash,data))       //判断词典中是否有该词
        {
            stack_push(stack,data);  //有则将该词压入栈中
            *index+=len;
            return 1;
        }
        len-=3;                      //否则从字符串后减一个字进行循环
        data=(char*)realloc(data,sizeof(char)*len+1);
        data[len]='\0';
    }
    return 0;
}
int main() {
    HASH hash[HASH_LEN]; //散列表
    STACK stack;
    STACK res;
    char *temp;
    int i=0;
    hash_init(hash);
    load_data(hash);
    int flag=0;
    char *t;
    //hash_print(hash);
    ifstream fin1("/Users/Iris/Desktop/straightforward/test.txt");
    char string[2000]={0};
    while(fin1>>string)
    {
        stack_init(&stack);
        stack_init(&res);
        i=0;
        while(i<strlen(string))
        {
            flag=0;
            temp=(char*)malloc(sizeof(char)*3*MAX+1);
            snprintf(temp,sizeof(char)*3*MAX+1,"%s",string+i);
            flag=for_match(hash,&stack,temp,&i);
            if(flag==0)
            {
                t=(char*)malloc(sizeof(char)*3+1);
                snprintf(t,sizeof(char)*3+1,"%s",string+i);
                stack_push(&stack,t);
                i+=3;
            }
        }
        while(temp=stack_pop(&stack))
            stack_push(&res,temp);
        while(temp=stack_pop(&res))
            fout<<temp<<"  ";
        fout<<endl;
    }
    return 0;
}
