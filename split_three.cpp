//
// Created by zhans on 2018/7/15.
//
#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;


class split_three {
public:

    void split_to_three (string& infile, string& train, string& develop, string& test) {
        int pi1[50] = {1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2, 3, 8, 4, 6, 2, 6, 4, 3, 3,
                       8, 3, 2, 7, 9, 5, 0, 2, 8, 8, 4, 1, 9, 7, 1, 6, 9, 3, 9, 9, 3, 7, 5, 1, 0};
        // + 5  4   3   2   1   1   2   3   4   5
        int pi2[50] = {6, 9, 6, 0, 4, 6, 0, 9, 7, 9, 1, 2, 0, 2, 6, 4, 5, 0, 6, 8, 3, 7, 5, 4, 4,
                       9, 4, 3, 8, 0, 7, 2, 4, 0, 0, 7, 4, 2, 0, 4, 0, 3, 7, 3, 3, 8, 2, 0, 6, 5};

        fstream fin, tr, de, te;
        fin.open(infile, ios::in);
        tr.open(train, ios::out);
        de.open(develop, ios::out);
        te.open(test, ios::out);

        string tmp;
        int i = 0;
        while (getline(fin, tmp)) {
            if (tmp.empty()) { continue; }
            int compare1 = pi1[(i / 10) % 50], compare2 = pi2[(i / 10) % 50];
            if (i % 10 == compare1) { de << tmp << endl; }
            if (i % 10 == compare2) { te << tmp << endl; }
            else { tr << tmp << endl; }
            i++;
        }
    }

    int split_three_main (int argc, char* argv[]) {
        if (argc != 6) { cerr << "Wrong input number" << endl; }
        string infile = argv[2], train = argv[3], develop = argv[4], test = argv[5];
        split_to_three(infile, train, develop, test);
        return 0;
    }

};
