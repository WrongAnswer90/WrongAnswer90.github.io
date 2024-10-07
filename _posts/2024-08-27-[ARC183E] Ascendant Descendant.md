# [My Blogs]()

### [[ARC183E] Ascendant Descendant](https://www.luogu.com.cn/problem/AT_arc183_e)

一个直接的想法是求出 $L_i,R_i$ 表示极大的区间 $[L_i,R_i]$ 满足 $\forall j\in[L_i,R_i],b_j\in \text{subtree}(a_i)$。由于树的性质，$[L_i,R_i]$ 之间要么相离，要么包含。

但是 $L_i,R_i$ 并不是 $i$ 能真正到达的点。因为 $i$ 只能一个一个交换过去，中途可能会有一些点阻碍着 $i$ 的交换。具体的，把 $[L_i,R_i]$ 构成的树形结构建出来，会阻碍交换的点就是满足 $siz_i=R_i-L_i+1$ 的整棵子树。必要性和充分性都比较显然。（但是想不到啊）

求 $[L_i,R_i]$ 是平凡的，一个 $\mathcal O(1)$ LCA 和线段树或者 ST 表就能在 $\mathcal O(m\log m)$ 的时间里求出来。后半部分实现的时候可以按照区间长度排序，拿一个 BIT 维护区间和，每次是区间内任选一个点单点加一即可。再用一个 'set' 维护删掉的点。总复杂度 $\mathcal O(n\log n)$。

```cpp
int n,m,L,R,mid,tot,fr[250010],a[250010],l[250010],r[250010];
int F[18][250010],G[18][250010],dfn[250010];
vi T[250010];
tup c[250010];
inline int get(int x,int y){return dfn[x]<dfn[y]?x:y;}
inline bool cmp(tup x,tup y){return (x.y-x.x)<(y.y-y.x)||((x.y-x.x)==(y.y-y.x)&&a[x.z]<a[y.z]);}
void dfs(int x,int fa=0){F[0][dfn[x]=++tot]=fa;for(auto to:T[x])dfs(to,x);}
inline int LCA(int x,int y)
{
	if(x==y)return x;
	if((x=dfn[x])>(y=dfn[y]))swap(x,y);
	int k=__lg(y-x++);
	return get(F[k][x],F[k][y-(1<<k)+1]);
}
inline int ask(int l,int r){int k=__lg(r-l+1);return LCA(G[k][l],G[k][r-(1<<k)+1]);}
namespace BIT
{
	int t[250010];
	inline void add(int x){for(;x<=m;x+=x&-x)++t[x];}
	inline int ask(int x){int s=0;for(;x;x-=x&-x)s+=t[x];return s;}
}
using namespace BIT;
set<int> st;
map<pii,int> hash;
inline void mian()
{
	read(n,m),fr[0]=1;int x,ans=1;
	for(int i=1;i<=m;++i)fr[i]=Cmul(fr[i-1],i);
	for(int i=2;i<=n;++i)read(x),T[x].eb(i);
	dfs(1);
	for(int i=1;i<18;++i)for(int j=1;j+(1<<i)-1<=n;++j)
	F[i][j]=get(F[i-1][j],F[i-1][j+(1<<(i-1))]);
	for(int i=1;i<=m;++i)read(a[i]);
	for(int i=1;i<=m;++i)read(G[0][i]);
	for(int i=1;i<18;++i)for(int j=1;j+(1<<i)-1<=m;++j)
	G[i][j]=LCA(G[i-1][j],G[i-1][j+(1<<(i-1))]);
	for(int i=1;i<=m;++i)
	{
		L=1,R=i;
		while(L<R)
		{
			mid=L+((R-L)>>1); 
			if(LCA(ask(mid,i),a[i])==a[i])R=mid;
			else L=mid+1;
		}
		c[i].x=L,L=i,R=m;
		while(L<R)
		{
			mid=L+((R-L+1)>>1);
			if(LCA(ask(i,mid),a[i])==a[i])L=mid;
			else R=mid-1;
		}
		c[i].y=L,c[i].z=i;
	}
	sort(c+1,c+1+m,cmp),st.insert(0),st.insert(m+1);
	for(int i=1;i<=m;++i)
	{
		int xx=c[i].x,yy=c[i].y;
		Mmax(c[i].x,(*(--st.lower_bound(c[i].z)))+1);
		Mmin(c[i].y,(*st.lower_bound(c[i].z))-1);
		int v=c[i].y-c[i].x+1-(ask(c[i].y)-ask(c[i].x-1));
		Mmul(ans,v),add(c[i].z);
		hash[mp(c[i].x*m+c[i].y,a[c[i].z])]++;
		if(ask(yy)-ask(xx-1)==yy-xx+1)st.insert(xx),st.insert(yy);
	}
	for(auto [x,y]:hash)Mmul(ans,power(fr[y],MOD-2));
	write(ans);
}
```