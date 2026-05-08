#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

/* Sequential Bubble Sort */
void seqBubble(vector<int> a) {

    int n = a.size();

    for (int i = 0; i < n - 1; i++) {

        for (int j = 0; j < n - i - 1; j++) {

            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

/* Parallel Bubble Sort (Odd-Even Sort) */
void parBubble(vector<int> a) {

    int n = a.size();

    for (int i = 0; i < n; i++) {

        #pragma omp parallel for
        for (int j = i % 2; j < n - 1; j += 2) {

            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

/* Merge Function */
void merge(vector<int> &a, int l, int m, int r) {

    vector<int> temp;

    int i = l;
    int j = m + 1;

    while (i <= m && j <= r) {

        if (a[i] < a[j]) {
            temp.push_back(a[i++]);
        } else {
            temp.push_back(a[j++]);
        }
    }

    while (i <= m) {
        temp.push_back(a[i++]);
    }

    while (j <= r) {
        temp.push_back(a[j++]);
    }

    for (int k = 0; k < temp.size(); k++) {
        a[l + k] = temp[k];
    }
}

/* Sequential Merge Sort */
void seqMerge(vector<int> &a, int l, int r) {

    if (l < r) {

        int m = (l + r) / 2;

        seqMerge(a, l, m);
        seqMerge(a, m + 1, r);

        merge(a, l, m, r);
    }
}

/* Parallel Merge Sort */
void parMerge(vector<int> &a, int l, int r) {

    if (l < r) {

        int m = (l + r) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parMerge(a, l, m);

            #pragma omp section
            parMerge(a, m + 1, r);
        }

        merge(a, l, m, r);
    }
}

int main() {

    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n), temp;

    cout << "Enter elements:\n";

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    double t1, t2;

    /* Sequential Bubble Sort */
    temp = arr;

    t1 = omp_get_wtime();
    seqBubble(temp);
    t2 = omp_get_wtime();

    cout << "Sequential Bubble Sort Time: "
         << t2 - t1 << endl;

    /* Parallel Bubble Sort */
    temp = arr;

    t1 = omp_get_wtime();
    parBubble(temp);
    t2 = omp_get_wtime();

    cout << "Parallel Bubble Sort Time: "
         << t2 - t1 << endl;

    /* Sequential Merge Sort */
    temp = arr;

    t1 = omp_get_wtime();
    seqMerge(temp, 0, n - 1);
    t2 = omp_get_wtime();

    cout << "Sequential Merge Sort Time: "
         << t2 - t1 << endl;

    /* Parallel Merge Sort */
    temp = arr;

    t1 = omp_get_wtime();
    parMerge(temp, 0, n - 1);
    t2 = omp_get_wtime();

    cout << "Parallel Merge Sort Time: "
         << t2 - t1 << endl;

    return 0;
}