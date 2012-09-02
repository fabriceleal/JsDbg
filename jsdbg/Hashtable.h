#include <string>

using namespace std;

#include "HashItem.h"

template <typename T> class Hashtable {
private:
	long p_size, p_items;
	HashItem<T> *p_table;

public:

	long GetSize(){ return p_size; }
	long GetItemsCount(){ return p_items; }

	Hashtable(long size) : p_size(0), p_items(0) { 
		if(size > 0){
			size = GetNextPrime(size);
			p_table = new HashItem<T>[size];
		}
	}

	~Hashtable(){ 
		if(p_table != NULL){
			delete[] p_table;
			p_table = NULL;
		}
	}

private:
	
	bool IsPrime(long val){
		for(long i = 2; i*i <= val; ++i){
			if((val % i) == 0)
				return false;
		}
		return true;
	}


	long GetNextPrime(long val){
		for(; ; ++val){
			if(IsPrime(val))
				break;
		}
		return val;
	}

public:

	// Dummy hash functions

	long HashFunction(long key){		
		return key % p_size;
	}

	long HashFunction(string &str){
		long hash = 0;
		for(long i = 0; i < (long)str.size(); ++i){
			int val = (long) str[i];
			hash = (hash * 256 + val) % p_size;
		}
		return hash;
	}

	// ***
	
	bool Insert(long key, T &obj){
		if(p_items == p_size)
			return false;

		long hash = HashFunction(key);

		while(p_table[hash].GetKey() != -1){
			hash ++;
			hash %= p_size;
		}

		p_table[hash].SetKey(hash);
		p_table[hash].SetObject(obj);

		p_items++;

		return true;
	}


	void Delete(long key){
		long hash = HashFunction(key);
		long orig = hash;

		while(p_table[hash].GetKey() != -1){
			if(p_table[hash].GetKey() == key){
				p_table[hash].SetKey(-1);
				items--;

				return;
			}

			hash++;
			hash %= p_size;

			if(orig == hash)
				return;
		}
	}

	void Find(long key, T *obj){
		long hash = HashFunction(key);
		long orig = hash;

		while(p_table[hash].GetKey() != -1){
			if(p_table[hash].GetKey() == key){
				if(obj != NULL)
					*obj = p_table[hash].GetObject();

				return true;
			}

			hash++;
			hash %= p_size;

			if(orig == hash)
				return false;
		}

		return false;
	}

};