#pragma once

#include "elevator.h"
#include <memory>
#include <vector>

void DispatchElevators(std::vector<std::unique_ptr<Elevator>> &elevators, int from, int to);