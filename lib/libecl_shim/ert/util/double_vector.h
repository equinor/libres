/*
   Copyright (C) 2011  Equinor ASA, Norway.

   The file 'vector_template.h' is part of ERT - Ensemble based Reservoir Tool.

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

#ifndef ERT_double_VECTOR_H
#define ERT_double_VECTOR_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdbool.h>

#include <ert/util/type_macros.h>
#include <ert/util/perm_vector.h>

typedef struct double_vector_struct double_vector_type;
typedef double (double_ftype) (double);


  int                  double_vector_lookup_bin( const double_vector_type * limits , double value , int guess);
  int                  double_vector_lookup_bin__( const double_vector_type * limits , double value , int guess);
  void                 double_vector_inplace_div( double_vector_type * vector , const double_vector_type * inv_factor);
  void                 double_vector_inplace_mul( double_vector_type * vector , const double_vector_type * factor);
  void                 double_vector_inplace_add( double_vector_type * vector , const double_vector_type * delta);
  void                 double_vector_inplace_sub( double_vector_type * vector , const double_vector_type * delta);
  void                 double_vector_set_read_only( double_vector_type * vector , bool read_only);
  bool                 double_vector_get_read_only( const double_vector_type * vector );
  void                 double_vector_memcpy_data( double * target, const double_vector_type * src );
  void                 double_vector_memcpy_from_data( double_vector_type * target , const double * src , int src_size );
  void                 double_vector_memcpy(  double_vector_type * target , const double_vector_type * src);
  void                 double_vector_memcpy_data_block( double_vector_type * target , const double_vector_type * src , int target_offset , int src_offset , int len);
  bool                 double_vector_growable( const double_vector_type * vector);
  void                 double_vector_select_unique(double_vector_type * vector);
  double_vector_type * double_vector_alloc( int init_size , double );
  double_vector_type * double_vector_alloc_private_wrapper(int init_size, double default_value , double * data , int alloc_size);
  double_vector_type * double_vector_alloc_shared_wrapper(int init_size, double default_value , double * data , int alloc_size);
  double_vector_type * double_vector_alloc_strided_copy( const double_vector_type * src , int start , int stop , int stride );
  double_vector_type * double_vector_alloc_copy( const double_vector_type * src);
  void                 double_vector_imul(double_vector_type * vector, int index, double factor);
  void                 double_vector_scale(double_vector_type * vector, double factor);
  void                 double_vector_div(double_vector_type * vector, double divisor);
  double               double_vector_reverse_iget(const double_vector_type * vector , int index);
  double               double_vector_iget(const double_vector_type * , int);
  double               double_vector_safe_iget(const double_vector_type * , int);
  double               double_vector_get_min(const double_vector_type * vector);
  double               double_vector_get_max(const double_vector_type * vector);
  int                  double_vector_get_min_index(const double_vector_type * vector, bool reverse);
  int                  double_vector_get_max_index(const double_vector_type * vector, bool reverse);
  double               double_vector_iadd( double_vector_type * vector , int index , double delta);
  void                 double_vector_resize( double_vector_type * vector , int new_size , double default_value );
  void                 double_vector_iset(double_vector_type *       , int , double);
  void                 double_vector_iset_block(double_vector_type * vector , int index , int block_size , double value);
  void                 double_vector_idel_block( double_vector_type * vector , int index , int block_size);
  double               double_vector_idel( double_vector_type * vector , int index);
  double               double_vector_del_value( double_vector_type * vector , double del_value);
  void                 double_vector_insert( double_vector_type * vector , int index , double value);
  void                 double_vector_append(double_vector_type *     , double);
  void                 double_vector_free_container(double_vector_type * vector);
  void                 double_vector_free(double_vector_type *);
  void                 double_vector_free__(void *);
  void                 double_vector_free_data(double_vector_type *);
  void                 double_vector_reset(double_vector_type *);
  void                 double_vector_reset__(void * __vector);
  int                  double_vector_size(const double_vector_type * );
  void                 double_vector_lshift(double_vector_type * vector , int shift);
  void                 double_vector_rshift(double_vector_type * vector , int shift);
  double               double_vector_pop(double_vector_type * vector);
  double               double_vector_get_first(const double_vector_type * vector);
  double               double_vector_get_last(const double_vector_type * );
  double *             double_vector_get_ptr(const double_vector_type * );
  double *             double_vector_alloc_data_copy( const double_vector_type * vector );
  const double *       double_vector_get_const_ptr(const double_vector_type * );
  bool double_vector_init_linear(double_vector_type * vector , double start_value, double end_value, int num_values);
  void                 double_vector_init_range(double_vector_type * vector , double value1 , double value2 , double delta);
  void                 double_vector_set_many(double_vector_type *  , int  , const double *  , int );
  void                 double_vector_set_all(double_vector_type * vector , double value);
  void                 double_vector_append_many(double_vector_type * vector , const double * data , int length);
  void                 double_vector_append_vector(double_vector_type * vector , const double_vector_type * other);
  void                 double_vector_shrink(double_vector_type * );
  double               double_vector_sum(const double_vector_type * );
  double               double_vector_get_default(const double_vector_type * );
  void                 double_vector_set_default(double_vector_type * vector, double default_value);
  void                 double_vector_append_default(double_vector_type * vector , double default_value);
  void                 double_vector_iset_default(double_vector_type * vector , int index , double default_value);
  bool                 double_vector_is_sorted( const double_vector_type * vector , bool reverse);
  bool                 double_vector_contains(const double_vector_type * vector , double value);
  bool                 double_vector_contains_sorted(const double_vector_type * vector , double value);
  int                  double_vector_index(const double_vector_type * vector , double value);
  int                  double_vector_index_sorted(const double_vector_type * vector , double value);
  void                 double_vector_sort(double_vector_type * vector);
  void                 double_vector_rsort(double_vector_type * vector);
  void                 double_vector_permute(double_vector_type * vector , const perm_vector_type * perm);
  perm_vector_type *   double_vector_alloc_sort_perm(const double_vector_type * vector);
  perm_vector_type *   double_vector_alloc_rsort_perm(const double_vector_type * vector);
  void                 double_vector_fprintf(const double_vector_type * vector , FILE * stream , const char * name , const char * fmt);
  void                 double_vector_fwrite(const double_vector_type * vector , FILE * stream);
  double_vector_type * double_vector_fread_alloc( FILE * stream );
  void                 double_vector_fread( double_vector_type * vector , FILE * stream );
  void                 double_vector_fwrite_data( const double_vector_type * vector , FILE * stream );
  void                 double_vector_fread_data( double_vector_type * vector , int size, FILE * stream);
  bool                 double_vector_equal(const double_vector_type * vector1 , const double_vector_type * vector2);
  int                  double_vector_first_equal(const double_vector_type * vector1, const double_vector_type * vector2, int offset);
  int                  double_vector_first_not_equal(const double_vector_type * vector1, const double_vector_type * vector2, int offset);
  void                 double_vector_apply(double_vector_type * vector , double_ftype *func);
  int                  double_vector_count_equal( const double_vector_type * vector , double cmp_value);
  int                  double_vector_element_size( const double_vector_type * vector );
  void                 double_vector_range_fill(double_vector_type * vector , double limit1 , double delta , double limit2);
  void                 double_vector_shift(double_vector_type * vector, double delta);

  UTIL_SAFE_CAST_HEADER( double_vector );
  UTIL_IS_INSTANCE_HEADER( double_vector );

#ifdef __cplusplus
}
#endif
#endif
//




