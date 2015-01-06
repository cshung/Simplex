#include "stdafx.h"
#pragma warning(disable : 4996)

#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

// Simplex lab with C++
int main()
{
    int number_of_constraints = 3;
    int number_of_variables = 5;
    vector<vector<double>> tableau;
    tableau.resize(number_of_constraints + 1);
    for (int m = 0; m < number_of_constraints + 1; m++)
    {
        tableau[m].resize(number_of_variables + 1);
    }

    // Step 1: Initialize tableau with sample data from book 
    tableau[0][0] = -6;   tableau[0][1] = -3;    tableau[0][2] = -3;    tableau[0][3] = 0;    tableau[0][4] = 0;    tableau[0][5] = 0;
    tableau[1][0] = 1;    tableau[1][1] = 3;     tableau[1][2] = 2;     tableau[1][3] = 1;    tableau[1][4] = 0;    tableau[1][5] = 0;
    tableau[2][0] = 2;    tableau[2][1] = 2;     tableau[2][2] = -1;    tableau[2][3] = 0;    tableau[2][4] = 1;    tableau[2][5] = 0;
    tableau[3][0] = 3;    tableau[3][1] = -1;    tableau[3][2] = 3;     tableau[3][3] = 0;    tableau[3][4] = 0;    tableau[3][5] = 1;

    // Step 2: Initialize the set of basic columns
    vector<int> basic_columns;
    basic_columns.push_back(3);
    basic_columns.push_back(4);
    basic_columns.push_back(5);

    // Step 3: Initialize the column_to_enter_basis;
    int column_to_enter_basis = 2;

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

    basic_columns[row] = column_to_enter_basis;

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

    return 0;
}