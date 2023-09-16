#pragma once
#include <iostream>
#include <unistd.h>
#include <fstream>
#include "constants.h"


int child_process(int fd[2], std::string &file_name, std::ifstream &file);
