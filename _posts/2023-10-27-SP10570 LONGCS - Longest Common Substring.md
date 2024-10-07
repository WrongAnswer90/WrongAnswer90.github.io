---
layout: post
title: SP10570 LONGCS - Longest Common Substring
subtitle: 题解
tags: 题解 SA 字符串 Luogu
show: true
---

[SP10570 LONGCS - Longest Common Substring](https://www.luogu.com.cn/problem/SP10570)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17792427.html)

提供一个后缀数组解法。

多字符串，中间加分隔符然后后缀排序求出 $sa$ 和 $height$。把每个字符串对应的位置染上颜色，问题变为寻找 $i,j$ 使得区间 $[i,j]$ 包含 $n$ 种颜色并且 $\min_{k=i+1}^{j}height_k$ 最大。可以从后向前扫一遍，枚举左端点，寻找合法的最靠左的右端点，颜色数可以开桶维护。对于区间 $\min$，由于左右端点单调不增，所以可以单调队列。复杂度 $\mathcal O(len\log len+len)$，瓶颈在于后缀排序。

```cpp
	int T,n,m,len,le,maxn,col[500020],height[500020],sa[500020],rk[500020],x[500020],xx[500020],y[500020],t[500020];
	char s[500020];
	deque<int> q;
	inline void mian()
	{
		read(T);
		while(T--)
		{
			read(n),m=128,memset(col,0,sizeof(col)),len=maxn=0,memset(t,0,sizeof(t));
			for(int i=1;i<=n;++i)
			{
				scanf("%s",s+1),le=strlen(s+1);
				for(int j=1;j<=le;++j)++t[x[++len]=s[j]],col[len]=i;
				if(i!=n)++t[x[++len]=++m];
			}
			if(n==1){write(le,'\n');continue;}
			memcpy(xx,x,sizeof(x)),swap(n,len);
			for(int i=2;i<=m;++i)t[i]+=t[i-1];
			for(int i=1;i<=n;++i)sa[t[x[i]]--]=i;
			for(int k=1,num=0;k<=n;k<<=1,num=0)
			{
				for(int i=n;i>n-k;--i)y[++num]=i;
				for(int i=1;i<=n;++i)if(sa[i]>k)y[++num]=sa[i]-k;
				for(int i=1;i<=m;++i)t[i]=0;
				for(int i=1;i<=n;++i)++t[x[i]];
				for(int i=1;i<=m;++i)t[i]+=t[i-1];
				for(int i=n;i>=1;--i)sa[t[x[y[i]]]--]=y[i];
				swap(x,y),x[sa[1]]=1;
				for(int i=2;i<=n;++i)
				{
					if(y[sa[i]]==y[sa[i-1]]&&y[sa[i]+k]==y[sa[i-1]+k])x[sa[i]]=x[sa[i-1]];
					else x[sa[i]]=x[sa[i-1]]+1;
				}
				if((m=x[sa[n]])==n)break;
			}
			for(int i=1;i<=n;++i)rk[sa[i]]=i;
			for(int i=1,k=0;i<=n;k?--k:0,++i)
			{
				while(xx[i+k]==xx[sa[rk[i]-1]+k])++k;
				height[rk[i]]=k;
			}
			memset(t,0,sizeof(t));
			while(!q.empty())q.pop_back();
			for(int i=n-len+1,sum=0,r=n-len+1;i>=1;--i)
			{
				if(col[sa[i]]&&!t[col[sa[i]]]++)++sum;
				while(r>=i&&sum==len&&t[col[sa[r]]]!=1)--t[col[sa[r--]]];
				while(!q.empty()&&q.front()>r)
				q.pop_front();
				if(sum==len)maxn=max(maxn,height[q.front()]);
				while(!q.empty()&&height[q.back()]>height[i])q.pop_back();
				q.eb(i);
			}
			write(maxn,'\n');
		}
	}
```