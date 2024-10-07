---
layout: post
title: P9361 [ICPC2022 Xi'an R] Contests
subtitle: 题解
tags: 题解 倍增 Luogu
show: true
---

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17921522.html)

[P9361 [ICPC2022 Xi'an R] Contests](https://www.luogu.com.cn/problem/P9361)

首先观察一下性质，每个 $l$ 在每场比赛里一定是对应着某个前缀。

发现题目要求的是最小的满足要求的 $l$，最暴力的大概是维护五个指针，每次答案 $+1$，然后尝试跳一步，什么时候某个前缀包含了 $x$ 当前的计数器就是答案。

考虑如何优化。这个东西显然满足单调性，首先想到的是二分答案或者整体二分之类的，但是发现不好 `chk`。考虑倍增，维护每个人 $i$ 跳 $2^j$ 步在第 $k$ 场比赛的前缀 $f_{i,j,k}$。合并的式子：

$$
\begin{aligned}
f_{i,j,k}=\max_{x=1}^m\max_{y=1}^{f_{i,j-1,x}}f_{a_{x,j},i-1,k}
\end{aligned}
$$

发现 $y$ 的循环可以用前缀 $max$ 优化掉。对于询问，从大到小枚举 $2^i$，尝试跳这些步，如果符合了条件就不跳，否则跳并且答案累加 $2^i$。最后 $ans+1$ 就是答案。

注意读入格式和判询问一开始就满足的情况。

```cpp
	int n,m,q,ans,flag,c[6],b[6],a[6][100010],f[100010][17][6],g[17][6][100010][6];
	inline void mian()
	{
		read(n,m);int x,y;
		for(int i=1;i<=m;++i)for(int j=1;j<=n;++j)read(a[i][j]),f[a[i][j]][0][i]=j;
		for(int from=1;from<=m;++from)
		{
			for(int to=1;to<=m;++to)
			{
				for(int k=1;k<=n;++k)
				g[0][from][k][to]=max(g[0][from][k-1][to],f[a[from][k]][0][to]);
			}
		}
		for(int i=1;i<=16;++i)
		{
			for(int j=1;j<=n;++j)
			{
				for(int to=1;to<=m;++to)
				{
					for(int from=1;from<=m;++from)
					Mmax(f[j][i][to],g[i-1][from][f[j][i-1][from]][to]);
				}
			}
			for(int from=1;from<=m;++from)
			{
				for(int to=1;to<=m;++to)
				{
					for(int k=1;k<=n;++k)
					g[i][from][k][to]=max(g[i][from][k-1][to],f[a[from][k]][i][to]);
				}
			}
		}
//		for(int i=0;i<=2;++i,puts(""))for(int k=1;k<=m;++k,puts(""))for(int j=1;j<=n;++j)write(f[j][i][k]);
		read(q);
		while(q--)
		{
			read(y,x),ans=1;
			for(int i=1;i<=m;++i)b[i]=f[x][0][i];
			for(int i=16;i>=0;--i)
			{
				memset(c,0,sizeof(c)),flag=1;
				for(int j=1;j<=m;++j)for(int to=1;to<=m;++to)Mmax(c[to],g[i][j][b[j]][to]);
				for(int j=1;j<=m;++j)if(c[j]>f[y][0][j])flag=0;
				if(flag)ans+=(1<<i),memcpy(b,c,sizeof(b));
			}
			flag=0;
			for(int j=1;j<=m;++j)for(int to=1;to<=m;++to)Mmax(c[to],g[0][j][b[j]][to]);
			for(int j=1;j<=m;++j)if(c[j]>f[y][0][j])flag=1;
			if(!flag){puts("-1");continue;}
			for(int j=1;j<=m;++j)if(b[j]>f[y][0][j])flag=0;
			write(ans+flag,'\n');
		}
	}
```