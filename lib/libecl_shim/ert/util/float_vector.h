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

#ifndef ERT_float_VECTOR_H
#define ERT_float_VECTOR_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdbool.h>

#include <ert/util/type_macros.h>
#include <ert/util/perm_vector.h>

typedef struct float_vector_struct float_vector_type;
typedef float (float_ftype) (float);


  int                  float_vector_lookup_bin( const float_vector_type * limits , float value , int guess);
  int                  float_vector_lookup_bin__( const float_vector_type * limits , float value , int guess);
  void                 float_vector_inplace_div( float_vector_type * vector , const float_vector_type * inv_factor);
  void                 float_vector_inplace_mul( float_vector_type * vector , const float_vector_type * factor);
  void                 float_vector_inplace_add( float_vector_type * vector , const float_vector_type * delta);
  void                 float_vector_inplace_sub( float_vector_type * vector , const float_vector_type * delta);
  void                 float_vector_set_read_only( float_vector_type * vector , bool read_only);
  bool                 float_vector_get_read_only( const float_vector_type * vector );
  void                 float_vector_memcpy_data( float * target, const float_vector_type * src );
  void                 float_vector_memcpy_from_data( float_vector_type * target , const float * src , int src_size );
  void                 float_vector_memcpy(  float_vector_type * target , const float_vector_type * src);
  void                 float_vector_memcpy_data_block( float_vector_type * target , const float_vector_type * src , int target_offset , int src_offset , int len);
  bool                 float_vector_growable( const float_vector_type * vector);
  void                 float_vector_select_unique(float_vector_type * vector);
  float_vector_type * float_vector_alloc( int init_size , float );
  float_vector_type * float_vector_alloc_private_wrapper(int init_size, float default_value , float * data , int alloc_size);
  float_vector_type * float_vector_alloc_shared_wrapper(int init_size, float default_value , float * data , int alloc_size);
  float_vector_type * float_vector_alloc_strided_copy( const float_vector_type * src , int start , int stop , int stride );
  float_vector_type * float_vector_alloc_copy( const float_vector_type * src);
  void                 float_vector_imul(float_vector_type * vector, int index, float factor);
  void                 float_vector_scale(float_vector_type * vector, float factor);
  void                 float_vector_div(float_vector_type * vector, float divisor);
  float               float_vector_reverse_iget(const float_vector_type * vector , int index);
  float               float_vector_iget(const float_vector_type * , int);
  float               float_vector_safe_iget(const float_vector_type * , int);
  float               float_vector_get_min(const float_vector_type * vector);
  float               float_vector_get_max(const float_vector_type * vector);
  int                  float_vector_get_min_index(const float_vector_type * vector, bool reverse);
  int                  float_vector_get_max_index(const float_vector_type * vector, bool reverse);
  float               float_vector_iadd( float_vector_type * vector , int index , float delta);
  void                 float_vector_resize( float_vector_type * vector , int new_size , float default_value );
  void                 float_vector_iset(float_vector_type *       , int , float);
  void                 float_vector_iset_block(float_vector_type * vector , int index , int block_size , float value);
  void                 float_vector_idel_block( float_vector_type * vector , int index , int block_size);
  float               float_vector_idel( float_vector_type * vector , int index);
  float               float_vector_del_value( float_vector_type * vector , float del_value);
  void                 float_vector_insert( float_vector_type * vector , int index , float value);
  void                 float_vector_append(float_vector_type *     , float);
  void                 float_vector_free_container(float_vector_type * vector);
  void                 float_vector_free(float_vector_type *);
  void                 float_vector_free__(void *);
  void                 float_vector_free_data(float_vector_type *);
  void                 float_vector_reset(float_vector_type *);
  void                 float_vector_reset__(void * __vector);
  int                  float_vector_size(const float_vector_type * );
  void                 float_vector_lshift(float_vector_type * vector , int shift);
  void                 float_vector_rshift(float_vector_type * vector , int shift);
  float               float_vector_pop(float_vector_type * vector);
  float               float_vector_get_first(const float_vector_type * vector);
  float               float_vector_get_last(const float_vector_type * );
  float *             float_vector_get_ptr(const float_vector_type * );
  float *             float_vector_alloc_data_copy( const float_vector_type * vector );
  const float *       float_vector_get_const_ptr(const float_vector_type * );
  bool float_vector_init_linear(float_vector_type * vector , float start_value, float end_value, int num_values);
  void                 float_vector_init_range(float_vector_type * vector , float value1 , float value2 , float delta);
  void                 float_vector_set_many(float_vector_type *  , int  , const float *  , int );
  void                 float_vector_set_all(float_vector_type * vector , float value);
  void                 float_vector_append_many(float_vector_type * vector , const float * data , int length);
  void                 float_vector_append_vector(float_vector_type * vector , const float_vector_type * other);
  void                 float_vector_shrink(float_vector_type * );
  float               float_vector_sum(const float_vector_type * );
  float               float_vector_get_default(const float_vector_type * );
  void                 float_vector_set_default(float_vector_type * vector, float default_value);
  void                 float_vector_append_default(float_vector_type * vector , float default_value);
  void                 float_vector_iset_default(float_vector_type * vector , int index , float default_value);
  bool                 float_vector_is_sorted( const float_vector_type * vector , bool reverse);
  bool                 float_vector_contains(const float_vector_type * vector , float value);
  bool                 float_vector_contains_sorted(const float_vector_type * vector , float value);
  int                  float_vector_index(const float_vector_type * vector , float value);
  int                  float_vector_index_sorted(const float_vector_type * vector , float value);
  void                 float_vector_sort(float_vector_type * vector);
  void                 float_vector_rsort(float_vector_type * vector);
  void                 float_vector_permute(float_vector_type * vector , const perm_vector_type * perm);
  perm_vector_type *   float_vector_alloc_sort_perm(const float_vector_type * vector);
  perm_vector_type *   float_vector_alloc_rsort_perm(const float_vector_type * vector);
  void                 float_vector_fprintf(const float_vector_type * vector , FILE * stream , const char * name , const char * fmt);
  void                 float_vector_fwrite(const float_vector_type * vector , FILE * stream);
  float_vector_type * float_vector_fread_alloc( FILE * stream );
  void                 float_vector_fread( float_vector_type * vector , FILE * stream );
  void                 float_vector_fwrite_data( const float_vector_type * vector , FILE * stream );
  void                 float_vector_fread_data( float_vector_type * vector , int size, FILE * stream);
  bool                 float_vector_equal(const float_vector_type * vector1 , const float_vector_type * vector2);
  int                  float_vector_first_equal(const float_vector_type * vector1, const float_vector_type * vector2, int offset);
  int                  float_vector_first_not_equal(const float_vector_type * vector1, const float_vector_type * vector2, int offset);
  void                 float_vector_apply(float_vector_type * vector , float_ftype *func);
  int                  float_vector_count_equal( const float_vector_type * vector , float cmp_value);
  int                  float_vector_element_size( const float_vector_type * vector );
  void                 float_vector_range_fill(float_vector_type * vector , float limit1 , float delta , float limit2);
  void                 float_vector_shift(float_vector_type * vector, float delta);

  UTIL_SAFE_CAST_HEADER( float_vector );
  UTIL_IS_INSTANCE_HEADER( float_vector );

#ifdef __cplusplus
}
#endif
#endif
//




