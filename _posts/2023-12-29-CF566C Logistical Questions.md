---
layout: post
title: CF566C Logistical Questions
subtitle: 题解
tags: 题解 点分治 数学 CF
show: true
---

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17935385.html)

[CF566C Logistical Questions](https://www.luogu.com.cn/problem/CF566C)

好强的题，感觉完全想不到。

如果对于每个点都计算答案的话复杂度是 $\mathcal O(n^2)$，但是由于题目中给了一个 $\frac{3}{2}$ 次方这么一个非常恶心人的东西，这个算法基本没有优化空间，所以考虑换一种思路，先选择一个点，然后尝试对答案进行调整。

对于一个点，它的答案是：

$$
f(u)=\sum_{i\in u}a_idep_i^{\frac{3}{2}}
$$

假设我们把当前答案尝试移动到 $u$ 的一个儿子 $v$，新的答案为：

$$
g(x)=\sum_{i\in v}a_i(dep_i-x)^{\frac{3}{2}}+\sum_{i\notin v}a_i(dep_i+x)^{\frac{3}{2}}
$$

其中 $x$ 可以表示 $(u,v)$ 的边权。这个东西好像还是没有什么性质，我们尝试对它求导：

$$
\begin{aligned}
g'(x)&=-\frac 3 2\sum_{i\in v}a_i(dep_i-x)^{\frac{1}{2}}+\frac 3 2\sum_{i\notin v}a_i(dep_i+x)^{\frac{1}{2}}\\

&=\frac 3 2(\sum_{i\notin v}a_i(dep_i+x)^{\frac{1}{2}}-\sum_{i\in v}a_i(dep_i-x)^{\frac{1}{2}})
\end{aligned}
$$

还是没有什么性质，我们尝试再次求导：

$$
g''(x)=\frac 3 4(\sum_{i\notin v}a_i(dep_i+x)^{-\frac 1 2}+\sum_{i\in v}a_i(dep_i-x)^{-\frac 1 2})
$$

因为 $x<dep_u$，所以二阶导数恒正，这说明这个函数是凸的！所以对于每一条边 $(u,v)$，如果答案靠近 $u$，则 $g'(x)$ 单增，所以 $g(x)$ 是单调递增并且增速越来越快，极小值不在 $x>0$ 的部分，否则 $g'(x)$ 单减，说明极值在 $x>0$ 的部分（在 $g'(x)=0$ 处）。

这个结论是对于一条边来说的，但是它扩展到树上也是对的。假设答案在 $u$，则对于所有的 $(u,v)$ 上述结论成立。对于其余的部分，随着答案尝试向下移动，$g'(x)$ 的前一部分 $i\notin v$ 的点会变得越来越多，$i\in v$ 的点会变得越来越少，相应的，随着向下走，整体来看 $g'(x)$ 是变大的，所以我们得到关键结论：**在树上一个点越远离答案点，它的答案越劣**（这里的点可以不只是树上的点，可以是边中间的实数的点）。

所以我们可以先选择一个点，然后尝试向下移动，判断的依据就是 $g'(0)$，如果其大于 $0$ 则说明答案不在这棵子树里，否则在，向下递归。但是这样复杂度不对，可以用点分治优化：每次选取重心作为当前的答案，然后尝试移动到重心的其余儿子，此时上述结论显然也是正确的。总复杂度 $\mathcal O(n\log n)$。

```cpp
	int n,cnt,head[200010],dep[200010],to[400010],v[400010],nex[400010],a[200010];
	inline void add(int x,int y,int z){to[++cnt]=y,v[cnt]=z,nex[cnt]=head[x],head[x]=cnt;}
	namespace Starch
	{
		int siz[200010],root,all,maxi;
		double delta,delta2,ans=INF,sum;
		bool vis[200010];
		void find(int x,int fa)
		{
			siz[x]=1;int maxn=0;
			for(int i=head[x];i;i=nex[i])
			{
				if(to[i]==fa||vis[to[i]])continue;
				dep[to[i]]=dep[x]+v[i],find(to[i],x),siz[x]+=siz[to[i]],maxn=max(maxn,siz[to[i]]);
			}
			if(max(maxn,all-siz[x])<=all/2)root=x;
		}
		void dfs(int x,int fa,int dep)
		{
			sum+=a[x]*pow(dep,1.5),delta+=a[x]*sqrt(dep);
			for(int i=head[x];i;i=nex[i])if(to[i]!=fa)dfs(to[i],x,dep+v[i]);
		}
		void dfs2(int x,int fa,int dep)
		{
			delta2+=a[x]*sqrt(dep);
			for(int i=head[x];i;i=nex[i])if(to[i]!=fa)dfs2(to[i],x,dep+v[i]);
		}
		void starch(int x)
		{
			find(x,0),find(x=root,0),vis[x]=1,sum=delta=0,dfs(x,0,0);
			if(sum<ans)ans=sum,maxi=x;
			for(int i=head[x];i;i=nex[i])
			{
				if(vis[to[i]])continue;
				delta2=0,dfs2(to[i],x,v[i]);
				if(delta-2*delta2<0)all=siz[to[i]],starch(to[i]);
			}
		}
	}
	using namespace Starch;
	inline void mian()
	{
		read(n),all=n;int x,y,z;
		for(int i=1;i<=n;++i)read(a[i]);
		for(int i=1;i<n;++i)read(x,y,z),add(x,y,z),add(y,x,z);
		starch(1),cout<<maxi<<" ";
		printf("%.7lf",ans);
	}
```