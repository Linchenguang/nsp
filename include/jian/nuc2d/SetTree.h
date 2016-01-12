#ifndef JIAN_NUC2D_SETTREE_H
#define JIAN_NUC2D_SETTREE_H

#include "N2DJob.h"

namespace jian {
namespace nuc2d {

class SetTree {
public:

    void set_tree(std::vector<res> &v0) {
        int i, j, temp;
        res *r, *preRes, *tempRes;
        loop *l;
        std::vector<loop *> s;

        // start iteration
        log("========== iteration ===========\n");

        std::vector<int> a(line.size() + 2, -1);
        for (int i = 0, index = 1; i < line.size(); i++) if (line[i] != '&') {
            a[i + 1] = index;
            index++;
        }

        // insert '|'
        std::vector<res> v1;
        for (int i = 0; i < v0.size(); i++) {
            append(v1, v0[i]);
            if (i+1<v0.size() && v0[i].type == ')' && v0[i + 1].type == '(') append(v1, res('|', v0[i+1].num), res('|', v0[i].num));
        }

        // main loop for tree setting
        std::vector<res> v;
        for (i = 0; i < (int) v1.size(); i++) {
            if (v.size() != 0 && v.back().type == ')' && v1[i].type != ')') get_loop(v, s, a);
            v.push_back(v1[i]);
        }

        log("\n----------- Last Iteration -------------\n");
        for (i = 0; i < (int) v.size(); i++) {
            if (v[i].type != '|') {
                log(v[i].type);
            }
        }
        log('\n');
        
        // add head
        l = new loop;
        for (auto &&item : v) l->push_back(item);
        r = new res(v[0].type, v[0].num);
        preRes = r;
        l->head = r;
        for (i = 1; i < (int) v.size(); i++) {
            r = new res(v[i].type, v[i].num);
            preRes->next = r;
            preRes = r;
        }

        // add interface to loop
        int hinge_pos = -1;
        for (tempRes = l->head; tempRes->next != NULL; tempRes = tempRes->next) {
            if (tempRes->next->num - tempRes->num > 1) {
                l->hinges.push_back(pair<int, int>(hinge_pos + 1, hinge_pos + 2));
                if (tempRes->next->type != '|') {
                    hinge_pos += 2;
                }

                preRes = tempRes;
                tempRes = new res('(', preRes->num + 1);
                tempRes->next = preRes->next;
                preRes->next = tempRes;
                preRes = tempRes;

                tempRes = new res(')', preRes->next->num - 1);
                tempRes->next = preRes->next;
                preRes->next = tempRes;

                l->type++;
            }
            hinge_pos++;
        }

        // delete '|'
        for (preRes = l->head, tempRes = preRes->next; tempRes != NULL; tempRes = tempRes->next) {
            if (tempRes->type == '|') {
                preRes->next = tempRes->next->next;
                delete tempRes->next;
                delete tempRes;
                tempRes = preRes->next;
                l->type--;
            }
            preRes = tempRes;
        }

        // set tree relation
        if (s.size() != 0) {
            l->son = s[0];
            for (int i = 0; i < s.size() - 1; i++) {
                s[i]->brother = s[i + 1];
            }
        }

        head = l;

        // delete '^'
        tempRes = l->head;
        l->head = l->head->next;
        delete tempRes; 

        // change head if the head does not exist
        if (l->getLen() == 2) {
            head = l->son;
            delete l;
        }

        // view helix
        if (view) {
            cout << "helix: \n";
            for (bp *b = l->s.head; b != NULL; b = b->next) {
                cout << a[b->res1.num] << '-' << a[b->res2.num] << endl;
            }

            if (v.size() != 0) {
                for (i = 0; i < (int) v.size(); i++) {
                    if (v[i].type != '|') {
                        cout << v[i].type;
                    }
                }
                cout << endl;
            }
        }

        // view loop
        if (view && l->head != NULL) {
            cout << "loop(number of branches:" << l->num_branches() << ") : \n";
            for (tempRes = l->head; tempRes != NULL; tempRes = tempRes->next) {
                cout << tempRes->type;
            }
            cout << endl;
            for (tempRes = l->head; tempRes != NULL; tempRes = tempRes->next) {
                cout << a[tempRes->num] << ' ';
            }
            cout << endl;
        }

        // print "iteration done"
        if (view) {
            cout << endl;
            cout << "           *******************\n";
            cout << "           * iteration done! *\n";
            cout << "           *******************\n";
        }

        if (view) {
            cout << line << endl;
            cout << seq << endl;
            printTree(head);
        }

        // extend hinge of ordinary 2D tree
    if (view) cout << "============ merge hinges of ordinary 2D tree ===================" << endl;
        extend_hinge(head, hinge_base_pair_num);

        if (view) {
            cout << line << endl;
            cout << seq << endl;
            printTree(head);
        }

    //    // construct pseudoknots
    //    constructPseudoknots();
    //
    //    if (view) {
    //        cout << line << endl;
    //        cout << seq << endl;
    //        printTree(pseudo_head);
    //    }

        // reset the num
        reset_num(head, a);
    //    reset_num(pseudo_head, a);
    }

    void reset_num(loop *l, const vector<int> &a) {
        if (l == NULL) {
            return;
        }

        // helix
        if (l->head != NULL) {
            for (res *tempRes = l->head; tempRes != NULL; tempRes = tempRes->next) {
                if (tempRes->num >= 1 && tempRes->num <= line.size() + 1) {
                    tempRes->num = a[tempRes->num];
                }
            }
        }

        // loop
        if (l->s.head != NULL) {
            for (bp *b = l->s.head; b != NULL; b = b->next) {
                if (b->res1.num >= 1 && b->res1.num <= line.size() + 1) {
                    b->res1.num = a[b->res1.num];
                }
                if (b->res2.num >= 1 && b->res2.num <= line.size() + 1) {
                    b->res2.num = a[b->res2.num];
                }
            }
        }

        // construct map matrix 'b'
        vector<int> b(l->size());
        string str = l->getSS();
        for (int i = 0, j = 0; i < b.size(); i++) {
            if (str[i] != '&') {
                b[i] = j;
                j++;
            }
        }

        // update hinge
        for (auto &hinge: l->hinges) {
            hinge.first = b[hinge.first];
            hinge.second = b[hinge.second];
        }

        reset_num(l->son, a);
        reset_num(l->brother, a);
    }

    void getLoop(vector<res> &v, vector<loop *> &s, const vector<int> &a) {
        int flag1, flag2, flag3, min, i, j, k, num, num1, num2;
        res *tempRes, *preRes;
        bp *b, *preB;
        loop *l;
        static int n = 0;

        n++;

        //    |j    |     |     |
        // ( (|( ( (|. . .|) ) )|
        //    |flag1|flag2|flag3|
        for (flag3 = 0, j = v.size() - 1; v[j].type == ')'; j--, flag3++);
        for (flag2 = 0; v[j].type == '.' || v[j].type == '&' || v[j].type == '|' || v[j].type == '[' || v[j].type == ']'; j--, flag2++);
        for (flag1 = 0; v[j].type == '(' && flag1 < flag3; j--, flag1++);
        j++;

        // print start infomation
        if (view) {
            cout << "\n------------------ iteration " << n << "------------------\n";
            for (i = 0; i < (int) v.size(); i++) {
                if (v[i].type != '|') {
                    cout << v[i].type;
                }
            }
            cout << endl;
        }
        
        // set loop
        l = new loop;
        k = j + flag1 - 1;
        tempRes = new res(v[k].type, v[k].num);
        preRes = tempRes;
        l->head = tempRes;
        for (k = j + flag1; k <= j + flag1 + flag2; k++) {
            tempRes = new res(v[k].type, v[k].num);
            preRes->next = tempRes;
            preRes = tempRes;
        }

        // set helix
        min = (flag1 > flag3) ? flag3 : flag1;
        k = 0;
        b = new bp;
        num1 = j + k;
        num2 = j + flag1 + flag2 + min - 1 - k;
        b->res1.type = v[num1].type;
        b->res2.type = v[num2].type;
        b->res1.num = v[num1].num;
        b->res2.num = v[num2].num;
        b->res1.next = b->res2.next = NULL;
        b->next = NULL;
        preB = b;
        l->s.head = b;
    //    l->s.len = min;
        for (k = 1; k < min; k++) {
            b = new bp;
            num1 = j + k;
            num2 = j + flag1 + flag2 + min - 1 - k;
            b->res1.type = v[num1].type;
            b->res2.type = v[num2].type;
            b->res1.num = v[num1].num;
            b->res2.num = v[num2].num;
            b->res1.next = b->res2.next = NULL;
            b->next = NULL;
            preB->next = b;
            preB = b;
        }

        // reset vector
        v.erase(v.begin() + j + flag1 - min, v.begin() + j + flag1 + flag2 + min);

        // add interface to loop
        int hinge_pos = 0;
        for (tempRes = l->head, l->type = 1; tempRes->next != NULL; tempRes = tempRes->next) {
            if (tempRes->next->num - tempRes->num > 1) {
                l->hinges.push_back(pair<int, int>(hinge_pos + 1, hinge_pos + 2));
                if (tempRes->next->type != '|') {
                    hinge_pos += 2;
                }

                preRes = tempRes;
                tempRes = new res('(', preRes->num + 1);
                tempRes->next = preRes->next;
                preRes->next = tempRes;
                preRes = tempRes;

                tempRes = new res(')', preRes->next->num - 1);
                tempRes->next = preRes->next;
                preRes->next = tempRes;
            }
            hinge_pos++;
        }

        // delete '|' from loop
        for (preRes = l->head, tempRes = preRes->next; tempRes != NULL; tempRes = tempRes->next) {
            if (tempRes->type == '|') {
                preRes->next = tempRes->next->next;
                delete tempRes->next;
                delete tempRes;
                tempRes = preRes->next;
            }
            preRes = tempRes;
        }

        // set tree relation
        num = l->hinges.size();
        i = s.size() - num;
        if (s.size() != 0 && num != 0) {
            l->son = s[i];
        }
        for (; i < (int) s.size(); i++) {
            if (i == (int) s.size() - 1) {
                s[i]->brother = NULL;
            } else {
                s[i]->brother = s[i + 1];
            }
        }
        //for (k = 0; k < num - 1; k++) {
        for (k = 0; k < num; k++) {
            s.pop_back();
        }

        // delete the loop which has only two residues
        if (l->getLen() == 2) {
            res::del(l->head);
            l->head = NULL;
        }

        // push new loop to s
        s.push_back(l);
        head = l;

        // view helix
        if (view) {
            cout << "helix: \n";
            for (b = l->s.head; b != NULL; b = b->next) {
                cout << a[b->res1.num] << '-' << a[b->res2.num] << endl;
            }

            if (v.size() != 0) {
                for (i = 0; i < (int) v.size(); i++) {
                    if (v[i].type != '|') {
                        cout << v[i].type;
                    }
                }
                cout << endl;
            }
        }

        // view loop
        if (view && l->head != NULL) {
            cout << "loop(number of branches:" << l->num_branches() << ") : \n";
            for (tempRes = l->head; tempRes != NULL; tempRes = tempRes->next) {
                cout << tempRes->type;
            }
            cout << endl;
            for (tempRes = l->head; tempRes != NULL; tempRes = tempRes->next) {
                cout << a[tempRes->num] << ' ';
            }
            cout << endl;
        }

        if (min == flag3) {
            return;
        } else {
            getLoop(v, s, a);
        }
    }

    pair<vector<loop *>, int> N2D::extend_hinge(loop *src, int hinge_base_pair_num) {
        static int it = 0;
        it++;
        vector<loop *> loop_list;
        loop *new_loop = new loop;

        if (src == NULL) {
    if (view) cerr << it << ' ' << "NULL return" << endl;
            return pair<vector<loop *>, int>(loop_list, 0);
        } else {
    if (view) cerr << '\n' << it << ' ' << "src->getSS(): " << src->getSS() << endl;
            /// Add son hinges to loop
    if (view) cerr << it << ' ' << src->getSS() << " Add son hinges to loop..." << endl;
            if (src->son != NULL) {
                auto son_list_pair = extend_hinge(src->son, hinge_base_pair_num);
                auto son_list = son_list_pair.first;
                if (son_list_pair.second == 1) {
                    loop *son = src->son;
                    loop *brother = src->brother;
                    if (son->son == NULL) {
                        src->son = son->brother;
    if (view) cerr << it << ' ' << src->getSS() << " delete son: " << son->getSS() << endl;
                        delete son;
                    } else {
                        loop *temp_loop = son->son;
                        for (; temp_loop->brother != NULL;
                             temp_loop = temp_loop->brother);
                        temp_loop->brother = son->brother;
                        src->son = son->son;
    if (view) cerr << it << ' ' << src->getSS() << " delete son: " << son->getSS() << endl;
                        delete son;
                    }
                }

    if (view) {
        cerr << it << ' ' << src->getSS() << " Load son tree..." << endl;
        cerr << it << ' ' << src->getSS() << " son_list.size(): " << son_list.size() << ' ';
        for (auto &hinge: src->hinges) {
            cout << "(" << hinge.first << ", " << hinge.second << ") ";
        }
        cerr << endl;
        cerr << it << ' ' << src->getSS() << " son_list_pair.second: " << son_list_pair.second << endl;
    }

                int hinge_index = 0;
                for (int son_index = 0; son_index < son_list.size(); son_index++) {
    if (view) {
        cout << it << ' ' << "\tson_index: " << son_index << endl;
        cout << it << ' ' << "\tson_list[son_index]->getSS(): " << son_list[son_index]->getSS() << endl;
        cout << it << ' ' << "\tson_list[son_index]->hinges.size(): " << son_list[son_index]->hinges.size() << endl;
    }
                    // extend loop
                    int res_index = 0;
                    for (auto temp_res = src->head; temp_res != NULL; temp_res = temp_res->next) {
                        if (res_index == src->hinges[hinge_index].first) {
                            auto next_res = temp_res->next->next;
                            *temp_res = *son_list[son_index]->head;
                            for (auto son_temp_res = son_list[son_index]->head->next; 
                                 son_temp_res != NULL; son_temp_res = son_temp_res->next) {
                                temp_res->next = new res(son_temp_res);
                                temp_res = temp_res->next;
                            }
                            temp_res->next = next_res;
                            break;
                        }
                        res_index++;
                    }

                    // update hinges
                    for (auto &hinge : son_list[son_index]->hinges) {
                        hinge.first += src->hinges[hinge_index].first;
                        hinge.second += src->hinges[hinge_index].first;
                    }
    if (view) cerr << it << ' ' << "\tInsert..." << endl;
                    if (son_list[son_index]->hinges.size() != 0) {
                        src->hinges.insert(src->hinges.begin() + hinge_index + 1, 
                               son_list[son_index]->hinges.begin(), 
                               son_list[son_index]->hinges.end());
                    }
    if (view) cerr << it << ' ' << "\tsrc->hinges.size(): " << src->hinges.size() << endl;
    if (view) cerr << it << ' ' << "\tErase..." << endl;
                    src->hinges.erase(src->hinges.begin() + hinge_index);
    if (view) cerr << it << ' ' << "\tsrc->hinges.size(): " << src->hinges.size() << endl;
                    hinge_index += son_list[son_index]->hinges.size();
                    int loop_len = son_list[son_index]->size();
                    //for (int i = hinge_index + son_list[son_index]->hinges.size();
                    for (int i = hinge_index; i < src->hinges.size(); i++) {
                        src->hinges[i].first += loop_len - 2;
                        src->hinges[i].second += loop_len - 2;
                    }
    if (view) {
        cerr << it << ' ' << "\t" << src->getSS() << " hinges: ";
        for (auto &hinge: src->hinges) {
            cout << "(" << hinge.first << ", " << hinge.second << ") ";
        }
        cerr << "\n" << endl;
    }
                }
    //if (view) cout << it << ' ' << "src->getSS(): " << src->getSS() << endl;
            }

    if (view) {
        cerr << it << ' ' << src->getSS() << " hinges: ";
        for (auto &hinge: src->hinges) {
            cout << "(" << hinge.first << ", " << hinge.second << ") ";
        }
        cerr << endl;
    }
    if (view) cout << it << ' ' << src->getSS() << " src->hinges.size(): " << src->hinges.size() << endl;
            /// Add hinge to loop and construct new loop
    if (view) cout << it << ' ' << src->getSS() << " Add hinge to loop and construct new loop..." << endl;
            if (src->s.head !=  NULL) {
    if (view) cout << it << ' ' << src->getSS() << " Load helix to new loop..." << endl;
                // load helix
                res *temp_res = new res(src->s.head->res1);
                new_loop->head = temp_res;
                temp_res->next = new res(src->s.head->res2);
                temp_res->next->next = NULL;
                int bp_index = 0;
                for (bp *temp_bp = src->s.head->next; temp_bp !=  NULL && bp_index < hinge_base_pair_num - 1; temp_bp = temp_bp->next) {
                    res *next_res = new res(temp_bp->res2);
                    next_res->next = temp_res->next;
                    temp_res->next = new res(temp_bp->res1);
                    temp_res = temp_res->next;
                    temp_res->next = next_res;
                    bp_index++;
                }
            }

    if (view) cout << it << ' ' << src->getSS() << " src->hinges.size(): " << src->hinges.size() << endl;
            int helix_len = src->s.len();
            if (helix_len < hinge_base_pair_num) {
    if (view) cout << it << ' ' << src->getSS() << " helix_len < hinge_base_pair_num" << endl;
                // turn '(' and ')' to '.'
                for (res *helix_res = new_loop->head; helix_res != NULL; helix_res = helix_res->next) {
                    helix_res->type = '.';
                }

                if (src->head != NULL) {
                    // merge helix and loop
                    if (src->s.head != NULL) {
                        res *temp_res = new_loop->head;
                        for (int i = 0; i < helix_len - 1; i++, temp_res = temp_res->next);
                        res *next_res = temp_res->next;
                        for (res *loop_res = src->head->next;
                             loop_res->next != NULL;
                             loop_res = loop_res->next) {
                            temp_res->next = new res(loop_res);
                            temp_res = temp_res->next;
                        }
                        temp_res->next = next_res;
                    } else {
                        new_loop->head = res::copy(src->head);
                    }
                }
                // Update hinges
                new_loop->hinges = src->hinges;
                int temp = helix_len == 0 ? 0 : helix_len - 1;
                //int temp = helix_len > hinge_base_pair_num ? hinge_base_pair_num - 1 : helix_len - 1;
                for (auto &hinge: new_loop->hinges) {
                    hinge.first += temp;
                    hinge.second += temp;
                }
                src->hinges = new_loop->hinges;
                res::del(src->head);
                bp::del(src->s.head);
                src->head = res::copy(new_loop->head);
                src->s.head = NULL;

    if (view) {
        cout << it << ' ' << src->getSS() << " new_loop->getSS(): " << new_loop->getSS() << endl;
        cout << it << ' ' << src->getSS() << " src->hinges.size(): " << src->hinges.size() << endl;
        cout << it << ' ' << src->getSS() << " new_loop->hinges.size(): " << new_loop->hinges.size() << endl;
    }

            } else {
    if (view) cout << it << ' ' << src->getSS() << " helix_len > hinge_base_pair_num" << endl;
                if (src->head != NULL) {
                    int temp = src->s.getLen() - hinge_base_pair_num;
                    bp *b = src->s.head;
                    for (int i = 0; b != NULL && i < temp; b = b->next, i++);
                    res *tail;
                    for (tail = src->head; tail->next != NULL; tail = tail->next);
                    int i = 0;
                    for (bp *temp_bp = b;
                         temp_bp->next != NULL && i < hinge_base_pair_num - 1;
                         temp_bp = temp_bp->next, i++) {
                        res *temp_res = new res(temp_bp->res1);
                        temp_res->next = src->head;
                        src->head = temp_res;
                        tail->next = new res(temp_bp->res2);
                        tail = tail->next;
                        tail->next = NULL;
                    }

                    // update hinges
                    for (auto &hinge: src->hinges) {
                        hinge.first += hinge_base_pair_num - 1;
                        hinge.second += hinge_base_pair_num - 1;
                    }
                }

                new_loop->hinges.push_back({hinge_base_pair_num - 1, hinge_base_pair_num});
    if (view) cout << it << ' ' << src->getSS() << " new_loop->hinges.size(): " << new_loop->hinges.size() << endl;
            }

            loop_list.push_back(new_loop);

            /// Set brother tree
    if (view) cerr << it << ' ' << src->getSS() << " Set brother tree..." << endl;
            if (src->brother != NULL) {
                auto brother_list_pair = extend_hinge(src->brother, hinge_base_pair_num);
                if (brother_list_pair.second == 1) {
                    loop *son = src->brother->son;
                    loop *brother = src->brother->brother;
                    if (son == NULL) {
                        delete src->brother;
                        src->brother = brother;
    if (view) cerr << it << ' ' << src->getSS() << " Delete brother..." << endl;
                    } else {
                        loop *temp_loop = son;
                        for (; temp_loop->brother != NULL; temp_loop = temp_loop->brother);
                        temp_loop->brother = brother;
                        delete src->brother;
                        src->brother = son;
                    }
                }
                auto brother_list = brother_list_pair.first;
                loop_list.insert(loop_list.end(), brother_list.begin(), brother_list.end());
                //brother_list.insert(brother_list.begin(), new_loop);
            }

            if (src->s.getLen() < hinge_base_pair_num) {
    if (view) cerr << it << ' ' << src->getSS() << " return 1" << endl;
                return {loop_list, 1};
            } else {
    if (view) cerr << it << ' ' << src->getSS() << " return 0" << endl;
                return {loop_list, 0};
            }
        }
    }

    void N2D::setPairs() {
        vector<int> v1;
        vector<int> v2;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == '(') {
                v1.push_back(i);
            } else if (line[i] == '[') {
                v2.push_back(i);
            } else if (line[i] == ')') {
                pairs[i] = v1.back();
                pairs[v1.back()] = i;
                v1.pop_back();
            } else if (line[i] == ']') {
    //std::cout << i << ' ' << v2.size() << ' ' << std::endl;
                pairs[i] = v2.back();
                pairs[v2.back()] = i;
                v2.pop_back();
            }
        }
    }

    void N2D::setLoops(loop *l) {
        if (l == NULL) {
            return;
        }
        if (l->head != NULL) {
            for (res *r = l->head; r != NULL; r = r->next) {
                if (r->num > 0 && r->num <= line.size()) {
                    loops[r->num - 1] = l;
                }
            }
        }
        if (l->s.head != NULL) {
            for (bp *b = l->s.head; b != NULL; b = b->next) {
                if (b->res1.num >= 1 && b->res1.num <= line.size()) {
                    loops[b->res1.num - 1] = l;
                }
                if (b->res2.num >= 1 && b->res2.num <= line.size()) {
                    loops[b->res2.num - 1] = l;
                }
            }
        }
        setLoops(l->son);
        setLoops(l->brother);
    }

    list<loop *> N2D::find_path(loop *src, loop *target) {
        if (src == NULL) {
            return list<loop *>();
        } else if (src == target) {
            list<loop *> loop_list;
            loop_list.push_front(src);
            return loop_list;
        } else {
            list<loop *> son_loop_list = find_path(src->son, target);
            if (son_loop_list.begin() != son_loop_list.end()) {
                son_loop_list.push_front(src);
                return son_loop_list;
            } else {
                list<loop *> brother_loop_list = find_path(src->brother, target);
                if (brother_loop_list.begin() != brother_loop_list.end()) {
                    return brother_loop_list;
                } else {
                    return list<loop *>();
                }
            }
        }
    }

    void N2D::constructPseudoknots() {
        if (view) cout << "\n=============== construct pseudoknots ===================\n\n";

        // initialize loops
        pairs.resize(line.size(), -1);
        loops.resize(line.size(), NULL);
        
        setPairs();
        pseudo_head = loop::copy(head);
        setLoops(pseudo_head);
        int flag = 1;
        while (flag != 0) {
            flag = 0;
            
            // find path to root node
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '[') {
                    if (loops[i] != loops[pairs[i]]) {
                        flag++;
                        list<loop *> list1 = find_path(pseudo_head, loops[i]);
                        list<loop *> list2 = find_path(pseudo_head, loops[pairs[i]]);
                        list<loop *>::iterator it1 = list1.begin();
                        list<loop *>::iterator it2 = list2.begin();
                        set<loop *> loop_set;
                        loop *root;
                        while (it1 != list1.end() || it2 != list2.end()) {
                            if (it1 != list1.end() && it2 != list2.end()) {
                                auto it11 = ++it1;
                                it1--;
                                auto it22 = ++it2;
                                it2--;
                                if (*it1 == *it2 && *it11 != *it22) {
                                    root = *it1;
                                    loop_set.insert(root);
                                } else if (*it1 != *it2) {
                                    loop_set.insert(*it1);
                                    loop_set.insert(*it2);
                                }
                                it1++;
                                it2++;
                            } else if (it1 != list1.end() && it2 == list2.end()) {
                                loop_set.insert(*it1);
                                it1++;
                            } else if (it1 == list1.end() && it2 != list2.end()) {
                                loop_set.insert(*it2);
                                it2++;
                            }
                        }

                        // create pseudoknot
                        //loop *l = new loop;
                        loop *l = pseudo_loop(root, loop_set).front();
                        l->type = -2;
                        l->len = l->size();
                        //l->len = l->getLen();

                        /*
                        // set pseudoknot
                        for (int i = ss.size() - 1; i >= 0; i--) {
                            if (loops[i] == l) {
                                res *r = new res(ss[i], i + 1);
                                r->next = l->head;
                                l->head = r;
                                l->len++;
                            }
                        }
                        */

                        // construct pseudoknot tree
                        pseudo_tree(root, pseudo_head, loop_set, l);

                        // set loops
                        /*
                        for (set<loop *>::iterator it = loop_set.begin(); it != loop_set.end(); it++) {
                            for (bp *b = (*it)->s.head; b != NULL; b = b->next) {
                                loops[b->res1.num - 1] = root;
                                loops[b->res2.num - 1] = root;
                            }
                            for (res *r = (*it)->head; r != NULL; r = r->next) {
                                loops[r->num - 1] = root;
                            }
                        }
                        */
                        setLoops(pseudo_head);

                        delete l;
                    }
                }
            }
        }
    }

    list<loop *> N2D::pseudo_loop(loop *src, set<loop *> loop_set) {
        list<loop *> loop_list;

        if (src == NULL) {
            return loop_list;
        } else if (!loop_set.count(src)) {
            loop_list = pseudo_loop(src->brother, loop_set);
            loop *l = new loop;
            loop_list.push_front(l);
            return loop_list;
        }

        loop *l = new loop(src);

        list<loop *> son_list = pseudo_loop(src->son, loop_set);
        if (son_list.begin() != son_list.end()) {
            int hinge_num = 0;
            for (res *temp_res = l->head; temp_res->next != NULL; temp_res = temp_res->next) {
                if (temp_res->type == '(' && temp_res->next->type == ')') {
                    loop *l2 = son_list.front();

                    if (l2->s.head == NULL && l2->head == NULL) {
                        delete l2;
                        son_list.pop_front();
                        hinge_num++;
                        continue;
                    }

    if (view) {
        cout << l->getSS() << ": ";
        for (auto &hinge: l->hinges) {
            cout << "(" << hinge.first << ", " << hinge.second << ") ";
        }
        cout << endl;
    }

                    /// load helix
                    if (l2->s.head != NULL) {
                        bp *temp_bp = l2->s.head;
                        for (int i = 0; i < hinge_base_pair_num; i++, temp_bp = temp_bp->next); 
                        for (bp *b = temp_bp; b != NULL; b = b->next) {
                            res *temp_next = temp_res->next;
                            temp_res->next = new res(b->res1);
                            temp_res = temp_res->next;
                            temp_res->next = new res(b->res2);
                            temp_res->next->next = temp_next;
                        }
                    }

                    /// load loop
                    if (l2->head != NULL) {
                        res *temp_next = temp_res->next;
                        res *starting_pt = l2->head;
                        for (int i = 0; i < hinge_base_pair_num; i++, starting_pt = starting_pt->next);
                        res *temp_res2 = starting_pt;
                        int l2_len = l2->size();
                        for (int i = 0;
                             i < l2_len - hinge_base_pair_num * 2 && temp_res2 != NULL;
                             temp_res2 = temp_res2->next, i++) {
                            temp_res->next = new res(temp_res2);
                            temp_res = temp_res->next;
                        }
                        temp_res->next = temp_next;
                    }

                    /// update hinge position
                    int helix_len_l2 = l2->s.getLen();
                    int loop_len_l2 = l2->size();
                    auto temp_pair = l->hinges[hinge_num];
                    l->hinges.erase(l->hinges.begin() + hinge_num);
                    int insert_pos = hinge_num;
                    for (auto &hinge : l2->hinges) {
                        l->hinges.insert(l->hinges.begin() + insert_pos, {
    //                        hinge.first + temp_pair.first + helix_len_l2 - hinge_base_pair_num, 
    //                        hinge.second + temp_pair.first + helix_len_l2 - hinge_base_pair_num});
                            hinge.first + temp_pair.first + helix_len_l2 + 1 - 2 * hinge_base_pair_num, 
                            hinge.second + temp_pair.first + helix_len_l2 + 1 - 2 * hinge_base_pair_num});
                        insert_pos++;
                        hinge_num++;
                    }
                    for (auto it = l->hinges.begin() + insert_pos; it != l->hinges.end(); it++) {
                        if (helix_len_l2 != 0) {
                            it->first += helix_len_l2 * 2 - 2 * hinge_base_pair_num;
                            it->second += helix_len_l2 * 2 - 2 * hinge_base_pair_num;
                        }
                        if (loop_len_l2 != 0) {
                            it->first += loop_len_l2 - 2 * hinge_base_pair_num;
                            it->second += loop_len_l2 - 2 * hinge_base_pair_num;
                        }
                    }
    if (view) {
        cout << l->getSS() << ": ";
        for (auto &hinge: l->hinges) {
            cout << "(" << hinge.first << ", " << hinge.second << ") ";
        }
        cout << endl;
    }

                    delete l2;
                    son_list.pop_front();
                }
            }
        }

        list<loop *> brother_list = pseudo_loop(src->brother, loop_set);
        if (brother_list.begin() != brother_list.end()) {
            brother_list.push_front(l);
            return brother_list;
        } else {
            loop_list.push_front(l);
            return loop_list;
        }
    }


};

} // namespace nuc2d
} // namespace jian

#endif
