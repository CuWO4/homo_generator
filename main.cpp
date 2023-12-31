#include "fraction.hpp"

#include <iostream>
#include <list>
#include <time.h>

typedef long long int64;

struct Homo_Unit {
    int next() {
        int res = __homo_unit[iter];
        iter = (iter + 1) % length;
        return res;
    }

    private:

    int iter = 0;
    int length = 6;
    int __homo_unit[6] = { 1, 1, 4, 5, 1, 4 };
};

const int Opt_Num = 6;

enum Opt_Type {
    NUM, ADD, SUB, MUL, DIV, COM
};

fraction num(fraction a, fraction b) { return a; }
fraction add(fraction a, fraction b) { return a + b; }
fraction sub(fraction a, fraction b) { return a - b; }
fraction mul(fraction a, fraction b) { return a * b; }
fraction div(fraction a, fraction b) { return a / b; }
fraction com(fraction a, fraction b) { return 10 * a + b; }
fraction (*Opt_Func[Opt_Num])(fraction a, fraction b) = { num, add, sub, mul, div, com };

int priority[Opt_Num] = { 3, 1, 1, 2, 2, 3 };
bool operator< (Opt_Type a, Opt_Type b) {
    return priority[a] < priority[b];
}

char Opt_Name[Opt_Num][10] = { "Num", "+", "-", "*", "/", "Com" };

struct Algebra_Node {
    Opt_Type opt_type = NUM;
    fraction val = 0;
    Algebra_Node *left = nullptr, *right = nullptr;

    Algebra_Node(Opt_Type __opt_type = NUM, fraction __val = 0, Algebra_Node *__left = nullptr, Algebra_Node *__right = nullptr) :
        opt_type(__opt_type), val(__val), left(__left), right(__right) {}

    void print(bool bracket_needed = false) {
        if (bracket_needed) std::cout << "(";
        if (opt_type == NUM || opt_type == COM) std::cout << val;
        else {
            left->print(left->opt_type < opt_type);
            std::cout << Opt_Name[opt_type];
            right->print(right->opt_type < opt_type);
        }
        if (bracket_needed) std::cout << ")";
    }
};

/**
 * @return return the root of algebra tree if there is one; otherwise nullptr
**/
Algebra_Node *build(std::list<Algebra_Node*> unsolved_node_list, int64 target_num) {
    if (unsolved_node_list.size() == 1) {
        if (unsolved_node_list.front()->val == target_num) {
            return unsolved_node_list.front();
        }
        else return nullptr;
    }

    for (auto cur_iter = unsolved_node_list.begin(); std::next(cur_iter) != unsolved_node_list.end(); cur_iter++) {
        Algebra_Node *new_node = new Algebra_Node;
        Algebra_Node *left = (new_node->left = *cur_iter); 
        Algebra_Node *right = (new_node->right = *std::next(cur_iter));

        /* can be optimized */
        cur_iter = unsolved_node_list.erase(cur_iter);
        cur_iter = unsolved_node_list.erase(cur_iter);
        cur_iter = unsolved_node_list.insert(cur_iter, new_node);

        for (int type = ADD; type <= MUL; type++) {
            new_node->opt_type = (Opt_Type)type;
            new_node->val = Opt_Func[type](left->val, right->val);

            Algebra_Node *root = build(unsolved_node_list, target_num);
            if (root != nullptr) return root;
        }
        if (right->val != 0) {
            new_node->opt_type = DIV;
            new_node->val = Opt_Func[DIV](left->val, right->val);

            Algebra_Node *root = build(unsolved_node_list, target_num);
            if (root != nullptr) return root;
        }
        if ((left->opt_type == NUM || left->opt_type == COM)
                && right->opt_type == NUM) {
            new_node->opt_type = COM;
            new_node->val = Opt_Func[COM](left->val, right->val);

            Algebra_Node *root = build(unsolved_node_list, target_num);
            if (root != nullptr) return root;
        }

        cur_iter = unsolved_node_list.erase(cur_iter);
        cur_iter = unsolved_node_list.insert(cur_iter, left);
        unsolved_node_list.insert(std::next(cur_iter), right);

        delete new_node;
    }

    return nullptr;
}

const int MAX_LENGTH = (int)1e3;    /* max deposition length */

Algebra_Node *generate(int64 target_num, Homo_Unit &homo_unit) {
    for (int len = 1; len < MAX_LENGTH; len++) {
        Homo_Unit backup = homo_unit;
        std::list<Algebra_Node*> base_list;
        for (int i = 0; i < len; i++) {
            base_list.push_back(new Algebra_Node(NUM, homo_unit.next()));
        }
        Algebra_Node *root = build(base_list, target_num);
        if (root != nullptr) {
            return root;
        }
        homo_unit = backup;
    }
    return nullptr;
}

/* [a, b) */
int64 rand_range(int64 a, int64 b) {
    return rand() % (b - a) + a;
}

Algebra_Node *generate_adding(int64 target_num, Homo_Unit &homo_unit, int granularity = 200) {
    bool first = true;
    Algebra_Node *res = nullptr;
    while (target_num > granularity) {
        int64 tmp = rand_range(1, granularity);
        target_num -= tmp;

        Algebra_Node *new_node = generate(tmp, homo_unit);
        if (first) {
            res = new_node;
        }
        else {
            res = new Algebra_Node(ADD, Opt_Func[ADD](res->val, new_node->val), res, new_node);
        }
        first = false;
    }
    if (target_num != 0) {
        Algebra_Node *new_node = generate(target_num, homo_unit);
        if (first) {
            res = new_node;
        }
        else {
            res = new Algebra_Node(ADD, Opt_Func[ADD](res->val, new_node->val), res, new_node);
        }
        first = false;
    }
    return res;
}

Algebra_Node *generate_multiplication(int64 target_num, Homo_Unit &homo_unit, int granularity = 1000) {
    bool first = true;
    Algebra_Node *res = nullptr;
    while (target_num > granularity) {
        int tmp;
        for (tmp = target_num - 1; tmp >= 1; tmp--) {
            if (target_num % tmp == 0) break;
        }
        if (tmp == 1) break;

        target_num /= tmp;

        Algebra_Node *new_node = tmp < granularity
            ? generate_adding(tmp, homo_unit)
            : generate_multiplication(tmp, homo_unit);
        if (first) {
            res = new_node;
        }
        else {
            res = new Algebra_Node(MUL, Opt_Func[MUL](res->val, new_node->val), res, new_node);
        }
        first = false;
    }
    if (target_num != 0) {
        Algebra_Node *new_node = generate_adding(target_num, homo_unit);
        if (first) {
            res = new_node;
        }
        else {
            res = new Algebra_Node(MUL, Opt_Func[MUL](res->val, new_node->val), res, new_node);
        }
        first = false;
    }
    return res;
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));

    int64 target_num = 0;
    std::cin >> target_num;

    Homo_Unit homo_unit;

    std::cout << target_num << '=';

    if (target_num == 0) {
        std::cout << "1-1";
    }
    else if (target_num > 0) {
        generate_multiplication(target_num, homo_unit)->print();
    }
    else { /* target_num < 0 */
        Algebra_Node *left = generate_multiplication(-1, homo_unit);
        Algebra_Node *right = generate_multiplication(-target_num, homo_unit);
        Algebra_Node(MUL, Opt_Func[MUL](left->val, right->val), left, right).print();
    }                                                                            
    std::cout << std::endl;

    return 0;
}