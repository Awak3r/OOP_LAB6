#include "include/NPC.h"
#include "include/Observer.h"
#include "include/Visitor.h"

int main() {
    NPC_array npcs;
    CombatVisitor combat;
    combat.add_observer(std::make_unique<FileLogger>("log.txt"));
    combat.add_observer(std::make_unique<Display>());
    NPCFactory::load_from_file_c_style("npc.txt", npcs);
    std::cout << "=== До боя ===" << std::endl;
    npcs.print_all();
    std::cout << "Всего NPC: " << npcs.get_size() << "\n\n";
    std::cout << "=== Начинается бой (радиус 50) ===" << std::endl;
    combat.do_combat(npcs, 50.0);
    std::cout << "\n=== После боя ===" << std::endl;
    npcs.print_all();
    std::cout << "Осталось NPC: " << npcs.get_size() << std::endl;
    NPCFactory::save_to_file("npcs_after.txt", npcs);
    return 0;
}