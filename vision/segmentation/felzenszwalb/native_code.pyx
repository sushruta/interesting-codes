import numpy as np
cimport numpy as np

DTYPE = np.float
ctypedef np.float_t DTYPE_t

####
# The code currently runs at ~200ms
# for a 512x512 image. There is still
# some room for improvement. It's extremely
# to make this function as fast as possible
#
####
def get_edges(np.ndarray[DTYPE_t, ndim=2] img):
  cdef int height = img.shape[0]
  cdef int width = img.shape[1]
  cdef int h, w, pixel
  cdef DTYPE_t val

  edges = []

  cdef np.ndarray img_pad = np.pad(img, (1, 1), 'constant', constant_values=0)

  cdef np.ndarray diff1 = np.abs(img_pad[1:-1, 1:-1] - img_pad[2:, 2:])
  cdef np.ndarray diff2 = np.abs(img_pad[1:-1, 1:-1] - img_pad[2:, 1:-1])
  cdef np.ndarray diff3 = np.abs(img_pad[1:-1, 1:-1] - img_pad[1:-1:, 2:])

  height = diff1.shape[0]
  width = diff1.shape[1]
  for h in xrange(height):
    for w in xrange(width):
      val = diff1[h, w]
      edges.append((val, h*width+w, (h+1)*width+(w+1)))
  
  height = diff2.shape[0]
  width = diff2.shape[1]
  for h in xrange(height):
    for w in xrange(width):
      val = diff2[h, w]
      edges.append((val, h*width+w, h*width+w+1))

  height = diff3.shape[0]
  width = diff3.shape[1]
  for h in xrange(height):
    for w in xrange(width):
      val = diff3[h, w]
      edges.append((val, h*width+w, (h+1)*width+w))
