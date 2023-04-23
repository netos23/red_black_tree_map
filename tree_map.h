//
// Created by Nikita Morozov on 01.04.2023.
//

#ifndef RED_BLACK_TREE_TREE_MAP_H
#define RED_BLACK_TREE_TREE_MAP_H

#include <algorithm>
#include <queue>

namespace collections {

    template<typename K, typename V>
    class MapEntry {
    protected:
        K key;
        V value;
    public:
        MapEntry(const K &key, const V &value) : key(std::move(key)), value(std::move(value)) {}

        const K &getKey() const {
            return key;
        }

        const V &getValue() const {
            return value;
        }

    };

    template<typename K, typename V>
    class Map {
    public:
        virtual bool containsKey(const K &key) = 0;

        virtual const V &put(const K &key, const V &val) = 0;

        virtual const V &remove(const K &key) = 0;

        virtual int size() = 0;

        virtual bool isEmpty() = 0;

        virtual void clear() = 0;

        virtual const V &operator[](const K &key) = 0;

        virtual ~Map() = default;
    };

    enum Color {
        red,
        black
    };


    template<typename K, typename V>
    class TreeMapEntry : public MapEntry<K, V> {
    private:
        TreeMapEntry *parent;
        TreeMapEntry *left;
        TreeMapEntry *right;
        Color color;
    public:
        TreeMapEntry(const K &key, const V &value, Color color) :
                MapEntry<K, V>(key, value),
                color(color),
                parent(nullptr),
                left(nullptr),
                right(nullptr) {}

        TreeMapEntry(const K &key, const V &value) :
                MapEntry<K, V>(key, value),
                color(black),
                parent(nullptr),
                left(nullptr),
                right(nullptr) {}

        TreeMapEntry(const K &key, const V &value, TreeMapEntry *parent) :
                MapEntry<K, V>(key, value),
                color(black),
                parent(parent),
                left(nullptr),
                right(nullptr) {}


        TreeMapEntry<K, V> *getParent() const {
            return parent;
        }

        void setParent(TreeMapEntry<K, V> *parent) {
            TreeMapEntry::parent = parent;
        }

        TreeMapEntry<K, V> *getLeft() const {
            return left;
        }

        void setLeft(TreeMapEntry<K, V> *left) {
            TreeMapEntry::left = left;
        }

        TreeMapEntry<K, V> *getRight() const {
            return right;
        }

        void setRight(TreeMapEntry<K, V> *right) {
            TreeMapEntry::right = right;
        }

        Color getColor() const {
            return color;
        }

        void setColor(Color color) {
            TreeMapEntry::color = color;
        }

        void setValue(const V &val) {
            MapEntry<K, V>::value = val;
        }

        void setKey(const K &key) {
            MapEntry<K, V>::key = key;
        }


    };


    template<typename K, typename V>
    class TreeMap : public Map<K, V> {
    private:
        int (*comparator)(const K &a, const K &b);

        int length;
        TreeMapEntry<K, V> *root;

        int compare(const K &a, const K &b) const;

        TreeMapEntry<K, V> *grandParentOf(const TreeMapEntry<K, V> *entry) const {
            auto *parent = parentOf(entry);
            return parent == nullptr ? nullptr : parent->getParent();
        }

        TreeMapEntry<K, V> *parentOf(const TreeMapEntry<K, V> *entry) const {

            return entry == nullptr ? nullptr : entry->getParent();
        }

        TreeMapEntry<K, V> *leftOf(const TreeMapEntry<K, V> *entry) const {

            return entry == nullptr ? nullptr : entry->getLeft();
        }

        TreeMapEntry<K, V> *rightOf(const TreeMapEntry<K, V> *entry) const {

            return entry == nullptr ? nullptr : entry->getRight();
        }

        Color colorOf(const TreeMapEntry<K, V> *entry) const {

            return entry == nullptr ? black : entry->getColor();
        }

        void rotateLeft(TreeMapEntry<K, V> *entry);

        void rotateRight(TreeMapEntry<K, V> *entry);


        void setColor(TreeMapEntry<K, V> *entry, Color color) {
            if (entry != nullptr) {
                entry->setColor(color);
            }
        }

        void balanceAfterInsert(TreeMapEntry<K, V> *entry);

        void balanceAfterRemove(TreeMapEntry<K, V> *entry);


        TreeMapEntry<K, V> *getEntry(const K &key) const;

        TreeMapEntry<K, V> *getSuccessor(TreeMapEntry<K, V> *entry) const;


    public:
        explicit TreeMap(int (*comparator)(const K &, const K &)) : comparator(comparator), length(0),
                                                                    root(nullptr) {}

        TreeMap() : comparator(nullptr), length(0),
                    root(nullptr) {}

        bool containsKey(const K &key) override {
            auto *entry = getEntry(key);
            return entry != nullptr;
        }

        bool isEmpty() override {
            return size() == 0;
        }

        void clear() override;

        const V &put(const K &key, const V &val) override;

        const V &remove(const K &key) override;

        int size() override {
            return length;
        }

        const V &operator[](const K &key) override;

        ~TreeMap<K, V>() override {
            clear();
        };
    };

    template<typename K, typename V>
    const V &TreeMap<K, V>::operator[](const K &key) {
        auto *entry = getEntry(key);
        return entry == nullptr ? root->getValue() : entry->getValue();
    }

    template<typename K, typename V>
    const V &TreeMap<K, V>::remove(const K &key) {
        auto *entry = getEntry(key);
        if (entry == nullptr) {
            return root->getValue();
        }

        auto val = entry->getValue();
        length--;

        // If strictly internal, copy successor's element to p and then make p
        // point to successor.
        if (entry->getLeft() != nullptr && entry->getRight() != nullptr) {
            auto *s = getSuccessor(entry);
            entry->setKey(s->getKey());
            entry->setValue(s->getValue());
            entry = s;
        } // p has 2 children

        auto *replacement = (entry->getLeft() != nullptr ? entry->getLeft() : entry->getRight());

        if (replacement != nullptr) {
            // Link replacement to parent
            replacement->setParent(entry->getParent());
            if (entry->getParent() == nullptr) {
                root = replacement;
            } else if (entry == entry->getParent()->getLeft()) {
                entry->getParent()->setLeft(replacement);
            } else {
                entry->getParent()->setRight(replacement);
            }

            // Null out links so they are OK to use by fixAfterDeletion.
            auto color = entry->getColor();
            delete entry;

            // Fix replacement
            if (color == black) {
                balanceAfterRemove(replacement);
            }
        } else if (entry->getParent() == nullptr) { // return if we are the only node.
            delete root;
            root = nullptr;
        } else { //  No children. Use self as phantom replacement and unlink.
            if (entry->getColor() == black) {
                balanceAfterRemove(entry);
            }

            if (entry->getParent() != nullptr) {
                if (entry == entry->getParent()->getLeft()) {
                    entry->getParent()->setLeft(nullptr);
                } else if (entry == entry->getParent()->getRight()) {
                    entry->getParent()->setRight(nullptr);
                }
                delete entry;
            }
        }
        return std::move(val);
    }

    template<typename K, typename V>
    const V &TreeMap<K, V>::put(const K &key, const V &val) {
        if (!root) {
            root = new TreeMapEntry<K, V>(key, val);
            length = 1;
            return root->getValue();
        }

        auto *cur = root;
        TreeMapEntry<K, V> *parent;
        int cmp;
        do {
            parent = cur;
            cmp = this->compare(key, parent->getKey());
            if (cmp < 0) {
                cur = parent->getLeft();
            } else if (cmp > 0) {
                cur = parent->getRight();
            } else {
                V old = parent->getValue();
                parent->setValue(val);
                return std::move(old);
            }
        } while (cur != nullptr);


        auto *entry = new TreeMapEntry<K, V>(key, val, parent);
        if (cmp < 0) {
            parent->setLeft(entry);
        } else {
            parent->setRight(entry);
        }
        length++;

        balanceAfterInsert(entry);

        return entry->getValue();
    }

    template<typename K, typename V>
    void TreeMap<K, V>::clear() {
        std::queue<TreeMapEntry<K, V> *> deletionQueue;
        deletionQueue.push(root);
        while (!deletionQueue.empty()) {
            auto *cur = deletionQueue.front();
            deletionQueue.pop();

            if(cur == nullptr){
                continue;
            }

            auto *left = cur->getLeft();
            if (left != nullptr) {
                deletionQueue.push(left);
            }

            auto *right = cur->getRight();
            if (right != nullptr) {
                deletionQueue.push(right);
            }

            delete cur;
        }
        length = 0;
        root = nullptr;
    }

    template<typename K, typename V>
    TreeMapEntry<K, V> *TreeMap<K, V>::getSuccessor(TreeMapEntry<K, V> *entry) const {
        if (entry == nullptr) {
            return nullptr;
        }

        if (entry->getRight() != nullptr) {
            auto *p = entry->getRight();
            while (p->getLeft() != nullptr) {
                p = p->getLeft();
            }
            return p;
        } else {
            auto *p = entry->getParent();
            auto *ch = entry;
            while (p != nullptr && ch == p->getRight()) {
                ch = p;
                p = p->getParent();
            }
            return p;
        }
    }

    template<typename K, typename V>
    TreeMapEntry<K, V> *TreeMap<K, V>::getEntry(const K &key) const {
        auto *p = root;

        while (p != nullptr) {
            int cmp = this->compare(key, p->getKey());
            if (cmp < 0) {
                p = p->getLeft();
            } else if (cmp > 0) {
                p = p->getRight();
            } else {
                return p;
            }
        }

        return nullptr;
    }

    template<typename K, typename V>
    int TreeMap<K, V>::compare(const K &a, const K &b) const {
        if (comparator) {
            return comparator(a, b);
        }

        if (a == b) {
            return 0;
        }

        return a > b ? 1 : -1;
    }

    template<typename K, typename V>
    void TreeMap<K, V>::rotateLeft(TreeMapEntry<K, V> *entry) {
        if (entry == nullptr) {
            return;
        }

        auto *r = entry->getRight();
        entry->setRight(r->getLeft());
        if (r->getLeft() != nullptr) {
            r->getLeft()->setParent(entry);
        }
        r->setParent(entry->getParent());

        if (entry->getParent() == nullptr) {
            root = r;
        } else if (entry->getParent()->getLeft() == entry) {
            entry->getParent()->setLeft(r);
        } else {
            entry->getParent()->setRight(r);
        }

        r->setLeft(entry);
        entry->setParent(r);
    }

    template<typename K, typename V>
    void TreeMap<K, V>::rotateRight(TreeMapEntry<K, V> *entry) {
        if (entry == nullptr) {
            return;
        }

        auto *l = entry->getLeft();
        entry->setLeft(l->getRight());

        if (l->getRight() != nullptr) {
            l->getRight()->setParent(entry);
        }
        l->setParent(entry->getParent());

        if (entry->getParent() == nullptr) {
            root = l;
        } else if (entry->getParent()->getRight() == entry) {
            entry->getParent()->setRight(l);
        } else {
            entry->getParent()->setLeft(l);
        }
        l->setRight(entry);
        entry->setParent(l);
    }

    template<typename K, typename V>
    void TreeMap<K, V>::balanceAfterInsert(TreeMapEntry<K, V> *entry) {
        auto *first = entry;
        first->setColor(red);

        while (first && first != root && first->getParent()->getColor() == red) {
            if (first->getParent() == leftOf(grandParentOf(first))) {
                auto *second = rightOf(grandParentOf(first));
                if (colorOf(second) == red) {
                    setColor(parentOf(first), black);
                    setColor(second, black);
                    setColor(grandParentOf(first), red);
                    first = grandParentOf(first);
                } else {
                    if (first == rightOf(parentOf(first))) {
                        first = parentOf(first);
                        rotateLeft(first);
                    }
                    setColor(parentOf(first), black);
                    setColor(parentOf(parentOf(first)), red);
                    rotateRight(parentOf(parentOf(first)));
                }
            } else {
                auto *second = leftOf(parentOf(parentOf(first)));
                if (colorOf(second) == red) {
                    setColor(parentOf(first), black);
                    setColor(second, black);
                    setColor(parentOf(parentOf(first)), red);
                    first = parentOf(parentOf(first));
                } else {
                    if (first == leftOf(parentOf(first))) {
                        first = parentOf(first);
                        rotateRight(first);
                    }
                    setColor(parentOf(first), black);
                    setColor(parentOf(parentOf(first)), red);
                    rotateLeft(parentOf(parentOf(first)));
                }
            }
        }
    }

    template<typename K, typename V>
    void TreeMap<K, V>::balanceAfterRemove(TreeMapEntry<K, V> *entry) {
        while (entry != root && colorOf(entry) == black) {
            if (entry == leftOf(parentOf(entry))) {
                auto *sib = rightOf(parentOf(entry));

                if (colorOf(sib) == red) {
                    setColor(sib, black);
                    setColor(parentOf(entry), red);
                    rotateLeft(parentOf(entry));
                    sib = rightOf(parentOf(entry));
                }

                if (colorOf(leftOf(sib)) == black &&
                    colorOf(rightOf(sib)) == black) {
                    setColor(sib, red);
                    entry = parentOf(entry);
                } else {
                    if (colorOf(rightOf(sib)) == black) {
                        setColor(leftOf(sib), black);
                        setColor(sib, red);
                        rotateRight(sib);
                        sib = rightOf(parentOf(entry));
                    }
                    setColor(sib, colorOf(parentOf(entry)));
                    setColor(parentOf(entry), black);
                    setColor(rightOf(sib), black);
                    rotateLeft(parentOf(entry));
                    entry = root;
                }
            } else { // symmetric
                auto *sib = leftOf(parentOf(entry));

                if (colorOf(sib) == red) {
                    setColor(sib, black);
                    setColor(parentOf(entry), red);
                    rotateRight(parentOf(entry));
                    sib = leftOf(parentOf(entry));
                }

                if (colorOf(rightOf(sib)) == black &&
                    colorOf(leftOf(sib)) == black) {
                    setColor(sib, red);
                    entry = parentOf(entry);
                } else {
                    if (colorOf(leftOf(sib)) == black) {
                        setColor(rightOf(sib), black);
                        setColor(sib, red);
                        rotateLeft(sib);
                        sib = leftOf(parentOf(entry));
                    }
                    setColor(sib, colorOf(parentOf(entry)));
                    setColor(parentOf(entry), black);
                    setColor(leftOf(sib), black);
                    rotateRight(parentOf(entry));
                    entry = root;
                }
            }
        }

        setColor(entry, black);
    }


} // collections

#endif //RED_BLACK_TREE_TREE_MAP_H
