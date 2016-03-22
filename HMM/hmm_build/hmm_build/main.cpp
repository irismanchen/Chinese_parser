//
//  main.cpp
//  viterbi_build
//
//  Created by 王曼晨 on 15/12/19.
//  Copyright © 2015年 王曼晨. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <math.h>
using namespace std;
const int N = 4;		//隐藏状态的数目
const int M = 4577;		//汉字的个数
const double VALUE = 1.0;	//平滑算法增加的值
class DB
{
private:
    map<string,int> cchar_map;  //汉字-编码映射
    map<int,string> index_map;  //编码-汉字映射
public:
    DB();
    DB(string file);
    string getCchar(int id);		//根据编码获得汉字
    int getObservIndex(string cchar);	//根据汉字获得编码
    int getStateIndex(char state);		//根据状态获得状态编号,B-0,M-1,E-2,S-3
    vector<int> makeObservs(string line);	//将输入的句子构造为发射符号序列
};
DB::DB(){}
DB::DB(string file){
    ifstream fin(file.c_str());
    if(!fin){
        cout << "Open input file fail ! " << endl;
        exit(-1);
    }
    string line = "";
    string word = "";
    string cchar = "";
    int id = 0;
    while(getline(fin, line)){
        istringstream strstm(line);
        strstm >> word;
        cchar = word;
        strstm >> word;
        id = atoi(word.c_str());   //加入map
        cchar_map[cchar] = id;
        index_map[id] = cchar;
    }
}
int DB::getStateIndex(char state){
    switch(state){
        case 'B' :
            return 0;
            break;
        case 'M' :
            return 1;
            break;
        case 'E' :
            return 2;
            break;
        case 'S' :
            return 3;
            break;
        default :
            return -1;
            break;
    }
}
int DB::getObservIndex(string cchar){
    map<string, int>::iterator iter = cchar_map.find(cchar);
    if(iter != cchar_map.end()){
        return iter -> second;
    }else{
        return -1;
    }
}
string DB::getCchar(int id){
    map<int, string>::iterator iter = index_map.find(id);
    if(iter != index_map.end()){
        return iter -> second;
    }else{
        return NULL;
    }
}
vector<int> DB::makeObservs(string line){
    vector<int> vec_observ; //输出符号的集合
    string cchar = "";      //存放每个汉字
    string word = "";       //存放一个单词
    int num = 0;            //单词的字数
    int index = -1;         //单词对应的编号
    istringstream strstm(line);
    while(strstm >> word){
        if(word.size()%3 != 0){
            cout << "单词不符合要求：" << word << endl;
            continue;
        }
        num =(int)word.size()/3;
        if(num == 0){
            continue;
        }
        else{
            for(int i = 0; i < num; i++){
                cchar = word.substr(3*i, 3);
                index = getObservIndex(cchar);
                vec_observ.push_back(index);
                //cout << "cchar = " << cchar << "   index = " << index << endl;
            }
        }
    }
    return vec_observ;
}

DB db("/Users/Iris/Desktop/viterbi/word2num.txt");

void turingAdd(const int count[], double prob[], int len){
    double sum = 0.0;
    for(int i = 0; i < len; ++i){
        sum += count[i];
    }
    sum = sum + VALUE * len;
    for(int i = 0; i < len; ++i){
        prob[i] = -log((count[i] + VALUE) / sum);//取负对数
    }
}

int main() {
    int Pi[N] = {0};		//隐藏状态(B,M,E,S)出现次数
    int A1[N][N] = {0};		//隐藏状态之间转移次数
    int A2[N][N][N] = {0};		//三阶状态转移次数
    int B1[N][M] = {0};		//隐藏状态到生成状态的出现次数
    int B2[N][N][M] = {0};		//三阶符号发射次数：隐藏－隐藏－观测
    ifstream fin1("/Users/Iris/Desktop/viterbi/traintmp.txt");
    string line = "";			//存放每一行的内容
    int line_num = 0;			//句子编号
    //int count = 0;
    while(getline(fin1, line))
    {
        line_num++;
        char state;
        string cchar="";
        int i,j,k,m;
        string::size_type pos = 0;
        if((pos = line.find("/", pos + 1)) != string::npos){
            state = line.at(pos + 1);          //抽取句子的第一个状态
            i = db.getStateIndex(state);
            Pi[i]++;
            cchar = line.substr(pos - 3, 3);   //抽取句子的第一个观察值
            m = db.getObservIndex(cchar);
            B1[i][m]++;
            if((pos = line.find("/", pos + 1)) != string::npos){    //抽取句子的第二个状态
                state = line.at(pos + 1);
                j = db.getStateIndex(state);
                A1[i][j]++;
                cchar = line.substr(pos - 3, 3);                     //抽取句子的第二个观察值
                m = db.getObservIndex(cchar);
                B1[j][m]++;
                B2[i][j][m]++;
                while((pos = line.find("/", pos + 1)) != string::npos){   //抽取句子的其他状态
                    state = line.at(pos + 1);
                    k = db.getStateIndex(state);
                    A1[j][k]++;
                    A2[i][j][k]++;
                    cchar = line.substr(pos - 3, 3);   //抽取句子的其他观察值
                    m = db.getObservIndex(cchar);
                    B1[k][m]++;
                    B2[j][k][m]++;
                    i = j;     //准备下次迭代
                    j = k;
                }
            }
        }
    }
    fin1.close();
    ofstream fout_1("/Users/Iris/Desktop/viterbi/Pi.txt");	//初始概率矩阵
    ofstream fout_2("/Users/Iris/Desktop/viterbi/A1.txt");	//二阶状态转移矩阵
    ofstream fout_3("/Users/Iris/Desktop/viterbi/A2.txt");	//三阶状态转移矩阵
    ofstream fout_4("/Users/Iris/Desktop/viterbi/B1.txt");	//二阶发射概率矩阵
    ofstream fout_5("/Users/Iris/Desktop/viterbi/B2.txt");	//三阶发射概率矩阵
    if(!(fout_1 && fout_2 && fout_3 && fout_4 && fout_5)){
        cout << "Create Matrix file failed !" << endl;
        exit(-1);
    }
    fout_1 << setprecision(8);
    fout_2 << setprecision(8);
    fout_3 << setprecision(8);
    fout_4 << setprecision(8);
    fout_5 << setprecision(8);
    double arr_pi[N] = {0.0};      //初始状态矩阵写入文件
    //turingGood(Pi, arr_pi, N);
    turingAdd(Pi, arr_pi, N);
    for(int i = 0; i < N; i++){
        fout_1 << arr_pi[i] << "\t";
    }
    fout_1 << endl;
    double arr_a_1[N] = {0.0};      //二阶状态转移矩阵写入文件
    for(int i = 0; i < N; i++){
        //turingGood(A1[i], arr_a_1, N);
        turingAdd(A1[i], arr_a_1, N);
        for(int j = 0; j < N; j++){
            fout_2 << arr_a_1[j] << "\t";
        }
        fout_2 << endl;
    }
    double arr_a_2[N] = {0.0};      //三阶状态转移矩阵写入文件
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            //turingGood(A2[i][j], arr_a_2, N);
            turingAdd(A2[i][j], arr_a_2, N);
            for(int k = 0; k < N; k++){
                fout_3 << arr_a_2[k] << "\t";
            }
            fout_3 << endl;
        }
    }
    double arr_b_1[M] = {0.0};      //二阶发射概率矩阵写入文件
    for(int i = 0; i < N; i++){
        //turingGood(B1[i], arr_b_1, M);
        turingAdd(B1[i], arr_b_1, M);
        for(int j = 0; j < M; j++){
            fout_4 << arr_b_1[j] << "\t";
        }
        fout_4 << endl;
    }
    double arr_b_2[M] = {0.0};     //三阶发射概率矩阵写入文件
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            //turingGood(B2[i][j], arr_b_2, M);
            turingAdd(B2[i][j], arr_b_2, M);
            for(int k = 0; k < M; k++){
                fout_5 << arr_b_2[k] << "\t";
            }
            fout_5 << endl;
        }
    }
    fout_1.close();
    fout_2.close();
    fout_3.close();
    fout_4.close();
    fout_5.close();
    return 0;
}
