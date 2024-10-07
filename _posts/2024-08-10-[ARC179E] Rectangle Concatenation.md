---
layout: post
title: ARC179E Rectangle Concatenation
subtitle: 题解
tags: 题解 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18352711)

### [[ARC179E] Rectangle Concatenation](https://www.luogu.com.cn/problem/AT_arc179_e)

唐完了。

稍微观察一下发现矩形只有两种形态。考虑暴力：从每个 $i$ 开始向后扫，设 $f_{j,0}$ 表示能否拼在左右，$f_{j,1}$ 表示能否拼在上下。设 $S_{l,r}$ 表示 $[l,r]$ 内矩形的面积和，没想到用面积判就败了：

$$
\begin{aligned}
f_{j,0}=(f_{j-1,0}\wedge x_j=x_{j-1})\vee(f_{j-1,1}\wedge S(i,j-1)=y_{j-1}x_j)\\
f_{j,1}=(f_{j-1,1}\wedge y_j=y_{j-1})\vee(f_{j-1,0}\wedge S(i,j-1)=x_{j-1}y_j)\\
\end{aligned}
$$

考虑优化：把 $j$ 从左向右扫，维护合法的 $i$。设 $S_0$ 表示 $f_{j,0}$ 合法的 $i$ 集合，$S_1$ 同理。发现 $j+1$ 的时候，观察式子左半部分，$S_0,S_1$ 可能会做一个清空操作。

右半部分看成面积前缀和相减，合法的 $i$ 是唯一的。是一个单点加入，容易维护。还需要拿一个桶维护 $|S_0\cup S_1|$。时间复杂度 $\mathcal O(n\log n)$ 或者 $\mathcal O(n)$。

```cpp
int n,s[300010],vis[300010],f0[300010],f1[300010],ans,sum;
pii a[300010];
inline void add(int x){sum+=!vis[x]++;}
inline void del(int x){sum-=!--vis[x];}
map<int,int> hash;
inline void mian()
{
	read(n),hash[0]=0;vi v0,v1;
	for(int i=1;i<=n;++i)read(a[i].fi,a[i].se),s[i]=s[i-1]+a[i].fi*a[i].se,hash[s[i]]=i;
	ans=1,add(1),add(1),v0.eb(1),v1.eb(1),f0[1]=f1[1]=1;
	for(int i=2;i<=n;++i)
	{
		int va0=1;
		if(hash.find(s[i-1]-a[i].se*a[i-1].fi)!=hash.end())va0=f0[hash[s[i-1]-a[i].se*a[i-1].fi]+1];
		int va1=1;
		if(hash.find(s[i-1]-a[i].fi*a[i-1].se)!=hash.end())va1=f1[hash[s[i-1]-a[i].fi*a[i-1].se]+1];
		if(a[i].fi!=a[i-1].fi){for(auto p:v0)del(p),f0[p]=0;v0.clear();}
		if(a[i].se!=a[i-1].se){for(auto p:v1)del(p),f1[p]=0;v1.clear();}
		int S=s[i-1]-a[i].fi*a[i-1].se;
		if(hash.find(S)!=hash.end()&&!f0[hash[S]+1]&&va1)
		add(hash[S]+1),v0.eb(hash[S]+1),f0[hash[S]+1]=1;
		S=s[i-1]-a[i].se*a[i-1].fi;
		if(hash.find(S)!=hash.end()&&!f1[hash[S]+1]&&va0)
		add(hash[S]+1),v1.eb(hash[S]+1),f1[hash[S]+1]=1;
		if(!f0[i])add(i),v0.eb(i),f0[i]=1;
		if(!f1[i])add(i),v1.eb(i),f1[i]=1;
		ans+=sum;
	}
	write(ans);
}
```