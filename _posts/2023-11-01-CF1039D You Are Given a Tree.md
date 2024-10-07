---
layout: post
title: CF1039D You Are Given a Tree
subtitle: 题解
tags: 题解 根号分治 CF
show: true
---

[CF1039D You Are Given a Tree](https://www.luogu.com.cn/problem/CF1039D)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17803955.html)

一种神奇套路：对答案根分，根分的依据是链的长度和答案大致是一个成反比的关系。

考虑确定了 $k$ 怎么做。因为一个点只能在一条链里，所以 dfs 的时候如果能拼成一条链就一定会拼成一条链，不然就把贡献传给父亲继续尝试。

对于 $k$ 比较小的时候可以暴力计算。对于 $k$ 较大的情况，发现答案一定非严格单减，并且递减的幅度越来越小。所以对于 $k$ 比较大的情况，可以通过枚举答案，二分找出答案对应的区间。

设块长为 $B$，对于 $k\leq B$ 暴力计算，$k>B$ 枚举答案不会超过 $\dfrac{n}{B}$，复杂度 $\mathcal O(Bn+\dfrac{n^2}{B}\log n)$，块长取 $1000$ 可以通过此题。

一点卡常技巧：dfs 的次数非常多，所以可以先 dfs 一遍，之后按照 dfn 序扫一遍即可。

```cpp
	int n,cnt,ans,bl,len,tot,maxn[100010],maxm[100010],up[100010],a[100010],fin[100010],head[100010],to[200010],nex[200010];
	inline void add(int x,int y){to[++cnt]=y,nex[cnt]=head[x],head[x]=cnt;}
	void dfs0(int k,int fa)
	{
		a[++tot]=k,up[k]=fa;
		for(int i=head[k];i;i=nex[i])if(to[i]!=fa)dfs0(to[i],k); 
	}
	void dfs()
	{
		for(int i=n;i>=1;--i)
		{
			int now=a[i];maxn[now]=maxm[now]=0;
			for(int j=head[now];j;j=nex[j])
			{
				if(to[j]==up[now])continue;
				int t=maxn[to[j]];
				if(t>=maxn[now])maxm[now]=maxn[now],maxn[now]=t;
				else Mmax(maxm[now],t);
			}
			if(maxn[now]+maxm[now]+1>=len)++ans,maxn[now]=0;
			else ++maxn[now];
		}
	}
    inline void mian()
    {
    	read(n),bl=1000;int x,y;
    	for(int i=1;i<n;++i)read(x,y),add(x,y),add(y,x);
    	dfs0(1,0);
    	for(int i=1;i<=bl;++i)len=i,ans=0,dfs(),fin[i]=ans;
    	int tt=ans;
    	for(int i=0,last=n+1;i<=tt;++i)
    	{
    		int l=bl+1,r=last,mid;
    		while(l<r)
    		{
    			len=mid=l+((r-l)>>1),ans=0,dfs();
    			if(ans>i)l=mid+1;
    			else r=mid;
			}
			for(int j=l;j<last;++j)fin[j]=i;
			last=l;
		}
		for(int i=1;i<=n;++i)write(fin[i],'\n');
	}
```