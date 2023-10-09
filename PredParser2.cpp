/*
PredParser.cpp
*/


#include "PredParser2.h"
#include "Node.h"
#include "Token.h"
#include <string>
using namespace std;

PredParser::PredParser() { 
    pt = ParseTree(); 
}

PredParser::PredParser(vector<Token *> &input_toks) {
  toks = input_toks;
  lookahead = toks.begin();

  pt = ParseTree();
}

// print calls print function on the ParseTree
void PredParser::print() {
  // cout << "PredParser print function, toks has size " << toks.size() << "
  // lookahead pointed at " << (*lookahead)->tag << "\n";
  pt.print();
  cout << "\n";
}

// parse function fills in pt based on tokens in toks
// for the activity we start at Mul, but the full grammar would require starting
// at Exp
void PredParser::top_down_parse() {
    // set up root
    //Node *new_tree = new Node("start");
    // create root
    Node *new_tree = Stmt();

    
    // Node *child1 = Factor();
    // Node *child2 = Mul();
    // Node *child3 = Term();
    // Node *child4 = Add();
    // // fill in code
    // new_tree->children.push_back(child1);
    // new_tree->children.push_back(child2);
    // new_tree->children.push_back(child3);
    // new_tree->children.push_back(child4);

  
    pt.root = new_tree;
}


Node *PredParser::Stmt() {
    // grammar: rel | if ...
    Node *new_tree = new Node("S");
    // case: children is rel
    Node *c1 = Rel();
    new_tree->children.push_back(c1);

    return new_tree;
    
}

Node *PredParser::Rel() {
    // grammar: exp boolop
    Node *new_tree = new Node("R");

    // children is exp and boolop
    Node *c1 = Exp();
    Node *c2 = Boolop();
    new_tree->children.push_back(c1);
    new_tree->children.push_back(c2);

    return new_tree;
}

Node *PredParser::Boolop() {
    // grammar: bepis | bepis | ep
    Node *new_tree = new Node("B");
    // if lookahead at the end, simply create epsilon tree
    if (lookahead == toks.end()) {
        new_tree->children.push_back(new Node("ep"));
    } else {
        std::cout << "you done fucked up";
    }
    return new_tree;
}

Node *PredParser::Exp() {
    // create new tree with internal label T and retrieve current token
    Node *new_tree = new Node("E");
    
    // two children = tree for T and A
    Node *c1 = Term();
    Node *c2 = Add();
    new_tree->children.push_back(c1);
    new_tree->children.push_back(c2);

    return new_tree;
}

Node *PredParser::Add() {

    // create new tree with internal label M and retrieve current token
    Node *new_tree = new Node("A");

    // if lookahead at the end, simply create epsilon tree
    if (lookahead == toks.end()) {
        new_tree->children.push_back(new Node("ep"));
    } else {

        Token *current_tok = *lookahead;

        // check token for ARITHOP and "*" or "/"
        if (current_tok->tag == ARITHOP &&
            dynamic_cast<Arithop *>(current_tok)->op == "+"|| dynamic_cast<Arithop *>(current_tok)->op == "-") {
        // the three children are a leaf for *, tree for F, and tree for M
        if (dynamic_cast<Arithop *>(current_tok)->op == "-") {
            Node *c1 = new Node("*");
            new_tree->children.push_back(c1);
        } else {
            Node *c1 = new Node("+");
            new_tree->children.push_back(c1);
        }

        lookahead++;

        Node *c2 = Factor();
        Node *c3 = Mul();
        new_tree->children.push_back(c2);
        new_tree->children.push_back(c3);

        } else {
        cout << "Parse error in Mul\n";
        }
    }
    return new_tree;
}

Node *PredParser::Term() {
    // create new tree with internal label T and retrieve current token
    Node *new_tree = new Node("T");
    
    // two children = tree for F and M
    Node *c1 = Factor();
    Node *c2 = Mul();
    new_tree->children.push_back(c1);
    new_tree->children.push_back(c2);

    return new_tree;
}

// For Mul, apply * Factor Mul or / Factor Mul depending on lookahead, o/w do
// epsilon
Node *PredParser::Mul() {

    // create new tree with internal label M and retrieve current token
    Node *new_tree = new Node("M");

    // if lookahead at the end, simply create epsilon tree
    if (lookahead == toks.end()) {
        new_tree->children.push_back(new Node("ep"));
    } else {

        Token *current_tok = *lookahead;

        // check token for ARITHOP and "*" or "/"
        if (current_tok->tag == ARITHOP &&
            dynamic_cast<Arithop *>(current_tok)->op == "*"|| dynamic_cast<Arithop *>(current_tok)->op == "/") {
            // the three children are a leaf for *, tree for F, and tree for M
            if (dynamic_cast<Arithop *>(current_tok)->op == "*") {
                Node *c1 = new Node("*");
                new_tree->children.push_back(c1);
            } else {
                Node *c1 = new Node("/");
                new_tree->children.push_back(c1);
            }
            
            lookahead++;

            Node *c2 = Factor();
            Node *c3 = Mul();
            new_tree->children.push_back(c2);
            new_tree->children.push_back(c3);

        } else if (current_tok->tag == ARITHOP) { // probably bad way to handle this
            new_tree->children.push_back(new Node("ep"));
        } else {
        cout << "Parse error in Mul\n";
        }
    }
    return new_tree;
}

// If lookahead is num or var, return leaf node, otherwise return tree
// corresponding to (Exp)
Node *PredParser::Factor() {

  Node *new_tree = new Node("F");

  // if we are at the end of the token list there must be an error
  if (lookahead == toks.end()) {
    cout << "Error in parsing, Factor needs a token but none exist\n";
  } else {

    // retrieve token pointer from lookahead
    Token *current_tok = *lookahead;

    // if NUM is the current token, create leaf node "num"
    if (current_tok->tag == NUM) {
        Node *new_leaf = new Node("num");
        new_tree->children.push_back(new_leaf);
        lookahead++;
    } 
    else if (current_tok->tag == ID) {
        Node *new_leaf = new Node("id");
        new_tree->children.push_back(new_leaf);
        lookahead++;
    }
    else if (current_tok->tag == TRUE) { 
        Node *new_leaf = new Node("true");
        new_tree->children.push_back(new_leaf);
        lookahead++;
    }
    else if (current_tok->tag == FALSE) { 
        Node *new_leaf = new Node("false");
        new_tree->children.push_back(new_leaf);
        lookahead++;
    }
    // do (exp) case here
    else {
        cout << "Parse error in Factor\n";
    }
  }
  return new_tree;
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