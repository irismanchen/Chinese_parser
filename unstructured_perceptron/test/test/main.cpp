//
//  main.cpp
//  test
//
//  Created by 王曼晨 on 15/12/30.
//  Copyright © 2015年 王曼晨. All rights reserved.
//

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;
map<string,int> w;
string si;         //当前的字
string si_1;       //上个字
#define fnum 2
string one="1";
string zero="0";
int judge()
{
    int sum[2]={0};
    string f[2][fnum];
    f[0][0]=si+"_"+zero;     //si_0;
    f[0][1]=si_1+"_"+si+"_"+zero;
    f[1][0]=si+"_"+one;     //si_1;
    f[1][1]=si_1+"_"+si+"_"+one;
    for(int i=0;i<fnum;i++)      //计算si被标为0、1的情况下特征向量与感知器向量的内积，谁大标为谁
    {
        if(w.find(f[0][i])!=w.end())
            sum[0]+=w[f[0][i]];
        if(w.find(f[1][i])!=w.end())
            sum[1]+=w[f[1][i]];
    }
    if(sum[0]>=sum[1])
        return 0;
    else
        return 1;
}
int main() {
    ifstream fin("/Users/Iris/Desktop/unstructured_perceptron/xl.txt");
    ofstream fout("/Users/Iris/Desktop/unstructured_perceptron/testup.txt");
    string line_in="";
    string word_in="";
    int ww;
    int j;
    int start=1;
    while(fin>>word_in>>ww)
    {
        w[word_in]=ww;
    }
    ifstream fin1("/Users/Iris/Desktop/unstructured_perceptron/test.txt");
    while(getline(fin1,line_in))
    {
        start=1;
        int num = (int)line_in.size()/3;      //有多少个字
        si_1="*";
        for(int i=0;i<num;i++)    //不要开分的词，应该标注为0
        {
            si.clear();
            si.insert(si.size(),line_in,3*i,3);
            j=judge();
            if(j==1&&start!=1)
                fout<<"  "<<si;
            else
                fout<<si;
            start=0;
            si_1=si;
        }
        fout<<"  "<<endl;
    }
    return 0;
}
