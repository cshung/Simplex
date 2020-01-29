The key reason why we can pivot the objective row and why pivoting the objective row leads to reduced cost is that
1) We are representing the objective as a linear function of the non-basic variables and so
2) it is valid to perform row operation on an equation, and also
3) As a linear equation that describe the objective function, the coefficients are reduced costs!

  3   5
  1   1   1   1   1
  3   2   1   0   0   1
  5   1   1   1   0   3
  2   5   1   0   1   4

4.50 is the optimal objective value

0.00 0.50 0.00 2.50 1.50

  2   4
 -1  -2  0   0
 -1   1  1   0   2
 -2   1  0   1   1

The linear program is unbounded

  3   5
 -1  -2   0   0   0
 -2   1   1   0   0   2
 -1   2   0   1   0   7
  1   0   0   0   1   3

-13.00 is the optimal objective value

3.00 5.00 3.00 0.00 0.00

  2   3
 -1   0   0   0
  1   1  -1   0   6
  2   3   0   1   4

The linear program is infeasible

// Useless variable

2 4
1 0 0 0
1 0 1 0 2
0 1 0 1 2

-0.00 is the optimal objective value

0.00 2.00 2.00 0.00

// A highly degenerate linear program from a weighted directed graph shortest path problem.

  3   5
  1   2   2   3   1
  1   1   0   0   0   1
  0   0   0  -1  -1  -1
 -1   0   1   1   0   0

3.00 is the optimal objective value

0.00 1.00 0.00 0.00 1.00

// We need to make sure we 'drive out' the artificial variables

1 2
 1 1 
-1 0 0

-0.00 is the optimal objective value

-0.00 0.00 
