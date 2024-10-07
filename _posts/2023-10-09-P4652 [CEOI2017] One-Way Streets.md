---
layout: post
title: P4652 [CEOI2017] One-Way Streets
subtitle: 题解
tags: 题解 边双 Luogu
show: true
---

[P4652 [CEOI2017] One-Way Streets](https://www.luogu.com.cn/problem/P4652)

基础图论。

题目中是关于无向图边方向的问题，而边双有一个优秀的性质：边双内的任意两点间至少有两条不经过同样的边的路径，因此对于边双内的边无论有没有题目中 $x$ 能走到 $y$ 的限制，它的方向都是不能确定的，因此首先边双缩点把问题转化为树上问题。

对于限制条件，从 $x$ 到 $y$ 的树上路径是唯一的，所以把这些边的方向确定即可。

由于每条边覆盖一次之后就没用了，所以可以使用并查集维护，每次把 $x$ 到 $y$ 路径上的点合并成一个集合，这样总复杂度是 $\mathcal O(n\log n)$ 的，使用按秩合并可以做到 $\mathcal O(n\alpha(n))$。

```cpp
int n,m,q,cnt=1,tot,num,upi[100001],dep[100001],fa[100001],up[100001],chose[100001],c[100001],id[200001],head[100001],dfn[100001],low[100001],to[200001],nex[200001];
int find(int x){return x==fa[x]?x:fa[x]=find(fa[x]);}
inline void add(int x,int y){to[++cnt]=y,nex[cnt]=head[x],head[x]=cnt;}
stack<int> st;
pii a[100001];
void tarjan(int k,int from)
{
	st.e(k),low[k]=dfn[k]=++tot;
	for(int i=head[k];i;i=nex[i])
	{
		if(i==(from^1))continue;
		if(!dfn[to[i]])tarjan(to[i],i),low[k]=min(low[k],low[to[i]]);
		else low[k]=min(low[k],dfn[to[i]]);
	}
	if(dfn[k]==low[k])
	{
		int y;++num;
		do c[y=st.top()]=num,st.pop();while(y!=k);
	}
}
void dfs(int k,int fat)
{
	up[k]=fat,dep[k]=dep[fat]+1,fa[k]=k;
	for(int i=head[k];i;i=nex[i])
	{
		if(to[i]==fat){upi[k]=id[i];continue;}
		dfs(to[i],k);
	}
}
inline void mian()
{
	read(n,m);int x,y;
	for(int i=1;i<=m;++i)read(a[i].fi,a[i].se),add(a[i].fi,a[i].se),add(a[i].se,a[i].fi);
	for(int i=1;i<=n;++i)if(!dfn[i])tarjan(i,0);
	memset(head,0,sizeof(head)),cnt=1;
	for(int i=1;i<=m;++i)if(c[a[i].fi]!=c[a[i].se])add(c[a[i].fi],c[a[i].se]),add(c[a[i].se],c[a[i].fi]),id[cnt^1]=i,id[cnt]=-i;
	for(int i=1;i<=num;++i)if(!fa[i])dfs(i,0);
	read(q);
	while(q--)
	{
		read(x,y),x=c[x],y=c[y];
		while(1)
		{
			x=find(x),y=find(y);
			if(x==y)break;
			if(dep[x]>dep[y])
			{
				if(upi[x]>0)chose[upi[x]]=1;else chose[-upi[x]]=-1;
				fa[x]=up[x];
			}
			else
			{
				if(upi[y]>0)chose[upi[y]]=-1;else chose[-upi[y]]=1;
				fa[y]=up[y];
			}
		}
	}
	for(int i=1;i<=m;++i)if(chose[i]==1)putchar('R');else if(chose[i]==-1)putchar('L');else putchar('B');
}

```