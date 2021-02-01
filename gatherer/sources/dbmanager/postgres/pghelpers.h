#ifndef PGHELPERS_H
#define PGHELPERS_H

#include "common/spaceobjects.h"
#include <libpq-fe.h>

class PgRecord;
StarSystem pgResult2StarSystem(PgRecord *pgRecord);

#endif //PGHELPERS_H
