#pragma once

#include "elevator.h"
#include <memory>
#include <vector>

void RenderElevatorPanel(std::vector<std::unique_ptr<Elevator>> &elevators);