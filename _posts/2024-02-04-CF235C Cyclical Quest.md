---
layout: post
title: CF235C Cyclical Quest
subtitle: 题解
tags: 题解 字符串 SAM CF
show: true
---

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/18007071)

[CF235C Cyclical Quest](https://www.luogu.com.cn/problem/CF235C)

SAM 做法：

对于主串建 SAM，然后 parent tree 上 DP 出 $f_i$ 表示节点 $i$ 后缀等价类的出现次数。询问先用 KMP 求最小循环元 $m$，然后接下来需要把 $[1,n],[2,n]\dots[m-1,n+m-1]$ 扔进 SAM 里跑。

暴力显然过不去，但是跑完一次可以删去最开始的字符然后继续跑。删去开头字符等价于在 parent tree 上向上跳，即当 $len_{fa_now}\geq n$ 时 $now\rightarrow fa_{now}$。同时跑不动的时候也向上跳，并把当前的 $len$ 设为 $len_{fa}$。

总复杂度线性。

后缀数组做法：

离线，把主串和询问串加循环元拼在一起，对于每个不是主串的后缀前后二分找到满足 LCP 长度为 $n$ 的区间然后前缀和相见即可。

好像比 SAM 简单一些，但是复杂度带一只 $\log$。

提供 SAM 代码：

```cpp
	int n,TT,m,now,r,ans,nex[1000010],f[2000010];
	char x[2000010],s[1000010];
	vector<int> T[2000010];
	namespace SAM
	{
		int last=1,cnt=1;
		struct{int ch[26],fa,len;}t[2000010];
		inline int add(int x)
		{
			int np=++cnt,p=last,q,nq;t[last=np].len=t[p].len+1,f[np]=1;
			while(p&&!t[p].ch[x])t[p].ch[x]=np,p=t[p].fa;
			if(!p)return t[np].fa=1,np;
			if(t[q=t[p].ch[x]].len==t[p].len+1)return t[np].fa=q,np;
			t[nq=++cnt]=t[q],t[nq].len=t[p].len+1,t[np].fa=t[q].fa=nq;
			while(p&&t[p].ch[x]==q)t[p].ch[x]=nq,p=t[p].fa;
			return np;
		}
	}
	using namespace SAM;
	void dfs(int x){for(auto to:T[x])dfs(to),f[x]+=f[to];}
	inline void mian()
	{
		read(s,TT),n=strlen(s+1);
		for(int i=1;i<=n;++i)add(s[i]-'a');
		for(int i=2;i<=cnt;++i)T[t[i].fa].eb(i);
		dfs(1);
		while(TT--)
		{
			read(x),n=strlen(x+1);
			for(int i=1;i<=n;++i)x[n+i]=x[i];
			for(int j=0,i=2;i<=n;nex[i++]=j)
			{
				while(j&&x[j+1]!=x[i])j=nex[j];
				if(x[j+1]==x[i])++j;
			}
			if(n%(n-nex[n])==0)m=n-nex[n];else m=n;
			now=1,r=0,ans=0;
			for(int i=n,r=0,len=0;now&&i<n+m;++i)
			{
				while(r<i)
				{
					++r;
					while(now&&!t[now].ch[x[r]-'a'])now=t[now].fa,len=t[now].len;
					now=t[now].ch[x[r]-'a'],++len;
				}
				while(t[t[now].fa].len>=n)now=t[now].fa;
				if(len>=n)ans+=f[now];
			}
			write(ans,'\n');
		}
	}
```