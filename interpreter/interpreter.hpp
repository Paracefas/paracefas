#pragma once

#include "./../ast/ast.hpp"

class Interpreter {
    Parser parser;
public:
    Interpreter () =default;
    void prompt(std::string c = "> ") {
        std::cout << "[INFO]: Paracefas v.0.1.\n\n";
        while (true) {
            std::string input;
            std::cout << c;
            std::cin>> input;
            if (input == "exit") std::exit(0);
            auto program = parser.generateAbstractSyntaxTree(input);
            std::cout << program << "\n";
        }
    }
};