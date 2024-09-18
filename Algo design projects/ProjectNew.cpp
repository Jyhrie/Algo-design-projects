#include "ProjectNew.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

/********************************************************** SUB FUNCTIONS **************************************************************/
void insertionSort(vector<int>& arr, int start, int end);                                       //insertion sort function           
void merge(vector<int>& arr, int left, int mid, int right);                                     //merge function for merge sort
void mergeSort(vector<int>& arr, int left, int right);                                          //merge sort function                         
void hybridSort(vector<int>& arr, int left, int right, int thresh);                             //hybrid sort function
void printVector(vector<int>& arr);                                                             //print vector function
vector<int> generate(int n, int seed);                                                          //generate random values                          
bool isSorted(const std::vector<int>& arr);                                                     //check if array is sorted                   
tuple<int,int, int, int> compare(int n, int thresh, int test, int enable, int seed);            //compare function for merge and hybrid sort            
void writecsv(const std::vector<int>& vec, const std::string& filename);                        //write to csv function

/********************************************************** GLOBAL VARIABLE **************************************************************/
int keyComparison = 0;


/********************************************************** INSERTION SORT **************************************************************/
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
            else break;     //correct position was found, break out of loop
        }
    }
}

/********************************************************** MERGE SORT **************************************************************/
// Function to merge the two half into a sorted data
void mergeSort(vector<int>& arr, int left, int right)
{
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);          // Sort left half
    mergeSort(arr, mid + 1, right);     // Sort right half
    merge(arr, left, mid, right);       // Merge the sorted halves
}

void merge(vector<int>& arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp vectors
    vector<int> L(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;

    // Merge the left and right subarrays to main array
    while (i < n1 && j < n2) 
    {
        keyComparison++;
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    // Copy remaining elements (if any) from L[] or R[], 
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}



/********************************************************** HYBRID SORT **************************************************************/
// Function to sort an array using hybrid sort
void hybridSort(vector<int>& arr, int left, int right, int thresh)
{
    if (left >= right) return;//check arr length of 0.
    
    int mid = left + (right - left) / 2; //get mid value of array
    
    if(right-left+1 <= thresh) //if length of array is <= S value, use insertionsort algorithm
    {
        insertionSort(arr, left, right);
    }
    else  //use mergesort algorithm
    {
        hybridSort(arr, left, mid,thresh);
        hybridSort(arr, mid + 1, right,thresh);
        merge(arr, left, mid, right);
    }
}

/********************************************************** PRINT VECTOR FUNCTION **************************************************************/
// Function to print a vector
void printVector(vector<int>& arr)
{
    for (int i = 0; i < arr.size(); i++)
        cout << arr[i] << " ";
    cout << endl;
}

/********************************************************** GENERATE RANDOM VALUE **************************************************************/
// Function to generate random values
vector<int> generate(int n, int seed) {
    std::vector<int> numbers;
    numbers.reserve(n);
    
    int min = 1000;
    int max = 10000000;

    if(seed != 0){ srand(seed); }//if seed is not 0, use set seed
    else{ srand(static_cast<unsigned>(time(0)));}

    for (int i = 0; i < n; ++i) {
        int randomNumber = min + (rand() % (max-min +1)); //range from 1000 to 10000000
        numbers.push_back(randomNumber);
    }
    
    return numbers;
}

/********************************************************** CHECK IS SORTED **************************************************************/
// Function to check if an array is sorted
bool isSorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1]) {
            //cout << arr[i] << "<" << arr[i-1] << "\n";
           return false;
        }
    }
    return true;
}

/********************************************************** COMPARE FUNCTION **************************************************************/
// Function to compare merge and hybrid sort
tuple<int,int, int, int> compare(int n, int thresh, int test, int enable, int seed)
{
    vector<int> arr;
    vector<int> dupe;

    int hybridResult = 0;
    int mergeResult = 0;

    int mergeTime = 0;
    int hybridTime = 0;

    switch(test)
    {
        case -1:
            for(int i = n ; i >= 1 ; i--){
                arr.push_back(i);
            }            
            break;
        case 0:
            arr = generate(n, seed);
            break;
        case 1:
            for(int i = 1; i <= n ; i++){
                arr.push_back(i);
            }
            break;
    }    

    dupe = arr;

    if(enable == 0)
    {
        auto mergeStart = std::chrono::high_resolution_clock::now();
        mergeSort(arr, 0, n-1);
        auto mergeStop = std::chrono::high_resolution_clock::now();
        mergeTime = std::chrono::duration_cast<std::chrono::milliseconds>(mergeStop - mergeStart).count();
        mergeResult = keyComparison;
        // cout << "merge " << mergeResult << " (time taken :" << mergeTime << ") | " ;
        
    
        keyComparison = 0; //reset keycomparison
        auto hybridStart = std::chrono::high_resolution_clock::now();
        hybridSort(dupe, 0, n-1, thresh);
        auto hybridStop = std::chrono::high_resolution_clock::now();
        hybridTime = std::chrono::duration_cast<std::chrono::milliseconds>(hybridStop - hybridStart).count();
        hybridResult = keyComparison;
        // cout << "hybrid " << hybridResult <<  " (time taken : " << hybridTime << ") ";

        if(!isSorted(arr))
        {
            cout << " merge failed to sort (enable = 0)\n";
        }
        if(!isSorted(dupe))
        {
            cout << " hybrid failed to sort \n";
        }
        
        // cout << " | Key comparison diff: " << hybridResult - mergeResult << "| Time diff: " << hybridTime - mergeTime << "\n";
    }
    else if(enable == 1)
    {
        mergeSort(arr, 0, n-1);
        cout << "merge " << keyComparison << " | ";
        mergeResult = keyComparison;
        
        if(!isSorted(arr))
        {
            cout << " merge failed to sort (enable = 1)\n";
        }
    }
    else if(enable == 2)
    {
        auto hybridStart = std::chrono::high_resolution_clock::now();
        hybridSort(dupe, 0, n-1, thresh);
        auto hybridStop = std::chrono::high_resolution_clock::now();
        hybridTime = std::chrono::duration_cast<std::chrono::milliseconds>(hybridStop - hybridStart).count();
        hybridResult = keyComparison;
        cout << "hybrid " << hybridResult << " time : " << hybridTime  << " \n";
        
        if(!isSorted(dupe))
        {
            cout << " hybrid failed to sort (enable = 2)\n";
        }
    }
    keyComparison = 0;
    return make_tuple(mergeResult, hybridResult, mergeTime, hybridTime);
}

/********************************************************** WRITE CSV FUNCTION **************************************************************/
// Function to write to a csv file
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

/********************************************************** MAIN FUNCTION **************************************************************/
// Driver code
int main()
{

    vector<int> tempArr;// = generate(20);//{15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    
    for(int i = 1; i < 500; i+=2)
    {
        tempArr.push_back(i);
    }

    int n = 1000000; //set n value

    cout << "\n=================== HYBRID SORT TIMING =======================\n";
    cout << "Calculate the time taken for hybrid sort\n";
    cout << "Best Case | ";
    compare(n, 10, 1, 2, 6);
    cout << "Worst Case | ";
    compare(n, 10, -1, 2, 6);
    cout << "================================================================\n\n";
    
    vector<int> hybridres;
    vector<int> hybridtime;
    vector<int> mergeres;
    vector<int> mergetime;
    writecsv(tempArr, "svalues.csv");
/*
    //part c(i): fixe s value and change n value
    cout << "========================================= PART C (i) =============================================\n";
    vector<int> nvalues;
    int correlation = 0;
    for(int i = 100; i <= 100000; i+=100)
    {
        cout << "n value: " << i << " | ";
        tuple<int,int, int,int>cmpres = compare(i, 12, 0, 0, 0);
        //n: data points
        //thresh: s value
        //test: 0 for both merge and hybrid, 1 for only merge, 2 for only hybrid.
        //enable: 0 to generate random values based on seed, 1 for best case, -1 for worst case
        //seed: 0 to use random seed per instance of compare, not 0 for fixed seed
        
        mergeres.push_back(get<0>(cmpres));
        hybridres.push_back(get<1>(cmpres));
        mergetime.push_back(get<2>(cmpres));
        hybridtime.push_back(get<3>(cmpres));
        
        nvalues.push_back(i);
        int keycomp = get<1>(cmpres) - get<0>(cmpres);
        int time = get<3>(cmpres)  - get<2>(cmpres);

        cout << "key comparison diff: " << keycomp << " | Time diff: " << time << "\n";
    }
    cout << "==================================================================================================\n\n";

    writecsv(hybridres, "Data Excel File/Part C(i)_hybridres.csv");
    writecsv(hybridtime, "Data Excel File/Part C(i)_hybridtime.csv");
    writecsv(mergeres, "Data Excel File/Part C(i)_mergeres.csv");
    writecsv(mergetime, "Data Excel File/Part C(i)_mergetime.csv");
    writecsv(nvalues, "Data Excel File/Part C(i)_nvalues.csv");
*//*
    // part c(ii): fix n value and change s value
    cout << "========================================= PART C (ii) =============================================\n";
    vector<int> svalues;
    vector<pair<int, int>> best_s_values; // To store <S value, time difference>
    for (int j = 0 ; j < 10 ; j++){
        for(int i = 15; i <= 30; i++) //after running for 5 times from 0 -100, we conclude the best range is within 10 to 55
        {   
            // cout << "s value: " << i << " | ";
            tuple<int,int, int,int>cmpres = compare(n, i, 0, 0, 1);
            //n: data points
            //thresh: s value
            //test: 0 for both merge and hybrid, 1 for only merge, 2 for only hybrid.
            //enable: 0 to generate random values based on seed, 1 for best case, -1 for worst case
            //seed: set the seed value so that the random values are the same for each s value (fair result)
            mergeres.push_back(get<0>(cmpres));
            hybridres.push_back(get<1>(cmpres));
            mergetime.push_back(get<2>(cmpres));
            hybridtime.push_back(get<3>(cmpres));
            
            svalues.push_back(i);
            int time = get<3>(cmpres)  - get<2>(cmpres);

            best_s_values.push_back(make_pair(i, time));
  
        //    cout << "key comparison diff: " << get<1>(cmpres) - get<0>(cmpres) << " | Time diff: " << time << "\n";
        }
        // Sort the vector based on time difference (second value of the pair)
        sort(best_s_values.begin(), best_s_values.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second < b.second; // Sort by time difference (ascending)
        });
    }
        // Display the top 10 best S values
        cout << "\nTop 10 best S values based on time difference:\n";
        for (int i = 0; i < 10 && i < best_s_values.size(); ++i) {
            cout << "S value: " << best_s_values[i].first << " | Time diff: " << best_s_values[i].second << "\n";
        }
            cout << "\n";
    cout << "==================================================================================================\n\n";
    // based on the output, we can see that the best range for s value is between 10 to 55. 
    // based on the output, we also can see at certain threshold, hybrid sort is faster than merge sort.
    // after running for 5 times the output is consistent, the best s value is 

    writecsv(hybridres, "Data Excel File/Part C(ii)_hybridres.csv");
    writecsv(hybridtime, "Data Excel File/Part C(ii)_hybridtime.csv");
    writecsv(mergeres, "Data Excel File/Part C(ii)_mergeres.csv");
    writecsv(mergetime, "Data Excel File/Part C(ii)_mergetime.csv");
    writecsv(svalues, "Data Excel File/Part C(ii)_svalues.csv");

    // part c(iii): change n value and change s value
    cout << "========================================= PART C (iii) =============================================\n";
    vector<pair<int, int>> best_s_values; // To store <S value, time difference>
    for (int j = 100 ; j < 1000 ; j+=100){ 
        for(int i = 10; i <= 55; i++) //after running for 5 times from 0 -100, we conclude the best range is within 10 to 55
        {   
            // cout << "s value: " << i << " | ";
            tuple<int,int, int,int>cmpres = compare(n, i, 0, 0, 1);
            //n: data points
            //thresh: s value
            //test: 0 for both merge and hybrid, 1 for only merge, 2 for only hybrid.
            //enable: 0 to generate random values based on seed, 1 for best case, -1 for worst case
            //seed: set the seed value so that the random values are the same for each s value (fair result)
            mergeres.push_back(get<0>(cmpres));
            hybridres.push_back(get<1>(cmpres));
            mergetime.push_back(get<2>(cmpres));
            hybridtime.push_back(get<3>(cmpres));
            bestOpNvalues.push_back(i);
            bestOpSvalues.push_back(j);
            int time = get<3>(cmpres)  - get<2>(cmpres);

            best_s_values.push_back(make_pair(i, time));
            
        });
        }
        sort(best_s_values.begin(), best_s_values.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second < b.second; // Sort by time difference (ascending) });
    }
    cout << "Top 10 best S values based on time difference:\n";
    for (int i = 0; i < 10 && i < best_s_values.size(); ++i) {
        cout << "S value: " << best_s_values[i].first << " | Time diff: " << best_s_values[i].second << "\n\n";
    }
    cout << "==================================================================================================\n\n";

    writecsv(hybridres, "Data Excel File/Part C(iii)_hybridres.csv");
    writecsv(hybridtime, "Data Excel File/Part C(iii)_hybridtime.csv");
    writecsv(mergeres, "Data Excel File/Part C(iii)_mergeres.csv");
    writecsv(mergetime, "Data Excel File/Part C(iii)_mergetime.csv");
    writecsv(bestOpNvalues, "Data Excel File/Part C(iii)_bestOpNvalues.csv");
    writecsv(bestOpSvalues, "Data Excel File/Part C(iii)_bestOpSvalues.csv");
*/
    // part d: compare merge and hybrid sort with 10 million data points
    cout << "========================================= PART D =============================================\n";
    vector<int> d_nvalues;
    for (int i = 15; i <= 40; i++)
        {
        cout << "s value: " << i << " | ";
        tuple<int,int, int,int>cmpres = compare(n, i, 0, 0, 1);
        //n: data points
        //thresh: s value
        //test: 0 for both merge and hybrid, 1 for only merge, 2 for only hybrid.
        //enable: 0 to generate random values based on seed, 1 for best case, -1 for worst case
        //seed: 0 to use random seed per instance of compare, not 0 for fixed seed
            
        mergeres.push_back(get<0>(cmpres));
        hybridres.push_back(get<1>(cmpres));
        mergetime.push_back(get<2>(cmpres));
        hybridtime.push_back(get<3>(cmpres));
            
        d_nvalues.push_back(i);
        int keycomp = get<1>(cmpres) - get<0>(cmpres);
        int time = get<3>(cmpres)  - get<2>(cmpres);
        cout << "key comparison diff: " << keycomp << " | Time diff: " << time << "\n";
    }
    cout << "==================================================================================================\n\n";

    writecsv(hybridres, "Data Excel File/Part D_hybridres.csv");
    writecsv(hybridtime, "Data Excel File/Part D_hybridtime.csv");
    writecsv(mergeres, "Data Excel File/Part D_mergeres.csv");
    writecsv(mergetime, "Data Excel File/Part D_mergetime.csv");
    writecsv(d_nvalues, "Data Excel File/Part D_nvalues.csv");

    return 0;
}

//best case for insertion sort [1, 2, 3, 4, ...... n]
//worst case for insertion sort [n, n-1, n-2, n-3, ..... 1]
