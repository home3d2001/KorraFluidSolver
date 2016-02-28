#ifndef UTILITY_H
#define UTILITY_H

#include <json/json.h>
#include <iostream>
#include <fstream>
#include <string>

// This file includes all the helpers for our fluid solvers

bool ParseJson(const char* filepath, Json::Value& root);

#endif UTILITY_H
