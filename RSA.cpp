//
// Created by Administrator on 2021/10/19.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>


//整数的n次幂求模, pow(x, n) % p
int mod(int x, int n, int p) {
    int tmp = x % p;
    for (int i = 0; i < n - 1; i++) {
        tmp = (x * tmp) % p;
    }
    return tmp;
}

/*加密函数*/
int *encrypt(char *arr, int e, int n, int len) {
    int *array;
    array = (int *) malloc(sizeof(int) * len);
    int c = 0;

    for (int i = 0; i < len; i++) {
        c = (int) arr[i];
        array[i] = mod(c, e, n);
    }
    array[len] = -1;
    for (int i = 0; i < len; i++)
        //printf("%c\t%d\n", arr[i], array[i]);
    return array;
}

//解密函数
char *decipher(int *arr, int d, int n, int len) {
    char *array;
    array = (char *) malloc(sizeof(char) * len);
    int c = 0, t = 0;
    int i = 0;
    for (i = 0; i < len; i++) {
        c = arr[i];
        t = mod(c, d, n);
        array[i] = (char) t;
    }
    array[len] = '\0';
    for (int i = 0; i < len; i++)
        //printf("%d\t%c\n", arr[i], array[i]);
    return array;
}

//最小公倍数
int minMultiple(int m, int n) {
    int t;
    if (m < n) {
        t = m;
        m = n;
        n = t;
    }
    for (int i = m;; i++) {
        if (i % m == 0 && i % n == 0) {
            return i;
        }
    }
}

//最大公约数
int maxDivisor(int m, int n) {
    int t;
    if (m < n) {
        t = m;
        m = n;
        n = t;
    }
    for (int i = n;; i--) {
        if (m % i == 0 && n % i == 0) {
            return i;
        }
    }


}


int *RSA_e(char arr[]) {
    int p = 47, q = 71, e = 79;
    int n = p * q;
    int l = (p - 1) * (q - 1);
    int d = l - 1;
    int *arr_e;
    int len = 0;

    //1 < E < L, gcd（E，L）= 1
    while (e <= 1 || e >= l || maxDivisor(e, l) != 1) {
        scanf("%d", &e);
    }
    for (; e * d % l != 1; d--);

    for (len = 0;; len++) {
        if (arr[len] == '\0') {
            break;
        }
    }
//    while (1) {
//        arr_e = encrypt(arr, e, n, len);
//        printf("Encrypt End.\n");
//        arr_d = decipher(arr_e, d, n, len);
//        for(int i = 0; i < len; i++){
//            printf("%c", arr_d[i]);
//        printf("\nDecipher End.\n");
//        return 0;
//    }

    arr_e = encrypt(arr, e, n, len);
    return arr_e;
}

char *RSA_d(int arr[]) {
    int p = 47, q = 71, e = 79;
    int n = p * q;
    int l = (p - 1) * (q - 1);
    int d = l - 1;
    char *arr_d;
    int len = 0;

    //1 < E < L, gcd（E，L）= 1
    while (e <= 1 || e >= l || maxDivisor(e, l) != 1) {
        scanf("%d", &e);
    }
    for (; e * d % l != 1; d--);

    for (len = 0;; len++) {
        if (arr[len] < 0) {
            break;
        }
    }
//    while (1) {
//        arr_e = encrypt(arr, e, n, len);
//        printf("Encrypt End.\n");
//        arr_d = decipher(arr_e, d, n, len);
//        for(int i = 0; i < len; i++){
//            printf("%c", arr_d[i]);
//        printf("\nDecipher End.\n");
//        return 0;
//    }

    arr_d = decipher(arr, d, n, len);
    return arr_d;
}