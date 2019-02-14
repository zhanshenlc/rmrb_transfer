//
// Created by zhanshenlc on 2018/7/15.
//
#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <tuple>
#include "rules.cpp"

using namespace std;
using namespace boost;


class extract_pos {
private:

    rules r;

    tuple<string, string> get_w_p (string token) {
        string word, pos;
        int index = 0;
        while (index != token.length()) {
            char ch = token[index++];
            if (ch == '/') { break; }
            word.push_back(ch);
        }
        while (index != token.length()) {
            char ch = token[index++];
            if (ch < 'A' || (ch > 'Z' && ch < 'a') || ch > 'z') { break; }
            pos.push_back(ch);
        }
        tuple<string, string> tup(word, pos);
        return tup;
    };

public:

    string extract_pos_line (string& line) {
        char_separator<char> sep("\t");
        tokenizer<char_separator<char>> tok(line, sep);
        string result;
        for (tokenizer<char_separator<char>>::iterator i = tok.begin(); i != tok.end(); ++ i) {
            tuple<string, string> tup = get_w_p(*i);
            string word = get<0>(tup), pos = get<1>(tup);
            if (word.find('[') != string::npos) { word.erase(0, 1); }
            result += word + "_" += pos + "\t";
        } return result;
    }

    string extract_pos_line_transfer (string& line) {
        char_separator<char> sep("\t");
        tokenizer<char_separator<char>> tok(line, sep);
        string result;
        for (tokenizer<char_separator<char>>::iterator i = tok.begin(); i != tok.end(); ++ i) {
            tuple<string, string> tup = get_w_p(*i);
            string word = get<0>(tup), pos = get<1>(tup);
            if (word.find('[') != string::npos) { word.erase(0, 1); }
            result += word + "_" += r.new_pos(pos) + "\t";
        }
    }

    int extract_pos_file (string& infile, string& outfile, char mode) {
        fstream fin, out;
        fin.open(infile, ios::in);
        out.open(outfile, ios::out);

        string tmp;
        switch (mode) {
            case 'n': {
                while (getline(fin, tmp)) {
                    out << extract_pos_line(tmp) << endl;
                } break;
            }
            case 'y': {
                while (getline(fin, tmp)) {
                    out << extract_pos_line_transfer(tmp) << endl;
                }
            }
            default: { return -1; }
        } return 0;
    }

    int extract_pos_main (int argc, char* argv[]) {
        if (argc != 4) { cerr << "Wrong input number" << endl; return -1;}
        string mode = argv[1], infile = argv[2], outfile = argv[3];
        char ch = '?';
        if (mode == "--ep-n") { ch = 'n'; }
        if (mode == "--ep-y") { ch = 'y'; }
        return extract_pos_file(infile, outfile, ch);
    }

};
