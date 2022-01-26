#include "TestUtility.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

bool is_same_file(const string& filename1, const string& filename2) {
    ifstream if1{filename1};
    if(!if1) {
        cout << "Can not open file " << filename1 << endl;
        return false;
    }



    ifstream if2{filename2};
    if (!if2) {
        cout << "Can not open file " << filename2 << endl;
        return false;
    }

    vector<char> v1;
    char c1;
    while (if1.get(c1)) {
        v1.push_back(c1);
    }

    vector<char> v2;
    char c2;
    while (if2.get(c2)) {
        v2.push_back(c2);
    }

    // Test v1 == v2
    if (v1.size() != v2.size()) {
        cout << "File length do not match" << endl;
        return false;
    }

    for (int i = 0; i < v1.size(); i++){
        if (v1[i] != v2[i]) {
            cout << "File content do not match" << endl;
            return false;
        }
    }
    return true;
}