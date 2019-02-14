//
// Created by zhanshenlc on 2018/7/15.
//
#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;


class remover {
public:

    string space_2_tab (string& line) {
        char_separator<char> sep("  "); // two spaces
        tokenizer<char_separator<char>> tok(line, sep);
        string result;
        for (tokenizer<char_separator<char>>::iterator i = tok.begin(); i != tok.end(); ++ i) {
            result += *i + "\t";
        } return result;
    }

    string remove_index (string& line) {
        char_separator<char> sep("  "); // two spaces
        tokenizer<char_separator<char>> tok(line, sep);
        string result;
        for (tokenizer<char_separator<char>>::iterator i = ++tok.begin(); i != tok.end(); ++ i) {
            result += *i + "\t";
        } return result;
    }

    string remove_pos (string& line) {
        char_separator<char> sep("  "); // two spaces
        tokenizer<char_separator<char>> tok(line, sep);
        string result;
        for (tokenizer<char_separator<char>>::iterator i = tok.begin(); i != tok.end(); ++ i) {
            string token = *i;
            if (token.find('[') != string::npos) { token.erase(0, 1); }
            string word = get_word(token);
            result += word + "\t";
        } return result;
    }

    string get_word (string& token) {
        string word;
        do {
            word.push_back(token[0]);
            token.erase(0, 1);
        } while (token[0] != '/');
        return word;
    }

    int remove_file (string& infile, string& outfile, char mode) {
        fstream fin, out;
        fin.open(infile, ios::in);
        out.open(outfile, ios::out);

        string tmp;
        switch (mode) {
            case 's': {
                while (getline(fin, tmp)) {
                    if (tmp.empty()) { continue; }
                    out << space_2_tab(tmp) << endl;
                } break;
            }
            case 'i': {
                while (getline(fin, tmp)) {
                    if (tmp.empty()) { continue; }
                    out << remove_index(tmp) << endl;
                } break;
            }
            case 'p': {
                while (getline(fin, tmp)) {
                    if (tmp.empty()) { continue; }
                    out << remove_pos(tmp) << endl;
                }
            }
            default: { return -1; }
        } return 0;
    }

    int remove_main (int argc, char* argv[]) {
        if (argc != 4) { cerr << "Wrong input number" << endl; return -1; }
        string mode = argv[1], infile = argv[2], outfile = argv[3];
        char ch = '?';
        if (mode == "--r-s") { ch = 's'; }
        if (mode == "--r-i") { ch = 'i'; }
        if (mode == "--r-p") { ch = 'p'; }
        return remove_file(infile, outfile, ch);
    }

};
