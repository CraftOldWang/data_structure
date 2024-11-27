#include<iostream>
#include<random>
#include <ctime> 
using namespace std;

void counting_sort(int a[], int len){
    int r[len]={0};
    for(int i=1;i<len;i++){
        for(int j =0;j<i;j++){
            if(a[j]<=  a[i]){
                r[i]++;
            }
            else{
                r[j]++;
            }
        }
    }
    for(int i=0;i<len;i++){
        while(r[i]!= i){
            int temp = a[i];
            a[i] = a[r[i]];
            a[r[i]] = temp;
            
            temp = r[r[i]];
            r[r[i]] = r[i];
            r[i] = temp; 
        }
    }
}

//似乎与上面的思想不太一样， 这个利用空间换时间，开了一个max-min+1长度的数组，
//计算每个数出现的次数，从而计算排名
//而上面是计算每个元素的排名。
void counting_sort_v2(int a[], int len)
{
    int max = a[0],min = a[0];
    for(int i=0;i<len;i++)
    {
        if(a[i]>max){
            max = a[i];
        }
        if(a[i]<min){
            min = a[i];
        }
        
    }
    int c[max-min+1] = {0};
    int output[len];
    
    //统计出现次数。
    for(int i=0;i<len;i++){
        c[a[i]-min]++;
    }

    //累加，从而计算出桶中元素应该在的的最末位置。
    //注意是从1开始（因为前面计数是这样），因此放到output的位置要-1；
    for(int i=1;i<max-min+1;i++){
        c[i] += c[i-1];
    }

    //
    for(int i=len-1 ;i>=0;i--){
        int offset = a[i]-min;
        output[c[offset] - 1] = a[i];
        c[offset]--;
    }

    //放回去
    for(int i=0;i<len;i++)
    {
        a[i]  = output [i];
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
    srand(time(NULL));

    int a[20];
    int len_a = sizeof(a)/sizeof(a[0]);

    for(int i = 1; i < len_a; i++) {
        a[i] = rand()%100;
    }
    print_arr(a,len_a);
    counting_sort_v2(a, len_a);
    print_arr(a,len_a);
    return 0;
}