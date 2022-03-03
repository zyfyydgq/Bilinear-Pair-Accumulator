//
// Created by Administrator on 2021/10/22.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <functional>
#include <malloc.h>
#include <ctime>
#include "/usr/local/include/pbc/pbc.h"
#include "/usr/local/include/pbc/pbc_test.h"

#define MAX 100000
clock_t __start;
clock_t __end;
clock_t _start_a;
clock_t _end_a;
clock_t _start_b;
clock_t _end_b;
double endtime;

using namespace std;

#define F_PATH "/home/zyfyydgq/pbc-0.5.14/param/a.param"

string *read_txt_client(string file_name, long n, int t_i) {
    string *str = new string[n];

    char *buffer = (char *) malloc(sizeof(char) * MAX);
    ifstream out;
    out.open("d_" + to_string(t_i) + file_name);

    for (int i = 0; i < n; i++) {
        out.getline(buffer, MAX);
        //cout << "e: " << buffer << endl;
        str[i] = buffer;
    }
    out.close();
    return str;
}


int main(int argc, char **argv) {

    cout << "\n\tacc\twit\tver\tDO\tCSP" << endl;
    for (int i = 0; i < 8; i++) {
        string *b = new string[MAX];
        i = 0;
        cout << i << "\t";

        //按block size，read_txt传参为i
//        string file_name = "test_file7.txt";
//        long file_length = 2048 * 1024;
//        long l = 256 * (i + 1);
//        long n = file_length / l;
//        b = read_txt_client(file_name, n, i);

        //按file size，read_txt传参为9
        string file_name = "test_file" + to_string(i) + ".txt";
        long file_length = 256 * 1024 * (i + 1);  //测试文件大小
        long l = 256;  //l为每块大小，等大
        long n = file_length / l;       //将文件分为n块
        b = read_txt_client(file_name, n, 9);


        //初始化
        _start_a = clock();
        pairing_t pairing;
        element_t s, g, acc, wit;
        element_t t, t_s, t_bj;
        element_t b_e[n], b_j, t_e[n];
        element_t temp1, temp2;

        pbc_demo_pairing_init(pairing, argc, argv);

        element_init_G1(g, pairing); //初始化群G1的g
        element_init_G1(s, pairing);
        element_init_G1(t, pairing);
        element_init_G1(t_s, pairing);
        element_init_G1(t_bj, pairing);
        element_init_G1(acc, pairing);
        element_init_G1(wit, pairing);
        element_init_G1(b_j, pairing);

        //初始化bi
        for (int i = 0; i < n; i++) {
            const char *t_str = b[i].data();
            element_init_G1(b_e[i], pairing);
            element_set_str(b_e[i], t_str, 10);
        }

        element_init_GT(temp1, pairing);
        element_init_GT(temp2, pairing);

        //default G1 = G2
        //printf("G1 = G2: %d\n", pairing_is_symmetric(pairing));

        //赋值
        element_random(s);
        element_random(g);


        //计算acc
        __start = clock();
        for (int i = 0; i < n; i++) {
            element_add(t, b_e[i], s);
            element_pow_zn(t, g, t);
            element_mul(acc, acc, t);
        }
        __end = clock();
        _end_a = clock();
        endtime = (double) (__end - __start) / CLOCKS_PER_SEC;
        cout << endtime << "\t";


        //计算wit
        _start_b = clock();
        int j = rand() % n;
        __start = clock();
        for (int i = 0; i < n; i++) {
            if (i != j) {
                element_add(t, b_e[i], s);
                element_pow_zn(t, g, t);
                element_mul(acc, acc, t);
            }
        }
        __end = clock();
        element_pow_zn(t_s, g, s);
        element_pow_zn(t_bj, g, b_j);
        element_add(t, t_s, t_s);
        _end_b = clock();
        endtime = (double) (__end - __start) / CLOCKS_PER_SEC;
        cout << endtime << "\t";

        __start = clock();
        //验证关系 e(acc, g) = e(wit, g^s * g^bj)
        element_pairing(temp1, acc, g);
        element_pairing(temp2, wit, t);
        //printf("If temp1 = temp2 ?\n");
        if (element_cmp(temp1, temp2) == 0) {
            //printf("Yes.\n");
        } else {
            //printf("No.\n");
        }
        __end = clock();

        endtime = (double) (__end - __start) / CLOCKS_PER_SEC;
        cout << endtime << "\t";
        endtime = (double) (_end_a - _start_a) / CLOCKS_PER_SEC;
        cout << endtime << "\t";
        endtime = (double) (_end_b - _start_b) / CLOCKS_PER_SEC;
        cout << endtime << "\n";

        delete b;
    }
    return 0;
}

