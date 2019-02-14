//
// Created by zhans on 2018/7/27.
//

#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;


class pos_confusion {
public:

    void pos_conf_file (string& ner_file, string& pos_file, string& log_file) {
        fstream ner_f, pos_f, log;
        ner_f.open(ner_file, ios::in);
        pos_f.open(pos_file, ios::in);
        log.open(log_file, ios::out);

        string tmp;
        vector<string> ner_s, poses;
        while (getline(ner_f, tmp)) { ner_s.push_back(tmp); }
        while (getline(pos_f, tmp)) { poses.push_back(tmp); }
        if (ner_s.size() != poses.size()) { log << "Warning: Different number of lines" << endl << endl; }
        int total = 0, correct = 0, wrong = 0;
        char_separator<char> sep("\t");
        for (int i = 0; i != ner_s.size() && i != poses.size(); i ++) {
            string ner_line = ner_s[i], pos_line = poses[i];
            tokenizer<char_separator<char>> nt(ner_line, sep), pt(pos_line, sep);
            tokenizer<char_separator<char>>::iterator n = nt.begin(), p = pt.begin();
            while (n != nt.end() && p != nt.end()) {
                string n_pos = get_pos(*n), pos = get_pos(*p);
                // string n_pos = get_ner_pos(*n),pos = get_pos(*p);
                ++n; ++p;
                if (n_pos == pos) { correct ++; }
                else { wrong ++; }
                total ++;
            }
            if (n != nt.end() || p != pt.end()) {
                log << "Warning: Different number of words at " << i + 1 << endl;
            }
        }
        log << "Total   :\t" << total << endl;
        log << "Correct :\t" << correct << endl;
        log << "Wrong   :\t" << wrong << endl;
        log << "Accuracy:\t" << (double) correct / total << endl;
    }

    int pos_conf_main (int argc, char* argv[]) {
        if (argc != 5) { cerr << "Wrong input number" << endl; return -1; }
        string ner_file = argv[2], pos_file = argv[3], log_file = argv[4];
        pos_conf_file(ner_file, pos_file, log_file);
        return 0;
    }

    string get_pos (string word) {
        string pos;
        do {
            pos.insert(pos.begin(), word[word.length() - 1]);
            word.pop_back();
        } while (word[word.length() - 1] != '_');
        return pos;
    }

    string get_ner_pos (string word) {
        string pos;
        do { word.pop_back(); } while (word[word.length() - 1] != '#');
        word.pop_back();
        do {
            pos.insert(pos.begin(), word[word.length() - 1]);
            word.pop_back();
        } while (word[word.length() - 1] != '/');
        return pos;
    }

};
