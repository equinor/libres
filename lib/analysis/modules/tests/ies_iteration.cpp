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

  bool_vector_type * ens_mask = bool_vector_alloc(testdata.active_ens_size, true);
  bool_vector_type * obs_mask = bool_vector_alloc(testdata.active_obs_size, true);

  ies_enkf_config_set_truncation(ies_config, 0.95);
  ies_enkf_config_set_ies_steplength(ies_config, 0.50);
  ies_enkf_config_set_ies_inversion(ies_config, IES_INVERSION_SUBSPACE_EXACT_R);
  ies_enkf_config_set_ies_aaprojection(ies_config, false);


  for (int iter=0; iter < num_iter; iter++) {
    ies_enkf_init_update(ies_data,
                         ens_mask,
                         obs_mask,
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

    printf("iter: %d\n", iter);
    test_assert_int_equal( ies_enkf_data_get_iteration_nr(ies_data), iter + 1);
  }

  init_stdA(testdata, A2);
  matrix_pretty_fprint(A1, "   IES", " %6.3f", stdout);
  matrix_pretty_fprint(A2, "Std ES", " %6.3f", stdout);
  test_assert_true( matrix_similar(A1, A2, 1e-3));

  bool_vector_free(ens_mask);
  bool_vector_free(obs_mask);
  matrix_free(A1);
  matrix_free(A2);
  ies_enkf_data_free(ies_data);
  rng_free( rng );
}


int main(int argc, char ** argv) {
  res::es_testdata testdata(argv[1]);
  cmp_std_ies(testdata);
}
