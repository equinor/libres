#include <ert/util/test_util.hpp>
#include <ert/util/rng.h>

#include <ert/res_util/es_testdata.hpp>

#include <ert/analysis/std_enkf.hpp>
#include "ies_enkf_data.h"
#include "ies_enkf_config.h"
#include "ies_enkf.h"


void init_stdA(const res::es_testdata& testdata, matrix_type * A2) {
  rng_type * rng = rng_alloc( MZRAN, INIT_DEFAULT );
  std_enkf_data_type * std_data = static_cast<std_enkf_data_type*>(std_enkf_data_alloc());
  std_enkf_set_truncation(std_data, 0.95);
  matrix_type * X = matrix_alloc(testdata.active_ens_size, testdata.active_ens_size);

  std_enkf_initX(std_data,
                 X,
                 nullptr,
                 testdata.S,
                 testdata.R,
                 testdata.dObs,
                 testdata.E,
                 testdata.D,
                 rng);

  matrix_inplace_matmul(A2, X);

  std_enkf_data_free(std_data);
  rng_free(rng);
}


void cmp_std_ies(const res::es_testdata& testdata) {
  int num_iter = 100;
  rng_type * rng = rng_alloc( MZRAN, INIT_DEFAULT );
  matrix_type * A1 = testdata.alloc_state("prior");
  matrix_type * A2 = testdata.alloc_state("prior");

  ies_enkf_data_type * ies_data = static_cast<ies_enkf_data_type*>(ies_enkf_data_alloc(rng));
  ies_enkf_config_type * ies_config = ies_enkf_data_get_config(ies_data);

  ies_enkf_config_set_truncation(ies_config, 0.95);
  ies_enkf_config_set_ies_steplength(ies_config, 0.50);
  ies_enkf_config_set_ies_inversion(ies_config, IES_INVERSION_SUBSPACE_EXACT_R);
  ies_enkf_config_set_ies_aaprojection(ies_config, false);


  for (int iter=0; iter < num_iter; iter++) {
    ies_enkf_init_update(ies_data,
                         testdata.ens_mask,
                         testdata.obs_mask,
                         testdata.S,
                         testdata.R,
                         testdata.dObs,
                         testdata.E,
                         testdata.D,
                         rng);

    ies_enkf_updateA(ies_data,
                     A1,
                     testdata.S,
                     testdata.R,
                     testdata.dObs,
                     testdata.E,
                     testdata.D,
                     NULL,
                     rng);

    test_assert_int_equal( ies_enkf_data_get_iteration_nr(ies_data), iter + 1);
  }

  init_stdA(testdata, A2);
  test_assert_true( matrix_similar(A1, A2, 1e-1));

  matrix_free(A1);
  matrix_free(A2);
  ies_enkf_data_free(ies_data);
  rng_free( rng );
}


matrix_type * matrix_delete_column(const matrix_type * m1, int column) {
  matrix_type * m2 = matrix_alloc(matrix_get_rows(m1), matrix_get_columns(m1) - 1);
  if (column > 0)
    matrix_copy_block(m2, 0, 0,
                      matrix_get_rows(m2), column,
                      m1, 0, 0);

  if (column < (matrix_get_columns(m1) - 1))
    matrix_copy_block(m2, 0, column,
                      matrix_get_rows(m2), matrix_get_columns(m2) - column,
                      m1, 0, column + 1);

  return m2;
}


matrix_type * swap_matrix(matrix_type * old_matrix, matrix_type * new_matrix) {
  matrix_free( old_matrix );
  return new_matrix;
}

/*
  This test verifies that the update iteration do not crash hard when
  realizations and observations are deactived between iterations.
*/

void test_deactivate(const char * testdata_file) {
  res::es_testdata testdata(testdata_file);
  int num_iter = 10;
  rng_type * rng = rng_alloc( MZRAN, INIT_DEFAULT );

  ies_enkf_data_type * ies_data = static_cast<ies_enkf_data_type*>(ies_enkf_data_alloc(rng));
  ies_enkf_config_type * ies_config = ies_enkf_data_get_config(ies_data);

  matrix_type * A0 = testdata.alloc_state("prior");
  matrix_type * A = matrix_alloc_copy(A0);

  ies_enkf_config_set_truncation(ies_config, 0.95);
  ies_enkf_config_set_ies_steplength(ies_config, 0.50);
  ies_enkf_config_set_ies_inversion(ies_config, IES_INVERSION_SUBSPACE_EXACT_R);
  ies_enkf_config_set_ies_aaprojection(ies_config, false);


  for (int iter=0; iter < num_iter; iter++) {
    if (iter == 3) {
      int iens = testdata.active_ens_size / 2;
      testdata.deactivate_realization( iens );
      A = matrix_alloc( matrix_get_rows(A0), bool_vector_count_equal(testdata.ens_mask, true));
      int iens_active = 0;
      for (int iens=0; iens < matrix_get_columns(A0); iens++) {
        if (bool_vector_iget(testdata.ens_mask, iens)) {
          matrix_copy_column(A, A0, iens_active, iens);
          iens_active += 1;
        }
      }
    }

    if (iter == 7)
      testdata.deactivate_obs( testdata.active_obs_size / 2 );

    ies_enkf_init_update(ies_data,
                         testdata.ens_mask,
                         testdata.obs_mask,
                         testdata.S,
                         testdata.R,
                         testdata.dObs,
                         testdata.E,
                         testdata.D,
                         rng);

    ies_enkf_updateA(ies_data,
                     A,
                     testdata.S,
                     testdata.R,
                     testdata.dObs,
                     testdata.E,
                     testdata.D,
                     NULL,
                     rng);

  }

  matrix_free(A);
  matrix_free(A0);

  ies_enkf_data_free(ies_data);
  rng_free( rng );
}


int main(int argc, char ** argv) {
  res::es_testdata testdata(argv[1]);
  cmp_std_ies(testdata);
  test_deactivate(argv[1]);
}
