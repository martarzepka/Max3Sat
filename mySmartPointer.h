#pragma once
#include <vector>

using namespace std;

template< typename T >class RefCounter;

template< typename T >class MySmartPointer
{
public:
	MySmartPointer();
	MySmartPointer(T* pPoint);
	MySmartPointer(const MySmartPointer& pOther);
	~MySmartPointer();

	RefCounter<T>* getCounter() { return pCounter; }
	T& operator*() { return(*pPointer); }
	T* operator->() { return(pPointer); }
	void operator=(const MySmartPointer& pOther);

	void deletePointer();
	void copyPointer(const MySmartPointer& pOther);

private:
	RefCounter<T>* pCounter;
	T* pPointer;
};

template <typename T>
MySmartPointer<T>::MySmartPointer()
{
	pPointer = NULL;
	pCounter = new RefCounter<T>();
	pCounter->add();
}

template <typename T>
MySmartPointer<T>::MySmartPointer(T* pPoint)
{
	pPointer = pPoint;
	pCounter = new RefCounter<T>();
	pCounter->add();
}

template <typename T>
MySmartPointer<T>::MySmartPointer(const MySmartPointer& pOther)
{
	copyPointer(pOther);
}

template <typename T>
MySmartPointer<T>::~MySmartPointer()
{
	deletePointer();
}

template<typename T>
void MySmartPointer<T>::operator=(const MySmartPointer& pOther)
{
	deletePointer();
	copyPointer(pOther);
}

template<typename T>
void MySmartPointer<T>::deletePointer()
{
	if (pCounter->dec() == 0)
	{
		if (pPointer != NULL)
		{
			delete pPointer;
		}
		if (pCounter != NULL)
			delete pCounter;
	}
}

template<typename T>
void MySmartPointer<T>::copyPointer(const MySmartPointer& pOther)
{
	pPointer = pOther.pPointer;
	pCounter = pOther.pCounter;
	pCounter->add();
}



template< typename T >class RefCounter
{
public:
	RefCounter() { count = 0; }
	int add();
	int dec();
	int get() { return(count); }

private:
	int count;
};

template<typename T>
int RefCounter<T>::add()
{
	return(++count);
}

template<typename T>
int RefCounter<T>::dec()
{
	return(--count);
}

