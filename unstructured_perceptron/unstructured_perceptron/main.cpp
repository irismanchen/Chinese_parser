//
//  main.cpp
//  unstructured_perceptron
//
//  Created by 王曼晨 on 15/12/29.
//  Copyright © 2015年 王曼晨. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <map>
using namespace std;
string line_in="";        //正在读的句子
string si;         //当前的字
string si_1;       //上个字
string si_2;       //下个字
int mi;       //当前字的标注（0、1）
string one="1";
string zero="0";
map<string,int> w;
map<string,int>::iterator iter;
int errornum=0;
#define fnum 2
void perceptron()         //对于每个字进行分类
{
    int sum[2]={0};
    string f[2][fnum];
    f[0][0]=si+"_"+zero;     //si_0;
    f[0][1]=si_1+"_"+si+"_"+zero;      //si-1_si_0
    //f[0][2]=si+"_"+si_2+"_"+zero;     //si_si+1_0
    f[1][0]=si+"_"+one;     //si_1;
    f[1][1]=si_1+"_"+si+"_"+one;       //si-1_si_1
    //f[1][2]=si+"_"+si_2+"_"+one;      //si_si+1_1
    for(int i=0;i<fnum;i++)     //初始化权重w的每一维都是0
    {
        if(w.find(f[0][i])==w.end())
            w[f[0][i]]=0;
        if(w.find(f[1][i])==w.end())
            w[f[1][i]]=0;
    }
    for(int i=0;i<fnum;i++)      //计算si被标为0、1的情况下特征向量与感知器向量的内积，谁大标为谁
    {
        sum[0]+=w[f[0][i]];
        sum[1]+=w[f[1][i]];
    }
    if(sum[mi]<=sum[1-mi])    //分类错误
    {
        errornum++;
        for(int i=0;i<fnum;i++)
        {
            w[f[mi][i]]++;
            w[f[1-mi][i]]--;
        }
    }
}
int main() {
    ofstream fout("/Users/Iris/Desktop/unstructured_perceptron/xl.txt");
    string line_out="";
    string word_in="";
    int times=6;
    while(times--){
        cout<<times<<endl;
        errornum=0;
        ifstream fin("/Users/Iris/Desktop/unstructured_perceptron/train.txt");
    while(getline(fin,line_in))       //训练过程
    {
        si_1="*";
        if(line_in.size()>=3)
        {
            line_out.clear();
            istringstream strstm(line_in);
            while(strstm>>word_in)
            {
                si.clear();
                if(word_in.size()%3!=0)
                {
                    cout<<word_in<<endl;
                    continue;
                }
                int num = (int)word_in.size()/3;       //total words in a phrase
                if(num==0)
                    continue;
                if(num==1)             //单个字，要开分的词，应该标注为1
                {
                    si = word_in;
                    mi = 1;
                    perceptron();
                    si_1=si;
                }
                else
                {
                    si.clear();
                    si.insert(si.size(),word_in,0,3);
                    mi = 1;               //要开分的词，应该标注为1
                    perceptron();
                    si_1=si;
                    for(int i=1;i<num;i++)    //不要开分的词，应该标注为0
                    {
                        si.clear();
                        si.insert(si.size(),word_in,3*i,3);
                        mi = 0;
                        perceptron();
                        si_1=si;
                    }
                }
            }
        }
    }
        cout<<errornum<<endl;
        fin.close();
    }
    for(iter=w.begin();iter!=w.end();iter++)
    {
        fout<<iter->first<<" "<<iter->second<<endl;
    }
    return 0;
}
