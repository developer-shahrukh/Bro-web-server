#include<map>
using namespace std;
int main()
{
map<string, int> m;
pair<string, int> p1("Good",10);
m.insert(p1);
m.insert(pair<string,int>("Some",20));
return 0;
}