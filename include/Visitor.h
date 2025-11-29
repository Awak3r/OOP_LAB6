#pragma once
#include <cstddef>
#include <cstring>
#include <list>
#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>
#include <cstdio>
#include <vector>
#include <cmath>
#include "NPC.h"
#include "Observer.h"

class NPCVisitor {
    public:
        virtual ~NPCVisitor() = default;
        virtual void visit_squirrel(std::list<std::string>& to_delete, 
                                std::unique_ptr<NPC>& attacker, 
                                std::unique_ptr<NPC>& target) {}
        virtual void visit_werewolf(std::list<std::string>& to_delete, 
                                std::unique_ptr<NPC>& attacker, 
                                std::unique_ptr<NPC>& target) {}
        virtual void visit_druid(std::list<std::string>& to_delete, 
                                std::unique_ptr<NPC>& attacker, 
                                std::unique_ptr<NPC>& target) {}
        void add_observer(std::unique_ptr<Observer>&& obs) {  
            observer_array.push_back(std::move(obs));
        }
        void notify(const std::string& event) {
            for (auto& obs : observer_array) {
                obs->update(event);
            }
        }
    private:
        std::list<std::unique_ptr<Observer>> observer_array;
};

class CombatVisitor : public NPCVisitor{
    public:
        std::string combat_event_string(std::unique_ptr<NPC>& npc, std::unique_ptr<NPC>& to_npc) {
            std::string event = "NPC " + to_npc->get_type() + " " + to_npc->get_name() + 
                            " убит. Убийца: " + npc->get_type() + " " + npc->get_name() + ".";
            return event;
        }
        void visit_squirrel(std::list<std::string>& to_delete, std::unique_ptr<NPC>& npc, std::unique_ptr<NPC>& to_npc) override{
            if (to_npc->get_type() == "druid" || to_npc->get_type() == "werewolf"){
                to_delete.push_back(to_npc->get_name());
                notify(combat_event_string(npc, to_npc));

            }
        }
        void visit_werewolf(std::list<std::string>& to_delete, std::unique_ptr<NPC>& npc, std::unique_ptr<NPC>& to_npc) override{
            if (to_npc->get_type() == "druid"){
                to_delete.push_back(to_npc->get_name());
                notify(combat_event_string(npc, to_npc));
            }
        }
        void visit_druid(std::list<std::string>& to_delete, std::unique_ptr<NPC>& npc, std::unique_ptr<NPC>& to_npc) override{}
        void do_combat(NPC_array& arr, double rad){
            std::list<std::string> to_delete;
            for (auto& npc : arr.get_npcs()){
                for (auto& to_npc : arr.get_npcs()){
                    if (npc.get() == to_npc.get()) {
                        continue;
                    }
                    if ((((npc->get_x_cord() - to_npc->get_x_cord()) * (npc->get_x_cord() - to_npc->get_x_cord())) +
                    ((npc->get_y_cord() - to_npc->get_y_cord()) * (npc->get_y_cord() - to_npc->get_y_cord()))) 
                    <= (rad * rad)){
                        if (npc->get_type() == "squirrel"){
                            visit_squirrel(to_delete, npc, to_npc);
                        }
                        if (npc->get_type() == "werewolf"){
                            visit_werewolf(to_delete, npc, to_npc);
                        }
                        if (npc->get_type() == "druid"){
                            visit_druid(to_delete, npc, to_npc);
                        }
                    }
                }
            }
            for(auto& cur: to_delete){
                arr.remove_npc(cur);
            }
        }
};

