#include <iostream>
#include <cstdlib>

using namespace std;

class Node {
  public:
    int key;
    Node *left;
    Node *right;

    Node(int k) {
      key = k;
      right = NULL;
      left = NULL;
    }
};

class Tree {
  public:
    Node *root;
    
    Tree() {
      root = NULL;
    }

    void insert(int k) {
      root = insertNode(root, k);
    }

    Node *insertNode(Node *node, int k) {
      if (node == NULL) {
        node = new Node(k);
      } else {
        bool flag = (rand()%2 == 0);
        //if (k < node->key) {
        if (flag) {
          node->left = insertNode(node->left, k);
        } else {
          node->right = insertNode(node->right, k);
        }
      }

      return node;
    }

    void print() {
      cout << endl << "--- inorder ---" << endl;
      inorder(root);
      cout << endl;

      cout << endl << "--- preorder ---" << endl;
      preorder(root);
      cout << endl;

      cout << endl << "--- postorder ---" << endl;
      postorder(root);
      cout << endl;
    }

    void inorder(Node *node) {
      if (!node) {
        return;
      }

      inorder(node->left);
      cout << " " << node->key << " ";
      inorder(node->right);
    }
    
    void preorder(Node *node) {
      if (!node) {
        return;
      }

      cout << " " << node->key << " ";
      preorder(node->left);
      preorder(node->right);
    }
    
    void postorder(Node *node) {
      if (!node) {
        return;
      }

      postorder(node->left);
      postorder(node->right);
      cout << " " << node->key << " ";
    }

    void myinsert(int *in, int *pre, int& idx, int sz, int s, int e) {
      root = in_pre_insert(in, pre, idx, sz, s, e);
      return;
    }

    Node *in_pre_insert(int *in, int *pre, Node *node, int& idx, int sz, int s, int e) {
      // termination case
      if (idx >= sz) { return NULL; }

      int elem = pre[idx];
      node = new Node(elem);

      int in_idx;
      for (int i=s; i<=e; i++) {
        if (elem == in[i]) {
          in_idx = i;
          break;
        }
      }

      if (s <= in_idx - 1) {
        idx++;
        node->left = in_pre_insert(in, pre, node->left, idx, s, in_idx - 1);
      }

      if (in_idx + 1 <= e) {
        idx++;
        node->right = in_pre_insert(in, pre, node->right, idx, in_idx + 1, e);
      }

      return node;
    }
};

// this assumes that the number actually
// exists in the list ...
int bsearch(int *list, int elem, int start, int end) {
  int mid = (start+end)/2;

  while (list[mid] != elem) {
    if (list[mid] < elem) {
      start = mid + 1;
    } else {
      end = mid - 1;
    }

    mid = (start+end)/2;
  }

  return mid;
}

void fancy_insert(int *in, int *pre, int& idx, int sz, int s, int e, Tree *t) {
  // termination case here ...
  if (idx == sz) { return; }
  if (s < 0) { return; }
  if (e >= sz) { return; }
  if (s > e) { return; }

  int elem = pre[idx];
  int in_idx = bsearch(in, elem, s, e);

  t->insert(elem);
  fancy_insert(in, pre, ++idx, sz, s, in_idx-1, t);
  fancy_insert(in, pre, ++idx, sz, s, in_idx+1, t);

  return;
}

int main() {
  int ll[10] = {5, 3, 8, 7, 2, 0, 9, 1, 4, 6};
  Tree tree;

  for (int i=0; i<10; i++) {
    tree.insert(ll[i]);
  }

  tree.print();

  // Tree t2;

  // int idx = 0;
  // int start = 0;
  // int end = 9;
  // fancy_insert(in_list, pre_list, idx, start, end, t2);

  return 0;
}
