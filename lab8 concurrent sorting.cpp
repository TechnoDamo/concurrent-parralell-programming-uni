#include <iostream>
#include<vector>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono;

void merge(vector<int> &array, int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    auto* leftArray = new int[subArrayOne],
            * rightArray = new int[subArrayTwo];

    // Copy data to temp arrays leftArray[]
    // and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    // Initial index of first sub-array
    // Initial index of second sub-array
    auto indexOfSubArrayOne = 0,
            indexOfSubArrayTwo = 0;

    // Initial index of merged array
    int indexOfMergedArray = left;

    // Merge the temp arrays back into
    // array[left..right]
    while (indexOfSubArrayOne < subArrayOne &&
           indexOfSubArrayTwo < subArrayTwo)
    {
        if (leftArray[indexOfSubArrayOne] <=
            rightArray[indexOfSubArrayTwo])
        {
            array[indexOfMergedArray] =
                    leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else
        {
            array[indexOfMergedArray] =
                    rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne)
    {
        array[indexOfMergedArray] =
                leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo)
    {
        array[indexOfMergedArray] =
                rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}


void mergeSort(vector<int> &array, int const begin, int const end)
{
    // Returns recursively
    if (begin >= end)
        return;

    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

void printArray(vector<int> v)
{
    for (auto i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

void genV(vector<int>& v, int size) {
    v.resize(size, 0);
    for (int i = 0; i < v.size(); i++) {
        v[i] = v.size() - i;
    }
}

int main()
{
    /*
     * check one thread sorting correctness
    vector<int> v;
    genV(v, 100);
    cout << "Given array is " << endl;
    printArray(v);

    mergeSort(v, 0, v.size() - 1);

    cout << "Sorted array is " << endl;
    printArray(v);


     * check concurrent sorting correctness
    vector<int> v;
    genV(v, 100);
    cout << "Given array is " << endl;
    printArray(v);

    thread first(mergeSort, ref(v), 0, v.size() / 2);
    thread second(mergeSort, ref(v), v.size() / 2, v.size());

    first.join();
    second.join();
    merge(v, 0, v.size() / 2, v.size() - 1);

    cout << "Sorted array is " << endl;
    printArray(v);
    */
    for (int i = 100; i < 40000000; i *= 2) {
        cout << "array of size " << i <<": " << endl;
        vector<int> v;
        genV(v, i);

        auto t1 = high_resolution_clock::now();
        mergeSort(v, 0, v.size());
        auto t2 = high_resolution_clock::now();
        cout << " time of one thread sorting: " << duration_cast<milliseconds>(t2 - t1).count() << endl;

        t1 = high_resolution_clock::now();
        thread first(mergeSort, ref(v), 0, v.size() / 2);
        thread second(mergeSort, ref(v), v.size() / 2, v.size());
        first.join();
        second.join();
        merge(v, 0, v.size() / 2, v.size());
        t2 = high_resolution_clock::now();
        cout << " time of two thread sorting: " << duration_cast<milliseconds>(t2 - t1).count() << endl;


        genV(v, i);
        t1 = high_resolution_clock::now();
        thread one(mergeSort, ref(v), 0, v.size() / 4);
        thread two(mergeSort, ref(v), v.size() / 4, v.size() / 2);
        thread three(mergeSort, ref(v), v.size() / 2, v.size() / 2 + v.size() / 4);
        thread four(mergeSort, ref(v), v.size() / 2 + v.size() / 4, v.size());
        one.join();
        two.join();
        three.join();
        four.join();
        merge(v, 0, v.size() / 4, v.size() / 2);
        merge(v, v.size() / 2, v.size() / 2 + v.size() / 4, v.size());
        merge(v, 0, v.size() / 2, v.size());
        t2 = high_resolution_clock::now();
        cout << " time of four thread sorting: " << duration_cast<milliseconds>(t2 - t1).count() << endl;
        cout << endl;
    }

    return 0;
}
