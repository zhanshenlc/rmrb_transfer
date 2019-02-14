//
// Created by zhanshenlc on 2018/7/8.
//

#include <string>

using namespace std;

class rules {
public:

    string new_pos(string& pos) {
        if (pos == "Ag") {return "a"; }
        if (pos == "a") {return "a"; }
        if (pos == "ad") {return "a"; }
        if (pos == "an") {return "a"; }
        if (pos == "Bg") {return "b"; }
        if (pos == "b") {return "b"; }
        if (pos == "c") {return "c"; }
        if (pos == "Dg") {return "d"; }
        if (pos == "d") {return "d"; }
        if (pos == "e") {return "e"; }

        if (pos == "f") {return "nd"; }
        if (pos == "g") {return "g"; }
        if (pos == "h") {return "h"; }
        if (pos == "i") {return "i"; }
        if (pos == "j") {return "j"; }
        if (pos == "k") {return "k"; }
        if (pos == "l") {return "i"; }
        if (pos == "Mg") {return "m"; }
        if (pos == "m") {return "m"; }
        if (pos == "Ng") {return "n"; }

        if (pos == "n") {return "n"; }
        if (pos == "nr") {return "nh"; }
        if (pos == "ns") {return "ns"; }
        if (pos == "nt") {return "ni"; }
        if (pos == "nx") {return "ws"; }
        if (pos == "nz") {return "nz"; }
        if (pos == "o") {return "o"; }
        if (pos == "p") {return "p"; }
        if (pos == "Qg") {return "q"; }
        if (pos == "q") {return "q"; }

        if (pos == "Rg") {return "r"; }
        if (pos == "r") {return "r"; }
        if (pos == "s") {return "nl"; }
        if (pos == "Tg") {return "nt"; }
        if (pos == "t") {return "nt"; }
        if (pos == "Ug") {return "u"; }
        if (pos == "u") {return "u"; }
        if (pos == "Vg") {return "v"; }
        if (pos == "v") {return "v"; }
        if (pos == "vd") {return "v"; }

        if (pos == "vn") {return "v"; }
        if (pos == "w") {return "wp"; }
        if (pos == "x") {return "x"; }
        if (pos == "Yg") {return "e"; }
        if (pos == "y") {return "e"; }
        if (pos == "z") {return "z"; }

        return "NaH";
    }

    string new_ne (string& ne) {
        if (ne == "ns") { return "Ns"; }
        if (ne == "nt") { return "Ni"; }
        if (ne == "nz") { return "Nz"; }
        if (ne == "nr") { return "Nh"; }
        if (ne == "i" || ne == "l") { return "Nl"; }

        return "NeH";
    }

};
