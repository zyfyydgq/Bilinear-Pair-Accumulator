//
// Created by Administrator on 2021/10/22.
//

#include <stdio.h>
#include <iomanip>
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

using namespace std;

#define F_PATH "/home/zyfyydgq/pbc-0.5.14/param/a.param"

string *read_txt_client(string file_name, long n, int t_i) {
    string *str = new string[n];

    char *buffer = (char *) malloc(sizeof(char) * MAX);
    ifstream out;
    //cout << file_name << endl;
    out.open("d_" + file_name);

    for (int i = 0; i < n; i++) {
        out.getline(buffer, MAX);
        //cout << "e: " << buffer << endl;
        str[i] = buffer;
    }
    //cout << str << endl;
    out.close();
    free(buffer);
    return str;
}


int main(int argc, char **argv) {

    pairing_t pairing;
    pbc_demo_pairing_init(pairing, argc, argv);
    cout << setiosflags(ios::fixed) << setprecision(6);
    cout << "index\tround\tacc\t\twit\t\tver\t\tDO\t\tCSP" << endl;
    int loop = 100;    //loop times
    //计时初始化
    clock_t __start, __end;
    clock_t _start_acc, _end_acc;
    clock_t _start_wit, _end_wit;
    double time_acc = 0, time_wit = 0, time_ver = 0, time_DO = 0, time_CSP = 0;

    for (int index = 0; index < 16; index++) {
        time_acc = 0, time_wit = 0, time_ver = 0, time_DO = 0, time_CSP = 0;
        for (int round = 0; round < loop; round++) {
            string *b = new string[MAX];
            string file_name;
            long l, n, file_length;
            if(index > 7){
                //block size(l) is changing
                //cout << "l "<< index - 8 << "\t" << round + 1 << "\t";
                file_name = "test_file_l" + to_string(index - 8) + ".txt";
                file_length = 2048 * 1024;
                n = 1024;
                l = 256 * (index - 7);
            }
            else{
                //file size is changing
                //cout << "f " << index << "\t" << round + 1 << "\t";
                file_name = "test_file_f" + to_string(index) + ".txt";
                file_length = 256 * 1024 * (index + 1);
                l = 256;  //file's length is l
                n = file_length / l;       //file has n blocks
            }
            b = read_txt_client(file_name, n, index);
            //cout << b[0] << endl;

            //初始化
            _start_acc = clock();
            element_t s, g, acc, wit;
            element_t t, t_s, t_bj, t_acc, t_wit;
            element_t b_e[n], b_j, t_e[n];
            element_t temp1, temp2;


            element_init_G1(g, pairing); //初始化群G1的g
            element_init_Zr(s, pairing);
            element_init_Zr(t, pairing);
            element_init_Zr(t_acc, pairing);
            element_init_Zr(t_wit, pairing);
            element_init_G1(acc, pairing);
            element_init_G1(wit, pairing);
            element_init_G1(b_j, pairing);

            //初始化bi
            for (int i = 0; i < n; i++) {
                string *t_str = (string *) b[i].data();
                //cout << b[i].data() << endl;
                element_init_Zr(b_e[i], pairing);
                element_from_hash(b_e[i], t_str, b[i].length());
            }

            element_init_GT(temp1, pairing);
            element_init_GT(temp2, pairing);

            //default G1 = G2
            //printf("G1 = G2: %d\n", pairing_is_symmetric(pairing));

            //赋值
            element_random(s);
            element_random(g);
            element_set0(t);
            element_set1(t_acc);
            element_set1(t_wit);


            //计算acc
            __start = clock();
            for (int i = 0; i < n; i++) {
                element_add(t, b_e[i], s);
                element_mul(t_acc, t_acc, t);

            }
            element_pow_zn(acc, g, t_acc);
            __end = clock();
            _end_acc = clock();
            time_acc += (double) (__end - __start) / CLOCKS_PER_SEC;
            //cout << (double) (__end - __start) / CLOCKS_PER_SEC << "\t";

            //计算wit
            _start_wit = clock();
            int j = rand() % n;
            __start = clock();
            for (int i = 0; i < n; i++) {
                if (i != j) {
                    element_add(t, b_e[i], s);
                    element_mul(t_wit, t_wit, t);
                }
            }
            element_pow_zn(wit, g, t_wit);
            __end = clock();
            element_add(t, b_e[j], s);
            element_pow_zn(b_j, g, t);
            _end_wit = clock();
            time_wit += (double) (__end - __start) / CLOCKS_PER_SEC;
            //cout << (double) (__end - __start) / CLOCKS_PER_SEC << "\t";


            __start = clock();
            //验证关系 e(acc, g) = e(wit, g^s * g^bj)
            element_pairing(temp1, acc, g);
            element_pairing(temp2, wit, b_j);
            //printf("If temp1 = temp2 ?\n");

            if (element_cmp(temp1, temp2) == 0) {
                //printf("Yes.\n");
            } else {
                //printf("No.\n");
            }
            __end = clock();

            time_ver += (double) (__end - __start) / CLOCKS_PER_SEC;
            //cout << (double) (__end - __start) / CLOCKS_PER_SEC << "\t";
            time_DO += (double) (_end_acc - _start_acc) / CLOCKS_PER_SEC;
            //cout << (double) (_end_acc - _start_acc) / CLOCKS_PER_SEC << "\t";
            time_CSP += (double) (_end_wit - _start_wit) / CLOCKS_PER_SEC;
            //cout << (double) (_end_wit - _start_wit) / CLOCKS_PER_SEC << "\n";



        }
        if(index > 7){
            cout << "l " << index - 8 << "\tavg\t";
        }
        else{
            cout << "f " << index << "\tavg\t";
        }

        cout << time_acc / loop << "\t";
        cout << time_wit / loop << "\t";
        cout << time_ver / loop << "\t";
        cout << time_DO / loop << "\t";
        cout << time_CSP / loop << "\n";
        if(index == 7) cout << endl;
        //break;
    }
    return 0;
}

