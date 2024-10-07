---
layout: post
title: CF1730F Almost Sorted
subtitle: 题解
tags: 题解 数数 状压DP CF
show: true
---

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/18005665)

[CF1730F Almost Sorted](https://www.luogu.com.cn/problem/CF1730F)

挺有意思的一道题。

刚看到可能有好几种思路，按照 $p$ 的大小填 $q$，或者按照下标顺序填等等。

~~试了几次之后~~ 考虑按照 $i$ 从小到大填入 $q_i$，设 $pos$ 为当前填了的最大的 $p_{q_i}$，由于题目的要求，$1\sim(pos-m-1)$ 的所有数一定都用过（$p_{q_j},j<i$）。所以状压记录值域的 $(pos-m)\sim pos$ 位上有哪些数还没填，这里的值域指的仍然是 $p_{q_i}$，根据 $i$ 和 $S$ 可以推出 $pos$。

然后求逆序对的话对于值域的最后 $m+1$ 位暴力计算，对于 $\leq pos-m$ 的部分，设 $b_{p_i}=i$ 则可以看成是一个询问 $i<pos-m$ 且 $b_i>b_{now}$ 的点的数量，二维前缀和即可，复杂度 $\mathcal O(n^2+n2^kk^2)$。

```cpp
	int n,m,b[5010],f[5010][512],a[5010];
	unsigned short t[5010][5010];
	inline void mian()
	{
		read(n,m),++m;
		for(int i=1;i<=n;++i)read(a[i]),++t[i][a[i]],b[a[i]]=i;
		for(int i=n;i>=1;--i)for(int j=1;j<=n;++j)t[i][j]+=t[i+1][j]+t[i][j-1]-t[i+1][j-1];
		memset(f,127,sizeof(f)),f[0][(1<<m)-1]=0;
		for(int i=1;i<=n;++i)
		{
			for(int j=1,pos;j<(1<<m);j+=2)
			{
				pos=i+m-__builtin_popcount(j)-1;
				if(f[i-1][j]>=inf)continue;
				for(int k=1;k+pos<=n&&k<=m;++k)
				{
					if(!(j>>(m-k)&1))break;
					int x=0;
					for(int l=0;l<m&&pos>l;++l)x+=(j>>l&1)&&b[pos-l]>b[pos+k];
					Mmin(f[i][(j<<k|1)&((1<<m)-1)],f[i-1][j]+x+(pos>m?t[b[pos+k]+1][pos-m]:0));
				}
				for(int k=0;k<m&&k<pos;++k)
				{
					if(j>>k&1)continue;
					int x=0;
					for(int l=0;l<k;++l)x+=(j>>l&1)&&b[pos-l]>b[pos-k];
					for(int l=k+1;l<m&&l<pos;++l)x+=(j>>l&1)&&b[pos-l]>b[pos-k];
					Mmin(f[i][j|1<<k],f[i-1][j]+x+(pos>m?t[b[pos-k]+1][pos-m]:0));
				}
			}
		}
		write(f[n][(1<<m)-1]);
	}
```