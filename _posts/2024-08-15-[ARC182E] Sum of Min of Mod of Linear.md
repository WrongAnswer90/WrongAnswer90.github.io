---
layout: post
title: ARC182E Sum of Min of Mod of Linear
subtitle: 题解
tags: 万欧 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18360984)

### [[ARC182E] Sum of Min of Mod of Linear](https://www.luogu.com.cn/problem/AT_arc182_e)

首先把 $A$ 排序去重。考虑什么时候 $a_i$ 会成为答案。

对于 $v_i=C\times i\bmod m$，一定是希望找一个尽量小的 $j$ 满足 $v_i+a_j\geq m$，这样可以恰好发生一次进位，贡献是 $m-a_j$。如果找不到 $j$，就会选择 $a_1$，贡献是 $a_1$。

这样思路就很清晰了：需要计算 $g_i$ 表示加了 $a_i$ 恰好进位，并且加了 $a_{i-1}$ 不进位的 $v_i$ 的数量，差分一下变成计算 $f_i$ 表示加了 $a_i$ 恰好进位的数的个数，则 $g_i=f_i-f_{i-1}$。这个可以看成：

$$
\sum_{j=0}^K \lfloor\frac{C\times j+a_i}{m}\rfloor-
\sum_{j=0}^K \lfloor\frac{C\times j}{m}\rfloor
$$

这样如果恰好进位了就会多算 $1$。最后再统计答案即可。总复杂度 $\mathcal O(n\log V)$。

```cpp
struct Node{int U,R,x;Node(){U=R=x=0;}};
inline Node operator +(const Node x,const Node y)
{
	Node z;z.U=x.U+y.U,z.R=x.R+y.R;
	z.x=x.x+y.x+x.U*y.R;
	return z;
}
inline Node power(Node x,int y)
{
	Node a=Node();
	for(;y;y>>=1,x=x+x)if(y&1)a=a+x;
	return a;
}
inline int div(int x,int y,int z,int t){return floor(((long double)1.0*x*y+z)/t);}
Node solve(int P,int Q,int R,int L,Node x,Node y)//x:up y:right
{
	if(!L)return Node();
	if(P>=Q)return solve(P%Q,Q,R,L,x,power(x,P/Q)+y);
	int m=div(L,P,R,Q);
	if(!m)return power(y,L);
	int cnt=L-div(Q,m,-R-1,P);
	return power(y,(Q-R-1)/P)+x+solve(Q,P,(Q-R-1)%P,m-1,y,x)+power(y,cnt);
}
int n,m,C,K,a[100010],f[100010],ans;
inline void mian()
{
	read(n,m,C,K),C%=m;Node U,R;U.U=1,R.R=1;
	for(int i=1;i<=n;++i)read(a[i]),a[i]%=m;
	sort(a+1,a+1+n),n=unique(a+1,a+1+n)-a-1;
	for(int i=1;i<=n;++i)f[i]=solve(C,m,a[i],K-1,U,R).x-solve(C,m,0,K-1,U,R).x;
	ans=K*(K-1)/2*C-m*solve(C,m,0,K-1,U,R).x;
	ans+=a[1]*(K-f[n]);//不进位
	for(int i=1;i<=n;++i)ans+=(f[i]-f[i-1])*(a[i]-m);//进位
	write(ans);
}
```