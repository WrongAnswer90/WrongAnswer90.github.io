---
layout: post
title: ARC173F Select and Split
subtitle: 题解
tags: 数数 行列式 生成函数 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18391786)

### [[ARC173F] Select and Split](https://www.luogu.com.cn/problem/AT_arc173_f)

~~在 Kevin 题解的基础上解释了一下~~。分裂这个过程感觉很不自然，考虑倒过来做合并。

经过简单的观察，可以发现一个集合的属性只和在 $[1,A]$ 内的元素个数和 $[A+1,A+B]$ 内的元素个数有关，分别设其为 $a_i,b_i$。

合并两个点的方案数是 $a_ib_j+a_jb_i$。合并两个集合 $S,T$ 的方案数可以看成从 $S,T$ 两个集合内各自任选一个点合并，拆到每个点的贡献上，即：

$$
\sum_{i\in S}a_i\sum_{j\in T}b_j=\sum_{i\in S}\sum_{j\in T}a_ib_j
$$

所以可以建图，$i,j$ 之间连 $a_ib_j+a_jb_i$ 条边，这样这个图的生成树个数就是合并的方案数。操作顺序的影响是平凡的，可以最后乘一个 $(n-1)!$ 解决。

因为数字是有标号，而有标号问题可以想到用二元指数型生成函数解决。所以现在只需要把有了 $a_i,b_i$ 之后，合并的方案数比较简单的表示出来。根据矩阵树定理，设 $L_{i,j}=[i=j](Ab_j+Ba_i),X_{i,j}=a_ib_j,Y_{i,j}=a_ib_j$，其中 $i,j>1$，则 $\det(L-X-Y)$ 即为答案。可以发现这个矩阵的形式比较简单（好像是对角矩阵加上若干个秩为 $1$ 的矩阵都可以这么做？），考虑降阶公式（太神奇了！），新设一个矩阵：

$$
\begin{vmatrix}
 
 L-X-Y & 0 & 0\\
 b & 1 & 0\\
 a & 0 & 1
\end{vmatrix}
$$

其中 $L-X-Y$ 是矩阵，$a,b$ 是行向量，第一排的两个 $0$ 都是空的列向量。容易发现该矩阵的行列式和 $G$ 相同。可以把 $a,b$ 都对应位相减到 $L-X-Y$ 上，于是有：

$$
\begin{vmatrix}
 L-X-Y & 0 & 0\\
 b & 1 & 0\\
 a & 0 & 1
\end{vmatrix}
=
\begin{vmatrix}
 L & a^T & b^T\\
 b & 1 & 0\\
 a & 0 & 1
\end{vmatrix}
$$

其中 $a^T,b^T$ 是列向量。因为 $L$ 是对角矩阵，所以可以直接消元：

$$
\begin{vmatrix}
 L & a^T & b^T\\
 b & 1 & 0\\
 a & 0 & 1
\end{vmatrix}
=
\begin{vmatrix}
 L & a^T & b^T\\
 0 & 1-\sum \frac{a_ib_i}{a_iB+b_iA} & -\sum \frac {b_i^2}{a_iB+b_iA}\\
 0 & -\sum \frac{a_i^2}{a_iB+b_iA} & 1-\sum \frac{a_ib_i}{a_iB+b_iA}
\end{vmatrix}
$$

然后分类讨论一下，就可以得到行列式为：

$$
\prod_i v_i(1-2\sum_{i}\frac{a_ib_i}{v_i}+\sum_{i,j}\frac{a_ia_jb_ib_j}{v_iv_j}-\sum_{i,j}\frac{a_i^2b_j^2}{v_iv_j})
$$

其中 $v_i=a_iB+b_iA$。其实后面的部分就相对比较平凡了。写出四个部分的指数型生成函数：

$$
\begin{aligned}
F_1(x,y)&=\sum_{i,j\geq 0}\frac{x^iy^j}{i!j!}(Aj+Bi)\\&=Ae^xye^y+Bxe^xe^y\\&=e^{x+y}(Ay+Bx)\\
W_1&=[x^Ay^B]e^xe^yF_1^{n-1}(x,y)\\
&=[x^Ay^B]e^{n(x+y)}(Ay+Bx)^{n-1}
\end{aligned}
$$

因为 $xe^x=\sum_{i\geq 1}i\frac{x^i}{i!}$。第二个式子中乘 $e^{x+y}$ 是因为需要统计第一个集合（计算行列式时删掉了）的贡献。

$$
\begin{aligned}
F_2(x,y)&=\sum_{i,j\geq 0}\frac{x^iy^j}{i!j!}ij=xye^{x+y}\\
W_2&=-2(n-1)[x^Ay^B]e^{x+y}F_2(x,y)F_1^{n-2}(x,y)\\&=-2(n-1)[x^Ay^B]e^{n(x+y)}xy(Ay+Bx)^{n-2}
\end{aligned}
$$

$n-1$ 是因为选一个做 $a_ib_i$ 的贡献，其余的做 $Ab_i+Ba_i$ 的贡献。

对于第三第四部分，可以发现如果 $i=j$ 则两部分的贡献抵消了，所以可以删去的 $i=j$（其实只是为了简单一点）。

$$
\begin{aligned}
W_3&=(n-1)^{\underline2}[x^Ay^B]e^{x+y}F_2^2(x,y)F_1^{n-3}(x,y)\\&=(n-1)^{\underline2}[x^Ay^B]e^{n(x+y)}x^2y^2(Ay+Bx)^{n-3}
\end{aligned}
$$

因为 $i,j$ 和 $j,i$ 都要贡献，所以是 $(n-1)^{\underline2}$ 而不是 $\binom{n-1}{2}$。

$$
\begin{aligned}
F_3(x,y)&=\sum_{i,j\geq 0}\frac{x^iy^j}{i!j!}i^2=e^y\sum_{i\geq 0}\frac{x^i}{i!}(i^{\underline 2}+i^{\underline 1})\\
&=e^y(x^2e^x+xe^x)=x(x+1)e^{x+y}\\
F_4(x,y)&=y(y+1)e^{x+y}\\
W_4&=-(n-1)^{\underline2}[x^Ay^B]e^{x+y}F_3(x,y)F_4(x,y)F_1^{n-3}(x,y)\\
&=-(n-1)^{\underline2}[x^Ay^B](Ay+Bx)^{n-3}xy(x+1)(y+1)e^{n(x+y)}\\
\end{aligned}
$$

使用了 $x^te^x=\sum_{i\geq t}i^{\underline t}\frac{x^i}{i!}$。最终答案即为 $\frac{(n-1)!}{n!}A!B!(W_1+W_2+W_3+W_4)$，$(n-1)!$ 是确定操作顺序，$n!$ 是消除集合之间的顺序。可以用组合数把除了 $e^{n(x+y)}$ 以外的部分展开然后 $\mathcal O(n)$ 算。

```cpp
int n,A,B,W1,W2,W3,W4,fr[600010],inv[600010],f[600010],pwa[600010],pwb[600010],pwn[600010];
inline int C(int n,int m){return m<0||m>n?0:Cmul(fr[n],inv[m],inv[n-m]);}
inline void mian()
{
	read(n,A,B),fr[0]=inv[0]=pwa[0]=pwb[0]=pwn[0]=1;
	for(int i=1;i<=600000;++i)
	{
		fr[i]=Cmul(fr[i-1],i),pwa[i]=Cmul(pwa[i-1],A);
		pwb[i]=Cmul(pwb[i-1],B),pwn[i]=Cmul(pwn[i-1],n);
	}
	inv[600000]=power(fr[600000],MOD-2);
	for(int i=599999;i>0;--i)inv[i]=Cmul(inv[i+1],i+1);
	for(int i=0;i<=n-1&&i<=A;++i)if(n-1-i<=B)
	Madd(W1,Cmul(pwn[A-i],pwn[B-(n-1-i)],inv[A-i],inv[B-(n-1-i)],C(n-1,i),pwb[i],pwa[n-1-i]));
	for(int i=0;i<=n-2&&i+1<=A;++i)if(n-2-i+1<=B)
	Madd(W2,Cmul(pwn[A-1-i],pwn[B-1-(n-2-i)],inv[A-1-i],inv[B-1-(n-2-i)],C(n-2,i),pwb[i],pwa[n-2-i]));
	Mmul(W2,2,n-1);
	for(int i=0;i<=n-3&&i+2<=A;++i)if(n-3-i+2<=B)
	Madd(W3,Cmul(pwn[A-2-i],pwn[B-2-(n-3-i)],inv[A-2-i],inv[B-2-(n-3-i)],C(n-3,i),pwb[i],pwa[n-3-i]));
	Mmul(W3,n-2,n-1);
	for(int i=0;i<=n-3&&i+1<=A;++i)if(n-3-i+1<=B)
	for(int j=0;j<2&&j+i+1<=A;++j)for(int k=0;k<2&&k+n-3-i+1<=B;++k)
	Madd(W4,Cmul(pwn[A-1-j-i],pwn[B-1-k-(n-3-i)],inv[A-1-j-i],inv[B-1-k-(n-3-i)],C(n-3,i),pwb[i],pwa[n-3-i]));
	Mmul(W4,n-2,n-1);
	write(Cmul(power(n,MOD-2),fr[A],fr[B],Cdel(Cadd(W1,W3),Cadd(W2,W4))));
}
```