#include <iostream>
#include<random>
using namespace std;

void insersion_sort(int a[], int len)
{
    for (int i = 1; i < len; i++) {
        int j = i;
        while(j>0 && a[j] <a[j -1]){
            int temp = a[j];
            a[j] = a[j-1];
            a[j-1] = temp;
            j--;
        }
    }
}

//可以不用一直交换，只需要移动别的，当前需要插入的元素最后插入即可
void insersion_sort_v2(int a[], int len)
{
    for (int i = 1; i < len; i++) {
        int j = i -1 ;
        int key = a[i];
        while(j>=0 && key <a[j]){
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = key;

    }
}

void print_arr(int a[], int len){
    for(int i=0;i<len;i++){
        cout<<a[i]<<" ";
    }
    cout<<endl;
}

int main()
{
    int a[20];
    int len_a = sizeof(a)/sizeof(a[0]);

    for(int i = 1; i < len_a; i++) {
        a[i] = rand()%100;
    }
    print_arr(a,len_a);
    insersion_sort(a, len_a);
    print_arr(a,len_a);
    return 0;
}