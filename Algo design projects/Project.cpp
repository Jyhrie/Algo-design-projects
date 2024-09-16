#include "Project.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <tuple>
#include <chrono>

using namespace std;
/*
int keyComparisonsMerge = 0;
int keyComparisonsHybrid = 0;


void insertionSort(vector<int>& arr, int left, int right)
{
    for (int i = left+1; i < right+1; ++i) {
        int key = arr[i];
        int j = i - 1;
        keyComparisonsHybrid++;
        while (j >= left && arr[j] > key) { //move elements of arr[0..i-1] that are greater than key to one position ahead of their current position
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
    
    vector<int> L(n1), R(n2); //copy the list into two halves
    
    for (int i = 0; i < n1; i++) //copy left and right from original array into new array
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;
    
    while (i < n1 && j < n2) { //merge temp arrays back into original array
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
    
    while (i < n1) { //copy the remaining elements of L & R back into original array if there are any
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

    //cout << "Start Merge \n";
    vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;
    
    while (i < n1 && j < n2) {
        keyComparisonsMerge++;
        //cout << "comparing " << L[i] << " & " << R[j] << " | " << keyComparisonsMerge << "\n"; 
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

void HybridSort(vector<int>& arr, int left, int right, int thresh)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    cout << right << "\n";
    cout << left << "\n";
    cout << thresh << "\n";
    if(right-left <= thresh)
    {
        insertionSort(arr, left, right);
    }
    else
    {
        cout << "recursion\n";
        HybridSort(arr, left, mid, thresh);
        HybridSort(arr, mid + 1, right, thresh);
    }
    mergeHybrid(arr, left, mid, right);
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


bool isSorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1]) {
            cout << arr[i] << "<" << arr[i-1] << "  ||  ";
            return false;
        }
    }
    return true;
}

tuple<int,int> comparison(int n, int thresh)
{
    vector<int> arr = generateNumbers(n);
    vector<int> duplicate = arr;
    
    auto hybridStart = std::chrono::high_resolution_clock::now();
    HybridSort(duplicate, 0, n-1 ,thresh);
    auto hybridStop = std::chrono::high_resolution_clock::now();
    auto hybridDuration = std::chrono::duration_cast<std::chrono::milliseconds>(hybridStop - hybridStart);
    cout << "\ key comparisons hybrid: " << keyComparisonsHybrid << ", time: " << hybridDuration.count() << "ms | Successfully Sorted: ";
    cout << isSorted(duplicate);
    
    auto mergeStart = std::chrono::high_resolution_clock::now();
    mergeSort(arr, 0, n - 1);
    auto mergeStop = std::chrono::high_resolution_clock::now();
    auto mergeDuration = std::chrono::duration_cast<std::chrono::milliseconds>(mergeStop - mergeStart);
    cout << " | total key comparisons Merge: " << keyComparisonsMerge << ", time: " << mergeDuration.count() << "ms. | Successfully Sorted: ";
    cout << isSorted(arr);
    
    cout << " | diff : " << keyComparisonsMerge - keyComparisonsHybrid << ", time diff: " << mergeDuration.count() - hybridDuration.count() << "ms";

    int tempkeyComparisonsHybrid = keyComparisonsHybrid;
    int tempkeyComparisonsMerge = keyComparisonsMerge;
    
    keyComparisonsHybrid = 0;
    keyComparisonsMerge = 0;
    
    return make_tuple(tempkeyComparisonsHybrid, tempkeyComparisonsMerge);
}

int optimal(int n, int thresh)
{
    vector<int> ordered;
    vector<int> dupeOrdered;
    for(int i = 0; i < n; i++)
    {
        ordered.push_back(i);
    }
    dupeOrdered = ordered;

    auto start = std::chrono::high_resolution_clock::now();
    HybridSort(ordered, 0, n-1, thresh);
    mergeSort(dupeOrdered, 0, n-1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
    
    cout << "\n\n Best Case | " << "Hybrid: " << keyComparisonsHybrid << " | " << "Default Merge: " << keyComparisonsMerge << " | Difference: " << keyComparisonsMerge-keyComparisonsHybrid << "\n";
    keyComparisonsHybrid = 0;
    keyComparisonsMerge = 0;
    return 0;
}

int worst(int n, int thresh)
{
    vector<int> ordered;
    vector<int> dupeOrdered;
    for(int i = n; i > 0; i--)
    {
        ordered.push_back(i);
    }
    dupeOrdered = ordered;

    auto start = std::chrono::high_resolution_clock::now();
    HybridSort(ordered, 0, n-1, thresh);
    mergeSort(dupeOrdered, 0, n-1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
    
    
    cout << "\n Worst Case | " << "Hybrid: " << keyComparisonsHybrid << " | " << "Default Merge: " << keyComparisonsMerge << " | Difference: " << keyComparisonsMerge-keyComparisonsHybrid << "\n";
    keyComparisonsHybrid = 0;
    keyComparisonsMerge = 0;
    return 0;
}

// Driver code
int oldmain()
{
    srand(static_cast<unsigned>(time(0)));
    int n = 100000;

    vector<int> arr = generateNumbers(n);
    int thresh = 6;

    int nArray[16] = {100, 250, 500, 1000, 2500, 5000 ,10000, 25000, 50000, 100000, 250000, 500000, 1000000, 2500000, 5000000, 10000000};

    cout << "fixed S \n";

    vector<int> resultHybrid;
    vector<int> resultMerge;
    for(int i = 0 ; i < 0; i++)
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
    int threshArray[25] = {1,3,5,7,9,12,15,17,19,21,23,25,27,30,35,40,50,75,100,200,300,400,500,750,1000};

    for(int i = 0; i < 0; i++)
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

    cout << "\n\n\n Fixed Array Size of 10 Million, Fixed S at 10\n";

    cout << "best start \n";
    optimal(10, 10);
    cout << "worst start \n";
    worst(10, 10);
    
    for(int i = 0; i<0; i++)
    {
        tuple<int,int> compResult = comparison(10000000, 10);
        resultHybrid.push_back(get<0>(compResult));
        resultMerge.push_back(get<1>(compResult));
        cout << "\n";
    }
    
    
    return 0;
}*/