/*
PredParser.cpp
*/


#include "PredParser.h"
#include "Node.h"
#include "Token.h"
#include <string>

// Constructor
PredParser::PredParser() {
    // Lookahead starts at beginning of token vector
    lookahead = toks.begin();
}

PredParser::PredParser(std::vector<Token*> &input_toks) {
    toks = input_toks;
    // Lookahead starts at beginning of token vector
    lookahead = toks.begin();
}

// Prints ParseTree
void PredParser::print() {

}

// Does a top down parse
// Call Stmt()?
void PredParser::top_down_parse() {

}

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
Node* PredParser::Stmt() {
    // Creation of the parent node for the statement
    Node* output_tree = new Node("S");

    // This variable will store what lookahead points at, so I don't have dereference what lookahead points at.
    // Intended to increase clarity of the code and help me not mix up my pointers/dereferences.
    Token* currenTok = *lookahead;
          
        // Check if lookahead token is "if"
        if ((currentTok)->tag == IF) {
            // Create new node for the if statement
            Node* ifStmt = new Node("if");
            // Lookahead
            lookahead++;

            // Update currentTok
            correntTok = *lookahead;

            // According to our grammar, (Rel) is next
            // Thus, check for (
            // Due to a discussion on polymorphism, I must cast Punct* and check for symbol. Also ensuring tag is PUNCT to be fully sure.
            if ((currentTok)->tag == PUNCT && dynamic_cast<Punct*>(currentTok)->symbol == "(") {
                // Create new node for ( for ifStmt
                Node* leftParen = new Node("(");
                ifStmt->children.push_back(leftParen);

                // Lookahead
                lookahead++;
                // Update currentTok
                correntTok = *lookahead;

                // We should have Rel; parse Rel
                Node* relNode = Relop();

                // Rel is a child of an if statement
                ifStmt->children.push_back(relNode);

                // According to our grammar, (Rel) ends in a )
                // Thus, check for )
                // Due to a discussion on polymorphism, I must cast Punct* and check for symbol. Also ensuring tag is PUNCT to be fully sure.
                if ((currentTok)->tag == PUNCT && dynamic_cast<Punct*>(currentTok)->symbol == ")") {
                // Create new node for ) for ifStmt
                Node* leftParen = new Node(")");
                ifStmt->children.push_back(leftParen);
                
                // Lookahead upon confirming ); Update currentTok
                lookahead++;
                correntTok = *lookahead;

                // We should have (then) Stmt; parse Stmt
                Node* thenStmt = Stmt();

                // (then) Stmt is a child of an if statement
                ifStmt->children.push_back(thenStmt);

                // Check for else
                    if ((*lookahead)->tag == ELSE) {
                        // Look ahead; Update currentTok
                        lookahead++;
                        correntTok = *lookahead;

                        // We should have Stmt; parse Stmt
                        Node* elseStmt = Stmt();

                        // (else) Stmt is a child of an if statement
                        ifStmt->children.push_back(elseStmt)
                }

                    // Finished parsing ifStmt. This should have S->if...
                    // Check this in office hours if not tested by fri
                    return ifStmt;
            
                }
                // case: ) missing
                else {
                    std::cout << "Error: Missing ) in ifStmt";
                }
            }
                // case: ( missing
                else {
                    std::cout << "Error: Missing ( in ifStmt";
            }
        }
    // Otherwise, currentTok is not "if", thus return Rel
    return Rel();
}

// Rel only has one choice, to go to Exp Relop
Node* PredParser::Rel();

// Boolop checks for < or >, if neither found go with epsilon
Node* PredParser::Boolop();

// Start with Exp, only one production, namely Term Add
Node* PredParser::Exp();

// Three choices for Add, which depends on the lookahead token
Node* PredParser::Add();

// If Term is current nonterminal, only one production to choose, namely Factor Mul
Node* PredParser::Term();

// For Mul, apply * Factor Mul or / Factor Mul depending on lookahead, o/w do epsilon
Node* PredParser::Mul();

//If lookahead is num or var, return leaf node, otherwise return tree corresponding to (Exp)
Node* PredParser::Factor();

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