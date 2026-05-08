#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

void printArray(vector<int>& arr) {
    for (int x : arr) cout << x << " ";
    cout << endl;
}

// ========== BUBBLE SORT ==========
void sequentialBubble(vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
    cout << "Sequential Bubble Sorted: "; printArray(arr);
}

void parallelBubble(vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        int start = i % 2;
        #pragma omp parallel for
        for (int j = start; j < n - 1; j += 2)
            if (arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
    }
    cout << "Parallel Bubble Sorted:   "; printArray(arr);
}

// ========== MERGE SORT ==========
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> left(arr.begin()+l, arr.begin()+m+1);
    vector<int> right(arr.begin()+m+1, arr.begin()+r+1);
    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size())
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    sequentialMergeSort(arr, l, m);
    sequentialMergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int l, int r, int depth = 0) {
    if (l >= r) return;
    int m = (l + r) / 2;
    if (depth < 4) {
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m, depth + 1);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r, depth + 1);
        }
    } else {
        parallelMergeSort(arr, l, m, depth + 1);
        parallelMergeSort(arr, m + 1, r, depth + 1);
    }
    merge(arr, l, m, r);
}

// ========== MAIN ==========
int main() {
    int n;
    cout << "Enter number of elements: "; cin >> n;
    vector<int> arr(n);
    cout << "Enter elements:\n";
    for (int& x : arr) cin >> x;

    cout << "\nOriginal: "; printArray(arr);

    // --- Bubble Sort ---
    double t1 = omp_get_wtime();
    sequentialBubble(arr);
    double t2 = omp_get_wtime();
    cout << "Sequential Bubble Time: " << (t2 - t1) << "s\n\n";

    t1 = omp_get_wtime();
    parallelBubble(arr);
    t2 = omp_get_wtime();
    cout << "Parallel Bubble Time:   " << (t2 - t1) << "s\n\n";

    // --- Merge Sort ---
    vector<int> arr2 = arr;
    t1 = omp_get_wtime();
    sequentialMergeSort(arr2, 0, arr2.size() - 1);
    t2 = omp_get_wtime();
    cout << "Sequential Merge Sorted: "; printArray(arr2);
    cout << "Sequential Merge Time:   " << (t2 - t1) << "s\n\n";

    vector<int> arr3 = arr;
    t1 = omp_get_wtime();
    parallelMergeSort(arr3, 0, arr3.size() - 1);
    t2 = omp_get_wtime();
    cout << "Parallel Merge Sorted:   "; printArray(arr3);
    cout << "Parallel Merge Time:     " << (t2 - t1) << "s\n";

    return 0;
}