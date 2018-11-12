/*
   Copyright (C) 2015  Statoil ASA, Norway.

   The file 'ies_enkf_config.h' is part of ERT - Ensemble based Reservoir Tool.

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

#ifndef IES_ENKF_CONFIG_H
#define IES_ENKF_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct ies_enkf_config_struct ies_enkf_config_type;


  ies_enkf_config_type * ies_enkf_config_alloc();
  void   ies_enkf_config_free(ies_enkf_config_type * config);

  int    ies_enkf_config_get_enkf_subspace_dimension( ies_enkf_config_type * config );
  void   ies_enkf_config_set_enkf_subspace_dimension( ies_enkf_config_type * config , int subspace_dimension);

  double ies_enkf_config_get_truncation( ies_enkf_config_type * config );
  void   ies_enkf_config_set_truncation( ies_enkf_config_type * config , double truncation);

  void ies_enkf_config_set_option_flags( ies_enkf_config_type * config , long flags);
  long ies_enkf_config_get_option_flags( const ies_enkf_config_type * config );

  double ies_enkf_config_get_steplength( ies_enkf_config_type * config );
  void   ies_enkf_config_set_steplength( ies_enkf_config_type * config , double steplength);

  double ies_enkf_config_get_gauss_newton_conv( ies_enkf_config_type * config );
  void   ies_enkf_config_set_gauss_newton_conv( ies_enkf_config_type * config , double gauss_newton_conv);

  int    ies_enkf_config_get_max_gauss_newton_it( ies_enkf_config_type * config );
  void   ies_enkf_config_set_max_gauss_newton_it( ies_enkf_config_type * config , int max_gauss_newton_it);



#ifdef __cplusplus
}
#endif
#endif
