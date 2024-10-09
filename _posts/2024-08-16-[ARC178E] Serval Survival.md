---
layout: post
title: ARC178E Serval Survival
subtitle: 题解
tags: 题解 数数 多项式 生成函数 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18362386)

### [[ARC178E] Serval Survival](https://www.luogu.com.cn/problem/AT_arc178_e)

非常生气，点开一道看起来很正常的计数，推着推着就发现需要多项式/fn。

首先对于“撞上了之后调头”这种东西有经典的思想：可以看成是互相穿过并没有调头。但是因为要求第 $i$ 只猫走过的路，所以可以看成是和撞上的猫互换身份。

手玩一下可以发现，如果第 $i$ 只猫向左走，它会和 $a_i$ 左边的第一只向右的猫互换身份，然后和 $a_i$ 右边的第一只向左的猫互换身份，然后和 $a_i$ 左边的第二只猫向右的猫互换身份$\dots$，位置指的都是初始位置。

第 $i$ 只猫走的路程就等于最后一只和他交换身份的猫直走能走的路程（向右为 $L-a_i$，向左为 $a_i$）。据此，我们可以找出第 $i$ 只猫的策略。

设 $a_i$ 左边向右走的猫的集合为 $L$，$a_i$ 右边向左走的猫的集合为 $R$，分类讨论：

#### 1.$\lvert L\rvert >\lvert R\rvert $

如果他向左走，最后和他交换身份的猫是 $L$ 中从右向左第 $\lvert R\rvert +1$ 只猫。如果他向右走则是 $L$ 中从右向左第 $\lvert R\rvert$ 只猫。因为是向右走，显然靠左边的猫一定走的更远，所以此时他一定会向左走。

#### 2.$\lvert L\rvert<\lvert R\rvert$

和上文同理，一定会选择向右走。

#### 3.$\lvert L\rvert=\lvert R\rvert$

此时他有两种策略：选择 $L$ 中最靠左的 $L-a_i$ 和 $R$ 中最靠右的 $a_i$，这是唯一有决策的点，需要选较大值。

考虑暴力 DP：设 $ans_i$ 表示第 $i$ 只猫的答案。

第一种情况 $\lvert L\rvert>\lvert R\rvert$：

$$
\begin{aligned}
ans_i&=\sum_{j=1}^i(L-a_j)2^{j-1}\sum_{k=0}^{i-j-1}\binom{i-j-1}{k}\binom{n-i}{k}\\
&=\sum_{j=1}^i(L-a_j)2^{j-1}\sum_{k=0}^{i-j-1}\binom{i-j-1}{k}\binom{n-i}{n-i-k}\\
&=\sum_{j=1}^i(L-a_j)2^{j-1}\binom{n-j-1}{n-i}\end{aligned}
$$

上面使用了范德蒙德卷积化简。发现组合数可以拆成 $i,j,i-j$ 相关的式子，可以 FFT。

对于 $\lvert L\rvert<\lvert R\rvert$ 的情况同理，可以把整个序列倒过来做一遍。

第二种情况 $\lvert L\rvert=\lvert R\rvert$，假设最大值在右侧取到：

$$
\begin{aligned}
ans_i&=\sum_{j=i+1}^n a_j\sum_{k=0}^{j-i-1}\binom{j-i-1}{k}\binom{i-L_j}{k+1}\\
&=\sum_{j=i+1}^n a_j\sum_{k=0}^{j-i-1}\binom{j-i-1}{k}\binom{i-L_j}{i-L_j-k-1}\\
&=\sum_{j=i+1}^n a_j\binom{j-1-L_j}{i-1-L_j}
\end{aligned}
$$

$L_i$ 表示最小的 $j$ 满足 $m-a_j<a_i$。还是范德蒙德卷积，但是这个式子并不好用 FFT 处理。考虑写成生成函数形式，令 $Ans(x)$ 表示 $\sum_{i=0}^n ans_i x^i$，则：

$$
Ans(x)=\sum_{j=2}^n a_j (1+x)^{j-1-L_j}x^{1+L_j}
$$

$(1+x)^{j-1-L_j}$ 是为了构造组合数系数，$x^{1+L_j}$ 是因为 $j$ 对 $i$ 贡献，$i$ 的下指标是 $i-L_j-1$，需要做一个整体平移。

因为 $L$ 是单减的，$i-1-L_i$ 是单增的，可以分治解决：设 $A_i$ 表示 $L_i+1$，$B_i$ 表示 $i-1-L_i$，$(1+x)^{B_l}x^{A_r}F_{l,r}(x)$ 表示 $l,r$ 区间对答案的贡献。合并：

$$
F_{l,r}(x)=F_{l,mid}(x)x^{A_r-A_{mid}}+F_{mid+1,r}(x)(1+x)^{B_{mid+1}-B_l}
$$

合并的时候暴力 FFT 即可。这样最终答案就是 $(1+x)^{B_{pos}}x^{A_n}F_{pos,n}(x)$，$pos$ 表示最小的 $i$ 满足 $i-1-L_i>0$。因为 $A,B$ 的值域都是 $n$ 的，一个点最多被 $\log n$ 次 FFT 包含，可以分析出复杂度是 $\mathcal O(n\log^2 n)$。代码写得很烂。

```cpp
namespace Poly
{
    const int MAXN=400000;
    int Shape,Invn[MAXN+10],R[MAXN*4+10],Prt[MAXN*4];
    inline void init()
    {
        Invn[0]=1;
        for(int i=1;i<=MAXN;++i)Invn[i]=Cmul(Invn[i-1],i);
        int tmp=power(Invn[MAXN],MOD-2);
        for(int i=MAXN;i>=1;--i)Invn[i]=Cmul(tmp,Invn[i-1]),Mmul(tmp,i);
    }
    inline int inv(int x){return x<=MAXN?Invn[x]:power(x,MOD-2);}
    inline void NTT(vi&A,int n,int opt)
    {
        static ull B[MAXN*4+10];ull iv=power(n,MOD-2);A.resize(n);
        for(int i=0;i<n;++i)B[i]=A[R[i]];
        for(int mid=1;mid<n;mid<<=1)
        {
            for(int j=0;j<n;j+=mid<<1)
            {
                for(int k=j;k<j+mid;++k)
                {
                    ull x=B[k],y=Prt[mid+k-j]*B[k+mid]%MOD;
                    B[k]=x+y,B[k+mid]=x+MOD-y;
                }
            }
        }
        if(opt)for(int i=0;i<n;++i)A[i]=B[i]%MOD;
        else{reverse(B+1,B+n);for(int i=0;i<n;++i)A[i]=Cmul(B[i]%MOD,iv);}
    }
    inline void init(int lim)
    {
        if(lim==Shape)return;
        int n=lim/2;Shape=lim;
        for(int i=0;i<lim;++i)R[i]=(R[i>>1]>>1)|((i&1)?n:0);
        for(int i=1;i<lim;i<<=1)
        {
            int wm=power(Root,(MOD-1)/(i<<1));Prt[i]=1;
            for(int j=1;j<i;++j)Prt[i+j]=Cmul(Prt[i+j-1],wm);
        }
    }
    inline vi FFT(vi A,vi B,int N1=-1,int N2=-1)
    {
        if(N1!=-1)A.resize(N1+1);if(N2!=-1)B.resize(N2+1);
        int n=A.size()-1,m=B.size()-1,N=1,len=0;
        while(N<=n+m)N<<=1,++len;
        init(N),NTT(A,N,1),NTT(B,N,1);
        for(int i=0;i<N;++i)A[i]=Cmul(A[i],B[i]);
        return NTT(A,N,0),A.resize(n+m+1),A;
    }
}
int n,m,dl,fr[100010],inv[100010],pre[100010],suf[100010],ans[100010];
int a[100010],L[100010],R[100010],pw[100010],b[100010],res[100010];
inline int C(int n,int m){return m<0||m>n?0:Cmul(fr[n],inv[m],inv[n-m]);}
vi add(vi L,vi R)
{
	int N=max(L.size(),R.size());L.resize(N),R.resize(N);
	for(int i=0;i<N;++i)Madd(L[i],R[i]);
	return L;
}
vi move(vi A,int x)
{
	A.resize(A.size()+x);
	for(int i=A.size()-1;i>=x;--i)A[i]=A[i-x];
	for(int i=0;i<x;++i)A[i]=0;
	return A;
}
vi Binom(vi A,int x)
{
	vi B(x+1);
	for(int i=0;i<=x;++i)B[i]=C(x,i);
	A=Poly::FFT(A,B);
	return A;
}
vi A,B;
vi solve(int l,int r)
{
	if(l==r)return {a[l]};
	int mid=l+((r-l)>>1);
	return add(move(solve(l,mid),B[mid]-B[r]),
	Binom(solve(mid+1,r),A[mid+1]-A[l]));
}
void solve()
{
	A.resize(n+1),B.resize(n+1);
	int pos=0;
	for(int i=1;i<=n;++i)
	{
		if(i-1-L[i]>=0)A[i]=i-1-L[i],B[i]=L[i]+1;
		else A[i]=B[i]=0,pos=i;
	}
	for(int i=pos+1;i<=n;++i)Mdel(res[i],a[i]);
	vi D;
	if(pos<n)D=move(Binom(solve(pos+1,n),A[pos+1]),B[n]);
	for(int i=1;i<(int)D.size();++i)Madd(res[i],D[i]);
}
inline void mian()
{
	Poly::init(),read(n,m),fr[0]=inv[0]=pw[0]=1;
	for(int i=1;i<=n;++i)fr[i]=Cmul(fr[i-1],i),pw[i]=Cmul(pw[i-1],2);
	inv[n]=power(fr[n],MOD-2);
	for(int i=n-1;i>0;--i)inv[i]=Cmul(inv[i+1],i+1);
	for(int i=1;i<=n;++i)read(a[i]),b[i]=m-a[i];
	a[0]=-inf,a[n+1]=inf,b[0]=inf,b[n+1]=-inf;
	for(int i=n,r=0;i>=1;R[i--]=r)
	{
		suf[i]=Cadd(suf[i+1],Cmul(power(2,n-i),a[i]));
		while(a[r+1]<b[i])++r;
	}
	for(int i=1,l=n+1;i<=n;L[i++]=l)
	{
		pre[i]=Cadd(pre[i-1],Cmul(power(2,i-1),b[i]));
		while(b[l-1]<=a[i])--l;
	}
	write(Cadd(max(a[1],b[1])%MOD,suf[2]),'\n');
	if(n==1)return;
	if(n==2)return write(Cadd(max(a[n],b[n])%MOD,pre[n-1]));
	for(int i=n;i>1;--i)Madd(dl,Cmul(a[i],pw[n-i]));
	vi X={0},Y={0},Z={0};Z.resize(n+1);
	for(int i=1;i<=n;++i)X.eb(Cmul(b[i],pw[i-1],fr[n-i-1]));
	for(int i=1;i<=n;++i)Y.eb(inv[i-1]);
	for(int i=1;i<n;++i)Z[i]=Cmul(a[n-i+1],pw[i-1],fr[n-i-1]);
	X=Poly::FFT(X,Y),Z=Poly::FFT(Z,Y);
	for(int i=2;i<n;++i)
	{
		ans[i]=Cadd(Cmul(X[i],inv[n-i]),max(a[i],b[i])%MOD,suf[i+1],pre[i-1],Cmul(inv[i-1],Z[n-i+1]));
		Madd(dl,Cmul(b[i-1],pw[i-2]));
		Mdel(dl,Cmul(a[i],pw[n-i]));
		Mdel(ans[i],dl);
	}
	solve();
	for(int i=1;i<=n;++i)Madd(ans[i],res[i]);
	memset(res,0,sizeof(res));int v=a[n];
	for(int i=1;i<=n;++i)a[i]=b[n-i+1],L[i]=n-R[n-i+1]+1;
	solve();
	for(int i=1;i<=n;++i)Madd(ans[i],res[n-i+1]);
	for(int i=2;i<n;++i)write(ans[i],'\n');
	if(n>1)write(Cadd(max(v,b[n])%MOD,pre[n-1]));
}
```