#ifndef IDBPROVIDER_H
#define IDBPROVIDER_H

#include "common/spaceobjects.h"
#include <string>

class IDBProvider {
public:
    virtual bool Init() = 0;
    virtual bool GetSystemByName(const std::string &name, StarSystem &starSystem) = 0;
    virtual bool UpdateSystem(StarSystem &system) = 0;
};


#endif //IDBPROVIDER_H
