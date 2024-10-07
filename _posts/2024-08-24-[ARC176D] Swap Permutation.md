# [My Blogs](https://www.cnblogs.com/WrongAnswer90/p/18377491)

### [[ARC176D] Swap Permutation](https://www.luogu.com.cn/problem/AT_arc176_d)

对每个位置分别算贡献。一个很重要的观察是其他所有数都是等价的（非常神奇）。设 $A$ 表示原来 $i$ 位置上的数，$B$ 表示原来 $i+1$ 位置上的数，$C$ 表示其他的数，设 $f_{0\sim 7}$ 表示经过 $m$ 次操作之后 $AB,BA,AC,CA,BC,CB,CC$ 的概率。每个位置 $i$ 的 $f$ 都是相同的。转移系数应该比较容易手搓出来，预处理 $f$ 可以暴力 DP，也可以矩阵快速幂（所以其实可以做 $m=10^{18}$ 的）。

设 $sum_i$ 表示 $\sum_j\text{abs}(i-j)$，$all$ 表示 $\sum_i\sum_j\text{abs}(i-j)$。对于第 $i$ 个位置，其贡献为：

$$
\begin{aligned}
&(f_0+f_1)\text{abs}(a_i-a_{i+1})\\
+&(f_2+f_3)\frac{(sum_{a_i}-\text{abs}(a_i-a_{i+1}))}{n-2}\\
+&(f_4+f_5)\frac{(sum_{a_{i+1}}-\text{abs}(a_i-a_{i+1}))}{n-2}\\
+&f_6\frac{all-sum_{a_{i+1}}-sum_{a_i}+\text{abs}(a_i-a_{i+1}))}{\binom{n-2}2}\\
\end{aligned}
$$

总复杂度 $\mathcal O(n+T^3\log m)$，其中 $T=7$。

```cpp
int n,m,a[200010];
struct Matrix{int f[7][7];Matrix(){memset(f,0,sizeof(f));}}A;
struct Vector{int f[7];Vector(){memset(f,0,sizeof(f));}}B;
inline Matrix operator *(const Matrix x,const Matrix y)
{
	Matrix z;
	for(int i=0;i<7;++i)for(int j=0;j<7;++j)for(int k=0;k<7;++k)
	Madd(z.f[i][j],Cmul(x.f[i][k],y.f[k][j]));
	return z;
}
inline Vector operator *(const Vector x,const Matrix y)
{
	Vector z;
	for(int j=0;j<7;++j)for(int k=0;k<7;++k)
	Madd(z.f[j],Cmul(x.f[k],y.f[k][j]));
	return z;
}
inline int calc(int x){return Cadd(Cmul(x-1,x,inv2),Cmul(n-x+1,n-x,inv2));}
//0:AB 1:BA 2:AC 3:CA 4:BC 5:CB 6:CC
inline void mian()
{
	read(n,m);int ans=0,all=0;
	for(int i=1;i<=n;++i)read(a[i]),Madd(all,calc(i));
	Mmul(all,inv2);
	if(n==2)return write(1);
	A.f[0][0]=Cmul(n-2,n-3,inv2),A.f[0][1]=1,A.f[0][2]=n-2,A.f[0][5]=n-2;
	A.f[1][0]=1,A.f[1][1]=Cmul(n-2,n-3,inv2),A.f[1][3]=n-2,A.f[1][4]=n-2;
	A.f[2][0]=1,A.f[2][2]=Cadd(Cmul(n-2,n-3,inv2),n-3),A.f[2][3]=1,A.f[2][4]=1,A.f[2][6]=n-3;
	A.f[3][1]=1,A.f[3][2]=1,A.f[3][3]=Cadd(Cmul(n-2,n-3,inv2),n-3),A.f[3][5]=1,A.f[3][6]=n-3;
	A.f[4][1]=1,A.f[4][2]=1,A.f[4][4]=Cadd(Cmul(n-2,n-3,inv2),n-3),A.f[4][5]=1,A.f[4][6]=n-3;
	A.f[5][0]=1,A.f[5][3]=1,A.f[5][4]=1,A.f[5][5]=Cadd(Cmul(n-2,n-3,inv2),n-3),A.f[5][6]=n-3;
	A.f[6][2]=1,A.f[6][3]=1,A.f[6][4]=1,A.f[6][5]=1,A.f[6][6]=Cdel(Cmul(n,n-1,inv2),4);
	B.f[0]=1;
	for(;m;m>>=1,A=A*A)if(m&1)B=B*A;
	int iv1=power(n-2,MOD-2),iv2=power(Cmul(n-2,n-3,inv2),MOD-2);
	for(int i=1;i<n;++i)
	{
		Madd(ans,Cmul(Cadd(B.f[0],B.f[1]),abs(a[i]-a[i+1])));
		Madd(ans,Cmul(iv1,Cadd(B.f[2],B.f[3]),Cdel(calc(a[i]),abs(a[i]-a[i+1]))));
		Madd(ans,Cmul(iv1,Cadd(B.f[4],B.f[5]),Cdel(calc(a[i+1]),abs(a[i]-a[i+1]))));
		Madd(ans,Cmul(iv2,B.f[6],Cadd(Cdel(all,calc(a[i]),calc(a[i+1])),abs(a[i]-a[i+1]))));
	}
	write(ans);
}
```