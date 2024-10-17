#ifndef RANDUM_HPP
#define RANDUM_HPP

#include <random>


inline std::random_device rd{};

inline std::seed_seq ss{ rd(),rd(),rd(),rd(),rd(),rd(),rd() };

static std::mt19937 mt{ ss };

namespace arc
{



	inline int randomi(int min = 0, int max = 9)
	{
		std::uniform_int_distribution<int> range(min, max);
		return range(mt);
	}

	inline float randomf(float min = 0.0f, float max = 9.9f)
	{
		std::uniform_real_distribution<float> range(min, max);
		return range(mt);
	}

	inline char randomc()
	{
		static const char alpha[]{ "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" };

		std::uniform_int_distribution<int> range(0, 51);
		return alpha[range(mt)];
	}

}
#endif // !RANDUM_HPP
