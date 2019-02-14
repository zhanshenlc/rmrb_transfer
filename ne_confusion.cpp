//
// Created by zhans on 2018/7/27.
//

#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <tuple>

using namespace std;
using namespace boost;


class ne_confusion{
public:

    void ne_conf_file (string& corrfile, string& testfile) {
        fstream corr, test, log, wrong, over;
        corr.open(corrfile, ios::in);
        test.open(testfile, ios::out);
        log.open("log.txt", ios::out);
        wrong.open("wrong.txt", ios::out);
        over.open("over.txt", ios::out);

        string tmp;
        vector<string> cs, ts;
        while (getline(corr, tmp)) { cs.push_back(tmp); }
        while (getline(test, tmp)) { ts.push_back(tmp); }

        int total = 0, correct = 0, recall = 0;
        if (cs.size() != ts.size()) { log << "Warning: Different number of lines." << endl << endl; }
        char_separator<char> sep("\t");
        for (int i = 0; i != cs.size() && i != ts.size(); i ++) {
            string left = cs[i], right = ts[i];
            tokenizer<char_separator<char>> at(left, sep), bt(right, sep);
            tokenizer<char_separator<char>>::iterator a = at.begin(), b = bt.begin();
            string a_long_ne, b_long_ne;
            while (a != at.end()) {
                string a_word = *a, b_word = *b, ne = get_ne(a_word);
                ++a; ++b;
                char a_title = ne[0];
                switch (a_title) {
                    case 'O': { continue; }
                    case 'S': {
                        if (a_word == b_word) { correct ++; }
                        else {
                            wrong << "line:\t" << i + 1 << endl;
                            wrong << "correct:\t" << a_word << endl;
                            wrong << "given  :\t" << b_word << endl;
                            wrong << "correct:\t" << left << endl;
                            wrong << "given  :\t" << right << endl << endl;
                        }
                        total ++;
                        break;
                    }
                    case 'B': {
                        a_long_ne += a_word;
                        b_long_ne += b_word;
                        break;
                    }
                    case 'I': {
                        a_long_ne += a_word;
                        b_long_ne += b_word;
                        break;
                    }
                    case 'E': {
                        a_long_ne += a_word;
                        b_long_ne += b_word;
                        if (a_long_ne == b_long_ne) { correct ++; }
                        else {
                            wrong << "line:\t" << i + 1 << endl;
                            wrong << "correct:\t" << a_long_ne << endl;
                            wrong << "given:  \t" << b_long_ne << endl;
                            wrong << "correct:\t" << left << endl;
                            wrong << "given:  \t" << right << endl << endl;
                        }
                        a_long_ne.clear();
                        b_long_ne.clear();
                        total ++;
                        break;
                    }
                    default: { continue; }
                }
            }
            a = at.begin();
            b = bt.begin();
            while (b != bt.end()) {
                string a_word = *a, b_word = *b, ne = get_ne(b_word);
                char b_title = ne[0];
                switch (b_title) {
                    case 'O': { continue; }
                    case 'S': {
                        if (b_word != a_word) {
                            over << "line:\t" << i + 1 << endl;
                            over << "correct:\t" << a_word << endl;
                            over << "given  :\t" << b_word << endl;
                            over << "correct:\t" << left << endl;
                            over << "given  :\t" << right << endl << endl;
                        }
                        recall ++;
                        break;
                    }
                    case 'B': {
                        a_long_ne += a_word;
                        b_long_ne += b_word;
                        break;
                    }
                    case 'I': {
                        a_long_ne += a_word;
                        b_long_ne += b_word;
                        break;
                    }
                    case 'E': {
                        a_long_ne += a_word;
                        b_long_ne += b_word;
                        if (b_long_ne != a_long_ne) {
                            over << "line:\t" << i + 1 << endl;
                            over << "correct:\t" << a_long_ne << endl;
                            over << "given  :\t" << b_long_ne << endl;
                            over << "correct:\t" << left << endl;
                            over << "given  :\t" << right << endl << endl;
                        }
                        a_long_ne.clear();
                        b_long_ne.clear();
                        recall ++;
                        break;
                    }
                    default: { continue; }
                }
            }
        }
        log << "Correct  :\t" << correct << endl;
        log << "Total    :\t" << total << endl;
        log << "Tagged   :\t" << recall << endl;
        double precision = correct * 1.0 / recall, recall_d = correct * 1.0 / total;
        log << "Precision:\t" << precision << endl;
        log << "Recall   :\t" << recall_d << endl;
        log << "F1-Score :\t" << 2 * precision * recall_d / (precision + recall_d) << endl;
    }

    int ne_conf_main (int argc, char* argv[]) {
        if (argc != 4) { cerr << "Wrong input number" << endl; return -1; }
        string infile = argv[2], outfile = argv[3];
        ne_conf_file(infile, outfile);
        return 0;
    }

    string get_ne (string word) {
        string ne;
        do {
            ne.insert(ne.begin(), word[word.length() - 1]);
            word.pop_back();
        } while (word[word.length() - 1] != '#');
        return ne;
    }

};