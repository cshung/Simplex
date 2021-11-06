# Simplex
This is an implementation of the Simplex algorthm for solving linear programs given in the standard form.

# How to use
The program assumes the linear program is alreay converted to the standard form as follow

```
min z = c'x
subject to Ax  = 0
       and  x >= 0
```

We have `m` constraints and `n` variables, therefore `c` is a `nx1` vector and `A` is a `mxn` matrix.

The input starts with `m` and `n`, then it follows with `n` numbers that is `c` and then follow with `mxn` numbers which is `A`.

Once the input is given, the program will start solving, showing the immediate tableau and final result.

# Insights into the problem

## Geometric insights
Starting with the objective function `z = c'x`. This equation defines a hyperplane. `z` is the shortest distance from the plane to the origin. So geometrically, we would like to slide the hyperplane towards the origin as close as possible while still maintain that a point on the hyperplane is still feasible.

The feasible region is an intersection of hyperplanes. Just like two lines intersects into a point or two planes intersects into a line, generally these hyperplane intersects into faces. Some of these faces turns into corners.

Intuitively the solution should be a corner. If we are not on a corner, then we should be able to either slide further, or if we can't, we should be on a face that is parallel to the objective hyperplane, so that we can move within the face and get to a corner.

## Algebraic translation
While geometry gives us idea on what the solution should look like, it is easier to manipulate algebra in a computer. 

Suppose we have a solution, that must be a `n` dimensional vector. Out of these `n` numbers, there are `d` of them that are non zero. Suppose they are not linearly dependent, then we can have non-trivial a linear combination of these columns summing up to zero. 

`a1 A1 + a2 A2 + ... + anAn = 0`

Note that by interpreting `A` as a row vector where each entry is a column vector, we can interpret this equation as a matrix equation.

`Aa = 0`, where `a` has `a1 .. an` as entry corresponding to the right column positions and `0` otherwise.

Now `A(x + a) = 0` and also `A(x - a)=0`. So `x` is lying right in the middle of a straight line joined by `x + a` and `x - a`, and both endpoints are feasible. So `x` cannot be a corner.

This shows us the concept of corner is related to the linear dependencies of the corresponding non zero columns. In particular, the columns associated to the non-zero entries of a corner corner solution must be linearly independent.

Note that there are `m` constraints, so we can have at most `m` linearly independent columns. We call that a basis. The solution must be `0` in the non basis columns. If the solution is also feasible (i.e. all entries are at least 0), then it is called a basic feasible solution.

# The algorithm
The idea of the simplex algorithm is to start with a basic feasible solution and work towards optimality. 

To get started, we will start with these simplifying assumptions and eventually remove them.

1. We are given a basic feasible solution.
2. The columns associated with the basis is the identity matrix.
3. The objective function does not depend on the basic variables (i.e. the coefficients associated with the basic columns is 0)

To improve upon the given solution, we want to change our basis. Because the solution is also non-negative, we can improve the objective by picking the column with a negative objective coefficient to enter the basis. 

Suppose we increase the value of the entering basis variable, then the program is no longer feasible because the equality constraints are violated, we must adjust the other variables. Looking at each constraint independently, each row is associated with exactly one basic variable, so it is easy to see that by adjust the value of the existing basic variables, we can maintain feasibility, and of course, we cannot decrease that beyond zero. So we should have a critical value such that we can increase the entering variable, and at least one of the basic variable should reach 0. That would be the column to leave the basis.

Now that we have picked a column to enter the basis, we would like to maintain that the basic column is still an identity matrix, and we can do that using a procedure similar to Gaussian elimination. First, we can scale the row such that entry becomes 1, and then we can subtract every other row with a multiple of these rows so that the basic column value becomes 0.

We also want to maintain that the objective function is independent of the basic variables. We can also do that by subtracting it with a multiple of the row.

With that, we have decreased the objective function value, as well as maintaining all the assumptions above, so we can iterate on it, until one of the two cases occurs:

- If it happens that the objective function have all positive coefficients, then it is impossible decrease the objective function value by picking anything to enter the basis. In that case we are done and found the optimal.
- Or if it happens that we can indefinitely increase the entering variable without hitting the corresponding basic variable to reach zero. In that case, the linear program is said to be unbounded because the objective function can go arbitrarily small.

# Eliminating the assumptions
In general we are not given a basic feasible solution, so we need to start with finding one. The idea is to introduce so artificial variables for each constraints. So instead of 

```
          min z  = c'x
subjected to Ax  = b
             x  >= 0
```
We optimize this instead
```
                   min y  = 1'a
subjected to [A|I][x;a]   = b
                     c'x  = z
                    x,a  >= 0
```

Using `b` as the initial basic feasible solution, the basic columns are obviously an identity matrix, the objective function can be easily subtracted by every row so that it is independent of the basic variables. The program cannot be minimized arbitrarily because it is obviously lowered bounded by 0.

Applying the simplex algorithm above, the optimal solution can either be all zero or not. In case it is all zero, then we reach a state such that the basis column are all not on the artifical variables, and the artificial variables are all zero. Now we can simply remove all those artifical variables and reach the assumptions we needed.

Note that by virtue of putting `c'x = z` into the constraint, we automatic get an equation for `z` that does not depend on the basic variables.

On the other hand, if the artificial variables cannot be all zero, that means there is no way we can satisfy all the constraints. In that case we can declare the program as infeasible.

# Gotchas
In case an objective function coefficient is zero, it does not make sense to choose it to enter the basis since that won't change objective value, but we must choose it to enter the basis if that could be used to drive the artifical variable away in the first phase. The goal of the first phase is to drive those variables out, so even if we reach 0, we still need to make sure all artifical variables are 0.

In case the given constraints are linearly dependent, we will not be able to form an identity matrix with basic columns, we must eliminate the redundant constraint in that case.