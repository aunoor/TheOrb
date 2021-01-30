#include "memprovider.h"

bool MemProvider::Init() {
    return true;
}

bool MemProvider::GetSystemByName(const std::string &name, StarSystem &starSystem) {
    for (const auto& system : m_systems) {
        if (system.Name == name) {
            starSystem = system;
            return true;
        }
    }
    starSystem = StarSystem();
    return true;
}

bool MemProvider::UpdateSystem(StarSystem &system) {

    auto it = m_systems.begin();
    while (it != m_systems.end()) {
        if (it->Id64 == system.Id64) {
            *it = system;
            return true;
        }
        it++;
    }

    //m_systems.insert_after(m_systems., system);
    m_systems.push_back(system);
    return true;
}
