---
layout: post
title: P10008 [集训队互测 2022] Range Minimum Element
subtitle: 题解
tags: 题解 数数 区间DP 双射 Luogu
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18350906)

### [P10008 [集训队互测 2022] Range Minimum Element](https://www.luogu.com.cn/problem/P10008)

难点在于双射构造。

首先考虑给定了 $b$ 如何进行判定。从小到大填数 $x$，每次把能填的地方（$b_i>x$ 的区间之外）全部填上 $x$，这样填一定是最优的。合法当且仅当这样生成的序列 $a$ 对应的 $b$ 就是 $b$ 本身。

发现通过这样的生成方式，合法的 $a$ 和 $b$ 是一一对应的，所以对 $b$ 计数可以转为对能被这样生成的 $a$ 计数。

如果给定了 $a$，如何判 $a$ 是否合法？找到第一个最小值的位置 $p$，把 $A[1,n]$ 变成 $A[1,p-1]$ 和 $A[p+1,n]$。因为 $[1,p-1]$ 中并没有被填上最小值，所以被 $[1,p-1]$ 包含的区间一定选择的都是大于最小值的数，并且他们的的并集一定是 $[1,p-1]$。然后递归判断 $A[1,p-1]$ 和 $A[p+1,n]$ 是否合法。

上述过程是值域从小到大，分裂序列。计数就考虑值域从大到小，合并序列。$f_{l,r,x}$ 表示区间 $[l,r]$ 填入了 $[x,c]$ 中的数的方案数。转移就是枚举最小值的位置 $p$：

$$
\begin{aligned}
f_{l,r,x}&\leftarrow [[l,r]\text{合法}]\times f_{l,r,x+1}\\
f_{l,r,x}&\leftarrow [[l,p-1]\text{合法}]\times f_{l,p-1,x+1}\times f_{p+1,r,x}
\end{aligned}
$$

第一种转移表示区间内没有 $=x$ 的数。预处理合法区间，暴力做复杂度是 $\mathcal O(n^3c)$。套路性的，不难归纳证明 $f_{l,r,x}$ 是一个关于 $x$ 的 $\mathcal O(r-l)$ 次多项式。

令 $x\leftarrow c-x+1$，这样可以暴力求得 $f_{1,n,1\sim n+1}$，然后可以拉插出 $x=c$ 的点值，复杂度是 $\mathcal O(m+n^4)$。

```cpp
int n,m,K,f[110][110][110];
bool v[110][110];
inline void mian()
{
	read(n,m,K);int x,y,ans=0;
	while(m--)read(x,y),v[x][y]=1;
	for(int i=n;i>=1;--i)
	{
		for(int j=i;j<=n;++j)
		{
			int x=-1,y=-1;
			for(int k=j;k>=i;--k)if(v[i][k]){x=k;break;}
			for(int k=i;k<=j;++k)if(v[k][j]){y=k;break;}
			if(x==-1||y==-1)continue;
			v[i][j]|=x>=y-1;
		}
	}
	for(int i=1;i<=n;++i)v[i][i-1]=1;
	for(int i=1;i<=n;++i)for(int j=i;j<=n;++j)f[1][i][j]=1;
	for(int i=1;i<=n+1;++i)for(int j=1;j<=n+1;++j)f[i][j][j-1]=1;
	for(int x=2;x<=n+1;++x)
	{
		for(int i=n;i>=1;--i)
		{
			for(int j=i;j<=n;++j)
			{
				if(v[i][j])f[x][i][j]=f[x-1][i][j];
				for(int k=i;k<=j;++k)if(v[i][k-1])
				Madd(f[x][i][j],Cmul(f[x-1][i][k-1],f[x][k+1][j]));
			}
		}
	}
	for(int i=1;i<=n+1;++i)
	{
		int v=1,x=1;
		for(int j=1;j<=n+1;++j)if(i!=j)Mmul(v,Cdel(i,j)),Mmul(x,Cdel(K,j));
		Madd(ans,Cmul(x,power(v,MOD-2),f[i][1][n]));
	}
	write(ans);
}
```