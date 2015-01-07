#include "stdafx.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cassert>
using namespace std;

// Simplex lab with C++

/* public */
void linear_programming(vector<double> objective_vector, vector<vector<double>> constraint_matrix, vector<double> constraint_vector);

/* private */
bool optimize_tableau(vector<vector<double>>& tableau, vector<int>& basic_columns, int phase);

/* private */
void show_tableau(int number_of_constraints, int number_of_variables, vector<vector<double>>& tableau, vector<int>& basic_columns);

int main()
{
    // Initialize tableau with sample data from book, sample problem comes from
    // Combinatorial Optimization - Algorithms and Complexity (Chapter 2 - example 2.5)

    // string line1 = "  3   5                ";
    // string line2 = "  1   1   1   1   1    ";
    // string line3 = "  3   2   1   0   0   1";
    // string line4 = "  5   1   1   1   0   3";
    // string line5 = "  2   5   1   0   1   4";

    // string line1 = "  3   5                ";
    // string line2 = "     -1  -2   0   0   0";
    // string line3 = " -2   1   1   0   0   2";
    // string line4 = " -1   1   0   1   0   3";
    // string line5 = "  1   0   0   0   1   3";

    // string endline = "\r\n";
    // istringstream iss(line1 + endline + line2 + endline + line3 + endline + line4 + endline + line5);

    istream& is = cin;

    int number_of_constraints;
    int number_of_variables;

    is >> number_of_constraints;
    is >> number_of_variables;

    vector<double> objective_vector;
    vector<vector<double>> constraint_matrix;
    vector<double> constraint_vector;
    objective_vector.resize(number_of_variables);
    constraint_matrix.resize(number_of_constraints);
    constraint_vector.resize(number_of_constraints);

    for (int m = 0; m < number_of_constraints; m++)
    {
        constraint_matrix[m].resize(number_of_variables);
    }

    for (int n = 0; n < number_of_variables; n++)
    {
        is >> objective_vector[n];
    }

    for (int m = 0; m < number_of_constraints; m++)
    {
        for (int n = 0; n < number_of_variables; n++)
        {
            is >> constraint_matrix[m][n];
        }
        is >> constraint_vector[m];
    }

    linear_programming(objective_vector, constraint_matrix, constraint_vector);

    return 0;
}

void linear_programming(vector<double> objective_vector, vector<vector<double>> constraint_matrix, vector<double> constraint_vector)
{
    // Step 1: Initialize tableau (for phase 1 to determine feasible basis)
    int number_of_constraints = constraint_matrix.size();
    int number_of_original_variables = objective_vector.size();
    int number_of_variables = number_of_original_variables + number_of_constraints;
    vector<vector<double>> tableau;
    tableau.resize(number_of_constraints + 2);
    for (int m = 0; m < number_of_constraints + 2; m++)
    {
        tableau[m].resize(number_of_variables + 1);
        for (int n = 0; n <= number_of_variables; n++)
        {
            if (m == 0)                                                                      // original objective row
            {
                if (1 <= n && n <= number_of_original_variables)
                {
                    tableau[m][n] = objective_vector[n - 1];                                 // original objective vector values
                }
                else
                {
                    tableau[m][n] = 0;                                                       // original objective value does not depend on artificial variables
                }
            }
            else if (m == number_of_constraints + 1)                                         // artificial objective row
            {
                if (n > number_of_original_variables)
                {
                    tableau[m][n] = 1;                                                       // each artificial variables contributes 1 to the artificial objective values
                }
                else
                {
                    tableau[m][n] = 0;                                                       // artificial objective value does not depend on original variables
                }
            }
            else
            {
                if (n == 0)                                                                  // required value column
                {
                    tableau[m][n] = constraint_vector[m - 1];                                // required values
                }
                else if (n > number_of_original_variables)                                   // artificial variables
                {
                    tableau[m][n] = (m == n - number_of_original_variables) ? 1 : 0;         // each artificial values binds to an unique constraint.
                }
                else
                {
                    tableau[m][n] = constraint_matrix[m - 1][n - 1];                         // required constraint coefficients
                }
            }
        }
    }

    // Step 2: Make sure constraint values are positive for first phase
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

    // Step 5: First phase optimization to find a basic feasible solution
    cout << "Phase 1: " << endl << endl;
    bool first_phase_optimization_bounded = optimize_tableau(tableau, basic_columns, /* phase = */1);

    assert(first_phase_optimization_bounded == true); /* The first phase optimization cannot possibly be unbounded. */

    cout << -tableau[number_of_constraints + 1][0] << " is the objective value for phase 1" << endl << endl;

    if (abs(tableau[number_of_constraints + 1][0]) < 1e-6)
    {
        // Step 6: Second phase optimization to find a optimal basic feasible solution
        cout << "Phase 2: " << endl << endl;
        bool second_phase_optimization_bounded = optimize_tableau(tableau, basic_columns, /* phase = */2);
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

            for (int n = 0; n < number_of_original_variables; n++)
            {
                cout << solution[n] << " ";
            }

            cout << endl;
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

bool optimize_tableau(vector<vector<double>>& tableau, vector<int>& basic_columns, int phase)
{
    int number_of_constraints = tableau.size() - 2;
    int number_of_variables = tableau[0].size() - 1;
    if (phase == 2)
    {
        number_of_variables -= number_of_constraints;
    }

    show_tableau(number_of_constraints, number_of_variables, tableau, basic_columns);

    int objective_row = phase == 1 ? number_of_constraints + 1 : 0;

    while (true) /* The loop will break when we reach optimality or conclude the linear program is unbounded */
    {
        // Step 1: Find the column_to_enter_basis
        // (bland's algorithm - first negative cost column enter basis)
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
                if (abs(tableau[number_of_constraints + 1][0]) < 1e-6)
                {
                    // TODO: In phase 1, we need to also make sure all artificial variables are out of the basis, how?

                    // This is what we should make sure
                    for (int m = 0; m < number_of_constraints; m++)
                    {
                        assert(basic_columns[m] <= number_of_variables - number_of_constraints);
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
                    // (bland's algorithm - first column reach maximal enter basis)
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

        cout << "Pivoting on row " << row_to_leave_basis << " column " << column_to_enter_basis << "." << endl<< endl;

        basic_columns[row_to_leave_basis - 1] = column_to_enter_basis;

        // Step 3: Scale the row to enter basis to have coefficient 1
        double factor = tableau[row_to_leave_basis][column_to_enter_basis];
        for (int n = 0; n < (number_of_variables + 1); n++)
        {
            tableau[row_to_leave_basis][n] = tableau[row_to_leave_basis][n] / factor;
        }

        // Step 4: Zero other rows
        for (int other_row = 0; other_row < (number_of_constraints + 2); other_row++)
        {
            if (other_row != row_to_leave_basis)
            {
                factor = tableau[other_row][column_to_enter_basis];
                for (int n = 0; n < (number_of_variables + 1); n++)
                {
                    tableau[other_row][n] = tableau[other_row][n] - factor * tableau[row_to_leave_basis][n];
                }
            }
        }

        show_tableau(number_of_constraints, number_of_variables, tableau, basic_columns);
    }
}

void show_tableau(int number_of_constraints, int number_of_variables, vector<vector<double>>& tableau, vector<int>& basic_columns)
{
    // Show the tableau
    for (int m = 0; m < (number_of_constraints + 2); m++)
    {
        for (int n = 0; n < (number_of_variables + 1); n++)
        {
            cout << setprecision(2) << fixed << tableau[m][n] << "\t";
        }
        cout << endl;
    }
    for (int i = 0; i < number_of_constraints; i++)
    {
        cout << basic_columns[i] << "\t";
    }
    cout << endl;
    cout << endl;
}