#ifndef DEEPPTR_HPP
#define DEEPPTR_HPP

// Template ausiliario per la gerarchia con distruzione e copia profonda

template<class T>
class DeepPtr
{
    private:
        T* pointed;

    public:

        // Costruttori e distruttore

        DeepPtr(T * = nullptr);
        DeepPtr(const DeepPtr&); // Copia profonda
        ~DeepPtr();

        T* target() const; // alias di operator->

        // Overloading degli operatori

        T& operator *() const;
        T* operator ->() const;

        DeepPtr& operator =(const DeepPtr&);
        bool operator ==(const DeepPtr&) const;
        bool operator !=(const DeepPtr&) const;
};


template<class T>
DeepPtr<T>::DeepPtr(T * item)
    : pointed(item)
{ }


template<class T>
DeepPtr<T>::DeepPtr(const DeepPtr& copy)
    : pointed(copy.pointed == nullptr ? nullptr : (copy.pointed)->deepClone())
{ }


template<class T>
DeepPtr<T>::~DeepPtr()
{
    if(pointed)
    {
        delete pointed;
    }
}


template<class T>
T* DeepPtr<T>::target() const
{
    return pointed;
}


template<class T>
T* DeepPtr<T>::operator->() const
{
    return pointed;
}


template <class T>
T& DeepPtr<T>::operator*() const
{
    return *pointed;
}


template <class T>
DeepPtr<T>& DeepPtr<T>::operator= (const DeepPtr& deep)
{
    if(this != &deep)
    {
        if(pointed)
        {
            delete pointed;
        }

        pointed = deep.pointed == nullptr ? nullptr : (deep.pointed)->deepClone();
    }

    return *this;
}


template <class T>
bool DeepPtr<T>::operator==(const DeepPtr& deep) const
{
    return *pointed == *(deep.pointed);
}


template <class T>
bool DeepPtr<T>::operator!=(const DeepPtr& deep) const
{
    return *pointed != *(deep.pointed);
}

#endif // DEEPPTR_HPP
