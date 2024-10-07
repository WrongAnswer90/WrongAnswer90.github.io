---
layout: post
title: ARC182C Sum of Number of Divisors of Product
subtitle: 题解
tags: 题解 组合意义 矩阵快速幂 状压DP AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18356106)

### [[ARC182C] Sum of Number of Divisors of Product](https://www.luogu.com.cn/problem/AT_arc182_c)

见过一万遍的套路啊啊啊啊啊。

首先如果序列乘积是 $\prod c_i^{p_i}$，答案就是 $\prod (p_i+1)$。$M$ 以内的质数个数只有 $6$ 个：$\{2,3,5,7,11,13\}$。

然后考虑这个式子的组合意义：对于每个质因子，可以看成在其每次出现的时候可以选它，这样每个质因子需要恰好被选一次。对于 $+1$ 的问题可以一开始就把所有无论选还是不选的答案设成 $1$。

状压上面这个东西，$0$ 表示这一位还没选，$1$ 表示选了。预处理转移矩阵，因为要求序列长度 $\leq n$，所以还要开一维记答案，总复杂度 $\mathcal O(T^3\log n)$，其中 $T=65$。

```cpp
int n,m,v[6];
struct Matrix{int f[65][65];Matrix(){memset(f,0,sizeof(f));}}M;
struct Vector{int f[65];Vector(){memset(f,0,sizeof(f));}}F;
inline Matrix operator *(const Matrix x,const Matrix y)
{
	Matrix z;
	for(int i=0;i<65;++i)for(int j=0;j<65;++j)for(int k=0;k<65;++k)
	Madd(z.f[i][j],Cmul(x.f[i][k],y.f[k][j]));
	return z;
}
inline Vector operator *(const Vector x,const Matrix y)
{
	Vector z;
	for(int j=0;j<65;++j)for(int k=0;k<65;++k)
	Madd(z.f[j],Cmul(x.f[k],y.f[k][j]));
	return z;
}
const int P[6]={2,3,5,7,11,13};
inline void mian()
{
	read(n,m),M.f[64][64]=1;
	for(int i=1;i<=m;++i)
	{
		int x=i;memset(v,0,sizeof(v));
		for(int j=0;j<6;++j)while(x%P[j]==0)++v[j],x/=P[j];
		for(int j=0;j<64;++j)
		{
			for(int k=j;;k=(k-1)&j)
			{
				int s=1;
				for(int l=0;l<6;++l)if((k^j)>>l&1)Mmul(s,v[l]);
				Madd(M.f[k][j],s);
				if(j==63)Madd(M.f[k][64],s);
				if(!k)break;
			}
		}
	}
	for(int i=0;i<64;++i)F.f[i]=1;
	for(;n;n>>=1,M=M*M)if(n&1)F=F*M;
	write(F.f[64]);
}
```