#pragma once

#include "Util.h"
#include <chrono>

namespace Misc
{
	namespace stm = std::chrono;

	typedef
		std::conditional<stm::high_resolution_clock::is_steady,
		stm::high_resolution_clock,
		stm::steady_clock>::type
		clock;

	class Stopwatch
	{
		clock::time_point m_start;
		clock::duration	  m_duration;

		public:

			void Start();
			void Stop();

			clock::duration GetTime();
	};
}