
//
// Created by User on 10.11.2021.
//
#include <iostream>
#include "ISCAS.h"
#include "ROBDD.h"
#include "ISCASCompile.h"
#include <set>
#include "RedBlackTree.h"
#include "PatriciaTree.h"
int main() {

    std::string str;
    std::cout << "Datei namen eingeben" << std::endl;
    std::cin >> str;
   ISCASCompile iscasCompile(str);
   std::cout << "startet" << std::endl;
   iscasCompile.genGraph();
   std::cout << iscasCompile << std::endl;
    std::cout << "ende" << std::endl;

/*
    PatriciaTree<unsigned,std::string> tree;
   // RedBlackTree<unsigned ,std::string> tree;

    auto res= tree.insert({55,"test1"}).first;


    tree.insert({66, "test3"});
    tree.insert({70, "test2"});

    tree.insert({23, "test6"});
    tree.insert({25, "test7"});


 tree.insert({11, "test4"});

 tree.insert({2, "test5"});


    std::cout << tree << std::endl;

 unsigned kuk=55;




    std::cout << " ---------------------------------------- " << std::endl;

    auto it = tree.begin();
   while (it != tree.end()) {
        std::cout << it->first << " " << it->second << std::endl;
        ++it;
   }
    */
    return 0;

}








