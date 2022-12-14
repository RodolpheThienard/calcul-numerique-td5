/**********************************************/
/* lib_poisson1D.c                            */
/* Numerical library developed to solve 1D    */ 
/* Poisson problem (Heat equation)            */
/**********************************************/
#include "lib_poisson1D.h"

//
void set_GB_operator_colMajor_poisson1D(f64* AB, i32 *lab, i32 *la, i32 *kv)
{
    // Init AB to 0
    memset(AB, 0, (*lab)*(*la));
    i32 i = 0;
    // Mid rows
    for(; i < *la * *lab; i += *lab){
        AB[ i + *kv + 0 ] = -1.0;
        AB[ i + *kv + 1 ] = 2.0;
        AB[ i + *kv + 2 ] = -1.0;
    }

    // Last row
    AB[1] = 0.0;
    AB[i - 1] = 0.0;
}

//
void set_GB_operator_colMajor_poisson1D_Id(f64* AB, i32 *lab, i32 *la, i32 *kv)
{
    u32 matrix_size = (*lab)*(*la);
    memset(AB, 0, matrix_size);

    for(i32 i = 0; i < matrix_size; i += *lab)
        AB[i + (*kv) + 1] = 1.0;
}

//
void set_dense_RHS_DBC_1D(f64* RHS, i32* la, f64* BC0, f64* BC1)
{
    memset(RHS, 0, *la);
    RHS[0] = *BC0;
    RHS[*la - 1] = *BC1;
}  

//
void set_analytical_solution_DBC_1D(f64* EX_SOL, f64* X, i32* la, f64* BC0, f64* BC1)
{
    for(i32 i = 0; i < *la; i++, EX_SOL++, X++){
        *EX_SOL = *BC0 + *X * (*BC1 - *BC0);
    }
}  

//
void set_grid_points_1D(f64* x, i32* la)
{
    i32 i;
    f64 h = 1.0 / (1.0*(*la +1.0));
    for(i = 0; i < (*la); i++)
        x[i] = (i+1)*h;
}


void write_GB_operator_rowMajor_poisson1D(f64* AB, i32* lab, i32* la, ascii* filename){
  FILE * file;
  i32 ii,jj;
  file = fopen(filename, "w");
  //Numbering from 1 to la
  if (file != NULL){
    for (ii=0;ii<(*lab);ii++){
      for (jj=0;jj<(*la);jj++){
	fprintf(file,"%lf\t",AB[ii*(*la)+jj]);
      }
      fprintf(file,"\n");
    }
    fclose(file);
  }
  else{
    perror(filename);
  }
}

void write_GB_operator_colMajor_poisson1D(f64* AB, i32* lab, i32* la, ascii* filename){
  FILE * file;
  i32 ii,jj;
  file = fopen(filename, "w");
  //Numbering from 1 to la
  if (file != NULL){
    for (ii=0;ii<(*la);ii++){
      for (jj=0;jj<(*lab);jj++){
	fprintf(file,"%lf\t",AB[ii*(*lab)+jj]);
      }
      fprintf(file,"\n");
    }
    fclose(file);
  }
  else{
    perror(filename);
  }
}

void write_GB2AIJ_operator_poisson1D(f64* AB, i32* la, ascii* filename){
  FILE * file;
  i32 jj;
  file = fopen(filename, "w");
  //Numbering from 1 to la
  if (file != NULL){
    for (jj=1;jj<(*la);jj++){
      fprintf(file,"%d\t%d\t%lf\n",jj,jj+1,AB[(*la)+jj]);
    }
    for (jj=0;jj<(*la);jj++){
      fprintf(file,"%d\t%d\t%lf\n",jj+1,jj+1,AB[2*(*la)+jj]);
    }
    for (jj=0;jj<(*la)-1;jj++){
      fprintf(file,"%d\t%d\t%lf\n",jj+2,jj+1,AB[3*(*la)+jj]);
    }
    fclose(file);
  }
  else{
    perror(filename);
  }
}

void write_vec(f64* vec, i32* la, ascii* filename){
  i32 jj;
  FILE * file;
  file = fopen(filename, "w");
  // Numbering from 1 to la
  if (file != NULL){
    for (jj=0;jj<(*la);jj++){
      fprintf(file,"%lf\n",vec[jj]);
    }
    fclose(file);
  }
  else{
    perror(filename);
  } 
}  

void write_xy(f64* vec, f64* x, i32* la, ascii* filename){
  i32 jj;
  FILE * file;
  file = fopen(filename, "w");
  // Numbering from 1 to la
  if (file != NULL){
    for (jj=0;jj<(*la);jj++){
      fprintf(file,"%lf\t%lf\n",x[jj],vec[jj]);
    }
    fclose(file);
  }
  else{
    perror(filename);
  } 
}  

i32 indexABCol(i32 i, i32 j, i32 *lab){
  return 0;
}
i32 dgbtrftridiag(i32 *la, i32*n, i32 *kl, i32 *ku, f64 *AB, i32 *lab, i32 *ipiv, i32 *info){
    for(u32 i = 1; i < *la; i++){

        u32 a_1 = (*lab) * (i-1) + 1;
        u32 a = (*lab) * (i) + 1;
        u32 b_1 = (*lab) * (i-1) + 2;
        u32 c_1 = (*lab) * (i);

        ipiv[i-1] = i;

        /* b(i-1) = b(i-1)/a(i-1) */
        AB[(*kl) + b_1] /= AB[(*kl) + a_1];

        /* a(i) = a(i) - b(i-1) / a(i-1) * c(i-1) */
        AB[(*kl) + a] -= AB[(*kl) + b_1] * AB[(*kl) + c_1];

    }
    
    return 0; // Value never used
}

void eig_poisson1D(double* eigval, int *la){
}

double eigmax_poisson1D(int *la){
  return 0;
}

double eigmin_poisson1D(int *la){
  return 0;
}

double richardson_alpha_opt(int *la){
  return 0;
}

void richardson_alpha(double *AB, double *RHS, double *X, double *alpha_rich, int *lab, int *la,int *ku, int*kl, double *tol, int *maxit, double *resvec, int *nbite){
  /*
    Algorithm richardson
    r??=b-Ax??
    while ||r^k+1|| > epsilon
      x^k+1 = x^k + M^-1(b-Ax^k)
  */
  
}

void extract_MB_jacobi_tridiag(double *AB, double *MB, int *lab, int *la,int *ku, int*kl, int *kv){

}

void extract_MB_gauss_seidel_tridiag(double *AB, double *MB, int *lab, int *la,int *ku, int*kl, int *kv){

}

void richardson_MB(double *AB, double *RHS, double *X, double *MB, int *lab, int *la,int *ku, int*kl, double *tol, int *maxit, double *resvec, int *nbite){

}

