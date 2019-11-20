/*
   Copyright (C) 2013  Equinor ASA, Norway.

   The file 'pca_plot_data.h'

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
#ifndef ERT_PCA_PLOT_DATA_H
#define ERT_PCA_PLOT_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ert/util/type_macros.h>
#include <ert/res_util/matrix.hpp>
#include <ert/util/double_vector.h>

#include <ert/enkf/pca_plot_vector.hpp>

  typedef struct pca_plot_data_struct pca_plot_data_type;

  pca_plot_data_type * pca_plot_data_alloc( const char * name , const matrix_type * PC, const matrix_type * PC_obs, const double_vector_type * singular_values);
  const double_vector_type * pca_plot_data_get_singular_values( const pca_plot_data_type * plot_data );
  void                 pca_plot_data_free( pca_plot_data_type * plot_data );
  const pca_plot_vector_type * pca_plot_data_iget_vector( const pca_plot_data_type * plot_data , int ivec);
  int                  pca_plot_data_get_size( const pca_plot_data_type * plot_data );
  const char         * pca_plot_data_get_name( const pca_plot_data_type * plot_data );
  int                  pca_plot_data_get_ens_size( const pca_plot_data_type * plot_data );

  UTIL_IS_INSTANCE_HEADER( pca_plot_data );

#ifdef __cplusplus
}
#endif
#endif
