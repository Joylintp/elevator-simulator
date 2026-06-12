#include "config.h"
#include "elevator.h"
#include "panel.h"
#include <memory>
#include <thread>
#include <vector>

int main() {
    // Generate elevators
    std::vector<std::unique_ptr<Elevator>> elevators(ELEVATOR_COUNT);
    for (int i = 0; i < ELEVATOR_COUNT; i++) {
        elevators[i] = std::make_unique<Elevator>(i + 1, 1);
    }

    // Start the elevators in each thread
    for (int i = 0; i < ELEVATOR_COUNT; i++) {
        std::thread t([&elevators, i]() { elevators[i] -> operate(); });
        t.detach();
    }

    RenderElevatorPanel(elevators);
    return 0;
}