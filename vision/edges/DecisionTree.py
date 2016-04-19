import numpy as np

def shannon_entropy(y):
  p_y = np.bincount(y)
  log_p_y = np.log(p_y)
  return -1.0 * np.dot(p_y, log_p_y)

def gini_entropy(y):
  p_y = np.bincount(y)
  return np.dot(p_y, 1.0 - p_y)

def information_gain(y, y_l, y_r):
  h_y = shannon_entropy(y)
  h_y_l = shannon_entropy(y_l)
  h_y_r = shannon_entropy(y_r)

  c_l = 1.0f * y_l.shape[0]
  c_r = 1.0f * y_r.shape[1]
  c = 1.0f * y.shape[0]

  return h_y - c_l * h_y_l - c_r * h_y_r


