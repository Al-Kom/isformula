// Laboratory work 2 on the discipline LOIS
// Completed by student of group 721702 BSUIR Komar Alexander Nazarovich
// CPP-file main file
// Version 1. Add interactive console UI
//

#include <iostream>
#include <cmath>
#include "lib/pllftree.h"

void check();

void test();

void calc();

int main() {
    for (;;) {
        std::cout << "Enter 1 to check formula, 2 to test yourself,"
                     " 3 to calculate truth table, else to exit\n";
        int choice = 0;
        std::cin >> choice;
        switch (choice) {
            case 1:
                check();
                continue;
            case 2:
                test();
                continue;
            case 3:
                calc();
                continue;
            default:
                std::cout << "exit!\n";
        }
        break;
    }
    return 0;
}

void check() {
    std::cout << "Enter a string to check\n";
    std::string str;
    std::cin >> str;
    PllfTree tree;
    try {
        tree.generate(str);
        if (tree.isTautology())
            std::cout << "String " + str + " is tautology\n";
        else
            std::cout << "String " + str + " is not tautology\n";
    } catch (BadSyntaxException &ex) {
        std::cerr << ex.what() << std::endl;
    }
}

void test() {
    // uncomment to make real random
//    srand(time(0));
    PllfTree tree;
    tree.randomGenerate();
    std::string test = tree.toString();
    std::cout << "Is it a tautology: " + test + "? (Enter 1 if yes, else if not)\n";
    int answer;
    std::cin >> answer;
    if ((answer == 1 && tree.isTautology()) || (answer != 1 && !tree.isTautology()))
        std::cout << "Right!\n";
    else
        std::cout << "Wrong!\n";
}

void calc() {
    std::cout << "Enter a string to calculate the truth table\n";
    std::string str;
    std::cin >> str;
    PllfTree tree;
    try {
        tree.generate(str);
        // init table
        std::string vars = tree.getVariables();
        int colsN = (int) vars.length() + 1;
        int rowsN = (int) pow(2, vars.length());
        bool **truthTable;
        truthTable = tree.getTruthTable();
        // output
        for (int i = 0; i < rowsN; ++i) {
            for (int j = 0; j < colsN; ++j) {
                std::cout << truthTable[i][j] << " ";
            }
            std::cout << "\n";
        }
    } catch (BadSyntaxException &ex) {
        std::cerr << ex.what() << std::endl;
    }
}


