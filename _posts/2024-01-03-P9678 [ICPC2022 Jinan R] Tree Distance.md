---
layout: post
title: P9678 [ICPC2022 Jinan R] Tree Distance
subtitle: 题解
tags: 题解 点分治 支配对 线段树 大数据结构 Luogu
show: true
---

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17944113)

[P9678 [ICPC2022 Jinan R] Tree Distance](https://www.luogu.com.cn/problem/P9678)

支配对，不是非常难。

显然如果 $a\leq b<c\leq d$ 且 $dis(a,d)>dis(b,c)$ 则点对 $(a,d)$ 是无用的，猜想点对数不会太多，事实也正是如此。

树上距离是很复杂的东西，考虑简化：使 $root$ 为根，暂且让 $dis(x,y)=dep_x+dep_y$（正确性后面会证）。此时一个点对 $(x,y)$ 是有用的的充要条件是不存在 $x\leq a<b\leq y$ 且$dep_a+dep_b<dep_x+dep_y$。

这个条件仍然不好刻画，观察一下性质可以发现充要条件可以变成：$dep_x,dep_y$ 是区间 $[x,y]$ 中 $dep$ 的最小和次小值，充分性和必要性都显然。这样钦定一个根最多会有 $\mathcal O(siz_x)$ 个点对。

一个推论是支配点对一定不会出现相交。所以可以类似分治的思想：把点按照编号排序，此时能和最左边的点匹配的点一定是类似单调栈一样的东西，可以记录 $nx_i$ 表示编号比 $i$ 大且 $dep$ 比 $i$ 小的最小的点，这样暴力跳，因为一定不会出现相交，分治的时候递归 $(now,nx_{now})$ 即可。

上面还有一个问题：我们是强制让 $dis(x,y)=dep_x+dep_y$ 的，但是实际距离可能并不是。但是这个并不会影响任何东西。首先如果 $(x,y)$ 作为支配点对出现，那不会影响任何东西（常熟可能会大一些）。如果通过 $a\leq x<y\geq b$ 用 $(x,y)$ 把 $(a,b)$ 叉掉了，那如果 $dis(a,b)$ 变得更小一定更能叉掉。所以上面讨论的实际上是一个必要的点对，但是复杂度是对的。剩下的东西就是淀粉质板子了。最后扫描线树状数组维护后缀 $\min$，复杂度 $\mathcal O(n\log^2n+q\log n)$。

```cpp
	int n,m,cnt,len,ans[1000010],head[200010],v[400010],to[400010],nex[400010];
	inline void add(int x,int y,int z){v[++cnt]=z,to[cnt]=y,nex[cnt]=head[x],head[x]=cnt;}
	vector<pii> ve[200010],qu[200010];
	namespace BIT
	{
		int t[200010];
		inline void add(int x,int y){for(;x;x-=x&-x)Mmin(t[x],y);}
		inline int ask(int x){int s=INF;for(;x<=n;x+=x&-x)Mmin(s,t[x]);return s;}
	}
	using namespace BIT;
	namespace Starch
	{
		int nx[200010],id[200010],siz[200010],dep[200010],a[200010],len,root,all;
		bool vis[200010];
		set<int> st;
		void find(int x,int fa)
		{
			siz[x]=1;int maxn=0;
			for(int i=head[x];i;i=nex[i])
			{
				if(to[i]==fa||vis[to[i]])continue;
				find(to[i],x),siz[x]+=siz[to[i]],Mmax(maxn,siz[to[i]]);
			}
			if(max(maxn,all-siz[x])<=all/2)root=x;
		}
		void dfs(int x,int fa)
		{
			a[++len]=x;
			for(int i=head[x];i;i=nex[i])
			{
				if(to[i]==fa||vis[to[i]])continue;
				dep[to[i]]=dep[x]+v[i],dfs(to[i],x);
			}
		}
		void calc(int l,int r)
		{
			int now=l+1;
			while(now&&now<=r)
			{
				ve[a[now]].eb(mp(a[l],dep[a[l]]+dep[a[now]]));
				if(dep[a[now]]<dep[a[l]]||!nx[now])return calc(now,r);
				calc(now,min(r,nx[now])),now=nx[now];
			}
		}
		inline bool cmp(int x,int y){return dep[a[x]]<dep[a[y]]||(dep[a[x]]==dep[a[y]]&&x>y);}
		void starch(int x)
		{
			find(x,0),find(x=root,0),vis[x]=1,dep[x]=len=0,dfs(x,0),sort(a+1,a+1+len);
			for(int i=1;i<=len;++i)id[i]=i;
			sort(id+1,id+1+len,cmp),st.clear();
			for(int i=1;i<=len;++i)
			{
				auto it=st.lower_bound(id[i]);
				if(it==st.end())nx[id[i]]=0;else nx[id[i]]=*it;
				st.insert(id[i]);
			}
			calc(1,len);
			for(int i=head[x];i;i=nex[i])if(!vis[to[i]])all=siz[to[i]],starch(to[i]);
		}
	}
	using namespace Starch;
	inline void mian()
	{
		read(n),all=n;int x,y,z;
		for(int i=1;i<=n;++i)t[i]=INF;
		for(int i=1;i<n;++i)read(x,y,z),add(x,y,z),add(y,x,z);
		read(m),starch(1);
		for(int i=1;i<=m;++i)read(x,y),qu[y].eb(mp(x,i));
		for(int i=1;i<=n;++i)
		{
			for(auto j:ve[i])add(j.fi,j.se);
			for(auto j:qu[i])ans[j.se]=ask(j.fi);
		}
		for(int i=1;i<=m;++i)write(ans[i]==INF?-1:ans[i],'\n');
	}
```