#ifndef SPACEOBJECTS_H
#define SPACEOBJECTS_H

#include <string>

struct SStarCoords{
    float x;
    float y;
    float z;
};

class StarSystem {
public:
    StarSystem();
    StarSystem(uint32_t id, uint64_t id64, std::string name, SStarCoords coords);

    uint32_t GetId();
    uint64_t GetId64();
    std::string GetName();
    SStarCoords GetCoords();

private:
    uint32_t m_id;
    uint64_t m_id64;
    std::string m_name;
    SStarCoords m_coords{};
};

#endif //SPACEOBJECTS_H
