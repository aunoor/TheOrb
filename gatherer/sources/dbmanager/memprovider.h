#ifndef MEMPROVIDER_H
#define MEMPROVIDER_H

#include "idbprovider.h"

#include <forward_list>

class MemProvider : public IDBProvider {
public:
    MemProvider() = default;

    bool Init() override;

    StarSystem GetSystemByName(const std::string &name) override;

    bool UpdateSystem(StarSystem &system) override;

private:
    std::list<StarSystem> m_systems;
};


#endif //MEMPROVIDER_H
