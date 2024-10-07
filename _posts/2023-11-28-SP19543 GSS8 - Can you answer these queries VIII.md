---
layout: post
title: SP19543 GSS8 - Can you answer these queries VIII
subtitle: 题解
tags: 题解 大数据结构 平衡树 Luogu
show: true
---

## [更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17862183.html)

[SP19543 GSS8 - Can you answer these queries VIII](https://www.luogu.com.cn/problem/SP19543)

fhq + 二项式定理。提供一个不太一样的思路。默认下标从 $1$ 开始。

首先插入删除，区间查询，想到可以平衡树维护或者离线下来做线段树。本文中是用的是 fhq，好写一些。

$k$ 非常的小，考虑对于每一个 $k$ 的答案如何维护。观察答案的式子：

$$

ans_k=\sum_{j=l}^ra_j(j+1-l)^k

$$

因为 $k$ 很小，可以直接把 $k$ 次方拆开，二项式定理转化一下：

$$

\begin{aligned}

ans_k&=\sum_{j=l}^ra_j(j+1-l)^k\\

&=\sum_{j=l}^ra_j\sum_{i=0}^k\binom{k}{i}(j+1)^i(-l)^{k-i}\\

&=\sum_{i=0}^k\binom{k}{i}(-l)^{k-i}\sum_{j=l}^ra_j(j+1)^i\\

\end{aligned}

$$

发现我们本质需要维护的是 $\sum a_j(j+1)^k$ 这个东西。上传的话就直接 $\mathcal O(k)$ 暴力加。但是题目中有插入和删除操作，下标会发生平移，$j$ 是会变的，考虑加了 $tmp$ 以后如何维护。设 $sum_i$ 表示 $i$ 子树内 $\sum a_j(j+1)^k$。

$$

\begin{aligned}

sum_k&=\sum_{j=l}^r a_j(j+1)^k\\

nsum_k&=\sum_{j=l}^r a_j(j+1+tmp)^k\\

&=\sum_{j=l}^ra_j\sum_{i=0}^k\binom{k}{i}(j+1)^itmp^{k-i}\\

&=\sum_{i=0}^k\binom{k}{i}tmp^{k-i}\sum_{j=l}^ra_j(j+1)^i\\

&=\sum_{i=0}^k\binom{k}{i}tmp^{k-i}sum_i

\end{aligned}

$$

直接 $\mathcal O(k^2)$ 更新即可，需要预处理组合数和阶乘。复杂度 $\mathcal O(mk^2\log n)$。其实可以用 MTT 等科技优化到 $\mathcal O(mk\log k\log n)$，但是因为 $k$ 非常小并且 MTT 常数巨大，感觉跑的不如暴力快。理论可以用常数更小的线段树代替 fhq，但是懒得写了。。

瓶颈在于插入和删除造成区间下标的平移。

```cpp
	int n,m;
	ui po[200010][11],po2[200010][11],C[11][11];
	namespace FHQ
	{
		int cnt,root;
		struct{int ls,rs,siz,tg;ui tag,val[11],sum[11];}t[200010];
		inline void update(int p){t[p].siz=t[t[p].ls].siz+t[t[p].rs].siz+1;for(int i=0;i<=10;++i)t[p].sum[i]=t[p].val[i]+t[t[p].ls].sum[i]+t[t[p].rs].sum[i];}
		inline void down(int p,int x)
		{
			t[p].tag+=x;
			if(x>0)
			{
				for(int i=10;i>=0;--i)for(int j=0;j<i;++j)t[p].sum[i]+=C[i][j]*po[x][i-j]*t[p].sum[j];
				for(int i=10;i>=0;--i)for(int j=0;j<i;++j)t[p].val[i]+=C[i][j]*po[x][i-j]*t[p].val[j];
			}
			else
			{
				for(int i=10;i>=0;--i)for(int j=0;j<i;++j)t[p].sum[i]+=C[i][j]*po2[-x][i-j]*t[p].sum[j];
				for(int i=10;i>=0;--i)for(int j=0;j<i;++j)t[p].val[i]+=C[i][j]*po2[-x][i-j]*t[p].val[j];
			}
		}
		inline void spread(int p){if(t[p].tag)down(t[p].ls,t[p].tag),down(t[p].rs,t[p].tag),t[p].tag=0;}
		void print(int p)
		{
			if(!p)return;spread(p),print(t[p].ls);
			write(p),write(t[p].ls),write(t[p].rs,'\n');
			for(int i=0;i<=10;++i)write((ll)t[p].val[i]);puts("");
			for(int i=0;i<=10;++i)write((ll)t[p].sum[i]);puts("");
			puts("");
			print(t[p].rs);
		}
		inline int newnode(int val,int id)
		{
			t[++cnt].siz=1,t[cnt].tg=rand();
			for(int i=0;i<=10;++i)t[cnt].sum[i]=t[cnt].val[i]=val*po[id+1][i];
			return cnt;
		}
		void split(int now,int k,int &x,int &y)
		{
			if(!now)return x=y=0,void();
			spread(now);
			if(k<=t[t[now].ls].siz)y=now,split(t[y].ls,k,x,t[y].ls),update(y);
			else x=now,split(t[x].rs,k-t[t[now].ls].siz-1,t[x].rs,y),update(x);
		}
		int merge(int x,int y)
		{
			if(!x||!y)return x|y;
			if(t[x].tg>t[y].tg)return spread(x),t[x].rs=merge(t[x].rs,y),update(x),x;
			return spread(y),t[y].ls=merge(x,t[y].ls),update(y),y;
		}
		int X,Y,Z;
		inline void ins(int val,int id){split(root,id,X,Y),down(Y,1),root=merge(merge(X,newnode(val,id+1)),Y);}
		inline void del(int id){split(root,id,Y,Z),split(Y,id-1,X,Y),down(Z,-1),root=merge(X,Z);}
		inline ui calc(int l,int r,int k)
		{
			split(root,r,Y,Z),split(Y,l-1,X,Y);
			int ans=0;
			for(int i=0;i<=k;++i)
			ans+=(ui)((k-i)&1?-1:1)*C[k][i]*po[l][k-i]*t[Y].sum[i];
			return root=merge(merge(X,Y),Z),ans;
		}
		inline void change(int x,int y){split(root,x,Y,Z),split(Y,x-1,X,Y);root=merge(merge(X,newnode(y,x)),Z);}
	}
	using namespace FHQ;
	inline void mian()
	{
		C[0][0]=1;
		for(int i=1;i<=10;++i)for(int j=0;j<=i;++j)C[i][j]=C[i-1][j]+(j?C[i-1][j-1]:0);
		for(int i=0;i<=200009;++i)po[i][0]=po2[i][0]=1;
		for(int i=0;i<=200009;++i)for(int j=1;j<=10;++j)po[i][j]=po[i][j-1]*i,po2[i][j]=po2[i][j-1]*(-i);
		read(n);int x,y;ll z;char ch;
		for(int i=1;i<=n;++i)read(x),root=merge(root,newnode(x,i));
		read(m);
		while(m--)
		{
			while((ch=getchar())==' ')ch=getchar();
			if(ch=='Q')read(x,y,z),write((ll)calc(x+1,y+1,z),'\n');
			else if(ch=='D')read(x),del(x+1);
			else if(ch=='I')read(x,y),ins(y,x);
			else read(x,y),change(x+1,y);
		}
	}
```