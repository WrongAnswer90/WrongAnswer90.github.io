---
layout: post
title: ARC184D Erase Balls 2D
subtitle: 题解
tags: 题解 数数 双射构造 CF
show: true
---

# [My Blogs](wronganswer90.github.io/2024/10/07/ARC184D-Erase-Balls-2D)

### [[ARC184D] Erase Balls 2D](https://www.luogu.com.cn/problem/AT_arc184_d)

剩下的点的不同方案不好计数，对操作方案计数会算重。

考虑在剩余点的方案和操作之间构建一个双射：对于一个剩下的方案，不断操作不会新删去当前剩下的点的点直到没有点可以操作，这样操作方案就和剩下的球的方案一一对应了。

所以需要计数的序列是，操作任意一个没有被删去且没有被操作的点都会导致其他没有被删去的点删掉的操作方案。设 $f_i$ 表示最后一次操作是 $i$，枚举下一次操作位置 $j>i$，检查 $i$ 是否能转移到 $j$ 就是直接枚举 $i<k<j$ 且 $a_j<a_k<a_i$，检查操作 $k$ 是否会删去其他值即可判断能否转移。时间复杂度 $\mathcal O(n^3)$。

具体实现可以新建两个点 $(0,n+1)$ 和 $(n+1,0)$ 避免讨论。

```cpp
int n,a[310],f[310],t[310][310];
inline int ask(int x1,int y1,int x2,int y2)
{
	return t[x2][y2]-t[x2][y1-1]-t[x1-1][y2]+t[x1-1][y1-1];
}
inline void mian()
{
	read(n);int x,y;
	for(int i=1;i<=n;++i)read(x,y),a[x]=y,++t[x][y];
	a[0]=n+1;
	for(int i=1;i<=n;++i)for(int j=1;j<=n;++j)t[i][j]+=t[i][j-1];
	for(int i=1;i<=n;++i)for(int j=1;j<=n;++j)t[i][j]+=t[i-1][j];
	f[0]=1;
	for(int i=0;i<=n;++i)
	{
		for(int j=i+1;j<=n+1;++j)if(a[j]<a[i])
		{
			int fl=1;
			for(int k=i+1;k<j;++k)if(a[j]<a[k]&&a[k]<a[i])
			{
				int ffl=1;
				if(ask(i+1,a[j]+1,k-1,a[k]-1)>0)ffl=0;
				if(ask(k+1,a[k]+1,j-1,a[i]-1)>0)ffl=0;
				if(ffl)fl=0;
			}
			if(fl)Madd(f[j],f[i]);
		}
	}
	write(f[n+1]);
}
```



