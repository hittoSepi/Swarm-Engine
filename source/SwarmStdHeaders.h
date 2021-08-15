#pragma once
#ifndef Std_Headers_H_
#define Std_Headers_H_

#ifdef _WIN32
#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include <fcntl.h>
#include <io.h>

// Standard headers
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <iostream>
#include <cmath>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <unordered_map>
#include <array>

// STL algorithms & functions
#include <algorithm>
#include <cassert>

#include <boost/uuid/uuid.hpp>

// C++ Stream stuff
#include <ostream>
#include <iosfwd>
#include <atomic>
#include <memory>

// Threading
#include <thread>
#include <mutex>

// optional, better nullptR?
#include <optional>

#endif
