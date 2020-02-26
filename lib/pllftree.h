//
// Created by ksan on 26.02.20.
//

#ifndef PLLFTREE_H
#define PLLFTREE_H

#include <exception>

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
    virtual ~Node();
    Node *parent;
    Node *left;
    Node *right;
    char data;
};

class PllfTree
{
public:
    PllfTree();

    virtual ~PllfTree();

    /*
     * Interpret input formula to tree
     */
    void generate(const char *str, int len);

private:
    Node *root_;
    Node *addBinaryOperation(Node *current, NodeData operation);
    void Clean();
};

class BadSyntaxException: public std::exception
{
public:
    const char* what() const noexcept override;
};

#endif //PLLFTREE_H
