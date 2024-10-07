---
layout: post
title: P3521 [POI2011] ROT-Tree Rotations
subtitle: 题解
tags: 题解 线段树合并 Luogu
show: true
---

[P3521 [POI2011] ROT-Tree Rotations](https://www.luogu.com.cn/problem/P3521)

首先合并两棵子树的时候只关心子树内值的个数，并不关心子树内具体是什么顺序，引导从下向上线段树合并计算代价。

每一个值只会出现一次，首先每个叶子节点开一棵动态开点值域为 $1-n$ 的线段树维护，初始只有自己的值的位置为 $1$。

然后对于每个非叶子节点，从下至上合并，两棵子树有两种方案，计算时使用 $sum1$ 和 $sum2$ 维护两种合并方式的代价，计算即为 $A$ 子树中 $l$ 到 $mid$ 值的个数和 $mid+1$ 到 $r$ 值的个数的乘积，另一种情况相反。

```cpp
    int n,cnt,ans,sum1,sum2;
	struct{int ls,rs,val;}t[4800001];
	inline void update(int p){t[p].val=t[t[p].ls].val+t[t[p].rs].val;}
	inline void add(int &now,int l,int r,int x)
	{
		if(!now)now=++cnt;
		if(l==r)return ++t[now].val,void();
		int mid=l+((r-l)>>1);
		if(x<=mid)add(t[now].ls,l,mid,x);
		else add(t[now].rs,mid+1,r,x);
		update(now);
	}
	inline void merge(int &x,int &y,int l,int r)
	{
		if(!x||!y)return x|=y,void();
		int mid=l+((r-l)>>1);
		t[x].val+=t[y].val,sum1+=t[t[x].ls].val*t[t[y].rs].val,sum2+=t[t[x].rs].val*t[t[y].ls].val;
		merge(t[x].ls,t[y].ls,l,mid),merge(t[x].rs,t[y].rs,mid+1,r);
	}
	void print(int k,int l,int r)
	{
		if(!k)return;
		int mid=l+((r-l)>>1);
		cout<<l<<" "<<r<<" "<<t[k].val<<endl,print(t[k].ls,l,mid),print(t[k].rs,mid+1,r);
	}
	int dfs()
	{
		int now=0,k;read(k);
		if(k)return add(now,1,n,k),now;
		int lef=dfs(),rig=dfs();
		return sum1=sum2=0,merge(lef,rig,1,n),ans+=min(sum1,sum2),lef;
	}
	inline void mian(){read(n),dfs(),write(ans);}
```