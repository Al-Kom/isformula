//
// Created by ksan on 26.02.20.
//

#include "pllftree.h"
#include "pllfv.h"

Node::~Node() {
    if(left)    delete left;
    if(right)   delete right;
}

PllfTree::PllfTree() { root_ = new Node(); }

PllfTree::~PllfTree() {

}

void PllfTree::generate(const char *str, int len) {
    Clean();
    if(!matchRegexp(DEFAULT_FORMULA_PATTERN, str, len)) {
        //manage error
        Clean();
        throw BadSyntaxException();
    }
    Node *current = root_;
    for (int i = 0; i < len; i++) {
        //check if atom
        if(str[i] >= 'A' && str[i] <= 'Z') {
            // set atom to current node
            current->data = str[i];
            continue;
        }
        //check if other token
        switch(str[i]) {
            //-----------CONSTANTS-------------------
            case CONST_ZERO:
            case CONST_ONE:
                // set the constant to current node
                current->data = str[i];
                break;
                //------------PARENTHESES AND UNARY OPERATION----------------
            case P_OPEN:
                //check if unary formula
                if(str[i + 1] == UN_NEG) {
                    // set negation operation to current node
                    current->data = UN_NEG;
                    i++;
                }
                // switch to left node
                current->left = new Node();
                current->left->parent = current;
                current = current->left;
                break;
            case P_CLOSE:
                current = current->parent;
                break;
                //-------------BINARY OPERATIONS------------
            case BIN_CONJ:
            case BIN_DISJ:
            case BIN_EQUAL:
                current = addBinaryOperation(current, (NodeData) str[i]);
                break;
            case BIN_IMPL_PART_1:
                //check if implication
                if (str[++i] == BIN_IMPL_PART_2) {
                    current = addBinaryOperation(current, BIN_IMPL);
                } else {
                    //manage error
                    Clean();
                    throw BadSyntaxException();
                }
                break;
                //-------------BINARY OPERATIONS------------
            default:
                //manage error
                Clean();
                throw BadSyntaxException();
        }//switch(str[i])
    }//for (int i = 0; i < str.length(); i++)
}

Node *PllfTree::addBinaryOperation(Node *current, NodeData operation) {
    // set binary operation to parent node
    current = current->parent;
    current->data = operation;
    //switch to right node
    current->right = new Node();
    current->right->parent = current;
    return current->right;
}

void PllfTree::Clean() {
    delete root_;
    root_ = new Node();
}

const char *BadSyntaxException::what() const noexcept {
    return "Bad formula syntax";
}
