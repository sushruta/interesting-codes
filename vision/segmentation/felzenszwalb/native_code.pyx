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
