---
layout: post
title: ARC083F Collecting Balls
subtitle: 题解
tags: 数数 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18375448)

### [[ARC083F] Collecting Balls](https://www.luogu.com.cn/problem/AT_arc083_d)

建图，连边 $(x_i,y_i+n)$，这样会形成一个基环树森林。对于基环树的每条边，需要把他归到他连接的两个点中任意一个，并且每个点只能拥有一条边。

对于每个基环树分别计算，树边归属的点一定是它两端深度较大的那个，环边归属点整体只有两种方式：顺时针和逆时针。确定了边挂在哪个点上之后，操作顺序的限制就是，如果边 $(x,y)$ 归到了点 $y$，则要求所有 $(y,z),z<x$ 的边需要在它之前操作。

把边看成点，再建一个图来表示拓扑关系：$(x,y)$ 表示 $x$ 需要在 $y$ 之前进行操作。不难证明这是一个森林：对于树边之间显然只有从下到上的边，环边上的边形成了若干条单向链，还有一些环边指向树边的边，所以一定是外向树森林。

然后是非常典的外向树拓扑序计数。对于点 $i$，自己需要放到最前面，然后儿子之间是随意排的，设 $f_i$ 表示 $i$ 子树内拓扑序数量，则：

$$
f_i=\prod f_{to}\frac{(siz_i-1)!}{\prod siz_{to}!}
$$

右边的分数是一个组合数系数，表示儿子之间可以任意排列。所以每个点的贡献恰好就是 $\frac {1}{siz_i}$。一个外向树森林的答案就是 $\frac{n!}{\prod siz_i}$。总复杂度 $\mathcal O(n)$。

```cpp
en,sum,sz,ans=1,b[200010],fr[200010],inv[200010];
vector<pii> T[200010];
vi G[200010],ve;
int vis[200010],deg[200010],siz[200010],Id[200010];
stack<int> st;
void dfs0(int x)
{
	vis[x]=1,sz+=2,sz-=T[x].size();
	for(auto [to,v]:T[x])
	{
		ve.eb(v);
		if(!vis[to])dfs0(to);
	}
}
void dfs(int x,int fa=0)
{
	st.e(x),vis[x]=2;
	for(auto [to,v]:T[x])if(to!=fa)
	{
		if(vis[to]==2)
		{
			while(st.top()!=to)b[++len]=st.top(),st.pop();
			b[++len]=to;
			return;
		}
		else dfs(to,x);
		if(len)return;
	}
	st.pop();
}
void dfs1(int x,int fa=0,int from=0)
{
	for(auto [to,v]:T[x])if(to!=fa&&vis[to]<3)
	{
		if(to<fa)G[from].eb(v),++deg[v];
		dfs1(to,x,v);
	}
}
void dfs2(int p)
{
	siz[p]=1;
	for(auto to:G[p])dfs2(to),siz[p]+=siz[to];
	Mmul(sum,inv[siz[p]]);
}
inline void mian()
{
	read(n),fr[0]=inv[0]=1;int x,y,val,S;
	for(int i=1;i<=2*n;++i)fr[i]=Cmul(fr[i-1],i);
	inv[2*n]=power(fr[2*n],MOD-2);
	S=fr[2*n];
	for(int i=2*n-1;i>0;--i)inv[i]=Cmul(inv[i+1],i+1);
	for(int i=1;i<=2*n;++i)Mmul(inv[i],fr[i-1]);
	for(int i=1;i<=2*n;++i)read(x,y),T[x].eb(mp(y+n,i)),T[y+n].eb(mp(x,i));
	for(int i=1;i<=2*n;++i)if(!vis[i])
	{
		len=sz=0,ve.clear(),dfs0(i),sort(ve.begin(),ve.end());
		ve.resize(unique(ve.begin(),ve.end())-ve.begin());
		if(sz!=0){puts("0");return;}
		Mmul(S,power(fr[ve.size()],MOD-2));
		dfs(i),b[len+1]=b[1],b[0]=b[len];
		for(int i=1;i<=len;++i)vis[b[i]]=3;
		for(int i=1;i<=len;++i)//right
		{
			int id=-1;
			for(auto [to,v]:T[b[i]])if(to==b[i-1]){id=v;break;}
			assert(id!=-1),dfs1(b[i],b[i-1],id),Id[i]=id;
		}
		Id[len+1]=Id[1];
		for(int i=0;i<len;++i)if(b[i+2]<b[i])
		G[Id[i+1]].eb(Id[i+2]),++deg[Id[i+2]];
		sum=fr[ve.size()];
		for(auto p:ve)if(!deg[p])dfs2(p);
		for(auto p:ve)G[p].clear(),deg[p]=0;
		val=sum,sum=fr[ve.size()];
		for(int i=1;i<=len;++i)
		{
			int id=-1;
			for(auto [to,v]:T[b[i]])if(to==b[i+1]){id=v;break;}
			assert(id!=-1),dfs1(b[i],b[i+1],id),Id[i]=id;
		}
		Id[0]=Id[len];
		for(int i=0;i<len;++i)if(b[i]<b[i+2])
		G[Id[i+1]].eb(Id[i]),++deg[Id[i]];
		for(auto p:ve)if(!deg[p])dfs2(p);
		Mmul(ans,Cadd(val,sum));
	}
	write(Cmul(S,ans));
}
```