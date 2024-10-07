---
layout: post
title: CF1891F A Growing Tree
subtitle: 题解
tags: 题解 BIT CF
show: true
---

[CF1891F A Growing Tree](https://www.luogu.com.cn/problem/CF1891F)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17799429.html)

有点诈骗。好多人都写的 LCT，但是这题其实连树剖都不需要。提供一个简单的单 $\log$ 小常数做法。

动态加点是假的，可以离线下来得到最后树的结构，记一下 dfn 序。

一个操作对一个点有可能贡献当且仅当操作在加点之后进行。

所以我们把询问从后向前扫一遍，扫到加点就记录这个点的答案为此时的点权，子树加就直接子树加，BIT 维护差分数组即可。

```cpp
	int T,q,cnt,tot,num,ans[500010],dfn[500010],nfd[500010],head[500010],to[500010],nex[500010];
	void dfs(int k){dfn[k]=++num;for(int i=head[k];i;i=nex[i])dfs(to[i]);nfd[k]=num;}
	inline void addt(int x,int y){to[++cnt]=y,nex[cnt]=head[x],head[x]=cnt;}
	struct{int opt,x,y;}b[500010];
	namespace BIT
	{
		int t[500010],N;
		inline void add(int x,int y){for(;x<=N;x+=x&-x)t[x]+=y;}
		inline int ask(int x){int s=0;for(;x;x-=x&-x)s+=t[x];return s;}
		inline void add(int l,int r,int x){add(l,x),add(r+1,-x);}
	}
	using namespace BIT;
	inline void mian()
	{
		read(T);
		while(T--)
		{
			read(q),N=q+5,cnt=num=0,tot=1;
			for(int i=1;i<=N;++i)t[i]=head[i]=0;
			for(int i=1;i<=q;++i)read(b[i].opt,b[i].x),b[i].opt==2?read(b[i].y):addt(b[i].x,b[i].y=++tot);
			dfs(1);
			for(int i=q;i>=1;--i)b[i].opt==1?ans[b[i].y]=ask(dfn[b[i].y]),void():add(dfn[b[i].x],nfd[b[i].x],b[i].y);
			ans[1]=ask(1);
			for(int i=1;i<=tot;++i)write(ans[i]);puts("");
		}
	}
```