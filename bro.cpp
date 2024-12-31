#include<iostream>
using namespace std;
// Amit [The Bro Programmer]
class Error
{
public: 
bool hasError()
{
return false;
}
string getError()
{
return "";
}
};
class Request
{
};
class Response
{
public:
void setContentType(string contentType)
{
// do nothing right now
}
Response & operator<<(string content)
{
// right now do nothing
 return *this;
}
};

class Bro
{
public:
void setStaticResourcesFolder(string staticResourcesFolder)
{
// do nothing 
}
void get(string urlPattern,void(*callBack)(Request &,Response &))
{
// do nothing
}
void listen(int portNumber,void(*callBack)(Error &))
{
// do nothing right now
}

};



// Bobby [The Web Application Developer - User of Bro Web Server]
int main()
{
Bro bro;
bro.setStaticResourcesFolder("Whatever");
bro.get("/",[](Response &response,Request &request) void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lan='en'>
<head>
<meta charset='utf-8'>
<title>Administrator</title>
</head>
<body>
<h1>Welcome</h1>
<h3>Some text</h3>
<a href='getCustomers'>Customers list<a>
</body>
</html>
)"""";
response.setContentType("text/html"); // Setting MIME Type
response<<html;
});

bro.get("/getCustomers",[](Request &request,Response &response)void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lan='en'>
<head>
<meta charset='utf-8'>
<title>Whatever</title>
</head>
<body>
<h1>List of Customers</h1>
<ul>
<li>Ramesh</li>
<li>Suresh</li>
<li>Mohan</li>
<ul>
</body>
</html>
)"""";
response.setContentType("text/html"); // Setting MIME Type
response<<html;
});

bro.listen(6060,[Error & error]()void{
if(error.hasError())
{
cout<<error.getError();
return;
}
cout<<"Bro HTTP Server is ready to accept request on port 6060"<<endl;
});
return 0;
}

