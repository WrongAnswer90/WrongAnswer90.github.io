---
layout: post
title: CF1423M Milutin's Plums
subtitle: 题解
tags: CF SMAWK 分治
show: true
---

# [My Blogs](https://wronganswer90.github.io/2024/11/12/CF1423M-Milutin's-Plums)

### [CF1423M Milutin's Plums](https://www.luogu.com.cn/problem/CF1423M)

完全单调性的定义：一个 $n\times m$ 的矩阵，如果对于所有 $S\subseteq \left \{ 1,2,3\dots n\right\}$，$T\subseteq \left \{1,2,3\dots m\right \}$，只保留 $S$ 中的行和 $T$ 中的列时，每行第一个最小值的位置都是非降的，则称其有完全单调性。

SMAWK 算法：一种能在线性的单点询问次数内，求出一个满足完全单调性矩阵每行最小值及其位置的算法。

#### 外层分治

首先可以通过分治得到一个简单的 $\mathcal O((n+m)\log n)$ 的做法：把所有奇数行删掉之后求出所有偶数行的决策点，奇数行的决策点一定在相邻两个之间。每层询问 $\mathcal O(n+m)$ 次。

上面复杂度带 $\log$ 的原因在于，矩阵可能变得 $n$ 非常小，但是 $m$ 仍然非常大。

#### reduce 操作

注意到每一行只需要保留最小值第一次出现的位置，所以有效的列数是 $\min(n,m)$ 而不是 $m$。SMAWK 的 reduce 操作可以用 $\mathcal O(m)$ 次操作，通过删去全都不可能是最小值的列，将 $m$ 变成 $\min(n,m)$。

考虑询问 $(x,y)$ 和 $(x,y+1)$ 之间的大小关系可以知道什么。如果 $f(x,y)>f(x,y+1)$，取一个只保留 $y,y+1$ 列的子矩形，则 $(x,y)$ 不是最小值点。由完全单调性得，$\forall i\geq x,f(i,y)$ 不是第 $y$ 行对应的最小值点。

而如果 $f(x,y)\leq f(x,y+1)$，同理说明 $\forall i\leq x,f(i,y)$ 也不是最小值点。

考虑维护一个指针 $k$，初始为 $1$，不断询问 $f(k,k)$ 和 $f(k,k+1)$ 的关系。

如果 $f(k,k)\leq f(k,k+1)$，如果 $k=n$ 则删掉第 $k+1$ 行。否则令 $k\leftarrow (k+1)$。容易发现这样的操作保证了，每时每刻，$\forall i\leq j\leq k,f(i,j)$ 都不是最小值点，也就是 $k$ 上方的一个三角形都是没用的。

如果 $f(k,k)>f(k,k+1)$，因为 $k$ 所在列的 $[1,k]$ 的位置都是没用的，这次询问又得到了 $k$ 下方的部分没用，所以删掉第 $k$ 列，令 $k\leftarrow \max(1,k-1)$。

这样分治的时候，每层先调用 reduce 操作，再按照一开始的分治运行即可。实现的时候可以封装一个交互的询问函数，记忆化已经问过的位置来减小常数。

```cpp
int n,m;
int a[1010][1010];
unordered_map<ull,int> hash;
#define id(x,y) 1000000ull*x+y
inline int get(int x,int y,int tp=0)
{
	if(hash.find(id(x,y))!=hash.end())
	return hash[id(x,y)];
	cout<<"? "<<x<<" "<<y<<endl;
	fflush(stdout);
	cin>>hash[id(x,y)];
//		hash[id(x,y)]=a[x][y];
	return hash[id(x,y)];
}

int nex[1000010],pre[1000010];
inline void init(int n)
{
	for(int i=0;i<n;++i)nex[i]=i+1;
	for(int i=1;i<=n;++i)pre[i]=i-1;
	pre[0]=0,nex[n]=n;
}
inline void del(int x)
{
	nex[pre[x]]=nex[x];
	pre[nex[x]]=pre[x];
}
vi solve(vi x,vi y)
{
	{
		for(int i=0;i+1<(int)y.size();++i)
		nex[y[i]]=y[i+1],pre[y[i+1]]=y[i];
		pre[y[0]]=0,nex[0]=y[0];
		nex[y.back()]=-1;
		int i=0,Y=y[0],tat=(int)y.size()-(int)x.size();
		while(tat>0)
		{
			if(get(x[i],Y,1)>get(x[i],nex[Y],1))
			{
				del(Y),--tat;
				if(i)Y=pre[Y],--i;
				else Y=nex[Y];
			}
			else
			{
				if(i+1==(int)x.size())
				del(nex[Y]),--tat;
				else Y=nex[Y],++i;
			}
		}
		y.clear();
		for(int i=0,j=nex[0];i<(int)x.size();++i)
		{
			if(j==-1)break;
			y.eb(j),j=nex[j];
		}
	}
	if(x.size()==1)return y;
	vi xx;
	for(int j=1;j<(int)x.size();j+=2)
	xx.eb(x[j]);
	vi pos=solve(xx,y);
	vi realpos,Pos(x.size());
	
	realpos.eb(0);
	for(int j=0,k=0;k<(int)xx.size();++j)
	{
		while(k<(int)xx.size()&&y[j]==pos[k])
		realpos.eb(j),++k;
	}
	realpos.eb((int)y.size()-1);
	int mx=inf,p=0;
	
	for(int j=0;j<(int)xx.size();++j)
	Pos[(j<<1)|1]=pos[j];
	for(int j=0;j<(int)x.size();j+=2)
	{
		mx=inf,p=-1;
		for(int k=realpos[j>>1];k<=realpos[(j>>1)+1];++k)
		if(Mmin(mx,get(x[j],y[k])))p=y[k];
		Pos[j]=p;
	}
	
	return Pos;
}
void mian()
{
	read(n,m);
//		for(int i=1;i<=n;++i)for(int j=1;j<=m;++j)
//		read(a[i][j]);
	vi X,Y;
	for(int i=1;i<=n;++i)X.eb(i);
	for(int i=1;i<=m;++i)Y.eb(i);
	int ans=inf;
	vi pos=solve(X,Y);
	for(int i=0;i<n;++i)Mmin(ans,get(i+1,pos[i]));
	cout<<"! "<<ans<<endl;
}
```
