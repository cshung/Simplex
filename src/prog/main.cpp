#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cassert>
#include <cmath>
using namespace std;

/* public */
void linear_programming(vector<double> objective_vector, vector<vector<double> > constraint_matrix, vector<double> constraint_vector);

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
    vector<vector<double> > constraint_matrix;
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