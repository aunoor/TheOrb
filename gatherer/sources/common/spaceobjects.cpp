#include "spaceobjects.h"

#include <utility>

StarSystem::StarSystem() {
    m_id = 0;
    m_id64 = 0;
}

StarSystem::StarSystem(uint32_t id, uint64_t id64, std::string name, SStarCoords coords) : StarSystem() {
    m_id = id;
    m_id64 = id64;
    m_name = std::move(name);
    m_coords = coords;
}

uint32_t StarSystem::GetId() {
    return m_id;
}

uint64_t StarSystem::GetId64() {
    return m_id64;
}

std::string StarSystem::GetName() {
    return m_name;
}

SStarCoords StarSystem::GetCoords() {
    return m_coords;
}

