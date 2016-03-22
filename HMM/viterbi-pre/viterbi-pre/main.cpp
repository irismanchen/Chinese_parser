//
//  main.cpp
//  viterbi
//
//  Created by 王曼晨 on 15/12/19.
//  Copyright © 2015年 王曼晨. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;
void makedb()
{
    ifstream fin("/Users/Iris/Desktop/viterbi/train.txt");
    ifstream fin1("/Users/Iris/Desktop/viterbi/test.txt");
    ofstream fout2("/Users/Iris/Desktop/viterbi/word2num.txt");
    map<string,int>map_cchar;
    int id=-1;
    string line="";
    string cchar="";
    string chartmp="";
    while(getline(fin,line))
    {
        if(line.size()>=3)
        {
            istringstream strstm(line);
            while(strstm>>cchar)
            {
                if(cchar.size()%3!=0)
                {
                    cout<<cchar<<endl;
                    continue;
                }
                int num = (int)cchar.size()/3;
                for(int i=0;i<num;i++)
                {
                    chartmp.clear();
                    chartmp.insert(chartmp.size(),cchar,3*i,3);
                    //cout<<chartmp<<endl;
                }
                if(map_cchar.find(chartmp)==map_cchar.end())
                {
                    ++id;
                    map_cchar[chartmp]=id;
                }
            }
        }
    }
    while(getline(fin1,line))
    {
        if(line.size()>=3)
        {
            istringstream strstm(line);
            while(strstm>>cchar)
            {
                if(cchar.size()%3!=0)
                {
                    cout<<cchar<<endl;
                    continue;
                }
                int num = (int)cchar.size()/3;
                for(int i=0;i<num;i++)
                {
                    chartmp.clear();
                    chartmp.insert(chartmp.size(),cchar,3*i,3);
                    //cout<<chartmp<<endl;
                }
                if(map_cchar.find(chartmp)==map_cchar.end())
                {
                    ++id;
                    map_cchar[chartmp]=id;
                }
            }
        }
    }
    map<string,int>::iterator iter;
    int cnt=0;
    for(iter=map_cchar.begin();iter!=map_cchar.end();++iter)
    {
        fout2<<iter->first<<" "<<iter->second<<endl;
        cnt++;
    }
    cout<<cnt<<endl;
}
void makeBMES()
{
    ifstream fin("/Users/Iris/Desktop/viterbi/train.txt");
    ofstream fout("/Users/Iris/Desktop/viterbi/traintmp.txt");
    if(!fin)
        cout<<"train file open fail!"<<endl;
    string word_in="";
    string word_out="";
    string line_in="";
    string line_out="";
    while(getline(fin,line_in))
    {
        if(line_in.size()>=3)
        {
            line_out.clear();
            istringstream strstm(line_in);
            while(strstm>>word_in)
            {
                word_out.clear();
                if(word_in.size()%3!=0)
                {
                    cout<<word_in<<endl;
                    continue;
                }
                int num = (int)word_in.size()/3;       //total words in a phrase
                if(num==0)
                    continue;
                if(num==1)
                {
                    word_out = word_in;
                    word_out+="/S";
                }
                else
                {
                    word_out.insert(word_out.size(),word_in,0,3);
                    word_out+="/B";
                    for(int i=1;i<num-1;i++)
                    {
                        word_out.insert(word_out.size(),word_in,3*i,3);
                        word_out+="/M";
                    }
                    word_out.insert(word_out.size(),word_in,3*num-3,3);
                    word_out+="/E";
                }
                line_out+=word_out;
            }
            fout<<line_out<<endl;
        }
    }
}
int main() {
    makedb();
    makeBMES();
    return 0;
}
