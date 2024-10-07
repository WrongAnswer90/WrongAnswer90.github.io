---
layout: post
title: ARC174E Existence Counting
subtitle: 题解
tags: 题解 DP优化 数数 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18384272)

### [[ARC174E] Existence Counting](https://www.luogu.com.cn/problem/AT_arc174_e)

比较机械的处理方式。和 NOID2T2 是一个性质，只不过简单多了。

枚举生成序列和 $P$ 的第一个不同位置 $i$，则第 $i$ 个位置能填的数的个数 $g_i$ 是 $<a_i$ 并且之前没有出现过的数，$g_i$ 可以简单用树状数组求出。

然后考虑如何统计答案。对于 LCP 中所有数的贡献：首先有了 $g_i$，设 $ans_i$ 表示第 $i$ 个位置开始字典序小于 $P$ 的序列的方案数，则 $ans_i=g_i(m-i)^{\underline{n-i}}$。

然后对于一个 $j$，他作为前缀，则所有 $i>j$ 的 $ans_i$ 都对它有贡献。求出 $ans$ 之后做一遍后缀和统计这部分的答案。

在 $i$ 后面出现的答案。直接的想法是分为第 $i$ 个位置和后面 $n-i$ 个位置两种情况分别统计。第一种情况是对所有 $<p_i$ 并且在前面没有出现过的数字有贡献，对偶一下，对于一个 $p_i$，会对他造成贡献的是 $j<i$ 并且 $p_j>p_i$ 的数，这样可以用树状数组维护单点加，后缀和。

但是第二种情况，在 $j<p_i$ 时，贡献是 $(g_i-1)(m-i-1)^{\underline{n-i-1}}$，但是 $j\geq p_i$ 时贡献是 $g_i(m-i-1)^{\underline{n-i-1}}$。可以看成全局没有出现过的 $+g_i(m-i-1)^{\underline{n-i-1}}$，然后 $j<p_i$ 的减去 $(m-i-1)^{\underline{n-i-1}}$，减去的这部分可以和上面第一种情况共用一个树状数组维护后缀和即可。总复杂度 $\mathcal O(n\log n)$。

```cpp
int n,m,a[300010],f[300010],fr[300010],inv[300010],ans[300010];
inline int C(int n,int m){return m<0||m>n?0:Cmul(fr[n],inv[m],inv[n-m]);}
struct BIT
{
	int t[300010];
	inline void add(int x,int y=1){for(;x<=m;x+=x&-x)Madd(t[x],y);}
	inline int ask(int x){int s=0;for(;x;x-=x&-x)Madd(s,t[x]);return s;}
}T1,T2;
bool vis[300010];
inline void mian()
{
	read(m,n),f[n+1]=fr[0]=inv[0]=1;int lst,tmp=0;
	for(int i=1;i<=m;++i)fr[i]=Cmul(fr[i-1],i);
	inv[m]=power(fr[m],MOD-2);
	for(int i=m-1;i>0;--i)inv[i]=Cmul(inv[i+1],i+1);
	for(int i=1;i<=n;++i)read(a[i]);
	for(int i=1;i<=n;++i)
	{
		T1.add(a[i]),lst=a[i]-1-T1.ask(a[i]-1),f[i]=Cmul(lst,C(m-i,n-i),fr[n-i]);
		Madd(tmp,Cmul(lst,C(m-i-1,n-i-1),fr[n-i]));
		T2.add(m-a[i]+2,Cdel(Cmul(C(m-i,n-i),fr[n-i]),Cmul(C(m-i-1,n-i-1),fr[n-i])));
		Madd(ans[a[i]],T2.ask(m-a[i]+1),tmp);
		vis[a[i]]=1;
	}
	for(int i=1;i<=m;++i)if(!vis[i])Madd(ans[i],T2.ask(m-i+1),tmp);
	for(int i=n;i>=1;--i)Madd(f[i],f[i+1]),Madd(ans[a[i]],f[i+1]);
	for(int i=1;i<=m;++i)write(ans[i],'\n');
}
```