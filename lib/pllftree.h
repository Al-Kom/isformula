// Laboratory work 2 on the discipline LOIS
// Completed by student of group 721702 BSUIR Komar Alexander Nazarovich
// H-file of library of propositional logic language formula verifier tree
// Version 3. Add calculation of truth table and tautology test.
// Clean code. Improve readability
//

#ifndef PLLFTREE_H
#define PLLFTREE_H

#include <exception>
#include <string>
#include <map>

enum NodeData : char {
    CONST_ZERO = '0',
    CONST_ONE = '1',
    P_OPEN = '(',
    P_CLOSE = ')',
    UN_NEG = '!',
    BIN_CONJ = '&',
    BIN_DISJ = '|',
    BIN_EQUAL = '~',
    BIN_IMPL_PART_1 = '-',
    BIN_IMPL_PART_2 = '>',
    BIN_IMPL = BIN_IMPL_PART_2
};

class Node {
public:
    Node();

    virtual ~Node();

    Node *parent;
    Node *left;
    Node *right;
    char data = ' ';

    std::string toString();
};

class PllfTree {
public:
    PllfTree();

    virtual ~PllfTree();

    /*
     * Interpret input formula to tree
     */
    void generate(std::string str);

    /*
     * Generate tree by random
     */
    void randomGenerate();

    /*
     * Return string interpretation of formula tree
     */
    std::string toString();

    std::string getVariables();

    bool **getTruthTable();

    bool isTautology();

private:
    /*
     * Root node of tree
     */
    Node *root_;

    /*
     * Add binary operation to tree.
     * Take pointer to node of left operand.
     * Return pointer to node of right operand.
     */
    Node *addBinaryOperation(Node *current, NodeData operation);

    /*
     * Clean tree.
     */
    void Clean();

    /*
     * Depth-first search to get merged data from current node and its sons
     */
    std::string dfsGetData(Node *current);

    /*
     * Generate random formula nodes and data of the nodes for current node.
     */
    void generateFormulaNode(Node *current);

    /*
     * Generate random atom data for current node.
     */
    void generateAtomNode(Node *current);

    /*
     * Generate random constant data for current node.
     */
    void generateConstantNode(Node *current);

    /*
     * Generate random unary formula nodes and data of the nodes for current node.
     */
    void generateUnaryFormulaNode(Node *current);

    /*
     * Generate random binary formula nodes and data of the nodes for current node.
     */
    void generateBinaryFormulaNode(Node *current);

    /*
     * Depth-first calculate logical value of node
     */
    bool dfsCalculate(Node *current, std::map<char, bool> valueMap);
};

class BadSyntaxException : public std::exception {
public:
    const char *what() const noexcept override;
};

#endif //PLLFTREE_H
