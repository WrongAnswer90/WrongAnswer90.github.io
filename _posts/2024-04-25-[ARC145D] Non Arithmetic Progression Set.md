---
layout: post
title: ARC145D Non Arithmetic Progression Set
subtitle: 题解
tags: 题解 构造 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18157231)

### [[ARC145D] Non Arithmetic Progression Set](https://www.luogu.com.cn/problem/AT_arc145_d)

考虑三进制，如果只选只有 $01$ 的数就一定合法。

然后可以考虑平移，即每个数同时 $\pm C$。如果原序列合法，经过该操作之后一定仍然合法。所以只需要构造一个序列，使得和 $\bmod\;n$ 意义下和 $m$ 同余。

暴力选取最小的 $n-1$ 个合法三进制数，接下来补一个非常大的数，使得 $sum$ 同余 $m$，打表可以发现第 $9999$ 个合法的数是 $1.7\times 10^6$ 左右，所以补的数如果接近上界就不会出现不合法的情况。

最后再算一下差值，通过平移把 $sum$ 变成 $m$。

```cpp
int n,m,len,sum,a[10010];
inline void mian()
{
	read(n,m);
	int sum=0;
	for(int i=0;n-1;++i)
	{
		int x=i,fl=1;
		while(x)fl&=(x%3!=2),x/=3;
		if(!fl)continue;
		a[++len]=i,sum+=i;
		if(len==n-1)break;
	}
	int ned=((m-sum)%n+n)%n;
	ned+=(9000000-ned)/n*n,a[n]=ned,sum+=ned,assert(sum%n==(m%n+n)%n);
	int delta=(sum-m)/n;
	for(int i=1;i<=n;++i)a[i]-=delta,write(a[i]);
}
```