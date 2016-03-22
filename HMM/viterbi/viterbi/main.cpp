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
#include <vector>
#include <stack>
#include <iomanip>
using namespace std;
const int N = 4;
const int M = 4577;
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
class HMM
{
public:
    int n;			//状态数目
    int m;			//可能的观察符号数目
    double Pi[N];		//初始状态概率
    double A1[N][N];	//状态转移概率矩阵
    double A2[N][N][N];	//状态转移概率矩阵
    double B1[N][M];	//符号发射概率矩阵
    double B2[N][N][M];	//符号发射概率矩阵
    HMM();
    HMM(string f_pi, string f_a1, string f_a2, string f_b1, string f_b2);
};
HMM::HMM(){}
HMM::HMM(string f_pi, string f_a1, string f_a2, string f_b1, string f_b2){
    ifstream fin_1(f_pi.c_str());
    ifstream fin_2(f_a1.c_str());
    ifstream fin_3(f_a2.c_str());
    ifstream fin_4(f_b1.c_str());
    ifstream fin_5(f_b2.c_str());
    if(!(fin_1 && fin_2 && fin_3 && fin_4 && fin_5)){
        exit(-1);
    }
    n = N;
    m = M;
    string line = "";
    string word = "";
    getline(fin_1, line);  //读取Pi
    istringstream strstm_1(line);
    for(int i = 0; i < N; i++){
        strstm_1 >> word;
        Pi[i] = atof(word.c_str());
        //cout<<Pi[i]<<endl;
    }
    for(int i = 0; i < N; i++){   //读取A1
        getline(fin_2, line);
        istringstream strstm_2(line);
        for(int j = 0; j < N; j++){
            strstm_2 >> word;
            A1[i][j] = atof(word.c_str());
        }
    }
    for(int i = 0; i < N; i++){    //读取A2
        for(int j = 0; j < N; j++){
            getline(fin_3, line);
            istringstream strstm_3(line);
            for(int k = 0; k < N; k++){
                strstm_3 >> word;
                A2[i][j][k] = atof(word.c_str());
            }
        }
    }
    for(int i = 0; i < N; i++){  //读取B1
        getline(fin_4, line);
        istringstream strstm_4(line);
        for(int j = 0; j < M; j++){
            strstm_4 >> word;
            B1[i][j] = atof(word.c_str());
        }
    }
    for(int i = 0; i < N; i++){   //读取B2
        for(int j = 0; j < N; j++){
            getline(fin_5, line);
            istringstream strstm_5(line);
            for(int k = 0; k < M; k++){
                strstm_5 >> word;
                B2[i][j][k] = atof(word.c_str());
            }
        }
    }
    fin_1.close();
    fin_2.close();
    fin_3.close();
    fin_4.close();
    fin_5.close();
}
DB db("/Users/Iris/Desktop/viterbi/word2num.txt");
HMM hmm("/Users/Iris/Desktop/viterbi/Pi.txt","/Users/Iris/Desktop/viterbi/A1.txt","/Users/Iris/Desktop/viterbi/A2.txt","/Users/Iris/Desktop/viterbi/B1.txt","/Users/Iris/Desktop/viterbi/B2.txt");
string viterbiTwo(string str_in){
    int row = (int)str_in.size() / 3;       //计算输入句子中的汉字个数
    //cout<<row<<endl;
    string str_out = "";
    if(row == 0){         //如果输入字符串为空，则直接返回空
        return str_out;
    }
    if(row < 2){      //如果只有一个字的话，则直接输出即可
        str_out = str_in + "  ";
        return str_out;
    }
    double **delta = new double *[row];   //分配矩阵空间
    int **path = new int *[row];
    for(int i = 0; i < row; i++){
        delta[i] = new double[N]();
        path[i] = new int[N]();
    }
    string cchar = "";	//存放汉字
    int min_path = -1;
    double val = 0.0;
    double min_val = 0.0;
    cchar = str_in.substr(0, 3);   //取第一个字
    int cchar_num = db.getObservIndex(cchar);   //第一个字的编号
    for(int i = 0; i < N; i++){     //初始化矩阵,给delta和path矩阵的第一行赋初值
        delta[0][i] = hmm.Pi[i] + hmm.B1[i][cchar_num];	//之前取过负对数了
        path[0][i] = -1;
    }
    for(int t = 1; t < row; t++){   //给delta和path的后续行赋值
        cchar = str_in.substr(3*t, 3);
        cchar_num = db.getObservIndex(cchar);
        for(int j = 0; j < N; j++){    //遍历当前字每个隐藏状态
            min_val = 100000.0;
            min_path = -1;
            for(int i = 0; i < N; i++){   //遍历上个字每个隐藏状态
                val = delta[t-1][i] + hmm.A1[i][j]; //加隐藏状态转移函数
                if(val < min_val){
                    min_val = val;
                    min_path = i;
                }
            }
            delta[t][j] = min_val + hmm.B1[j][cchar_num];
            path[t][j] = min_path;
        }
    }
    min_val = 100000.0;
    min_path = -1;
    for(int i = 0; i < N; i++){        //找delta矩阵最后一行的最大值
        if(delta[row-1][i] < min_val){
            min_val = delta[row-1][i];
            min_path = i;
        }
    }
    stack<int> path_st;
    path_st.push(min_path);
    for(int i = row - 1; i > 0; i--){    //从min_path出发,回溯得到最可能的路径
        min_path = path[i][min_path];
        path_st.push(min_path);
    }
    for(int i = 0; i < row; i++){   //释放二维数组
        delete []delta[i];
        delete []path[i];
    }
    delete []delta;
    delete []path;
    int pos = 0;
    int index = -1;
    while(!path_st.empty()){
        index = path_st.top();
        path_st.pop();
        str_out.insert(str_out.size(), str_in, pos, 3);
        if(index == 2 || index == 3){   //状态为E或S
            str_out.append("  ");
        }
        pos += 3;
    }
    return str_out;
}
string viterbiThree(string str_in){
    int row = (int)str_in.size() / 3;
    string str_out = "";
    if(row == 0){
        return str_out;
    }
    if(row < 2){
        str_out = str_in + "  ";
        return str_out;
    }
    double ***delta = new double **[row];
    int ***path = new int **[row];
    for(int i = 0; i < row; i++){
        delta[i] = new double *[N];
        path[i] = new int *[N];
        for(int j = 0; j < N; j++){
            delta[i][j] = new double[N];
            path[i][j] = new int[N];
            for(int k = 0; k < N; k++){
                delta[i][j][k] = 0.0;
                path[i][j][k] = 0;
            }
        }
    }
    //初始化矩阵,给delta和path矩阵的第1个面赋初值
    //初始状态需要两个面，第0面不赋值，只给第1个面赋值
    string cchar_1 = str_in.substr(0, 3);	//第1个字
    string cchar_2 = str_in.substr(3, 3);	//第2个字
    int num_1 = db.getObservIndex(cchar_1);	//第1个字的编号
    int num_2 = db.getObservIndex(cchar_2);	//第2个字的编号
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            delta[1][i][j] = hmm.Pi[i] + hmm.B1[i][num_1] +
            hmm.A1[i][j] + hmm.B2[i][j][num_2];	//负对数
            path[1][i][j] = -1;
        }
    }
    string cchar_3 = "";	//存放汉字
    int min_path = -1;
    double val = 0.0;
    double min_val = 0.0;
    //给delta和path的后续面赋值（对数）
    //第0、1面为初始面，后续面从2开始，到row-1为止
    for(int t = 2; t < row; t++){
        cchar_3 = str_in.substr(3*t, 3);
        int num_3 = db.getObservIndex(cchar_3);
        for(int j = 0; j < N; j++){
            for(int k = 0; k < N; k++){
                min_val = 100000.0;
                min_path = -1;
                for(int i = 0; i < N; i++){
                    val = delta[t-1][i][j] + hmm.A2[i][j][k];
                    if(val < min_val){
                        min_val = val;
                        min_path = i;
                    }
                }
                delta[t][j][k] = min_val + hmm.B2[j][k][num_3];
                path[t][j][k] = min_path;
            }
        }
    }
    //找delta矩阵最后一个面的最大值，最后一个面为row-1
    min_val = 100000.0;
    int min_path_i = -1;
    int min_path_j = -1;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(delta[row-1][i][j] < min_val){
                min_val = delta[row-1][i][j];
                min_path_i = i;
                min_path_j = j;
            }
        }
    }
    //从min_path_i和min_path_j出发,回溯得到最可能的路径
    //回溯从row-1开始，到2为止
    stack<int> path_st;
    path_st.push(min_path_j);
    path_st.push(min_path_i);
    for(int t = row - 1; t > 1; t--){
        int min_path_k = path[t][min_path_i][min_path_j];
        path_st.push(min_path_k);
        min_path_j = min_path_i;
        min_path_i = min_path_k;
    }
    for(int i = 0; i < row; i++){       //释放三维数组
        for(int j = 0; j < N; j++){
            delete []delta[i][j];
            delete []path[i][j];
        }
        delete []delta[i];
        delete []path[i];
    }
    delete []delta;
    delete []path;
    int pos = 0;          //根据标记好的状态序列分词
    int index = -1;
    while(!path_st.empty()){
        index = path_st.top();
        path_st.pop();
        str_out.insert(str_out.size(), str_in, pos, 3);
        if(index == 2 || index == 3){
            str_out.append("  ");
        }
        pos += 3;
    }
    return str_out;
}
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
int main()
{
    string strline_in,strline_right,strline_out_1;
    ifstream fin2("/Users/Iris/Desktop/viterbi/test.txt");
    ifstream fin3("/Users/Iris/Desktop/viterbi/test.answer.txt");
    ofstream fout2("/Users/Iris/Desktop/viterbi/viterbitest2.txt");
    ofstream fout3("/Users/Iris/Desktop/viterbi/viterbitest3.txt");
    long count=0;      //句子总数
    long count_1 = 0;	//隐马尔科夫模型（二阶）切分完全正确的句子总数
    long count_out_1_all = 0;	//隐马尔科夫模型切分总数
    long count_right_all = 0;	//准确的切分总数
    long count_out_1_right_all = 0;	//隐马尔科夫模型切分正确总数
    while(getline(fin2,strline_in,'\n'))    //没切的
    {
        //cout<<strline_in<<endl;
        //cout<<strline_in.size()<<endl;
        strline_out_1 = viterbiThree(strline_in);
        fout3<<strline_out_1<<endl;
        getline(fin3,strline_right);      //切好的
        //cout<<strline_right<<endl;
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
    cout<< "隐马尔科夫模型(三阶)"<<endl;
    cout<<"准确率：" << kk_1*100 << "%"<<endl;
    cout<<"召回率：" << kk_2*100 << "%" << endl;
    cout<<"F值："<<F1<<endl;
    return 0;
}
