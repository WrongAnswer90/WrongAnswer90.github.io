---
layout: post
title: CF1340F Nastya and CBS
subtitle: 题解
tags: 题解 字符串 Hash 线段树 CF
show: true
---

[更好的阅读体验](https://www.cnblogs.com/WrongAnswer90-home/p/17975061)

[CF1340F Nastya and CBS](https://www.luogu.com.cn/problem/CF1340F)

绷不住了，30min 写完，虚空调试 2h+/lh/lh。

如果要准确做的话太困难了，考虑 hash。多次区间询问，考虑线段树。

一个区间如果内部合法，把内部能匹配的都匹配上，一定是左边一段右括号加上右边一段左括号。节点需要记录左边长度，右边长度和左右分别的 hash 值。

合并的时候，如果左儿子和右儿子有内部不合法的情况当前节点直接不合法。

另一种不合法的情况是左儿子的后缀和右儿子的前缀如果出现不能匹配的那这个区间就不合法。这个东西可以类似单侧递归线段树实现，线段树上二分查询左儿子一段后缀的 hash 或者右儿子一段前缀的 hash 与另外一个儿子的 hash 比较，若不相等则一定不合法，复杂度是 $\mathcal O(\log^2n)$。

比较左儿子后缀和右儿子的前缀长度。如果前者大于后者，则把左儿子的后缀的一段前缀拼上右儿子的全部后缀作为当前节点的后缀，反之亦然。

细节比较多，注意就算区间不合法也需要维护前后缀不匹配括号的长度，因为需要线段树上二分。模数卡 $998244353$，可以用 $10^9+7$。

代码写的比较丑，见谅/wul。

```cpp
	int n,m,K,a[100010],fr[100010];
	namespace Segment
	{
		struct Node{int l,r,le,ri,ln,rn,fl;}t[400010],NUL;
		Node mul(Node nd1,Node nd2,int id)
		{
			Node nd;nd=NUL,nd.l=nd1.l,nd.r=nd2.r,nd.fl=0;
			if(nd1.fl||nd2.fl)return nd.fl=1,nd;
			if(nd1.rn>nd2.ln)
			{
				nd.le=nd1.le,nd.ln=nd1.ln;
				nd.rn=nd1.rn-nd2.ln+nd2.rn;
				nd.ri=Cmul(nd2.ri,fr[nd1.rn-nd2.ln]);
				Madd(nd.ri,Cdel(nd1.ri,Cmul(nd2.le,fr[nd1.rn-nd2.ln])));
			}
			else
			{
				nd.ri=nd2.ri,nd.rn=nd2.rn;
				nd.ln=nd1.ln+nd2.ln-nd1.rn;
				nd.le=Cmul(nd1.le,fr[nd2.ln-nd1.rn]);
				Madd(nd.le,Cdel(nd2.le,Cmul(nd1.ri,fr[nd2.ln-nd1.rn])));
			}
			return nd;
		}
		Node queryr(int p,int k)
		{
			if(!k)return NUL;
			if(t[p].l==t[p].r)return t[p];
			if(t[p*2+1].rn>=k)return queryr(p*2+1,k);
			return mul(queryr(p*2,k-t[p*2+1].rn+t[p*2+1].ln),t[p*2+1],p);
		}
		Node queryl(int p,int k)
		{
			if(!k)return NUL;
			if(t[p].l==t[p].r)return t[p];
			if(t[p*2].ln>=k)return queryl(p*2,k);
			return mul(t[p*2],queryl(p*2+1,k-t[p*2].ln+t[p*2].rn),p);
		}
		Node add(Node nd1,Node nd2,int id)
		{
			Node nd;nd=NUL,nd.l=nd1.l,nd.r=nd2.r,nd.fl=0;
			nd.ln=nd1.ln+max(nd2.ln-nd1.rn,0);
			nd.rn=nd2.rn+max(nd1.rn-nd2.ln,0);
			if(nd1.fl||nd2.fl)
			return nd.fl=1,nd;
			if(nd1.rn>nd2.ln)
			{
				if(queryr(id*2,nd2.ln).ri!=nd2.le)return nd.fl=1,nd;
				nd.le=nd1.le,nd.ri=Cmul(nd2.ri,fr[nd1.rn-nd2.ln]);
				Madd(nd.ri,Cdel(nd1.ri,Cmul(nd2.le,fr[nd1.rn-nd2.ln])));
			}
			else
			{
				if(queryl(id*2+1,nd1.rn).le!=nd1.ri)return nd.fl=1,nd;
				nd.ri=nd2.ri,nd.le=Cmul(nd1.le,fr[nd2.ln-nd1.rn]);
				Madd(nd.le,Cdel(nd2.le,Cmul(nd1.ri,fr[nd2.ln-nd1.rn])));
			}
			return nd;
		}
		void build(int p,int l,int r)
		{
			t[p].l=l,t[p].r=r;
			if(l==r)
			{
				if(a[l]<0)t[p].ln=1,t[p].rn=t[p].ri=0,t[p].le=-a[l];
				else t[p].rn=1,t[p].ln=t[p].le=0,t[p].ri=a[l];
				return;
			}
			int mid=l+((r-l)>>1);
			build(p*2,l,mid),build(p*2+1,mid+1,r);
			t[p]=add(t[p*2],t[p*2+1],p);
		}
		void change(int p,int x,int y)
		{
			if(t[p].l==t[p].r)
			{
				if(y<0)t[p].ln=1,t[p].rn=t[p].ri=0,t[p].le=-y;
				else t[p].rn=1,t[p].ln=t[p].le=0,t[p].ri=y;
				return;
			}
			x<=t[p*2].r?change(p*2,x,y):change(p*2+1,x,y);
			t[p]=add(t[p*2],t[p*2+1],p);
		}
		Node ask(int p,int l,int r)
		{
			if(l<=t[p].l&&r>=t[p].r)return t[p];
			if(r<=t[p*2].r)return ask(p*2,l,r);
			if(l>t[p*2].r)return ask(p*2+1,l,r);
			Node lef=ask(p*2,l,r);
//			assert(lef.fl||lef.rn<=t[p*2].rn);
			Node rig=ask(p*2+1,l,r);
//			assert(rig.fl||rig.ln<=t[p*2+1].ln);
			return add(lef,rig,p);
		}
		void print(int p)
		{
			write('(',t[p].l,',',t[p].r,')',t[p].fl,' ',t[p].le,' ',t[p].ln,' ',t[p].ri,' ',t[p].rn,'\n');
			if(t[p].l==t[p].r)return;
			print(p*2),print(p*2+1);
		}
	}
	using namespace Segment;
	inline void mian()
	{
		read(n,K),fr[0]=1,NUL={0,0,0,0,0,0,0};int opt,x,y;
		for(int i=1;i<=n;++i)read(a[i]),fr[i]=Cmul(fr[i-1],131);
		build(1,1,n),read(m);int M=m;
		while(m--)
		{
//			cerr<<M-m<<endl;
			read(opt,x,y);
			if(opt==1)change(1,x,y);
			else
			{
				Node nd=ask(1,x,y);
				if(!nd.ln&&!nd.rn&&!nd.le&&!nd.ri&&!nd.fl)puts("Yes");
				else puts("No");
			}
		}
	}
```