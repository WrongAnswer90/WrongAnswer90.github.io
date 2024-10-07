---
layout: post
title: CF903G Yet Another Maxflow Problem
subtitle: 题解
tags: 题解 最小割 线段树 CF
show: true
---

[CF903G Yet Another Maxflow Problem](https://www.luogu.com.cn/problem/CF903G)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/18079174)

神奇的模拟最小割。（？）[另一道类似的题](https://www.luogu.com.cn/problem/CF1861F)

暴力流显然跑不过。考虑最大流等于最小割，考虑割去那些边使得 $a_1,b_n$ 不连通。

一个重要的性质是 $A$ 链上和 $B$ 链上都最多只会删一条边。原因很显然，如果删了很多条边，可以只保留 $A$ 中最靠前的边和 $B$ 中最靠后的边。为了方便处理，设连接 $A_i,A_{i+1}$ 的边编号为 $i$，连接 $B_i,B_{i+1}$ 的边编号为 $i+1$，所以编号为 $an,b0$ 的边的边权为 $0$。

如果断了 $ai$ 和 $bj$ 条边，中间的边需要断掉的是 $x_i\leq ai\wedge y_i\geq b_j$ 的所有边。所以扫描线扫描断开 $A$ 的边的编号，同时维护 $f_i$ 表示假设此时断开 $bi$ 边的代价。

$f_i$ 的初值即为 $B_i$。当扫到一条边 $(x,y,z)$ 时，需要做的是 $[1,y]$ 前缀加 $z$，还需要求全局最小值，线段树维护即可，这样可以求出 $g_i$ 表示断 $ai$ 条边的最小代价为 $A_i+g_i$。

对于修改，就是单点修改，查询全局最大值。可以用堆维护，这里直接偷懒线段树废物再利用了/tx/tx。复杂度是 $\mathcal O((m+q)\log n)$。

```cpp
	int n,m,q,a[200010],b[200010],v[200010];
	vector<pii> ve[200010];
	namespace Segment
	{
		#define ls(x) (t[x].l+t[x].r)
		#define rs(x) ((t[x].l+t[x].r)^1)
		struct{int l,r,v,tg;}t[400010];
		inline void update(int x){t[x].v=min(t[ls(x)].v,t[rs(x)].v);}
		inline void down(int x,int y){t[x].tg+=y,t[x].v+=y;}
		inline void spread(int x){down(ls(x),t[x].tg),down(rs(x),t[x].tg),t[x].tg=0;}
		void build(int p,int l,int r)
		{
			t[p].l=l,t[p].r=r,t[p].tg=0;
			if(l==r)return t[p].v=v[l],void();
			int mid=l+((r-l)>>1);
			build(ls(p),l,mid),build(rs(p),mid+1,r),update(p);
		}
		void modify(int p,int l,int r,int x)
		{
			if(l<=t[p].l&&r>=t[p].r)return down(p,x);
			spread(p);
			if(l<=t[ls(p)].r)modify(ls(p),l,r,x);
			if(r>t[ls(p)].r)modify(rs(p),l,r,x);
			update(p);
		}
	}
	using namespace Segment;
	inline void mian()
	{
		read(n,m,q);int x,y,z;
		for(int i=1;i<n;++i)read(a[i],b[i]),v[i+1]=b[i];
		while(m--)read(x,y,z),ve[x].eb(mp(y,z));
		build(1,1,n);
		for(int i=1;i<=n;++i)
		{
			for(auto p:ve[i])modify(1,1,p.fi,p.se);
			v[i]=a[i]+t[1].v;
		}
		build(1,1,n),write(t[1].v,'\n');
		while(q--)read(x,y),modify(1,x,x,y-a[x]),a[x]=y,write(t[1].v,'\n');
	}
```