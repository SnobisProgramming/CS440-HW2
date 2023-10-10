/*
PredParser.cpp
*/

// Note: Grade PredParser2. Original PredParser kept for historical purposes - see push history.

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

    pt = ParseTree();
}

// from Dr. Shallue
// Prints ParseTree
void PredParser::print() {
  std::cout << "PredParser print function, toks has size " << toks.size() << " lookahead pointed at " << (*lookahead)->tag << "\n";
  pt.print();
  cout << "\n";
}

// from Dr. Shallue
// Does a top down parse
void PredParser::top_down_parse() {
    // create root
    Node* newNode = Stmt();

    pt.root = newNode;
}

// Helper function to update currentTok.
// Also helps avoiding scope issues (where we'd have to define currentTok multiple times below)
// Choosing to use this at the end of each function except for outputTree
void PredParser::updateCurrentTok() {
    if (lookahead != toks.end()) {
        // With this order, the function can be called with currentTok being at the start.
        currentTok = *lookahead;
        lookahead++;
    }
    else {
        std::cout << "Error: Tried to iterate at end of tok list";
    }
}

// Stmt checks for if.  If not found, go with Rel production instead
Node* PredParser::Stmt() {
    // Create parent node for the statement
    Node* outputTree = new Node("S");

    // Initialize currentTok at beginning
    // Should be Rel or if
    updateCurrentTok();
          
        // Check if lookahead token is "if"
        if (currentTok->tag == IF) {
            // Create new node for the if statement
            Node* ifNode = new Node("if");

            // Add ifNode as child of outputTree
            outputTree->children.push_back(outputTree);


            // Update currentTok, Should be (
            updateCurrentTok();

            // According to our grammar, (Rel) is next, check for (
            // Due to a discussion on polymorphism, I must cast Punct* and check for symbol. Also ensuring tag is PUNCT to be fully sure.
            if (currentTok->tag == PUNCT && dynamic_cast<Punct*>(currentTok)->symbol == "(") {
                // Create new node for ( for outputTree
                Node* leftParen = new Node("(");
                outputTree->children.push_back(leftParen);

                // Update currentTok, should be Rel
                updateCurrentTok();

                // We should have Rel; parse Rel
                Node* relNode = Rel();

                // Rel is a child of an if statement
                outputTree->children.push_back(relNode);

                // Update currentTok, should be )
                updateCurrentTok();

                // According to our grammar, (Rel) ends in a )
                // Thus, check for )
                // Due to a discussion on polymorphism, I must cast Punct* and check for symbol. Also ensuring tag is PUNCT to be fully sure.
                if (currentTok->tag == PUNCT && dynamic_cast<Punct*>(currentTok)->symbol == ")") {
                // Create new node for ) for outputTree
                Node* rightParen = new Node(")");
                outputTree->children.push_back(rightParen);
                
                // Update currentTok, should be else
                updateCurrentTok();

                // We should have (then) Stmt; parse Stmt
                Node* thenStmt = Stmt();

                // (then) Stmt is a child of an if statement
                outputTree->children.push_back(thenStmt);

                // Think this is old code, keeping around for testing
                // Check for else
                    if (currentTok->tag == ELSE) {
                        // We should have Stmt; parse Stmt
                        Node* elseStmt = Stmt();

                        // (else) Stmt is a child of an if statement
                        outputTree->children.push_back(elseStmt)

                        // Update currentTok, should be Stmt
                        updateCurrentTok();
                    else {
                        std::cout << "Error: Missing Else in Stmt";
                    }
                }

                    // Finished parsing outputTree
                    return outputTree;
            
                }
                // case: ) missing
                else {
                    std::cout << "Error: Missing ) in Stmt";
                }
            }
                // case: ( missing
                else {
                    std::cout << "Error: Missing ( in Stmt";
            }
        }
    // Otherwise, currentTok is not "if", thus return Rel
    return Rel();
}

// Rel only has one choice, to go to Exp Relop
Node* PredParser::Rel() {
    // Create parent node for Rel
    Node* relNode = new Node("R");

    // Update currentTok, should be Exp
    updateCurrentTok();

    // Parse Exp
    Node* expNode = Exp();

    // Add as child of boolNode
    relNode->children.push_back(expNode);

    // Parse Boolop
    Node* boolNode = Boolop();

    // Add as child of boolNode
    relNode->children.push_back(expNode);

    return relNode;
};

// Boolop checks for < or >, if neither found go with epsilon
Node* PredParser::Boolop() {
    // Create parent node for Boolop
    Node* boolNode = new Node("B");

    // Update currentTok, should be < or >
    updateCurrentTok();

    // Check for <, >
    if (currentTok->tag == PUNCT && (dynamic_cast<Relop*>(currentTok)->op == "<" || dynamic_cast<Relop*>(currentTok)->op == ">")) {
        // Create new node for operator
        Node* opNode = new Node(dynamic_cast<Relop*>(currentTok)->op);

        // Add opNode as a child of boolNode
        boolNode->children.push_back(opNode);

        // Update currentTok, should be Exp
        updateCurrentTok();

        // Parse Exp
        Node* expNode = Exp();

        // Add as child of boolNode
        boolNode->children.push_back(expNode);

        return boolNode;
    }

    // Otherwise epsilon
    return nullptr;
}

// Start with Exp, only one production, namely Term Add
Node* PredParser::Exp() {
    // Create parent node for Exp
    Node* expNode = new Node("E");

    // Update currentTok, should be Term
    updateCurrentTok();

    // Parse Term
    Node* termNode = Term();

    // Add it to expNode
    expNode->children.push_back(termNode);

    // Update currentTok, should be Add
    updateCurrentTok();

    // Parse Add
    Node* addNode = Add();

    // Add it to expNode
    expNode->children.push_back(addNode);

    return expNode;

}

// Three choices for Add, which depends on the lookahead token
Node* PredParser::Add() {
    // Create parent node for Add
    Node* addNode = new Node("A");

    // Update currentTok, should be +, - or epsilon
    updateCurrentTok();

    // Case: Addition, subtraction
    // These two cases would have the same output, so we can make one if statement for them
    if (currentTok->tag == PUNCT && (dynamic_cast<Arithop*>(currentTok)->op == "+" || dynamic_cast<Arithop*>(currentTok)->op == "-")) {
        // Create new node for operator
        Node* opNode = new Node(dynamic_cast<Arithop*>(currentTok)->op);

        // Add opNode as a child of A
        addNode->children.push_back(opNode);

        // Update currentTok, should be Term
        updateCurrentTok();

        // Now parse factor
        Node* termNode = Term();

        // Add Factor node as child of M
        addNode->children.push_back(termNode);

        // Update currentTok, should be Add
        updateCurrentTok();

        // Recursively parse Add as our grammar states
        // using nextMulNode as the var name to indicate it's the Mul inside the grammar
        Node* nextAddNode = Add();

        // Add nextMulNode as a child of current mulNode
        addNode->children.push_back(nextAddNode);

        // Finished parsing Mul
        // Note: If epsilon case, the above if won't be met, simply returning "M". Correct implementation?
        return addNode;
    }

    else {
        return nullptr;
    }
}

// If Term is current nonterminal, only one production to choose, namely Factor Mul
Node* PredParser::Term() {
    // Create paren node for Term
    Node* termNode = new Node("T");

    // Parse Factor
    Node* factorNode = Factor();

    // Add Factor node as a child of T
    termNode->children.push_back(termNode);

    // Update currentTok, should be Mul
    updateCurrentTok();

    // Parse Mul
    Node* mulNode = Mul();

    // Add mulNode as a child of T
    mulNode->children.push_back(termNode);

    // Return Term
    return termNode;

}

// For Mul, apply * Factor Mul or / Factor Mul depending on lookahead, o/w do epsilon
Node* PredParser::Mul() {
    // Create parent node for Mul
    Node* mulNode = new Node("M");

    // Update currentTok, should be * or /
    updateCurrentTok();

    // Case: Multiplication or division
    // These two cases would have the same output, so we can make one if statement for them
    if (currentTok->tag == PUNCT && (dynamic_cast<Arithop*>(currentTok)->op == "*" || dynamic_cast<Arithop*>(currentTok)->op == "/")) {
        // Create new node for operator
        Node* opNode = new Node(dynamic_cast<Arithop*>(currentTok)->op);

        // Add opNode as a child of M
        mulNode->children.push_back(opNode);

        // Update currentTok, should be Factor
        updateCurrentTok();

        // Now parse factor
        Node* factorNode = Factor();

        // Add Factor node as child of M
        mulNode->children.push_back(factorNode);

        // Update currentTok, should be Mul
        updateCurrentTok();

        // Recursively parse Mul as our grammar states
        // using nextMulNode as the var name to indicate it's the Mul inside the grammar
        Node* nextMulNode = Mul();

        // Add nextMulNode as a child of current mulNode
        mulNode->children.push_back(nextMulNode);
    }

    // Finished parsing Mul
    // Note: If epsilon case, the above if won't be met, simply returning "M". Correct implementation?
    return mulNode;
}

//If lookahead is num or var, return leaf node, otherwise return tree corresponding to (Exp)
Node* PredParser::Factor() {
    
    // Create parent node for Factor
    Node* factorNode = new Node("F");

    // Update curentTok, should be num/var/true/false/(
    updateCurrentTok();

    // Case: Factor is a num
    if (currentTok->tag == NUM) {
        // Create new node for num
        // Aware that I can do this on one line, but doing it on two lines helps me understand what's going on.
        Node* numNode = new Node("num");
        // Add numNode as a child of F
        factorNode->children.push_back(numNode);
    }

    // Case: Factor is a var
    else if (currentTok->tag == ID) {
        // Create new node for var
        Node* varNode = new Node("id");
        // Add varNode as child of F
        factorNode->children.push_back(varNode);
    }

    // Case: Factor is true
    else if (currentTok->tag == TRUE) {
        // Create new node for true
        Node* trueNode = new Node("true");
        // Add trueNode as child of F
        factorNode->children.push_back(trueNode);
    }

    // Case: Factor is false
    else if (currentTok->tag == FALSE) {
        // Create new node for false
        Node* falseNode = new Node("false");
        // Add falseNode as child of F
        factorNode->children.push_back(falseNode);
    }

    // ** uncomment after implementing + testing exp **

    // Case: Factor is (Exp)
    // Ensure currentTok is (
    else if (currentTok->tag == PUNCT && dynamic_cast<Punct*>(currentTok)->symbol == "(") {
        // Note: Need to be careful when I lookahead if I do so after all these ifs.
        // Create new node for ( for F
        Node* leftParen = new Node("(");
        // Add leftParen as child of F
        factorNode->children.push_back(leftParen);

        // Update currentTok, should be Exp
        updateCurrentTok();

        // Now we call Exp()
        Node* expNode = Exp();
        // Add the exp as a child of factor
        factorNode->children.push_back(expNode);

        // Update currentTok, should be )
        updateCurrentTok();
       
        // Case: Ensure currentTok is )
        if (currentTok->tag == PUNCT && dynamic_cast<Punct*>(currentTok)->symbol == ")") {
            // Create new node for ) for F
            Node* rightParen = new Node(")");

            // Add leftParen as child of F
            factorNode->children.push_back(rightParen);
        }

        // Case: Missing closing paren
        // Note: Or maybe expr is bad? Probably not - the error would be in there (once I implent it), right?
        else {
            std::cout << "Error: Missing ), or maybe Expr is bad in Factor.";
        }
    }
    // Case: Invalid factor inputted
    else {
        std::cout << "Error: Invalid factor inputted for Factor.";
    }
    return factorNode;
}

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