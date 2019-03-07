#include "Helper.h"
#include "FileOperator.h"
#include <random>
#include <ctime>
#include <limits>

namespace Core
{
	void Helper::GenerateRandomNumbers(const float minValue, const float maxValue, const size_t count, ctd::vector<float> &randomNumbers)
	{
		static std::random_device randomDevice;
		static std::mt19937 generator(randomDevice());

		// Always seed the generator with the current system time.
		generator.seed(static_cast<unsigned int>(time(nullptr)));

		std::uniform_real_distribution<float> distribution(minValue, maxValue);

		randomNumbers.resize(count);
		for (size_t ii = 0; ii < count; ++ii)
		{
			randomNumbers[ii] = distribution(generator);
		}
	}

	void Helper::RandomizeRadial(const float radius, const size_t count, ctd::vector<float> &randomNumbers)
	{
		// Generate the random values via rejection sampling.
		float radiusSquared = radius * radius;
		randomNumbers.resize(count);

		ctd::vector<float> randomValues;
		GenerateRandomNumbers(0.0f, 1.0f, count, randomValues);
		size_t randomIndex = 0;

		for (size_t ii = 0; ii < count; ii += 3)
		{
			if (randomIndex == randomValues.size())
			{
				// Regenerate the random value, we've ran out. There's a very low probability
				// of this code every actually executing.
				GenerateRandomNumbers(0.0f, 1.0f, count, randomValues);
				randomIndex = 0;
			}

			float x, y;
			do
			{
				x = radius * (2.0f * randomValues[randomIndex + 0] - 1.0f);
				y = radius * (2.0f * randomValues[randomIndex + 1] - 1.0f);
				++randomIndex;
			} while ((x * x + y * y) > radiusSquared);

			randomNumbers[ii + 0] = x;
			randomNumbers[ii + 1] = y;

			// Unused since this generates values in a 2D plane yet we have to use 3 components because OpenRL doesn't support a 2 component texture.
			randomNumbers[ii + 2] = 0.0f;
		}
	}

	float Helper::Random(float minValue, float maxValue)
	{
		int32 maxInt = std::numeric_limits<int32>::max();

		int32 r = RandomInt(0, maxInt);
		float fUnit = static_cast<float>((float)r / maxInt);
		float fDiff = maxValue - minValue;

		return minValue + fUnit * fDiff;
	}
}