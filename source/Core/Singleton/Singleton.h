#pragma once
#include <memory>

#include "Core/Framework.h"

 template<typename T>
class   Singleton {
public:
    static T& GetInstance()
    {
        static const std::unique_ptr<T> instance{new T{}};
		return *instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    Singleton() {}
};
