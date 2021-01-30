#ifndef PGHELPERS_H
#define PGHELPERS_H

#include "common/spaceobjects.h"
#include <libpq-fe.h>

StarSystem pgResult2StarSystem(PGresult *pgResult, int row);
bool checkSelectPgResult(PGresult *pgResult, std::string *error);

#endif //PGHELPERS_H
