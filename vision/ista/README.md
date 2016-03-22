Iterative Shrinkage-Thresholding Algorithm for Linear Inverse Problems

On a very high level, we are aiming to solve the problem of determining `x` 
such that `||Ax - b||_2` is minimized.

There are a lot of solutions. But one attractive one is FISTA. F for fast. 
It's fast because it avoids calculation of A.T * A multiple number of times.

Most implementations use L1 regularization. I have done the same.

For a much better understanding, reading up these links would be highly useful

1. [Paper on FISTA which is also a decent introduction of ISTA](http://people.rennes.inria.fr/Cedric.Herzet/Cedric.Herzet/Sparse_Seminar/Entrees/2012/11/12_A_Fast_Iterative_Shrinkage-Thresholding_Algorithmfor_Linear_Inverse_Problems_(A._Beck,_M._Teboulle)_files/Breck_2009.pdf)
2. [Benchmarks, Discussion and Implementation in Matlab](http://www.eecs.berkeley.edu/~yang/software/l1benchmark/)

### But why should I care? ###

Well, for one, it seems to be used a lot in computing overcomplete dictionary - 
a step very common when one has a small dataset and want to build on it.

Often, `A` and `x` both are unknown and we progress in an EM style to 
estimate both.

This [paper](http://soumith.ch/pedestrian-cvpr-13.pdf) is a great resource 
on this.
