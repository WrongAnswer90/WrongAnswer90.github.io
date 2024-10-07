---
layout: post
title: CF1662C European Trip
subtitle: 题解
tags: 题解 矩阵快速幂 容斥 CF
show: true
---

# [My Blogs]()

[CF1662C European Trip](https://www.luogu.com.cn/problem/CF1662C)

感觉很不错的矩阵乘法加速题。

从 $n,k$ 的数据范围大致可以看出是矩阵乘法加速递推。

设 $f_{k,u,v}$ 表示从 $u$ 走到 $v$ 走了 $k$ 步的合法方案数，初始状态 $f_1$ 即为邻接矩阵，最终答案为 $\sum_{i=1}^{n} f_{k,i,i}$。

正常的转移方程为 $f_{k,u,v}=\sum_{l=1}^{n}f_{k-1,u,l} \times f_{0,l,v}$，考虑加入限制。

一条边 $(u,v)$ 在被第 $k-1$ 步时从 $u$ 走到 $v$，然后第 $k$ 步又走了回来，那么第 $k-2$ 步一定会走到 $u$ 节点。所以我们转移到底 $k$ 步时先正常的从 $k-1$ 转移过来，然后选择在第 $k-2$ 步时加一些限制条件，减去不合法的方案数，这样所有不合法的方案就会被不重不漏的减去。

![](https://cdn.luogu.com.cn/upload/image_hosting/4ppef9k1.png)

设第 $k-2$ 步时 走过的边是 $(x,u)$，第 $k-1$ 步不能走回 $x$，因为这种情况在转移 $k-1$ 时已经被减去了，所以非法方案是从 $u$ 走到 $1,2,3,4$ 四个点然后走回来一共四种情况，即为 $f_{k-2,u,v} \times (deg_u-1)$

所以最终的转移方程为

$$
f_{k,u,v}=\sum_{l=1}^{n}f_{k-1,u,l} \times f_{1,l,v}-f_{k-2,u,v} \times (deg_v-1)
$$

考虑优化，$f_k$ 是一个矩阵的形式，变形一下：

$$
f_k=f_{k-1} \times f_1-f_{k-2} \times (DEG-I)
$$

其中 $DEG_{i,i}=deg_i$，其余为 $0$。

是线性递推形式，用矩阵快速幂优化。

$$
\begin{bmatrix}
f_{k-1},f_{k-2}
\end{bmatrix}

\times

\begin{bmatrix}
f_1,I\\
DEG-I,0
\end{bmatrix}
=
\begin{bmatrix}
f_{k},f_{k-1}\\
\end{bmatrix}

$$

注意当 $k=2$ 时 $
f_k=f_{k-1} \times f_1-f_{k-2} \times DEG$，度数不需要减 $1$，因为第 $k-2$ 步并没有从一个 $x$ 走到 $u$。

本题的精髓在于如何不重不漏考虑不合法的方案数，矩阵乘法加速的是外层的递推，只不过里面的转移恰好又需要用到矩阵乘法，需要搞清楚两个矩乘的关系。

核心代码如下：

```cpp
int n,m,q,deg[100];
struct Matrix1{int f[100][100];};
struct Matrix2{Matrix1 f[2][2];}a;
struct Line2{Matrix1 f[2];}b;
Matrix1 operator *(const Matrix1 m1,const Matrix1 m2)
{
	Matrix1 m3;memset(m3.f,0,sizeof(m3.f));
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		{
			for(int k=0;k<n;++k)
			m3.f[i][j]=(m3.f[i][j]+m1.f[i][k]*m2.f[k][j])%MOD;
		}
	}
	return m3;
}
Matrix1 operator +(const Matrix1 m1,const Matrix1 m2)
{
	Matrix1 m3;memset(m3.f,0,sizeof(m3.f));
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		m3.f[i][j]=(m1.f[i][j]+m2.f[i][j])%MOD;
	}
	return m3;
}
Matrix2 operator *(const Matrix2 m1,const Matrix2 m2)
{
	Matrix2 m3;memset(m3.f,0,sizeof(m3.f));
	for(int i=0;i<2;++i)
	{
		for(int j=0;j<2;++j)
		{
			for(int k=0;k<2;++k)
			m3.f[i][j]=m3.f[i][j]+m1.f[i][k]*m2.f[k][j];
		}
	}
	return m3;
}
Line2 operator *(const Line2 m1,const Matrix2 m2)
{
	Line2 m3;memset(m3.f,0,sizeof(m3.f));
	m3.f[0]=m1.f[0]*m2.f[0][0]+m1.f[1]*m2.f[1][0];
	m3.f[1]=m1.f[0]*m2.f[0][1]+m1.f[1]*m2.f[1][1];
	return m3;
}
inline void mian()
{
	read(n,m,q),q-=2;int x,y,ans=0;
	while(m--)read(x,y),++deg[x-1],++deg[y-1],b.f[1].f[x-1][y-1]=b.f[1].f[y-1][x-1]=a.f[0][0].f[x-1][y-1]=a.f[0][0].f[y-1][x-1]=1;
	b.f[0]=b.f[1]*b.f[1];
	if(q==-1){for(int i=0;i<n;++i)ans+=b.f[1].f[i][i];write(ans%MOD);return;}
	for(int i=0;i<n;++i)a.f[0][1].f[i][i]=1,a.f[1][0].f[i][i]=1-deg[i]+MOD,b.f[0].f[i][i]=0;
//		for(int i=0;i<n;++i,puts(""))for(int j=0;j<n;++j)write(b.f[0].f[i][j]);
	for(;q;q>>=1,a=a*a)if(q&1)b=b*a;
	for(int i=0;i<n;++i)ans+=b.f[0].f[i][i];
	write(ans%MOD);
}
```