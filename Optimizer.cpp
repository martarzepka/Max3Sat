#include "Optimizer.h"

#include <cfloat>
#include <iostream>
#include <windows.h>

using namespace std;

COptimizer::COptimizer(CProblem* pProb)
{
	pProblem = pProb;
	MySmartPointer<vector<bool>> first(new vector<bool>());
	first->assign(pProblem->getNumberOfVariables(), true);
	pBestFound = MySmartPointer<CIndividual>(new CIndividual(this, first));
	bestFitness = pBestFound->fitness();
	randEngine.seed(random_device()());
	randEngine.discard(3200);
}

void COptimizer::initialize()
{
	double division = (double)pProblem->getNumberOfVariables();
	double desc = pProblem->getNumberOfVariables() / populationSize;
	for (int i = 0; i < populationSize / 2; i++)
	{
		population.push_back(newCIndividual((int)division));
		division -= desc;
	}
	for (int i = populationSize / 2; i < populationSize; i++)
	{
		population.push_back(newCIndividual(0));
	}
	searchForBest();
}

void COptimizer::runIteration()
{
	vector<MySmartPointer<CIndividual>>  newPopulation;

	while (newPopulation.size() < populationSize)
	{
		int parent1Index = chooseParent();
		int parent2Index = chooseParent();
		pair<MySmartPointer<CIndividual>, MySmartPointer<CIndividual>> children = population[parent1Index]->crossover(population[parent2Index]);

		children.first->mutation();
		children.second->mutation();

		newPopulation.push_back(children.first);
		newPopulation.push_back(children.second);
	}

	population = newPopulation;

	searchForBest();
}

MySmartPointer<CIndividual> COptimizer::pGetBestFound()
{
	return pBestFound;
}

bool COptimizer::randomBool()
{
	uniform_int_distribution<> ri(0, 1);
	return ri(randEngine);
}

double COptimizer::randomDouble()
{
	uniform_real_distribution<> rd(0.0, 1.0);
	return rd(randEngine);
}

int COptimizer::randomPopulationIndex()
{
	uniform_int_distribution<> ri(0, populationSize - 1);
	return ri(randEngine);
}

MySmartPointer<CIndividual> COptimizer::newCIndividual(int division)
{
	MySmartPointer<vector<bool>> pGen(new vector<bool>());
	pGen->assign(pProblem->getNumberOfVariables(), true);
	for (int i = 0; i < division; i++)
	{
		pGen->at(pProblem->getInClausesDesc()->at(i)) = pProblem->getBetterTF()->at(pProblem->getInClausesDesc()->at(i));
	}
	for (int i = division; i < pProblem->getNumberOfVariables(); i++)
	{
		pGen->at(pProblem->getInClausesDesc()->at(i)) = randomBool();
	}
	return  MySmartPointer<CIndividual>(new CIndividual(this, pGen));
}

int COptimizer::chooseParent()
{
	int index1 = randomPopulationIndex();
	int index2 = randomPopulationIndex();
	if (population[index1]->fitness() > population[index1]->fitness())
		return index1;
	return index2;
}

void COptimizer::searchForBest()
{
	for (int i = 0; i < populationSize; i++)
	{
		double fit = population[i]->fitness();
		if (fit > bestFitness)
		{
			bestFitness = fit;
			pBestFound = population[i];
		}
	}
}

CIndividual::CIndividual(COptimizer* pOpt)
{
	pOptimizer = pOpt;
	pGenotype = MySmartPointer<vector<bool>>(new vector<bool>());
}

CIndividual::CIndividual(COptimizer* pOpt, MySmartPointer<vector<bool>> pGen)
{
	pOptimizer = pOpt;
	pGenotype = pGen;
}

CIndividual::CIndividual(CIndividual& pOther)
{
	pOptimizer = pOther.pOptimizer;
	pGenotype = new vector<bool>();
	for (int i = 0; i < pOther.pGenotype->size(); i++)
		pGenotype->push_back(pOther.pGenotype->at(i));
}

pair<MySmartPointer<CIndividual>, MySmartPointer<CIndividual>> CIndividual::crossover(MySmartPointer<CIndividual> pOther)
{

	MySmartPointer<CIndividual> child1;
	MySmartPointer<CIndividual> child2;
	if (crossoverProbability > pOptimizer->randomDouble())
	{
		child1 = new CIndividual(pOptimizer);
		child2 = new CIndividual(pOptimizer);

		for (int i = 0; i < pOptimizer->getProblem()->getNumberOfVariables(); i++)
		{
			if (pOptimizer->randomBool())
			{
				child1->addGene(getGene(i));
				child2->addGene(pOther->getGene(i));
			}
			else
			{
				child1->addGene(pOther->getGene(i));
				child2->addGene(getGene(i));
			}
		}
	}
	else
	{
		child1 = new CIndividual(*this);
		child2 = new CIndividual(*pOther);
	}

	return make_pair(child1, child2);
}

void CIndividual::mutation()
{
	MySmartPointer<vector<bool>> pGen(new vector<bool>());
	for (int i = 0; i < pOptimizer->getProblem()->getNumberOfVariables(); i++)
	{
		if (mutationProbability > pOptimizer->randomDouble())
			pGen->push_back(!pGenotype->at(i));
		else
			pGen->push_back(pGenotype->at(i));
	}
	if (pOptimizer->getProblem()->evaluate(pGen) > fitness())
		pGenotype = pGen;
}

double CIndividual::fitness()
{
	return pOptimizer->getProblem()->evaluate(pGenotype);
}

void CIndividual::addGene(bool gene)
{
	pGenotype->push_back(gene);
}

bool CIndividual::getGene(int index)
{
	return pGenotype->at(index);
}

int* CIndividual::piGetResultAsTable()
{
	int* tab;
	tab = new int[pGenotype->size()];
	for (int i = 0; i < pGenotype->size(); i++)
	{
		tab[i] = pGenotype->at(i);
	}
	return tab;
}


