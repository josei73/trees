//
// Created by User on 06.12.2021.
//

#ifndef ASSIG2_PATRICIATREE_H
#define ASSIG2_PATRICIATREE_H

#include <limits>
#include <list>
#include <iostream>


template<class K, class D>
class PatriciaTree {
public:
    typedef std::pair<const K, D> value_type;

    static bool isLeft(K key, unsigned bitPos) {
        return (key & (1 << bitPos)) == 0;
    }

    struct Node {
        Node(value_type value, unsigned bitPos, Node *succ) : m_Val(value), m_BitPos(bitPos) {
            bool bIsLeft = isLeft(value.first, bitPos);
            left = bIsLeft ? this : succ;
            right = bIsLeft ? succ : this;
        }

        Node(value_type value, unsigned bitPos) {
            Node(value, bitPos, nullptr);
        }

        Node &operator=(const Node &node) = delete;

        Node(const Node &node) = delete;

        friend std::ostream &operator<<(std::ostream &os, const Node &node) {
            os << node.m_Val.first << "  " << node.m_Val.second << std::endl;
            if (node.left != nullptr && node.left->m_BitPos > node.m_BitPos)
                os << *node.left;
            if (node.right != nullptr && node.right->m_BitPos > node.m_BitPos)
                os << *node.right;
            return os;
        }


        value_type m_Val;
        Node *left = nullptr;
        Node *right = nullptr;
        unsigned m_BitPos = 0;

    };

    class iterator : public std::iterator<std::forward_iterator_tag, value_type> {
    public:
        iterator(Node *n_Start, bool isRoot) : m_Nodes2Visit() {
            if (isRoot)
                goDown(n_Start);
            else if (n_Start != nullptr)
                m_Nodes2Visit.push_back(n_Start);
        }

        iterator &operator++() {
            Node *pLast = m_Nodes2Visit.back();
            m_Nodes2Visit.pop_back();
            if (pLast->right && pLast->m_BitPos < pLast->right->m_BitPos)
                goDown(pLast->right);
            return *this;
        }

        iterator operator++(int) {
            iterator res(*this);
            Node *pLast = m_Nodes2Visit.back();
            m_Nodes2Visit.pop_back();
            if (pLast->right && pLast->m_BitPos < pLast->right->m_BitPos)
                goDown(pLast->right);
            return res;
        }

        friend bool operator==(const iterator &itArg1, const iterator &itArg2) {
            return itArg1.m_Nodes2Visit == itArg2.m_Nodes2Visit;
        }

        friend bool operator!=(const iterator &itArg1, const iterator &itArg2) {
            return itArg1.m_Nodes2Visit != itArg2.m_Nodes2Visit;
        }

        typename iterator::value_type &operator*() {
            return m_Nodes2Visit.back()->m_Val;
        }

        typename iterator::value_type *operator->() {
            return &m_Nodes2Visit.back()->m_Val;
        }

    private:
        std::list<Node *> m_Nodes2Visit;

        void goDown(Node *pTmp) {
            int lastbit = -1;
            for (; pTmp && lastbit < (int) pTmp->m_BitPos; pTmp = pTmp->left) {
                m_Nodes2Visit.push_back(pTmp);
                lastbit = pTmp->m_BitPos;
            }
        }

    };

    class NodeHandler {
    public:
        const int NODE = 0;
        const int DAD = 1;

        NodeHandler(const NodeHandler &other) = delete;

        NodeHandler &operator=(const NodeHandler &other) = delete;

        explicit NodeHandler(Node *&n) : paTree(n) {
            for (unsigned ui = 1; ui > 0; --ui) {
                m_Nodes[ui] = nullptr;
            }
            m_Nodes[NODE] = n;
        }


        Node *node(unsigned kind) {
            return m_Nodes[kind];
        }

        void down(bool left) {
            for (unsigned ui = 1; ui > 0; --ui)
                m_Nodes[ui] = m_Nodes[ui - 1];
            m_Nodes[NODE] = left ? node(DAD)->left : node(DAD)->right;
        }

        bool isNull() {
            return m_Nodes[NODE] == nullptr;
        }

        void set(Node *n, unsigned kind) {
            if (node(kind + 1) == nullptr)
                paTree = n;
            else if (node(kind) != nullptr ?
                     node(kind + 1)->left == node(kind) :
                     isLeft(n->m_Val.first, node(kind + 1)->m_BitPos))
                node(kind + 1)->left = n;
            else
                node(kind + 1)->right = n;
            m_Nodes[kind] = n;
        }

        void search(const K &key) {
            search(key, ~0);
        }

        void search(const K &key, unsigned maxPos) {
            int lastBitPos = -1;
            while (!isNull() && lastBitPos < (int) node(NODE)->m_BitPos && maxPos > node(NODE)->m_BitPos) {
                lastBitPos = node(NODE)->m_BitPos;
                down(isLeft(key, lastBitPos));
            }
        }

    private :
        Node *&paTree = nullptr;
        Node *m_Nodes[2];
    };

private:
    Node *root = nullptr;


public:


    PatriciaTree() {}

    void destroyTree(Node *pNode) {

        if (pNode) {

            if (pNode->left && pNode->left->m_BitPos > pNode->m_BitPos) {
                destroyTree(pNode->left);
            }
            if (pNode->right && pNode->right->m_BitPos > pNode->m_BitPos) {
                destroyTree(pNode->right);
            }
        }
        delete pNode;
    }

    ~PatriciaTree() {
        destroyTree(root);
    }


    PatriciaTree(const PatriciaTree &patriciaTree) = delete;

    PatriciaTree &operator=(const PatriciaTree &patriciaTree) = delete;

    bool isEmpty() const {
        return root == nullptr;
    }


    std::pair<iterator, bool> insert(const value_type &value) {
        NodeHandler h(root);
        h.search(value.first);
        unsigned index = 0;
        if (h.isNull()) {
            if (h.node(h.DAD) != nullptr) {
                while (isLeft(value.first, index) == isLeft(h.node(h.DAD)->m_Val.first, index) &&
                       index < h.node(h.DAD)->m_BitPos)
                    ++index;
                if (index == h.node(h.DAD)->m_BitPos)
                    ++index;
            }
        } else if (h.node(h.NODE)->m_Val.first != value.first) {
            while (isLeft(value.first, index) == isLeft(h.node(h.NODE)->m_Val.first, index))
                ++index;
        } else
            return std::pair<iterator, bool>({h.node(h.NODE), false}, false);
        NodeHandler nh(root);
        nh.search(value.first, index);
        Node *node = new Node(value, index, nh.node(nh.NODE));
        nh.set(node, nh.NODE);
        return std::pair<iterator, bool>({node, false}, true);
    }

    friend std::ostream &operator<<(std::ostream &os, const PatriciaTree &patriciaTree) {
        if (patriciaTree.root)
            os << *patriciaTree.root;
        return os;
    }

    iterator begin() {
        return iterator(root, true);
    }

    iterator end() {
        return iterator(nullptr, true);
    }

    iterator find(const K &key) {
        NodeHandler h(root);
        h.search(key);
        if (!h.isNull() && h.node(h.NODE)->m_Val.first == key)
            return iterator(h.node(h.NODE), false);
        return end();
    }

};


#endif //ASSIG2_PATRICIATREE_H
