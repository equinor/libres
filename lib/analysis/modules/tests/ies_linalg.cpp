#include <ert/util/test_util.hpp>
#include <ert/util/rng.h>

#include <ert/res_util/es_testdata.hpp>

#include "ies_enkf_data.h"
#include "ies_enkf_config.h"
#include "ies_enkf.h"



void update_exact_scheme_subspace_no_truncation_diagR(const res::es_testdata& testdata, const bool_vector_type * ens_mask, const bool_vector_type * obs_mask, ies_enkf_data_type* ies_data, matrix_type * A, rng_type * rng) {
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
                   A,
                   testdata.S,
                   testdata.R,
                   testdata.dObs,
                   testdata.E,
                   testdata.D,
                   NULL,
                   rng);
}


/*
TEST 1 (Consistency between exact scheme and subspace scheme with no truncation and exact diagonal R):
 - ANALYSIS_SET_VAR IES_ENKF ENKF_TRUNCATION         1.0
 - ANALYSIS_SET_VAR IES_ENKF IES_STEPLENGTH          0.6
 - ANALYSIS_SET_VAR IES_ENKF IES_INVERSION           1
 - ANALYSIS_SELECT IES_ENKF
should give same result as:
 - ANALYSIS_SET_VAR IES_ENKF IES_STEPLENGTH          0.6
 - ANALYSIS_SET_VAR IES_ENKF IES_INVERSION           0
 - ANALYSIS_SELECT IES_ENKF

*/

void test_consistency_exact_scheme_subspace_no_truncation_diagR(const res::es_testdata& testdata) {
  rng_type * rng = rng_alloc( MZRAN, INIT_DEFAULT );
  matrix_type * A1 = testdata.alloc_state("prior");
  matrix_type * A2 = testdata.alloc_state("prior");

  ies_enkf_data_type * ies_data1 = static_cast<ies_enkf_data_type*>(ies_enkf_data_alloc(rng));
  ies_enkf_config_type * ies_config1 = ies_enkf_data_get_config(ies_data1);

  ies_enkf_data_type * ies_data2 = static_cast<ies_enkf_data_type*>(ies_enkf_data_alloc(rng));
  ies_enkf_config_type * ies_config2 = ies_enkf_data_get_config(ies_data2);

  bool_vector_type * ens_mask = bool_vector_alloc(testdata.active_ens_size, true);
  bool_vector_type * obs_mask = bool_vector_alloc(testdata.active_obs_size, true);

  ies_enkf_config_set_truncation(ies_config1, 1.0);
  ies_enkf_config_set_ies_steplength(ies_config1, 0.6);
  ies_enkf_config_set_ies_inversion(ies_config1, IES_INVERSION_SUBSPACE_EXACT_R);

  ies_enkf_config_set_ies_steplength(ies_config2, 0.6);
  ies_enkf_config_set_ies_inversion(ies_config2, IES_INVERSION_EXACT);

  update_exact_scheme_subspace_no_truncation_diagR(testdata, ens_mask, obs_mask, ies_data1, A1, rng);
  update_exact_scheme_subspace_no_truncation_diagR(testdata, ens_mask, obs_mask, ies_data2, A2, rng);
  test_assert_true( matrix_similar(A1, A2, 5e-6));

  bool_vector_free(ens_mask);
  bool_vector_free(obs_mask);
  matrix_free(A1);
  matrix_free(A2);
  ies_enkf_data_free(ies_data1);
  ies_enkf_data_free(ies_data2);
  rng_free( rng );
}

/*
TEST 2 (consistency between subspace inversion schemes with lowrank R):
 - ANALYSIS_SET_VAR IES_ENKF ENKF_TRUNCATION         0.999
 - ANALYSIS_SET_VAR IES_ENKF IES_STEPLENGTH          0.6
 - ANALYSIS_SET_VAR IES_ENKF IES_INVERSION           2
 - ANALYSIS_SELECT IES_ENKF
should give same result as
 - ANALYSIS_SET_VAR IES_ENKF ENKF_TRUNCATION         0.999
 - ANALYSIS_SET_VAR IES_ENKF IES_STEPLENGTH          0.6
 - ANALYSIS_SET_VAR IES_ENKF IES_INVERSION           3
 - ANALYSIS_SELECT IES_ENKF
*/

void test_consistency_scheme_inversions(const res::es_testdata& testdata) {
  rng_type * rng = rng_alloc( MZRAN, INIT_DEFAULT );
  matrix_type * A1 = testdata.alloc_state("prior");
  matrix_type * A2 = testdata.alloc_state("prior");

  ies_enkf_data_type * ies_data1 = static_cast<ies_enkf_data_type*>(ies_enkf_data_alloc(rng));
  ies_enkf_config_type * ies_config1 = ies_enkf_data_get_config(ies_data1);

  ies_enkf_data_type * ies_data2 = static_cast<ies_enkf_data_type*>(ies_enkf_data_alloc(rng));
  ies_enkf_config_type * ies_config2 = ies_enkf_data_get_config(ies_data2);

  bool_vector_type * ens_mask = bool_vector_alloc(testdata.active_ens_size, true);
  bool_vector_type * obs_mask = bool_vector_alloc(testdata.active_obs_size, true);

  ies_enkf_config_set_truncation(ies_config1, 0.95);
  ies_enkf_config_set_ies_steplength(ies_config1, 0.6);
  ies_enkf_config_set_ies_inversion(ies_config1, IES_INVERSION_SUBSPACE_EE_R);

  ies_enkf_config_set_truncation(ies_config2, 0.95);
  ies_enkf_config_set_ies_steplength(ies_config2, 0.6);
  ies_enkf_config_set_ies_inversion(ies_config2, IES_INVERSION_SUBSPACE_RE);

  update_exact_scheme_subspace_no_truncation_diagR(testdata, ens_mask, obs_mask, ies_data1, A1, rng);
  update_exact_scheme_subspace_no_truncation_diagR(testdata, ens_mask, obs_mask, ies_data2, A2, rng);
  test_assert_true( matrix_similar(A1, A2, 5e-6));

  bool_vector_free(ens_mask);
  bool_vector_free(obs_mask);
  matrix_free(A1);
  matrix_free(A2);
  ies_enkf_data_free(ies_data1);
  ies_enkf_data_free(ies_data2);
  rng_free( rng );
}


int main(int argc, char ** argv) {
  res::es_testdata testdata(argv[1]);
  test_consistency_exact_scheme_subspace_no_truncation_diagR(testdata);
  test_consistency_scheme_inversions(testdata);
}
