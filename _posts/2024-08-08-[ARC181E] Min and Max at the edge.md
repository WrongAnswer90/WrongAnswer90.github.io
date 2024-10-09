---
layout: post
title: ARC181E Min and Max at the edge
subtitle: 题解
tags: 题解 线段树 最小生成树 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18348887)

### [[ARC181E] Min and Max at the edge](https://www.luogu.com.cn/problem/AT_arc181_e)

场上没人过的神题。（大概是搬运的官方题解）

先考虑如何 chk 一个图是否存在好生成树。观察好生成树的限制，发现其对于非树边的限制是在生成树上连接两点的路径有关。而 Kruskal 的证明就是对于每条非树边，其边权大于所有其路径上的树边，两者很像。

但是题目中的限制是点的限制，转到边上的想法是给点赋点权 $X_i$，然后 $(u,v)$ 的边权为 $\lvert X_u-X_v\rvert$。考虑把 $X$ 设成一个递增的序列，比如 $X_i=10^i$，这样边权一定互不相同。 

对于一棵好的生成树，设 $S(u,v)$ 表示 $u,v$ 之间的树上路径，则对于点 $i$ 要求 $\forall i\in S(u,v),X_u\leq X_i\leq X_v$。而对于边 $(x,y)$，$X_u\leq X_x,X_y\leq X_v$。设其根据上述定义的边权是 $w(x,y)=\lvert X_x-X_y\rvert$，则 $\forall (x,y)\in S(u,v),w(x,y)<w(u,v)$。这就证明了好的生成树一定是新图中的最小生成树。

因为可以构造出边权互不相同的情况，所以这也证明了好的生成树也是唯一的。跑出来这个图的最小生成树之后，检查每条非树边，如果都满足限制则该图是好的。

但是题目有多次询问，而且这个最小生成树也不是那么好求。不难发现其实图中定义的边权只需要满足 $\forall i<j<k,w(i,j)<w(i,k)$ 并且 $\forall i<j<k,w(i,k)>w(j,k)$ ，即满足区间包含单调性即可。

所以可以把边 $w(u,v)$ 赋为 $v\times(n+1)-u$，这样得到的生成树一定满足对于任意非树边 $(u,v)$，其路径上的最大值就是 $v$。原因是非树边 $(u,v)$ 路径上的边权应当都小于 $w(u,v)$，所以如果存在大于 $v$ 的点则一定不合法。而且因为是 $-u$，所以合并两个连通块的时候，一边选择的较小的点会尽可能的大，这也满足路径上的最小值尽量是 $u$。

但是这样还是需要 chk 路径上点的最小值是否满足条件。可以再反着做一遍，$w(u,v)=(n-u+1)\times(n+1)-(n-v+1)$，同理得这样求出来的东西的最小值一定合法，最大值尽量合法。因为已经证明了好的生成树唯一，所以判一下两棵树是否相同即可。

这样就将问题转化成了比较好的形式：删边求最小生成树，可以看成是找一条非树边 $(x,y)$ 满足 $x\in \text{subtree}(i),y\notin \text{subtree(i)},w(x,y)$ 最小。转成 dfn 序之后可以看成是两个 3-side 矩形查最小值，可以扫描线线段树维护，复杂度是 $\mathcal O(n\log n)$。

```cpp
int n,m,fa[200010];
int find(int x){return x==fa[x]?x:fa[x]=find(fa[x]);}
set<int> st;
inline void update(int id)
{
	auto it=st.find(id);
	if(it==st.end())st.insert(id);
	else st.erase(it);
}
int flag=1;
struct Node
{
	int x,y,z,t;
	Node(int X=0,int Y=0,int Z=0,int T=0){x=X,y=Y,z=Z,t=T;}
};
inline bool cmp1(Node x,Node y){return x.z<y.z;}
struct MST
{
	int cnt,head[200010],to[400010],nex[400010];
	int tot,siz[200010],dfn[200010];
	pii ans[200010];
	bool ins[200010];
	Node a[200010];
	vector<tup> L[200010],R[200010];
	vector<pair<int,pii>> ve[200010];
	inline pii get(pii x,pii y){return x.fi<y.fi?x:y;}
	struct{int l,r;pii v;}t[400010];
	#define ls(x) (t[x].l+t[x].r)
	#define rs(x) (ls(x)^1)
	void build(int p,int l,int r)
	{
		t[p]={l,r,mp(INF,0)};
		if(l==r)return; 
		int mid=l+((r-l)>>1);
		build(ls(p),l,mid),build(rs(p),mid+1,r);
	}
	void change(int p,int x,pii y)
	{
		t[p].v=get(t[p].v,y);
		if(t[p].l==t[p].r)return;
		change(ls(p)^(x>t[ls(p)].r),x,y);
	}
	pii ask(int p,int l,int r)
	{
		if(l<=t[p].l&&r>=t[p].r)return t[p].v;
		pii v=mp(INF,0);
		if(l<=t[ls(p)].r)v=get(v,ask(ls(p),l,r));
		if(r>t[ls(p)].r)v=get(v,ask(rs(p),l,r));
		return v;
	}
	inline void add(int x,int y){to[++cnt]=y,nex[cnt]=head[x],head[x]=cnt;}
	void dfs(int x,int Fa=0)
	{
		dfn[x]=++tot,siz[x]=1;
		for(int i=head[x];i;i=nex[i])if(to[i]!=Fa)
		dfs(to[i],x),siz[x]+=siz[to[i]];
	}
	inline void solve()
	{
		int x,y;
		for(int i=1;i<=n;++i)fa[i]=i;
		sort(a+1,a+1+m,cmp1);
		for(int i=1;i<=m;++i)
		{
			x=find(a[i].x),y=find(a[i].y),ans[i]=mp(INF,0ll);
			if(x==y)continue;
			ins[a[i].t]=1,update(a[i].t),fa[x]=y;
			add(a[i].x,a[i].y),add(a[i].y,a[i].x);
		}
		for(int i=2;i<=n;++i)flag&=find(i-1)==find(i);
		dfs(1),build(1,1,n);
		for(int i=1;i<=m;++i)
		{
			if(!ins[a[i].t])continue;
			x=dfn[a[i].x]<dfn[a[i].y]?a[i].y:a[i].x;
			L[dfn[x]-1].eb(tup(dfn[x],dfn[x]+siz[x]-1,a[i].t));
			R[dfn[x]+siz[x]].eb(tup(dfn[x],dfn[x]+siz[x]-1,a[i].t));
		}
		for(int i=1;i<=m;++i)if(!ins[a[i].t])
		ve[min(dfn[a[i].x],dfn[a[i].y])].eb(mp(max(dfn[a[i].x],dfn[a[i].y]),mp(a[i].z,a[i].t)));
		for(int i=1;i<=n;++i)
		{
			for(auto [x,y]:ve[i])change(1,x,y);
			for(auto [x,y,z]:L[i])ans[z]=get(ans[z],ask(1,x,y));
			ve[i].clear();
		}
		build(1,1,n);
		for(int i=1;i<=m;++i)if(!ins[a[i].t])
		ve[max(dfn[a[i].x],dfn[a[i].y])].eb(mp(min(dfn[a[i].x],dfn[a[i].y]),mp(a[i].z,a[i].t)));
		for(int i=n;i>=1;--i)
		{
			for(auto [x,y]:ve[i])change(1,x,y);
			for(auto [x,y,z]:R[i])ans[z]=get(ans[z],ask(1,x,y));
			ve[i].clear();
		}
	}
}t[2];
inline void mian()
{
	read(n,m);int x,y;
	for(int i=1;i<=m;++i)
	{
		read(x,y);
		if(x>y)swap(x,y);
		t[0].a[i]=Node(x,y,y*inf-x,i);
		t[1].a[i]=Node(x,y,(n-x+1)*inf-(n-y+1),i);
	}
	t[0].solve(),t[1].solve();
	if(!flag){for(int i=1;i<=m;++i)puts("No");return;}
	for(int i=1;i<=m;++i)
	{
		int fl=1;
		if(t[0].ins[i])fl&=t[0].ans[i].se!=0,update(i),update(t[0].ans[i].se);
		if(t[1].ins[i])fl&=t[1].ans[i].se!=0,update(i),update(t[1].ans[i].se);
		if(st.empty()&&fl)puts("Yes");else puts("No");
		if(t[0].ins[i])update(i),update(t[0].ans[i].se);
		if(t[1].ins[i])update(i),update(t[1].ans[i].se);
	}
}
```