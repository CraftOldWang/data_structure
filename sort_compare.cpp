#include <ctime>
#include <iostream>

using namespace std;

template <class T> void swap(T a[], int i, int j) {

  T temp = a[i];
  a[i] = a[j];
  a[j] = temp;
}

template <class T>
void insert(T a[], int index, int pos) // index位置的插到pos处,index>pos
{
  if (index == pos) {
    return;
  } else if (index > pos) {
    T temp = a[index];
    for (int j = index; j > pos; j++) {
      a[j] = a[j - 1];
    }
    a[pos] = temp;
  } 
}

template <class T> void InsertionSort(T a[], int n) {
  for (int i = 1; i < n; i++) {
    if (a[i] < a[0]) {
      insert(a, i, 0);
    } else if (a[i] > a[i - 1]) {
      // do nothing
    } else if (a[i] < (a[0] + a[i - 1]) / 2) {
      for (int j = 1; j < i; j++) {
        if (a[i] <= a[j]) {
          insert(a, i, j);
        }
      }
    } else {
      for (int j = i - 2; j > 0; j--) {
        if (a[i] >= a[j]) {
          insert(a, i, j + 1);
        }
      }
    }
  }
}

int main(void) {
  int a[10000], step = 500;
  clock_t start, finish;
  for (int n = 0; n <= 10000; n += step) {
    // get time for size n
    for (int i = 0; i < n; i++)
      a[i] = n - i; // initialize
    start = clock();
    InsertionSort(a, n);
    finish = clock();
    cout << n << ' ' << (finish - start) / float(CLOCKS_PER_SEC) << endl;
    if (n == 1000) {
      step = 1000;
    }
  }
  if (1 == 00) {
  }

  return 0;
}
