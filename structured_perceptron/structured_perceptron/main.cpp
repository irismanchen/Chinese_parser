//
//  main.cpp
//  structured_perceptron
//
//  Created by 王曼晨 on 15/12/30.
//  Copyright © 2015年 王曼晨. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
using namespace std;
string sentence;      //读进来的句子
int y[1000];       //标准答案句子对应的标注序列
int vy[1000];
map<string,int> w;    //感知器向量
map<string,int>::iterator iter1;
vector<int> tag;     //最后一个字到第一个字的标注
string si;       //当前字
string si_1;     //上个字
string one="1";
string zero="0";
#define fnum 3
string f[2][fnum];
int v[1000][2];       //记录当前字标注为1或者0的情况下最大值
int b[1000][2];         //记录当前字标注为1或者0的情况下上个字的标注
int wordnum;       //当前字符是句子中第几个字
int num;            //当前句子的字数
void perceptron()
{
    f[0][0]=si+"_"+zero;     //si_0;
    f[0][1]=si_1+"_"+si+"_"+zero;      //si-1_si_0
    f[1][0]=si+"_"+one;      //si_1;
    f[1][1]=si_1+"_"+si+"_"+one;       //si-1_si_1
    int cnt1,cnt2;
    for(int i=0;i<fnum;i++)     //初始化权重w的每一维都是0
    {
        if(w.find(f[0][i])==w.end())
            w[f[0][i]]=0;
        if(w.find(f[1][i])==w.end())
            w[f[1][i]]=0;
    }
    /*for(int i=0;i<fnum-1;i++)
    {
        cout<<f[0][i]<<"  "<<f[1][i]<<endl;
    }*/
    for(int i=0;i<=1;i++)        //当前字符划分为0或者1
    {
        cnt1=0;         //当前字符划分为0最大权重
        cnt2=0;         //当前字符划分为1最大权重
        if(i==0)        //如果当前字符划分为0
        {
            for(int j=0;j<=1;j++)     //上个字符划分为0或者1
            {
                if(j==0)     //如果上个字符划分为0
                {
                    cnt1+=v[wordnum-1][0];
                    f[0][2]="0_0";
                    for(int k=0;k<fnum;k++)
                    {
                        cnt1+=w[f[0][k]];
                    }
                }
                else{      //如果上个字符划分为1
                    cnt2+=v[wordnum-1][1];
                    f[0][2]="1_0";
                    for(int k=0;k<fnum;k++)
                    {
                        cnt2+=w[f[0][k]];
                    }
                }
            }
            //cout<<"000cnt1\2: "<<cnt1<<"  "<<cnt2<<endl;
            if(cnt1>=cnt2)
            {
                b[wordnum][0]=0;       //记录这个字划分为0的情况下，上个字的划分状态为0
                v[wordnum][0]=cnt1;
            }
            else
            {
                b[wordnum][0]=1;       //记录这个字划分为0的情况下，上个字的划分状态为1
                v[wordnum][0]=cnt2;
            }
        }
        else       //如果当前字符划分为1
        {
            for(int j=0;j<=1;j++)     //上个字符划分为0或者1
            {
                if(j==0)     //如果上个字符划分为0
                {
                    cnt1+=v[wordnum-1][0];
                    f[1][2]="0_1";
                    for(int k=0;k<fnum;k++)
                    {
                        cnt1+=w[f[1][k]];
                    }
                }
                else{      //如果上个字符划分为1
                    cnt2+=v[wordnum-1][1];
                    f[1][2]="1_1";
                    for(int k=0;k<fnum;k++)
                    {
                        cnt2+=w[f[1][k]];
                    }
                }
            }
            //cout<<"111cnt1\2: "<<cnt1<<"  "<<cnt2<<endl;
            if(cnt1>=cnt2)
            {
                b[wordnum][1]=0;       //记录上个字的划分状态
                v[wordnum][1]=cnt1;
            }
            else
            {
                b[wordnum][1]=1;       //记录上个字的划分状态
                v[wordnum][1]=cnt2;
            }
        }
    }
}
void viterbi()
{
    si_1="*";
    int rec;
    num = (int)sentence.size()/3;     //共有num个字
    memset(v,0,sizeof(v));
    b[0][0]=1;
    b[0][1]=1;
    si.clear();
    si.insert(si.size(),sentence,0,3);      //第一个字
    f[0][0]=si+"_"+zero;     //si_0;
    f[0][1]=si_1+"_"+si+"_"+zero;      //si-1_si_0
    f[0][2]="*_0";
    f[1][0]=si+"_"+one;      //si_1;
    f[1][1]=si_1+"_"+si+"_"+one;       //si-1_si_1
    f[1][2]="*_1";
    /*for(int i=0;i<fnum;i++)
    {
        cout<<f[0][i]<<"  "<<f[1][i]<<endl;
    }*/
    wordnum=0;
    for(int i=0;i<fnum;i++)     //初始化权重w的每一维都是0
    {
        if(w.find(f[0][i])==w.end())
            w[f[0][i]]=0;
        if(w.find(f[1][i])==w.end())
            w[f[1][i]]=0;
    }
    for(int i=0;i<fnum;i++)     //第零个字的初始v
    {
        v[0][0]+=w[f[0][i]];
        v[0][1]+=w[f[1][i]];
    }
    //cout<<v[0][0]<<"  "<<v[0][1]<<endl;
    si_1=si;
    for(int i=1;i<num;i++)
    {
        wordnum=i;
        si.clear();
        si.insert(si.size(),sentence,3*i,3);
        perceptron();
        si_1=si;
    }
    tag.clear();
    if(v[num-1][1]>=v[num-1][0])
        rec=1;
    else
        rec=0;
    tag.push_back(rec);      //最后一个字的标注
    for(int i=num-1;i>0;i--)
    {
        rec=b[i][rec];      //前一个字的标注
        tag.push_back(rec);
    }
}
void modify()
{
    string truey;
    string falsey;
    string pre_truey;
    string pre_falsey;
    for(int i=wordnum;i<num;i++)
    {
        si.clear();
        si.insert(si.size(),sentence,3*i,3);     //错误的字
        if(i==0)
        {
            si_1="*";                //如果第一个字就错了，前一个字是＊
            pre_falsey="*";       //前一个字标注是＊
            pre_truey="*";
        }
        else{
            si_1.clear();
            si_1.insert(si_1.size(),sentence,3*i-3,3);    //否则直接找前一个字
            if(y[i-1]==1)  pre_truey=one;
            else  pre_truey=zero;
            if(vy[i-1]==1)  pre_falsey=one;
            else  pre_falsey=zero;
        }
        if(y[i]==1)  truey=one;
        else  truey=zero;
        if(vy[i]==1)  falsey=one;
        else  falsey=zero;
        f[0][0]=si+"_"+truey;             //错误的字应该对应的特征向量
        f[0][1]=si_1+"_"+si+"_"+truey;
        f[0][2]=pre_truey+"_"+truey;
        f[1][0]=si+"_"+falsey;             //错误的字对应的特征向量
        f[1][1]=si_1+"_"+si+"_"+falsey;
        f[1][2]=pre_falsey+"_"+falsey;
        for(int j=0;j<fnum;j++)     //初始化权重w的每一维都是0
        {
            if(w.find(f[0][j])==w.end())
                w[f[0][j]]=0;
            if(w.find(f[1][j])==w.end())
                w[f[1][j]]=0;
        }
        for(int j=0;j<fnum;j++)
        {
            //cout<<f[0][j]<<"  "<<f[1][j]<<endl;
            w[f[0][j]]++;
            w[f[1][j]]--;
        }
    }
}
int main() {
    ofstream fout("/Users/Iris/Desktop/structured_perceptron/xl.txt");
    w["0_1"]=0;
    w["1_0"]=0;
    w["0_0"]=0;
    w["1_1"]=0;
    w["*_1"]=0;
    w["*_0"]=0;
    int errornum=0;
    int times=7;
    while(times--){
        ifstream fin("/Users/Iris/Desktop/structured_perceptron/train.txt");
        ifstream fin1("/Users/Iris/Desktop/structured_perceptron/y.txt");
        errornum=0;
    while(fin>>sentence)
    {
        viterbi();
        wordnum=0;
        num = (int)sentence.size()/3;     //共有num个字
        //cout<<num<<endl;
        for(int i=0;i<num;i++)
            fin1>>y[i];
        vector<int>::reverse_iterator iter=tag.rbegin();
        for(;iter!=tag.rend();iter++)
        {
            vy[wordnum]=*iter;
            //cout<<*iter<<" ";
            wordnum++;
        }
        //cout<<endl<<wordnum<<endl;
        for(int k=0;k<num;k++)
        {
            if(y[k]!=vy[k])
            {
                errornum++;
                wordnum=k;
                modify();
                break;
            }
        }
        //cout<<"**************************"<<endl;
    }
        fin.close();
        fin1.close();
    cout<<errornum<<endl;
    }
    for(iter1=w.begin();iter1!=w.end();iter1++)
    {
        fout<<iter1->first<<" "<<iter1->second<<endl;
    }
    return 0;
}
