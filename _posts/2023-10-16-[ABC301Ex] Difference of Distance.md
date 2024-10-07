---
layout: post
title: [ABC301Ex] Difference of Distance
subtitle: 
tags: 边双
---

### [AT_abc301_h [ABC301Ex] Difference of Distance](https://www.luogu.com.cn/problem/AT_abc301_h)

### [更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17766886.html)

一道基础图论，很好口胡，但是实现不太简单。

考虑离线，把询问挂在边上，按边权从小到大处理。

处理到一个边权时，把边权小于它的边的两端用并查集合并，对于等于这个边权的边在并查集上建图，跑一边 tarjan，因为问的是边，所以把边双锁点。

对于当前的边权，一个询问的答案是 $1$ 当且仅当它的两个点属于不同的边双并且两个边双的树上路径上有这条边，可以转化一下，设 $x,y$ 为两个边双的编号，$(l,r)$ 为询问的这条边，上面的限制等价于 $dis(x,l)+dis(y,r)+1=dis(x,y)$ 或 $dis(x,r)+dis(y,l)+1=dis(x,y)$，树剖或者倍增都可以。

实现的时候要小心一些，否则复杂度就假了。我们只能对属于当前边权的边的两个端点进行 tarjan，需要开数组记录，这样对于每一条边会遍历它两端的端点，复杂度是线性的。建树和建图的时候只把需要的节点的数据清空。复杂度为 $\mathcal O(n\log n)$。

```cpp
int n,m,q,len=0,num,color,cnt,tot,top[200001],dep[200001],fa[200001][21],col[200001],head[200001],to[400001],nex[400001],vis[200001],dfn[200001],low[200001],b[200001],ans[200001];
inline void add(int x,int y){to[++cnt]=y,nex[cnt]=head[x],head[x]=cnt;}
namespace DSU
{
	int fat[200001];
	int find(int x){return x==fat[x]?x:fat[x]=find(fat[x]);}
}
using namespace DSU;
struct Node{int x,y,z;};
struct Edge{int x,y,z;vector<Node> q;}a[200001];
bool cmp(Edge e1,Edge e2){return e1.z<e2.z;}
stack<int> st;
vector<int> T[200001];
void tarjan(int k,int from)
{
	dfn[k]=low[k]=++num,st.e(k);
	for(int i=head[k];i;i=nex[i])
	{
		if(i==(from^1))continue;
		if(!dfn[to[i]])tarjan(to[i],i),low[k]=min(low[k],low[to[i]]);
		else low[k]=min(low[k],dfn[to[i]]);
	}
	if(dfn[k]==low[k])
	{
		int y;dep[++color]=0;
		do col[y=st.top()]=color,st.pop();while(y!=k);
	}
}
void dfs(int k,int fat)
{
	dep[k]=dep[fat]+1,top[k]=top[fat],fa[k][0]=fat;
	for(int i=1;i<=20;++i)fa[k][i]=fa[fa[k][i-1]][i-1];
	for(auto to:T[k])if(to!=fat)dfs(to,k);
}
inline int LCA(int x,int y)
{
	if(dep[x]>dep[y])swap(x,y);
	for(int i=20;i>=0;--i)if(dep[fa[y][i]]>=dep[x])y=fa[y][i];
	for(int i=20;i>=0;--i)if(fa[x][i]!=fa[y][i])x=fa[x][i],y=fa[y][i];
	return x==y?x:fa[x][0];
}
inline int dis(int x,int y){return dep[x]+dep[y]-2*dep[LCA(x,y)];}
inline void mian()
{
	read(n,m);int x,y,z;
	for(int i=1;i<=m;++i)read(a[i].x,a[i].y,a[i].z);
	read(q);
	for(int i=1;i<=q;++i)read(z,x,y),a[z].q.eb((Node){x,y,i});
	sort(a+1,a+1+m,cmp);
	for(int i=1;i<=n;++i)fat[i]=i;
	for(int i=1,j=0;i<=m;i=j+1)
	{
		tot=0;
		while(a[i+1].z==a[i].z)++i;
		for(int k=j+1;k<=i;++k)
		{
			int x=find(a[k].x),y=find(a[k].y);
			if(x!=y)
			{
				if(!vis[x])vis[x]=1,dfn[x]=0,head[b[++tot]=x]=0;
				if(!vis[y])vis[y]=1,dfn[y]=0,head[b[++tot]=y]=0;
			}
		}
		cnt=1,num=color=0;
		for(int k=j+1;k<=i;++k)
		{
			int x=find(a[k].x),y=find(a[k].y);
			if(x!=y)add(x,y),add(y,x);
		}
		for(int k=1;k<=tot;++k){vis[b[k]]=0;if(!dfn[b[k]])tarjan(b[k],0);}
		for(int k=1;k<=color;++k)T[k].clear();
		for(int k=j+1;k<=i;++k)
		{
			int l=col[find(a[k].x)],r=col[find(a[k].y)];
			if(l!=r)T[l].eb(r),T[r].eb(l);
		}
		for(int k=1;k<=tot;++k)if(!dep[col[b[k]]])top[0]=col[b[k]],dfs(col[b[k]],0);
		for(int k=j+1;k<=i;++k)
		{
			int l=col[find(a[k].x)],r=col[find(a[k].y)];
			if(l==r)continue;
			for(auto [x,y,z]:a[k].q)
			{
				int xx=find(x),yy=find(y);
				if(!col[xx]||!col[yy]||xx==yy||top[col[xx]]!=top[col[yy]])continue;
				int d=dis(col[xx],col[yy]);
				if(dis(col[xx],l)+dis(col[yy],r)+1==d||dis(col[xx],r)+dis(col[yy],l)+1==d)
				ans[z]=1;
			}
		}
		for(int k=1;k<=tot;++k)col[b[k]]=0;
		while(j<m&&a[j+1].z<=a[i].z)
		++j,find(a[j].x)==find(a[j].y)?0:fat[find(a[j].x)]=find(a[j].y);
	}
	for(int i=1;i<=q;++i)write(ans[i],'\n');
}
```