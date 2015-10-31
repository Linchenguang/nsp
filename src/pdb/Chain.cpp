#include "Chain.h"

using namespace jian;

Chain::Chain() {
}

Chain::Chain(MolFile &pdb_file) {
    if (!pdb_file.eof()) {
        name = pdb_file.chain_name();
        int model_num = pdb_file.model_num();
        while (!pdb_file.eof() && model_num == pdb_file.model_num() && name == pdb_file.chain_name()) {
            residues.push_back(Residue(pdb_file));
        }
    }
}

Chain::Chain(vector<string> &lines, string type) {
    /// Set name
    name = "";
    name += lines[0][21];

    /// set residues
    vector<string> strings;
    for (int i = 0; i < (int) lines.size(); i++) {
        if (!strings.empty() && lines[i].substr(22, 5) != strings.back().substr(22, 5)) {
            Residue res(strings);
            if (boost::to_upper_copy(type) == "RNA") {
                if (set<string>{"A", "U", "G", "C"}.count(res.name)) {
                    residues.push_back(Residue(strings));
                }
            } else if (boost::to_upper_copy(type) == "DNA") {
                if (set<string>{"DA", "DT", "DG", "DC"}.count(res.name)) {
                    residues.push_back(Residue(strings));
                }
            } else {
                residues.push_back(Residue(strings));
            }
            strings.clear();
        }
        strings.push_back(lines[i]);
    }
    Residue res(strings);
    if (boost::to_upper_copy(type) == "RNA") {
        if (set<string>{"A", "U", "G", "C"}.count(res.name)) {
            residues.push_back(Residue(strings));
        }
    } else if (boost::to_upper_copy(type) == "DNA") {
        if (set<string>{"DA", "DT", "DG", "DC"}.count(res.name)) {
            residues.push_back(Residue(strings));
        }
    } else {
        residues.push_back(Residue(strings));
    }
    strings.clear();
}

int Chain::atom_nums() {
    int num = 0;
    for (auto &&residue: residues) {
        for (auto &&atom: residue) {
            num++;
        }
    }
    return num;
}

void Chain::push(Residue *residue) {
    residues.push_back(*residue);
}

void Chain::push(Residue &residue) {
    residues.push_back(residue);
}

Residue &Chain::operator [](int n) {
    return residues[n];
}

const Residue &Chain::operator [](int n) const {
    return residues[n];
}

vector<Residue>::iterator Chain::begin() {
    return residues.begin();
}

vector<Residue>::iterator Chain::end() {
    return residues.end();
}


