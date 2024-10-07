---
layout: post
title: CF1131G Most Dangerous Shark
subtitle: 题解
tags: 题解 DP优化 单调栈 CF
show: true
---

[CF1131G Most Dangerous Shark](https://www.luogu.com.cn/problem/CF1131G)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/18070928)

题解区大佬的题解都看不懂捏。提供一个可能更简单的思路？

首先可以线性求出 $pre_i,nex_i$ 表示向左，向右能推倒的最远的骨牌的下标，以求 $pre$ 为例，具体方法是从小到大扫 $i$，初始化 $j$ 为 $i-1$，然后不断执行如果 $i-j<a_i$ 则令 $j$ 变成 $pre_j-1$，否则 $pre_i$ 为 $j+1$，求 $nex$ 同理。

正确性显然，复杂度是均摊 $\mathcal O(n)$，证明的话考虑扫到 $i$ 的时候，设当前的势能 $\varphi(i)$ 表示从 $i$ 开始，跳 $pre$ 跳几次会跳到 $1$，每次势能最多增加 $1$，跳了几次 $pre$ 势能就会减少多少，而势能显然是非负的。

有了 $pre$ 和 $nex$ 之后就可以 dp 了，设 $f_i$ 为推倒前 $i$ 块最小代价，则

$$
f_i=\min_{pre_i-1\leq j} f_j+c_i
$$

然后用 $f_{i-1}+c_i$ 更新 $f_{nex_i}$ 即可，瓶颈在前 $pre$ 的部分，看起来只能带 $\log$，但是由于 $pre$ 的特殊性质，可以优化到 $\mathcal O(n)$。

考虑 $\log$ 怎么做，线段树太平凡了，考虑单调栈，二分查询。但是注意到 $pre$ 有个很强的性质：$[pre_i,i]$ 不会出现交叉不包含的情况，所以对于查询到的一个元素，**当前单调栈中**该元素之后的所有元素都一定不会被查询到，可以直接弹掉，这样就不需要二分了，总复杂度均摊 $\mathcal O(n)$。

```cpp
	int n,m,q,top,f[10000010],st[10000010],pre[10000010],nex[10000010];
	pii a[10000010];
	vector<pii> ve[250010];
	inline void mian()
	{
		read(n,m),m=0;int x,y;
		for(int i=1;i<=n;++i)
		{
			read(x),ve[i].resize(x);
			for(int j=0;j<x;++j)read(ve[i][j].fi);
			for(int j=0;j<x;++j)read(ve[i][j].se);
		}
		read(q);
		while(q--){read(x,y);for(auto p:ve[x])a[++m]=p,a[m].se*=y;}
		for(int i=1,j=0;i<=m;pre[i++]=j+1,j=i-1)while(j&&i-j<a[i].fi)j=pre[j]-1;
		for(int i=m,j=m+1;i>=1;nex[i--]=j-1,j=i+1)while(j<=m&&j-i<a[i].fi)j=nex[j]+1;
		memset(f,127,sizeof(f)),f[0]=0;
		for(int i=1;i<=m;++i)
		{
			while(top&&st[top-1]>=pre[i]-1)--top;
			Mmin(f[i],f[st[top]]+a[i].se),Mmin(f[nex[i]],f[i-1]+a[i].se);
			while(top&&f[st[top]]>=f[i])--top;
			st[++top]=i;
		}
		write(f[m]);
	}
```