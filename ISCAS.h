#ifndef __ISCAS_H__
#define __ISCAS_H__

#include <vector>
#include <string>
#include <tuple>
#include <iostream>

class ISCAS {
public: // types

    enum Kind {AND,NAND,OR,XOR,NOR,NOT};

    typedef std::vector<std::string> StrVec;
    typedef std::tuple<std::string,Kind,StrVec> Assignment;
    typedef std::vector<Assignment> Assignments;

public: // constructor

    ISCAS(FILE*);


public: // operators

    friend std::ostream& operator<<(std::ostream& os,const ISCAS& crArg) {
        crArg.print(os);
        return os;
    }

public: // methods

    void addInput(const std::string&);
    void addOutput(const std::string&);
    void addAssignment(const Assignment&);

    const StrVec& inputs() const {
        return m_Inputs;
    }

    const StrVec& outputs() const {
        return m_Outputs;
    }

    const Assignments& assignments() const {
        return m_Assignments;
    }

    bool isValid() const {
        return m_bValid;
    }

private: // methods

    void print(std::ostream&) const;

private: // members

    StrVec		m_Inputs;
    StrVec		m_Outputs;
    Assignments	m_Assignments;
    bool		m_bValid = true;
};

#endif // __ISCAS_H__