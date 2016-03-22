import numpy as np

class LinearInverseSolver:
  def __init__(self, A, b, eta=2e-5, t=0.0002):
    self.A = A
    self.b = b
    self.eta = eta
    self.t = t

  def solve(self):
    pass

  @staticmethod
  def t_operator(self, alpha, X):
    return np.maximum((np.abs(X) - alpha), 0.0) * np.sign(X)

class ISTASolver(LinearInverseSolver):
  def solve(self):
    new_X = np.random()
    loss_history = []
    loss_history.append(new_X)
    for i in xrange(num_iterations):
      x = new_X - 2 * self.t * np.dot(self.A.T, np.dot(self.A, new_X) - self.b)
      new_X = LinearInverseSolver.t_operator(self.eta * self.t, x)
      loss_history.append(new_X)
    return loss_history

class FISTASolver(LinearInverseSolver):
  def __init__(self):
    self.ab = np.dot(A.T, b)
    self.aa = np.dot(A.T, A)

  def solve(self):
    # some prelim
    # we want to initialize X_0 as A\b
    # if Ax=b, then x = A\b. A good starting point!
    Xk1 = np.linalg.lstsq(self.A, self.b)
    Xk = Xk1
    tk1, tk = (1.0, 1.0)
    L = 1.0
    beta = 1.5

    loss_history = []

    ## TODO : we iterate over a fixed number
    ## of iterations. We can have better 
    ## stopping criteria - look at references
    for it in xrange(num_iters):
      Yk = Xk + ((tk1 - 1.0) / tk) * (Xk - Xk1)

      backtrack = True
      tmp = np.dot(self.aa, Yk) - self.ab
      while backtrack:
        Gk = Yk - (1.0/L) * tmp
        x_tmp = LinearInverseSolver.t_operator(Gk, self.eta / L)

        axb_tmp = b - np.dot(A, x_tmp)
        xkyk = x_tmp - Yk

        tmp1 = np.dot(axb, axb)
        tmp2 = tmp1 + np.dot(xkyk, tmp) + (L/2.0)*np.dot(xkyk, xkyk)

        if tmp1 <= tmp2:
          Xk1 = Xk
          Xk = x_tmp
          backtrack = False
        else
          L = beta * L

      t_tmp = 0.5 * (1 + np.sqrt(1 + 4 * tk * tk))
      tk1 = tk
      tk = t_tmp

      f = np.dot(axb, axb) + self.eta * np.abs(Xk)
      loss_history.append(f)

    return loss_history

if __name__ == '__main__':
  # load an image.
  # blur it
  # apply linear inverse solver to deblur it
  # display and all that stuff
