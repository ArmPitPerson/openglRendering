/// RandomTools by Carl Findahl (C) 2017
/// A Kukon Project

#ifndef RANDOMENGINE_H
#define RANDOMENGINE_H

#include <random>
#include "PCG/pcg_random.hpp"

class RandomEngine {
public:
	RandomEngine() : randomGenerator(mEntropySeeder) {}
	RandomEngine (const RandomEngine&) = delete;
	RandomEngine& operator=(const RandomEngine&) = delete;

	// Creates a uniform randomly distributed integer in range [min, max]
	int uniform(const int min, const int max);

	// Creates a uniform randomly distributed real number in range [min, max]
	double uniform(const double min, const double max);

	// Creates a normal distribution around [mean] with given standard [deviation]
	double normal(const double mean, const double deviation = 5.0);

	// Return true or false with given probability (0.0-1.0)
	bool trigger(const double probability);

	// Shuffles the container in between the two random access iterators
	// std::swap must be defined for the type in the container and first / last
	// Must be random access iterators!
	template<typename _It, typename IsIt = decltype(*std::declval<_It&>(), void(), ++std::declval<_It&>(), void())>
	void shuffle(_It first, _It last);

	// Pick a random element from the given container that support subscripting
	template<typename T>
	decltype(auto) pick(T& container);

	// Pick a random element from the given container that support subscripting
	template<typename T>
	decltype(auto) pick(const T& container) const;

private:
	pcg_extras::seed_seq_from<std::random_device> mEntropySeeder; // Truly random if available
	pcg32 randomGenerator; // The generator itself

};

template<typename _It, typename IsIt = decltype(*std::declval<_It&>(), void(), ++std::declval<_It&>(), void())>
void RandomEngine::shuffle(_It first, _It last) {
	std::shuffle(first, last, randomGenerator);
}

template<typename T>
decltype(auto) RandomEngine::pick(T& container) {
	return container[uniform(0, container.size() - 1)];
}

template<typename T>
decltype(auto) RandomEngine::pick(const T& container) const {
	return container[uniform(0, container.size() - 1)];
}

#endif // RANDOMENGINE_H
