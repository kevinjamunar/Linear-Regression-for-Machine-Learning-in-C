#ifndef _learn_h  
#define _learn_h

//holds function declarations

void create_train_matrices(int train_houses, int train_atts, int test_houses, int test_atts);

void populate_train_matrices(int train_houses, int train_atts, FILE* fp_train);

void solve_XTX_inverse(int train_atts);

void calculate_weight(int train_houses, int train_atts);

void calculate_price(int train_houses, int train_atts, int test_houses, int test_atts, FILE* fp_test);

#endif
