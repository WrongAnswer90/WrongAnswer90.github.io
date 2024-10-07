---
layout: post
title: CF513G3 Inversions problem
subtitle: 题解
tags: 题解 数数 DP优化 CF
show: true
---

[CF513G3 Inversions problem](https://www.luogu.com.cn/problem/CF513G3)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17773143.html)

推式子题。

## task 1

直接爆搜，统计每种结果的答案，最后加在一起除以总方案数。

## task 2

数据范围变大，显然不能记录整个数组的状态，考虑拆位算贡献。设 $f_{i,j,k}$ 表示交换了 $k$ 步，$(i,j)$ 构成一对逆序对的概率，直接暴力枚举 $i,j$ 和反转区间的两个端点暴力 $\mathcal O(n^4k)$ 转移。

```cpp
namespace WrongAnswer_90
{
	int n,m,a[101];
	ld f[101][101][1001];
	inline void mian()
	{
		read(n,m);
		for(int i=1;i<=n;++i)read(a[i]);
		for(int i=1;i<=n;++i)for(int j=i;j<=n;++j)f[i][j][0]=(a[i]>a[j]);
		for(int k=1;k<=m;++k)
		{
			for(int i=1;i<=n;++i)
			{
				for(int j=i;j<=n;++j)
				{
					for(int k1=1;k1<=n;++k1)
					{
						for(int k2=k1;k2<=n;++k2)
						{
							int ii=i,jj=j;
							if(i>=k1&&i<=k2)ii=k1+k2-i;
							if(j>=k1&&j<=k2)jj=k1+k2-j;
							if(ii>jj)f[i][j][k]+=(1-f[jj][ii][k-1])/(1.0*n*(n+1)/2);
							else f[i][j][k]+=f[ii][jj][k-1]/(1.0*n*(n+1)/2);
						}
					}
				}
			}
		}
		ld ans=0;
		for(int i=1;i<=n;++i)for(int j=i;j<=n;++j)ans+=f[i][j][m];
		printf("%.10Lf",ans);
	}
}
```

## task3

考虑进一步优化。状态的设计很难再简化了，考虑优化转移的过程。

枚举 $k1,k2$ 两个端点很呆，实际上我们只关心反转的区间的中点位置，枚举端点是为了计算次数，所以我们其实可以只枚举区间的中点计算。

分为以下三种情况：$i,j$ 位置均未变化，$i$ 变 $j$ 不变，$j$ 变 $i$ 不变，$i,j$ 均发生改变。

设 $g(i)$ 为一次翻转区间总的方案数即 $\dfrac{i(i+1)}{2}$。

均不发生变化时，可能是区间没有越过 $i,j$ 中的任何一个，也有可能时中点恰好在 $i,j$ 中的一个并且不越过另一个，所以转移方程为 

$$

f_{i,j,t}=\dfrac{(g(i-1)+g(j-i-1)+g(n-j)+min(i,j-i)+min(j-i,n-j+1))}{g(n)}\times f_{i,j,t-1}

$$

$i$ 变 $j$ 不变时，枚举 $i$ 原来的位置，转移方程为

$$
f_{i,j,t}=\sum_{k=1}^{j-1}\dfrac{\min(k,j-i,i,j-k)}{g(n)}f_{k,j,t-1}
$$

类似的，$j$ 不变为

$$
f_{i,j,t}=\sum_{k=i+1}^{n}\dfrac{\min(k-i,n-j+1,j-i,n-k+1)}{g(n)}f_{i,k,t-1}
$$

对于 $i,j$ 都改变，则 $i,j$ 相对顺序会改变，方程也要变形

$$
f_{i,j,t}=\sum_{k=1}^{2n-1}\dfrac{min(k-j,n-j+1,i,n-(k-i)+1)}{g(n)}(1-f_{k-j,k-i,t-1})
$$

但是这样是 $\mathcal O(n^3)$ 的，还是过不了。发现转移的一定是一段区间，根据 $\min$ 的取值来划分，把 $\min$ 改写成不等式，就变成了常数。所以可以直接前缀和优化成 $\mathcal O(n^2)$。

对于 $k$ 的取值，打表发现取太大了没有意义，所以和 $2000$ 取一个 $\min$ 然后 DP 即可。

本人太菜，写的非常丑，需要维护横着的前缀和，竖着的前缀和，横着的乘一个 $i$ 的前缀和，竖着的乘一个 $j$ 的前缀和，斜着的前缀和，斜着的乘一个 $i$ 的前缀和，斜着的乘一个 $j$ 的前缀和。巨大难写加巨大常数。

```cpp
namespace WrongAnswer_90
{
	int n,m,a[101];
	ld f[101][101][2],all,s1[101][101],s2[101][101],s3[101][101],s4[101][101],s5[101][101],s6[101][101],s7[101][101];//s1:f_{k,j}*k s2:f_{k,j} s3:f_{i,k}*k s4:f_{i,k}
	inline void mian()
	{
		read(n,m),all=1.0*n*(n+1)/2;
		for(int i=1;i<=n;++i)read(a[i]);
		for(int i=1;i<=n;++i)for(int j=1;j<=n;++j)f[i][j][0]=(a[i]>a[j]);
		int now=0,pre=1;
		for(int t=1;t<=min(1000ll,m);++t)
		{
			now^=1,pre^=1;
			for(int i=1;i<=n;++i)
			{
				for(int j=1;j<=n;++j)
				{
					s1[i][j]=s1[i-1][j]+f[i][j][pre]*i,s2[i][j]=s2[i-1][j]+f[i][j][pre];
					s3[i][j]=s3[i][j-1]+f[i][j][pre]*j,s4[i][j]=s4[i][j-1]+f[i][j][pre];
					s5[i][j]=s5[i-1][j-1]+f[i][j][pre]*i,s6[i][j]=s6[i-1][j-1]+f[i][j][pre]*j;
					s7[i][j]=s7[i-1][j-1]+f[i][j][pre];
				}
			}
			for(int i=1;i<=n;++i)
			{
				for(int j=i+1;j<=n;++j)
				{
					f[i][j][now]=f[i][j][pre]*(i*(i-1)/2+(j-i)*(j-i-1)/2+(n-j)*(n-j+1)/2+min(i,j-i)+min(j-i,n-j+1))/all;
					f[i][j][now]+=s1[min(i-1,j-i)][j]/all;
					if(i-1>j-i)f[i][j][now]+=(s2[i-1][j]-s2[j-i][j])*(j-i)/all;
					if(j-i>i)f[i][j][now]+=(s2[j-i][j]-s2[i][j])*i/all;
					f[i][j][now]+=((s2[j-1][j]-s2[max(i,j-i)][j])*j-(s1[j-1][j]-s1[max(i,j-i)][j]))/all;
					
					f[i][j][now]+=(s3[i][min(j-1,n-j+1+i)]-s3[i][i]-(s4[i][min(j-1,n-j+1+i)]-s4[i][i])*i)/all;
					if(j-1>max(i,n-j+i+1))f[i][j][now]+=(s4[i][j-1]-s4[i][max(i,n-j+i+1)])*(n-j+1)/all;
					if(n+1-j+i>j)f[i][j][now]+=(s4[i][n+1-j+i]-s4[i][j])*(j-i)/all;
					f[i][j][now]+=((s4[i][n]-s4[i][max(j,n+1-j+i)])*(n+1)-(s3[i][n]-s3[i][max(j,n-j+i+1)]))/all;
					
					f[i][j][now]+=(min(n-j+1,i)*(min(n-j+1,i)+1)/2-s5[min(n-j+1,i)][min(n-j+1,i)+(j-i)])/all;
					if(i>n-j+1)f[i][j][now]+=(n-j+1)*(i-(n-j+1)-(s7[i][j]-s7[n-j+1][n+1-i]))/all;
					if(n+1-j>i)f[i][j][now]+=i*((n+1-j-i)-(s7[n+1-j][n+1-i]-s7[i][j]))/all;
					if(n-j+i>max(i,n+1-j))f[i][j][now]+=(n-j+i-max(i,n+1-j)-(s7[n-j+i][n]-s7[max(i,n+1-j)][max(j,n+1-i)]))*(n+1)/all-
					((n+max(j+1,n+2-i))*(n-max(j,n+1-i))/2-s6[n-j+i][n]+s6[max(i,n+1-j)][max(j,n+1-i)])/all;
				}
			}
		}
		ld ans=0;
		for(int i=1;i<=n;++i)for(int j=i;j<=n;++j)ans+=f[i][j][now];
		printf("%.10Lf",ans);
	}
}
```