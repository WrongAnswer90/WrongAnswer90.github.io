---
layout: post
title: ARC184E Accumulating Many Times
subtitle: 题解
tags: 题解 数数 NTT Lucas定理 AT
show: true
---

# [My Blogs](https://wronganswer90.github.io/2024/10/09/ARC184E-Accumulating-Many-Times)

### [[ARC184E] Accumulating Many Times](https://www.luogu.com.cn/problem/AT_arc184_e)

感觉比 C 简单啊/kk。文中序列下标从 $0$ 开始。

根据前缀和的定义，一个序列 $A[0,n-1]$ 做 $k$ 次前缀和之后，每个位置可以看成是每次可以向后走若干步。所以 $i$ 对 $j$ 的贡献就等价于一共要走 $j-i$ 步，每次可以走若干步的方案数，插板法可以得到系数是 $\binom{j-i+k-1}{j-i}$。

二进制下的组合数，套路的用 Lucas 定理简化成：$(j-i)\subseteq (j-i+k-1)$。这同时也等价于 $(j-i)$ 和 $(k-1)$ 无交。

这样可以发现一个结论：因为 $j-i$ 的最大值是 $n-1$，所以 $k$ 的有效位只有 $\log (n-1)$ 位。设 $t=\log(n-1)+1$，则做了 $2^t$ 次前缀和后序列会恢复原序列，如果序列的第一位是 $1$，则 $2^t$ 也是最小的能使得序列为原序列的前缀和次数。

同时不同的序列构成了若干个环的关系，在环上走一步就是做一遍前缀和。现在要把不同的序列按照所在环归类，一个想法是寻找环的一个代表元，不妨设其为其中字典序最小的序列。

考虑如何找一个给定序列所在换的代表元：首先把序列的前导零删除，找到环长。从低位向高位确定 $k$，设 $k$ 的第 $i$ 位是 $k_i$，则因为 $a_0$ 是 $1$，所以 $k_i$ 会影响的第一位就是 $a_{2^i}$。$k$ 只有 $\log$ 位，每次根据上面的结论 $\mathcal O(m)$ 暴力计算出 $a_{2^i}$ 当前的值，就能判断出 $k_i$ 应当是 $0$ 还是 $1$ 了。

求出了 $k$ 之后还需要找到代表元是什么，可以用一遍 NTT 解决。这样就把原题给定的所有序列分类好了，对于同一类的，答案就是 $\sum_i\sum_j(k_i-k_j)\bmod len$，可以用树状数组解决。总复杂度 $\mathcal O(nm\log m)$。