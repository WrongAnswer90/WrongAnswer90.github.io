---
layout: post
title: ARC180E LIS and Inversion
subtitle: 题解
tags: 题解 DP优化 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18381998)

### [[ARC180E] LIS and Inversion](https://www.luogu.com.cn/problem/AT_arc180_e)

首先考虑要求代价为 $0$ 的一个暴力 DP：$f_{i,j}$ 表示填了前 $i$ 个数，此时相对值域末尾为 $j$ 的数结尾的 LIS 的最大值。填第 $i+1$ 个数的时候，把它插在某两个数之间，所以转移是：

$$
f_{i,j}=
\begin{cases}
f_{i-1,j-1}\qquad\qquad\qquad j>i-a_{i}\\
\max_{k<j}\{f_{i-1,k}+1\}\quad 1<j\leq i-a_{i}\\
1\qquad\qquad\qquad\qquad\;\; j=1\\
\end{cases}
$$

经过观察，可以发现取前缀最大值的操作是不必要的，第二种情况可以直接令 $f_{i,j}=f_{i-1,j-1}+1$。

证明：假设存在 $f_{i-1,k}>f_{i-1,j-1}$，应当用 $k$ 贡献到 $j$。但是因为 $k+1\leq j\leq i-a_i$，所以 $k+1\leq i-a_i$，所以此时 $f_{i,k+1}\geq f_{i-1,k}+1$。在 $f$ 相同的情况下，显然是下标越小的越有前途，所以 $k+1$ 不劣于 $j$，$j$ 是没有用的。

这样 $f$ 的转移就变成了：先平移一位，在最前面加入 $0$，然后做一个 $[1,i-a_i]$ 的前缀 $+1$ 的操作。然后可以进一步发现，如果允许代价为 $k$，则可以看成把 $k$ 个位置的 $a_i$ 变成 $0$，即前缀加变成了全局加。

所以考虑统计每个位置被多少次前缀加覆盖，这可以用差分简单实现，这样就求出来了 $f$。在最后位置 $i$ 的答案上界是 $i$，所以可以花费 $j$ 的代价（$j\leq i-f_i$）来获得一个 $f_i+j$ 的答案。可以从 $n$ 到 $1$ 做一遍扫描线，每次令 $t_{f_i}=f_i$，还需要查询前缀 $\max$，可以用树状数组维护。总复杂度 $\mathcal O(n\log n)$。（最后这一步好像可以 $\mathcal O(n)$ 的谔谔）

```cpp
int n,a[250010],f[250010],ans[250010];
namespace BIT
{
	int t[250010];
	inline void add(int x,int y){for(;x<=n;x+=x&-x)Mmax(t[x],y);}
	inline int ask(int x){int s=0;for(;x;x-=x&-x)Mmax(s,t[x]);return s;}
}
using namespace BIT;
inline void mian()
{
	read(n);int x;
	for(int i=1;i<=n;++i)read(x),f[n-i+1]++,f[n-x+1]--;
	for(int i=1;i<=n;++i)f[i]+=f[i-1];
	for(int i=n;i>=1;--i)add(f[i],f[i]),ans[i]=i-ask(i);
	for(int i=1;i<=n;++i)write(ans[i]);
}
```