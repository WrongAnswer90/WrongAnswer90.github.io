#include<bits/stdc++.h>
#include<windows.h>
#include<wchar.h>
using namespace std;
inline void commit()
{
	char s[210];
	time_t tmp;
	time(&tmp);
	struct tm *p;
	p=localtime(&tmp);
	sprintf(s,"git commit -m \"Updated on %d-%02d-%02d %02d:%02d\"",1900+p->tm_year,
	1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min);
	
	system("git add .");
	system(s);
	int z;
	again:
	z=system("git push -f origin main");
	if(z){Sleep(20000);goto again;}
}
int main()
{
	commit();
}
