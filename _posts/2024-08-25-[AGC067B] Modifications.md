# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18378739)

### [[AGC067B] Modifications](https://www.luogu.com.cn/problem/AT_agc067_b)

谔谔，做过类似的题还是不会啊啊啊。

首先考虑给定一个 $a$ 序列如何进行判定。倒着做这个覆盖的过程，每次可以看成是，如果 $[l_i,r_i]$ 剩下的点的颜色都相同，则可以把 $[l_i,r_i]$ 删掉。如果最后能删空就是合法的。

区间 DP 判定这个过程：$f_{l,r}$ 表示考虑了左右端点都则区间 $[l,r]$ 的操作之后，区间 $[l,r]$ 的合法方案数。

发现直接计数似乎没有什么简单的合法充要条件。考虑进行容斥，用 $C^{r-l+1}$ 减去不合法的方案数就是答案。

考虑不合法区间的形态，最后一定是若干个断点没有被覆盖，把区间分成了若干个小区间。再考虑什么样的断点情况是真实存在的。除了断点之外的区间都能删空，所以如果单把断点的颜色序列拉出来，则对于一个相同颜色的颜色段，要求不能有跨过内部点的操作区间。

设 $g_{l,r,p}$ 表示 $r$ 是断点，$r$ 前面第一个和 $r$ 颜色不同的断点的位置在 $p$ 的方案数。首先预处理 $vis_{l,r,p}$ 表示 $[l,r]$ 的子操作区间能不能把 $p$ 覆盖。枚举 $r$ 前面第一个断点的位置 $i$。分 $i$ 的颜色和 $r$ 相同和不同讨论：

$$
\begin{aligned}
g_{l,r,p}&\overset+\leftarrow g_{l,i,p}f_{i+1,r-1}\\
g_{l,r,i}&\overset+\leftarrow g_{l,i,p}f_{i+1,r-1}(C-1)
\end{aligned}
$$

上述转移要求 $vis_{p+1,r-1,i}=0$。然后计算 $f$，枚举右边第一个断点容斥：

$$
f_{l,r}\overset-\leftarrow g_{l,i,p}f_{i+1,r}
$$

该转移要求 $vis_{p+1,r,i}=0$。总复杂度 $\mathcal O(n^4)$。一点小细节是 $g_{l,r,p}$ 只对除了 $r$ 以外的断点进行了检查合法，比如存在一个 $[r,r]$ 的操作区间，但是根据上面的 DP $g_{l,r,p}$ 还是会有值的。但是可以发现在用到 $g$ 的时候对 $r$ 进行了合法检查，所以直接这么写就是对的。

```cpp
int n,m,C,f[110][110],g[110][110][110];
bitset<110> vis[110][110];
vi ve[110];
inline void mian()
{
	read(n,m,C);int x,y;
	for(int i=1;i<=m;++i)read(x,y),ve[y].eb(x);
	for(int i=0;i<=n;++i)f[i+1][i]=1;
	for(int i=1;i<=n;++i)
	{
		for(int j=i;j<=n;++j)
		{
			int pos=j+1;vis[i][j]=vis[i][j-1];
			for(auto p:ve[j])if(p>=i)Mmin(pos,p);
			for(int k=pos;k<=j;++k)vis[i][j][k]=1;
		}
	}
	for(int i=n;i>=1;--i)for(int j=i;j<=n;++j)
	{
		f[i][j]=power(C,j-i+1),g[i][i-1][i-1]=1;
		for(int p=i-1;p<j;++p)for(int k=i-1;k<j;++k)if(!vis[p+1][j-1][k])
		Madd(g[i][j][k],Cmul(g[i][k][p],f[k+1][j-1],C-1)),
		Madd(g[i][j][p],Cmul(g[i][k][p],f[k+1][j-1]));
		for(int p=i-1;p<j;++p)for(int k=i;k<=j;++k)if(!vis[p+1][j][k])
		Mdel(f[i][j],Cmul(g[i][k][p],f[k+1][j]));
	}
	write(f[1][n]);
}
```