/*
Classes for creating a parse tree.
Source: Dragon book, Chapter 2 and appendix
*/

#ifndef NODE_H
#define NODE_H

#include "Token.h"
#include <string>
#include <vector>

using namespace std;

// A Node stores a symbol and a vector of pointers to its children
class Node{
  public:
    string symbol;  // could be a nonterminal or a terminal
    vector<Node*> children;

    Node(){ symbol = ""; }
    Node(string T){ symbol = T; }

    // to print, print nonTerm, followed by :, then print all children
    void print(){
      cout << symbol;
      if(children.size() > 0) cout << ": ";
      for(long i = 0; i < children.size(); i++){
        children.at(i)->print();
        cout << " ";
      }
    }

    // helper function for a multi-line print.  Notice that tree is passed by reference
    void to_string_helper(string &tree, string space){
      // base case: if no children, just print symbol
      if(children.size() == 0){
        tree.append(symbol);
      }else{
        // otherwise, print symbol, new line, spacing for each child
        tree.append(symbol);
        string bigger_space = space;
        bigger_space.append("   ");
        
        for(long i = 0; i < children.size(); i++){
          tree.append("\n");
          tree.append(space);
          tree.append("|");

          children.at(i)->to_string_helper(tree, bigger_space);
        }
      }
    }
};

// A ParseTree is made of nodes, but the only actual data is the pointer to the root
class ParseTree{
  public:
    Node* root;
    ParseTree(){ root = nullptr; }

    // recursive deep copy on a given node
    Node* rec_copy(Node* current){
      // create new node
      Node* new_tree = new Node(current->symbol);
      // now copy the children
      for(long i = 0; i < current->children.size(); i++){
        Node* new_child = rec_copy(current->children.at(i));
        new_tree->children.push_back(new_child);
      }
      return new_tree;
    }

    // rule of 3 for ParseTree.  Simply apply the Node function to the root
    ParseTree(ParseTree &other){ 
      // copy the other tree if it is not null
      if(other.root){
        Node* new_tree = rec_copy(other.root);
        root = new_tree;
      }else root = nullptr;
    }
    ParseTree& operator=(ParseTree& other){
      ParseTree new_pt;

      if(other.root){
        Node* new_tree = rec_copy(other.root);
        new_pt.root = new_tree;
      }else new_pt.root = nullptr;

      return(new_pt);
    }
    // recursive delete on a given node
    void rec_delete(Node* current){
      //cout << "calling rec_delete on " << current->symbol << "\n";
      // delete all children recursively
      for(long i = 0; i < current->children.size(); i++){
        rec_delete(current->children.at(i));
      }
      // now delete current
      delete current;
    }

    ~ParseTree(){
      //cout << "ParseTree destructor\n";
      if(root){
        rec_delete(root);
        root = nullptr;
      }else root = nullptr;
    }

    // print function prints to stdout, calls print on roote node
    void print(){ 
      //cout << "ParseTree.print() called\n"; 
      if(root){
        root->print();
      }else{
        cout << "empty tree\n";
      } 
    }

    // to_string calls the to_string helper function on the root node
    string to_string(){
      if(root){
        string tree_string = "";
        root->to_string_helper(tree_string, "   ");
        return tree_string;
      }else{
        return "Empty";
      }
    }
};

#endif