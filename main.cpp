/*
Building a simple arithmetic parser.
Using simple top-down strategy.
Andrew Shallue, Fall 2023
*/

// Note: Grade PredParser2. Original PredParser kept for historical purposes - see push history.

#include <iostream>

#include "Token.h"
#include "Node.h"
#include "PredParser2.h"
#include "PredParser2.cpp"

using namespace std;

int main(){

    cout << "Hello World\n";

    // create a token list for 5 * 3
    vector<Token*> toks0;
    toks0.push_back(new Num(5));
    toks0.push_back(new Arithop("*"));
    toks0.push_back(new Num(3));
    
    cout << "Toks0: ";
    for(long i = 0; i < toks0.size(); i++){
        toks0.at(i)->print();
        cout << " ";
    }
    cout << "\n";

    PredParser pp0 = PredParser(toks0);
    pp0.top_down_parse();
    pp0.print();
    cout << "\n";

    // create a token list for 5 + 2 * 3
    vector<Token*> toks1;
    toks1.push_back(new Num(5));
    toks1.push_back(new Arithop("+"));
    toks1.push_back(new Num(2));
    toks1.push_back(new Arithop("*"));
    toks1.push_back(new Num(3));

    cout << "Toks1: ";
    for(long i = 0; i < toks1.size(); i++){
        toks1.at(i)->print();
        cout << " ";
    }
    cout << "\n";

    PredParser pp = PredParser(toks1);
    pp.top_down_parse();
    pp.print();
    cout << "\n";

    // now test predictive parsing
    PredParser pp1 = PredParser(toks1);
    pp1.top_down_parse();
    pp1.print();
    cout << "\n";

    // test 2:  5 * x + 2 * 3
    vector<Token*> toks2;
    toks2.push_back(new Num(5));
    toks2.push_back(new Arithop("*"));
    toks2.push_back(new Word(ID, "x"));
    toks2.push_back(new Arithop("+"));
    toks2.push_back(new Num(2));
    toks2.push_back(new Arithop("*"));
    toks2.push_back(new Num(3));
    cout << "Toks2: ";
    for(long i = 0; i < toks2.size(); i++){
        toks2.at(i)->print();
        cout << " ";
    }
    cout << "\n";
    
    PredParser pp2 = PredParser(toks2);
    pp2.top_down_parse();
    pp2.print();
    cout << "\n";

    // test 3:  (5 + x) * (5 + y)
    vector<Token*> toks3;
    std::cout << "1";
    toks3.push_back(new Punct("("));
    std::cout << "2";
    toks3.push_back(new Num(5));
    toks3.push_back(new Arithop("+"));
    toks3.push_back(new Word(ID, "x"));
    toks3.push_back(new Punct(")"));
    toks3.push_back(new Arithop("*"));
    toks3.push_back(new Punct("("));
    toks3.push_back(new Num(5));
    toks3.push_back(new Arithop("+"));
    toks3.push_back(new Word(ID, "y"));
    toks3.push_back(new Punct(")"));
    cout << "Toks3: ";
    for(long i = 0; i < toks3.size(); i++){
        toks3.at(i)->print();
        cout << " ";
    }
    cout << "\n\n";
    PredParser pp3 = PredParser(toks3);

    pp3.top_down_parse();
    pp3.print();

    cout << "\n";
    cout << pp3.pt.to_string() << "\n";

    // test 4:  5 * x
    vector<Token*> toks4;
    toks4.push_back(new Num(5));
    toks4.push_back(new Arithop("*"));
    toks4.push_back(new Word(ID, "x"));
    cout << "Toks4: ";
    for(long i = 0; i < toks4.size(); i++){
        toks4.at(i)->print();
        cout << " ";
    }
    cout << "\n";

    PredParser pp4 = PredParser(toks4);
    pp4.top_down_parse();
    pp4.print();
    cout << "\n";
    cout << pp4.pt.to_string() << "\n";

    // test 5: if(x < 3) x + 2 else true
    vector<Token*> toks5;
    toks5.push_back(new Word(IF, "if"));
    toks5.push_back(new Punct("("));
    toks5.push_back(new Word(ID, "x"));
    toks5.push_back(new Relop("<"));
    toks5.push_back(new Num(3));
    toks5.push_back(new Punct(")"));
    toks5.push_back(new Word(ID, "x"));
    toks5.push_back(new Arithop("+"));
    toks5.push_back(new Num(2));
    toks5.push_back(new Word(ELSE, "else"));
    toks5.push_back(new Word(TRUE, "true"));
    cout << "Toks5: ";
    for(long i = 0; i < toks5.size(); i++){
        toks5.at(i)->print();
        cout << " ";
    }
    cout << "\n";

    PredParser pp5 = PredParser(toks5);
    pp5.top_down_parse();
    pp5.print();
    cout << "\n";
    cout << pp5.pt.to_string() << "\n";

    // test 6: (2 * 2 * 2   should give a parse error
    vector<Token*> toks6;
    toks6.push_back(new Punct("("));
    toks6.push_back(new Num(2));
    toks6.push_back(new Arithop("*"));
    toks6.push_back(new Num(2));
    toks6.push_back(new Arithop("*"));
    toks6.push_back(new Num(2));

    cout << "Toks6: ";
    for(long i = 0; i < toks6.size(); i++){
        toks6.at(i)->print();
        cout << " ";
    }
    cout << "\n";

    PredParser pp6 = PredParser(toks6);
    pp6.top_down_parse();
    pp6.print();

}