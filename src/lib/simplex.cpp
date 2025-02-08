#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <iomanip>
#include <cassert>
#include <cmath>
using namespace std;

// Simplex lab with C++

/* public */
void linear_programming(vector<double> objective_vector, vector<vector<double> > constraint_matrix, vector<double> constraint_vector);

/* private */
bool optimize_tableau(vector<vector<double> >& tableau, vector<int>& basic_columns, int phase, stack<vector<double> >& transformations);

/* private */
void show_tableau(int phase, int number_of_constraints, int number_of_variables, vector<vector<double> >& tableau, vector<int>& basic_columns);

/* private */
void pivot(int phase, int number_of_constraints, int number_of_variables, vector<vector<double> >& tableau, vector<int>& basic_columns, int row_to_leave_basis, int column_to_enter_basis, stack<vector<double> >& transformations);

void linear_programming(vector<double> objective_vector, vector<vector<double> > constraint_matrix, vector<double> constraint_vector)
{
    // Step 1: Initialize tableau (for phase 1 to determine feasible basis)
    int number_of_constraints = constraint_matrix.size();
    int number_of_original_variables = objective_vector.size();
    int number_of_variables = number_of_original_variables + number_of_constraints;
    vector<vector<double> > tableau;
    tableau.resize(number_of_constraints + 2);
    for (int m = 0; m < number_of_constraints + 2; m++)
    {
        tableau[m].resize(number_of_variables + 2);
        for (int n = 0; n <= number_of_variables + 1; n++)
        {
            if (m == 0)                                                                      // original objective row
            {
                if (1 <= n && n <= number_of_original_variables)
                {
                    tableau[m][n] = objective_vector[n - 1];                                 // original objective vector values
                }
                else if (n == number_of_variables + 1)
                {
                    tableau[m][n] = 0;                                                       // dual variables
                }
                else
                {
                    tableau[m][n] = 0;                                                       // original objective value does not depend on artificial variables
                }
            }
            else if (m == number_of_constraints + 1)                                         // artificial objective row
            {
                if (n <= number_of_original_variables)
                {
                    tableau[m][n] = 0;                                                       // artificial objective value does not depend on original variables
                }
                else if (n <= number_of_variables)
                {
                    tableau[m][n] = 1;                                                       // each artificial variable contributes 1 to the artificial objective values
                }
                else
                {
                    tableau[m][n] = 0;                                                       // dual variables
                }
            }
            else
            {
                if (n == 0)                                                                  // required value column
                {
                    tableau[m][n] = constraint_vector[m - 1];                                // required values
                }
                else if (n <= number_of_original_variables)
                {
                    tableau[m][n] = constraint_matrix[m - 1][n - 1];                         // required constraint coefficients
                }
                else if (n <= number_of_variables)                                           // artificial variables
                {
                    tableau[m][n] = (m == n - number_of_original_variables) ? 1 : 0;         // each artificial value binds to a unique constraint.
                }
                else
                {
                    tableau[m][n] = 0;                                                       // dual variables
                }
            }
        }
    }

    // Step 2: Make sure constraint values are positive for the first phase
    for (int m = 1; m <= number_of_constraints; m++)
    {
        if (tableau[m][0] < 0)
        {
            for (int n = 0; n <= number_of_original_variables; n++)
            {
                tableau[m][n] = -tableau[m][n];
            }
        }
    }

    // Step 3: Initialize relative costs
    // relative cost represents the unit change in the objective function whenever a non-basic variable changes a little bit
    // it is used to decide the variable to enter the basis
    for (int m = 1; m <= number_of_constraints; m++)
    {
        for (int n = 0; n <= number_of_variables; n++)
        {
            tableau[number_of_constraints + 1][n] -= tableau[m][n];
        }
    }

    // Step 4: Initialize the set of basic columns
    vector<int> basic_columns;
    for (int m = 1; m <= number_of_constraints; m++)
    {
        basic_columns.push_back(m + number_of_original_variables);
    }

    stack<vector<double> > transformations;

    // Step 5: First phase optimization to find a basic feasible solution
    cout << "Phase 1: " << endl << endl;
    bool first_phase_optimization_bounded = optimize_tableau(tableau, basic_columns, /* phase = */1, transformations);
    number_of_constraints = tableau.size() - 2;

    assert(first_phase_optimization_bounded == true); /* The first phase optimization cannot possibly be unbounded. */

    cout << -tableau[number_of_constraints + 1][0] << " is the objective value for phase 1" << endl << endl;

    if (abs(tableau[number_of_constraints + 1][0]) < 1e-6)
    {
        // Step 6: Second phase optimization to find an optimal basic feasible solution
        cout << "Phase 2: " << endl << endl;
        bool second_phase_optimization_bounded = optimize_tableau(tableau, basic_columns, /* phase = */2, transformations);
        if (second_phase_optimization_bounded)
        {
            cout << -tableau[0][0] << " is the optimal objective value" << endl << endl;
            vector<double> solution;
            solution.resize(number_of_original_variables);
            for (int n = 0; n < number_of_original_variables; n++)
            {
                solution[n] = 0;
            }

            for (unsigned int i = 0; i < basic_columns.size(); i++)
            {
                solution[basic_columns[i] - 1] = tableau[i + 1][0];
            }

            cout << "The variable values are:" << endl;
            for (int n = 0; n < number_of_original_variables; n++)
            {
                cout << solution[n] << " ";
            }
            cout << endl << endl;

            vector<double> dual;
            for (int i = 0; i < number_of_constraints; i++)
            {
                dual.push_back(tableau[i + 1][number_of_variables + 1]);
            }
            while (!transformations.empty())
            {
                //
                // The dual variables in the tableau are corresponding to the transformed A
                // We need to transform it back to the original A by doing the transformation in "reversed" steps
                //
                // It is NOT undo, it is applying the transpose.
                // 
                // In particular, denote the overall transformation to A by left multiplying a matrix L, we have found
                // c - (LA)'y = reduced cost and (Lb)'y = objective
                //
                // which simplifies to
                // c - A'(L'y) = reduced cost and b'(L'y) = objective
                //
                // and so L'y is the dual variable for the original problem
                //
                // The following code compute L'y by applying the transpose of the transform matrices in reverse
                // The transpose of scaling a row is still scaling the same row
                // But the transpose of subtracting row 1 by factor of row 2 will be subtracting row 2 by factor of row 1
                //
                vector<double> transformation = transformations.top();
                transformations.pop();
                if (transformation[0] == 1) {
                    int row = static_cast<int>(transformation[1]);
                    double scale = transformation[2];
                    dual[row - 1] *= scale;
                } else if (transformation[0] == 2) {
                    int row1 = static_cast<int>(transformation[1]);
                    int row2 = static_cast<int>(transformation[2]);
                    double scale = transformation[3];
                    dual[row2 - 1] -= scale * dual[row1 - 1];
                }
            }
            
            cout << "The dual variable values are:" << endl;
            for (int i = 0; i < number_of_constraints; i++)
            {
                cout << dual[i] << " ";
            }
            cout << endl << endl;
        }
        else
        {
            cout << "The linear program is unbounded" << endl;
        }
    }
    else
    {
        cout << "The linear program is infeasible" << endl;
    }
}

bool optimize_tableau(vector<vector<double> >& tableau, vector<int>& basic_columns, int phase, stack<vector<double> >& transformations)
{
    int number_of_constraints = tableau.size() - 2;
    int number_of_variables = tableau[0].size() - 2;
    if (phase == 2)
    {
        number_of_variables -= number_of_constraints;
    }

    show_tableau(phase, number_of_constraints, number_of_variables, tableau, basic_columns);

    int objective_row = phase == 1 ? number_of_constraints + 1 : 0;

    while (true) /* The loop will break when we reach optimality or conclude the linear program is unbounded */
    {
        // Step 1: Find the column_to_enter_basis
        // (Bland's algorithm - first negative cost column enters basis)
        int column_to_enter_basis = -1;
        for (int c = 1; c <= number_of_variables; c++)
        {
            if (tableau[objective_row][c] < 0)
            {
                column_to_enter_basis = c;
                break;
            }
        }

        if (column_to_enter_basis == -1)
        {
            if (phase == 1)
            {
                // If the optimal value of the first phase is 0
                if (abs(tableau[objective_row][0]) < 1e-6)
                {
                    // We need to check that all artificial variables are eliminated from the basis
                    for (int m = 0; m < number_of_constraints; m++)
                    {
                        assert(tableau[m + 1][basic_columns[m]] == 1);
                        if (basic_columns[m] > number_of_variables - number_of_constraints)
                        {
                            // If the artificial variable is still in the basis
                            bool found = false;
                            // Find any non-zero, non-artificial variable to enter the basis
                            for (int n = 1; n <= number_of_variables - number_of_constraints; n++)
                            {
                                if (abs(tableau[m + 1][n]) > 1e-6)
                                {
                                    pivot(phase, number_of_constraints, number_of_variables, tableau, basic_columns, m + 1, n, transformations);
                                    found = true;
                                    break;
                                }
                            }
                            if (!found)
                            {
                                // In this case, all the non-artificial variable columns are 0, meaning the whole constraint 
                                // is simply 0 = 0, eliminate this constraint.
                                tableau.erase(tableau.begin() + (m + 1));
                                basic_columns.erase(basic_columns.begin() + m);
                                number_of_constraints--;
                                number_of_variables--;
                                for (int row = 0; row < number_of_constraints + 2; row++)
                                {
                                    tableau[row].erase(tableau[row].begin() + basic_columns[m]);
                                }
                                cout << "Eliminating the redundant constraint #" << (m+1) << endl << endl;
                                show_tableau(phase, number_of_constraints, number_of_variables, tableau, basic_columns);
                            }
                        }
                    }
                }
            }
            // we reached optimal
            return true;
        }

        // Step 2: Find the row_to_leave_basis
        bool theta_uninitialized = true;
        double theta = 0;
        int row_to_leave_basis = -1;
        for (int m = 1; m <= number_of_constraints; m++)
        {
            if (tableau[m][column_to_enter_basis] > 0)
            {
                double theta_candidate = /* basic variable value = */ tableau[m][0] / tableau[m][column_to_enter_basis];
                if (theta_uninitialized)
                {
                    row_to_leave_basis = m;
                    theta = theta_candidate;
                    theta_uninitialized = false;
                }
                else if (theta > theta_candidate)
                {
                    // (Bland's algorithm - first column to reach maximal enters basis)
                    row_to_leave_basis = m;
                    theta = theta_candidate;
                }
            }
        }

        if (row_to_leave_basis == -1)
        {
            // No row can leave basis - the linear program is unbounded
            return false;
        }

        // Step 3: Pivoting on row_to_leave_basis and column_to_enter_basis
        pivot(phase, number_of_constraints, number_of_variables, tableau, basic_columns, row_to_leave_basis, column_to_enter_basis, transformations);

        show_tableau(phase, number_of_constraints, number_of_variables, tableau, basic_columns);
    }
}

void pivot(int phase, int number_of_constraints, int number_of_variables, vector<vector<double> >& tableau, vector<int>& basic_columns, int row_to_leave_basis, int column_to_enter_basis, stack<vector<double> >& transformations)
{
    cout << "Pivoting on row " << row_to_leave_basis << " column " << column_to_enter_basis << "." << endl<< endl;
    basic_columns[row_to_leave_basis - 1] = column_to_enter_basis;

    // Step 1: Scale the row to enter basis to have coefficient 1
    double scale = tableau[row_to_leave_basis][column_to_enter_basis];

    for (int n = 0; n < (number_of_variables + 1); n++)
    {
        tableau[row_to_leave_basis][n] = tableau[row_to_leave_basis][n] / scale;
    }

    // To maintain the invariant that the reduced cost row is c - A'y
    // as the row scale down, the coefficient scale up
    tableau[row_to_leave_basis][number_of_variables + 1] *= scale;

    // Record the transformation so that we can use that to extract dual variable value for the original problem
    transformations.push({1, static_cast<double>(row_to_leave_basis), 1/scale});

    // Step 2: Zero other rows
    for (int other_row = 0; other_row < (number_of_constraints + 2); other_row++)
    {
        if (other_row != row_to_leave_basis)
        {
            double factor = tableau[other_row][column_to_enter_basis];
            for (int n = 0; n < (number_of_variables + 1); n++)
            {
                tableau[other_row][n] = tableau[other_row][n] - factor * tableau[row_to_leave_basis][n];
            }
            if (other_row == 0)
            {
                // To maintain the invariant that the reduced cost row is c - A'y
                // as the reduced row is subtracted, the coefficient of the that row increases
                tableau[row_to_leave_basis][number_of_variables + 1] += factor;
            }
            if (1 <= other_row and other_row <= number_of_constraints)
            {
                // Record the transformation so that we can use that to extract dual variable value for the original problem
                transformations.push({2, static_cast<double>(other_row), static_cast<double>(row_to_leave_basis), factor});
            }
        }
    }
}

/**
 * How to read a tableau:
 *
 * The first row represents the relative cost, the first element is the negative objective, and the rest is how much the objective could change 
 * if there is a unit change in the corresponding variable while maintaining the equality constraints.
 *
 * In the first phase, we will have a second relative cost row corresponding to the original objective.
 *
 * The middle row represents b = Ax, the first column is b, the remaining column is A, the last column is the dual variable.
 * The value of b is also the values of the basic variable, the non-basic variables will be zero.
 * The dual variables maintains the invariant that c - A'y is the reduced cost row, for the transformed A.
 *
 * The numbers at the bottom indicate which columns are the basic columns
 */
void show_tableau(int phase, int number_of_constraints, int number_of_variables, vector<vector<double> >& tableau, vector<int>& basic_columns)
{
    int dual_column = tableau[0].size() - 1;
    int objective_row = phase == 1 ? number_of_constraints + 1 : 0;
    for (int p = 0; p < (3 - phase); p++)
    {
        if (p == 1)
        {
            objective_row = 0;
        }
        for (int n = 0; n < (number_of_variables + 1); n++)
        {
            cout << setprecision(2) << fixed << tableau[objective_row][n] << "\t";
        }
        if (p == 0)
        {
            cout << "Dual";
        }
        cout << endl;
    }
    for (int n = 0; n < (number_of_variables + 2); n++)
    {
        cout << "----\t";
    }
    cout << endl;

    for (int m = 1; m <= number_of_constraints; m++)
    {
        for (int n = 0; n < (number_of_variables + 1); n++)
        {
            cout << setprecision(2) << fixed << tableau[m][n] << "\t";
        }
        cout << setprecision(2) << fixed << tableau[m][dual_column];
        cout << endl;
    }
    for (int n = 0; n < (number_of_variables + 2); n++)
    {
        bool found = false;
        for (int i = 0; i < number_of_constraints; i++)
        {
            if (basic_columns[i] == n)
            {
                cout << i << "\t";
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "\t";
        }
    }
    cout << endl << endl;
}
