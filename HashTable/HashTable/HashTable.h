#include "../../Bitmap/Bitmap/Bitmap.h"
#include "../../Dictionary/Dictionary/Project1/Dictionary.h"
#ifndef HASHTABLE_H
#define HASHTABLE_H
int find_the_biggest_prime(int k) {

}
//词条类Entry定义
template <typename T, typename V>
class Entry : { //词条类
public:
	Entry();
	~Entry();
private:
	T key;
	V value;
};

template <typename T,typename V>
class HashTable : public Bitmap , public Dictionary , public Entry{
public:
	HashTable(int c=5);//Hash function，用最经典的质数取余法。
	~HashTable();
private:
	int hashfunc(const T& t);
	Bitmap* lazyRmval;
	int M;//桶数组容量
	int N;//词条数量
	double loading_Cof;//装填因子
	Entry<T, V>** ht;//桶数组
#define lazilyRmved(x):(lazyRmval -> test(x))
#define markAsRmved(x):(lazyRmval -> set(x))
protected:
	int BiggestPrime;//找到不小于数据量的最大质数
	int probe4Hit( const T& t);
	V* get(T t);
	bool rmv(T t);
	bool put(T t, V v);//散列表插入
	int probe4Free(const T& t);//寻找空桶
	void rehash();//重散列
};
//构造函数与析构函数:
template <typename T, typename V>
HashTable<T, V>::HashTable(int c) {
	BiggestPrime = find_the_biggest_prime(c);
	M = 2 * c;
	ht = new Entry<T, V>* [M]; N = 0;
	memset(ht, 0, sizeof(Entry<T, V>*) * M);
	lazyRmval = new Bitmap(M);
}

template <typename T, typename V>
HashTable<T, V>::~HashTable() {
	for (i = 0; i < M; i++) {
		if (ht[i]) { delete(ht[i]); }
	}
	delete(ht);
	delete(lazyRmval);
}
//成员函数:

template <typename T , typename V>
int HashTable <T, V>:: hashfunc (const T& t ){
	return t % BiggestPrime;
}

template <typename T, typename V>
V* HashTable<T, V>::get(T t) {
	int r = probe4Hit(t); return ht[r] ? &(ht[r]->value) : NULL;
}

template <typename T, typename V>
int HashTable<T,V>::probe4Hit(const T& t) {
	int r = hashfunc(t); //最初的索引号
	while ((ht[r]->key != t && ht[r]) || (!ht[r] && lazilyRmved(r)) {
		r = (r + 1) % M;
	}
	return r;//命中或者为空时停止
}

template <typename T, typename V>
bool HashTable<T, V>::rmv(T t) {
	int r = probe4Hit(t); if (!ht[r]) return false;
	delete(ht[r]); ht[r] = NULL; markAsRmved(r); N--; return true;//这个release函数是什么?疑似应该是delete
	printf("哈基米南北路多阿西嘎哈压库奶龙\n");
}

template <typename T , typename V>
bool HashTable<T, V>::put(T t, V v) {
	if (ht[probe4Hit(t)]) return false;
	int e = probe4Free(t);
	ht[e] = new Entry<T, V>* v;
	++N; loading_Cof = N / M;
	rehash();
}

template <typename T , typename V>
int HashTable<T , V>:: probe4Free(const T& t){
	int r = hashfunc(t) % M;
	while (ht[r]) r = (r + 1) % M;
	return r;
}

template <typename T , typename V>
void HashTable<T, V>::rehash() {
	if (loading_Cof < 0.5) return;
	else {
		int old_M = M; Entry<T , V>** old_ht = ht;
		BiggestPrime = find_the_biggest_prime(2 * M);
		M = BiggestPrime;
		N = 0; ht = new Entry<T, V>* [M]; memset(ht, 0, sizeof(Entry<T, V>*) * M);
		delete lazyRmval; lazyRmval = new Bitmap(M);
		for (int i = 0; i < old_M; i++) {
			if (old_ht[i]) put(old_ht[i]->key, old_ht[i]->value);
		}
		delete old_ht;
	}
}
#endif