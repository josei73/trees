//
// Created by User on 21.11.2021.
//

#ifndef ASSIG1_ISCASCOMPILE_H
#define ASSIG1_ISCASCOMPILE_H

#include "ROBDD.h"
#include "ISCAS.h"
#include <vector>
#include <set>
#include <sstream>
#include "PatriciaTree.h"

class ISCASCompile {
public:


    explicit ISCASCompile(const std::string &str);

    ISCASCompile(const ISCASCompile &) = default;

    ISCASCompile &operator=(const ISCASCompile &) = default;

    friend std::ostream &operator<<(std::ostream &, const ISCASCompile &);

    void drawGraph(const std::vector<std::basic_string<char>> &output);

    void genGraph();

private:
    RedBlackTree<std::string, const Func &> m_Func;

    void recGraph(const Func *func);

    PatriciaTree<unsigned, std::string> labels;
    std::set<const Func *> setofFuncs;
    std::ostringstream erg;
    const std::string fileName;

};

#endif //ASSIG1_ISCASCOMPILE_H
