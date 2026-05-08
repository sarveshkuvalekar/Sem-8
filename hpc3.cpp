#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
using namespace std;

int main() {
    int n;
    cout << "Enter number of elements: "; cin >> n;
    vector<int> arr(n);
    cout << "Enter elements:\n";
    for (int& x : arr) cin >> x;

    int minVal = INT_MAX, maxVal = INT_MIN, sum = 0;

    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        minVal = min(minVal, arr[i]);
        maxVal = max(maxVal, arr[i]);
        sum += arr[i];
    }

    double avg = (double)sum / n;

    cout << "\nMinimum : " << minVal << endl;
    cout << "Maximum : " << maxVal << endl;
    cout << "Sum     : " << sum    << endl;
    cout << "Average : " << avg   << endl;

    return 0;
}