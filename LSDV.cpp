#include<iostream>
#include<vector>
#include<algorithm>
#include<list>

using namespace std;

//最小生成树
struct Edge
{
	Edge(int start, int end, int value) :start(start), end(end), value(value) {}
	int start;
	int end;
	int value;
};

//Kruskal
vector<int> father;

int getfather(int pos) {
	if (father[pos] == pos)return pos;
	father[pos] = getfather(father[pos]);
	return father[pos];
}

//n为点个数
int Kruskal(vector<Edge>& edges,int n) {
	sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
		return a.value < b.value;
		});
	for (int i = 0; i < n; i++) {
		father.push_back(i);
	}
	int count = 0;
	for (auto& p : edges) {
		int fa_st = getfather(p.start), fa_ed = getfather(p.end);
		if (fa_st != fa_ed) {
			father[fa_ed] = fa_st;
			count += p.value;
		}
	}
	return count;
}

//Prime
int Prime(vector<Edge>& edges, int n) {
	vector<list<pair<int, int>>>myedges(n);//first->point,second->value
	for (auto& p : edges) {
		myedges[p.start].emplace_back(p.end, p.value);
		myedges[p.end].emplace_back(p.start, p.value);
	}
	vector<bool>visit(n, false);
	visit[0] = true;
	int count = 0;
	for (int i = 1; i < n; i++) {
		int cur_min = INT_MAX;
		int next = 0;
		for (int j = 0; j < n; j++) {
			if (visit[j]) {
				for (auto& p : myedges[j]) {
					if (!visit[p.first]) {
						if (p.second < cur_min) {
							cur_min = p.second;
							next = p.first;
						}
					}
				}
			}
		}
		visit[next] = true;
		count += cur_min;
	}
	
	return count;
}

//最短路径

//Dijkstra(LS)

//n为点个数,begin为起点
vector<int> Dijkstra(vector<Edge>& edges, int n,int begin) {
	vector<int>dist(n, -1);
	vector<int>path(n, -1);
	vector<bool>visit(n, false);
	dist[begin] = 0;
	visit[begin] = true;
	vector<list<pair<int, int>>>myedges(n);//first->point,second->value
	for (auto& p : edges) {
		myedges[p.start].emplace_back(p.end, p.value);
		myedges[p.end].emplace_back(p.start, p.value);
	}
	for (auto& p : myedges[begin]) {
		dist[p.first] = p.second;
		path[p.first] = begin;
	}
	for (int i = 1; i < n; i++) {
		int now_min = INT_MAX;
		int cur = -1;
		for (int j = 0; j < n; j++) {
			if (!visit[j] && dist[j] != -1) {
				if (dist[j] < now_min) {
					now_min = dist[j];
					cur = j;
				}
			}
		}
		visit[cur] = true;
		for (auto&p : myedges[cur]) {
			if (dist[p.first] == -1) { 
				dist[p.first] = dist[cur] + p.second;
				path[p.first] = cur;
			}
			else {
				if (dist[p.first] > dist[cur] + p.second) {
					dist[p.first] = dist[cur] + p.second;
					path[p.first] = cur;
				}
			}
		}
	}
	return path;
}

//Bellman-Ford(DV)

vector<vector<pair<int, int>>> BellmanFord(vector<Edge>& edges, int n) {
	vector<vector<pair<int, int>>>mygraph(n, vector<pair<int,int>>(n, {-1,-1}));//first->value,second->next
	for (auto& p : edges) {
		mygraph[p.start][p.end].first = p.value;
		mygraph[p.end][p.start].first = p.value;
	}
	bool flag = true;
	while (flag) {
		flag = false;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {//第i个点的路由表
				if (i == j)continue;
				for (int k = 0; k < n; k++) {//判断能否从其他可达的节点访问j
					if (k == j)continue;
					if (mygraph[i][k].first != -1 && mygraph[k][j].first != -1) {
						if (mygraph[i][j].first == -1) {
							mygraph[i][j].first = mygraph[i][k].first + mygraph[k][j].first;
							mygraph[i][j].second = k;//下一跳
							flag = true;
						}
						else {
							if (mygraph[i][j].first > mygraph[i][k].first + mygraph[k][j].first) {
								mygraph[i][j].first = mygraph[i][k].first + mygraph[k][j].first;
								mygraph[i][j].second = k;//下一跳
								flag = true;
							}
						}
					}
				}
			}
		}
	}
	return mygraph;
}
