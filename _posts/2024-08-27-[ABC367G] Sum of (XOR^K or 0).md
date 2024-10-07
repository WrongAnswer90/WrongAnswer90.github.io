---
layout: post
title: ABC367G Sum of (XOR^K or 0)
subtitle: 题解
tags: 题解 FWT AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18383402)

### [[ABC367G] Sum of (XOR^K or 0)](https://www.luogu.com.cn/problem/AT_abc367_g)

考虑求出 $ans_i$ 表示选了 $m$ 的倍数个数，异或和是 $i$ 的方案数再统计答案。

先考虑 $m=1$ 怎么做。相当于是 $ans_i=[x^i]\prod_j (x^0+x^{a_j})$，这里的乘法是异或卷积。如果直接 `xor-FWT` 复杂度不如暴力。令 $F_i(x)$ 表示第 $i$ 个数对应的集合幂级数，列出式子：

$$
\begin{aligned}
f_k&=[x^k]\text{FWT}(\prod_i F_i(x))\\
&=\prod_i ((-1)^{\text{popcount}(k\And a_i)}+(-1)^{\text{popcount}(k\And 0)})\\
&=\prod_i ((-1)^{\text{popcount}(k\And a_i)}+1)\\
\end{aligned}
$$

所以只需要对于每个 $k$ 求出求出 $\text{popcount}(k\And a_i)$ 是奇数和偶数的 $j$ 的个数。考虑构造一个新的集合幂级数 $G_i(x)$，满足 $G_i(x)=x^{a_i}$，所以 $\text{FWT}(G_i(x))=\sum_j x^j(-1)^{\text{popcount}(j\And a_i)}$。

$$
g_k=\sum_{i}(-1)^{\text{popcount}(k\And a_i)}=\sum_i [x^k]\text{FWT}(G_i(x))
$$

因为 FWT 是线性变换，所以

$$
g_k=[x^k]\text{FWT}(\sum_i G_i(x))
$$

$x$ 表示 $\text{popcount}(k\And a_i)$ 是偶数的个数，$y$ 表示奇数，则有

$$
\left\{\begin{matrix}
x+y=n\\
x-y=g_k
\end{matrix}\right.
$$

可以对于每个 $k$ 解出来 $x,y$。这样就求出来了异或卷积的 FWT 数组 $f_i$，把 $f$ IFWT 回去就得到了 $ans_i$。

对于如何拓展到 $m\not=1$，其实也很简单，看成在 $x$ 中选若干个，$y$ 中选若干个，要求选的总数是 $m$ 的倍数并且在 $y$ 中每选一个都要乘一个 $-1$。预处理 $a_{i,j}=\sum_{k\equiv j\pmod m}\binom{i}{k}$ 和 $b_{i,j}=\sum_{k\equiv j\pmod m}\binom{i}{k}(-1)^k$，然后把 $a,b$ 合并起来就能求出 $c_i$ 表示有 $i$ 个 $1$，$n-i$ 个 $-1$，在之中选 $m$ 的倍数个的所有方案的权值乘积和，然后令 $f_i=c_x$ 即可。总复杂度 $\mathcal O(nm+k2^k)$，其中 $k=20$。

```cpp
const int N=1<<20;
int n,m,K,f[N],F[200010][110],G[200010][110],H[200010];
inline void mian()
{
	read(n,m,K),F[0][0]=G[0][0]=1;int x,y,iv=power(N,MOD-2),ans=0;
	for(int i=0;i<n;++i)for(int j=0;j<m;++j)
	Madd(F[i+1][j],F[i][j]),Madd(F[i+1][(j+1)%m],F[i][j]),
	Madd(G[i+1][j],G[i][j]),Mdel(G[i+1][(j+1)%m],G[i][j]);
	for(int i=0;i<=n;++i)for(int j=0;j<m;++j)
	Madd(H[i],Cmul(F[i][j],G[n-i][(m-j)%m]));
	for(int i=1;i<=n;++i)read(x),++f[x];
	for(int i=1;i<N;i<<=1)for(int j=0;j<N;j+=i<<1)for(int k=j;k<j+i;++k)
	x=f[k]+f[k+i],y=f[k]-f[k+i],f[k]=x,f[k+i]=y;
	for(int i=0;i<N;++i)f[i]=H[(n+f[i])>>1];
	for(int i=1;i<N;i<<=1)for(int j=0;j<N;j+=i<<1)for(int k=j;k<j+i;++k)
	x=Cadd(f[k],f[k+i]),y=Cdel(f[k],f[k+i]),f[k]=x,f[k+i]=y;
	for(int i=0;i<N;++i)Madd(ans,Cmul(f[i],iv,power(i,K)));
	write(ans);
}
```