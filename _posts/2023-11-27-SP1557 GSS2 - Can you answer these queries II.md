---
layout: post
title: SP1557 GSS2 - Can you answer these queries II
subtitle: 题解
tags: 题解 大数据结构 线段树 Luogu
show: true
---

[SP1557 GSS2 - Can you answer these queries II](https://www.luogu.com.cn/problem/SP1557)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17860263.html)

扫描线。把询问挂在右端点上，扫描右端点，纵轴仍为序列维。

对于这种出现多次的数只算一次的，记 $pre_i$ 表示 $i$ 这个值上一次的出现位置，套路化的可以强制让出现多次的在 $pre_i<l\wedge i$ 统计，用二维线段树状物维护，但是这道题可以做的更简单。

如果强制让选的右端点就是当前扫描到的右端点的话，设 $f_i$ 为 $i$ 到当前的 $r$ 的出现多次算一次最大子段和，则此时在最右边扩展一位的影响是将 $pre_{a_i}+1\sim i$ 的 $f_i$ 全部加 $a_i$。这样强制右端点在 $r$ 的答案即为 $\max_{i=l}^r f_i$。可以线段树维护。


但是我们需要求的是右端点 $l\leq i\leq r$ 的答案。这里的 $\geq l$，但是去掉这个限制也是无所谓的，因为我们已经限制了左端点的取值，只要把线段树上初值全部设成 $0$，不合法的区间答案正好是 $0$，也正好处理了题目中可以一个都不选的限制。这样线段树历史最值维护一下即可，复杂度 $\mathcal O(m\log n)$。

```cpp
	int n,m,ans[100010],a[100010],c[200010];
	vector<pii> ve[100010];
	namespace Segment
	{
		struct{int l,r,maxm,maxn,tag1,tag2;}t[400010];
		inline void update(int p){t[p].maxn=max(t[p*2].maxn,t[p*2+1].maxn),t[p].maxm=max(t[p*2].maxm,t[p*2+1].maxm);}
		inline void down(int p,int tag1,int tag2){t[p].maxm=max(t[p].maxm,t[p].maxn+tag2),t[p].maxn+=tag1,t[p].tag2=max(t[p].tag2,t[p].tag1+tag2),t[p].tag1+=tag1;}
		inline void spread(int p){down(p*2,t[p].tag1,t[p].tag2),down(p*2+1,t[p].tag1,t[p].tag2),t[p].tag1=t[p].tag2=0;}
		void build(int p,int l,int r)
		{
			t[p].l=l,t[p].r=r;
			if(l==r)return;
			int mid=l+((r-l)>>1);
			build(p*2,l,mid),build(p*2+1,mid+1,r);
		}
		void modify(int p,int l,int r,int k)
		{
			if(l<=t[p].l&&r>=t[p].r)return down(p,k,k);
			spread(p);
			if(l<=t[p*2].r)modify(p*2,l,r,k);
			if(r>t[p*2].r)modify(p*2+1,l,r,k);
			update(p);
		}
		int ask(int p,int l,int r)
		{
			if(l<=t[p].l&&r>=t[p].r)return t[p].maxm;
			spread(p);int s=-INF;
			if(l<=t[p*2].r)s=max(s,ask(p*2,l,r));
			if(r>t[p*2].r)s=max(s,ask(p*2+1,l,r));
			return s;
		}
		void print(int p)
		{
			write(t[p].l),write(t[p].r),write(t[p].maxn),write(t[p].maxm,'\n');
			if(t[p].l==t[p].r)return;
			spread(p);
			print(p*2),print(p*2+1);
		}
	}
	using namespace Segment;
	inline void mian()
	{
		read(n),build(1,1,n);int x,y;
		for(int i=1;i<=n;++i)read(a[i]);
		read(m);
		for(int i=1;i<=m;++i)read(x,y),ve[y].eb(mp(x,i));
		for(int i=1;i<=n;++i)
		{
			modify(1,c[a[i]+100000]+1,i,a[i]),c[a[i]+100000]=i;
			for(auto p:ve[i])ans[p.se]=ask(1,p.fi,i);
		}
		for(int i=1;i<=m;++i)write(ans[i],'\n');
	}
```