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


class Observer {
    public:
        virtual void update(const std::string& event) = 0;
        virtual ~Observer() = default;
};

class FileLogger: public Observer {
    public:
        FileLogger() : filename("log.txt") {}
        FileLogger(const std::string& name) : filename(name) {}
        void update(const std::string& event) override {
            FILE* log = fopen(filename.c_str(), "a");  
            if (log) {
                fprintf(log, "%s\n", event.c_str());
                fclose(log);
            }
        }
        ~FileLogger() = default;
    private:
        std::string filename;
};

class Display: public Observer {
public:
    void update(const std::string& event) override {
        std::cout << event << std::endl;
    }
};