//
// Created by Administrator on 2021/10/21.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>

using namespace std;

char random_char() {
    int tmp = rand() % 62;
    if (tmp < 26) {
        return 'A' + tmp % 26;
    } else if (tmp < 52) {
        return 'a' + (tmp - 26) % 26;
    } else {
        return '0' + (tmp - 52) % 10;
    }
}

char random_ascii() {
    return '!' + rand() % 94;
}

void pre_build() {
    for (int i = 0; i < 16; i++) {
        ofstream File;
        int step, length;
        string index;
        if(i < 8){
            step =  256 * 1024;
            length = 256 * 1024;
            index = to_string(i);
            File.open("test_file_f" + index + ".txt");
        } else{
            step =  0;
            length = 2048 * 1024;
            index = to_string(i - 8);
            File.open("test_file_l" + index + ".txt");
        }
        for (int j = 0; j < length + i * step; j++) {
            File << random_char();
        }
        File.close();
        if(i < 8){
            cout << "The test_file_f" + index + ".txt create successfully." << endl;
        } else{
            cout << "The test_file_l" + index + ".txt create successfully." << endl;
        }
    }
    //system("dir test_file.txt");
    //system("notepad test_file.txt");
}


