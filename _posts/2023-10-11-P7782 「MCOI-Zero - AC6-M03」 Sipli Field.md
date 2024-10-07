---
layout: post
title: P7782 「MCOI-Zero - AC6-M03」 Sipli Field
subtitle: 题解
tags: 题解 点分治 Luogu
show: true
---

[P7782 「MCOI-Zero / AC6-M03」 Sipli Field](https://www.luogu.com.cn/problem/P7782)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17756232.html)

单 log 淀粉做法。

回想正常淀粉计算的是树上的路径问题，但题目中要求计算经过每个点的答案，这样我们选取重心后一棵子树对另一棵子树的答案就会少算，所以我们淀粉时不仅要算根的答案，也要考虑子树间的相互贡献。

首先以根为重心 dfs 一遍统计出 $f_i$ 表示深度为 $i$ 的点有多少个，然后直接扫一遍计算根的答案，可以用前缀和优化到 $\mathcal O(n)$。

但是这样会多算两个端点都在同一子树内的答案，所以接下来对于每棵子树做同样的事减去多余的贡献。

对于子树间的答案，一个深度为 $i$ 的点，它的贡献 $d_i=\sum_{j=L-i}^{R-i}f_j$，这个过程也可以用前缀和优化。注意到一个儿子的路径一定也经过它的父亲，所以 $d_i=d_i+\sum_{j\in son_i}d_j$，最后一遍树上差分统计即可。

常数巨大，dfs 了 114514 次，甚至被 $\mathcal O(n\log^2 n)$ 吊着打/kk。

```cpp
int s2[2000001],f[2000001],g[2000001],sum[1000001],ans[1000001],vis[1000001],siz[1000001],head[1000001],nex[2000001],to[2000001];
int dt,cnt,n,root,col,all,tot,L,R;
#define ca(d) (d<0?0:s2[d])
inline void add(int x,int y){to[++cnt]=y,nex[cnt]=head[x],head[x]=cnt;}
void find(int k,int fa)
{
	siz[k]=1;int maxn=0;
	for(int i=head[k];i;i=nex[i])
	{
		if(to[i]==fa||vis[to[i]])continue;
		find(to[i],k),siz[k]+=siz[to[i]],maxn=max(maxn,siz[to[i]]);
	}
	if(max(maxn,all-siz[k])<=all/2)root=k;
}
void dfs1(int k,int fa,int depth,int v)
{
	f[depth]+=v,dt=max(dt,depth);
	for(int i=head[k];i;i=nex[i])if(to[i]!=fa&&!vis[to[i]])dfs1(to[i],k,depth+1,v);
}
void dfs2(int k,int fa,int depth,int v)
{
	sum[k]+=(ca(min(dt,R-depth))-ca(min(dt,L-depth-1)))*v;
	for(int i=head[k];i;i=nex[i])if(to[i]!=fa&&!vis[to[i]])dfs2(to[i],k,depth+1,v);
}
void dfs3(int k,int fa,int depth,int v)
{
	g[depth]+=v,dt=max(dt,depth);
	for(int i=head[k];i;i=nex[i])if(to[i]!=fa&&!vis[to[i]])dfs3(to[i],k,depth+1,v);
}
void up(int k,int fa){for(int i=head[k];i;i=nex[i])if(to[i]!=fa&&!vis[to[i]])up(to[i],k),sum[k]+=sum[to[i]];}
void modify(int k,int fa)
{
	ans[k]+=sum[k],sum[k]=0;
	for(int i=head[k];i;i=nex[i])if(to[i]!=fa&&!vis[to[i]])modify(to[i],k);
}
void getsiz(int k,int fa)
{
	siz[k]=1;
	for(int i=head[k];i;i=nex[i])if(!vis[to[i]]&&to[i]!=fa)getsiz(to[i],k),siz[k]+=siz[to[i]];
}
void starch(int k,int fa)
{
	find(k,fa),k=root,getsiz(k,fa),vis[k]=1,ans[k]*=2;
	dt=0,dfs1(k,fa,0,1),s2[0]=f[0];
	for(int i=1;i<=dt;++i)s2[i]=s2[i-1]+f[i];
	for(int i=0;i<=dt;++i)ans[k]+=f[i]*(ca(min(dt,R-i))-ca(min(dt,L-i-1)));
	for(int i=head[k];i;i=nex[i])if(to[i]!=fa&&!vis[to[i]])dfs2(to[i],k,1,1);
	dfs1(k,fa,0,-1);
	for(int i=head[k];i;i=nex[i])
	{
		if(to[i]==fa||vis[to[i]])continue;
		dt=0,dfs3(to[i],k,1,1),s2[0]=g[0];
		for(int i=1;i<=dt;++i)s2[i]=s2[i-1]+g[i];
		for(int i=1;i<=dt;++i)ans[k]-=g[i]*(ca(min(dt,R-i))-ca(min(dt,L-i-1)));
		dfs3(to[i],k,1,-1);
		dfs2(to[i],k,1,-1);
		up(to[i],k),modify(to[i],k);
	}
	ans[k]/=2;int kk=k;
	for(int i=head[kk];i;i=nex[i])if(!vis[to[i]]&&to[i]!=fa)all=siz[to[i]],starch(to[i],kk);
}
void mian()
{
	read(n,L,R);int x;
	for(int i=2;i<=n;++i)read(x),add(x,i),add(i,x);
	all=n,starch(1,0);
	for(int i=1;i<=n;++i)write(ans[i],'\n');
}
```