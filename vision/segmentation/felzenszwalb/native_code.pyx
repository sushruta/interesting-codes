import cython
cimport cython

import numpy as np
cimport numpy as np

DTYPE = np.double
ctypedef np.double_t DTYPE_t

####
# The code currently runs at ~6ms
# for a 512x512 image. There is still
# some room for improvement. It's extremely
# to make this function as fast as possible
#
####
@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
def get_edges(np.ndarray[DTYPE_t, ndim=2] img):
  cdef unsigned int height = np.shape(img)[0]
  cdef unsigned int width = np.shape(img)[1]
  cdef unsigned int h, w
  cdef unsigned int cnt = 0

  h = height
  w = width

  cdef unsigned int totalrows = 2*(h-1)*(w-1) + (h-1)*width + height*(w-1)

  cdef np.ndarray[DTYPE_t, ndim=1] edge = np.zeros(3, dtype=DTYPE)
  cdef np.ndarray[DTYPE_t, ndim=2] edges = np.zeros((totalrows, 3), dtype=DTYPE)

  cdef np.ndarray[DTYPE_t, ndim=2] diff1 = np.abs(img[:-1, :-1] - img[1:, 1:], dtype=DTYPE)
  cdef np.ndarray[DTYPE_t, ndim=2] diff2 = np.abs(img[:, :-1] - img[:, 1:], dtype=DTYPE)
  cdef np.ndarray[DTYPE_t, ndim=2] diff3 = np.abs(img[:-1, :] - img[1:, :], dtype=DTYPE)
  cdef np.ndarray[DTYPE_t, ndim=2] diff4 = np.abs(img[1:, :-1] - img[:-1, 1:], dtype=DTYPE)

  cdef double[:, :] diff1_view = diff1
  cdef double[:, :] diff2_view = diff2
  cdef double[:, :] diff3_view = diff3
  cdef double[:, :] diff4_view = diff3

  cdef double[:] edge_view = edge
  cdef double[:, :] edges_view = edges

  for h in xrange(height):
    for w in xrange(width):
      if h < h-1 and w < w-1:
        edge_view[0] = diff1_view[h, w]
        edge_view[1] = h*width+w
        edge_view[2] = (h+1)*width+(w+1)
        edges_view[cnt] = edge_view
        cnt = cnt + 1
      if h < h-1:
        edge_view[0] = diff3_view[h, w]
        edge_view[1] = h*width+w
        edge_view[2] = (h+1)*width+w
        edges_view[cnt] = edge_view
        cnt = cnt + 1
      if w < w-1:
        edge_view[0] = diff2_view[h, w]
        edge_view[1] = h*width+w
        edge_view[2] = h*width+(w+1)
        edges_view[cnt] = edge_view
        cnt = cnt + 1
      if h > 0 and w < w-1:
        edge_view[0] = diff4_view[h, w]
        edge_view[1] = h*width+w
        edge_view[2] = (h-1)*width+w
        edges_view[cnt] = edge_view
        cnt = cnt + 1

  return edges

## @cython.boundscheck(False)
## @cython.wraparound(False)
## @cython.nonecheck(False)
cdef class UnionFindSet:
  cdef int num
  cdef object nodes_raw
  cdef object ranks_raw
  cdef object size_raw
  cdef object max_edge_raw

  def __init__(self, num):
    self.num = num
    self.nodes_raw = np.arange(num, dtype=np.int)
    self.ranks_raw = np.ones(num, dtype=np.int)
    self.size_raw = np.ones(num, dtype=np.int)
    self.max_edge_raw = np.zeros(num, dtype=np.double)

  def findSet(self, int elem):
    return self.findSet_c(elem)

  cdef int findSet_c(self, int elem):
    ## Getting this kind of a view is very important
    ## for speeds. Otherwise, python land again LOL.
    cdef np.ndarray[np.int_t, ndim=1] nodes = self.nodes_raw
    if nodes[elem] is not elem:
      nodes[elem] = self.findSet(nodes[elem])
    return nodes[elem]

  def unionSet(self, int elem1, int elem2):
    self.unionSet_c(elem1, elem2)
    return

  cdef unionSet_c(self, int elem1, int elem2):
    cdef np.ndarray[np.int_t, ndim=1] nodes = self.nodes_raw
    cdef np.ndarray[np.int_t, ndim=1] ranks = self.ranks_raw
    cdef np.ndarray[np.int_t, ndim=1] size = self.size_raw
    if nodes[elem1] == nodes[elem2]:
      return

    cdef int elem1Root = self.findSet(elem1)
    cdef int elem2Root = self.findSet(elem2)

    if ranks[elem1Root] > ranks[elem2Root]:
      nodes[elem2Root] = elem1Root
      size[elem1Root] += size[elem2Root]
    elif ranks[elem2Root] > ranks[elem1Root]:
      nodes[elem1Root] = elem2Root
      size[elem2Root] += size[elem1Root]
    else:
      nodes[elem1Root] = elem2Root
      ranks[elem2Root] = ranks[elem2Root] + 1
      size[elem2Root] += size[elem1Root]

  def getMaxEdge(self, int node):
    return self.getMaxEdge_c(node)

  cdef double getMaxEdge_c(self, int node):
    cdef np.ndarray[np.double_t, ndim=1] max_edge = self.max_edge_raw
    return max_edge[node]

  def setMaxEdge(self, int node, double weight):
    self.setMaxEdge_c(node, weight)
    return

  cdef setMaxEdge_c(self, int node, double weight):
    cdef np.ndarray[np.double_t, ndim=1] max_edge = self.max_edge_raw
    max_edge[node] = weight
    return

  def getSize(self, int node):
    return self.getSize_c(node)

  cdef int getSize_c(self, int node):
    cdef np.ndarray[np.int_t, ndim=1] size = self.size_raw
    return size[node]

@cython.cdivision(True)
cdef class Segmentation:
  cdef object edges_raw
  cdef object ufs
  cdef int k

  def __init__(self, np.ndarray[np.double_t, ndim=2] edges):
    self.edges_raw = edges
    self.ufs = UnionFindSet(edges.shape[0])
    self.k = 300 ## need to do cross-validation

  cdef int tau_func(self, int v):
    cdef int root = self.ufs.findSet(v)
    return self.k / self.ufs.getSize(root)

  cdef double int_func(self, int v):
    cdef int root = self.ufs.findSet(v)
    return self.ufs.getMaxEdge(root)

  cdef double mint_func(self, int v_i, int v_j):
    cdef double int_c_i = self.int_func(v_i)
    cdef int tau_c_i = self.tau_func(v_i)

    cdef double int_c_j = self.int_func(v_j)
    cdef int tau_c_j = self.tau_func(v_j)

    return min(int_c_i + tau_c_i, int_c_j + tau_c_j)

  cdef iterate(self, int iter_val):
    cdef np.ndarray[np.double_t, ndim=2] edges = self.edges_raw
    cdef double w_o_q = edges[iter_val, 0]
    cdef int v_i = <int>edges[iter_val, 1]
    cdef int v_j = <int>edges[iter_val, 2]

    cdef double mint_i_j = self.mint_func(v_i, v_j)
    cdef int root_v_i, root_v_j, root
    cdef double max_edge

    if w_o_q <= mint_i_j:
      root_v_i = self.ufs.findSet(v_i)
      root_v_j = self.ufs.findSet(v_j)

      self.ufs.unionSet(v_i, v_j)
      root = self.ufs.findSet(v_i)

      max_edge = max(w_o_q, max(self.ufs.getMaxEdge(v_i), self.ufs.getMaxEdge(v_j)))
      self.ufs.setMaxEdge(root, max_edge)
    return

  def segment(self):
    self.segment_c()

  cdef segment_c(self):
    cdef np.ndarray[np.double_t, ndim=2] edges = self.edges_raw
    for i in xrange(edges.shape[0]):
      self.iterate(i)
    return
