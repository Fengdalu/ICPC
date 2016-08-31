// 边表存在 edge 里 , n 为点数 ,r 为源 , 全部为 1-based
// realdom[u] 为 dominator tree 中 u 的 father , 根或不能访问到的节点的 realdom 为 -1
int n,m,r;
int parent[maxn],label[maxn],cnt,real[maxn];
vector<int> edge[maxn],succ[maxn],pred[maxn];
int semi[maxn],idom[maxn],ancestor[maxn],best[maxn];
vector<int> bucket[maxn];
int realdom[maxn];
void dfs(int u) {
	label[u]=++cnt; real[cnt]=u;
	for(vector<int>::iterator it=edge[u].begin();it!=edge[u].end();++it) {
		int v=*it;if(v==parent[u] || label[v]!=-1) continue;
		parent[v]=u; dfs(v);
	}
}
void link(int v,int w) { ancestor[w]=v; }
void compress(int v) {
	int a=ancestor[v];
	if(ancestor[a]==0) return;
	compress(a);
	if(semi[best[v]]>semi[best[a]]) best[v]=best[a];
	ancestor[v]=ancestor[a];
}

int eval(int v) {
	if(ancestor[v]==0) return v;
	compress(v); return best[v];
}

void dominator() { // clear succ & pred & parent[r],let cnt=0 first
	cnt=0;
	for(int i=1;i<=n;++i){ succ[i].clear(), pred[i].clear(); }
	for(int i=1;i<=n;++i) label[i]=-1;
	parent[r]=-1; dfs(r);// r is root
	for(int u=1;u<=n;++u) {
		for(vector<int>::iterator it=edge[u].begin();it!=edge[u].end();++it) {
			int v=*it;
			if(label[u]!=-1&&label[v]!=-1) {
				succ[label[u]].push_back(label[v]);
				pred[label[v]].push_back(label[u]);
			}
		}
	}
	for(int i=1;i<=n;++i)
		semi[i]=best[i]=i, idom[i]=ancestor[i]=0, bucket[i].clear();
	for(int w=cnt;w >= 2;--w) {
		int p=label[parent[real[w]]];
		for(vector<int>::iterator it=pred[w].begin();it!=pred[w].end();++it) {
			int v=*it, u=eval(v);
			if(semi[w]>semi[u]) semi[w]=semi[u];
		}
		bucket[semi[w]].push_back(w); link(p,w);
		for(int i=0;i<bucket[p].size();++i) {
			int v=bucket[p][i], u=eval(v);
			idom[v]=(semi[u]<p?u:p);
		}
		bucket[p].clear();
	}
	for(int w=2;w<=cnt;++w) {
		if(idom[w]!=semi[w]) idom[w]=idom[idom[w]];
	}
	idom[1]=0;
	for(int i=1;i<=n;++i) realdom[i]=-1;
	for(int i=2;i<=cnt;++i) {
		int u=real[idom[i]],v=real[i];
		// u is immediate dominator of v(i==1?)
		realdom[v]=u;
	}
}
