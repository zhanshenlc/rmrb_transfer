#include <iostream>
#include <ctime>
#include "remover.cpp"
#include "split_three.cpp"
#include "extract_pos.cpp"
#include "extract_ner.cpp"
#include "add_same_pos.cpp"
#include "change_pos.cpp"
#include "ne_confusion.cpp"
#include "pos_confusion.cpp"

using namespace std;


int main(int argc, char* argv[]) {
    clock_t start = clock(), finish;

    string mode = argv[1];
    if (mode == "--r-s" || mode == "--r-i" || mode == "--r-p") {
        remover rm;
        int i = rm.remove_main(argc, argv);
        if (i == -1) { return -1; }
    } else if (mode == "--st") {
        split_three st;
        int i = st.split_three_main(argc, argv);
        if (i == -1) { return -1; }
    } else if (mode == "--ep-n" || mode == "--ep-y") {
        extract_pos ep;
        int i = ep.extract_pos_main(argc, argv);
        if (i == -1) { return -1; }
    } else if (mode == "--en") {
        extract_ner en;
        int i = en.extract_ner_main(argc, argv);
        if (i == -1) { return -1; }
    } else if (mode == "--asp") {
        add_same_pos asp;
        int i = asp.add_same_pos_main(argc, argv);
        if (i == -1) { return -1; }
    } else if (mode == "--cp-f" || mode == "--cp-s") {
        change_pos cp;
        int i = cp.change_pos_main(argc, argv);
        if (i == -1) { return -1; }
    } else if (mode == "--nc") {
        ne_confusion nc;
        int i = nc.ne_conf_main(argc, argv);
        if (i == -1) { return -1; }
    } else if (mode == "--pc") {
        pos_confusion pc;
        int i = pc.pos_conf_main(argc, argv);
        if (i == -1) { return -1;}
    } else { cerr << "Wrong mode" << endl; return -1; }

    finish = clock();
    cout << "Time used:\t" << (double) (start - finish) / CLOCKS_PER_SEC << "seconds" << endl;
    return 0;
}