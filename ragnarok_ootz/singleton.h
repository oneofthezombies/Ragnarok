#pragma once

template<typename T>
class Singleton
{
public:
    static T* getInstance()
    {
        if (!_instance)
            _instance = new T;

        return _instance;
    }

protected:
    static T* _instance;

    Singleton() 
    {
    }

    virtual ~Singleton() 
    {
    }
};

template<typename T>
T* Singleton<T>::_instance = 0;
