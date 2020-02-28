//
// Created by ksan on 26.02.20.
//

#include "pllftree.h"
#include "pllfv.h"

Node::~Node() {
    delete left;
    delete right;
}

std::string Node::toString() {
    if(data == BIN_IMPL)
        return "->";
    return std::string({data});
}

PllfTree::PllfTree() { root_ = new Node(); }

PllfTree::~PllfTree() = default;

void PllfTree::generate(std::string str) {
    Clean();
    if(!matchRegexp(DEFAULT_FORMULA_PATTERN, str.c_str(), str.length())) {
        //manage error
        Clean();
        throw BadSyntaxException();
    }
    Node *current = root_;
    for (auto i = 0; i < str.length(); i++) {
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

std::string PllfTree::toString() {
    return dfsGetData(root_);
}

std::string PllfTree::dfsGetData(Node *current) {
    if(current->left) {
        if(current->data == UN_NEG)
            return "(!" + dfsGetData(current->left) + ")";     // unary formula
        // binary formula
        return "(" + dfsGetData(current->left) + current->toString() + dfsGetData(current->right) + ")";
    }
    // atom and constant
    return current->toString();
}

void PllfTree::randomGenerate() {
    Clean();
    Node *current = root_;
    generateFormulaNode(current);
}

void PllfTree::generateFormulaNode(Node *current) {
    switch (rand() % 4) {
        case 0:
            generateAtomNode(current);
            break;
        case 1:
            generateConstantNode(current);
            break;
        case 2:
            generateUnaryFormulaNode(current);
            break;
        case 3:
            generateBinaryFormulaNode(current);
            break;
    }
}

void PllfTree::generateAtomNode(Node *current) {
    current->data = char(rand() % ('Z' - 'A') + 'A');
}

void PllfTree::generateConstantNode(Node *current) {
    current->data = (rand() % 2) ? CONST_ONE : CONST_ZERO;
}

void PllfTree::generateUnaryFormulaNode(Node *current) {
    current->data = UN_NEG;
    current->left = new Node();
    current->left->parent = current;
    generateFormulaNode(current->left);
}

void PllfTree::generateBinaryFormulaNode(Node *current) {
    //set random binary operation
    switch(rand()%4) {
        case 0:
            current->data = BIN_IMPL;
            break;
        case 1:
            current->data = BIN_CONJ;
            break;
        case 2:
            current->data = BIN_DISJ;
            break;
        case 3:
            current->data = BIN_EQUAL;
            break;
    }
    //generate left operand
    current->left = new Node();
    current->left->parent = current;
    generateFormulaNode(current->left);
    //generate right operand
    current->right = new Node();
    current->right->parent = current;
    generateFormulaNode(current->right);
}


const char *BadSyntaxException::what() const noexcept {
    return "Bad formula syntax";
}
