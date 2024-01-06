#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <ctime>
#include <vector>
#include <iostream>

#include "v5.h"
#include "v5_vcs.h"
#define _USE_MATH_DEFINES

#define waitUntil(condition) \
  do                         \
  {                          \
    wait(5, msec);           \
  } while (!(condition))

#define repeat(iterations) \
  for (int iterator = 0; iterator < iterations; iterator++)