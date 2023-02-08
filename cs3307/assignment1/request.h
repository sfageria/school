
#include <vector>
#include "gameObject.h"
#include "json.hpp"

#ifndef REQUEST_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define REQUEST_H

void initRequest();
std::vector<Game> createVector(nlohmann::json & myjson);
//void createVector(nlohmann::json myjson);



#endif
