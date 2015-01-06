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

void optimize_tableau(vector<vector<double>>& tableau, vector<int>& basic_columns);

void linear_programming(vector<double> objective_vector, vector<vector<double>> constraint_matrix, vector<double> constraint_vector);

int main()
{
    // Initialize tableau with sample data from book, sample problem comes from
    // Combinatorial Optimization - Algorithms and Complexity (Chapter 2 - example 2.5)

    string line1 = "  3   5";
    string line2 = "      1   1   1   1   1";
    string line3 = "  3   2   1   0   0   1";
    string line4 = "  5   1   1   1   0   3";
    string line5 = "  2   5   1   0   1   4";
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
    // 1st phase linear programming to find a feasible basis 
    int number_of_constraints = constraint_matrix.size();
    int number_of_variables = objective_vector.size() + number_of_constraints;
    vector<vector<double>> tableau;
    tableau.resize(number_of_constraints + 1);
    for (int m = 0; m < number_of_constraints + 1; m++)
    {
        tableau[m].resize(number_of_variables + 1);
        for (int n = 0; n <= number_of_variables; n++)
        {
            if (m == 0)
            {
                if (1 <= n && n <= number_of_constraints)
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
                else if (1 <= n && n <= number_of_constraints)
                {
                    tableau[m][n] = (m == n) ? 1 : 0;
                }
                else
                {
                    tableau[m][n] = constraint_matrix[m - 1][n - number_of_constraints - 1];
                }
            }
        }
    }

    // TODO: Compute relative cost

    // Step 2: Initialize the set of basic columns
    vector<int> basic_columns;
    basic_columns.push_back(1);
    basic_columns.push_back(2);
    basic_columns.push_back(3);

    optimize_tableau(tableau, basic_columns);
}

void optimize_tableau(vector<vector<double>>& tableau, vector<int>& basic_columns)
{
    int number_of_constraints = tableau.size() - 1;
    int number_of_variables = tableau[0].size() - 1;
    while (true) /* optimal will break */
    {
        // Step 3: Initialize the column_to_enter_basis (bland's algorithm - first negative cost column enter basis)
        int column_to_enter_basis = -1;
        for (int c = 1; c <= number_of_variables; c++)
        {
            if (tableau[0][c] < 0)
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
        for (int other_row = 0; other_row < (number_of_constraints + 1); other_row++)
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
        for (int m = 0; m < (number_of_constraints + 1); m++)
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