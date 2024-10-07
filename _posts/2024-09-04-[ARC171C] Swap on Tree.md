---
layout: post
title: ARC171C Swap on Tree
subtitle: 题解
tags: 题解 数数 多项式 AT
show: true
---

# [My Blogs]()

### [[ARC171C] Swap on Tree](https://www.luogu.com.cn/problem/AT_arc171_c)

科技改变生活。以 6ms 的速度拿下了目前最优解（

如果已经确定了 $u$ 的一个儿子 $v$ 内部的操作顺序，考虑在某个时刻交换 $(u,v)$。设 $a[1,k]$ 是操作 $v$ 子树内部时 $v$ 上面的颜色，可以发现在第 $i$ 个时刻交换和在第 $j$ 个时刻交换，方案本质不同的充要条件是 $a_i\not= a_j$。

考虑 $i$ 和 $i$ 的所有儿子，如果选择了 $S$ 作为要与 $i$ 交换的子树集合，则权值是 $|S|!\prod_{i\in S}(\sum_j jf_{i,j})\prod_{i\notin S}(\sum_j f_{i,j})$，其中 $f_{i,j}$ 表示 $i$ 子树内颜色改变次数是 $j$ 的方案数。$|S|!$ 是因为在确定 $i$ 和集合内所有点的操作顺序，$jf_{i,j}$ 是因为对于在集合里的点有颜色切换次数个本质不同的交换方案。

暴力做是 $\mathcal O(n^2)$ 的。设 $g_{i,0}=\sum_i f_{x,i},g_{i,1}=\sum_i if_{x,i}$。设多项式 $F_u(x)=g_{u,0}+g_{u,1}x$，则 $f_{u,k+1}$ 即为 $k![x^k]\prod_{v\in\text{son}(u)}F_v(x)$。

注意到要乘的多项式的总个数是 $\mathcal O(n)$ 的，所以可以用你喜欢的方法比如分治 NTT 或者先 ln 再 exp 做到 $\mathcal O(n\log^2 n)$ 或者 $\mathcal O(n\log n)$。

```cpp
int n,fr[200010],g[200010][2];
vi T[200010];
vp F;
vi solve(int l,int r)
{
	if(l==r)return {F[l].fi,F[l].se};
	int mid=(l+r)>>1;
	return FFT(solve(l,mid),solve(mid+1,r));
}
void dfs(int x,int fa=0)
{
	for(auto to:T[x])if(to!=fa)dfs(to,x);
	F.clear();
	for(auto to:T[x])if(to!=fa)F.eb(mp(g[to][0],g[to][1]));
	if(F.empty())return g[x][0]=g[x][1]=1,void();
	vi ve=solve(0,F.size()-1);
	for(int i=1;i<=ve.size();++i)Mmul(ve[i-1],fr[i-1]),Madd(g[x][1],Cmul(ve[i-1],i)),Madd(g[x][0],ve[i-1]);
}
inline void mian()
{
	read(n),init(),fr[0]=1;int x,y;
	for(int i=1;i<=n;++i)fr[i]=Cmul(fr[i-1],i);
	for(int i=1;i<n;++i)read(x,y),T[x].eb(y),T[y].eb(x);
	dfs(1),write(g[1][0]);
}
```