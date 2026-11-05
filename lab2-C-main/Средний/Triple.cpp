#include "Triple.h"

int exampleA(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return sum;
}

int exampleB(int arr[], int n) {
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (arr[i] == arr[j]) count++;
    return count;
}

int exampleC(int n) {
    int count = 0;
    for (int i = 1; i < n; i *= 2) count++;
    return count;
}

int exampleD(int n) {
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = n; j > 0; j /= 2) count++;
    return count;
}

int exampleE(int n) {
    int count = 0;
    for (int i = 0; i * i < n; i++) count++;
    return count;
}