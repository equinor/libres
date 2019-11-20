/*
   Copyright (C) 2011  Equinor ASA, Norway.

   The file 'sched_time.c' is part of ERT - Ensemble based Reservoir Tool.

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

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <ert/util/util.hpp>
#include <ert/util/type_macros.hpp>

#include <ert/sched/sched_types.hpp>
#include <ert/sched/sched_time.hpp>

/*****************************************************************/

/**
   The sched_time_type is an attempt at unifiing the DATES and TSTEP
   keywords; they are both related to stepping forward in time. For a
   reasonable control over the timestepping we need (at least) two
   pieces of information:

     1. What is true time at the start/end of the current step.
     2. How long is the step.

   The DATES keywords give the true time at the end of the step,
   whereas the TSTEP keyword gives the length of the step.
*/

#define SCHED_TIME_TYPE_ID 66195407

struct sched_time_struct {
  UTIL_TYPE_ID_DECLARATION;
  time_t            date;
  double            tstep_length;    /* Length of TSTEP - in days. */
  sched_time_enum   time_type;
};




sched_time_type * sched_time_alloc( time_t date , double tstep_length , sched_time_enum  time_type ) {
  sched_time_type * time_node = (sched_time_type*)util_malloc( sizeof * time_node );
  UTIL_TYPE_ID_INIT( time_node , SCHED_TIME_TYPE_ID );
  time_node->time_type    = time_type;
  time_node->tstep_length = tstep_length;
  time_node->date         = date;

  return time_node;
}

static UTIL_SAFE_CAST_FUNCTION( sched_time , SCHED_TIME_TYPE_ID )

void sched_time_free( sched_time_type * time_node ) {
  free( time_node );
}

void sched_time_free__( void * arg ) {
  sched_time_free( sched_time_safe_cast( arg ));
}


time_t sched_time_get_date( const sched_time_type * time_node ) {
  return time_node->date;
}


time_t sched_time_get_type( const sched_time_type * time_node ) {
  return time_node->time_type;
}
