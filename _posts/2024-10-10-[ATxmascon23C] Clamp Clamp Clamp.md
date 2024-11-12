---
layout: post
title: ATxmascon23C Clamp Clamp Clamp
subtitle: 题解
tags: AT 数数 递推
show: true
---

# [My Blogs](https://wronganswer90.github.io/2024/10/10/ATxmascon23C-Clamp-Clamp-Clamp)

### [[ATxmascon23C]Clamp Clamp Clamp](https://atcoder.jp/contests/xmascon23/tasks/xmascon23_c)


首先看成先确定一个数，然后再选 $n$ 个区间，不断地把这个数放到区间里。

如果所有区间的交不为空，容易发现答案一定是 $n$。先用 $(n!)^2$ 确定后面的相对顺序，然后 $(2n+1)$ 确定第一个数插在哪里，所以首先 $v_n=(2n+1)(n!)^2$。

考虑枚举 $k$ 作为极长的有交的后缀，假设最后取到答案的是交的左端点 $i$，后 $k$ 个的方案数就是 $kl^{\underline{k-1}}(2n-i)^{\underline{k}}$，第倒数 $k+1$ 个的方案数是 $\binom{i-k+1}{2}$，前 $k$ 个的方案数是 $(2n-2k-1)!2^{-(n-k-1)}$，则方案数就是：

$$
\begin{aligned}
&kl^{\underline{k+1}}(2n-i)^{\underline{k}}(2n-2k-1)!2^{-(n-k)}\\
=&k2^{k-n}i!(2n-i)!\binom{2n-2k-1}{i-k-1}
\end{aligned}
$$

需要对于每个 $i$ 算这个东西。看上去就不是很有封闭形式。因为要对每个 $i$ 算，所以把 $2^{-n},i!,(2n-i)!$ 都提出来，考虑写一下递推式：

$$
\begin{aligned}
&\sum_{k=1}^{n-1}k2^{k}\binom{2n-2k-1}{i-k-1}\qquad\qquad\qquad\qquad (1)\\
=&\sum_{k=1}^{n-1}k2^{k}\binom{2n-2(k+1)}{i-(k+1)}+\sum_{k=1}^{n-1}k2^{k}\binom{2n-2(k+1)}{i-(k+1)-1}\\
=&\frac 1 2(\sum_{k=2}^{n}(k-1)2^{k}\binom{2n-2k}{i-k}+\sum_{k=2}^{n}(k-1)2^{k}\binom{2n-2k}{(i-1)-k})\\
&\sum_{k=1}^{n-1}2^{k}\binom{2n-2k-1}{i-k-1}\qquad\qquad\qquad\qquad\;\; (2)\\
=&\sum_{k=1}^{n-1}2^{k}\binom{2n-2(k+1)}{i-(k+1)}+\sum_{k=1}^{n-1}2^{k}\binom{2n-2(k+1)}{i-(k+1)-1}\\
=&\frac 1 2(\sum_{k=2}^{n}2^{k}\binom{2n-2k}{i-k}+\sum_{k=2}^{n}2^{k}\binom{2n-2k}{(i-1)-k})\\
&\sum_{k=1}^{n-1}k2^{k}\binom{2n-2k}{i-k}
\qquad\qquad\qquad\qquad\quad\;\; (3)\\
=&\sum_{k=1}^{n-1}k2^{k}\binom{2n-2k-1}{i-k}+\sum_{k=1}^{n-1}k2^{k}\binom{2n-2k-1}{(i-1)-k}\\
&\sum_{k=1}^{n-1}2^{k}\binom{2n-2k}{i-k}
\qquad\qquad\qquad\qquad\quad\;\;\;\; (4)\\
=&\sum_{k=1}^{n-1}2^{k}\binom{2n-2k-1}{i-k}+\sum_{k=1}^{n-1}2^{k}\binom{2n-2k-1}{(i-1)-k}\\
\end{aligned}
$$

设 $\displaystyle f(i,x,y)=\sum_{k=1}^{n-1}k^x2^k\binom{2n-2k-y}{i-k}$，其中 $x,y\in\\{0,1\\}$，则可以整理出递推式：

$$
\begin{aligned}
f(i,1,1)&=\frac 1 2(f(i,1,0)-f(i,0,0)+(n-1)2^n\binom 0 {i-n})\\
&+\frac 1 2(f(i-1,1,0)-f(i-1,0,0)+(n-1)2^n\binom 0 {i-1-n})\\
f(i,0,1)&=\frac 1 2(f(i,0,0)+2^n\binom 0 {i-n}-2\binom{2n-2}{i-1})\\
&+\frac 1 2(f(i-1,0,0)+2^n\binom 0 {i-1-n}-2\binom{2n-2}{i-2})\\
f(i,1,0)&=f(i,1,1)+f(i-1,1,1)\\
f(i,0,0)&=f(i,0,1)+f(i-1,0,1)
\end{aligned}
$$

这样知道了 $f(n)$ 就能推出来 $f(n-1)$，时间复杂度 $\mathcal O(n)$。

```cpp
const int N=20000000;
int n,m,n2,iv2,now,pre=1,ans[N+10],f[2][2][2],fr[N+10],inv[N+10];
inline int C(int n,int m){return m>n||m<0?0:Cmul(fr[n],inv[m],inv[n-m]);}
inline void mian()
{
	fr[0]=inv[0]=1;int x;
	for(int i=1;i<=N;++i)fr[i]=Cmul(fr[i-1],i);
	inv[N]=power(fr[N],MOD-2);
	for(int i=N-1;i>=1;--i)inv[i]=Cmul(inv[i+1],i+1);
	read(n,m);n2=power(2,n),iv2=power(n2,MOD-2);
	for(int i=1,nw=2;i<n;++i,Mmul(nw,2))
	{
		Madd(f[now][0][0],Cmul(nw,C(2*n-2*i,2*n-i-1)));
		Madd(f[now][0][1],Cmul(nw,C(2*n-2*i-1,2*n-i-1)));
		Madd(f[now][1][0],Cmul(i,nw,C(2*n-2*i,2*n-i-1)));
		Madd(f[now][1][1],Cmul(i,nw,C(2*n-2*i-1,2*n-i-1)));
	}
	ans[2*n]=f[now][1][1];
	for(int i=2*n-1;i>=0;--i)
	{
		now^=1,pre^=1,memset(f[now],0,sizeof(f[now]));
		f[now][0][1]=Cdel(f[pre][0][0],f[pre][0][1]);
		f[now][1][1]=Cdel(f[pre][1][0],f[pre][1][1]);
		f[now][0][0]=Cdel(Cmul(2,Cadd(f[now][0][1],C(2*n-2,i-1),C(2*n-2,i-2))),f[pre][0][0]);
		if(i==n||i==n+1)Mdel(f[now][0][0],n2);
		f[now][1][0]=Cdel(Cadd(f[now][1][1],f[now][1][1],f[pre][0][0],f[now][0][0]),f[pre][1][0]);
		if(i==n||i==n+1)Mdel(f[now][1][0],Cmul(n-1,n2));
		ans[i]=f[now][1][1];
	}
	for(int i=0;i<=2*n;++i)Mmul(ans[i],fr[i],fr[2*n-i],iv2);
	for(int i=0;i<=n;++i)
	ans[i]=ans[2*n-i]=Cadd(ans[i],ans[2*n-i]);
	Madd(ans[n],Cmul(2*n+1,fr[n],fr[n]));
	while(m--)read(x),write(ans[x]);
}
```
