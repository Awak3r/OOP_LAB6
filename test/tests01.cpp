#include <gtest/gtest.h>
#include "../include/NPC.h"
#include "../include/Observer.h"
#include "../include/Visitor.h"

#include <fstream>

// ==================== Тесты NPC ====================

TEST(NPCTest, DefaultConstructor) {
    NPC npc;
    ASSERT_EQ(npc.get_x_cord(), 0);
    ASSERT_EQ(npc.get_y_cord(), 0);
    ASSERT_EQ(npc.get_name(), "");
}

TEST(NPCTest, ParameterizedConstructor) {
    NPC npc("TestNPC", 100.5, 200.3);
    ASSERT_EQ(npc.get_name(), "TestNPC");
    ASSERT_EQ(npc.get_x_cord(), 100.5);
    ASSERT_EQ(npc.get_y_cord(), 200.3);
}

TEST(NPCTest, CopyConstructor) {
    NPC npc1("Original", 150, 250);
    NPC npc2(npc1);
    ASSERT_TRUE(npc1 == npc2);
    ASSERT_EQ(npc2.get_name(), "Original");
}

TEST(NPCTest, AssignmentOperator) {
    NPC npc1("First", 100, 200);
    NPC npc2("Second", 300, 400);
    npc2 = npc1;
    ASSERT_TRUE(npc1 == npc2);
}

TEST(NPCTest, Setters) {
    NPC npc;
    npc.set_name("NewName");
    npc.set_x(123.45);
    npc.set_y(678.90);
    ASSERT_EQ(npc.get_name(), "NewName");
    ASSERT_EQ(npc.get_x_cord(), 123.45);
    ASSERT_EQ(npc.get_y_cord(), 678.90);
}

// ==================== Тесты дочерних классов ====================

TEST(SquirrelTest, TypeCheck) {
    squirrel sq("Белка1", 100, 200);
    ASSERT_EQ(sq.get_type(), "squirrel");
    ASSERT_EQ(sq.get_name(), "Белка1");
}

TEST(WerewolfTest, TypeCheck) {
    werewolf ww("Оборотень1", 150, 250);
    ASSERT_EQ(ww.get_type(), "werewolf");
    ASSERT_EQ(ww.get_name(), "Оборотень1");
}

TEST(DruidTest, TypeCheck) {
    druid dr("Друид1", 200, 300);
    ASSERT_EQ(dr.get_type(), "druid");
    ASSERT_EQ(dr.get_name(), "Друид1");
}

// ==================== Тесты NPCFactory ====================

TEST(NPCFactoryTest, CreateSquirrel) {
    std::string type = "squirrel";
    std::string name = "TestSquirrel";
    auto npc = NPCFactory::create_npc(type, name, 100, 200);
    
    ASSERT_NE(npc, nullptr);
    ASSERT_EQ(npc->get_type(), "squirrel");
    ASSERT_EQ(npc->get_name(), "TestSquirrel");
    ASSERT_EQ(npc->get_x_cord(), 100);
    ASSERT_EQ(npc->get_y_cord(), 200);
}

TEST(NPCFactoryTest, CreateWerewolf) {
    std::string type = "werewolf";
    std::string name = "TestWerewolf";
    auto npc = NPCFactory::create_npc(type, name, 150, 250);
    
    ASSERT_NE(npc, nullptr);
    ASSERT_EQ(npc->get_type(), "werewolf");
}

TEST(NPCFactoryTest, CreateDruid) {
    std::string type = "druid";
    std::string name = "TestDruid";
    auto npc = NPCFactory::create_npc(type, name, 200, 300);
    
    ASSERT_NE(npc, nullptr);
    ASSERT_EQ(npc->get_type(), "druid");
}

TEST(NPCFactoryTest, InvalidType) {
    std::string type = "dragon";
    std::string name = "InvalidNPC";
    ASSERT_THROW(NPCFactory::create_npc(type, name, 100, 200), std::logic_error);
}

// ==================== Тесты NPC_array ====================

TEST(NPCArrayTest, DefaultConstructor) {
    NPC_array arr;
    ASSERT_EQ(arr.get_size(), 0);
}

TEST(NPCArrayTest, AddNPC) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 200));
    ASSERT_EQ(arr.get_size(), 1);
}

TEST(NPCArrayTest, AddMultipleNPCs) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 200));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 150, 250));
    arr.add_NPC(std::make_unique<druid>("Друид1", 200, 300));
    ASSERT_EQ(arr.get_size(), 3);
}

TEST(NPCArrayTest, RemoveByCoordinates) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 200));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 150, 250));
    
    arr.remove_at(100, 200);
    ASSERT_EQ(arr.get_size(), 1);
}

TEST(NPCArrayTest, RemoveByName) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 200));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 150, 250));
    
    arr.remove_npc("Белка1");
    ASSERT_EQ(arr.get_size(), 1);
}

TEST(NPCArrayTest, Clear) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 200));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 150, 250));
    
    arr.clear();
    ASSERT_EQ(arr.get_size(), 0);
}

TEST(NPCArrayTest, PrintAll) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 200));
    
    testing::internal::CaptureStdout();
    arr.print_all();
    std::string output = testing::internal::GetCapturedStdout();
    
    ASSERT_TRUE(output.find("squirrel") != std::string::npos);
    ASSERT_TRUE(output.find("Белка1") != std::string::npos);
}

// ==================== Тесты файловых операций ====================

TEST(FileOperationsTest, SaveAndLoad) {
    const char* filename = "test_npcs.txt";
    
    // Создаём и сохраняем
    NPC_array arr1;
    arr1.add_NPC(std::make_unique<squirrel>("Белка1", 100, 200));
    arr1.add_NPC(std::make_unique<werewolf>("Оборотень1", 150, 250));
    arr1.add_NPC(std::make_unique<druid>("Друид1", 200, 300));
    
    NPCFactory::save_to_file(filename, arr1);
    
    // Загружаем
    NPC_array arr2;
    NPCFactory::load_from_file_c_style(filename, arr2);
    
    ASSERT_EQ(arr2.get_size(), 3);
    
    // Удаляем тестовый файл
    std::remove(filename);
}

TEST(FileOperationsTest, LoadInvalidCoordinates) {
    const char* filename = "test_invalid.txt";
    
    // Создаём файл с некорректными координатами
    std::ofstream file(filename);
    file << "squirrel Белка1 600 700\n";  // Выходит за пределы
    file << "werewolf Оборотень1 150 250\n";
    file.close();
    
    NPC_array arr;
    NPCFactory::load_from_file_c_style(filename, arr);
    
    // Должна загрузиться только одна корректная запись
    ASSERT_EQ(arr.get_size(), 1);
    
    std::remove(filename);
}

TEST(FileOperationsTest, LoadNonExistentFile) {
    NPC_array arr;
    ASSERT_THROW(NPCFactory::load_from_file_c_style("nonexistent.txt", arr), 
                 std::runtime_error);
}

// ==================== Тесты Observer ====================

TEST(ObserverTest, FileLoggerCreation) {
    FileLogger logger("test.log");
    logger.update("Test event");
    
    // Проверяем, что файл создан
    std::ifstream file("test.log");
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line, "Test event");
    
    file.close();
    std::remove("test.log");
}

TEST(ObserverTest, DisplayOutput) {
    Display display;
    
    testing::internal::CaptureStdout();
    display.update("Test message");
    std::string output = testing::internal::GetCapturedStdout();
    
    ASSERT_TRUE(output.find("Test message") != std::string::npos);
}

// ==================== Тесты CombatVisitor ====================

TEST(CombatTest, SquirrelKillsWerewolf) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 100));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 110, 110));
    
    CombatVisitor combat;
    combat.add_observer(std::make_unique<Display>());
    
    combat.do_combat(arr, 50.0);
    
    // Оборотень должен быть убит
    ASSERT_EQ(arr.get_size(), 1);
    
    // Проверяем, что остался squirrel
    for (const auto& npc : arr.get_npcs()) {
        ASSERT_EQ(npc->get_type(), "squirrel");
    }
}

TEST(CombatTest, SquirrelKillsDruid) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 100));
    arr.add_NPC(std::make_unique<druid>("Друид1", 110, 110));
    
    CombatVisitor combat;
    combat.do_combat(arr, 50.0);
    
    // Друид должен быть убит
    ASSERT_EQ(arr.get_size(), 1);
}

TEST(CombatTest, WerewolfKillsDruid) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 100, 100));
    arr.add_NPC(std::make_unique<druid>("Друид1", 110, 110));
    
    CombatVisitor combat;
    combat.do_combat(arr, 50.0);
    
    // Друид должен быть убит
    ASSERT_EQ(arr.get_size(), 1);
}

TEST(CombatTest, DruidDoesNotAttack) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<druid>("Друид1", 100, 100));
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 110, 110));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 120, 120));
    
    CombatVisitor combat;
    combat.do_combat(arr, 50.0);
    
    // Друид никого не убивает, но может быть убит
    // Белка убьёт оборотня и друида
    ASSERT_EQ(arr.get_size(), 1);
}

TEST(CombatTest, OutOfRange) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 100));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 400, 400));
    
    CombatVisitor combat;
    combat.do_combat(arr, 50.0);  // Малый радиус
    
    // Никто не должен погибнуть - далеко друг от друга
    ASSERT_EQ(arr.get_size(), 2);
}

TEST(CombatTest, SameTypeNoKill) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 100));
    arr.add_NPC(std::make_unique<squirrel>("Белка2", 110, 110));
    
    CombatVisitor combat;
    combat.do_combat(arr, 50.0);
    
    // Белки не убивают белок
    ASSERT_EQ(arr.get_size(), 2);
}

TEST(CombatTest, ComplexBattle) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 100));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 110, 110));
    arr.add_NPC(std::make_unique<druid>("Друид1", 120, 120));
    arr.add_NPC(std::make_unique<squirrel>("Белка2", 130, 130));
    
    CombatVisitor combat;
    combat.do_combat(arr, 100.0);
    
    // После боя должны остаться только белки
    ASSERT_LE(arr.get_size(), 2);
    
    for (const auto& npc : arr.get_npcs()) {
        ASSERT_EQ(npc->get_type(), "squirrel");
    }
}

TEST(CombatTest, WithObservers) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 100, 100));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 110, 110));
    
    CombatVisitor combat;
    combat.add_observer(std::make_unique<FileLogger>("combat_test.log"));
    combat.add_observer(std::make_unique<Display>());
    
    testing::internal::CaptureStdout();
    combat.do_combat(arr, 50.0);
    std::string output = testing::internal::GetCapturedStdout();
    
    // Проверяем, что событие залогировано
    ASSERT_TRUE(output.find("убит") != std::string::npos);
    
    // Проверяем файл
    std::ifstream log("combat_test.log");
    ASSERT_TRUE(log.is_open());
    
    std::string line;
    std::getline(log, line);
    ASSERT_TRUE(line.find("убит") != std::string::npos);
    
    log.close();
    std::remove("combat_test.log");
}

// ==================== Граничные случаи ====================

TEST(EdgeCaseTest, CoordinatesAtBoundary) {
    auto npc1 = NPCFactory::create_npc("squirrel", "Edge1", 0, 0);
    auto npc2 = NPCFactory::create_npc("werewolf", "Edge2", 500, 500);
    
    ASSERT_EQ(npc1->get_x_cord(), 0);
    ASSERT_EQ(npc2->get_x_cord(), 500);
}

TEST(EdgeCaseTest, EmptyArray) {
    NPC_array arr;
    CombatVisitor combat;
    
    ASSERT_NO_THROW(combat.do_combat(arr, 100.0));
    ASSERT_EQ(arr.get_size(), 0);
}

TEST(EdgeCaseTest, SingleNPC) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Одинокая", 100, 100));
    
    CombatVisitor combat;
    combat.do_combat(arr, 100.0);
    
    // Один NPC не может сражаться сам с собой
    ASSERT_EQ(arr.get_size(), 1);
}

TEST(EdgeCaseTest, VeryLargeRadius) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Белка1", 0, 0));
    arr.add_NPC(std::make_unique<werewolf>("Оборотень1", 500, 500));
    
    CombatVisitor combat;
    combat.do_combat(arr, 1000.0);
    
    // Белка убьёт оборотня даже на большом расстоянии
    ASSERT_EQ(arr.get_size(), 1);
}

// ==================== Main ====================

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}