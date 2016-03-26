/*
 * Useful Datastructure for bottom-up
 * merging of segment proposals
 *
 * This is just a simple placeholder code
 * written to understand how it works. Most
 * probably, I'll use the boost implementation.
 */
#include <iostream>
#include <cstdlib>

template<typename T>
class Node {
  public:
    Node() {
      rank = 1;
    }
    void singleton(T val) {
      value = val;
      parent = this;
    }

  private:
    T value;
    int rank;
    Node<T>* parent;
};


template<typename T>
class UnionFindSet {
  public:
    UnionFindSet(int size) {
      nodes = new Node<T>[size];
      numSets = 0;
      index = 0;
    }
    ~UnionFindSet() {
      delete[] nodes;
    }

    void singleton(T value) {
      nodes[index++].singleton(value);
    }

    Node<T>* find_parent(Node<T>* element) {
      if (element.parent != element) {
        element.parent = find_parent(element.parent);
      }

      return element.parent;
    }

    Node<T>& find_set(Node<T>& element) {
      Node<T> *root = find_parent(&element);
      return *root;
    }


    void union_set(Node<T>& elem1, Node<T>& elem2) {
      Node<T>& elem1Root = find_set(elem1);
      Node<T>& elem2Root = find_set(elem2);

      if (elem1Root == elem2Root) {
        return;
      }

      if (elem1Root.rank > elem2Root.rank) {
        elem2Root.parent = &elem1Root;
      } else if (elem1Root.rank < elem2Root.rank) {
        elem1Root.parent = &elem2Root;
      } else {
        elem1Root.parent = &elem2Root;
        elem1Root.rank++;
      }

      numSets--;
      return;
    }

  private:
    Node<T> *nodes;
    int *ranks;
    int numSets;
    int index;

    bool isRoot(Node<T>& elem) {
      if (elem.parent = &elem) {
        return true;
      }
      return false;
    }
};
