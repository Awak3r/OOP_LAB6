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

#define MAX_LENGTH 256

class NPC {
    public:
        NPC() : x_cord(0), y_cord(0) {}
        NPC(const std::string& nam, double x, double y): name(nam), x_cord(x), y_cord(y) {}
        NPC(const NPC& other) : name(other.name), x_cord(other.x_cord), y_cord(other.y_cord) {}
        NPC& operator=(const NPC& other) {
            if (this != &other) {
                name = other.name;
                x_cord = other.x_cord;
                y_cord = other.y_cord;
            }
            return *this;
        }
        bool operator==(const NPC& other) const {
            return x_cord == other.x_cord && y_cord == other.y_cord && name == other.name;
        }
        void set_x(double x) { x_cord = x; }
        void set_y(double y) { y_cord = y; }
        void set_name(const std::string& nam) { name = nam; }
        double get_x_cord() const { return x_cord; }
        double get_y_cord() const { return y_cord; }
        std::string get_name() const { return name; }
        virtual std::string get_type() const { return "NPC"; }
        virtual ~NPC() noexcept = default;
        
    private:
        double x_cord;
        double y_cord;
        std::string name;
};

class NPC_array {
    public:
        size_t get_size() const { return array.size(); }
        void add_NPC(std::unique_ptr<NPC>&& npc) {
            array.push_back(std::move(npc));
        }
        void remove_at(double x, double y) {
            array.remove_if([x, y](const std::unique_ptr<NPC>& npc) {
                return npc->get_x_cord() == x && npc->get_y_cord() == y;
            });
        }
        void remove_npc(const std::string& name) {
            array.remove_if([&name](const std::unique_ptr<NPC>& npc) {
                return npc->get_name() == name;
            });
        }
        std::list<std::unique_ptr<NPC>>& get_npcs() {
            return array;
        }
        const std::list<std::unique_ptr<NPC>>& get_npcs() const {
            return array;
        }
        void print_all() const {
            for (const auto& npc : array) {
                std::cout << npc->get_type() << " " << npc->get_name() << " " 
                        << npc->get_x_cord() << " " << npc->get_y_cord() << "\n";
            }
        }
        void clear() {
            array.clear();
        }
    private:
        std::list<std::unique_ptr<NPC>> array;
};

class squirrel: public NPC {
    public:
        squirrel() : NPC() {}
        squirrel(const std::string& nam, double x, double y) : NPC(nam, x, y) {}
        std::string get_type() const override { return "squirrel"; }
};

class werewolf: public NPC {
    public:
        werewolf() : NPC() {}
        werewolf(const std::string& nam, double x, double y) : NPC(nam, x, y) {}
        std::string get_type() const override { return "werewolf"; }
};

class druid: public NPC {
    public:
        druid() : NPC() {}
        druid(const std::string& nam, double x, double y) : NPC(nam, x, y) {}
        std::string get_type() const override { return "druid"; }
};


class NPCFactory {
    public:
        static std::unique_ptr<NPC> create_npc(const std::string& npc_type, 
                                            const std::string& name, 
                                            double x, double y) {
            if (npc_type == "squirrel") {
                return std::make_unique<squirrel>(name, x, y);
            }
            else if (npc_type == "werewolf") {
                return std::make_unique<werewolf>(name, x, y);
            }
            else if (npc_type == "druid") {
                return std::make_unique<druid>(name, x, y);
            }
            else {
                throw std::logic_error("invalid NPC type: " + npc_type);
            }
        }
        static void load_from_file_c_style(const char* filename, NPC_array& arr) {
            FILE* file = fopen(filename, "r");
            if (!file) {
                throw std::runtime_error("can't open file");
            }
            char type[MAX_LENGTH];
            char name[MAX_LENGTH];
            double x, y;
            int line_number = 0;
            while (fscanf(file, "%255s %255s %lf %lf", type, name, &x, &y) == 4) {
                line_number++;
                if (x < 0 || x > 500 || y < 0 || y > 500) {
                    fprintf(stderr, "Line %d: invalid NPC coords (%.2f, %.2f)\n", 
                            line_number, x, y);
                    continue;
                }
                try {
                    std::string type_str(type);
                    std::string name_str(name);
                    auto npc = create_npc(type_str, name_str, x, y);
                    arr.add_NPC(std::move(npc));
                } catch (const std::exception& e) {
                    fprintf(stderr, "Line %d: error creating NPC: %s\n", 
                            line_number, e.what());
                }
            }
            fclose(file);
        }
        static void save_to_file(const char* filename, NPC_array& arr) {
            FILE* file = fopen(filename, "w");
            if (!file) {
                throw std::runtime_error("can't open file");
            }
            for (auto& npc : arr.get_npcs()) {
                fprintf(file, "%s %s %lf %lf\n", 
                        npc->get_type().c_str(), 
                        npc->get_name().c_str(),
                        npc->get_x_cord(), 
                        npc->get_y_cord());
            }
            fclose(file);
        }
};



