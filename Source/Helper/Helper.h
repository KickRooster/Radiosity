#pragma once
#include <BaseInclude.h>
#include "..\RHI\OpenRLRHI\RLShader.h"
#include <random>

#include VECTOR_INCLUDE_PATH

namespace Core
{
	class Helper
	{
	private:
		static inline int RandomInt(const int minValue, const int maxValue)
		{
			static std::default_random_engine randomEngine;
			using Distribution = std::uniform_int_distribution<int>;
			static Distribution rand;

			return rand(randomEngine, Distribution::param_type(minValue, maxValue));
		}
	public:
		static void GenerateRandomNumbers(const float minValue, const float maxValue, const size_t count, ctd::vector<float> &randomNumbers);
		static void RandomizeRadial(const float radius, const size_t count, ctd::vector<float> &randomNumbers);
		static float Random( float minValue,  float maxValue);
	};

}