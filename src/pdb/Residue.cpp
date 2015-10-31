#include "Residue.h"

namespace jian {

Residue::Residue() {
}

Residue::Residue(MolFile &pdb_file) {
    if (!pdb_file.eof()) {
        name = pdb_file.res_name();
        num = pdb_file.res_num();
        int model_num = pdb_file.model_num();
        std::string chain_name = pdb_file.chain_name();
        while (!pdb_file.eof() && model_num == pdb_file.model_num() && chain_name == pdb_file.chain_name() && num == pdb_file.res_num() && name == pdb_file.res_name()) {
            atoms.push_back(Atom(pdb_file));
        }
    }
}

Residue::Residue(vector<string> &strings) {
    name = boost::trim_copy(strings[0].substr(17, 3));

    /* set number and num */
    string str = strings[0].substr(22, 5);
    for (int i = 0; i < (int) str.size(); i++) {
        if (str[i] != ' ') {
            number += str[i];
        }
    }
    num = atoi(strings[0].substr(22, 5).c_str());

    /* set atoms */
    int flag = 0;
    for (int i = 0; i < (int) strings.size(); i++) {
        atoms.push_back(Atom(strings[i]));
    }

    // set atomNum
    atomNum = atoms.size();
}

ostream &operator <<(ostream &output, const Residue &residue) {
    int atom_num = 1;
    output << fixed << setprecision(3);
    for (auto &atom: residue.atoms) {
        output << "ATOM" 
               << setw(7)  << atom_num << "  "
               << left << setw(4)  << atom.name
               << right << setw(3) << residue.name
               << setw(2)  << "A"
               << setw(4)  << 1 
               << setw(12) << atom.x 
               << setw(8)  << atom.y 
               << setw(8)  << atom.z 
               << "\n";
        atom_num++;
    }
    return output;
}

vector<Atom>::iterator Residue::begin() {
    return atoms.begin();
}

vector<Atom>::iterator Residue::end() {
    return atoms.end();
}

Residue::Residue(Point *p, int len, int type) {
    num = 0;
    number = "";
    atomNum = len;
    if (type == 0) {
        name = "A";
        if (len == 22) {
            string temp[] = {"P", "O1P", "O2P", "O5*", "C5*", "C4*", "O4*", "C3*", "O3*", "C2*", "O2*", "C1*", "N9", "C8", "N7", "C5", "C6", "N6", "N1", "C2", "N3", "C4"};
            for (int i = 0; i < len; i++) {
                Atom atom(p[i], name, temp[i], i + 1);
                atoms.push_back(atom);
            }
        } else if (len = 19) {
            string temp[] = {"O5*", "C5*", "C4*", "O4*", "C3*", "O3*", "C2*", "O2*", "C1*", "N9", "C8", "N7", "C5", "C6", "N6", "N1", "C2", "N3", "C4"};
            for (int i = 0; i < len; i++) {
                Atom atom(p[i], name, temp[i], i + 1);
                atoms.push_back(atom);
            }
        }
    } else if (type == 1) {
        name = "U";
        if (len == 20) {
            string temp[] = {"P", "O1P", "O2P", "O5*", "C5*", "C4*", "O4*", "C3*", "O3*", "C2*", "O2*", "C1*", "N1", "C2", "O2", "N3", "C4", "O4", "C5", "C6"};
            for (int i = 0; i < len; i++) {
                Atom atom(p[i], name, temp[i], i + 1);
                atoms.push_back(atom);
            }
        } else if (len == 17) {
            string temp[] = {"O5*", "C5*", "C4*", "O4*", "C3*", "O3*", "C2*", "O2*", "C1*", "N1", "C2", "O2", "N3", "C4", "O4", "C5", "C6"};
            for (int i = 0; i < len; i++) {
                Atom atom(p[i], name, temp[i], i + 1);
                atoms.push_back(atom);
            }
        }
    } else if (type == 2) {
        name = "G";
        if (len == 23) {
            string temp[] = {"P", "O1P", "O2P", "O5*", "C5*", "C4*", "O4*", "C3*", "O3*", "C2*", "O2*", "C1*", "N9", "C8", "N7", "C5", "C6", "O6", "N1", "C2", "N2", "N3", "C4"};
            for (int i = 0; i < len; i++) {
                Atom atom(p[i], name, temp[i], i + 1);
                atoms.push_back(atom);
            }
        } else if (len == 20) {
            string temp[] = {"O5*", "C5*", "C4*", "O4*", "C3*", "O3*", "C2*", "O2*", "C1*", "N9", "C8", "N7", "C5", "C6", "O6", "N1", "C2", "N2", "N3", "C4"};
            for (int i = 0; i < len; i++) {
                Atom atom(p[i], name, temp[i], i + 1);
                atoms.push_back(atom);
            }
        }
    } else {
        name = "C";
        if (len == 20) {
            string temp[] = {"P", "O1P", "O2P", "O5*", "C5*", "C4*", "O4*", "C3*", "O3*", "C2*", "O2*", "C1*", "N1", "C2", "O2", "N3", "C4", "N4", "C5", "C6"};
            for (int i = 0; i < len; i++) {
                Atom atom(p[i], name, temp[i], i + 1);
                atoms.push_back(atom);
            }
        } else if (len == 17) {
            string temp[] = {"O5*", "C5*", "C4*", "O4*", "C3*", "O3*", "C2*", "O2*", "C1*", "N1", "C2", "O2", "N3", "C4", "N4", "C5", "C6"};
            for (int i = 0; i < len; i++) {
                Atom atom(p[i], name, temp[i], i + 1);
                atoms.push_back(atom);
            }
        }
    }
}

Atom &Residue::operator [](int n) {
    return atoms[n];
}

const Atom &Residue::operator [](int n) const {
    return atoms[n];
}

Atom &Residue::operator [](std::string atom_name) {
    auto result = std::find_if(atoms.begin(), atoms.end(), [&](const Atom &atom){return atom.name == atom_name;});
    BOOST_ASSERT(result != atoms.end() && "atom index outside");
    return *result;
}

const Atom &Residue::operator [](std::string atom_name) const {
    auto result = std::find_if(atoms.begin(), atoms.end(), [&](const Atom &atom){return atom.name == atom_name;});
    BOOST_ASSERT(result != atoms.end() && "atom index outside");
    return *result;
}

std::vector<Atom> Residue::operator [](const std::vector<std::string> &atom_names) const {
    std::vector<Atom> atom_list;
    for (auto &&atom: atoms) {
        if (std::count(atom_names.begin(), atom_names.end(), atom.name)) {
            atom_list.push_back(atom);
        }
    }   
    std::sort(atom_list.begin(), atom_list.end(), [&](const Atom &atom1, const Atom &atom2) {
        return std::distance(std::find(atom_names.begin(), atom_names.end(), atom1.name), 
                             std::find(atom_names.begin(), atom_names.end(), atom2.name)) > 0;
    });
    return atom_list;
}

Point *Residue::getBaseMassCenter()
{
    double temp = 0;
    double x = 0;
    double y = 0;
    double z = 0;
    for (int i = 0; i < (int) atoms.size(); i++)
    {
        string name = atoms[i].name;
        if (name == "N9" || name == "C8" || name == "N7" ||
            name == "C5" || name == "C6" || name == "N6" ||
            name == "O6" || name == "N1" || name == "C2" ||
            name == "N2" || name == "N3" || name == "C4" ||
            name == "N1" || name == "O2" || name == "N4" ||
            name == "O4"
            )
        {
            temp += atoms[i].mass;
            x += atoms[i].mass * atoms[i].x;
            y += atoms[i].mass * atoms[i].y;
            z += atoms[i].mass * atoms[i].z;
        }
    }

    Point *p = new Point(x / temp, y / temp, z / temp);
    return p;
}

int Residue::getAtomPos(string str, Point &c)
{
    int i, flag;

    for (flag = 0, i = 0; i < (int) (int) atoms.size(); i++)
    {
        if (str == atoms[i].name)
        {
            flag++;
            break;
        }
    }
    if (flag == 1)
    {
        c.x = atoms[i].x;
        c.y = atoms[i].y;
        c.z = atoms[i].z;
        return 0;
    }
    else
    {
        return -1;
    }
}

Point *Residue::getBaseVec() {
    Matr_ *a, *b, *c;
    Point *p;
    double x, y, z, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, x5, y5, z5;
    double r;
    int k;

    for (k = 0; k < (int) atoms.size(); k++) {
        name = atoms[k].name;
        if (name == "C2") {
            x1 = atoms[k].x;
            y1 = atoms[k].y;
            z1 = atoms[k].z;
        } else if (name == "C4") {
            x2 = atoms[k].x;
            y2 = atoms[k].y;
            z2 = atoms[k].z;
        } else if (name == "C6") {
            x3 = atoms[k].x;
            y3 = atoms[k].y;
            z3 = atoms[k].z;
        } else if (name == "C1*") {
            x4 = atoms[k].x;
            y4 = atoms[k].y;
            z4 = atoms[k].z;
        } else if (name == "C2*") {
            x5 = atoms[k].x;
            y5 = atoms[k].y;
            z5 = atoms[k].z;
        } 
    }

    a = new Matr_(2, 2);
    b = new Matr_(2, 1);
    a->data[0][0] = x1 - x2; a->data[0][1] = y1 - y2;
    a->data[1][0] = x1 - x3; a->data[1][1] = y1 - y3;
    b->data[0][0] = z2 - z1;
    b->data[1][0] = z3 - z1;
    c = a->inverse()->multiply(b);

    x2 = x5 - x4; y2 = y5 - y4; z2 = z5 - z4;
    x1 = c->data[0][0]; y1 = c->data[1][0]; z1 = 1;
    if (x1 * x2 + y1 * y2 + z1 * z2 < 0) {
        x1 = -x1; y1 = -y1; z1 = -1;
    }
    r = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
    x = x1 / r; y = y1 / r; z = z1 / r;

    p = new Point(x, y, z);
    return p;
}

int Residue::getAmount() {
    return atoms.size();
}

int Residue::nextTo(Residue &r) {
    Point *p1, *p2;
    for (int i = 0; i < r.atoms.size(); i++) {
        if (r.atoms[i].name == "O3*") {
            p1 = r.atoms[i].coord();
        }
    }
    for (int i = 0; i < atoms.size(); i++) {
        if (atoms[i].name == "O5*") {
            p2 = atoms[i].coord();
        }
    }
    if (p1->dist(p2) < 4) {
        return 1;
    } else {
        return 0;
    }
}

} /// namespace jian
