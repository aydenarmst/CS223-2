#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

#include <vector>
#include <stdexcept>

#include "Hash.h"

using std::vector;
using std::pair;

// Can be used for tracking lazy deletion for each element in your table
enum EntryState {
    EMPTY = 0,
    VALID = 1,
    DELETED = 2
};

template<typename K, typename V>
class ProbingHash : public Hash<K,V> { // derived from Hash
private:
    // Needs a table and a size.
    // Table should be a vector of std::pairs for lazy deletion

    
//thought it would be easier to read and understand with an elemnt of the pairs and info for the entry state
    struct HashEntry{
        std::pair<K,V> element;
        EntryState info;

        HashEntry(const std::pair<K,V> &pair = std::pair<K,V>(), EntryState i = EMPTY) : element(pair), info{i} {}
        HashEntry(std::pair<K,V> &&pair, EntryState i = EMPTY) : element{ std::move(pair)}, info{i} {}
    };

    std::vector<HashEntry> hashTable;
    int currentSize;

public:
    ProbingHash(int n = 11) : hashTable(findNextPrime(n)){
        makeEmpty();
    }

    void makeEmpty(){
        currentSize = 0;
        for( auto & thisVector : hashTable){
            if(thisVector.info == VALID || thisVector.info == DELETED ){
                thisVector.info = DELETED;
            }
            thisVector.info = EMPTY;  
        }
    }

    ~ProbingHash(){
        this->hashTable.clear();
    }

    bool contains(const std::pair<K,V> &pair) const;

    int findPos(const K &key) const;

    bool isActive(int currentPos)const {
        return hashTable[currentPos].info == VALID;
    }

    bool isDeleted(int currentPos) const{
        return hashTable[currentPos].info == DELETED;
    }

    int size() {
        return this->currentSize;
    }

    V operator[](const K& key);

    bool insert(const std::pair<K, V>& pair);

    void erase(const K& key);

    void clear(){
        this->hashTable.erase(std::next(hashTable.begin()), std::next(hashTable.end()));

    }

    int bucket_count();

    float load_factor();

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

    int hash(const K& key) {
        return 0;       
    }

    size_t myhash(const K& key) const{
        static std::hash<K> hf;
        return hf(key) % hashTable.size();
    }
    
    void rehash();

    
};

template<typename K, typename V>
int ProbingHash<K,V>::bucket_count(){
    //returns total size allocated
    return this->hashTable.size();
}

template<typename K, typename V>
float ProbingHash<K,V>::load_factor(){
    // returns the floats of the current size of how many elements are in the table to the amount of elements
    return (float(this->currentSize)/ float(this->bucket_count()));
}


template<typename K, typename V>
V ProbingHash<K,V>::operator[](const K& key){
    //searches the table from beginning to end for a key and returns the value
    int iterator = 1;
    int currentPos = myhash(key);
        
    while(hashTable[currentPos].info != DELETED && hashTable[currentPos].element.first != key){
            currentPos += iterator;
            iterator++;

            if((unsigned long)currentPos >= hashTable.size()) 
                currentPos -= hashTable.size();
        }
        return hashTable[currentPos].element.first;
    std::cout << "Error in finding key value pair" << std::endl;
    return 0;
}


template<typename K, typename V>
bool ProbingHash<K,V>::contains(const std::pair<K,V> &pair) const{
        return isActive(findPos(pair.first));
    }




template<typename K, typename V>
int ProbingHash<K,V>::findPos(const K &key) const{
        int offset = 1;
        int currentPos = myhash(key);
        
        while(hashTable[currentPos].info != EMPTY && hashTable[currentPos].element.first != key){
            currentPos += offset;
            offset++;

            if((unsigned long)currentPos >= hashTable.size()) 
                currentPos -= hashTable.size();
        }
        return currentPos;
    }




template<typename K, typename V>
bool ProbingHash<K,V>::insert(const std::pair<K, V>& pair){
    int currentPos = findPos(pair.first);
    if(isActive(currentPos)) return false;

    hashTable[currentPos].element = pair;
    hashTable[currentPos].info = VALID;

    if((unsigned long)++currentSize > hashTable.size() / 2) rehash();
    return true;
}



template<typename K, typename V>
void ProbingHash<K,V>::erase(const K& key){
    int currentPos = findPos(key);

    if(!isActive(currentPos)){
        std::cout << "Cannot remove something thats not there" << std::endl;
    }

    --currentSize;
    hashTable[currentPos].info = DELETED;
}



template<typename K, typename V>
void ProbingHash<K,V>::rehash(){
     auto old = hashTable;

     hashTable.resize(findNextPrime(2 * hashTable.size()));

     for(auto & del : hashTable){
        del.info = EMPTY;
    }

    currentSize = 0;

    for(auto & entry : old){
        if(entry.info == VALID)
            insert(std::move(entry.element));
    }
}

#endif //__PROBING_HASH_H