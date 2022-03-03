//
// Created by Administrator on 2021/10/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "CannotResolve"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <functional>
#include <malloc.h>
#include <ctime>
#include "RSA.h"
#include "preBuild.h"
#include "PBC.h"

#define MAX 100000
clock_t _start, _end, __start, __end;
double endtime, endtime_c, endtime_t, endtime_b;

using namespace std;
hash<string> h;

string *read_txt(string file_name, long l, long n) {
    long count = 0;
    ifstream out;
    out.open(file_name);
    ofstream in;
    in.open("d_" + file_name, ios::trunc);
    string *str = new string[MAX];
    //cout << file_name << "'s Info:" << endl;

    long i = 0;
    string ciphertext = "";

    while (!out.eof()) {
        int *arr_e;
        char *buffer = (char *) malloc(sizeof(char) * MAX);
        ciphertext = "";
        i = 0;

        out.get(buffer, l + 1);
        //record ci
        __start = clock();
        arr_e = RSA_e(buffer);
        while (arr_e[i] != -1) {
            ciphertext += to_string(arr_e[i++]);
        }
        __end = clock();
        endtime_c += (double) (__end - __start) / CLOCKS_PER_SEC;

        //record ti
        __start = clock();
        size_t _n = h(buffer);
        __end = clock();
        endtime_t += (double) (__end - __start) / CLOCKS_PER_SEC;

        //record bi
        string str_n = to_string(_n);
        while (str_n.length() < 20) {
            str_n = "0" + str_n;
        }
        __start = clock();
        str[count++] = ciphertext + str_n;
        __end = clock();
        endtime_b += (double) (__end - __start) / CLOCKS_PER_SEC;     //record bi

        //cout << "e   : " << ciphertext << endl;
        //cout << "hash: " << str_n << endl;
        //cout << "all : " << ciphertext + str_n << endl;

        in << ciphertext + str_n << "\n";
        free(buffer);
        free(arr_e);
    }
    out.close();
    in.close();

    return str;
}

int _read_txt(string file_name) {
    int file_length = 256 * 1024;   //测试文件
    int n = 8 * 1024;               //将文件分为n块
    int l = file_length / n;        //l为每块大小，等大

    char buffer[l];
    ifstream out;
    out.open(file_name);
    ofstream in;
    in.open("d_" + file_name, ios::trunc);

    cout << file_name << "'s Info:" << endl;
    int *arr_e;
    int i = 0;
    string ciphertext = "";
    while (!out.eof()) {
        ciphertext = "";
        i = 0;
        out.get(buffer, l);
        arr_e = RSA_e(buffer);
        while (arr_e[i] != -1) {
            ciphertext += to_string(arr_e[i++]);
        }
        //cout << "the line is: " << buffer << endl;
        //cout << "e: " << ciphertext << endl;
        //cout << "d: " << RSA_d(RSA_e(buffer)) << endl;


        size_t n = h(buffer);
        string str_n = to_string(n);
        while (str_n.length() < 20) {
            str_n = "0" + str_n;
        }

        cout << "e   : " << ciphertext << endl;
        cout << "hash: " << str_n << endl;
        cout << "all : " << ciphertext + str_n << endl;
        in << ciphertext + str_n << "\n";
    }
    out.close();
    in.close();
}


int main(int argc, char **argv) {
    pre_build();  //预处理，生成固定大小内容随机的file
    string file_name;
    cout << "\n\tci\tti\tbi\tall" << endl;
    long l, n, file_length;
    for (int i = 0; i < 16; i++) {
        if(i > 7){
            //block size(l) is changing
            file_name = "test_file_l" + to_string(i - 8) + ".txt";
            file_length = 2048 * 1024;
            l = 256 * (i - 7);
        }
        else{
            //file size is changing
            file_name = "test_file_f" + to_string(i) + ".txt";
            file_length = 256 * 1024 * (i + 1);
            l = 256;  //file's length is l

        }
        n = file_length / l;       //file has n blocks

        _start = clock();
        read_txt(file_name, l, n);
        _end = clock();
        endtime += (double) (_end - _start) / CLOCKS_PER_SEC;
//        for (int j = 0; j < n; j++) {
//            cout << j << " : " << str[j] << endl;
//        }

        if(i > 7){
            cout << "l " << i - 8 << "\t";
        }
        else{
            cout << "f " << i << "\t";
        }
        cout << endtime_c << "\t";
        cout << endtime_t << "\t";
        cout << endtime_b << "\t";
        cout << endtime << endl;
        endtime = 0;
        endtime_c = 0;
        endtime_t = 0;
        endtime_b = 0;
        //free(str);
    }
    cout << "Main End." << endl;
    return 0;
}

#pragma clang diagnostic pop

