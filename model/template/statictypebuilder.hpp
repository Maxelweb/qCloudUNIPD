#ifndef STATICTYPEBUILDER_HPP
#define STATICTYPEBUILDER_HPP

#include <map>
#include <QString>

class Server;


// Template per la creazione della mappa statica dei tipi della gerarchia.
// Questo template è creato ad hoc per la gerarchia Server al fine di aumentarne l'estendibilità del codice
// e per implementare il polimorfismo nella serializzazione e unserializzazione dei dati.

// Nel caso si voglia aggiungere una classe derivata sarà necessario dichiarare e inizializzare
// una variabile statica e privata usando il corrente template e implementare il metodo QString staticType() const.


template<class T>
class StaticTypeBuilder
{


    private:
        T * pointer;
    public:
        StaticTypeBuilder();
        ~StaticTypeBuilder();
};

template<class T>
StaticTypeBuilder<T>::StaticTypeBuilder()
{
    pointer = new T();
    if(T::itemsMap == nullptr)
        T::itemsMap = new std::map<QString, Server*>();

    T::itemsMap->insert({pointer->staticType(), pointer});
}


template<class T>
StaticTypeBuilder<T>::~StaticTypeBuilder()
{
    delete pointer;
}

#endif // STATICTYPEBUILDER_HPP
