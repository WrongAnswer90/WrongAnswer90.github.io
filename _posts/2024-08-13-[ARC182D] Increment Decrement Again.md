# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18356203)

### [[ARC182D] Increment Decrement Again](https://www.luogu.com.cn/problem/AT_arc182_d)

判掉 $m=2$。接下来有个奇妙的转化：看成 $A$ 不对 $m$ 取模，要求变为 $\forall i<n,|a_i-a_{i+1}|<m\wedge a_i\not= a_{i+1}$。

然后发现 $a$ 的大小关系是不会变的，所以如果固定了 $a'_1$，则后面的都是固定的。可以先令 $a'_1=b_1$，根据大小关系求出任意一组合法的 $a'$，接下来问题变为最小化 $\sum_{i=1}^n|a_i-a'_i-km|$，可以二分类似找中位数的方法解决。复杂度 $\mathcal O(n\log n\log V)$。

```cpp
int n,m,L,R,mid,p,a[200010],b[200010],c[200010];
inline void mian()
{
	read(n,m);
	for(int i=1;i<=n;++i)read(a[i]);
	for(int i=1;i<=n;++i)read(b[i]);
	if(m==2){if(a[1]==b[1])puts("0");else puts("-1");return;}
	c[1]=b[1];
	for(int i=2;i<=n;++i)
	{
		int val=(b[i]-b[i-1]+m)%m;
		if(a[i]>a[i-1])c[i]=c[i-1]+val;
		else c[i]=c[i-1]+val-m;
	}
	for(int i=1;i<=n;++i)c[i]=a[i]-c[i];
	sort(c+1,c+1+n),L=-1000*inf,R=1000*inf;
	while(L<R)
	{
		mid=L+((R-L+1)>>1),p=lower_bound(c+1,c+1+n,mid*m)-c;
		if(p<=(n+1)/2)L=mid;
		else R=mid-1;
	}
	int ans=0,s=0;
	for(int i=1;i<=n;++i)ans+=abs(c[i]-L*m);
	for(int i=1;i<=n;++i)s+=abs(c[i]-(L+1)*m);
	write(min(ans,s));
}
```