//
// Created by User on 10.11.2021.
//

#ifndef ASSIG1_ROBDD_H
#define ASSIG1_ROBDD_H

#include <iostream>
#include <map>
#include "RedBlackTree.h"
#include <string>


class Func {

public:

    Func(const Func &fun) = delete;

    Func &operator=(const Func &fun) = delete;

    explicit Func(bool b);

    Func(unsigned iVar, const Func &t, const Func &e);

    const Func *getThen(unsigned iVar) const;

    const Func *getElse(unsigned iVar) const;

    unsigned getVar() const;

    bool isTrue() const;

    bool isFalse() const;

    bool isConstant() const;

    friend std::ostream &operator<<(std::ostream &os, const Func &);


private:
    unsigned m_ciVar = 0;
    const Func *m_cThen = nullptr;
    const Func *m_cElse = nullptr;

};

class Triple {
public :
    Triple(unsigned iVar, const Func &fThen, const Func &fElse);

    bool operator<(const Triple &trip) const;

private :
    unsigned m_ciVar = 0;
    const Func &m_cThen;
    const Func &m_cElse;
};

class FuncTriple {
public:
    FuncTriple(const Func &funT, const Func &funTr, const Func &funTri);

    bool operator<(const FuncTriple &trip) const;

private :
    const Func &func;
    const Func &funcT;
    const Func &funcTr;
};

class ROBDD {

public:
    ROBDD();

    ~ROBDD();

    ROBDD(const ROBDD &r) = default;

    ROBDD &operator=(const ROBDD &r) = default;

    void destroy();

    const Func &genVar(const std::string &st);

    const Func &genTrue() const;

    const Func &genFalse() const;

    const Func &AND(const Func &f, const Func &g);

    const Func &OR(const Func &f, const Func &g);

    const Func &NOT(const Func &f);

    const Func &XOR(const Func &f, const Func &g);

    const Func &ite(const Func &i, const Func &t, const Func &e);

private :
    const Func *m_cTrue = nullptr;
    const Func *m_cFalse = nullptr;
   RedBlackTree<Triple, const Func *> m_Unique;
    RedBlackTree<FuncTriple, const Func *> m_Computed;
    RedBlackTree<std::string, unsigned> m_names;
    unsigned cnt = 0;

};


#endif //ASSIG1_ROBDD_H
