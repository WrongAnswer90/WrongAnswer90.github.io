---
layout: post
title: ARC182F Graph of Mod of Linear
subtitle: 题解
tags: 数论 同余 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18356144)

### [[ARC182F] Graph of Mod of Linear](https://www.luogu.com.cn/problem/AT_arc182_f)

首先判掉 $A\leq 1$ 的情况，接下来默认 $A\geq 2$。原图是基环树森林，数连通块数等价于数环的个数。

比较自然的一点是，把问题分为 $A,N$ 是否互质。因为如果 $A$ 和 $N$ 互质，则 $Ai+B$ 在 $\mod N$ 意义下互不相同，所以每个点都在一个环里。

#### 1. $A\not \perp N$

首先定义 $f^K(i)$ 表示从 $i$ 开始走 $K$ 步会走到哪。显然：

$$f^K(i)=A^Ki+B\sum_{j<K}A^j=A^Ki+B\frac{A^K-1}{A-1}$$

然后是一步神奇的转化。我们希望能只保留在环上的点，而 $f^N(i)$ 一定在环上，并且对于 $i\in[0,N-1]$，$f^N(i)$ 一定涵盖了所有在环上的点（所有在环上的点的 $f$ 已经涵盖了）。所以环上的点可以表示为：

$$(iA^N+B\frac{A^N-1}{A-1})\bmod N$$

对于 $i$ 向后走一步：

$$A(iA^N+B\frac{A^N-1}{A-1})+B\equiv jA^N+B\frac{A^N-1}{A-1}\pmod N$$

取 $d=\gcd(A^N,N)$：

$$
\frac{A^N}{d}j=\frac{A^N}{d}Ai+\frac{A^N}{d}B\pmod \frac{N}{d}
$$

因为 $\frac{A^N}{d}$ 和 $\frac{N}{d}$ 互质，所以：

$$
j=Ai+\frac{A^N}{d}B\pmod \frac{N}{d}
$$

所以问题可以从 $(A,B,N)$ 变为 $(A\bmod \frac N d,B\times\frac{A^N} d\bmod \frac N d,\frac N d)$，转成了互质的情况。

#### 2. $A\perp N$

设 $C_i$ 表示第 $i$ 个点所在的环长，则 $\sum \frac 1 {C_i}$ 就是答案。

从 $i$ 开始走 $K$ 次走回自己，要求的就是：

$$
A^Ki+B\frac{A^K-1}{A-1}\equiv i\pmod N
$$

这个同余方程的最小解。变形一下：

$$
\begin{aligned}
(A^K-1)(i+B\frac{1}{A-1})&\equiv 0\pmod N\\
A^K&\equiv 1\pmod {\frac{N(A-1)}{\gcd(N(A-1),i(A-1)+B)}}\\
\end{aligned}
$$

把模数再变形一下。设 $d=\gcd(A-1,B)$，令 $A'=\frac{A-1}{d}$，$B'=\frac B d$，则：

$$
A^K\equiv 1\pmod {\frac{NA'}{\gcd(NA',iA'+B')}}
$$

此时 $A',B'$ 已经互质，所以 $NA'$ 的贡献只有 $N$ 中的因子。设 $N'=\frac{N}{\gcd(N,A'^N)}$，则：

$$
A^K\equiv 1\pmod {\frac{NA'}{\gcd(N',iA'+B')}}
$$

令 $y=(iA'+B )\bmod N'$，则如果 $i\in [0,N-1]$，则每个 $y$ 都一定恰好被覆盖了 $\frac N {N'}$ 次。所以现在要求的就是：

$$
\begin{aligned}
&\frac N{N'}\sum_{i<N'} C_{\gcd(i,N')}\\
=&\frac N{N'}\sum_{i|N'} C_i\varphi(\frac{N'}{i})
\end{aligned}
$$

其中 $C_i$ 是满足 $A^{C_i}\equiv 1\pmod {\frac{NA'}{\gcd(N',i)}}$ 的最小整数。

首先有性质：

1. 若 $x|y$，则 $\varphi(x)|\varphi(y)$。
2. $A^k\equiv 1\pmod n$ 的解都是 $\varphi(n)$ 的因数并且 $k$ 一定能被表示成 $xk_0$，其中 $k_0$ 表示最小解。

若 $n1\leq n2,n1|n2,n2|N'$，则：

$$
\begin{aligned}
A^{C_1}&\equiv 1\pmod {\frac{NA'}{n1}}\\
A^{C_1}&\equiv 1\pmod {\frac{NA'}{n2}}\\
A^{C_2}&\equiv 1\pmod {\frac{NA'}{n2}}\\
\end{aligned}
$$

所以 $C_1$ 一定是 $C_2$ 的倍数。所以可以用记忆化搜索的方式求解所有的 $C$：考虑把 $\varphi(NA')$ 质因数分解，$C_1$ 初始化为 $\varphi(NA')$，然后不断尝试除 $\varphi(NA')$ 的因子，继续用快速幂判是否合法。

这样求出 $n=1,C_1$ 的答案，然后将 $n$ 乘一个 $\frac{N'}{n}$ 的质因数，令 $C'$ 初始化为 $C_1$，再进行不断尝试除质因子的过程。复杂度一个很松的上界是 $\mathcal O(q\omega_{max}(10^{12})d_{max}(10^{12})))$，$w$ 表示质因子个数，$d$ 表示因数个数。但是完全跑不满满满，所以还是能过的。

```cpp
int n,m,cnt,phi[1000010],pr[300010];
bitset<1000010> vis;
map<int,bool> hash;
vi V[1000010],ve2;
inline int power(int x,int y,int z){int s=1;for(;y;y>>=1,x=x*x%z)if(y&1)s=s*x%z;return s;}
int solve(int A,int B,int N)
{
	if(__gcd(A,N)!=1)
	{
		int d=__gcd(power(A,N,N),N);
		return solve(A%(N/d),power(A,N,N)/d%(N/d)*B%(N/d),N/d);
	}
	if(A==1)return __gcd(B,N);
	if(A==0||N==1)return 1;
	int A_,N_,G=__gcd(A-1,B);
	A_=(A-1)/G,N_=N;
	int ans=0,x=A_,I=0,J=0,Phi=N*A_;
	vector<pii> L,R,ve;ve2.clear(),hash.clear();
	for(int i=2,c;i*i<=x;++i)if(!(x%i)){c=1;while(!(x%i))x/=i,++c;L.eb(mp(i,c));}
	if(x>1)L.eb(mp(x,1));
	x=N;for(int i=2,c;i*i<=x;++i)if(!(x%i)){c=1;while(!(x%i))x/=i,++c;R.eb(mp(i,c));}
	if(x>1)R.eb(mp(x,1));
	while(I<(int)L.size()&&J<(int)R.size())
	{
		if(L[I].fi==R[J].fi)ve.eb(mp(L[I].fi,L[I].se+R[J].se)),++I,++J;
		else if(L[I].fi<R[J].fi)ve.eb(L[I++]);else ve.eb(R[J++]);
	}
	while(I<(int)L.size())ve.eb(L[I++]);
	while(J<(int)R.size())ve.eb(R[J++]);
	for(auto [x,y]:ve){Phi=Phi/x*(x-1);y>1?ve2.eb(x),0:0;for(auto p:V[x-1])ve2.eb(p);}
	sort(ve2.begin(),ve2.end()),ve2.erase(unique(ve2.begin(),ve2.end()),ve2.end());
	function<void(int,int)> dfs=[&](int n,int C)
	{
		if(hash.find(n)!=hash.end())return;
		hash[n]=1;
		for(auto x:ve2)while(C%x==0&&power(A,C/x,N*A_/n)<=1)C/=x;
		ans+=N/N_*phi[N_/n]/C;
		for(auto p:V[N_/n])dfs(n*p,C);
	};
	dfs(1,Phi);
	return ans;
}
inline void mian()
{
	read(n,m),phi[1]=1;int x,y;
	for(int i=2;i<=1000000;++i)
	{
		if(!vis[i])phi[i]=i-1,pr[++cnt]=i;
		for(int j=1;j<=cnt&&pr[j]*i<=1000000;++j)
		{
			vis[i*pr[j]]=1,phi[i*pr[j]]=phi[i]*(pr[j]-1);
			if(i%pr[j]==0){phi[i*pr[j]]+=phi[i];break;}
		}
	}
	for(int i=1;i<=cnt;++i)for(int j=pr[i];j<=1000000;j+=pr[i])V[j].eb(pr[i]);
	for(int i=1;i<=m;++i)read(x,y),write(solve(x,y,n),'\n');
}
```