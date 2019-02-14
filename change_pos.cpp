//
// Created by zhans on 2018/7/15.
//
#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <tuple>
#include "rules.cpp"

using namespace std;
using namespace boost;


class change_pos {
private:

    tuple<string, string> get_w_n (string token) {
        string ne;
        do {
            ne.insert(ne.begin(), token[token.length() - 1]);
            token.pop_back();
        } while (token[token.length() - 1] != '/');
        do {
            token.pop_back();
        } while (token[token.length() - 1] != '#');
        token.pop_back();
        tuple<string, string> tup(token, ne);
        return tup;
    }

    string get_pos (string token) {
        string pos;
        do {
            pos.insert(pos.begin(), token[token.length() - 1]);
            token.pop_back();
        } while (token[token.length() - 1] != '_');
        return pos;
    }

public:

    string change_pos_line (string& ner_line, string& pos_line) {
        char_separator<char> sep("\t");
        tokenizer<char_separator<char>> tok_n(ner_line, sep), tok_p(pos_line, sep);
        tokenizer<char_separator<char>>::iterator n = tok_n.begin(), p = tok_p.begin();
        string result;
        while (n != tok_n.end() && p != tok_p.end()) {
            tuple<string, string> tup = get_w_n(*n);
            string word = get<0>(tup), pos = get_pos(*p), ne = get<1>(tup);
            result += word + "/" += pos + "#" += ne + "\t";
            ++ n; ++ p;
        } return result;
    }

    string change_pos_file (string& ner_file, string& pos_file, string& outfile) {
        fstream ner, pos, out;
        ner.open(ner_file, ios::in);
        pos.open(pos_file, ios::in);
        out.open(outfile, ios::out);

        string tmp;
        vector<string> ner_words, poses;
        while (getline(ner, tmp)) { ner_words.push_back(tmp); }
        while (getline(pos, tmp)) { poses.push_back(tmp); }
        for (int i = 0; i != ner_words.size() && i != poses.size(); i++) {
            out << change_pos_line(ner_words[i], poses[i]) << endl;
        }
    }

    string change_pos_line_with (string& pos, string& line) {
        char_separator<char> sep("\t");
        tokenizer<char_separator<char>> tok(line, sep);
        string result;
        for (tokenizer<char_separator<char>>::iterator i = tok.begin(); i != tok.end(); ++ i) {
            tuple<string, string> tup = get_w_n(*i);
            string word = get<0>(tup), ne = get<1>(tup);
            result += word + "/" += pos + "#" += ne + "\t";
        } return result;
    }

    void change_pos_file_with (string& pos, string& infile, string& outfile) {
        fstream fin, out;
        fin.open(infile, ios::in);
        out.open(outfile, ios::out);

        string tmp;
        while (getline(fin, tmp)) {
            out << change_pos_line_with(pos, tmp) << endl;
        }
    }

    int change_pos_main (int argc, char* argv[]) {
        if (argc != 5) { cerr << "Wrong input number" << endl; return -1; }
        string mode = argv[1], nerf_or_pos = argv[2], pos_or_ner = argv[3], outfile = argv[4];
        if (mode == "--cp-f") { change_pos_file(nerf_or_pos, pos_or_ner, outfile); }
        if (mode == "--cp-s") { change_pos_file_with(nerf_or_pos, pos_or_ner, outfile); }
        return 0;
    }

};
