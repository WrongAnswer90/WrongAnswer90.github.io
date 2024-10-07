---
layout: post
title: CF125E MST Company
subtitle: 题解
tags: 题解 WQS二分 CF
show: true
---

[CF125E MST Company](https://www.luogu.com.cn/problem/CF125E)

对于一类凸函数，有时我们寻找极值是简单的，但如果加上一维限制，问题就变成了函数在某个特定位置的值，这时问题不好处理

wqs 二分通过二分斜率后寻找极值，可以用复杂度加一只 $\log$ 的代价消去一维的限制。

具体来说，在本题中，设以 $1$ 为端点的边为特殊边，以特殊边选的个数横坐标，对于 MST 的权值作为纵坐标，可以得到一个下凸的函数（感性理解为什么是凸的即可，如果强制选太多权值会变大，选太少的话也不优秀），我们可以用 Kruskal 求出它的最小值。

然后进行对斜率的二分，即每次用一条斜率不同的直线去截这个函数，这样就能得到想要的函数值。

本题一个不太好处理的点是要求输出方案，这里其实有一个比较简单的处理方式。

我们在二分出斜率后，如果尽量不选特殊边一定会得到 $\leq need$ 的条数，如果尽量选特殊边一定会得到 $\geq need$ 的条数。

因此，我们先以尽量不选特殊边的策略跑一边 Kruskal，记下这时选了的特殊边，这些特殊边一定是必选的。

然后我们用必选的特殊边合并并查集后，再跑一遍 Kruskal，这时全选非特殊边是合法的，一定不会有特殊边比非特殊边，只有可能出现权值相等的情况。

所以我们考虑优先选一些权值相等的特殊边，具体的，我们此时的策略是：如果 $need - \text{已选的条数}>0$，那么优先选特殊边，否则优先选非特殊边。

注意优先选指的是权值相等的情况下优先选哪一类，如果选的特殊边数量还没有满足要求，但是此时有一条非特殊边比特殊边优秀，那么也要先选特殊边。

代码如下（真的十分丑陋）：

```cpp
int n,m,q,l,r,ans1,ans2,cnt1,cnt2,mid,n1,n2,vis[100001],fa[1001];
vector<int> ans;
struct Node{int x,y,z,id;}a[100001],b[100001];
bool cmp(Node nd1,Node nd2){return nd1.z<nd2.z;}
namespace DSU
{
	inline void init(){for(int i=1;i<=n;++i)fa[i]=i;}
	int find(int x){return x==fa[x]?x:fa[x]=find(fa[x]);}
}
using namespace DSU;
inline void checkl(int val)
{
	cnt1=0,ans1=0,init();
	for(int i=1,j=1;i<=n1||j<=n2;)
	{
		int x=0,y=0;
		while(i<=n1&&find(a[i].x)==find(a[i].y))++i;
		while(j<=n2&&find(b[j].x)==find(b[j].y))++j;
		if(i>n1&&j>n2)break;
		if(i>n1)x=INF;else x=a[i].z+val;
		if(j>n2)y=INF;else y=b[j].z;
		x<y?++cnt1,ans1+=x,fa[find(a[i].x)]=find(a[i].y),++i:(ans1+=y,fa[find(b[j].x)]=find(b[j].y),++j);
	}
}
inline void checkr(int val)
{
	cnt2=0,ans2=0,init();
	for(int i=1,j=1;i<=n1||j<=n2;)
	{
		int x=0,y=0;
		while(i<=n1&&find(a[i].x)==find(a[i].y))++i;
		while(j<=n2&&find(b[j].x)==find(b[j].y))++j;
		if(i>n1&&j>n2)break;
		if(i>n1)x=INF;else x=a[i].z+val;
		if(j>n2)y=INF;else y=b[j].z;
		x<=y?++cnt2,ans2+=x,fa[find(a[i].x)]=find(a[i].y),++i:(ans2+=y,fa[find(b[j].x)]=find(b[j].y),++j);
	}
}
inline void mian()
{
	read(n,m,q);int x,y,z;
	for(int i=1;i<=m;++i)read(x,y,z),x==1||y==1?a[++n1]={x,y,z,i}:b[++n2]={x,y,z,i};
	sort(a+1,a+1+n1,cmp),sort(b+1,b+1+n2,cmp),l=-inf,r=inf;
	while(l<r)
	{
		mid=l+((r-l)>>1),checkl(mid),checkr(mid);
		if(cnt1<=q&&cnt2>=q)l=r=mid;
		else if(cnt1>q)l=mid+1;
		else r=mid-1;
	}
	if(l==inf||l==-inf)return puts("-1"),void();
	checkl(l),checkr(l),init(),write(n-1,'\n');
	vector<pii> now;
	for(int i=1,j=1;i<=n1||j<=n2;)
	{
		int x=0,y=0;
		while(i<=n1&&find(a[i].x)==find(a[i].y))++i;
		while(j<=n2&&find(b[j].x)==find(b[j].y))++j;
		if(i>n1&&j>n2)break;
		if(i>n1)x=INF;else x=a[i].z+l;
		if(j>n2)y=INF;else y=b[j].z;
		x<y?(--q,fa[find(a[i].x)]=find(a[i].y),vis[a[i].id]=1,now.eb(mp(a[i].x,a[i].y)),ans.eb(a[i++].id)):(fa[find(b[j].x)]=find(b[j].y),j++);
	}
	init();
	for(int j=0;j<(int)now.size();++j)fa[find(now[j].fi)]=find(now[j].se);
	for(int i=1,j=1;i<=n1||j<=n2;)
	{
		int x=0,y=0;
		while(i<=n1&&(find(a[i].x)==find(a[i].y)||vis[a[i].id]))++i;
		while(j<=n2&&find(b[j].x)==find(b[j].y))++j;
		if(i>n1&&j>n2)break;
		if(i>n1)x=INF;else x=a[i].z+l;
		if(j>n2)y=INF;else y=b[j].z;
		x<=y&&q?--q,fa[find(a[i].x)]=find(a[i].y),vis[a[i].id]=1,ans.eb(a[i++].id):(fa[find(b[j].x)]=find(b[j].y),ans.eb(b[j++].id));
	}
	for(int i=0;i<(int)ans.size();++i)write(ans[i]);
}
```