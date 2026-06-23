#pragma once
#include "Mesh.h"
#include <memory>
#include <string>

std::unique_ptr<Mesh> loadObj(const std::string& path);
