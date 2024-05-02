#include <iostream>
#include<vector>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <set>
#include <omp.h>
#include<chrono>
using namespace std;

// whether two matrices are equal or not
bool compMatr(vector<vector<int>> M1, vector<vector<int>> M2) {
    for (int i = 0; i < M1.size(); i++) {
        for (int j = 0; j < M1.size(); j++) {
            if (M1[i][j] != M2[i][j]) return false;
        }
    }
    return true;
}

// outputting adjacency matrix of a graph
void showAdjMatr(const vector<vector<int>>& adjMatr) {
    for (const auto& i: adjMatr) {
        for (auto j: i) {
            cout << j << " ";
        }
        cout << endl;
    }
}

// generating certain amount of nodes in a given range
set<set<int>> genNodes(int l, int r, int n) {
    r--;
    srand(static_cast<unsigned>(time(nullptr))); // Providing a seed value
    set<set<int>> s;
    while (s.size() < n) {
        while (true) {
            set<int> p;
            p.insert(l + (rand() % (r - l + 1)));
            p.insert(l + (rand() % (r - l + 1)));
            if (p.size() == 2 && s.find(p) == s.end()) {
                s.insert(p);
                break;
            }
        }
    }
    return s;
}

// generating a graph
vector<vector<int>> genGraph(int amountOfEdges, int amountOfNodes) {
    //cout << "started generating graph " << endl;
    vector<int> v(amountOfNodes, 0);
    vector<vector<int>> adjMatr(amountOfNodes, v);
    set<set<int>> s = genNodes(0, amountOfNodes, amountOfEdges);
    for (const auto& i: s) {
        auto it1 = i.begin();
        auto it2 = ++i.begin();
        adjMatr[*it1][*it2] = 1;
        adjMatr[*it2][*it1] = 1;
    }
    //cout << "finished generating graph " << endl;
    return adjMatr;
}

// permutation of rows
void permRows(vector<vector<int>> &adjMatr, int row1Ind, int row2Ind) {
    vector<int> temp(adjMatr.size(), 0);
    for (int i = 0; i < adjMatr.size(); i++) {
        temp[i] = adjMatr[row1Ind][i];
    }
    adjMatr[row1Ind] = adjMatr[row2Ind];
    adjMatr[row2Ind] = temp;
}


// permutation of columns
void permColumns(vector<vector<int>> &adjMatr, int col1Ind, int col2Ind) {
    vector<int> temp(adjMatr.size(), 0);
    for (int i = 0; i < adjMatr.size(); i++) {
        temp[i] = adjMatr[i][col1Ind];
    }
    for (auto & i : adjMatr) {
        i[col1Ind] = i[col2Ind];
    }
    for (int i = 0; i < adjMatr.size(); i++) {
        adjMatr[i][col2Ind] = temp[i];
    }
}

// generating an isomorphic copy of a graph
void genIso(vector<vector<int>> &adjMatr) {
    //cout << "started constructing isomorphic graph" << endl;
    srand(static_cast<unsigned>(time(nullptr))); // Providing a seed value
    // generating random amounts of rows and columns permutations
    int p = 1 + (rand() % (10 - 1 + 1));
    for (int i = 0; i < p; i++) {
        int rc1 = (rand() % (adjMatr.size()));

        int rc2 = (rand() % (adjMatr.size()));

        //cout << "rows/columns: " << rc1 << " " << rc2 << endl;
        permRows(adjMatr, rc1, rc2);
        permColumns(adjMatr, rc1, rc2);
        //showAdjMatr(adjMatr);
        //cout << endl;
    }
    //cout << "finished constructing isomorphic graph " << endl;
    return;
}
bool res = false;
void checkIso(vector<vector<int>> adjMatr, const vector<vector<int>>& adjMatrTest, long long k) {
    if (k >= adjMatr.size()) return;
    if (res) return;
    for (int i = 0; i < adjMatr.size(); i++) {
        for (int j = i; j < adjMatr.size(); j++) {
            std::vector<std::vector<int>> tempMatr = adjMatr;  // Create a temporary copy of the matrix for each thread
            permRows(tempMatr, i, j);
            permColumns(tempMatr, i, j);
            if (compMatr(tempMatr, adjMatrTest)) {
                {
                    res = true;
                }
            }
            checkIso(tempMatr, adjMatrTest, k + 1);
        }
    }
}
void checkIsoP(std::vector<std::vector<int>> adjMatr, const std::vector<std::vector<int>>& adjMatrTest, long long k, bool& res) {
    if (k >= adjMatr.size()) return;
    if (res) return;
#pragma omp parallel for collapse(2) shared(adjMatr, adjMatrTest, res)
    for (int i = 0; i < adjMatr.size(); i++) {
        for (int j = i; j < adjMatr.size(); j++) {
            std::vector<std::vector<int>> tempMatr = adjMatr;  // Create a temporary copy of the matrix for each thread
            permRows(tempMatr, i, j);
            permColumns(tempMatr, i, j);
            if (compMatr(tempMatr, adjMatrTest)) {
            #pragma omp critical
                {
                    res = true;
                }
            }
            checkIsoP(tempMatr, adjMatrTest, k + 1, res);
        }
    }
}

int main() {
    for (int i = 3; i < 30; i++) {
        cout << "n = " << i << endl;
        vector<vector<int>> adjMatr = genGraph(i, i);
        vector<vector<int>> adjMatrTest = adjMatr;
        genIso(adjMatrTest);

        long long t = 0, tp = 0;

        for (int c = 0; c < 100; c++) {
            vector<vector<int>> adjMatr = genGraph(i, i);
            vector<vector<int>> adjMatrTest = adjMatr;
            genIso(adjMatrTest);
            auto t0 = std::chrono::high_resolution_clock::now();
            checkIso(adjMatr, adjMatrTest, 0);
            auto t1 = std::chrono::high_resolution_clock::now();
            t += duration_cast<std::chrono::milliseconds>(t1-t0).count();
            res = false;
        }
        cout <<  "average time: " << t / 100 << endl;


        for (int c = 0; c < 100; c++) {
            vector<vector<int>> adjMatr = genGraph(i, i);
            vector<vector<int>> adjMatrTest = adjMatr;
            genIso(adjMatrTest);
            auto t0 = std::chrono::high_resolution_clock::now();
            checkIsoP(adjMatr, adjMatrTest, 0, res);
            auto t1 = std::chrono::high_resolution_clock::now();
            tp += duration_cast<std::chrono::milliseconds>(t1-t0).count();
            res = false;
        }
        cout <<  "average concurrent time: " << tp / 100 << endl;
        cout << endl;
    }

    return 0;
}
