#include<bits/stdc++.h>
#include<windows.h>
#include<wchar.h>
using namespace std;
inline void commit()
{
	char s[210];
	time_t tmp;
	//ababa
	time(&tmp);
	struct tm *p;
	p=localtime(&tmp);
	sprintf(s,"git commit -m \"Updated on %d-%02d-%02d %02d:%02d\"",1900+p->tm_year,
	1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min);
	
	int x=system("git add .");
	int y=system(s);
	int z;
	again:
	z=system("git push -f origin main");
	cerr<<x<<" "<<y<<" "<<z<<endl;
	if(z)
	{
		Sleep(20000);
		goto again;
	}
}
int len;
int len2;
string s[100010],t;

std::string string_to_UTF8(const std::string &str)
{
	int nwLen=MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,NULL,0);
	 
	wchar_t *pwBuf=new wchar_t[nwLen+1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf,nwLen*2+2);
	
	MultiByteToWideChar(CP_ACP,0,str.c_str(),str.length(),pwBuf,nwLen);
	
	int nLen=WideCharToMultiByte(CP_UTF8,0,pwBuf,-1,NULL,0,NULL,NULL);
	 
	char *pBuf=new char[nLen+1];
	ZeroMemory(pBuf,nLen+1);
	 
	WideCharToMultiByte(CP_UTF8,0,pwBuf,nwLen,pBuf,nLen,NULL,NULL);
	 
	string retStr(pBuf);
	 
	delete []pwBuf;
	delete []pBuf;
	
	pwBuf=NULL;
	pBuf=NULL;
	
	return retStr;
}

inline void get()
{
	ifstream fin("C:\\Users\\Administrator\\Desktop\\1.md");
	while(getline(fin,s[len]))++len;
	
	string path="F:\\Blogs\\WrongAnswer90.github.io\\_posts\\",date="";
	time_t tmp;
	time(&tmp);
	struct tm *p;
	p=localtime(&tmp);
	char hlp[4];
	
	date+=to_string(1900+p->tm_year)+"-";
	sprintf(hlp,"%02d",1+p->tm_mon);
	date+=(string)hlp+"-";
	sprintf(hlp,"%02d",p->tm_mday);
	date+=(string)hlp+"-";
	
	string name,subtitle,tags;
	cerr<<"Post Name:";
	getline(cin,name);
	
	cerr<<"Post Subtitle:";
	getline(cin,subtitle);
	
	cerr<<"Post Tags:";
	getline(cin,tags);
	
	ofstream fout((path+date+name+".md").c_str());


	t+="---\n";
	t+="layout: post\n";
	string TT="",S=name;
	for(int i=0;i<(int)S.size();++i)
	{
		if(S[i]=='['||S[i]==']')continue;
		TT+=S[i];
	}
	t+="title: "+TT+"\n";
	t+="subtitle: "+subtitle+"\n";
	if((int)tags.length()>0)t+="tags: "+tags+"\n";
	t+="show: true\n";
	t+="---\n\n";
	
	S=date+S;
	string T;
	for(int i=0;i<(int)S.size();++i)
	{
		if(S[i]=='['||S[i]==']')continue;
		if(S[i]=='-')T+='/';
		else if(S[i]==' ')T+='-';
		else T+=S[i];
	}
	t+="# [My Blogs](https://wronganswer90.github.io/"+T+")\n";
	t+="\n";
	
	
	fout<<string_to_UTF8(t);
	for(int i=0;i<len;++i)fout<<s[i]<<"\n";
}
int main()
{
	get();
	commit();
}
