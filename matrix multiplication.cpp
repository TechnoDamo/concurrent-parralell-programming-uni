#include <bits/stdc++.h>
#include<thread>
#include<chrono>
using namespace std;

// ordinary matrix multiplication
vector<vector<int>> mulMat(vector<vector<int>> mat1, vector<vector<int>> mat2)
{
    vector<int> v(mat2[0].size(), 0);
    vector<vector<int>> rslt(mat1.size(), v);

    for (int i = 0; i < mat1.size(); i++) {
        for (int j = 0; j < mat2[0].size(); j++) {
            for (int k = 0; k < mat2.size(); k++) {
                rslt[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return rslt;
}

// function for parallel matrix multiplication
void mulMatP(vector<vector<int>> mat1, vector<vector<int>> mat2, vector<vector<int>> &rslt, int pos)
{
    // pos stays for the submatrix of the initial matrix
    int iBeg, iEnd, jBeg, jEnd;
    if (pos == 0) {
        iBeg = 0;
        jBeg = 0;
        iEnd = mat1.size() / 2;
        jEnd = mat2[0].size() / 2;
    } else if (pos == 1) {
        iBeg = mat1.size() / 2;
        jBeg = mat2[0].size() / 2;
        iEnd = mat1.size();
        jEnd = mat2[0].size();
    } else if (pos == 2) {
        iBeg = 0;
        jBeg = mat2[0].size() / 2;
        iEnd = mat1.size() / 2;
        jEnd = mat2[0].size();
    } else if (pos == 3) {
        iBeg = mat1.size() / 2;
        jBeg = 0;
        iEnd = mat1.size();
        jEnd = mat2[0].size() / 2;
    }

    for (int i = iBeg; i < iEnd; i++) {
        for (int j = jBeg; j < jEnd; j++) {
            for (int k = 0; k < mat2.size(); k++) {
                rslt[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

}


// generating square matrix size * size
vector<vector<int>> genMat(int size, int value) {
    vector<int> v(size, value);
    vector<vector<int>> V(size, v);
    return V;
}

// outputting matrix
void showMat(const vector<vector<int>> &v1) {
    cout << endl;
    for (int i = 0; i < v1.size(); i++) {
        for (int j = 0; j < v1[0].size(); j++) {
            cout << v1[i][j] << " ";
        }
        cout << endl;
    }
}



int main()
{
    /*
     * To test that multiplication works properly
    vector<vector<int>> v1 = genMat(5, 1);
    vector<vector<int>> v2 = genMat(5, 1);
    vector<vector<int>> res = mulMat(v1, v2);
    showMat(v1);
    showMat(v2);
    showMat(res);

    * To test that parrallel multiplication works properly
    vector<vector<int>> res = genMat(10, 0);
    vector<vector<int>> v1 = genMat(10, 1);
    vector<vector<int>> v2 = genMat(10, 1);
    thread th1(mulMatP, v1, v2, ref(res), 0);
    thread th2(mulMatP, v1, v2, ref(res), 1);
    thread th3(mulMatP, v1, v2, ref(res), 2);
    thread th4(mulMatP, v1, v2, ref(res), 3);
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    showMat(res);
     */
    for (int i = 4; i < 400000; i *= 2) {
        vector<vector<int>> v1 = genMat(i, 1);
        vector<vector<int>> v2 = genMat(i, 1);
        auto t1 = std::chrono::high_resolution_clock::now();
        vector<vector<int>> res = mulMat(v1, v2);
        auto t2 = std::chrono::high_resolution_clock::now();
        cout << "size " << i << endl;
        cout << "Ordinary multiplication takes:   " << duration_cast<std::chrono::nanoseconds>(t2-t1).count() << endl;
        res = genMat(i, 0);
        t1 = std::chrono::high_resolution_clock::now();
        thread th1(mulMatP, v1, v2, ref(res), 0);
        thread th2(mulMatP, v1, v2, ref(res), 1);
        thread th3(mulMatP, v1, v2, ref(res), 2);
        thread th4(mulMatP, v1, v2, ref(res), 3);
        th1.join();
        th2.join();
        th3.join();
        th4.join();
        t2 = std::chrono::high_resolution_clock::now();
        cout << "Concurrent multiplication takes: " << duration_cast<std::chrono::nanoseconds>(t2-t1).count() << endl;
    }
    return 0;
}

