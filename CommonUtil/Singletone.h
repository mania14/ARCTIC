#pragma once

template < class T >

class Singletone
{
public:
	static T& This() {
		return pInstance;
	};

private:
	static T pInstance;
};

template<class T> T Singletone<T>::pInstance;