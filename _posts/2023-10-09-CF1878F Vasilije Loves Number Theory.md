---
layout: post
title: CF1878F Vasilije Loves Number Theory
subtitle: 题解
tags: 题解 模拟 CF
show: true
---

[CF1878F Vasilije Loves Number Theory](https://www.luogu.com.cn/problem/CF1878F)

首先约数个数是积性函数，题目中要求 $\gcd(n,a)=1$，所以 $a$ 和 $n$ 互质，$n=d(a)d(n)$ ,于是问题转化为 $n$ 是否整除 $d(n)$。

观察题目，$n$ 可能会非常大，但这也启发我们把它质因数分解来存储，而对于 $d(n)$ 直接用一个变量 $ans$ 存下来，考虑 $n\times x$ 之后 $ans$ 会有什么变化。

设操作前 $n=p_1^{c_1}p_2^{c_2}\cdots p_k^{c_k}$，则 $ans=(c_1+1)(c_2+1)\cdots(c_k+1)$，直接乘 $x$ 是不好算的，考虑把 $x$ 也拆成若干个质数依次计算，对于 $x$ 的每个质因子 $p_i$，$ans$ 会变成 $(c_1+1)(c_2+1)\cdots(c_i+2)\cdots(c_k+1)$，所以贡献是好计算的。

代码细节比较多，不太好写，实际实现可以用 map 存储每一位质因数的位置，对于操作 $2$ 直接记下开始的状态暴力 memset 即可，复杂度 $\mathcal O(q(\log n+\sqrt x))$。

```cpp
int T,q,n,ans,prans,cnt,stcnt,all,pr[2001],st[2001],p[2001],now[2001];
map<int,int> mp1,mpr;
void mian()
{
	read(T);int x,y;
	while(T--)
	{
		read(n,q),ans=1,memset(st,0,sizeof(st)),mp1.clear(),mpr.clear(),cnt=0;
		for(int i=2;i*i<=n;++i)
		{
			if(n%i==0)
			{
				pr[++cnt]=i,mpr[i]=cnt;
				while(n%i==0)n/=i,++st[cnt];
				ans*=(st[cnt]+1);
			}
		}
		if(n!=1)pr[++cnt]=n,mpr[n]=cnt,st[cnt]=1,ans*=2;
		stcnt=cnt,prans=ans,mp1=mpr,memcpy(now,st,sizeof(st)),memcpy(p,pr,sizeof(p));
		while(q--)
		{
			read(x);
			if(x==2)cnt=stcnt,ans=prans,mp1=mpr,memcpy(now,st,sizeof(now)),memcpy(p,pr,sizeof(pr));
			else
			{
				read(y);
				for(int i=2;i*i<=y;++i)
				{
					if(y%i==0)
					{
						if(!mp1[i])p[++cnt]=i,mp1[i]=cnt;
						int pos=mp1[i];
						while(y%i==0)ans+=ans/(now[pos]+1),++now[pos],y/=i;
					}
				}
				if(y!=1)
				{
					if(!mp1[y])p[++cnt]=y,mp1[y]=cnt;
					int pos=mp1[y];
					ans+=ans/(now[pos]+1),++now[pos];
				}
				int flag=1,x=ans;
				for(int i=1;i<=cnt;++i)
				{
					int sum=0;
					while(x%p[i]==0)x/=p[i],++sum;
					if(sum>now[i])flag=0;
				}
				if(x!=1)flag=0;
				puts(flag?"yes":"no");
			}
		}
		puts("");
	}
}
```