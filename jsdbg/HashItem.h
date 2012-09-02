template <typename T> class HashItem {
private:
	long p_key;
	T p_obj;
public:
	HashItem() : p_key(0){ }
	~HashItem(){ }

	long GetKey(){ return p_key; }
	void SetKey(long key){ p_key = key; }

	T GetObject(){ return p_obj; }
	void SetObject(T obj) { p_obj = obj; }

	bool operator ==(HashItem &item){
		return (p_key == item.GetKey());
	}

	bool operator = (HashItem &item){
		p_key = item.GetKey();
		p_obj = item.GetObject();
	}

};