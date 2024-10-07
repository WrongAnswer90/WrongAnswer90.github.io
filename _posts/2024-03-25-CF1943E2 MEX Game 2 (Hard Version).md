---
layout: post
title: CF1943E2 MEX Game 2 (Hard Version)
subtitle: 题解
tags: 题解 CF
show: true
---

[CF1943E2 MEX Game 2 (Hard Version)](https://www.luogu.com.cn/problem/CF1943E2)

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/18094430)

好难啊好难啊好难啊，完全想不到 QAQ。

显然满足单调性，进行二分答案 $mid$，表示答案在双方最优策略下能否达到 $mid$。

Alice 的策略很简单，每次选取 $[0,mid-1]$ 中 $f$ 最小的没有选过的元素即可。

而 Bob 应当尽量把操作平均摊到多个 $f$ 上，不然一个 $f_j$ 忽然减了太多被 Alice 直接叉掉看起来就很不优。如果把 $i<mid$ 的 $F$ 进行排序得到 $H$，则 Bob 应当保证操作的过程中 $H$ 的大小顺序不改变。因为如果改变了没有意义，可以改变操作的对象使得操作后的 $H$ 仍然有序。

因此，Bob 的最优方案是游戏开始前选择一段 $H$ 的前缀 $H'$，然后每次操作的时候不断地对 $H'$ 的最大的元素减一执行 $K$ 次。Alice 每次删掉 $H$ 中最小的元素。暴力模拟这个过程可以做到 $\mathcal O(n^2\log n)$ 或者 $\mathcal O(n^3\log n)$。

考虑如何优化，二分答案不能丢，选取 $H$ 的前缀这个操作也很难舍去。

考虑对于一个前缀，操作过程中 Alice 和 Bob 一定会相遇，即存在一个 $j$ 使得在这个时刻 Alice 把 $[0,j-1]$ 全部删去了，Bob 把 $[j,i]$ 减成了 $\{x\dots x,x+1\dots x+1\}$ 的形式。

接下来是 Alice “怼着” Bob 向后走，当前 $[j,i]$ 的区间，可以用 $(sum,len)$ 两个数来表示剩的数的总和和剩的数的个数。此时假设 Bob 先手，每个回合是：

$$

sum\rightarrow sum-K

$$

然后

$$

sum\rightarrow sum-minn\\
len\rightarrow len-1

$$

其中 $minn=\lfloor\frac{sum}{len}\rfloor$。

现在需要知道的是 $sum$ 和 $len$ 哪个先变成非正数，如果 $sum$ 先则 Bob 胜利，否则 Alice 胜。所以可以预处理 $g_i$ 表示 $len=i$ 时 $sum$ 的最大值。需要满足：

$$
f_i-K-\lfloor\frac{f_i-K}i\rfloor\leq f_{i-1}\\

f_i=\lfloor\frac{f_{i-1}\times i}{i-1}\rfloor+K
$$

这样就做完了，代码还是比较好写的。复杂度是 $\mathcal O(n\log^2n)$，瓶颈在于排序。好像可以用倍增做到一只 $\log$？

```cpp
	int T,n,m,b[200010],f[200010],a[200010],s[200010],L,R,mid;
	inline bool chk(int x)
	{
		for(int i=0;i<=x;++i)b[i]=a[i];
		sort(b,b+x),s[0]=b[0];
		for(int i=1,j=1;i<x;++i)
		{
			s[i]=b[i]+s[i-1];
			while(s[i]-s[j-1]-b[j]*(i-j+1)>(j)*m)++j;
			if(s[i]-s[j-1]-(j-1)*m<=f[i-j+1])
			return 1;
		}
		return 0;
	}
	inline void mian()
	{
		read(T);
		while(T--)
		{
			read(n,m),L=0,R=n+1,f[1]=m;
			for(int i=2;i<=n+1;++i)f[i]=(ld)i*f[i-1]/(i-1)+m;
			for(int i=0;i<=n;++i)read(a[i]);
			while(L<R)
			{
				mid=L+((R-L)>>1);
				if(chk(mid))R=mid;
				else L=mid+1;
			}
			write(L-chk(L),'\n');
		}
	}
```