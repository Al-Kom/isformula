// Laboratory work 2 on the discipline LOIS
// Completed by student of group 721702 BSUIR Komar Alexander Nazarovich
// CPP-file of library of propositional logic language formula verifier tree
// Version 3. Add calculation of truth table and tautology test.
// Clean code. Improve readability
//

#include <cmath>
#include <utility>
#include <vector>
#include "pllftree.h"
#include "pllfv.h"

Node::~Node() {
    delete left;
    delete right;
}

std::string Node::toString() {
    if (data == BIN_IMPL)
        return "->";
    return std::string({data});
}

PllfTree::PllfTree() { root_ = new Node(); }

PllfTree::~PllfTree() = default;

void PllfTree::generate(std::string str) {
    Clean();
    if (!matchRegexp(DEFAULT_FORMULA_PATTERN, str.c_str(), str.length())) {
        // manage error
        Clean();
        throw BadSyntaxException();
    }
    Node *current = root_;
    for (ulong i = 0; i < str.length(); i++) {
        // check if atom
        if (str[i] >= 'A' && str[i] <= 'Z') {
            // set atom to current node
            current->data = str[i];
            continue;
        }
        // check if other token
        switch (str[i]) {
            //-----------CONSTANTS-------------------
            case CONST_ZERO:
            case CONST_ONE:
                // set the constant to current node
                current->data = str[i];
                break;
                //------------PARENTHESES AND UNARY OPERATION----------------
            case P_OPEN:
                // check if unary formula
                if (str[i + 1] == UN_NEG) {
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
                // check if implication
                if (str[++i] == BIN_IMPL_PART_2) {
                    current = addBinaryOperation(current, BIN_IMPL);
                } else {
                    // manage error
                    Clean();
                    throw BadSyntaxException();
                }
                break;
                //-------------ERROR-----------
            default:
                // manage error
                Clean();
                throw BadSyntaxException();
        }// switch(str[i])
    }// for (int i = 0; i < str.length(); i++)
}

Node *PllfTree::addBinaryOperation(Node *current, NodeData operation) {
    // set binary operation to parent node
    current = current->parent;
    current->data = operation;
    // switch to right node
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
    // left exists only in formulas
    if (current->left) {
        // unary formula
        if (current->data == UN_NEG)
            return "(!" + dfsGetData(current->left) + ")";
        // binary formula
        return "(" + dfsGetData(current->left) + current->toString()
                + dfsGetData(current->right) + ")";
    }
    // atom or constant
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
    // set random binary operation
    switch (rand() % 4) {
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
    // generate left operand
    current->left = new Node();
    current->left->parent = current;
    generateFormulaNode(current->left);
    // generate right operand
    current->right = new Node();
    current->right->parent = current;
    generateFormulaNode(current->right);
}

std::string PllfTree::getVariables() {
    std::string vars;
    std::string strTree = toString();
    // find variables
    for (char i : strTree)
        if (i >= 'A' && i <= 'Z')
            if (vars.find(i, 0) == std::string::npos)
                vars += i;
    return vars;
}

bool **PllfTree::getTruthTable() {

    std::string vars = getVariables();
    // decl table
    int colsN = (int) vars.length() + 1;
    int rowsN = (int) pow(2, vars.length());
    bool **truthTable = new bool *[rowsN];
    for (int i = 0; i < rowsN; ++i)
        truthTable[i] = new bool[colsN];

    // init table
    for (int row = 0; row < rowsN; ++row) {
        int val = row;
        for (int col = colsN - 2; col >= 0; --col) {
            truthTable[row][col] = val % 2;
            val /= 2;
        }
    }
    // calc table
    std::map<char, bool> valueMap;
    for (int i = 0; i < rowsN; ++i) {
        // init map of vars
        valueMap.clear();
        for (int j = 0; j < (int) vars.length(); ++j)
            valueMap.insert(std::pair<char, bool>(vars[j], truthTable[i][j]));
        // calc result
        truthTable[i][colsN - 1] = calculate(valueMap);
    }
    return truthTable;
}

bool PllfTree::calculate(std::map<char, bool> valueMap) {
    return dfsCalculate(root_, std::move(valueMap));
}

bool PllfTree::dfsCalculate(Node *current, std::map<char, bool> valueMap) {
    // formula
    if (current->left) {
        // unary formula
        if (current->data == UN_NEG)
            return !dfsCalculate(current->left, valueMap);
        // binary formula
        switch (current->data) {
            case BIN_CONJ:
                return dfsCalculate(current->left, valueMap) && dfsCalculate(current->right, valueMap);
            case BIN_DISJ:
                return dfsCalculate(current->left, valueMap) || dfsCalculate(current->right, valueMap);
            case BIN_EQUAL:
                return (dfsCalculate(current->left, valueMap) && dfsCalculate(current->right, valueMap))
                       || (!dfsCalculate(current->left, valueMap) && !dfsCalculate(current->right, valueMap));
            case BIN_IMPL:
                return !dfsCalculate(current->left, valueMap) || dfsCalculate(current->right, valueMap);
        }
    }
    // constant
    if (current->data == CONST_ONE)
        return true;
    else if (current->data == CONST_ZERO)
        return false;
    else if (current->data >= 'A' && current->data <= 'Z')    // atom
        return valueMap.at(current->data);
    else
        throw BadSyntaxException();

}

bool PllfTree::isTautology() {
    bool **truthTable = getTruthTable();
    int colsN = (int) getVariables().length() + 1;
    int rowsN = (int) pow(2, getVariables().length());
    for (int i = 0; i < rowsN; ++i) {
        if (!truthTable[i][colsN - 1])
            return false;
    }
    return true;
}


const char *BadSyntaxException::what() const noexcept {
    return "Bad formula syntax";
}
