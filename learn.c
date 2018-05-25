#include <stdio.h>
#include <stdlib.h>
#include "learn.h"

double **X; //Train Matrix
double **XT; //Train Matrix transpose
double *Y; //Price Vector
double **XTX; //X * XT
double **XTX_inverse_XT; //(X * XT)^-1 * XT
double **temp_matrix; //used for multiplication
double *W; //used for weight
double **identity;
double ** temp2;
double ** test_matrix;
double * final_vector;

int main(int argc, char* argv[])
{

FILE* fp_train;
FILE* fp_test;

fp_train = fopen(argv[1], "r");
fp_test = fopen(argv[2], "r");

int train_atts;
int train_houses;

int test_houses;


fscanf(fp_train, "%d", &train_atts);

int test_atts = train_atts;

fscanf(fp_train, "%d", &train_houses);

fscanf(fp_test, "%d", &test_houses); //the atts are the same as the attributes of the train file


create_train_matrices(train_houses, train_atts, test_houses, test_atts); //create train matrices
populate_train_matrices(train_houses, train_atts, fp_train); //adds train data

int f;

//free temp_matrix
for (f=0; f<train_atts+1; f++)
{
	double *ptr = temp_matrix[f];
	free(ptr);
}

solve_XTX_inverse(train_atts);

//free XTX

for (f=0; f<train_houses; f++)
{
	double *ptr = XTX[f];
	free(ptr);
}

calculate_weight(train_houses, train_atts);

//free temp2
//free XT
//free XTX_inverse_XT
//free identity

for (f=0; f<train_atts+1; f++)
{
	double *ptr = temp2[f];	
	free(ptr);
	double *ptr2 = XT[f];
	free(ptr2);
	double *ptr3 = XTX_inverse_XT[f];
	free(ptr3);
}

for (f=0; f<train_houses; f++)
{
	double *ptr = identity[f];
	free(ptr);
}

calculate_price(train_houses, train_atts, test_houses, test_atts, fp_test);

//free test_matrix
//free W
//free Y
//free X
for (f=0; f<test_houses; f++)
{
	double *ptr = test_matrix[f];
	free(ptr);
}
for (f=0; f<train_houses; f++)
{
	double *ptr = X[f];
	free(ptr);
}
free(W);
free(Y);


int j;

for(j = 0; j < test_houses;  j++)
{
	printf("%0.0f\n",final_vector[j]);
}

free(final_vector);

fclose(fp_train);
fclose(fp_test);

return 0;
}

void create_train_matrices(int train_houses, int train_atts, int test_houses, int test_atts)
{
	train_atts = train_atts + 1; // for the 1's column
	test_atts = train_atts;
	

	//printf("the create matrices value of trian atts is : %d", train_atts);

 	X = (double**)malloc(sizeof(double*)*train_houses);
 	XT =(double**)malloc(sizeof(double*)*train_atts);
 	Y = (double*)malloc(sizeof(double)*train_houses);
 	XTX = (double**)malloc(sizeof(double*)*train_houses);
    XTX_inverse_XT = (double**)malloc(sizeof(double*)*train_atts);
	temp_matrix = (double**)malloc(sizeof(double*)*train_atts);
	W = (double*)malloc(sizeof(double)*train_houses);
	identity = (double**)malloc(sizeof(double*)*train_houses);
	test_matrix = (double**)malloc(sizeof(double*)*test_houses);
	final_vector = (double*)malloc(sizeof(double)*test_houses);

	temp2 = (double**)malloc(sizeof(double*)*train_atts);
 
	int i = 0;

	for(i = 0; i < train_houses; i++)
	{
	 	X[i] = (double*)malloc(sizeof(double)*train_atts); 
	 	XTX[i] = (double*)malloc(sizeof(double)*train_houses);
		identity[i] = (double*)malloc(sizeof(double)*train_houses);
 	}

	for(i = 0; i < train_atts; i++)
    {
    	XT[i] = (double*)malloc(sizeof(double)*train_houses);
	    XTX_inverse_XT[i] = (double*)malloc(sizeof(double)*train_houses);
		temp_matrix[i] = (double*)malloc(sizeof(double)*train_atts);
		temp2[i] = (double*)malloc(sizeof(double)*train_houses);
	}
	
	for(i = 0; i < test_houses; i++)
	{
		test_matrix[i] = (double*)malloc(sizeof(double)*test_atts);
	}

return;
}

void populate_train_matrices(int train_houses, int train_atts, FILE* fp_train)
{
	int i;
	int j;

   	train_atts = train_atts + 2; //for the 1's column & the Price vector

	for(i = 0; i < train_houses; i++)
	{
		for(j = 0; j < train_atts; j++)
		{
			if(j == 0)
			{
				//first column
				X[i][j] = 1; // Regular X
				XT[j][i] = 1; // X Transpose
			} 
            
			else if(j == train_atts - 1) //last column
			{
				fscanf(fp_train, "%lf", &Y[i]); //lf is used to read in doubles
			}
 			else
			{
				fscanf(fp_train, "%lf", &X[i][j]);
				fgetc(fp_train); //gets the commas
				//printf("the value of X[%d][%d] is : %lf\n",i,j,X[i][j]);
				XT[j][i] = X[i][j];
			}
		}
	}
	for(i = 0; i < train_atts-1; i++)
	{
		for(j = 0; j < train_atts-1; j++)
		{
			if(i == j)
			{
				identity[i][j] = 1;
			}
			else
			{
				identity[i][j] = 0;
			}
		}
	}

	// SOLVING FOR (X Transpose * X) 

	//j is already declared
	int k;
	int l;
	
		for(j = 0; j < train_atts-1; j++)   
		{
			for(k = 0; k < train_atts-1; k++) 
				//temp_matrix[j][k] = temp;
			{
				double temp = 0;

				for(l = 0; l < train_houses; l++)
				{
					//temp = temp + (X[j][l] * XT[l][k]);
					temp = temp + (XT[j][l] * X[l][k]);
				}
				temp_matrix[j][k] = temp;
				//printf("matrix[%d][%d] is %lf\n",j,k, temp);
			}  
		}
			int a;
			int b;
			//printf("The value of train_atts - 1 is: %d ", train_atts - 1);
			for(a = 0; a < train_atts-1; a++)   
			{
				for(b = 0; b < train_atts-1; b++)
				{
					XTX[a][b] = temp_matrix[a][b];
					//printf("final matrix is [%d][%d] is: %lf\n",a,b,XTX[a][b]);
				}
			} 	
return;
}

void solve_XTX_inverse (int train_atts)
{

double pivot_val;
double temp;

int i;
int j;
int k;
int l;

int pivot_checked = 0;

train_atts = train_atts + 1;

//printf("The value of train_atts is : %d\n", train_atts);

	for(i = 0; i < train_atts; i++) //row
	{
		for(j = 0; j < train_atts; j++) //column
		{
			if(i == j && pivot_checked == 0) //reached a pivot point
			{
				if(XTX[i][j] != 0)
				{	
					pivot_val = XTX[i][j];
					j = 0; //sets j back to the start of the row
					pivot_checked = 1;
				}
				else if(XTX[i][j] == 0) //NEW CODE HERE Checks if the diagonal has a zero in it.
				{
					int f;

					for(f=0; f<train_atts; f++)
					{
						if(XTX[f][j] != 0) //found the kind of non-zero col
						{
							int g;
							pivot_val = XTX[f][j];
							for(g = 0; g <train_atts; g++)
							{	
								temp = XTX[f][g]; //row with non-zero
								XTX[i][g] = (temp + XTX[i][g]);
							}
							pivot_checked = 1;
							break;
						}	
					} 
					j=0;
				}	 //END of NEW CODE
			}
			if(pivot_checked == 1)
			{ //here is where the pivot row is divided.
				XTX[i][j] = XTX[i][j] / pivot_val;
				//printf("the value of XTX[%d][%d] is: %lf\n", i,j,XTX[i][j]);
				identity[i][j] = identity[i][j] / pivot_val; 
			}
		}
		pivot_checked = 0;

		// Pivot row has been created at this point *******

		int temp_set = 0;

		for(k = 0; k < train_atts; k++) 
		{
			for(l = 0; l < train_atts; l++)
			{
				if(k != i && l == i && temp_set == 0) //gets the value to change the subsequent rows by (temp)
				{
					//printf("l : %d, i : %d, k : %d\n", l,i,k);
					temp = XTX[k][l];
					//printf("the value of temp is : %lf", temp);
					temp_set = 1; 
					l = 0; //resets to the ith row 0th column.
				}	
				if(k != i && temp_set == 1)
				{
					//printf("the value of -temp is: %lf\n", -temp);
					XTX[k][l] = (-temp * XTX[i][l]) + XTX[k][l];
					//printf("The value of XTX[%d][%d] is : %lf\n", k,l, XTX[k][l]);
					identity[k][l] = (-temp * identity[i][l]) + identity[k][l];
				}
			}
			temp_set = 0;
		}
	}
return;
}

// NB : AT this point the identity matrix holds the inverse 

void calculate_weight(int train_houses, int train_atts)
{

	train_atts = train_atts+1; //for the 1's column	

	int j;
	int k;
	int l;

//XTX is a square matrix
//XT may not be a square matrix

		for(j = 0; j < train_atts; j++)   
		{
			for(k = 0; k < train_houses; k++) 
				//temp_matrix[j][k] = temp;
			{
				double temp = 0;
				for(l = 0; l < train_atts; l++)
				{
					//identity holds the inverse because of rref
					temp = temp + (identity[j][l] * XT[l][k]);
				}
				temp2[j][k] = temp;
				//printf("matrix[%d][%d] is %lf\n",j,k, temp);
			}  
		}
			int a;
			int b;
			for(a = 0; a < train_atts; a++)   
			{
				for(b = 0; b < train_houses; b++)
				{
					XTX_inverse_XT[a][b] = temp2[a][b];
					//printf("final matrix hi my name is Kevin [%d][%d] is: %lf\n",a,b,XTX[a][b]);
				}
			} 	

		// MULTIPLYING BY Y
		for(j = 0; j < train_atts; j++)   
		{
			double temp = 0;
			for(k = 0; k < train_houses; k++) 
			{
				temp = temp + (XTX_inverse_XT[j][k] * Y[k]);
			}  	
			W[j] = temp;
		}
return;
}

void calculate_price(int train_houses, int train_atts, int test_houses, int test_atts, FILE* fp_test)
{
	//X * W = Y

	//test_matrix * W => Y

	//test_matrix = test_houses * train_atts
	//W = train_atts * 1

	//final_vector = (test_houses * train_atts ) * (train_atts * 1) 
	//populates the test matrix
	int i;
	int j;
	int k;

   	test_atts = test_atts + 1; //for the 1's column 

	for(i = 0; i < test_houses; i++)
	{
		for(j = 0; j < test_atts; j++)
		{
			if(j == 0)
			{
				//first column
				test_matrix[i][j] = 1; // Regular X
			} 
 			else
			{
				fscanf(fp_test, "%lf", &test_matrix[i][j]);
				fgetc(fp_test); 
				//gets the commas
				//printf("the value of X[%d][%d] is : %lf\n",i,j,X[i][j]);
			}
		}
	}

	//Finished populating the test matrix

	//Multiplying the matrix

		for(j = 0; j < test_houses; j++)   
		{
			double temp = 0;

			for(k = 0; k < test_atts; k++) 
			{
				temp = temp + (test_matrix[j][k] * W[k]);
			}  	
			final_vector[j] = temp;
		}

	return;

}

