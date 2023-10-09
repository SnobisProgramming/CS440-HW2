/* Code for a basic lexer.  Copied from Dragon Book, page 81
Andrew Shallue, August 2023
*/

#include <iostream>
#include <string>

using namespace std;

#ifndef TOKEN_H
#define TOKEN_H

// base class that has only a tag, no values.  Tags are represented as integers.
class Token{
    public:
        int tag;
        Token(){ tag = 0; }
        Token(int t){ tag = t; }

        // print will print <tag> to cout.  toString will create a string with just the tag
        virtual void print(){ std::cout << "<" << tag << ">"; }

        // two tokens are equal if the tag is the same
        virtual bool equal(Token &other){ return tag == other.tag; }
};

// creating tags by assigning unique numbers to keywords
// could also do #define NUM 256.  Currently missing static
#define NUM 256
#define ID 257
#define TRUE 258
#define FALSE 259
#define IF 260
#define ELSE 261
#define RELOP 270
#define ARITHOP 271
#define ERROR 0
#define PUNCT 300

// two subclasses of Token.  Set the tag, and include an extra piece of information.
// Help with how to call parent class constructor from: 
// https://www.tutorialspoint.com/what-are-the-rules-for-calling-the-superclass-constructor-cplusplus
// Help with downcasting:  https://www.geeksforgeeks.org/dynamic-_cast-in-cpp/
class Num : public Token{
    public:
        double value;
        Num() : Token() {}
        Num(double v) : Token(NUM) {
            value = v;
        }

        // print will print <tag, value> to cout.  
        void print() override{ std::cout << "<" << tag << ", " << value << ">"; }

        // two nums are equal if the values match.  Note that the tags must match since both are nums
        // use downcasting to detect if other has type Num.  If downcast fails, no match, return false
        bool equal(Token &other) override{ 
            Num* downcast = dynamic_cast<Num*>(&other);
            if(downcast){
                return value == downcast->value;
            }else return false;
        }
};

class Word : public Token{
    public:
        string lexeme;

        Word() : Token() {}
        Word(int t, string s) : Token(t) { lexeme = s; }

        // print will print <tag, value> to cout.  
        void print() override{ std::cout << "<" << tag << ", " << lexeme << ">"; }

        // two words are equal if tag and value both match.  Same downcast trick
        bool equal(Token &other) override { 
            Word* downcast = dynamic_cast<Word*>(&other);
            if(downcast){
                return lexeme == downcast->lexeme && tag == downcast->tag;
            }
            else return false;
        }
};

class Relop : public Token{
    public:
        string op;

        Relop() : Token() {}
        Relop(string o) : Token(RELOP) { op = o; }

        // print will print <tag, value> to cout.  
        void print() override{ std::cout << "<" << tag << ", " << op << ">"; }

        // two Relops are equal if values match.  Same downcast trick
        bool equal(Token &other) override { 
            Relop* downcast = dynamic_cast<Relop*>(&other);
            if(downcast){
                return op == downcast->op;
            }else return false;
        }
};

class Arithop : public Token{
    public:
        string op;

        Arithop() : Token() {}
        Arithop(string o) : Token(ARITHOP) { op = o; }

        // print will print <tag, value> to cout.  
        void print() override{ std::cout << "<" << tag << ", " << op << ">"; }

        // two Relops are equal if values match.  Same downcast trick
        bool equal(Token &other) override { 
            Arithop* downcast = dynamic_cast<Arithop*>(&other);
            if(downcast){
                return op == downcast->op;
            }else return false;
        }
};

class Punct : public Token{
    public:
        string symbol;

        Punct() : Token() {}
        Punct(string p) : Token(PUNCT) { symbol = p; }

        // print will print <tag, value> to cout.  
        void print() override{ std::cout << "<" << tag << ", " << symbol << ">"; }

        // two Relops are equal if values match.  Same downcast trick
        bool equal(Token &other) override { 
            Punct* downcast = dynamic_cast<Punct*>(&other);
            if(downcast){
                return symbol == downcast->symbol;
            }else return false;
        }
};


#endif