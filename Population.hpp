/*
 * Population.hpp
 *
 *  Created on: Jun 22, 2013
 *      Author: terhorst
 */

#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include <array>
#include <stdexcept>
#include <vector>
#include <memory>

#include <python2.7/Python.h>

#define PY_ARRAY_UNIQUE_SYMBOL twolocus_ARRAY_API
#define NO_IMPORT_ARRAY
#include <numpy/arrayobject.h>

using namespace std;

// Parameters
#define N 2000

class Population {
public:
	typedef array<int, 4> PopulationCounts;
	Population(vector<int>, double, double, double);
	PyObject* simulate(int, int);

private:
	PopulationCounts initCounts;
        double r, sX, sY;
        array<double, 4> F;

	mt19937 engine;
	uniform_int_distribution<int> uid = uniform_int_distribution<int>(0, N - 1);
	binomial_distribution<int> nrecomb;
	array<int, N> pop;
	array<float, N> fitnesses;
	PopulationCounts counts;

	void initFromCounts();
	vector<int> randomWithoutReplacement(int k);
	void mutate(int nm);
	void recombine(int nr);
	void select();
	void assign_generation(PyObject*, int, int);
};

#endif /* POPULATION_HPP */
