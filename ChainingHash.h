/*
 *  Separate chaining hashtable
 */

#ifndef __CHAINING_HASH_H
#define __CHAINING_HASH_H

// Standard library includes
#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include <algorithm>
#include <functional>


// Custom project includes
#include "Hash.h"

// Namespaces to include
using std::vector;
using std::list;
using std::pair;

using std::cout;
using std::endl;

//
// Separate chaining based hash table - derived from Hash
//
template<typename K, typename V>
class ChainingHash : public Hash<K,V> {
private:

    std::vector<std::list<std::pair<K,V>>> hashTable;
    int currentSize;
  


public:
    ChainingHash(int n = 11) {
        hashTable.resize(findNextPrime(n));
        currentSize = 0;
    }


    ~ChainingHash() {
        this->clear();
    }

    int size() {
        return this->currentSize;
    }

    V operator[](const K& key);


    bool insert(const std::pair<K, V>& pair);
    bool insert(std::pair<K,V> &&pair);

    void erase(const K& key);

    void clear() {
        for( auto & thisList : hashTable)
            thisList.clear();   
    }

    int bucket_count();


    float load_factor();

    bool contains(const K& key) const;
    bool match(const std::pair<K,V> &pair) const;




private:
    int findNextPrime(int n)
    {
        while (!isPrime(n))
        {
            n++;
        }
        return n;
    }

    int isPrime(int n)
    {
        for (int i = 2; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    int hash(const K& key){
        return key % currentSize;
    }

    void rehash();

    size_t myhash(const K& key) const;


};

template<typename K, typename V>
float ChainingHash<K,V>::load_factor(){
    // returns the floats of the current size of how many elements are in the table to the amount of elements
    return (float(this->currentSize)/ float(this->bucket_count()));
}

template<typename K, typename V>
int ChainingHash<K,V>::bucket_count(){
    //returns total size allocated
    return this->hashTable.size();
}


template<typename K, typename V>
V ChainingHash<K,V>::operator[](const K& key){
    //searches the table from beginning to end for a key and returns the value
    for(auto iterator = hashTable[myhash(key)].begin(); iterator != hashTable[myhash(key)].end(); iterator++){

        if(iterator->first == key) return iterator->second;

    }

    std::cout << "Error in finding key value pair" << std::endl;
    return 0;

}

template<typename K, typename V>
size_t ChainingHash<K,V>::myhash(const K& key) const{
    //hash function 
    static std::hash<K> hf;
    return hf(key) % hashTable.size();

}


//iterator over linked list
template<typename K, typename V>
bool ChainingHash<K,V>::contains(const K& key) const{
    for(auto iterator = hashTable[myhash(key)].begin(); iterator != hashTable[myhash(key)].end(); iterator++){

        if(iterator->first == key) return true;

    }
    return false;
}

//iterating over the vector

template<typename K, typename V>
bool ChainingHash<K,V>::match(const std::pair<K,V> &pair) const{

    auto & whichList = hashTable[myhash(pair.first)];
    return std::find(whichList.begin(), whichList.end(), pair) != whichList.end();

}

//function to move the pair
template<typename K, typename V>
bool ChainingHash<K,V>::insert(std::pair<K,V> && pair){
	const std::pair<K,V> moved = std::move(pair);
	return insert(moved); 
}

//actual insert function that calculates if needing rehash
template<typename K, typename V>
bool ChainingHash<K,V>::insert(const std::pair<K, V>& pair){
    if(match(pair)) return false;

    else if(contains(pair.first)){
        auto & whichList = hashTable[myhash(pair.first)];
        for(auto iterator = whichList.begin(); iterator != whichList.end(); ++iterator){
            if(iterator->first == pair.first){
                auto iterator2 = whichList.erase(iterator);
                whichList.insert(iterator2, pair);
                
            }
        }
        return true;
    }

    else{
        
        hashTable[myhash(pair.first)].push_back(pair);
        
        //i.e if load factor is above .75 then rehash
        if((unsigned long)++currentSize > hashTable.size() / 2) rehash();
        return true;
    }
}

template<typename K, typename V>
void ChainingHash<K,V>::erase(const K& key){
    auto & whichList = hashTable[myhash(key)];
    //iterates through the table with the hash key used as whichlist 
    for(auto iterator = whichList.begin(); iterator != whichList.end(); ++iterator){
        if(iterator->first == key){
            iterator = whichList.erase(iterator);
            --currentSize;
        }
    }
}

template<typename K, typename V>
void ChainingHash<K,V>::rehash(){
    auto old = hashTable;
    //rehashes to next prime to promote better distribution
    hashTable.resize(findNextPrime(2 * hashTable.size()));

    for(auto & del : hashTable){
        del.clear();
    }

    currentSize = 0;
   
    
    for(auto & del : old){
        for(auto & pair : del){
            insert(std::move(pair));
        }
    }

}

#endif //__CHAINING_HASH_H

