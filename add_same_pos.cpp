//
// Created by zhans on 2018/7/15.
//
#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <tuple>

using namespace std;
using namespace boost;


class add_same_pos {
public:

    string add_same_pos_line (string& pos, string& line) {
        char_separator<char> sep("\t");
        tokenizer<char_separator<char>> tok(line, sep);
        string result;
        for (tokenizer<char_separator<char>>::iterator i = tok.begin(); i != tok.end(); ++ i) {
            result += *i + "_" + pos + "\t";
        } return result;
    }

    void add_same_file (string& pos, string& infile, string& outfile) {
        fstream fin, out;
        fin.open(infile, ios::in);
        out.open(outfile, ios::out);

        string tmp;
        while (getline(fin, tmp)) { out << add_same_pos_line(pos, tmp) << endl; }
    }

    int add_same_pos_main (int argc, char* argv[]) {
        if (argc != 5) { cerr << "Wrong input number" << endl; return -1; }
        string pos = argv[2], infile = argv[3], outfile = argv[4];
        add_same_file(pos, infile, outfile);
        return 0;
    }

};
