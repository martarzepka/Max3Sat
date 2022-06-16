#pragma once

#include <string>
#include <vector>
#include "mySmartPointer.h"
#include <queue>

using namespace std;

const int NumberOfVariablesInClauses = 3;

class CProblem
{
public:
	CProblem();
	
	virtual ~CProblem();

	bool  bLoad(std::string sSourcePath) { return load(sSourcePath); };

	double dEvaluate(MySmartPointer<vector<bool>>pdGenotype) { return evaluate(pdGenotype); };

	int iGetNumberVariables() { return getNumberOfVariables(); }

	bool  load(string sourcePath);

	void prepare();

	double evaluate(MySmartPointer<vector<bool>> pGenotype);

	int getNumberOfVariables() { return(numberOfVariables); }

	MySmartPointer<vector<bool>> getBetterTF() { return pBetterTF; };

	vector<int>* getInClausesDesc() { return pInClausesDesc; };


protected:
	int numberOfVariables;
	int numberOfCorrectClauses;
	vector<int>* pClausesCorectness;
	vector<vector<int> >* pVariablesInClauses;
	vector<int>* pInClausesDesc;
	vector<int>* pNumberOfTrue;
	vector<int>* pNumberOfFalse;
	MySmartPointer<vector<bool>> pBetterTF;
	MySmartPointer<vector<bool>> pLastChecked;

	void loadVariable(string variable, int clauseNumber);
	void addCorrect(int clauseNumber);
	void subCorrect(int clauseNumber);

};//class CProblem


