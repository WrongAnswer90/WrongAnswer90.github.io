---
layout: post
title: CF1862F Magic Will Save the World
subtitle: 题解
tags: 题解 bitset DP优化 CF
show: true
---

bitset 优化可行性 DP。
注意到所有怪物需要魔法的和是一定的，问题转为判定是否能够恰好消耗 $i$ 点水魔法和 $sum-i$ 点火魔法，用 $f_i$ 表示这种分割方案是否可行，直接 dp 大概率会超时，使用 bitset 优化即可，最后根据 $f_i$ 统计答案。

代码：

```cpp
int T,A,B,n,minn,sum;
inline void mian()
{
    read(T);int x;
    while(T--)
    {
        read(A,B,n),sum=0,minn=INF;
        bitset<1000001> f;f[0]=1;
        while(n--)read(x),f|=(f<<x),sum+=x;
        for(int i=0;i<=1000000;++i)if(f[i])minn=min(minn,max((i-1)/A+1,(sum-i-1)/B+1));
        write(minn,'\n');
    }
}

```