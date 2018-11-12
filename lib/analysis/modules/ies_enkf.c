/*
   Copyright (C) 2011  Statoil ASA, Norway.

   The file 'ies_enkf.c' is part of ERT - Ensemble based Reservoir Tool.

   ERT is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ERT is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
   for more details.
*/


#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <time.h>

#include <ert/util/util.hpp>
#include <ert/util/type_macros.hpp>
#include <ert/util/rng.hpp>
#include <ert/util/bool_vector.hpp>

#include <ert/res_util/matrix.hpp>
#include <ert/res_util/matrix_blas.hpp>


#include <ert/analysis/analysis_module.hpp>
#include <ert/analysis/analysis_table.hpp>
#include <ert/analysis/enkf_linalg.hpp>
#include <ert/analysis/std_enkf.hpp>

#include <ies_enkf_config.h>

typedef struct ies_enkf_data_struct ies_enkf_data_type;

#define IES_ENKF_TYPE_ID 19640202

#define ENKF_SUBSPACE_DIMENSION_KEY      "ENKF_SUBSPACE_DIMENSION"
#define ENKF_TRUNCATION_KEY              "ENKF_TRUNCATION"
#define STEPLENGTH_KEY                   "STEPLENGTH"
#define GAUSS_NEWTON_CONV_KEY            "GAUSS_NEWTON_CONV"
#define ITER_KEY                         "ITER"

//#define DEFAULT_ANALYSIS_SCALE_DATA true


//**********************************************
// IES "object" data definition
//**********************************************
/*
  The configuration data used by the ies_enkf module is contained in a
  ies_enkf_data_struct instance. The data type used for the ies_enkf
  module is quite simple; with only a few scalar variables, but there
  are essentially no limits to what you can pack into such a datatype.

  All the functions in the module have a void pointer as the first
  argument, this will immediately be casted to a ies_enkf_data_type
  instance, to get some type safety the UTIL_TYPE_ID system should be
  used

  The data structure holding the data for your analysis module should
  be created and initialized by a constructor, which should be
  registered with the '.alloc' element of the analysis table; in the
  same manner the desctruction of this data should be handled by a
  destructor or free() function registered with the .freef field of
  the analysis table.
*/

struct ies_enkf_data_struct {
   UTIL_TYPE_ID_DECLARATION;
   int       iteration_nr;            // Keep track of the outer iteration loop
   double    truncation;              // Controlled by config key: ENKF_TRUNCATION_KEY
   int       subspace_dimension;      // Controlled by config key: ENKF_SUBSPACE_DIMENSION_KEY (-1: use Truncation instead)
   double    steplength;              // Step length in Gauss Newton iteration
   double    gauss_newton_conv;       // Gauss Newton convergence criteria
   int       max_gauss_newton_it;     // Maximum number of Gauss Newton iterations
   int nrobs;                         // Initial nrobs used for checks in subsequent calls
   int ens_size;                      // Initial ens_size used for checks in subsequent calls
   int state_size;                    // Initial state_size used for checks in subsequent calls
   matrix_type * W;                   // Coefficient matrix used to compute Omega = I + W (I -11'/N)/sqrt(N-1)
   matrix_type * A0;                  // Prior ensemble used in Ei=A0 Omega_i
   matrix_type * E;                   // Prior ensemble of measurement perturations (should be the same for all iterations)
   bool      converged;               // GN has converged
   ies_enkf_config_type * config;     // This I don't understand but I assume I include data from the ies_enkf_config_type defined in ies_enkf_config.c
};

static UTIL_SAFE_CAST_FUNCTION( ies_enkf_data , IES_ENKF_TYPE_ID )
static UTIL_SAFE_CAST_FUNCTION_CONST( ies_enkf_data , IES_ENKF_TYPE_ID )

void * ies_enkf_data_alloc( rng_type * rng) {
  ies_enkf_data_type * data = util_malloc( sizeof * data);
  UTIL_TYPE_ID_INIT( data , IES_ENKF_TYPE_ID );
  data->iteration_nr         = 0;
  data->truncation           = 0.0;
  data->subspace_dimension   = 0;
  data->steplength           = 0.0;
  data->gauss_newton_conv    = 0.0;
  data->max_gauss_newton_it  = 0;
  data->nrobs                = 0;
  data->ens_size             = 0;
  data->state_size           = 0;
  data->W                    = NULL;   // Coefficient matrix from previous iteration
  data->A0                   = NULL;   // Initial ensemble
  data->E                    = NULL;   // Initial ensemble of measurement perturbations
  data->converged            = false;
  data->config               = ies_enkf_config_alloc();
  return data;
}



// When is this called ?????
void ies_enkf_data_free( void * arg ) {
  ies_enkf_data_type * data = ies_enkf_data_safe_cast( arg );
  ies_enkf_config_free( data->config );
  free( data );
}



// Are these used???
//**********************************************
// Set / Get iteration number
//**********************************************
void ies_enkf_set_iteration_nr( ies_enkf_data_type * data , int iteration_nr) {
  data->iteration_nr = iteration_nr;
}

int ies_enkf_get_iteration_nr( const ies_enkf_data_type * data ) {
  return data->iteration_nr;
}


/*
 * -------------------------------------------------------------------------------------------------------------
 * I E n K S
 * IEnKS update (IES searching the solution in ensemble subspace)
 * -------------------------------------------------------------------------------------------------------------
*/
void ies_enkf_updateA( void * module_data,
                       matrix_type * A ,    // Updated ensemble A retured to ERT.
                       matrix_type * Y ,    // Ensemble of predicted measurements (S)
                       matrix_type * R ,    // Measurement error covariance matrix (not used)
                       matrix_type * dObs , // Actual observations (not used)
                       matrix_type * E ,    // Ensemble of observation perturbations
                       matrix_type * D ,    // (d+E-Y) Ensemble of perturbed observations - Y
                       const module_info_type * module_info,
                                             rng_type * rng) {

   ies_enkf_data_type * data = ies_enkf_data_safe_cast( module_data );

   int nrobs         = matrix_get_rows( Y );
   int ens_size      = matrix_get_columns( Y );
   int state_size    = matrix_get_rows( A );
   int nrmin         = util_int_min( ens_size , nrobs);
   double nsc        = 1.0/sqrt(ens_size - 1.0);
   int i;

//   clock_t t;
//   double cpu_time=0.0;
   double diffW=0.0;
   double diff1W=0.0;
   double diff2W=0.0;
   double costfp=0.0;
   double costfd=0.0;


   matrix_type * W   = matrix_alloc( ens_size , ens_size  );  // Coefficient matrix
   matrix_type * DW  = matrix_alloc( ens_size , ens_size  );  // Coefficient matrix W - data->W
   matrix_type * H   = matrix_alloc( nrobs    , ens_size  );  // Innovation vector "H= S*W+D-Y"
   matrix_type * YY  = matrix_alloc( nrobs    , ens_size );   // Matrix of predicted measurement anomalies "YY= (Y-mean(Yi))/sqrt(N-1)"
   matrix_type * S   = matrix_alloc( nrobs    , ens_size );   // Predicted ensemble anomalies scaled with inv(Omeaga)

   matrix_type * YT  = matrix_alloc( ens_size, nrobs     );   // YY^T used in linear solver
   matrix_type * ST  = matrix_alloc( ens_size, nrobs     );   // current S^T used in linear solver
   matrix_type * STO = matrix_alloc( ens_size, nrobs     );   // previous S^T used in linear solver
   matrix_type * SD  = matrix_alloc( ens_size, nrobs     );   // difference between ST and STO in linear solver
   matrix_type * X1  = matrix_alloc( nrobs   , nrmin     );   // Used in subspace inversion
   matrix_type * X3  = matrix_alloc( nrobs   , ens_size  );   // Used in subspace inversion
   matrix_type * X   = matrix_alloc( ens_size, ens_size  );   // Used for Omega and transform matrix

   double      * eig = (double*)util_calloc( nrmin , sizeof * eig);



   data->iteration_nr=data->iteration_nr+1 ;

   data->steplength           = ies_enkf_config_get_steplength( data->config );
   data->gauss_newton_conv    = ies_enkf_config_get_gauss_newton_conv( data->config );
   data->truncation           = ies_enkf_config_get_truncation( data->config );
   data->subspace_dimension   = ies_enkf_config_get_enkf_subspace_dimension( data->config );

   printf("\n\n\n***********************************************************************\n");
   fprintf(stdout,"IES Iteration %d \n", data->iteration_nr);
   fprintf(stdout,"----steplength= %f\n", data->steplength);
   fprintf(stdout,"----truncation= %f %d\n", data->truncation, data->subspace_dimension);
   fprintf(stdout,"----ens_size  = %d\n", ens_size);
   fprintf(stdout,"----nrobs     = %d\n", nrobs );
   fprintf(stdout,"----state_size= %d\n", state_size );

   fprintf(stdout,"Input matrices\n");
//   matrix_pretty_fprint_submat(R,"R","%11.5f",stdout,0,nrobs-1,0,nrobs-1) ;
//   matrix_matmul_with_transpose(R, E, E, false, true);
//   matrix_scale(R,nsc*nsc);
//   matrix_pretty_fprint_submat(R,"Re","%11.5f",stdout,0,nrobs-1,0,nrobs-1) ;
   matrix_pretty_fprint_submat(E,"E","%11.5f",stdout,0,2,0,7) ;
   matrix_pretty_fprint_submat(D,"D","%11.5f",stdout,0,2,0,7) ;
   matrix_pretty_fprint_submat(Y,"Y","%11.5f",stdout,0,2,0,7) ;
   matrix_pretty_fprint_submat(A,"A","%11.5f",stdout,0,2,0,7) ;



   if (data->nrobs == 0) data->nrobs=nrobs;
   if (data->ens_size == 0) data->ens_size=ens_size;
   if (data->state_size == 0) data->state_size=state_size;

   if (data->state_size != state_size){
      fprintf(stdout,"Big Trouble in IES: state_size has changed from initial iteration: %d to %d\n",data->state_size, state_size);
   }
   if (data->ens_size != ens_size){
      fprintf(stdout,"Big Trouble in IES: ens_size has changed from initial iteration: %d to %d\n",data->ens_size, ens_size);
   }
   if (data->nrobs != nrobs){
      fprintf(stdout,"Big Trouble in IES: nrobs size has changed from initial iteration: %d to %d\n",data->nrobs, nrobs);
   }

   if (!data->W){
      // We store W from previous iteration to use it in itreation scheme                     (Line 9)
      printf("Allocating data->W\n");
      data->W=matrix_alloc( ens_size , ens_size  );
      matrix_pretty_fprint_submat(data->W,"Ini data->W","%11.5f",stdout,0,4,0,7) ;
      matrix_set(data->W , 0.0) ;
      }
   if (!data->A0){
      // We store the initial ensemble to use it uin final update equation                    (Line 11)
      printf("Allocating and assigning data->A0 \n");
      data->A0=matrix_alloc( state_size , ens_size  );
      matrix_assign(data->A0,A);
      matrix_pretty_fprint_submat(data->A0,"Ini data->A0","%11.5f",stdout,0,4,0,7) ;
      }
   if (!data->E){
      // We store the initial ensemble perturbations for the HACK just below.
      printf("Allocating and assigning data->E \n");
      data->E=matrix_alloc( nrobs , ens_size  );
      matrix_assign(data->E,E);
      matrix_pretty_fprint_submat(data->E,"Ini data->E","%11.5f",stdout,0,4,0,7) ;
      printf("\n\n");
      }


/* -------------------------------------------------------------------------------------------------------------
*  The matrix D is supposed to be the initial perturbed observations minus g(E_i)
*  We subtract the current resampled measurement perturbations and add the intitial ones.
*
*  THIS IS A HACK.  The coorect solution would be that ERT sample the initial obs perturbations and sends the same
*                   matrix E into IES in each iteration. This crashes if the number of observations changes from
*                   the initial iteration.
*/
   matrix_inplace_sub(D,E);          // Subtract new measurement perturbations              D=D-E
   matrix_inplace_add(D,data->E);    // Add old measurement perturbations                   D=D-E+data->E
   matrix_assign(E,data->E);         // Working with initial measurement perts from now on. E=data->E
   matrix_pretty_fprint_submat(E,"data->E","%11.5f",stdout,0,2,0,7) ;
   matrix_scale(E,nsc);              // Scale E = E/sqrt(ens_size-1) for consistency with algorithm when used for Cdd in Line 9.


/* -------------------------------------------------------------------------------------------------------------
*  Subtract mean of predictions to generate predicted ensemble anomaly matrix                 (Line 5)
*   YY(1,:)=(Y(1,:) - sum(Y(1,1:ens_size))/real(ens_size)) / sqrt(ens_size-1)
   POSSIBLE TO USE and CHANGE Y? Then we do not need the YY matrix.
*/
   matrix_assign(YY,Y);              // YY=Y
   matrix_subtract_row_mean( YY );   // YY=YY*(I-(1/ens_size)*11)
   matrix_scale(YY,nsc);             // YY=YY / sqrt(ens_size-1)
   matrix_pretty_fprint_submat(YY,"YY","%11.5f",stdout,0,2,0,7) ;

/*
* TO DO (special case)
*    Computing the projection YY= YY * (Ai^+ * Ai) (only used when ndim<ens_size-1)
*            if (ndim < ens_size-1 .and. beta /= 0.0 .and. lcyyreg) then
*               print *,'Activating AAi projection for Y'
*               allocate (AAi(ens_size,ens_size))
*               do k=1,ndim
*                  Ai(k,:)=( Ei(k,:) - sum(Ei(k,1:ens_size))/real(ens_size) )/n1
*               enddo
*               call aaprojection(Ai,AAi,ndim,ens_size,1.0)
*               YY=matmul(YY,AAi)
*               deallocate(AAi)
*            endif
*/


/* -------------------------------------------------------------------------------------------------------------
*  X=Omega = I + W (I-11'/sqrt(ens_size)) /  from Eq. (36).                                   (Line 6)
*  When solving the system S = YY inv(Omega) we write
*    S Omega = YY   or rather    Omega^T S^T = YY^T
*  Here we compute the W (I-11'/N) / sqrt(N-1)  and transpose it (subtracting the mean over the columns and dividing by sqrt(N-1)).
*/

   matrix_pretty_fprint_submat(data->W,"data->W","%11.5f",stdout,0,2,0,7) ;

/* Constructing Omega for LU solver and solve for S */
   matrix_assign(X,data->W) ;       // X=data->W (from previous iteration used to solve for S)
   matrix_subtract_row_mean(X);     // X=X*(I-(1/N)*11')
   matrix_scale(X,nsc);             // X/sqrt(N-1)
   matrix_inplace_transpose(X);     // X=transpose(X)
   for (i = 0; i < ens_size; i++){  // X=X+I
      matrix_iadd(X,i,i,1.0);
   }
   matrix_transpose(YY,YT);         // RHS stored in YT
   matrix_dgesv(X,YT);              // Solve system and return S in YT                        (Line 7)
   matrix_transpose(YT,S);          // Copy solution to S

//   t = clock();
//   t = clock() - t;
//   cpu_time = ((double)t)/CLOCKS_PER_SEC; // in seconds
//   printf("---LU linear solver took %f seconds to execute \n", cpu_time);

/* -------------------------------------------------------------------------------------------------------------
*     Innovation  from Eq. (47)    H = S*W + D - Y                                            (Line 8)    */
   matrix_assign(H,D) ;                            // H=D=dobs + E - Y
   matrix_dgemm(H,S,data->W,false,false,1.0,1.0);  // H=S*W + H

   printf("\nSolved for S and H\n");
   matrix_pretty_fprint_submat(S,"S","%11.5f",stdout,0,2,0,7) ;
   matrix_pretty_fprint_submat(H,"H","%11.5f",stdout,0,2,0,7) ;


/* -------------------------------------------------------------------------------------------------------------
* Compute X1 and eig from subspace inversion scheme                                           (Line 9)    */
   enkf_linalg_lowrankE( S , E , X1 , eig , data->truncation , data->subspace_dimension);

/*  X3 = X1 * diag(eig) * X1' * H (Similar to Eq. 14.31, Evensen (2007)) */
   enkf_linalg_genX3(X3 , X1 , H , eig);

/* -------------------------------------------------------------------------------------------------------------
*  Update data->W = (1-steplength) * data->W +  steplength * S' * X3                          (Line 9)    */
   matrix_assign(W,data->W); // Store previous W for convergence test
   matrix_dgemm(data->W , S , X3 , true , false , data->steplength , 1.0-data->steplength);
   matrix_pretty_fprint_submat(data->W,"Current W","%11.5f",stdout,0,2,0,7) ;


/* -------------------------------------------------------------------------------------------------------------
*  Compute new ensemble solution for current iteration                                        (Line 10)
*     X= I + W/sqrt(N-1)
*     Ei=A0*X   */

   matrix_assign(X,data->W);
   matrix_scale(X,nsc);
   for (i = 0; i < ens_size; i++){
      matrix_iadd(X,i,i,1.0);
   }
   matrix_pretty_fprint_submat(X,"X","%11.5f",stdout,0,7,0,7) ;

/* -------------------------------------------------------------------------------------------------------------
*  Compute final update and store it in A                                                     (Line 11)   */
   matrix_pretty_fprint_submat(data->A0,"data->A0","%11.5f",stdout,0,2,0,7) ;
   matrix_pretty_fprint_submat(A,"A^f","%11.5f",stdout,0,2,0,7) ;
   matrix_matmul(A,data->A0,X);
   matrix_pretty_fprint_submat(A,"A^a","%11.5f",stdout,0,2,0,7) ;


/* -------------------------------------------------------------------------------------------------------------
*  Computes ||data->W - W|| and checks for convergence                                        (Line 12)   */
   matrix_sub(DW,data->W,W);
   diff1W = 0.0;
   diff2W = 0.0;
   fprintf(stdout,"Residuals per realization:\n");
   for (i = 0; i < ens_size; i++){
       diffW=matrix_column_column_dot_product(DW,i,DW,i)/ens_size;
       diff2W+=diffW;
       fprintf(stdout," %12.6f ",diffW);
       if ((i+1)%8 == 0) fprintf(stdout,"\n") ;
       if (diffW > diff1W) diff1W=diffW ;
   }
   diff2W=diff2W/ens_size;

//       J= sum_j  W_j'* W_j +  D_j ' * R^(-1) * D_j
   costfp=0.0;
   costfd=0.0;
   for (i = 0; i < ens_size; i++){
       costfp += matrix_column_column_dot_product(W,i,W,i);
       costfd += matrix_column_column_dot_product(D,i,D,i);
   }

   fprintf(stdout,"\nresidual: %d %f %f %f %f %f\n", data->iteration_nr-1, diff1W, diff2W, costfp/ens_size, costfd/ens_size, (costfp + costfd)/ens_size)  ;


   if (diff1W < data->gauss_newton_conv) {
      fprintf(stdout,"Exiting at iteration: %d -- diff1W= %f\n", data->iteration_nr, diff1W)  ;
      data->converged = true;
      matrix_free( data->A0 );
      matrix_free( data->W );
      matrix_free( data->E );
      data->converged            = true;
   }

   matrix_free( W );
   matrix_free( DW );
   matrix_free( H  );
   matrix_free( YY );
   matrix_free( S  );
   matrix_free( YT );
   matrix_free( ST );
   matrix_free( STO);
   matrix_free( SD );
   matrix_free( X1 );
   matrix_free( X3 );
   matrix_free( X  );
}





//**********************************************
// Set / Get basic types
//**********************************************
bool ies_enkf_set_int( void * arg , const char * var_name , int value) {
  ies_enkf_data_type * module_data = ies_enkf_data_safe_cast( arg );
  {
    bool name_recognized = true;

    if (strcmp( var_name , ENKF_SUBSPACE_DIMENSION_KEY) == 0)
      ies_enkf_config_set_enkf_subspace_dimension(module_data->config , value);
    else if (strcmp( var_name , ITER_KEY) == 0)
      ies_enkf_set_iteration_nr( module_data , value );
    else
      name_recognized = false;

    return name_recognized;
  }
}

int ies_enkf_get_int( const void * arg, const char * var_name) {
  const ies_enkf_data_type * module_data = ies_enkf_data_safe_cast_const( arg );
  {
    if (strcmp(var_name , ITER_KEY) == 0)
      return module_data->iteration_nr;
    else if (strcmp(var_name , ENKF_SUBSPACE_DIMENSION_KEY) == 0)
      return ies_enkf_config_get_enkf_subspace_dimension(module_data->config);
    else
      return -1;
  }
}

bool ies_enkf_set_double( void * arg , const char * var_name , double value) {
  ies_enkf_data_type * module_data = ies_enkf_data_safe_cast( arg );
  {
    bool name_recognized = true;

    if (strcmp( var_name , ENKF_TRUNCATION_KEY) == 0)
      ies_enkf_config_set_truncation( module_data->config , value );
    else if (strcmp( var_name , STEPLENGTH_KEY) == 0)
      ies_enkf_config_set_steplength( module_data->config , value );
    else if (strcmp( var_name , GAUSS_NEWTON_CONV_KEY) == 0)
      ies_enkf_config_set_gauss_newton_conv( module_data->config , value );
    else
      name_recognized = false;

    return name_recognized;
  }
}

double ies_enkf_get_double( const void * arg, const char * var_name) {
  const ies_enkf_data_type * module_data = ies_enkf_data_safe_cast_const( arg );
  {
    if (strcmp(var_name , ENKF_TRUNCATION_KEY) == 0)
      return ies_enkf_config_get_truncation( module_data->config );

    if (strcmp(var_name , STEPLENGTH_KEY) == 0)
      return ies_enkf_config_get_steplength(module_data->config);

    if (strcmp(var_name , GAUSS_NEWTON_CONV_KEY) == 0)
      return ies_enkf_config_get_gauss_newton_conv(module_data->config);

    return -1;
  }
}


long ies_enkf_get_options( void * arg , long flag ) {
  ies_enkf_data_type * module_data = ies_enkf_data_safe_cast( arg );
  {
    return ies_enkf_config_get_option_flags( module_data->config );
  }
}

bool ies_enkf_has_var( const void * arg, const char * var_name) {
  {
    if (strcmp(var_name , ITER_KEY) == 0)
      return true;
    else if (strcmp(var_name , STEPLENGTH_KEY) == 0)
      return true;
    else if (strcmp(var_name , GAUSS_NEWTON_CONV_KEY) == 0)
      return true;
    else if (strcmp(var_name , ENKF_TRUNCATION_KEY) == 0)
      return true;
    else if (strcmp(var_name , ENKF_SUBSPACE_DIMENSION_KEY) == 0)
      return true;
    else
      return false;
  }
}


//**********************************************
// Symbol table
//**********************************************
#ifdef INTERNAL_LINK
#define LINK_NAME IES_ENKF
#else
#define LINK_NAME EXTERNAL_MODULE_SYMBOL
#endif


analysis_table_type LINK_NAME = {
  .name            = "IES_ENKF",
  .initX           = NULL,
  .updateA         = ies_enkf_updateA ,
  .alloc           = ies_enkf_data_alloc,
  .freef           = ies_enkf_data_free,
  .set_int         = ies_enkf_set_int ,
  .set_double      = ies_enkf_set_double ,
  .get_options     = ies_enkf_get_options ,
  .complete_update = NULL,
  .has_var         = ies_enkf_has_var,
  .get_int         = ies_enkf_get_int,
  .get_double      = ies_enkf_get_double,
};

