#include "Problem.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <fstream>

using namespace std;

CProblem::CProblem()
{
    numberOfVariables = 0;
    numberOfCorrectClauses = 0;
    pClausesCorectness = new vector<int>();
    pClausesCorectness->push_back(0);
    pVariablesInClauses = new vector<vector<int> >();
    pNumberOfTrue = new vector<int>();
    pNumberOfFalse = new vector<int>();
    pInClausesDesc = new vector<int>();
    pBetterTF = MySmartPointer<vector<bool>>(new vector<bool>());
    pLastChecked = MySmartPointer<vector<bool>>(new vector<bool>());
}

CProblem::~CProblem()
{
    delete pClausesCorectness;
    delete pVariablesInClauses;
    delete pNumberOfTrue;
    delete pNumberOfFalse;
    delete pInClausesDesc;
}

bool CProblem::load(string sourcePath)
{
    ifstream fin;
    sourcePath += ".txt";
    fin.open(sourcePath);
    string word;
    int clauseNumber = 1;
    fin >> word;
    do
    {
        pClausesCorectness->push_back(0);

        for (int i = 0; i < NumberOfVariablesInClauses; i++)
        {
            fin >> word;
            loadVariable(word, clauseNumber);
        }
        fin >> word;
        clauseNumber++;
        fin >> word;

    } while (!fin.eof());
    fin.close();

    numberOfVariables = pVariablesInClauses->size();
    prepare();

    return true;
}

void CProblem::prepare()
{
    priority_queue<pair<int, int>> betterTF;

    for (int i = 0; i < numberOfVariables; i++)
    {
        pLastChecked->push_back(true);
        if (pNumberOfTrue->at(i) > pNumberOfFalse->at(i))
        {
            betterTF.push(make_pair(pNumberOfTrue->at(i), i));
            pBetterTF->push_back(true);
        }
        else
        {
            betterTF.push(make_pair(pNumberOfFalse->at(i), i));
            pBetterTF->push_back(false);
        }
    }
    while (!betterTF.empty())
    {
        pInClausesDesc->push_back(betterTF.top().second);
        betterTF.pop();
    }
}

void CProblem::loadVariable(string variable, int clauseNumber)
{
    int variableNumber;
    bool greaterThan0 = true;
    if (variable[0] == '-')
    {
        greaterThan0 = false;
        variableNumber = stoi(variable.substr(1));
    }
    else
        variableNumber = stoi(variable);

    while (pVariablesInClauses->size() <= variableNumber)
    {
        pVariablesInClauses->push_back(vector<int>());
        pNumberOfTrue->push_back(0);
        pNumberOfFalse->push_back(0);
    }

    if (greaterThan0)
    {
        pVariablesInClauses->at(variableNumber).push_back(clauseNumber);
        addCorrect(clauseNumber);
        pNumberOfTrue->at(variableNumber)++;
    }
    else
    {
        pVariablesInClauses->at(variableNumber).push_back(clauseNumber * (-1));
        pNumberOfFalse->at(variableNumber)++;
    }
}

void CProblem::addCorrect(int clauseNumber)
{
    if (pClausesCorectness->at(clauseNumber)++ == 0)
        numberOfCorrectClauses++;
}

void CProblem::subCorrect(int clauseNumber)
{
    if (pClausesCorectness->at(clauseNumber)-- == 1)
        numberOfCorrectClauses--;
}

double CProblem::evaluate(MySmartPointer<vector<bool>> pGenotype)
{
    for (int variable = 0; variable < numberOfVariables; variable++)
    {
        if (pGenotype->at(variable) != pLastChecked->at(variable))
        {
            for (int clause = 0; clause < pVariablesInClauses->at(variable).size(); clause++)
            {
                int tmp = pVariablesInClauses->at(variable)[clause];
                if (pGenotype->at(variable))
                {
                    if (tmp > 0)
                        addCorrect(abs(tmp));
                    else
                        subCorrect(abs(tmp));
                }
                else
                {
                    if (tmp < 0)
                        addCorrect(abs(tmp));
                    else
                        subCorrect(abs(tmp));
                }
                //printClausesCorectness();
                //cout << numberOfCorrectClauses << '\n';
            }
        }
    }
    pLastChecked = pGenotype;

    return (double)numberOfCorrectClauses / (double)(pClausesCorectness->size() - 1);
}

