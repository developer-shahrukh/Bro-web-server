#include<map>
#include<iostream>
using namespace std;
void createDataMap(char *str,map<string,string> &dataMap)
{
char *ptr1,*ptr2;
ptr1=str;
ptr2=str;
while(1)
{
while(*ptr2!='\0' && *ptr2!='=') ptr2++;
if(*ptr2=='\0') return;
*ptr2='\0';
string key=string(ptr1);
ptr1=ptr2+1;
ptr2=ptr1;
while(*ptr2!='\0' && *ptr2!='&') ptr2++;
if(*ptr2=='\0')
{
dataMap.insert(pair<string,string>(key,string(ptr1)));
break;
}
else
{
*ptr2='\0';
dataMap.insert(pair<string,string>(key,string(ptr1)));
ptr1=ptr2+1;
ptr2=ptr1;
}
} // infinite loop ends here 
}


int main()
{
char dataInRequest[]={"nm=Prafull&ct=ujjain"};
map<string,string> dataMap;
createDataMap(dataInRequest,dataMap);
auto iterator=dataMap.begin();
while(iterator!=dataMap.end())
{
cout<<iterator->first<<","<<iterator->second<<endl;
++iterator;
}
return 0;
}