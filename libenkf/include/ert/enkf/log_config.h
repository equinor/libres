/*
   Copyright (C) 2017  Statoil ASA, Norway.

   The file 'log_config.h' is part of ERT - Ensemble based Reservoir Tool.

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

#ifndef ERT_LOG_CONFIG_H
#define ERT_LOG_CONFIG_H

#include <ert/util/log.h>

typedef struct log_config_struct log_config_type;

log_config_type * log_config_alloc_load(const char *);
void              log_config_free(log_config_type *);

const char *             log_config_get_log_file(const log_config_type *);
const message_level_type log_config_get_log_level(const log_config_type *);

#endif
