//
// Created by User on 21.11.2021.
//

#include "ISCASCompile.h"
#include <cassert>

ISCASCompile::ISCASCompile(const std::string &str) : m_Func(), labels(), setofFuncs(), erg(),
                                                     fileName(str) {


}

std::ostream &operator<<(std::ostream &os, const ISCASCompile &iscasCompile) {
    unsigned choice = 0;
    std::cout << "  Bitte geben sie 1 Für eine SVG Datei eine 2 Für eine TXT " << std::endl;
    std::cin >> choice;
    if (choice == 1) {
        FILE *p = popen("dot -Tsvg -o graph.svg", "w");
        if (p) {
            std::cout << "Graph wird erstellt" << std::endl;
            fprintf(p, "%s", iscasCompile.erg.str().c_str());
        } else
            std::cout << "Graph konnte nicht erstellt" << std::endl;
        pclose(p);
        std::cout << "graph.svg wurde erstellt" << std::endl;
    } else if (choice == 2) {
        FILE *p = fopen("graph.txt", "w");
        if (p) {
            std::cout << "Txt wird erstellt" << std::endl;
            fprintf(p, "%s", iscasCompile.erg.str().c_str());
            std::cout << "graph.txt wurde erstellt" << std::endl;
        } else
            std::cout << "txt konnte nicht erstellt" << std::endl;
        pclose(p);
    } else
        std::cout << "Keine 1 oder 2 gewählt" << std::endl;

    return os;
}


void ISCASCompile::recGraph(const Func *func) {
    auto fun = setofFuncs.find(func);
    if (fun == setofFuncs.end()) {
        setofFuncs.insert(func);
        if (func->isConstant()) {
            if (func->isTrue()) {
                erg << "\"" << func << "\"" << std::endl;
                erg << " [shape=box,label = \"1\"]" << std::endl;
            } else if (func->isFalse()) {
                erg << "\"" << func << "\"" << std::endl;
                erg << " [shape=box,label = \"0\"]" << std::endl;
            }
            return;
        }
        recGraph(func->getThen(func->getVar()));
        recGraph(func->getElse(func->getVar()));
        erg << "\"" << func << "\"" << "[label = " << labels.find(func->getVar())->second << "]" << std::endl;
        erg << *func << std::endl;
    }
}


void ISCASCompile::drawGraph(const std::vector<std::basic_string<char>> &output) {
    erg << "digraph { " << std::endl;
    erg << "subgraph cluster_0 { " << std::endl;
    erg << "style=invis " << std::endl;
    for (const auto &x: output)
        erg << x << " [shape=plaintext] " << std::endl;
    erg << "}" << std::endl;

    for (const auto &x: output) {
        auto func = m_Func.find(x);
        erg << func->first << " -> " << "\"" << &func->second << "\"" << std::endl;
        erg << "\"" << &func->second << "\"" << "[label = " << labels.find(func->second.getVar())->second << "]"
            << std::endl;
        recGraph(&func->second);

    }
    erg << "}" << std::endl;


}

void ISCASCompile::genGraph() {
    FILE *file = fopen(fileName.c_str(), "r");
    if (file == nullptr) {
        std::cout << " File wurde nicht gefunden  " << std::endl;
        exit(0);
    }
    ISCAS iscas(file);
    fclose(file);
    if (iscas.isValid()) {
        ROBDD bdd;
        auto &input = iscas.inputs();
        auto &output = iscas.outputs();
        auto &assigemnts = iscas.assignments();

        labels.insert({~0, "1"});
        labels.insert({~0 - 1, "0"});
        for (const auto &i: input) {
            auto &func = bdd.genVar(i);
            m_Func.insert({i, func});
            labels.insert({func.getVar(), i});
        }


        for (const auto &x: assigemnts) {
            auto it = std::get<2>(x).begin();
            auto func1 = &m_Func.find(*it)->second;
            ++it;
            while (it != std::get<2>(x).end()) {
                switch (std::get<1>(x)) {
                    case iscas.NAND:
                    case iscas.AND:
                        func1 = &bdd.AND(*func1, m_Func.find(*it)->second);
                        break;
                    case iscas.NOR:
                    case iscas.OR:
                        func1 = &bdd.OR(*func1, m_Func.find(*it)->second);
                        break;

                    case iscas.XOR:
                        func1 = &bdd.XOR(*func1, m_Func.find(*it)->second);
                        assert(std::get<2>(x).size() == 2);
                        break;
                    default:
                        break;
                }
                ++it;
            }
            if (std::get<1>(x) == iscas.NAND || std::get<1>(x) == iscas.NOR || std::get<1>(x) == iscas.NOT)
                func1 = &bdd.NOT(*func1);

            m_Func.insert({std::get<0>(x), *func1});
        }
        drawGraph(output);
    } else
        std::cout << "File koennte nicht gelesen werden" << std::endl;

}
