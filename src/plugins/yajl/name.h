#ifndef ELEKTRA_PLUGIN_YAJL_NAME_H
#define ELEKTRA_PLUGIN_YAJL_NAME_H

#include "kdb.h"

ssize_t elektraKeyCountLevel(const Key *cur);
ssize_t elektraKeyCountEqualLevel(const Key *cmp1, const Key *cmp2);

#endif
