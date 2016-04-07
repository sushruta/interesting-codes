import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

import native_code

def rgb2gray(rgb):
  r, g, b = rgb[:,:,0], rgb[:,:,1], rgb[:,:,2]
  gray = 0.2989 * r + 0.5870 * g + 0.1140 * b
  return gray

def is_sorted(mat):
  h = mat.shape[0]
  for i in xrange(1, h):
    if mat[i-1, 0] > mat[i, 0]:
      return False
  return True

def app():
  imgg = rgb2gray(mpimg.imread('images/einstein.png'))
  img = imgg.astype(np.double)
  #print(img.shape)
  #imgplot1 = plt.imshow(img)
  
  edges = native_code.get_edges(img)
  edges = edges[edges[:, 0].argsort()]

  seg = native_code.Segmentation(edges)
  seg.segment()

#if __name__ == '__main__':
#  app()
