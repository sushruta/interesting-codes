/*
 * Why this program?
 * a) It's an application of Union-Find datastructure
 * b) When deciding on the bottom-up segment proposals,
 * one has to choose the segment with least weight in
 * its minimum spanning tree and this algorithm will
 * be useful there.
 *
 * Look at `Efficient Graph-Based Image Segmentation` by
 * Pedro F. Felzenszwalb and Daniel P. Huttenlocher for
 * details on that method.
 *
 * This program is written to run http://www.spoj.com/problems/MST/
 */
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

#define gc getchar_unlocked

void scanint(int &x) {
  register int c = gc();
  x = 0;
  for (; (c<48 || c>57); c=gc());
  for (; (c>47 && c<58); c=gc()) {
    x = (x << 1) + (x << 3) + c - 48;
  }
}

class NodeTuple {
  public:
    int e_id;
    int v1_id;
    int v2_id;
    int edge;

    bool operator < (const NodeTuple& nt) const {
      return (edge < nt.edge);
    }
};

class UnionFindSet {
  public:
    UnionFindSet(int n) {
      nodes = (int *) malloc(sizeof(int) * (n+1));
      ranks = (int *) malloc(sizeof(int) * (n+1));
      for (int i=0; i<n+1; i++) {
        nodes[i] = i;
        ranks[i] = 1;
      }
    }

    int find_set(int elem) {
      if (nodes[elem] != elem) {
        nodes[elem] = find_set(nodes[elem]);
      }
      return nodes[elem];
    }

    void union_set(int elem1, int elem2) {
      if (nodes[elem1] == nodes[elem2]) {
        return;
      }

      int elem1Root = find_set(elem1);
      int elem2Root = find_set(elem2);

      if (ranks[elem1Root] > ranks[elem2Root]) {
        nodes[elem2Root] = elem1Root;
      } else if (ranks[elem1Root] < ranks[elem2Root]) {
        nodes[elem1Root] = elem2Root;
      } else {
        nodes[elem1Root] = elem2Root;
        ranks[elem2Root]++;
      }
    }
  private:
    int *nodes;
    int *ranks;
};

int main() {
  int m, n, k;
  scanint(n);
  scanint(m);
  int cnt = m;
  int *v1_arr = (int *) malloc(sizeof(int) * m);
  int *v2_arr = (int *) malloc(sizeof(int) * m);
  int *e_arr = (int *) malloc(sizeof(int) * m);

  while (cnt--) {
    scanint(v1_arr[cnt]);
    scanint(v2_arr[cnt]);
    scanint(e_arr[cnt]);
  }

  NodeTuple *nodes = (NodeTuple *) malloc(sizeof(NodeTuple) * m);
  for (int i=0; i<m; i++) {
    nodes[i].e_id = i + 1;
    nodes[i].edge = e_arr[i];
    nodes[i].v1_id = v1_arr[i];
    nodes[i].v2_id = v2_arr[i];
  }

  std::sort(nodes, nodes+m);

  UnionFindSet ufs(n);
  int numEdges = 0;
  long long int totalWeight = 0;
  for (int i=0; i<m; i++) {
    if (ufs.find_set(nodes[i].v1_id) != ufs.find_set(nodes[i].v2_id)) {
      ufs.union_set(nodes[i].v1_id, nodes[i].v2_id);
      numEdges++;
      totalWeight += nodes[i].edge;
    }

    if (numEdges == n-1) {
      break;
    }
  }

  cout << totalWeight << endl;
  return 0;
}
