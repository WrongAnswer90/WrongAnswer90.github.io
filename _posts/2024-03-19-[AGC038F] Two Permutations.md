[AT_agc038_f [AGC038F] Two Permutations](https://www.luogu.com.cn/problem/AT_agc038_f)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/18083524)

下面默认 $P,Q$ 均整体加了 $1$，下标从 $1$ 开始。

首先考虑如果 $a_i=i$，则 $p_j=i$ 的 $a_j=j$。然后多迭代几次，一定会回到 $i$。这提示我们把排列拆成若干个环，每个环 $R$ 满足 $p_{r_{len}}=r_1\wedge\forall i<len,p_{r_i}=r_{i+1}$。这样每个环要么满足全部 $a_i=i$（方案 $1$），要么满足全部 $a_i=p_i$（方案 $2$）。

这样对于每个 $i$ 可以求出 $ca_i$ 和 $cb_i$ 表示在 $a$ 和 $b$ 的那个环里。接下来是分讨：

1. $a_i=b_i=i$。此时该位置的贡献一定为 $0$。

2. $a_i=i\wedge b_i\ne i$。此时若 $cb_i$ 选择方案 $1$ 则收益为 $0$，否则收益为 $1$。

3. $a_i\ne i\wedge b_i=i$。此时若 $ca_i$ 选择方案 $1$ 则收益为 $0$，否则收益为 $1$。

4. $a_i\ne i\wedge b_i\ne i$。此时若 $ca_i$ 和 $cb_i$ 均选择方案 $1$ 则收益为 $0$，若 $ca_i,cb_i$ 均选择方案 $2$ 且 $a_i=b_i$ 则收益为 $0$，否则收益为 $1$。

$1,2,3$ 都是平凡的，$4$ 是一个反着的文理分科模型，无法用流图建模。

传统的文理分科是归到 $S$ 集合是割掉和 $T$ 的边，归到 $T$ 集合是割掉 $S$ 的边。这里归到 $S$ 是选择方案 $1$，归到 $T$ 是选择方案 $2$。因为只有 $ca,cb$ 之间的关系，$ca,cb$ 内部的情况是不会造成贡献的。考虑经典套路，对于 $b$ 的意义进行翻转。此时变成了：

4. 若 $ca_i$ 归到 $S$，$cb_i$ 归到 $T$，则代价为 $1$，若 $ca_i$ 归到 $T$，$cb_i$ 归到 $S$ 且 $a_i=b_i$，则代价为 $1$。

这样就是[这道题](https://www.luogu.com.cn/problem/P2057)了，两点间连容量为 $1$ 的边即可。因为建出来的图是二分图，暴力跑 dinic 就是对的。总复杂度 $\mathcal O(n\sqrt n)$。思路还是很清晰的，第一步需要一点观察。

```cpp
	int n,num,ans,s[200010],t[200010],ca[100010],cb[100010],a[100010],b[100010];
	bool vis[100010];
	int S,T,cnt=1,head[200050],to[1000010],nex[1000010],v[1000010],d[200050],now[200050];
	inline void Add(int x,int y,int z){to[++cnt]=y,v[cnt]=z,nex[cnt]=head[x],head[x]=cnt;}
	inline void add(int x,int y,int z){Add(x,y,z),Add(y,x,0);}
	inline void clr(){memset(head,0,sizeof(head)),cnt=1;}
	inline bool bfs()
	{
		queue<int> q;memset(d,0,sizeof(d)),d[S]=1,now[S]=head[S],q.e(S);
		while(!q.empty())
		{
			int nw=q.front();q.pop();
			for(int i=head[nw];i;i=nex[i])if(v[i]&&!d[to[i]])
			{
				d[to[i]]=d[nw]+1,q.e(to[i]),now[to[i]]=head[to[i]];
				if(to[i]==T)return 1;
			}
		}
		return 0;
	}
	int dinic(int x,int flow)
	{
		if(x==T)return flow;
		int rest=flow,t;
		for(int i=now[x];i&&rest;i=nex[i])
		{
			now[x]=i;
			if(d[to[i]]!=d[x]+1||!v[i])continue;
			t=dinic(to[i],min(rest,v[i])),rest-=t;
			if(!t)d[to[i]]=0;
			v[i]-=t,v[i^1]+=t;
		}
		return flow-rest;
	}
	inline void mian()
	{
		read(n),ans=n;int x;
		for(int i=1;i<=n;++i)read(a[i]),++a[i];
		for(int i=1;i<=n;++i)read(b[i]),++b[i];
		for(int i=1;i<=n;++i)if(!ca[i]){++num,x=a[i];do ca[x]=num,x=a[x];while(!ca[x]);}
		int la=num;
		for(int i=1;i<=n;++i)if(!cb[i]){++num,x=b[i];do cb[x]=num,x=b[x];while(!cb[x]);}
		S=num+1,T=S+1;
		for(int i=1;i<=n;++i)
		{
			if(a[i]==i)
			{
				if(b[i]==i)--ans;
				else ++t[cb[i]];
			}
			else
			{
				if(b[i]==i)++s[ca[i]];
				else
				{
					add(cb[i],ca[i],1);
					if(a[i]==b[i])add(ca[i],cb[i],1);
				}
			}
		}
		for(int i=1;i<=la;++i)add(S,i,s[i]);
		for(int i=la+1;i<=num;++i)add(i,T,t[i]);
		while(bfs())while((x=dinic(S,inf)))ans-=x;
		write(ans);
	}
```