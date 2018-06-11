#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <iostream>
#include <SDL/SDL_thread.h>
#include <vector>
using namespace std;

//product matrix rows and columns
const int Nrows = 4;
const int Ncols = 4;

vector <double> row[Nrows];
vector <double> col[Ncols];
double C[Nrows][Ncols];


//This function is a thread entry point 

int dotProduct (void *data )
{
int i, j;
sscanf ( (char *) data, "%d %d", &i, &j );
int len, na, nb;
na = row[i].size();
nb = col[j].size();
if (na > nb) //take the shorter one
        len = nb;
else
        len = na;

double sum = 0.0;
for (int k = 0; k < len; k++ )
        sum += row[i][k] * col[j][k];
C[i][j] = sum;

return 0;
}

void printMat ( double *m, int nRows, int nCols )
{

for (int i = 0; i < nRows; i++ ) {
printf("\n");
for (int j = 0; j < nCols; j++ )
        printf("%3.2f\t", *(m+i*nCols+j) );
}
}
int main()
{
SDL_Thread *sumThread;
//m1 is 4X3 and m2 is 3X4; product is 4X4
double m1[][3]={{0.866, -0.5,0}, {0.5,0.866,0},{0,0,1}, {0,0,1}};
double m2[][4]={{0.5,-0.866,0,0}, {0.866,0.5,0,0},{0,0,1,1}};

for (int i = 0; i < Nrows; i++ )
for (int j = 0; j < 3; j++ )
row [i].push_back ( m1[i][j] );

for (int j = 0; j < Ncols; j++ )
        for (int i = 0; i < 3; i++ )
                col[j].push_back (m2[i][j] );

char buffer[100];
for (int i = 0; i < Nrows; i++ ) {
for (int j = 0; j < Ncols; j++ ) {
sprintf(buffer, "%d %d", i, j );
sumThread = SDL_CreateThread(dotProduct, (void *)buffer);

if (sumThread == NULL ) {
cout << "\nFailed: " << SDL_GetError() << endl;



}else {
//Wait for the thread to complete
SDL_WaitThread(sumThread, NULL );
}

}

}

printMat ( &m1[0][0], Nrows, 3);
printf("\n-----------------------");
printMat ( &m2[0][0], 3, Ncols );
printf("\n-----------------------");
printMat(&C[0][0], Nrows, Ncols );

return 0;


}
