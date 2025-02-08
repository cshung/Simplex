# Simplex
This is an implementation of the Simplex algorthm for solving linear programs given in the standard form.

# How to use
The program assumes the linear program is alreay converted to the standard form as follow:

$$\begin{align*}
                      \min z &= c^T x \\
  \text{subject to} \quad Ax &= b \\
                           x &\geq 0
\end{align*}$$

We have $m$ constraints and $n$ variables, therefore $c$ is a $n \times 1$ vector, $A$ is a $m \times n$ matrix and $b$ is a $m \times 1$ vector.

The input starts with $m$ and $n$, then it follows with $n$ numbers that is $c$ and then follow with $m \times (n+1)$ numbers which is $[A|b]$ in row major order.

Once the input is given, the program will start solving, showing the immediate tableau and final result.

# Insights into the problem

## Geometric insights
Starting with the objective function $z = c^Tx$. This equation defines a hyperplane. Suppose further that $c$ is normalized so that $||c|| = 1$, 
we have $|z|$ is the shortest distance from the plane to the origin. So geometrically, we would like to slide the hyperplane towards the origin as close as possible while still maintain that a point on the hyperplane is still feasible.

The feasible region is an intersection of hyperplanes. Just like two lines intersects into a point or two planes intersects into a line, generally these hyperplane intersects into faces. Some of these faces turns into corners.

Intuitively the solution should be a corner. If we are not on a corner, then we should be able to either slide further, or if we can't, we should be on a face that is parallel to the objective hyperplane, so that we can move within the face and get to a corner.

## Algebraic translation
While geometry gives us idea on what the solution should look like, it is easier to manipulate algebra in a computer. 

Suppose we have a $n$ dimensional solution vector $x$, as such it might have $d$ non-zero entries. Suppose further the columns in $A$ corresponding to the non-zero entries in $x$ are not linearly dependent, then we can have a non-trivial (i.e. not all zero) linear combination of these columns summing up to zero. 

$a_1 A_1 + a_2 A_2 + \cdots + a_dA_d = 0$

By interpreting $A$ as a row vector where each entry is a column vector, we can interpret this equation as a matrix equation $Aa = 0$, where $a$ has $a_1 \cdots a_d$ as entry corresponding to the right column positions and $0$ otherwise.

Now there exists $A(x + da) = b$ and also $A(x - da) = b$ with $x-da \ge 0$ and $x+da \ge 0$ for small enough $d$. So $x$ is lying right in the middle of a straight line joined by $x + da$ and $x - da$, and both endpoints are feasible, and so $x$ cannot be a corner.

> We must construct $a$ using the non-negative columns only because we need to make sure there exists $d$ such that $x-da \ge 0$.

This shows us the concept of a corner is related to the linear dependencies of the corresponding non zero columns. In particular, the columns associated to the non-zero entries of a corner corner solution must be linearly independent.

Note that there are $m$ constraints, so we can have at most $m$ linearly independent columns. We call that a basis. The solution must be $0$ in the non basis columns. If the solution is also feasible (i.e. all entries are at least 0), then it is called a basic feasible solution.

# The algorithm
The idea of the simplex algorithm is to start with a basic feasible solution and work towards optimality. 

To get started, we will start with these simplifying assumptions and eventually remove them.

1. We are given a basic feasible solution.
2. The columns associated with the basis is the identity matrix.
3. The objective function does not depend on the basic variables (i.e. the coefficients associated with the basic columns is 0)

To get started, we simplify the problem further. Instead of minimizing $z = c^Tx$, we make $z$ a variable and add a constraint $c^Tx - z = 0$. Because of our assumption 3, the coefficient of this constraint will be 0 for the basic columns.

Suppose we increase the value of the non-basic variable $f$ by $d$, to maintain feasibility, we will have to decrease the values of the basic variables, and we suppose the change is small enough so that the basic variables are still non-negative. For the new constraint we just added, because the coefficients of the basic variables are $0$, we notice that $z$ will be increased by exactly $d$ times the coefficient in that constraint.

For this reason, we call this new constraint the reduced costs. The reduced costs is the amount the objective function will increase if we increase the value of the corresponding non-basic variable by 1.

To reduce the objective value upon the given solution, we want to change our basis by picking the column with a negative reduced cost to enter the basis. 

Suppose we increase the value of the entering basis variable, then the program is no longer feasible because the equality constraints are violated, we must adjust the other variables. Looking at each constraint independently, each row is associated with exactly one basic variable, so it is easy to see that by adjust the value of the existing basic variables, we can maintain feasibility, and of course, we cannot decrease that beyond zero. So we should have a critical value such that we can increase the entering variable, and at least one of the basic variable should reach 0. That would be the column to leave the basis.

Now that we have picked a column to enter the basis, we would like to maintain that the basic column is still an identity matrix, and we can do that using a procedure similar to Gaussian elimination. First, we can scale the row such that entry becomes 1, and then we can subtract every other row with a multiple of these rows so that the basic column value becomes 0.

With that, we have decreased the objective function value, as well as maintaining all the assumptions above, so we can iterate on it, until one of the two cases occurs:

- If it happens that the reduced costs are all positive, then it is impossible decrease the objective function value by picking anything to enter the basis. In that case we are done and found the optimal.
- Or if it happens that we can indefinitely increase the entering variable without hitting the corresponding basic variable to reach zero (i.e. the basic variable adjustment corresponding to the change is all increasing, ). In that case, the linear program is said to be unbounded because the objective function can go arbitrarily small.

# Eliminating the assumptions
In general we are not given a basic feasible solution, so we need to start with finding one. The idea is to introduce so artificial variables $a$ for each constraints. So instead of 

$$\begin{align*}
\min z &= c^T x \\
\text{subject to} \quad Ax &= b \\
x &\geq 0
\end{align*}$$

We optimize this instead:

$$\begin{align*}
\min y \\
\text{subject to} \quad Ax + a &= b \\
c^T x &= z \\
a^T1 &= y \\
x, a &\geq 0
\end{align*}$$

Now obviously $x = 0$, $a = b$ is a basic feasible solution, the basic columns are obviously an identity matrix, and the objective function is obviously independent of the basic variables. The program cannot be minimized arbitrarily because it is obviously lowered bounded by 0.

Applying the simplex algorithm above, the optimal solution can either be all zeroes or not. In case it is all zero, then we reach a state such that the basis column are all not on the artifical variables, and the artificial variables are all zero. Now we can simply remove all those artifical variables and replace the objective from minimizing $y$ to minimizing $z$ reach all the assumptions we needed above.

On the other hand, if the artificial variables cannot be all zero, that means there is no way we can satisfy all the constraints. In that case we can declare the program as infeasible.

# Dual
By maintaining the dual variables $y$ so that reduced cost row (of the original objective) to be $c - y^TA$, we find $y$ to be the optimal solution to the dual problem in case the program is feasible and bounded. Note that the dual variables only work on the transformed $A$. To get the dual variables for the original problem, we need to transform the dual variables back to the original space. The code explained how this is done.

# Gotchas
In case a reduced cost is zero, it does not make sense to choose it to enter the basis since that won't change objective value, but we must choose it to enter the basis if that could be used to drive the artifical variable away in the first phase. The goal of the first phase is to drive those variables out, so even if we reach 0, we still need to make sure all artifical variables are 0.

In case the given constraints are linearly dependent, we will not be able to form an identity matrix with basic columns, we must eliminate the redundant constraint in that case.