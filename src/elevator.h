#pragma once

#include <mutex>
#include <set>

class Elevator {
private:
    int elevator_id = 1;
    int current_floor = 1;
    int direction = 0;
    std::set<int> destinations[3];
    std::mutex mtx;

public:
    Elevator(int elevator_id, int current_floor);
    void display_floor(void);
    int get_current_floor(void);
    int get_direction(void);
    void move(int current, int floor);
    void operate(void);
};