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


class extract_ner {
private:

    rules r;

    bool is_symbol (char ch) { return not isalpha(ch); }

    string get_word (string& token) {
        string word;
        do {
            word.push_back(token[0]);
            token.erase(0, 1);
        } while (token[0] != '/');
        return word;
    }

    tuple<string, string> get_w_p (string& token) {
        string word, pos;
        int index = 0;
        while (index != token.length()) {
            char ch = token[index++];
            if (ch == '/') { break; }
            word.push_back(ch);
        }
        while (index != token.length()) {
            char ch = token[index++];
            if (is_symbol(ch)) { break; }
            pos.push_back(ch);
        }
        tuple<string, string> tup(word, r.new_pos(pos));
        return tup;
    };

    tuple<string, string, string> get_w_p_n (string& token) {
        string word, pos, ne;
        int index = 0;
        while (index != token.length()) {
            char ch = token[index++];
            if (ch == '/') { break; }
            word.push_back(ch);
        }
        while (index != token.length()) {
            char ch = token[index++];
            if (is_symbol(ch)) { break; }
            pos.push_back(ch);
        }
        do {
            ne.insert(ne.begin(), token[token.length() - 1]);
            token.pop_back();
        } while (token[token.length() - 1] != ']');
        tuple<string, string, string> tup(word, r.new_pos(pos), r.new_ne(ne));
        return tup;
    }

public:

    string extract_ner_line (string& line) {
        char_separator<char> sep("\t");
        tokenizer<char_separator<char>> tok(line, sep);
        string result;
        bool inNer = false, the_only_exception= false, has_exception =false;
        vector<string> ner_words;
        for (tokenizer<char_separator<char>>::iterator i = tok.begin(), name; i != tok.end(); ++ i) {
            string token = *i;
            if (token[0] == '[' && token.find(']') != string::npos) {
                token.erase(0, 1);
                tuple<string, string, string> tup = get_w_p_n(token);
                string word = get<0>(tup), pos = get<1>(tup), ne = get<2>(tup);
                result += word + "/" += pos + "#S-" += ne + "\t";
            }
            else if (inNer) {
                if (token[0] == '[') {
                    the_only_exception = true;
                    ner_words.push_back(token);
                } else if (the_only_exception) {
                    if (token.find(']') != string::npos) {
                        ner_words.push_back(token);
                        the_only_exception = false;
                        has_exception = true;
                    } else { ner_words.push_back(token); }
                } else if (token.find(']') != string::npos) {
                    ner_words.push_back(token);
                    result += ner_reform(ner_words, has_exception);
                    has_exception = false;
                    ner_words.clear();
                    inNer = false;
                } else { ner_words.push_back(token); }
            } else if (token[0] == '[') {
                ner_words.push_back(token);
                inNer = true;
            } else if (token.find("nr") != string::npos) {
                name = i + 1;
                if (name == tok.end()) { result + get_word(token) + "/nh#S-Nh\t"; }
                else {
                    string next = *name;
                    if (next.find("nr") != string::npos) {
                        result += get_word(token) + "/nh#B-Nh\t" += get_word(next) + "/nh#E-Nh\t";
                        ++ i;
                    } else { result += get_word(token) + "/nh#S-Nh\t"; }
                }
            } else if (token.find("ns") != string::npos) {
                result += get_word(token) + "/ns#S-Ns\t";
            } else if (token.find("nt") != string::npos) {
                result += get_word(token) + "/ni#S-Ni\t";
            } else if (token.find("nz") != string::npos) {
                result += get_word(token) + "/nz#S-Nz\t";
            } else if (token.find('i') != string::npos || token.find('l') != string::npos) {
                result += get_word(token) + "/i#S-Nl\t";
            } else {
                tuple <string, string> tup = get_w_p(token);
                string word = get<0>(tup), pos = get<1>(tup);
                result += word + "/" += pos + "#O\t";
            }
        } return result;
    }

    string ner_reform (vector<string>& ner_words, bool hasException) {
        string result;
        if (hasException) {
            string first = ner_words[0];
            first.erase(0, 1);
            ner_words.erase(ner_words.begin());
            ner_words.insert(ner_words.begin(), first);
            bool inNer = false;
            vector<string> nested;
            for (int i = 0; i != ner_words.size(); i++) {
                if (ner_words[i][0] == '[') {
                    inNer = true;
                    nested.push_back(ner_words[i]);
                    continue;
                }
                if (inNer) {
                    if (ner_words[i].find(']') != string::npos) {
                        inNer = false;
                        nested.push_back(ner_words[i]);
                        result += ner_reform(nested, false);
                    } else { nested.push_back(ner_words[i]); }
                } else {
                    if (ner_words[i].find("ns") != string::npos) {
                        result += get_word(ner_words[i]) + "/ns#S-Ns\t";
                    } else if (ner_words[i].find("nt") != string::npos) {
                        result += get_word(ner_words[i]) + "/ni#S-Ni\t";
                    } else if (ner_words[i].find("nz") != string::npos) {
                        result += get_word(ner_words[i]) + "/nz#S-Nz\t";
                    } else if (ner_words[i].find('i') != string::npos || ner_words[i].find('l') != string::npos) {
                        result += get_word(ner_words[i]) + "/i#S-Nl\t";
                    } else {
                        tuple<string, string> tup = get_w_p(ner_words[i]);
                        string word = get<0>(tup), pos = get<1>(tup);
                        result += word + "/" += pos + "#O\t";
                    }
                }
            } return result;
        }
        string first = ner_words[0], last = ner_words[ner_words.size() - 1];
        first.erase(0, 1);
        tuple<string, string> tup_f = get_w_p(first);
        tuple<string, string, string> tup_l = get_w_p_n(last);
        string word_f = get<0>(tup_f), pos_f = get<1>(tup_f);
        string word_l = get<0>(tup_l), pos_l = get<1>(tup_l), ne = get<2>(tup_l);
        result += word_f + "/" += pos_f + "#B-" += ne + "\t";
        if (ner_words.size() == 2) { return result += word_l + "/" += pos_l + "#E-" += ne + "\t"; }
        for (int i = 1; i != ner_words.size() - 1; i++) {
            tuple<string, string> tup = get_w_p(ner_words[i]);
            string word =get<0>(tup), pos = get<1>(tup);
            result += word + "/" += pos + "#I-" += ne + "\t";
        }
        return result += word_l + "/" += pos_l + "#E-" += ne + "\t";
    }

    void extract_ner_file (string& infile, string& outfile) {
        fstream fin, out;
        fin.open(infile, ios::in);
        out.open(outfile, ios::out);

        string tmp;
        while (getline(fin, tmp)) {
            out << extract_ner_line(tmp) << endl;
        }
    }

    int extract_ner_main (int argc, char* argv[]) {
        if (argc != 4) { cerr << "Wrong input number"; return -1;}
        string infile = argv[2], outfile = argv[3];
        extract_ner_file(infile, outfile);
        return 0;
    }

};
