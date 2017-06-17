
#pragma once

#define RELEASE_PTR(p)	if(p){p->Release(); p = nullptr;}
#define DELETE_PTR(p)	if(p){delete p; p = nullptr;}

template<typename T>
class CRefCounter
{
public:
	CRefCounter();
	CRefCounter(T* ptr);
	virtual ~CRefCounter();

	void AddRef();
	void Release();

private:
	T*		m_pPointer;
	int		m_nCounter;
};

template<typename T>
inline CRefCounter<T>::CRefCounter()
	: m_pPointer(nullptr)
	, m_nCounter(0)
{
}

template<typename T>
CRefCounter<T>::CRefCounter(T* ptr)
	: m_pPointer(ptr)
	, m_nCounter(1)
{
}

template<typename T>
inline CRefCounter<T>::~CRefCounter()
{
}

template<typename T>
inline void CRefCounter<T>::AddRef()
{
	++m_nCounter;
}

template<typename T>
inline void CRefCounter<T>::Release()
{
	if (--m_nCounter == 0)
	{
		DELETE_PTR(m_pPointer);
	}
}
