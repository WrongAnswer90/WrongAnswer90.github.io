---
layout: post
title: CF1801F Another n-dimensional chocolate bar
subtitle: 题解
tags: 题解 DP优化 数论分块 CF
show: true
---

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17949702)

[CF1801F Another n-dimensional chocolate bar](https://www.luogu.com.cn/problem/CF1801F)

高妙的数论分块优化 DP。

第一步设计状态就有很大问题，如果直接设 $f_{i,j}$ 表示前 $i$ 个数成绩为 $j$ 那就死了。这完全没有利用到整除的性质。正确做法是设 $f_{i,j}$ 表示前 $i$ 个数把 $k$ “削”成了 $j$（要求 $\prod_{k=i+1}^nb_k\geq j$）。转移：

$$
f_{i,\lceil\frac j {b_i}\rceil}\leftarrow f_{i,j}\times \lfloor\frac{a_i}{b_i}\rfloor\times\frac 1 {a_i}
$$

利用向上取整的性质

$$
f_{i,\lfloor\frac {j-1} {b_i}\rfloor+1}\leftarrow f_{i,j}\times \lfloor\frac{a_i}{b_i}\rfloor\times\frac 1 {a_i}
$$


注意到 $j$ 如果被“削”两次：

$\lfloor\frac{\lfloor\frac {j-1} x\rfloor+1-1}{y}\rfloor+1=\lfloor\frac j {xy}\rfloor+1$

所以无论 $k$ 被怎么削，状态只会在 $S=\{\lfloor\frac k i\rfloor|i\in [1,k]\}$ 的 $\mathcal O(\sqrt k)$ 个数中，所以状态数变成了 $\mathcal O(n\sqrt k)$。

第二个问题，一眼想到把 $a$ 整除分块变成 $\mathcal O(\sqrt a)$ 个有价值的 $b$，看上去非常的有道理，但乘上状态数当场爆炸。正确的做法是对于状态进行数论分块，对于 $\lfloor\frac j x\rfloor$ 相同的 $x$ 的区间，只需要取最优的左端点进行转移。这样复杂度可以由类似杜教筛的分析方式得出，为 $\mathcal O(k^{0.75})$。

代码很好写，但是小心炸精度。

```cpp
	int n,m,cnt,v[7010],a[110];
	double f[110][7010];
	short pos[10000010];
	inline void mian()
	{
		read(n,m);
		for(int i=1;i<=n;++i)read(a[i]);
		for(int l=1,r;l<m;l=r+1)v[++cnt]=(m-1)/l,r=(m-1)/((m-1)/l);
		v[++cnt]=0,reverse(v+1,v+1+cnt);
		for(int i=1;i<=cnt;++i)pos[v[i]]=i;
		f[0][cnt]=1;
		for(int i=1;i<=n;++i)
		{
			for(int j=1;j<=cnt;++j)
			{
				if(!f[i-1][j])continue;
				for(int l=1,r;l<=v[j];l=r+1)
				{
					r=v[j]/(v[j]/l);
					Mmax(f[i][pos[v[j]/l]],f[i-1][j]*(a[i]/l)/a[i]);
				}
				Mmax(f[i][1],f[i-1][j]*(a[i]/(v[j]+1))/a[i]);
			}
		}
		printf("%.10lf",f[n][1]*m);
	}
```