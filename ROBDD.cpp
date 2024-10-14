//
// Created by User on 16.11.2021.
//

#include "ROBDD.h"

const static unsigned TRUE = ~0;
const static unsigned FALSE = TRUE - 1;

Func::Func(bool b) : m_ciVar(b ? TRUE : FALSE) {}

Func::Func(unsigned iVar, const Func &t, const Func &e) : m_ciVar(iVar), m_cThen(&t), m_cElse(&e) {}

const Func *Func::getThen(unsigned int iVar) const { return iVar == m_ciVar ? m_cThen : this; }

const Func *Func::getElse(unsigned iVar) const { return iVar == m_ciVar ? m_cElse : this; }

unsigned Func::getVar() const { return m_ciVar; }

bool Func::isTrue() const { return m_ciVar == TRUE; }

bool Func::isFalse() const { return m_ciVar == FALSE; }

bool Func::isConstant() const { return isTrue() || isFalse(); }

std::ostream &operator<<(std::ostream &os, const Func &func) {
    os << "\"" << &func << "\"" << "->"
       << "\"" << func.m_cThen << "\"" << "[label=1]" << std::endl;

    os << "\"" << &func << "\"" << "->"
       << "\"" << func.m_cElse << "\"" << "[label=0,color=red]" << std::endl;
    return os;

}


Triple::Triple(unsigned iVar, const Func &fThen, const Func &fElse)
        : m_ciVar(iVar), m_cThen(fThen), m_cElse(fElse) {
}

bool Triple::operator<(const Triple &trip) const {
    return m_ciVar < trip.m_ciVar || (m_ciVar == trip.m_ciVar && &m_cElse < &trip.m_cElse) ||
           (m_ciVar == trip.m_ciVar && &m_cElse == &trip.m_cElse && &m_cThen < &trip.m_cThen);
}


FuncTriple::FuncTriple(const Func &funT, const Func &funTr, const Func &funTri) :
        func(funT), funcT(funTr), funcTr(funTri) {}

bool FuncTriple::operator<(const FuncTriple &trip) const {
    return &func < &trip.func || (&func == &trip.func && &funcT < &trip.funcT) ||
           (&func == &trip.func && &funcT == &trip.funcT && &funcTr < &trip.funcTr);
}


ROBDD::ROBDD() : m_cTrue(new Func(1)), m_cFalse(new Func(0)),m_Unique(),m_Computed(),m_names() {}

ROBDD::~ROBDD() {
    delete m_cFalse;
    delete m_cTrue;
    destroy();
}

void ROBDD::destroy() {
    for (auto &x: m_Unique)
        delete x.second;
}

const Func &ROBDD::genTrue() const {
    return *m_cTrue;
}

const Func &ROBDD::genFalse() const {
    return *m_cFalse;
}

const Func &ROBDD::genVar(const std::string &st) {
    auto resstring = m_names.find(st);
    if (resstring == m_names.end()) {
        ++cnt;
        resstring = m_names.insert({st, cnt}).first;

    }
    Triple entry(resstring->second, genTrue(), genFalse());
    auto res = m_Unique.find(entry);
    if (res == m_Unique.end())
        res = m_Unique.insert({entry, new Func(resstring->second, genTrue(), genFalse())}).first;
    return *res->second;

}


const Func &ROBDD::AND(const Func &f, const Func &g) {
    return ite(f, g, genFalse());
}

const Func &ROBDD::NOT(const Func &f) {
    return ite(f, genFalse(), genTrue());
}


const Func &ROBDD::OR(const Func &f, const Func &g) {
    return ite(f, genTrue(), g);
}


const Func &ROBDD::XOR(const Func &f, const Func &g) {
    return ite(f, NOT(g), g);
}


const Func &ROBDD::ite(const Func &i, const Func &t, const Func &e) {
    if (i.isTrue())
        return t;
    else if (i.isFalse())
        return e;
    else if (t.isTrue() && e.isFalse())
        return i;
    else {
        FuncTriple args(i, t, e);
        auto res = m_Computed.find(args);
        if (res != m_Computed.end())
            return *res->second;
        const unsigned ciVar = std::min(std::min(i.getVar(), t.getVar()), e.getVar());
        const Func &TH = ite(*i.getThen(ciVar), *t.getThen(ciVar), *e.getThen(ciVar));
        const Func &EL = ite(*i.getElse(ciVar), *t.getElse(ciVar), *e.getElse(ciVar));
        if (&TH == &EL) {
            m_Computed.insert({args, &TH});
            return TH;
        }
        Triple entry(ciVar, TH, EL);
        auto ress = m_Unique.find(entry);
        if (ress == m_Unique.end())
            ress = m_Unique.insert({entry, new Func(ciVar, TH, EL)}).first;
        m_Computed.insert({args, ress->second});
        return *ress->second;
    }
}


