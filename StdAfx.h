#pragma once

#pragma warning(disable : 4267)

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
	#include <Windows.h>
#endif

/************<C headers>*************/
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cwchar>
#include <ciso646>

/************<Containers>*************/
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <unordered_map>

/************<Streams>*************/
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

/************<Threading>*************/
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

/************<Misc>*************/
#include <memory>
#include <utility>
#include <exception>
#include <algorithm>
#include <chrono>
#include <type_traits>