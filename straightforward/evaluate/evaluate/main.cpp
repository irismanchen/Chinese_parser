//
//  main.cpp
//  evaluate
//
//  Created by 王曼晨 on 15/12/23.
//  Copyright © 2015年 王曼晨. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;
vector<int> getPos(string strline_right, string strline_in){    //计算切分标记的位置
    int pos_1 = 0;
    int pos_2 = -2;
    int pos_3 = 0;
    string word = "";
    vector<int> vec;
    int length = (int)strline_right.length();  //切分后的字符串
    while(pos_2 < length){
        pos_1 = pos_2;    //前面的分词标记
        pos_2 = (int)strline_right.find("  ", pos_1 + 2);  //后面的分词标记
        if(pos_2 > pos_1){
            word  = strline_right.substr(pos_1 + 2, pos_2 - pos_1 - 2);   //将两个分词标记之间的单词取出
            //cout<<word<<endl;
            pos_3 = (int)strline_in.find(word, pos_3);    //根据单词去输入序列中查出出现的位置
            vec.push_back(pos_3);       //将位置存入数组
            //cout<<pos_3<<" ";
            pos_3 = pos_3 + (int)word.size();
        }
        else{
            break;
        }
    }
    //cout<<endl;
    return vec;
}
vector<int> getCount_2(string strline, vector<int> vec_right, vector<int> vec_out)
{      //vec_right 正确的分词标记位置集合,vec_out   函数切分得到的分词标记位置集合
    vector<int> vec(4, 0);	//存放计算结果
    map<int, int> map_result;
    for(int i = 0; i < vec_right.size(); i++){
        map_result[vec_right[i]] += 1;
    }
    for(int i = 0; i < vec_out.size(); i++){
        map_result[vec_out[i]] += 2;
    }
    map<int, int>::iterator p_pre, p_cur;
    int count_value_1 = 0;    //只在vec_right中
    int count_value_2 = 0;    //只在vec_out中
    int count_value_3 = 0;    //切分正确的数目
    p_pre = map_result.begin();
    p_cur = map_result.begin();
    while(p_cur != map_result.end()){
        while(p_cur != map_result.end() && p_cur -> second == 3){
            p_pre = p_cur;
            ++count_value_3;	//切分正确的数目
            ++p_cur;		   //迭代器后移
        }
        while(p_cur != map_result.end() && p_cur -> second != 3){
            if(p_cur -> second == 1){
                ++count_value_1;
            }
            else if(p_cur -> second == 2){
                ++count_value_2;
            }
            ++p_cur;
        }
        if(p_cur == map_result.end() && p_cur == (++p_pre)){
            continue;
        }
        if(count_value_1 > 0 && count_value_2 == 0)
            vec[1]+= count_value_1;
        else if(count_value_1 == 0 && count_value_2 > 0)
            vec[2] += count_value_2;
        else if(count_value_1 > 0 && count_value_2 > 0)
            vec[3] += count_value_2;
        count_value_1 = 0;
        count_value_2 = 0;
    }
    vec[0] += count_value_3;
    return vec;
}

int main() {
    ifstream fin1("/Users/Iris/Desktop/straightforward/sftest.txt");
    ifstream fin2("/Users/Iris/Desktop/viterbi/test.txt");
    ifstream fin3("/Users/Iris/Desktop/viterbi/test.answer.txt");
    string strline_in,strline_right,strline_out_1;
    long count=0;      //句子总数
    long count_1 = 0;	//隐马尔科夫模型（二阶）切分完全正确的句子总数
    long count_out_1_all = 0;	//隐马尔科夫模型切分总数
    long count_right_all = 0;	//准确的切分总数
    long count_out_1_right_all = 0;	//隐马尔科夫模型切分正确总数
    while(getline(fin2,strline_in,'\n'))    //没切的
    {
        getline(fin1,strline_out_1);      //系统切好的
        getline(fin3,strline_right);      //正确切好的
        count++;
        vector<int> vec_right = getPos(strline_right, strline_in);
        vector<int> vec_out_1 = getPos(strline_out_1, strline_in);
        if(vec_right == vec_out_1){
            count_1++;
        }
        vector<int> vec_count_1 = getCount_2(strline_in, vec_right, vec_out_1);
        int count_out_1_right = vec_count_1[0];     //切分正确的数量
        int count_right = (int)vec_right.size();         //准确的切分数量
        int count_out_1 = (int)vec_out_1.size();	        //切分得到的数量
        count_right_all += count_right;
        count_out_1_all += count_out_1;
        count_out_1_right_all += count_out_1_right;
    }
    double kk_1 = (double)count_out_1_right_all / count_out_1_all;	//隐马尔科夫模型（二阶）准确率
    double kk_2 = (double)count_out_1_right_all / count_right_all;	//隐马尔科夫模型（二阶）召回率
    double F1 = (double) 2.0*kk_1*kk_2/(kk_1+kk_2);
    cout<< "正向最大匹配算法"<<endl;
    cout<<"准确率：" << kk_1*100 << "%"<<endl;
    cout<<"召回率：" << kk_2*100 << "%" << endl;
    cout<<"F值："<<F1<<endl;

    return 0;
}
