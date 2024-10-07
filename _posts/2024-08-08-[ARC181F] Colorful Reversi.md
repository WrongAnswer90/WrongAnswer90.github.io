# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18349625)

### [[ARC181F] Colorful Reversi](https://www.luogu.com.cn/problem/AT_arc181_f)

首先观察一下，对于 $a,b,c,a$ 这种情况来说，两个 $a$ 之间永远不可能发生操作。而 $a,b,c,b,a$ 这种情况，两个 $a$ 之间是有关联的。有一个很天才的想法是建树，一开始只有一个节点表示 $a_1$，维护一个指针 $pos$ 表示当前在树上的哪个节点，接下来依次加入每个点 $a_i$：

1. 若 $pos$ 所在点的颜色和 $a_i$ 相同，则 $pos$ 不动。
2. 否则若 $pos$ 所在点有邻点的颜色是 $a_i$，则 $pos$ 走向该邻点。
3. 否则新建一个节点，颜色为 $a_i$，$pos$ 走向该节点。

这样就得到了一棵操作树，它有一些很好的性质：

1. 假设生成时在上面走过的路径是 $B$，则操作 $l,r$ 等价于把 $B[l,r]=\{x,y,y\dots y,y,x\}$ 变成 $B[l,r]=\{x,x\dots x,x\}$。
2. 对于原序列，如果能够操作 $[l,r]$ 则 $l,r$ 一定属于同一个点，否则 $l,r$ 属于不同的点。
3. 对于路径 $\{v_1,v_2,v_3\dots v_k\}$，通过操作将其变成 $\{v'_1,v'_2,v'_3\dots v'_k\}$ 的最小代价是 $\sum_{i=1}^k d(v_i,v'_i)$，其中 $d(x,y)$ 表示两点树上的距离。

接下来，对于 $pos$ 的起始节点 $x$ 和终止节点 $y$，最终序列的形态是若干个颜色段，而颜色恰好就是从 $x$ 走到 $y$ 所经过的简单路径上的颜色。证明很简单，显然路径上两个不同的颜色段永远无法合并，而如果存在其他的颜色则其两边的颜色一定是相同的，能再操作一次。

所以拉出树上 $x,y$ 之间的路径，对于此路径外的部分一定会合并到路径上，可以先 dfs 一遍算出贡献，这样可以得到从 $B$ 得到一个新的序列 $C$。接下来要解决问题的就是：现在有一个序列 $C[1,n]$，满足 $\forall i<n,|C_i-C_{i+1}|\leq 1$。现在要生成一个新的序列 $C'$，满足：

1. $C'_1=C_1,C'_n=C_n$。
2. $\forall i<n,C'_{i+1}-C'_i\in[0,1]$。
3. $\forall i<n\wedge C'_{i+1}=C’_i+1,C_i=C'_i,C_{i+1}=C'_{i+1}$。

在此基础上满足 $\sum_{i=1}^n |C_i-C'_i|$ 最小。设 $f_{i}$ 表示考虑了 $\leq i$ 的 $C$，$C'_i=C_i$ 时代价的最小值，$pre_i$ 表示 $C_i$ 上一次出现的位置。转移比较简单：

$$
f_i=\min
\begin{cases}
f_{i-1}\quad\quad\quad\quad\quad\quad\quad\quad\quad\quad C_i=C_{i-1}+1\\
f_{pre_i}+\sum_{j=pre_i}^{i}|C_i-C_j|\;\;\;\;\;pre_i\not=-1
\end{cases}
$$

因为 $pre_i$ 是上一次出现的位置，所以对于所有的 $j$ 要么 $C_j$ 全部大于 $C_i$，要么全部小于 $C_i$，可以前缀和处理一下 $\mathcal O(1)$ 转移。这样 $f_n$ 加上之前把树缩成一条链的代价就是答案，复杂度是 $\mathcal O(n)$ 或 $\mathcal O(n\log n)$。

```cpp
int n,ans,cnt,len,s[500010],lst[500010],vis[500010],real[500010];
int b[500010],c[500010],a[500010],f[500010],t[500010],fa[500010];
map<int,int> T[500010];
void dfs(int x,int Fa)
{
	fa[x]=Fa;
	for(auto [a,y]:T[x])if(y!=Fa)dfs(y,x);
}
void dfs1(int x)
{
	++vis[x];
	for(auto [b,y]:T[x])if(y!=fa[x]&&!vis[y])
	dfs1(y),ans+=a[y],a[x]+=a[y];
}
inline void mian()
{
	read(n);int x,now=1,col;
	read(c[1]),a[cnt=1]=1,col=c[1];
	for(int i=2;i<=n;++i)
	{
		read(c[i]),x=c[i];
		if(x!=col)
		{
			if(!T[now][x])T[now][x]=++cnt,T[cnt][col]=now;
			now=T[now][x],col=x;
		}
		++a[now];
	}
	dfs(1,0);
	while(now)b[++len]=now,vis[now]=1,now=fa[now];
	for(int i=1;i<=len;++i)real[b[i]]=len-i+1,dfs1(b[i]);
	vi ve={0};ve.eb(1),now=1;
	for(int i=2,pos=1;i<=n;++i)
	{
		if(c[i]!=c[i-1])
		{
			assert(T[now][c[i]]);
			now=T[now][c[i]];
		}
		if(vis[now]==2)pos=now;
		ve.eb(real[pos]);
	}
	memset(f,127,sizeof(f)),f[1]=0,s[1]=ve[1],lst[ve[1]]=1;
	for(int i=2;i<(int)ve.size();++i)
	{
		s[i]=s[i-1]+ve[i];
		if(ve[i]==ve[i-1]+1)f[i]=f[i-1];
		int tmp=s[i-1]-s[lst[ve[i]]],v;
		if(ve[i-1]>=ve[i])v=tmp-ve[i]*(i-1-lst[ve[i]]);
		else v=ve[i]*(i-1-lst[ve[i]])-tmp;
		Mmin(f[i],f[lst[ve[i]]]+v);
		lst[ve[i]]=i;
	}
	cout<<f[ve.size()-1]+ans;
}
```