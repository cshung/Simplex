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
int main()
{
    int number_of_constraints = 3;
    int number_of_variables = 8;
    vector<vector<double>> tableau;
    tableau.resize(number_of_constraints + 1);
    for (int m = 0; m < number_of_constraints + 1; m++)
    {
        tableau[m].resize(number_of_variables + 1);
    }

    // Step 1: Initialize tableau with sample data from book 
    // Combinatorial Optimization - Algorithms and Complexity
    // Chapter 2

    // TODO: Compute the first line from input
    // TODO: Refactor so that we can run phase 2
    string line1 = " -8   0   0   0 -10  -8  -3  -1  -1";
    string line2 = "  1   1   0   0   3   2   1   0   0";
    string line3 = "  3   0   1   0   5   1   1   1   0";
    string line4 = "  4   0   0   1   2   5   1   0   1";

    string endline = "\r\n";
    istringstream iss(line1 + endline + line2 + endline + line3 + endline + line4);

    for (int m = 0; m <= number_of_constraints; m++)
    {
        for (int n = 0; n <= number_of_variables; n++)
        {
            iss >> tableau[m][n];
        }
    }

    // Step 2: Initialize the set of basic columns
    vector<int> basic_columns;
    basic_columns.push_back(1);
    basic_columns.push_back(2);
    basic_columns.push_back(3);

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
        for (int i =0 ; i < number_of_constraints; i++)
        {
            cout << basic_columns[i] << "\t";
        }
        cout << endl;
        cout << endl;
    }

    return 0;
}