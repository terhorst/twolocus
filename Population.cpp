//============================================================================
// Name        : Population.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Population.hpp"

int recomb_rules[4][4][2] = {
	{
		{0,0},
		{0,1},
		{0,2},
		{1,2},
	},
	{
		{0,1},
		{1,1},
		{0,3},
		{1,3},
	},
	{
		{0,2},
		{0,3},
		{2,2},
		{2,3},
	},
	{
		{1,2},
		{1,3},
		{2,3},
		{3,3},
	}
};


void Population::initFromCounts() {
	int k = 0;
	counts.fill(0);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < initCounts[i]; j++) {
			fitnesses[k] = F[i];
			counts[i]++;
			pop[k++] = i;
		}
}

vector<int> Population::randomWithoutReplacement(int k) {
	int s;
	vector<int> ret;
	ret.assign(k, -1);
	for (int i = 0; i < k; i++) {
		do {
			s = uid(engine);
		} while (std::find(ret.begin(), ret.end(), s) != ret.end());
		ret[i] = s;
	}
	return ret;
}

void Population::mutate(int nm) {
	// Perform nm mutations
	if (nm == 0) {
		return;
	}
	vector<int> mutants = randomWithoutReplacement(nm);
}

void Population::recombine(int nr) {
	// Perform nr recombinations
	if (nr == 0)
		return;
	int oldType, newType, p1, p2;
	vector<int> parents = randomWithoutReplacement(2 * nr);
	for (int k = 0; k < nr; k++) {
		p1 = parents[k];
		p2 = parents[2*k];
		oldType = pop[p1];
		newType = recomb_rules[pop[p1]][pop[p2]][(int)(rand() < 0.5)];
		pop[p1] = newType;
		fitnesses[p1] = F[newType];
		counts[oldType]--;
		counts[newType]++;
	}
}

void Population::select() {
	int ty, a;
	discrete_distribution<int> dd(fitnesses.begin(), fitnesses.end());
	counts.fill(0);
	array<int, N> oldPop;
	copy(pop.begin(), pop.end(), oldPop.begin());
	for (int i = 0; i < N; i++) {
		a = dd(engine);
		ty = oldPop[a];
		counts[ty]++;
		fitnesses[i] = F[ty];
		pop[i] = ty;
	}
}

Population::Population(vector<int> pc, double recomb, double ssX, double ssY) : initCounts({pc[0], pc[1], pc[2], pc[3]}), r(recomb), 
    sX(ssX), sY(ssY), F({
            (1 + ssX / 2) * (1 + ssY / 2),
            (1 + ssX / 2) * (1 - ssY / 2),
            (1 - ssX / 2) * (1 + ssY / 2),
            (1 - ssX / 2) * (1 - ssY / 2)
            }), nrecomb(binomial_distribution<int>(N, recomb)) {}

PyObject* Population::simulate(int n, int T) {
	npy_intp ndims[3] = {n, T, 4};
	PyObject* ret = PyArray_SimpleNew(3, ndims, NPY_INT32);

	for (int m = 0; m < n; m++) {
		initFromCounts();
		assign_generation(ret, m, 0);
		for (int t = 1; t < T; t++) {
			// Number of recombinants in this generation
			recombine(nrecomb(engine));
			select();
			assign_generation(ret, m, t);
		}
	}
	return ret;
}

void Population::assign_generation(PyObject* a, int m, int t) {
	for (int i = 0; i < 4; i++)
		*(int*)PyArray_GETPTR3((PyArrayObject*)a, m, t, i) = counts[i];
}
