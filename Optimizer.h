#pragma once

#include "Problem.h"

#include <random>
#include <vector>

using namespace std;

const int populationSize = 100;
const double crossoverProbability = 0.7;
const double mutationProbability = 0.2;

class  CIndividual;

class COptimizer
{
public:
	COptimizer(CProblem *pcProblem);
	~COptimizer() {};

	void vInitialize() { initialize(); };
	void vRunIteration() { runIteration(); };

	MySmartPointer<CIndividual> pcGetBestFound() { return pGetBestFound(); }

	void initialize();
	void runIteration();

	MySmartPointer<CIndividual> pGetBestFound();
	CProblem* getProblem() { return pProblem; };

	bool randomBool();
	double randomDouble();
	int randomPopulationIndex();

private:

	CProblem* pProblem;
	mt19937 randEngine;
	MySmartPointer<CIndividual> pBestFound;
	double bestFitness;
	vector<MySmartPointer<CIndividual>>  population;


	MySmartPointer<CIndividual> newCIndividual(int division);
	int chooseParent();
	void searchForBest();
};//class COptimizer


class  CIndividual
{
public:
	CIndividual(COptimizer* pOpt);
	CIndividual(COptimizer* pOpt, MySmartPointer<vector<bool>> pGen);
	CIndividual(CIndividual& pOther);
	~CIndividual() {};

	pair<MySmartPointer<CIndividual>, MySmartPointer<CIndividual>> crossover(MySmartPointer<CIndividual> pOther);
	void mutation();
	double fitness();

	void addGene(bool gene);
	bool getGene(int index);

	MySmartPointer<vector<bool>> getGenotype() { return pGenotype; };

	int* piGetResultAsTable();

private:
	COptimizer* pOptimizer;
	MySmartPointer<vector<bool>> pGenotype;

};//class  CIndividual