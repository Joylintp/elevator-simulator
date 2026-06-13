#include "config.h"
#include "elevator.h"
#include <chrono>
#include <iostream>
#include <mutex>
#include <set>
#include <thread>

Elevator::Elevator(int elevator_id, int current_floor) : elevator_id(elevator_id), current_floor(current_floor), direction(0) {
    std::lock_guard<std::mutex> lock(mtx);
    destinations[0] = destinations[1] = destinations[2] = std::set<int>();
}

void Elevator::display_floor(void) {
    std::cout << "Elevator [" << elevator_id << "] is currently on " << current_floor << "F\n";
}

int Elevator::get_current_floor(void) {
    return current_floor;
}

int Elevator::get_direction(void) {
    return direction;
}

void Elevator::move(int current, int floor) {
    std::lock_guard<std::mutex> lock(mtx);
    if (direction == 0) {
        if (current == current_floor) {
            direction = (current < floor ? 1 : -1);
            destinations[0].insert(current);
            destinations[0].insert(floor);
        } else if (current < current_floor) {
            direction = -1;
            destinations[0].insert(current);
            if (floor < current) {
                destinations[0].insert(floor);
            } else {
                destinations[1].insert(floor);
            }
        } else {
            direction = 1;
            destinations[0].insert(current);
            if (floor > current) {
                destinations[0].insert(floor);
            } else {
                destinations[1].insert(floor);
            }
        }
    } else if (direction == -1) {
        if (current_floor >= current) {
            destinations[0].insert(current);
            if (current > floor) {
                destinations[0].insert(floor);
            } else {
                destinations[1].insert(floor);
            }
        } else {
            destinations[1].insert(current);
            if (current < floor) {
                destinations[1].insert(floor);
            } else {
                destinations[2].insert(floor);
            }
        }
    } else {
        if (current_floor <= current) {
            destinations[0].insert(current);
            if (current < floor) {
                destinations[0].insert(floor);
            } else {
                destinations[1].insert(floor);
            }
        } else {
            destinations[1].insert(current);
            if (current > floor) {
                destinations[1].insert(floor);
            } else {
                destinations[2].insert(floor);
            }
        }
    }
}

void Elevator::operate(void) {
    while (true) {
        if (direction != 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::unique_lock lock(mtx);

            if (destinations[0].count(current_floor)) {
                destinations[0].erase(current_floor);
            }
            if (destinations[0].empty()) {
                destinations[0] = destinations[1];
                destinations[1] = destinations[2];
                destinations[2].clear();
                if (destinations[0].empty()) {
                    direction = 0;
                } else {
                    direction = -direction;
                }
            }
            current_floor += direction;

            lock.unlock();
        }
    }
}