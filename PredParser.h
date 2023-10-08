/*
C++ Parser, Andrew Shallue, CS 440, Fall 2023
Designing a simple predictive parser for arithmetic, following Chap 2 of the Dragon Book
*/

#ifndef PREDPARSER_H
#define PREDPARSER_H

#include "Node.h"
#include "Token.h"
#include <vector>

class PredParser {
    public:
        PredParser();
        PredParser(std::vector<Token*> &input_toks);
        void updateCurrentTok();
        void print();
        void top_down_parse();
        Node* Stmt();
        Node* Rel();
        Node* Boolop();
        Node* Exp();
        Node* Add();
        Node* Term();
        Node* Mul();
        Node* Factor();
    
    private:
        ParseTree pt;
        std::vector<Token*> toks;
        std::vector<Token*>:iterator lookahead;
};

#endif