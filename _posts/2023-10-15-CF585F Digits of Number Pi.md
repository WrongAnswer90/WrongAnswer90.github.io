---
layout: post
title: CF585F Digits of Number Pi
subtitle: 题解
tags: 题解 数位DP 字符串 ACAM CF
show: true
---

[CF585F Digits of Number Pi](https://www.luogu.com.cn/problem/CF585F)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17765571.html)

观察数据范围，考虑数位 DP。

首先把长串中 $len\geq\lfloor \frac{d}{2}\rfloor$ 的串提出来，塞进一个 trie 里，然后建立 ACAM，然后直接 DP 就行了。

设 $f_{i,j,0/1,0/1,0/1}$ 表示当前在 trie 图上走了 j 步到达了第 i 个节点，是否已经出现子串，是否满足低位限制，是否满足高位限制，枚举 $k$ 表示第 $j$ 个字符是什么，然后在 AC 自动机上跑。第三个 $0/1$ 在走到一个结束点后就一定是 $1$，第四个维度选了一个大于 $x$ 串当前位的字符后变成 $1$，第五个维度也是同理，细节比较多，但是实在是没有任何思维含量。详细的转移方程见代码。

```cpp
inline void Madd(int&x,int y){x+y>=MOD?x=x+y-MOD:x=x+y;}
int n,d,ans,f[2][500001][2][2][2];
char s[1001],t1[1001],t2[1001];
namespace ACAM
{
	int end[500001],trie[500001][10],fail[500001],cnt=1;
	queue<int> q;
	inline void build()
	{
		for(int i=0;i<10;++i)trie[0][i]=1;
		for(int i=1;i+d/2-1<=n;++i)
		{
			int now=1;
			for(int j=i;j<=min(n,i+d/2-1);++j)
			{
				if(!trie[now][s[j]-'0'])trie[now][s[j]-'0']=++cnt;
				now=trie[now][s[j]-'0'];
				if(j>=i+d/2-1)end[now]=1;
			}
		}
		q.e(1);
		while(!q.empty())
		{
			int now=q.front();q.pop();
			for(int i=0;i<10;++i)
			{
				if(trie[now][i])fail[trie[now][i]]=trie[fail[now]][i],q.e(trie[now][i]);
				else trie[now][i]=trie[fail[now]][i];
			}
		}
	}
}
using namespace ACAM;
#define g f[pre][trie[i][k]]
#define nw f[now][i]
inline void mian()
{
	scanf("%s%s%s",s+1,t1+1,t2+1),ans=0,n=strlen(s+1),d=strlen(t1+1);
	for(int i=1;i<=d;++i)t1[i]-='0',t2[i]-='0';
	build(),f[1][1][0][0][0]=1;int now=0,pre=1;
	for(int j=0;j<d;++j)
	{
		now^=1,pre^=1,memset(f[pre],0,sizeof(f[pre]));
		for(int i=1;i<=cnt;++i)
		{
			for(int k=0;k<10;++k)
			{
				int is=end[trie[i][k]];
				if(k<t1[j+1])
				{
					if(k<t2[j+1])
					{
						Madd(g[is|0][1][1],nw[0][1][0]),Madd(g[is|0][1][1],nw[0][1][1]);
						Madd(g[is|1][1][1],nw[1][1][0]),Madd(g[is|1][1][1],nw[1][1][1]);
					}
					else if(k==t2[j+1])
					{
						Madd(g[is|0][1][0],nw[0][1][0]),Madd(g[is|0][1][1],nw[0][1][1]);
						Madd(g[is|1][1][0],nw[1][1][0]),Madd(g[is|1][1][1],nw[1][1][1]);
					}
					else
					{
						Madd(g[is|0][1][1],nw[0][1][1]),Madd(g[is|1][1][1],nw[1][1][1]);
					}
				}
				else if(k==t1[j+1])
				{
					if(k<t2[j+1])
					{
						Madd(g[is|0][0][1],nw[0][0][0]),Madd(g[is|0][0][1],nw[0][0][1]);
						Madd(g[is|0][1][1],nw[0][1][0]),Madd(g[is|0][1][1],nw[0][1][1]);
						Madd(g[is|1][0][1],nw[1][0][0]),Madd(g[is|1][0][1],nw[1][0][1]);
						Madd(g[is|1][1][1],nw[1][1][0]),Madd(g[is|1][1][1],nw[1][1][1]);
					}
					else if(k==t2[j+1])
					{
						Madd(g[is|0][0][0],nw[0][0][0]),Madd(g[is|0][0][1],nw[0][0][1]);
						Madd(g[is|0][1][0],nw[0][1][0]),Madd(g[is|0][1][1],nw[0][1][1]);
						Madd(g[is|1][0][0],nw[1][0][0]),Madd(g[is|1][0][1],nw[1][0][1]);
						Madd(g[is|1][1][0],nw[1][1][0]),Madd(g[is|1][1][1],nw[1][1][1]);
					}
					else
					{
						Madd(g[is|0][0][1],nw[0][0][1]),Madd(g[is|0][1][1],nw[0][1][1]);
						Madd(g[is|1][0][1],nw[1][0][1]),Madd(g[is|1][1][1],nw[1][1][1]);
					}
				}
				else
				{
					if(k<t2[j+1])
					{
						Madd(g[is|0][1][1],nw[0][0][0]),Madd(g[is|0][1][1],nw[0][0][1]);
						Madd(g[is|0][1][1],nw[0][1][0]),Madd(g[is|0][1][1],nw[0][1][1]);
						Madd(g[is|1][1][1],nw[1][0][0]),Madd(g[is|1][1][1],nw[1][0][1]);
						Madd(g[is|1][1][1],nw[1][1][0]),Madd(g[is|1][1][1],nw[1][1][1]);
					}
					else if(k==t2[j+1])
					{
						Madd(g[is|0][1][0],nw[0][0][0]),Madd(g[is|0][1][0],nw[0][1][0]);
						Madd(g[is|0][1][1],nw[0][0][1]),Madd(g[is|0][1][1],nw[0][1][1]);
						Madd(g[is|1][1][0],nw[1][0][0]),Madd(g[is|1][1][0],nw[1][1][0]);
						Madd(g[is|1][1][1],nw[1][0][1]),Madd(g[is|1][1][1],nw[1][1][1]);
					}
					else
					{
						Madd(g[is|0][1][1],nw[0][0][1]),Madd(g[is|0][1][1],nw[0][1][1]);
						Madd(g[is|1][1][1],nw[1][0][1]),Madd(g[is|1][1][1],nw[1][1][1]);
					}
				}
			}
		}
	}
	for(int i=1;i<=cnt;++i)
	{
		Madd(ans,f[pre][i][1][0][0]),Madd(ans,f[pre][i][1][0][1]);
		Madd(ans,f[pre][i][1][1][0]),Madd(ans,f[pre][i][1][1][1]);
	}
	write(ans);
}
```