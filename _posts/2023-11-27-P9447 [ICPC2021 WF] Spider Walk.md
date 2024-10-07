---
layout: post
title: P9447 [ICPC2021 WF] Spider Walk
subtitle: 题解
tags: 题解 线段树 Luogu
show: true
---

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17860148.html)

很有意思的一道题。

设 $f_i$ 表示第 $i$ 根线的答案，首先有一个关键结论：任意两根相邻的线答案只差一定小于 $1$。原因显然，可以在无限远的地方加一根线来构造。该结论可以扩展一下，对于距离为 $d$ 的两根线，答案之差不会超过 $d$。

考虑进行倒着加线，考虑加了一根线会有什么影响：首先会把这两根线的答案交换，然后用上面的结论去更新答案。

对于交换的两根线，交换了之后不一定是最优解，可能可以用其他线的答案更新，这个东西似乎不太好高效维护。

但是事实上，只用左右相邻的两根线来更新代价就是对的。如果存在使用距离它大于等于 $2$ 的线来更新它的答案比和它相邻的线优秀，假设用来更新的线在此线的右边，则一定有：

$$ f_x+x-i>f_{i+1}+1 $$

移一下项，

$$ f_{i+1}-f_x>x-(i+1)$$

与结论相悖，所以原命题正确。接下来考虑用这两根线去更新其他线的答案。发现操作本质是对 $i\in [l,r]$ 的 $i$ 执行 $f_i=\min(f_i,k+i)$（或者 $i$ 的符号相反），可以直接 segment beats 线段树维护。用 $tagl,tagr$ 分别表示左边和右边取 $\min$ 的最小值，下传标记的细节见代码。

```cpp
	int n,m,s;
	namespace Segment
	{
		struct{int l,r,tagl,tagr;}t[800010];
		inline void downl(int p,int tagl){t[p].tagl=min(t[p].tagl,tagl);}
		inline void downr(int p,int tagr){t[p].tagr=min(t[p].tagr,tagr);}
		inline void spread(int p)
		{
			downl(p*2,t[p].tagl),downl(p*2+1,t[p].tagl+t[p*2+1].l-t[p*2].l);
			downr(p*2+1,t[p].tagr),downr(p*2,t[p].tagr+t[p*2+1].r-t[p*2].r);
			t[p].tagl=t[p].tagr=INF;
		}
		void build(int p,int l,int r)
		{
			t[p].l=l,t[p].r=r,t[p].tagl=t[p].tagr=INF;
			if(l==r)return;
			int mid=l+((r-l)>>1);
			build(p*2,l,mid),build(p*2+1,mid+1,r);
		}
		int ask(int p,int x)
		{
			if(t[p].l==t[p].r)return min(t[p].tagl,t[p].tagr);
			return spread(p),x<=t[p*2].r?ask(p*2,x):ask(p*2+1,x);
		}
		void modifys(int p,int x,int y)
		{
			if(t[p].l==t[p].r)return t[p].tagl=t[p].tagr=y,void();
			spread(p);
			if(x<=t[p*2].r)modifys(p*2,x,y);else modifys(p*2+1,x,y);
		}
		void modifyl(int p,int x,int y)
		{
			if(x<=t[p].l)return downl(p,y+t[p].l-x);
			spread(p),modifyl(p*2+1,x,y);
			if(x<=t[p*2].r)modifyl(p*2,x,y);
		}
		void modifyr(int p,int x,int y)
		{
			if(x>=t[p].r)return downr(p,y+x-t[p].r);
			spread(p),modifyr(p*2,x,y);
			if(x>t[p*2].r)modifyr(p*2+1,x,y);
		}
		inline void change(int x,int y){modifyl(1,x,y),modifyr(1,x,y),modifyl(1,1,n-x+1+y),modifyr(1,n,x+y);}
	}
	using namespace Segment;
	pii a[500010];
	inline bool cmp(pii p1,pii p2){return p1.fi>p2.fi;}
	inline void mian()
	{
		read(n,m,s),build(1,1,n),change(s,0);
		for(int i=1;i<=m;++i)read(a[i]);
		sort(a+1,a+1+m,cmp);
		for(int i=1;i<=m;++i)
		{
			a[i].fi=a[i].se,a[i].se=a[i].fi%n+1;
			int lef=ask(1,a[i].fi),rig=ask(1,a[i].se);
			swap(lef,rig);
			modifys(1,a[i].fi,lef),modifys(1,a[i].se,rig);
			int Minl=ask(1,a[i].fi-1==0?n:a[i].fi-1);
			if(Minl+1<lef)modifys(1,a[i].fi,Minl+1);
			Minl=ask(1,a[i].se==n?1:a[i].se+1);
			if(Minl+1<rig)modifys(1,a[i].se,Minl+1);
			change(a[i].fi,lef),change(a[i].se,rig);
		}
		for(int i=1;i<=n;++i)write(ask(1,i),'\n');
	}
```