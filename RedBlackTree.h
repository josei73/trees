//
// Created by User on 30.11.2021.
//

#ifndef JESSY_OSEI_36640_ASSIG1_MAP_H
#define JESSY_OSEI_36640_ASSIG1_MAP_H

#include <list>
#include <iostream>

template<class K, class D>
class RedBlackTree {
public:
    typedef std::pair<const K, D> value_type;

    struct Node {
        Node(const value_type &value) : m_Val(value) {
        }

        Node &operator=(const Node &node) = delete;

        Node(const Node &node) = delete;

        friend std::ostream &operator<<(std::ostream &os, const Node &node) {
            os << node.m_Val.first << "  " << node.m_Val.second << std::endl;

            if (node.left)
                os << *node.left;
            if (node.right)
                os << *node.right;
            return os;
        }


        bool is4Node() {
            return left != nullptr && left->m_bIsRed && right != nullptr && right->m_bIsRed;
        }

        void convert4Node() {
            left->m_bIsRed = false;
            right->m_bIsRed = false;
            m_bIsRed = true;
        }

        value_type m_Val;
        Node *left = nullptr;
        Node *right = nullptr;
        bool m_bIsRed = true;
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
            goDown(pLast->right);
            return *this;
        }

        iterator operator++(int) {
            iterator res(*this);
            Node *pLast = m_Nodes2Visit.back();
            m_Nodes2Visit.pop_back();
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
            for (; pTmp; pTmp = pTmp->left)
                m_Nodes2Visit.push_back(pTmp);
        }
    };

    class NodeHandler {
    public:
        const int NODE = 0;
        const int DAD = 1;
        const int G_DAD = 2;
        const int GG_DAD = 3;

        NodeHandler(const NodeHandler &other) = delete;

        NodeHandler &operator=(const NodeHandler &other) = delete;

        explicit NodeHandler(Node *&n) : rbTree(n) {
            for (unsigned ui = 3; ui > 0; --ui) {
                m_Nodes[ui] = nullptr;
            }
            m_Nodes[NODE] = n;
        }

        ~NodeHandler() {
            delete[] m_Nodes;

        }

        Node *node(unsigned kind) {
            return m_Nodes[kind];
        }

        void down(bool left) {
            for (unsigned ui = 3; ui > 0; --ui)
                m_Nodes[ui] = m_Nodes[ui - 1];
            m_Nodes[NODE] = left ? node(NODE)->left : node(NODE)->right;
        }

        bool isNull() {
            return m_Nodes[NODE] == nullptr;
        }

        void set(Node *n, unsigned kind) {
            if (node(kind + 1) == nullptr)
                rbTree = n;
            else if (node(kind) != nullptr ? node(kind + 1)->left == node(kind) :
                     n->m_Val.first < node(kind + 1)->m_Val.first)
                node(kind + 1)->left = n;
            else
                node(kind + 1)->right = n;
            m_Nodes[kind] = n;
        }

        void rotate(unsigned kind) {
            Node *dad = node(kind);
            Node *son = node(kind - 1);
            bool sonColour = son->m_bIsRed;
            son->m_bIsRed = dad->m_bIsRed;
            dad->m_bIsRed = sonColour;
            if (dad->left == son) {
                dad->left = son->right;
                son->right = dad;
            } else {
                dad->right = son->left;
                son->left = dad;
            }
            set(son, kind);
        }

        void split() {
            Node *dad = node(DAD);
            if (dad != nullptr && dad->m_bIsRed) {
                if ((node(G_DAD)->m_Val.first < dad->m_Val.first) != (dad->m_Val.first < node(NODE)->m_Val.first)) {
                    rotate(DAD);
                }
                rotate(G_DAD);
            }
        }


    private :
        Node *&rbTree = nullptr;
        Node **m_Nodes = new Node *[4];
    };

private:
    Node *root = nullptr;

    Node *search(K key, Node *rpt) const {
        Node *tmp = rpt;
        if (tmp != nullptr) {
            if (key < tmp->m_Val.first) {
                return search(key, tmp->left);
            } else if (tmp->m_Val.first < key) {
                return search(key, tmp->right);
            } else {
                return tmp;
            }
        }
        return nullptr;
    }


public:
    void copyTree(Node *src) {
        if (src != nullptr) {
            insert(src->m_Val.first, src->m_Val.second);
            copyTree(src->left);
            copyTree(src->right);
        }
    }

    RedBlackTree() {}

    void destroyTree(Node *pNode) {
        if (pNode != nullptr) {
            destroyTree(pNode->left);
            destroyTree(pNode->right);
            delete pNode;
        }
    }

    ~RedBlackTree() {
        destroyTree(root);
    }

    RedBlackTree &operator=(const RedBlackTree &tree) = delete;

    RedBlackTree(const RedBlackTree &redBlackTree) = delete;

    bool isEmpty() const {
        return root == nullptr;
    }

    std::pair<iterator, bool> insert(const value_type &value) {
        NodeHandler h(root);
        while (!h.isNull()) {
            if (h.node(h.NODE)->is4Node()) {
                h.node(h.NODE)->convert4Node();
                h.split();
            }
            if (value.first < h.node(h.NODE)->m_Val.first)
                h.down(true);
            else if (h.node(h.NODE)->m_Val.first < value.first)
                h.down(false);
            else
                return std::pair<iterator, bool>({h.node(h.NODE), false}, false);
        }
        Node *node = new Node(value);
        h.set(node, h.NODE);
        h.split();
        root->m_bIsRed = false;
        return std::pair<iterator, bool>({node, false}, true);
    }

    friend std::ostream &operator<<(std::ostream &os, const RedBlackTree &redBlackTree) {
        if (redBlackTree.root)
            os << *redBlackTree.root;
        return os;
    }

    iterator begin() {
        return iterator(root, true);
    }

    iterator end() {
        return iterator(nullptr, true);
    }

    iterator find(const K &key) {
        return iterator(search(key, root), false);
    }

};


#endif //JESSY_OSEI_36640_ASSIG1_MAP_H
