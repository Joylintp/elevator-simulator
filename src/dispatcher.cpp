#include "config.h"
#include "elevator.h"
#include <algorithm>
#include <memory>
#include <vector>

int CalculateDistance(Elevator &elevator, int from, int to) {
    if (elevator.get_direction() == 0) {
        return abs(from - elevator.get_current_floor());
    } else if (elevator.get_direction() == -1) {
        if (elevator.get_current_floor() > from && from > to) {
            return elevator.get_current_floor() - from;
        } else {
            return (elevator.get_current_floor() - 1) + (from - 1);
        }
    } else {
        if (elevator.get_current_floor() < from && from < to) {
            return from - elevator.get_current_floor();
        } else {
            return (10 - elevator.get_current_floor()) + (10 - from);
        }
    }
}

void DispatchElevators(std::vector<std::unique_ptr<Elevator>> &elevators, int from, int to) {
    std::vector<int> distance(ELEVATOR_COUNT);
    for (int i = 0; i < ELEVATOR_COUNT; i++) {
        distance[i] = CalculateDistance(*elevators[i], from, to);
    }
    int closest_elevator = min_element(distance.begin(), distance.end()) - distance.begin();
    elevators[closest_elevator] -> move(from, to);
}