/*
   Copyright (C) 2017  Statoil ASA, Norway. 
    
   The file 'environment_varlist.h' is part of ERT - Ensemble based Reservoir Tool. 
    
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

#ifndef ENVIRONMENT_VARLIST_H
#define ENVIRONMENT_VARLIST_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct env_varlist_struct env_varlist_type;

env_varlist_type * env_varlist_alloc();

void               env_varlist_add(env_varlist_type * list, char * var_name, char * var_value);
int                env_varlist_get_size(env_varlist_type * list);
char            ** env_varlist_alloc_varlist(env_varlist_type * list);
char             * env_varlist_get_value(env_varlist_type * list, char * var);

void               env_varlist_free(env_varlist_type * list);

#ifdef __cplusplus
}
#endif
#endif
