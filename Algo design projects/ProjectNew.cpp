#include "ProjectNew.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

int keyComparison = 0;


/* Function to sort array using insertion sort */
void insertionSort(vector<int>& arr, int start, int end) //algo sorts array arr using insertionsort from index start to end 
{
    for(int i = start+1; i<=end; i++){ //pick item from slot[]
        for(int j = i; j > start; j--){ //find correct position to insert item
            
            keyComparison++; //key comparison happens even if item is already in correct position;
            if(arr[j] < arr[j-1]) //if arr[j] < previously sorted item
            {
                int temp = arr[j-1]; //swap position with previously sorted item
                arr[j-1] = arr[j];
                arr[j] = temp;
            }
            else //correct position was found, break out of loop
            {
                break;
            }
        }
    }
}

void merge(vector<int>& arr, int left, 
           int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp vectors
    vector<int> L(n1), R(n2);

    // Copy data to temp vectors L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    // Merge the temp vectors back 
    // into arr[left..right]
    while (i < n1 && j < n2) {
        keyComparison++;
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

    // Copy the remaining elements of L[], 
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], 
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// begin is for left index and end is right index
// of the sub-array of arr to be sorted
void mergeSort(vector<int>& arr, int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void hybridSort(vector<int>& arr, int left, int right, int thresh)
{
    if (left >= right) //check arr length of 0.
        return;
    
    int mid = left + (right - left) / 2; //get mid value of array
    
    if(right-left+1 <= thresh) {//if length of array is <= S value, use insertionsort algorithm
        insertionSort(arr, left, right);
    }
    else { //use mergesort algorithm
        hybridSort(arr, left, mid,thresh);
        hybridSort(arr, mid + 1, right,thresh);
        merge(arr, left, mid, right);
    }
}

// Function to print a vector
void printVector(vector<int>& arr)
{
    for (int i = 0; i < arr.size(); i++)
        cout << arr[i] << " ";
    cout << endl;
}

vector<int> generate(int n, int seed) {
    std::vector<int> numbers;
    numbers.reserve(n);
    
    int min = 1000;
    int max = 10000000;

    if(seed != 0){ srand(seed); }//if seed is not 0, use set seed
    else{ srand(static_cast<unsigned>(time(0)));}
    
    for (int i = 0; i < n; ++i) {
        int randomNumber = min + (rand() % (max-min+1)); 
        numbers.push_back(randomNumber);
    }
    
    return numbers;
}

bool isSorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1]) {
            cout << arr[i] << "<" << arr[i-1] << "\n";
           return false;
        }
    }
    return true;
}

tuple<int,int, int, int> compare(int n, int thresh, int test, int enable, int seed)
{
    vector<int> arr;
    vector<int> dupe;
    switch(test)
    {
    case -1:
        for(int i = n ; i >= 1;i--){
            arr.push_back(i);
        }
        dupe = arr;
        break;
    case 0:
        arr = generate(n, seed);
        dupe = arr;
        break;
    case 1:
        for(int i = 1; i < n+1;i++){
            arr.push_back(i);
        }
        dupe = arr;
        break;
    }

    int hybridResult = 0;
    int mergeResult = 0;

    int mergeTime = 0;
    int hybridTime = 0;
    
    if(enable == 0)
    {
        auto mergeStart = std::chrono::high_resolution_clock::now();
        mergeSort(arr, 0, n-1);
        auto mergeStop = std::chrono::high_resolution_clock::now();
        mergeTime = std::chrono::duration_cast<std::chrono::milliseconds>(mergeStop - mergeStart).count();
        cout << "merge " << keyComparison << " time taken :" << mergeTime << " | " ;
        mergeResult = keyComparison;
    
        keyComparison = 0; //reset keycomparison
        auto hybridStart = std::chrono::high_resolution_clock::now();
        hybridSort(dupe, 0, n-1, thresh);
        auto hybridStop = std::chrono::high_resolution_clock::now();
        hybridTime = std::chrono::duration_cast<std::chrono::milliseconds>(hybridStop - hybridStart).count();
        hybridResult = keyComparison;
        cout << "hybrid " << hybridResult <<  " time taken : " << hybridTime ;

        if(!isSorted(arr))
        {
            cout << "merge failed to sort \n";
        }
        if(!isSorted(dupe))
        {
            cout << "hybrid failed to sort \n";
        }
    }
    else if(enable == 1)
    {
        mergeSort(arr, 0, n-1);
        cout << "merge " << keyComparison << " | ";
        mergeResult = keyComparison;
        
        if(!isSorted(arr))
        {
            cout << "merge failed to sort \n";
        }
    }
    else if(enable == 2)
    {
        auto hybridStart = std::chrono::high_resolution_clock::now();
        hybridSort(dupe, 0, n-1, thresh);
        auto hybridStop = std::chrono::high_resolution_clock::now();
        hybridTime = std::chrono::duration_cast<std::chrono::milliseconds>(hybridStop - hybridStart).count();
        hybridResult = keyComparison;
        cout << "hybrid " << hybridResult << "time : " << hybridTime ;
        
        if(!isSorted(dupe))
        {
            cout << "hybrid failed to sort \n";
        }
    }

    cout << " | diff: " << hybridResult - mergeResult <<"\n";
    
    keyComparison = 0;


    return make_tuple(mergeResult, hybridResult, mergeTime, hybridTime);
}

void writecsv(const std::vector<int>& vec, const std::string& filename) {
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

// Driver code
int main()
{

    vector<int> tempArr;// = generate(20);//{15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};

    
    for(int i = 1; i < 500; i+=2)
    {
        tempArr.push_back(i);
    }

    int n = 1000000;
    cout << "Best Case | ";
    compare(n, 10, 1, 2, 6);
    cout << "Worst Case | ";
    compare(n, 10, -1, 2, 6);
    
    vector<int> hybridres;
    vector<int> hybridtime;
    writecsv(tempArr, "svalues.csv");

    vector<int> nvalues;
    for(int i = 50000; i <= 1000000; i+=50000)
    {
        tuple<int,int, int,int>cmpres = compare(i, 12, 0, 0, 6);
        //n: data points
        //thresh: s value
        //test: 0 for both merge and hybrid, 1 for only merge, 2 for only hybrid.
        //enable: 0 to generate random values based on seed, 1 for best case, -1 for worst case
        //seed: 0 to use random seed, not 0 for fixed seed
        hybridres.push_back(get<1>(cmpres));
        hybridtime.push_back(get<3>(cmpres));
        nvalues.push_back(i);
    }
    writecsv(hybridres, "hybridres.csv");
    writecsv(hybridtime, "hybridtime.csv");
    writecsv(nvalues, "nvalues.csv");

    
    return 0;
}

//best case for insertion sort [1, 2, 3, 4, ...... n]
//worst case for insertion sort [n, n-1, n-2, n-3, ..... 1]
