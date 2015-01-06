#include "stdafx.h"
#pragma warning(disable : 4996)

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

// Simplex lab with C++

/* public */
void linear_programming(vector<double> objective_vector, vector<vector<double>> constraint_matrix, vector<double> constraint_vector);

/* private */
void optimize_tableau(vector<vector<double>>& tableau, vector<int>& basic_columns, int phase);

int main()
{
    // Initialize tableau with sample data from book, sample problem comes from
    // Combinatorial Optimization - Algorithms and Complexity (Chapter 2 - example 2.5)

    /*string line1 = "  3   5                ";
    string line2 = "  1   1   1   1   1    ";
    string line3 = "  3   2   1   0   0   1";
    string line4 = "  5   1   1   1   0   3";
    string line5 = "  2   5   1   0   1   4";*/

    string line1 = "  3   5                ";
    string line2 = "     -1  -2   0   0   0";
    string line3 = " -2   1   1   0   0   2";
    string line4 = " -1   1   0   1   0   3";
    string line5 = "  1   0   0   0   1   3";

    string endline = "\r\n";
    istringstream iss(line1 + endline + line2 + endline + line3 + endline + line4 + endline + line5);

    int number_of_constraints;
    int number_of_variables;

    iss >> number_of_constraints;
    iss >> number_of_variables;

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
        iss >> objective_vector[n];
    }

    for (int m = 0; m < number_of_constraints; m++)
    {
        for (int n = 0; n < number_of_variables; n++)
        {
            iss >> constraint_matrix[m][n];
        }
        iss >> constraint_vector[m];
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
            if (m == 0) // original objective row
            {
                if (1 <= n && n <= number_of_original_variables)
                {
                    tableau[m][n] = objective_vector[n - 1];
                }
                else
                {
                    tableau[m][n] = 0;
                }
            }
            else if (m == number_of_constraints + 1)
            {
                if (n > number_of_original_variables)
                {
                    tableau[m][n] = 1;
                }
                else
                {
                    tableau[m][n] = 0;
                }
            }
            else
            {
                if (n == 0)
                {
                    tableau[m][n] = constraint_vector[m - 1];
                }
                else if (n > number_of_original_variables)
                {
                    tableau[m][n] = (m == n - number_of_original_variables) ? 1 : 0;
                }
                else
                {
                    tableau[m][n] = constraint_matrix[m - 1][n - 1];
                }
            }
        }
    }

    // Step 2: Initialize relative costs
    for (int m = 1; m <= number_of_constraints; m++)
    {
        for (int n = 0; n <= number_of_variables; n++)
        {
            tableau[number_of_constraints + 1][n] -= tableau[m][n];
        }
    }

    // Step 3: Initialize the set of basic columns
    vector<int> basic_columns;
    for (int m = 1; m <= number_of_constraints; m++)
    {
        basic_columns.push_back(m + number_of_original_variables);
    }

    optimize_tableau(tableau, basic_columns, /* phase = */1);
    optimize_tableau(tableau, basic_columns, /* phase = */2);
}

void optimize_tableau(vector<vector<double>>& tableau, vector<int>& basic_columns, int phase)
{
    int number_of_constraints = tableau.size() - 2;
    int number_of_variables = tableau[0].size() - 1;
    if (phase == 2)
    {
        number_of_variables -= number_of_constraints;
    }

    int objective_row = phase == 1 ? number_of_constraints + 1 : 0;
    while (true) /* optimal will break */
    {
        // Step 3: Initialize the column_to_enter_basis (bland's algorithm - first negative cost column enter basis)
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
            // we reached optimal
            break;
        }

        // Step 4: Find out which row to enter basis
        bool first = true;
        double theta = 0;
        int row = -1;
        for (int m = 1; m <= number_of_constraints; m++)
        {
            if (tableau[m][column_to_enter_basis] > 0)
            {
                double theta_candidate = /* basic variable value */tableau[m][0] / tableau[m][column_to_enter_basis];
                if (first)
                {
                    row = m;
                    theta = theta_candidate;
                    first = false;
                }
                else if (theta > theta_candidate)
                {
                    row = m;
                    theta = theta_candidate;
                }
            }
        }

        basic_columns[row - 1] = column_to_enter_basis;

        // Step 5: Scale the row to enter basis to have coefficient 1
        double factor = tableau[row][column_to_enter_basis];
        for (int n = 0; n < (number_of_variables + 1); n++)
        {
            tableau[row][n] = tableau[row][n] / factor;
        }

        // Step 6: Zero out the other rows
        for (int other_row = 0; other_row < (number_of_constraints + 2); other_row++)
        {
            if (other_row != row)
            {
                factor = tableau[other_row][column_to_enter_basis];
                for (int n = 0; n < (number_of_variables + 1); n++)
                {
                    tableau[other_row][n] = tableau[other_row][n] - factor * tableau[row][n];
                }
            }
        }

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
}