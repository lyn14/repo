//写在代码之前的话：
/***切记在启动游戏前完全退出Visual Studio***/

#include <iostream>
#include <queue>
typedef enum { UNDISCOVERED, DISCOVERED, VISITED }VStatus; //顶点的被访问状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } Etype; //边的类型

template <class Tv, class Te >
class Graph {
private:
	VStatus vs;
	Etype et;
	void reset() {
		for (int i = 0; i < n; i++) {
			status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1;
			parent(i) = -1; priority(i) = INT_MAX;

		}
	}
public:
	int n; //顶点总数
	virtual Tv insert(Tv const&) = 0;
	virtual Tv& remove(int) = 0;
	virtual VStatus& status(int) = 0;
	virtual Etype& type(int, int) = 0;
	void DFS(int v, int& clock);
	void BFS(int v, int& clock);
	void dfs(int v);
	void bfs(int v);//分别为DFS与BFS两个函数的上层调用者
	virtual int firstNbr(int i) = 0;
	virtual int nextNbr(int i, int j) = 0;
	virtual int& parent(int) = 0;
};

template <typename Tv, typename Te >
void Graph <Tv, Te > ::DFS(int v, int& clock) {
	status(v) = UNDISCOVERED;
	for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
		switch (status(u))
			case UNDISCOVERED:
				type(v, u) = TREE; parent(u) = v; DFS(u); break;
			case DISCOVERED:
				type(v, u) = BACKWARD; break;
			case VISITED:
				break;
}

template <typename Tv, typename Te >
void Graph <Tv, Te> ::BFS(int v, int& clock) {
	Queue<int> q;
	status(v) = DISCOVERED; q.enqueue(v); clock++;
	while (!q) {
		int temp = q.dequeue();
		for (int u = firstNbr(temp); u > -1; u = nextNbr(temp, u))
			if (status(u) == UNDISCOVERED) {
				status(u) = DISCOVERED; q.enqueue(u); clock++;
				type(v, u) = TREE; parent(u) = v; //引入辅助树，在该树中，v是u的祖先（父节点）
			}
			else //即u 已经被访问完毕或已被发现
				type(v, u) = CROSS;
	}

}

template <typename Tv, typename Te>
void Graph<Tv, Te> ::dfs(int v) {
	reset(); int clock = 0; int u = v;
	do
	{
		if (status(v) == UNDISCOVERED) {
			DFS(v, clock);
		}
		v = ++v % n;
	} while (u != v);
}

template <typename Tv, typename Te>
void Graph<Tv, Te> ::bfs(int v) {
	reset(); int clock = 0; int u = v;
	do
	{
		if (status(v) == UNDISCOVERED) {
			BFS(v, clock);
		}
		v = ++v % n;
	} while (u != v);
}

template <typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te>{
public:

};