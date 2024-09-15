#include "Project.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <tuple>

using namespace std;

int keyComparisonsMerge = 0;
int keyComparisonsHybrid = 0;

void insertionSort(vector<int>& arr, int left, int right)
{
    for (int i = left; i < right; ++i) {
        int key = arr[i];
        int j = i - 1;

        keyComparisonsHybrid++;
        while (j >= left && arr[j] > key) {
            keyComparisonsHybrid++;
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void mergeHybrid(vector<int>& arr, int left, 
                     int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    keyComparisonsHybrid++;
    while (i < n1 && j < n2) {
        keyComparisonsHybrid++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge(vector<int>& arr, int left, 
                     int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;
    
    while (i < n1 && j < n2) {
        keyComparisonsMerge++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    //put back in place in original array
    while (i < n1) { 
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void HybridSort(vector<int>& arr, int left, int right, int thresh)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    if(right-left < thresh)
    {
        insertionSort(arr, left, right);
    }
    else
    {
        HybridSort(arr, left, mid, thresh);
        HybridSort(arr, mid + 1, right, thresh);
        mergeHybrid(arr, left, mid, right);
    }
}

void mergeSort(vector<int>& arr, int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    
    merge(arr, left, mid, right);
}

void printVector(vector<int>& arr)
{
    for (int i = 0; i < arr.size(); i++)
        cout << arr[i] << " ";
    cout << endl;
}

vector<int> generateNumbers(int n) {
    std::vector<int> numbers;
    numbers.reserve(n); 

    int min = 1000;
    int max = 10000000;
    
    for (int i = 0; i < n; ++i) {
        int randomNumber = min + rand() % (max-min+1); 
        numbers.push_back(randomNumber);
    }
    
    return numbers;
}

void writeVectorToCSV(const std::vector<int>& vec, const std::string& filename) {
    std::ofstream file(filename);  // Open file for writing
    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
        return;
    }

    // Write vector elements to the file separated by commas
    for (size_t i = 0; i < vec.size(); ++i) {
        file << vec[i];
        if (i != vec.size() - 1) {
            file << ",";  // Add a comma between elements, but not after the last one
        }
    }

    file.close();  // Close the file
}

tuple<int,int> comparison(int n, int thresh)
{
    vector<int> arr = generateNumbers(n);
    vector<int> duplicate = arr;
    mergeSort(arr, 0, n - 1);
    HybridSort(duplicate, 0, n-1 ,thresh);
    cout << "\ key comparisons hybrid: " << keyComparisonsHybrid;
    cout << " | total key comparisons Merge: " << keyComparisonsMerge;
    cout << " | diff : " << keyComparisonsMerge - keyComparisonsHybrid;

    int tempkeyComparisonsHybrid = keyComparisonsHybrid;
    int tempkeyComparisonsMerge = keyComparisonsMerge;
    
    keyComparisonsHybrid = 0;
    keyComparisonsMerge = 0;
    
    return make_tuple(tempkeyComparisonsHybrid, tempkeyComparisonsMerge);
}

// Driver code
int main()
{
    srand(static_cast<unsigned>(time(0)));
    int n = 100000;

    vector<int> arr = generateNumbers(n);
    int thresh = 6;

    int nArray[16] = {100, 250, 500, 1000, 2500, 5000 ,10000, 25000, 50000, 100000, 250000, 500000, 1000000, 2500000, 5000000, 10000000};

    cout << "fixed S \n";

    vector<int> resultHybrid;
    vector<int> resultMerge;
    for(int i = 0 ; i < 16; i++)
    {
        cout << "\nn = " << nArray[i] << " : ";
        tuple<int,int> compResult = comparison(nArray[i], thresh);
        resultHybrid.push_back(get<0>(compResult));
        resultMerge.push_back(get<1>(compResult));
    }
    writeVectorToCSV(resultHybrid, "fixedS_HybridAlgorithm.csv");
    writeVectorToCSV(resultMerge, "fixedS_MergeAlgorithm.csv");
    resultHybrid.clear();
    resultMerge.clear();

    cout << "\n\nfixed n \n";
    int threshArray[25] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};

    for(int i = 0; i < 15; i++)
    {
        cout << "\nS = " << threshArray[i] << " : ";
        tuple<int,int> compResult = comparison(100000, threshArray[i]);
        resultHybrid.push_back(get<0>(compResult));
        resultMerge.push_back(get<1>(compResult));
    }

    writeVectorToCSV(resultHybrid, "fixedN_HybridAlgorithm.csv");
    writeVectorToCSV(resultMerge, "fixedN_MergeAlgorithm.csv");
    resultHybrid.clear();
    resultMerge.clear();
    
    
    return 0;
}