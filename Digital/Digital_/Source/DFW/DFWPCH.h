#pragma once

#ifdef DFW_PLATFORM_WINDOWS
	#include <Windows.h>
#endif 

#include <Defines/Defines.h>

// Boost
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/container_hash/hash.hpp>

// STL
#include <stdint.h>

#include <iostream>
#include <sstream>
#include <filesystem>
#include <string>

#include <chrono>
#include <algorithm>
#include <memory>
#include <functional>
#include <limits>

#include <typeindex>
#include <typeinfo>

#include <array>
#include <vector>
#include <unordered_map>
#include <map>
