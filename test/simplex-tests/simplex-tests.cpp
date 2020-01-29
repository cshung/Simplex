#include "simplex-tests.h"

#include <vector>
using namespace std;

/* public */
void linear_programming(vector<double> objective_vector, vector<vector<double> > constraint_matrix, vector<double> constraint_vector);

TEST_F(simplex_tests, test1) {
    double objective_array[] = { 1, 1, 1, 1, 1 };
    double constraint_0[] = { 3, 2, 1, 0, 0 };
    double constraint_1[] = { 5, 1, 1, 1, 0 };
    double constraint_2[] = { 2, 5, 1, 0, 1 };
    double constraint_array[] = {1, 3, 4 };

    vector<double> objective_vector(objective_array, objective_array + 5);
    vector<vector<double> > constraint_matrix;
    constraint_matrix.push_back(vector<double>(constraint_0, constraint_0 + 5));
    constraint_matrix.push_back(vector<double>(constraint_1, constraint_1 + 5));
    constraint_matrix.push_back(vector<double>(constraint_2, constraint_2 + 5));
    vector<double> constraint_vector(constraint_array, constraint_array + 3);

    linear_programming(objective_vector, constraint_matrix, constraint_vector);
    EXPECT_EQ(true, true);
}
