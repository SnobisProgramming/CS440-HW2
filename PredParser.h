/*
C++ Parser, Andrew Shallue, CS 440, Fall 2023
Designing a simple predictive parser for arithmetic, following Chap 2 of the Dragon Book
*/

#ifndef PREDPARSER_H
#define PREDPARSER_H

#include "Node.h"
#include "Token.h"
#include <string>

class PredParser{
    public:
        ParseTree pt;
        vector<Token*> toks;

        // lookahead is an iterator that points at the next token in toks
        vector<Token*>::iterator lookahead;

    public:
        PredParser();
        PredParser(vector<Token*> &input_toks);

        // print calls print function on the ParseTree
        void print();

        // parse function fills in pt based on tokens in toks
        void top_down_parse();

        // match is for leaf tokens
        // check if lookahead not at end, check if lookahead is correct token, if so create leaf and push ahead
        // never mind, not implemented.  I would need a million cases to cast appropriate token depending on tag
        // Is there a way to detect the type of an object passed?
        //Node* match(int t, string s);

        // if there is a way (polymorphic equal?), we could instead do:
        //Node* match()

        // if not, then function probably should be
        //Node* match(int t, string s, string type)?

        // Stmt checks for if.  If not found, go with Rel production instead
        Node* Stmt() {
          // creation of the parent node for the statement
          Node* output_tree = new Node("S");
          
          // Check if lookahead token is "if"
          if ((*lookahead)->tag() == IF) {
            // Create new node for the if statement
            Node* ifStmt = new Node("if");
            // Lookahead
            lookahead++;
          }

          // According to our grammar, (Rel) is next
          // Thus, check for (
          if ((*lookahead)->lexeme == "(") {
            // Lookahead upon confirming (
            lookahead++;

            // We should have Rel; parse Rel
            Node* relNode = Relop();

            // Rel is a child of an if statement
            ifStmt->children.push_back(relNode);

            // According to our grammar, (Rel) ends in a )
            // Thus, check for )
            if ((*lookahead)->lexeme == ")") {
              // Lookahead upon confirming )
              lookahead++;

              // We should have (then) Stmt; parse Stmt
              Node* thenStmt = Stmt();

              // (then) Stmt is a child of an if statement
              ifStmt->children.push_back(thenStmt);

              // Check for else
              // question - is else required in our grammar?
              if ((*lookahead)->tag == ELSE) {
                // Look ahead
                lookahead++;

                // We should have Stmt; parse Stmt
                Node* elseStmt = Stmt();

                // (else) Stmt is a child of an if statement
                ifStmt->children.push_back(elseStmt)
              }

              // Finished parsing ifStmt
              return ifStmt;
          
            }
            // if needed
            else {
              // case: ) missing
            }
          else {
            // case: ( missing
          }
        else {
          // return relop?
          
        }
        }
        }
        // Rel only has one choice, to go to Exp Relop
        Node* Rel();
        // Boolop checks for < or >, if neither found go with epsilon
        Node* Boolop();
        // Start with Exp, only one production, namely Term Add
        Node* Exp();
        // Three choices for Add, which depends on the lookahead token
        Node* Add();
        // If Term is current nonterminal, only one production to choose, namely Factor Mul
        Node* Term();
        // For Mul, apply * Factor Mul or / Factor Mul depending on lookahead, o/w do epsilon
        Node* Mul();
        //If lookahead is num or var, return leaf node, otherwise return tree corresponding to (Exp)
        Node* Factor();
};

/* Here is the grammar that this parser will parse:
Stmt -> Rel | if ( Rel ) Stmt else Stmt 
Rel -> Exp Boolop
Boolop -> < Exp | > Exp | epsilon
Exp -> Term Add
Add -> + Term Add | - Term Add | epsilon
Term -> Factor Mul
Mul -> * Factor Mul | / Factor Mul | epsilon
Factor -> num | var | true | false | ( Exp )
*/

#endif