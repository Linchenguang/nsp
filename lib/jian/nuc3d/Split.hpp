#pragma once

#include "../pdb.hpp"
#include "../nuc2d.hpp"

namespace jian {

class Split {
public:
    std::string _lib;
    std::string _ss;
    std::string _family = "other";
    std::string _name;
    std::string _type = "RNA";
    std::string _seq;
    Model _mol;
    int _hinge_size = 2;
    int _helix_num = 0;
    int _loop_num = 0;

    Split(Par par) {
        _lib = Env::lib();

        if (par.count("pdb_file")) _name = par["pdb_file"][0];
        if (par.count("pdb")) _name = par["pdb"][0];
        if (par.count("name")) _name = par["name"][0];
        if (par.count("job")) _name = par["job"][0];

        if (par.count("secondary_structure")) _ss = par["secondary_structure"][0];
        if (par.count("ss")) _ss = par["ss"][0];

        if (par.count("library_path")) _lib = par["library_path"][0];
        if (par.count("library")) _lib = par["library"][0];
        if (par.count("lib")) _lib = par["lib"][0];

        if (par.count("family")) _family = par["family"][0];

        if (par.count("type")) _type = boost::to_upper_copy(par["type"][0]);

        if (par.count("hinge")) _hinge_size = boost::lexical_cast<int>(par["hinge"][0]);

        _lib += "/" + _type;

        if (_ss == "") throw "Split::Split error! Please provide the 2D structure";
        if (_name == "") throw "Split::Split error! Please provide the pdb file";

        NucSS::check_ss(_ss);

        _mol = Model(_name);
        _name = _mol.name;
        _seq = jian::seq(_mol);
    }

    void operator ()() {
        SSTree ss_tree; ss_tree.make(_seq, _ss, _hinge_size);
        LOOP_TRAVERSE(ss_tree.head(),
            parse_helix(L);
            parse_loop(L);
        );
    }

    void parse_helix(loop *l) {
        if (l->has_helix()) {
            _helix_num++;

            // write to pdb file
            int len = l->s.len();
            std::string pdb_path = _lib + "/templates/";
            std::string helix_name = _name + "-" + boost::lexical_cast<std::string>(_helix_num) + "-helix-" + boost::lexical_cast<std::string>(len);
            std::vector<int> nums(len * 2);
            int index = 0;
            for (auto b = l->s.head; b != NULL; b = b->next) {
                nums[index] = b->res1.num - 1;
                nums[2 * len - 1 - index] = b->res2.num - 1;
                index++;
            }
            write_pdb(sub(_mol, nums), pdb_path + helix_name + ".pdb");

            // write to records file
            std::string helix_info_file = _lib + "/records/helix";
            std::ofstream ofile(helix_info_file.c_str(), ios::app);
            ofile << helix_name << ' ' << l->s.len() << ' ' << l->s.seq() << ' ' << l->s.ss() << ' ' << _family << endl;
            ofile.close();
        } else {
            // pass
        }
    }

    void parse_loop(loop *l) {
        if (l->has_loop()) {
            _loop_num++;

            int len = l->len();
            auto type = l->hinges.size();
            std::string pdb_path = _lib + "/templates/";
            std::string loop_name = _name + "-" + boost::lexical_cast<std::string>(_loop_num) + "-" + std::string(l->is_open() ? "open_" : "") + "loop-" + boost::lexical_cast<std::string>(type);
            std::vector<int> all_nums, hinge_nums;
            all_nums.reserve(len);
            for (auto r = l->head; r != NULL; r = r->next) all_nums.push_back(r->num - 1);
            hinge_nums.reserve(type * 2 + (l->is_open() ? 0 : 4));
            if (!l->is_open()) append(hinge_nums, l->at(0).num - 1, l->at(1).num - 1);
            for (auto &&pair: l->hinges) append(hinge_nums, l->at(pair.first-1).num - 1, l->at(pair.first).num - 1, 
                                                l->at(pair.second).num - 1, l->at(pair.second+1).num - 1);
            if (!l->is_open()) append(hinge_nums, l->at(len - 2).num - 1, l->at(len - 1).num - 1);
            write_pdb(sub(_mol, all_nums), pdb_path + loop_name + ".pdb");
            write_pdb(sub(_mol, hinge_nums), pdb_path + loop_name + ".hinge.pdb");

            // write to info file
            string l_ss = l->ss();
            std::string loop_info_file = _lib + "/records/" + (l->is_open() ? "open_" : "") + "loop";

            std::ofstream ofile(loop_info_file.c_str(), ios::app);
            ofile << loop_name << ' ' << l->num_sons() << ' ' << l->seq() << ' ' << l->ss() << ' ' << _family << endl;
            ofile.close();

        } else {
            // pass
        }
    }
};

} // namespace jian

