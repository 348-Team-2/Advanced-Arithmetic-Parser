/**
* @file math.hpp
 * @brief Public interface for loading the standard mathematical library.
 */

#pragma once

#include "registry.hpp"

/**
 * @brief Populates the given Registry with all standard operations, functions, and constants.
 * @param registry The active Registry instance to load data into.
 */
void load_registry(Registry& registry);