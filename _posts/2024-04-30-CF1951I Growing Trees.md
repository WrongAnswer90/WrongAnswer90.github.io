---
layout: post
title: CF1951I Growing Trees
subtitle: 题解
tags: 题解 网络流 贪心 CF
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18167978)

### [CF1951I Growing Trees](https://www.luogu.com.cn/problem/CF1951I)

首先考虑确定了 $x_i$ 如何判定是否合法。可以很容易的找出这样一个必要条件：$\forall i,x_i\leq k$。

这是两个点的情况，考虑点数更多的情况。手玩之后可以推广到：对于任意导出子图，要求其内部的边数 $\leq k(\lvert S\rvert-1)$。这个条件也是充分的，证明类似 $\mathrm{Hall}$ 定理。

可以先把二次函数差分，这样可以算出每**新加**一条边的代价，是一个一次函数。

每次贪心的选择当前边权最小的边把其 $x_i$ 加一，正确性应该可以用拟阵去证~~但是不会~~。

然后就需要判定是否仍然合法，$G(S)\leq k(\lvert S\rvert-1)$ 等价于 $G(S)-k\lvert S\rvert\leq -k$，是最大权闭合子图，选定一条边就需要选定其两端点，点的收益是 $-k$，边的收益是 $x_i$。

但是由于代价一定是负的，直接跑最大流肯定是把和源点连着的边全都切了，一定会跑出来 $0$。

所以必须要枚举一个点（原图中的）强制选进来然后跑最大流。这部分其实可能也可以用退流的方式，即每次退掉一个原图中的点对应的点连向 $T$ 的流量，但是这个题 $nm$ 数据范围都很小，暴力就能过。

现在问题在于 $k$ 非常大。考虑求出每条边最多加多少次，在全局做值域从小到大的做这个过程，每次需要找到接下来会爆的是哪条边。可以直接二分或倍增，即确定一个值，然后把当前所有没有爆的边权值小于等于当前值的边都加进来，即 $a_ix^2+b_ix-(a_i(x-1)^2+b_i(x-1))\leq mid$，然后做上述的判定。用这种方式找到最大的 $mid$ 满足其是合法中的最大值。

接下来会出现问题：可能有多条边的下一个权值是 $mid+1$，这样不能确定到底是哪条边爆了。

一个处理技巧是：先让所有边的权值都小于等于 $mid$。然后扫所有没有爆的边，把这条边的上界改成 $mid+1$ 再跑一遍判定，如果不合法，那这条边一定爆了，这条边的上界再回到 $mid$，同时求出了这条边的 $x_i$。否则这条边的上界就是 $mid+1$。注意上述过程每条边的 $mid$ 是不同的。

这样至少会有一条边爆掉。总复杂度是 $\mathcal O(m(\log V+m)nm^3)$，但是 $m^3$ 的流跑不满，可以通过。

```cpp
int T_,n,m,k,x[110];
ll L,R,mid,v[3010],ans;
int S,T,cnt,head[110],now[110],d[110],to[3010],nex[3010];
inline void Add(int x,int y,ll z){to[++cnt]=y,nex[cnt]=head[x],head[x]=cnt,v[cnt]=z;}
inline void add(int x,int y,ll z){Add(x,y,z),Add(y,x,0);}
queue<int> q;
inline bool bfs()
{
	while(!q.empty())q.pop();
	memset(d,0,sizeof(d)),q.e(S),d[S]=1,now[S]=head[S];
	while(!q.empty())
	{
		int nw=q.front();q.pop();
		for(int i=head[nw];i;i=nex[i])
		{
			if(!d[to[i]]&&v[i])
			{
				d[to[i]]=d[nw]+1,now[to[i]]=head[to[i]],q.e(to[i]);
				if(to[i]==T)return 1;
			}
		}
	}
	return 0;
}
ll dinic(int x,ll flow)
{
	if(x==T)return flow;
	ll rest=flow,t;
	for(int i=head[x];i&&rest;i=nex[i])
	{
		if(!v[i]||d[to[i]]!=d[x]+1)continue;
		t=dinic(to[i],min(v[i],rest)),rest-=t,v[i]-=t,v[i^1]+=t;
		if(!t)d[to[i]]=0;
	}
	return flow-rest;
}
struct Node{int x,y,z,t;}b[110];
bitset<110> vis,can;
inline bool chk(ll mid)
{
	ll maxn=-INF,v;
	for(int j=1;j<=n;++j)
	{
		memset(head,0,sizeof(head)),cnt=1,ans=-k;
		for(int i=1;i<=n;++i)if(i==j);else add(i,T,k);
		for(int i=1;i<=m;++i)
		{
			if(vis[i])add(S,i+n,x[i]),ans+=x[i];
			else
			{
				v=max(0ll,(mid+can[i]+b[i].z-b[i].t)/2/b[i].z);
				if(v>k)ans+=inf;
				add(S,i+n,v),ans+=v;
			}
			add(i+n,b[i].x,INF),add(i+n,b[i].y,INF);
		}
		while(bfs())while((v=dinic(S,INF)))ans-=v;
		Mmax(maxn,ans);
	}
	return maxn<=-k;
}
inline void mian()
{
	read(T_);
	while(T_--)
	{
		read(n,m,k),S=n+m+1,T=S+1,vis.reset(),L=0;
		for(int i=1;i<=m;++i)read(b[i].x,b[i].y,b[i].z,b[i].t);
		for(int j=1;j<=m;++j)
		{
			R=1e18;
			while(L<R)
			{
				mid=L+((R-L+1)>>1);
				if(!chk(mid))R=mid-1;
				else L=mid;
			}
//				cerr<<"CLEAR:\n";
//				cout<<chk(45)<<endl;
//				exit(0);
//				cout<<chk(19,1);exit(0);
//				cout<<L<<" "<<chk(L)<<" "<<chk(L+1)<<endl;
			can.reset();
			for(int i=1;i<=m;++i)
			{
				if(vis[i])continue;
				can[i]=1;
				if(!chk(L))can[i]=0;
			}
			for(int i=1;i<=m;++i)
			{
				if(vis[i])continue;
				if(!can[i])
				{
					x[i]=max(0ll,(L+b[i].z-b[i].t)/2/b[i].z);
					vis[i]=1;break;
				}
			}
			can.reset();
		}
		ans=0;
		for(int i=1;i<=m;++i)ans+=1ll*x[i]*x[i]*b[i].z+1ll*x[i]*b[i].t;
		write(ans,'\n');
	}
}
```