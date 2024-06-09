#pragma once

#include <string>

#define THROW(EXCEPTION, MESSAGE) throw EXCEPTION(std::string(MESSAGE))