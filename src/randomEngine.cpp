#include "randomEngine.h"


int RandomEngine::uniform(const int min, const int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(randomGenerator);
}

double RandomEngine::uniform(const double min, const double max) {
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(randomGenerator);
}

double RandomEngine::normal(const double mean, const double deviation) {
	std::normal_distribution<double> normalDistributor(mean, deviation);
	return normalDistributor(randomGenerator);
}

bool RandomEngine::trigger(const double probability) {
	std::bernoulli_distribution probabilityTrigger(probability);
	return probabilityTrigger(randomGenerator);
}
