#include "memprovider.h"

bool MemProvider::Init() {
    return true;
}

StarSystem MemProvider::GetSystemByName(const std::string &name) {
    for (const auto& system : m_systems) {
        if (system.Name == name) {
            return system;
        }
    }
    return StarSystem();
}

bool MemProvider::UpdateSystem(StarSystem &system) {

    auto it = m_systems.begin();
    while (it != m_systems.end()) {
        if (it->Id64 == system.Id64) {
            *it = system;
        }
        return true;
    }

    //m_systems.insert_after(m_systems., system);
    m_systems.push_front(system);
    return true;
}
