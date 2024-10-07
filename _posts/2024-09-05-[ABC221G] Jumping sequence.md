---
layout: post
title: ABC221G Jumping sequence
subtitle: 题解
tags: 题解 DP优化 AT
show: true
---

# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18399404)

### [[ABC221G] Jumping sequence](https://www.luogu.com.cn/problem/AT_abc221_g)

做法有点深刻，优化方式非常深刻。

首先是哈密顿距离和切比雪夫距离的转化：把坐标系旋转四十五度，变成每次向左上，右上，左下，右下走 $\sqrt 2 a_i$ 的距离，要求最后走到 $(A+B,A-B)$。然后这样可以对于横纵坐标分开做了（非常神奇）。

问题转化成了询问是否存在序列 $x$ 满足 $\forall i,x_i\in\{-1,1\},A=\sum x_ia_i$。先钦定全都是 $+1$ 或者 $-1$ 之后可以变形为，询问是否能从 $a$ 中选择若干个数使得和为 $A$。可以直接 `bitset` 做到 $\mathcal O(\frac{nV^2}{w})$，但是存在更优秀的做法。

首先从前向后找到最大的 $mid$ 满足 $\sum_{j\leq mid}a_j\leq A$。然后如果存在解，则一定是前面删掉若干个元素然后后面再加进来若干个元素。可以设 $f_{l,r,i}$ 表示从 $[l,mid]$ 中选择一些负数，$[mid+1,r]$ 中选择一些正数，能否凑出来 $i$。

一个关键性质是 $i$ 只需要保留 $[-V,V]$ 的部分，其中 $V$ 是题目中数列的值域。证明比较简单，对于一种方案，假设选取的左侧负数是 $x_1\dots x_k$，右侧选取的正数是 $y_1\dots y_s$，如果当前 $i$ 是正的就在左边加负数，否则在右边加正数。如果有一个序列取空了剩下的就直接全取了。因为 $A-\sum_{j\leq mid}a_j\leq V$，所以上面这样值域不会超过 $[-V,V]$。

但是 $f$ 的状态就是 $\mathcal O(nV^2)$ 的。注意到如果固定了 $i,r$，合法的 $l$ 是一段前缀。可以记录 $g_{i,j}$ 表示当前右端点是 $i$，值是 $j$，合法的最大左端点是 $g_{i,j}$。转移：

$$
\begin{aligned}
g_{i+1,j}&\leftarrow g_{i,j}\\
g_{i+1,j+a_i}&\leftarrow g_{i,j}\\
g_{i+1,j-a_k}&\leftarrow k\;\;\;\;\;\;\;\;\;\;\;\;\;\;k\leq g_{i+1,j}
\end{aligned}
$$

现在复杂度瓶颈在于第三种转移。但是可以发现许多第三种转移都是没用的：如果 $k\leq g_{i,j}$，则可以在 $i-1$ 的时候就转移掉这个 $k$。所以每次只需要转移新添进候选值的 $k$，即 $k\in(g_{i,j},g_{i+1,j}]$。因为 $g$ 单调不降，对于一个 $j$，$k$ 的枚举次数即为 $(\sum_{i}g_{i,j}-g_{i-1,j})\leq n$，所以这样总复杂度是 $\mathcal O(nV)$。输出方案也比较平凡，只需要记录从哪个状态转移过来的即可。

```cpp
int n,A,B,a[2010],f[2010][4010];
pii from[2010][4010];
bitset<2010> v1,v2;
void solve(int X)
{
    if(X<0)puts("No"),exit(0);
    int v=0,I=0;memset(f,0,sizeof(f));
    while(I<n&&v+a[I+1]<=X)v+=a[++I];
    if(I==n&&v<X)puts("No"),exit(0);
    v=X-v,f[I][2000]=I+1,v1.reset();
    for(int i=I+1;i<=n;++i)
    {
        for(int j=4000;j>=0;--j)
        {
            if(Mmax(f[i][j],f[i-1][j]))from[i][j]=mp(i-1,j);
            if(j>a[i])if(Mmax(f[i][j],f[i-1][j-a[i]]))
            from[i][j]=mp(i-1,j-a[i]);
            for(int k=f[i-1][j];k<f[i][j];++k)if(j>=a[k])
            if(Mmax(f[i][j-a[k]],k))from[i][j-a[k]]=mp(i,j);
        }
    }
    if(!f[n][2000+v])puts("No"),exit(0);
    for(int i=1;i<=I;++i)v1[i]=1;
    int i=n,j=2000+v,x,y;
    while(i!=I||j!=2000)
    {
        x=from[i][j].fi,y=from[i][j].se;
        if(x==i)v1[f[i][j]]=0;
        else if(y!=j)v1[i]=1;
        i=x,j=y;
    }
}
inline void mian()
{
    read(n,A,B),A=A+B,B=A-B-B;
    for(int i=1;i<=n;++i)read(a[i]),A+=a[i],B+=a[i];
    if((A&1)||(B&1))return puts("No"),void();
    solve(A>>1),v2=v1,solve(B>>1),puts("Yes");
    for(int i=1;i<=n;++i)
    {
        if(v1[i]==v2[i]){if(v1[i])putchar('R');else putchar('L');}
        else{if(v2[i])putchar('U');else putchar('D');}
    }
}
```