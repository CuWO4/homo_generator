#include "fraction.hpp"

#include <iostream>
#include <vector>

fraction target_num = 0;

const int homo_unit[6] = { 1, 1, 4, 5, 1, 4 };

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

    Algebra_Node(Opt_Type __opt_type = NUM, fraction __val = 0) :
        opt_type(__opt_type), val(__val) {}
    ~Algebra_Node() {
        if (left != nullptr) left->~Algebra_Node();
        delete left;
        if (right != nullptr) right->~Algebra_Node();
        delete right;
    }

    void print() {
        print_agent();
        std::cout << std::endl;
    }
    void print_agent(bool bracket_needed = false) {
        if (bracket_needed) std::cout << "(";
        if (opt_type == NUM || opt_type == COM) std::cout << val;
        else {
            left->print_agent(left->opt_type < opt_type);
            std::cout << Opt_Name[opt_type];
            right->print_agent(right->opt_type < opt_type);
        }
        if (bracket_needed) std::cout << ")";
    }
};

/**
 * @return whether there is a way to deposite target number into unsolved_node_list
**/
bool build(std::vector<Algebra_Node*> unsolved_node_list) {
    if (unsolved_node_list.size() == 1) {
        if (unsolved_node_list[0]->val == target_num) {
            std::cout << target_num << '=';
            unsolved_node_list[0]->print();
            return true;
        }
        else return false;
    }

    for (int i = 0, size = unsolved_node_list.size(); i < size - 1; i++) {
        Algebra_Node *new_node = new Algebra_Node;
        Algebra_Node *left = (new_node->left = unsolved_node_list[i]);
        Algebra_Node *right = (new_node->right = unsolved_node_list[i + 1]);

        /* can be optimized */
        unsolved_node_list.erase(unsolved_node_list.begin() + i);
        unsolved_node_list.erase(unsolved_node_list.begin() + i);
        unsolved_node_list.insert(unsolved_node_list.begin() + i, new_node);

        for (int type = ADD; type <= MUL; type++) {
            new_node->opt_type = (Opt_Type)type;
            new_node->val = Opt_Func[type](left->val, right->val);
            if (build(unsolved_node_list)) return true;
        }
        if (right->val != 0) {
            new_node->opt_type = DIV;
            new_node->val = Opt_Func[DIV](left->val, right->val);
            if (build(unsolved_node_list)) return true;
        }
        if ((left->opt_type == NUM || left->opt_type == COM)
                && right->opt_type == NUM) {
            new_node->opt_type = COM;
            new_node->val = Opt_Func[COM](left->val, right->val);
            if (build(unsolved_node_list)) return true;
        }

        unsolved_node_list.erase(unsolved_node_list.begin() + i);
        unsolved_node_list.insert(unsolved_node_list.begin() + i, left);
        unsolved_node_list.insert(unsolved_node_list.begin() + i + 1, right);
    }

    return false;
}

const int MAX_LENGTH = (int)1e3;    /* max deposition length */

int main(int argc, char **argv) {
    std::cin >> target_num;

    for (int len = 1; len < MAX_LENGTH; len++) {
        std::vector<Algebra_Node*> base_list;
        for (int i = 0; i < len; i++) {
            base_list.push_back(new Algebra_Node(NUM, homo_unit[i % 6]));
        }
        if (build(base_list)) break;
    }
    
    return 0;
}