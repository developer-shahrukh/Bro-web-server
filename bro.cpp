#include<iostream>
#include<map>
#include<forward_list>
#include<string.h>
#include<windows.h>
#include<unistd.h>
using namespace std;
// Amit [The Bro Programmer]
class Validator
{
private:
Validator(){}
public:
static bool isValidMIMEType(string &mimeType)
{
// do nothing right now
return true;
}
static bool isValidPath(string &path)
{
// do nothing right now
return true;
}
static isValidURLFormate(string &url)
{
// do nothing right now
return true;
}
};

class Error
{
private:
string error;
public: 
Error(string error)
{
this->error=error;
}
bool hasError()
{
return this->error.length()>0;
}
string getError()
{
return this->error;
}
};
class Request
{
};
class Response
{
private:
string contentType;
forward_list<string> content;
forward_list<string>::iterator contentIterator;
unsigned long contentLength;
public:
Response()
{
this->contentLength=0;
this->contentIterator=this->content.before_begin();
}
~Response()
{
// Not yet implement
}
void setContentType(string contentType)
{
if(Validator::isValidMIMEType(contentType))
{
this->contentType=contentType;
}
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
private:
string staticResourcesFolder;
map<string,void (*)(Request &,Response &)> urlMappings;
public:
Bro()
{}
~Bro()
{}
void setStaticResourcesFolder(std::string staticResourcesFolder)
{
if(Validator::isValidPath(staticResourcesFolder))
{
this->staticResourcesFolder=staticResourcesFolder;
}
else
{
// not yet decided
}
// do nothing 
}
void get(std::string url,void(*callBack)(Request &,Response &))
{
if(Validator::isValidURLFormate(url))
{
urlMappings.insert (pair<string, void(*)(Request &,Response &)>(url,callBack));
}
// do nothing
}
void listen(int portNumber,void(*callBack)(Error &))
{
WSADATA wsaData;
WORD ver;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
int serverSocketDescriptor;
char requestBuffer[4096];
int requestLength;
int x;
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(serverSocketDescriptor<0)
{
WSACleanup();
Error error("Unable to create Socket");
callBack(error);
return;
}
struct sockaddr_in serverSocketInformation;
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
int successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
close(serverSocketDescriptor);
WSACleanup();
char a[101];
sprintf(a,"Unable to bind socket to port : %d",portNumber);
Error error(a);
callBack(error);
return;
}
successCode=::listen(serverSocketDescriptor,10);
if(successCode<0)
{
close(serverSocketDescriptor);
WSACleanup();
Error error("Unable to accept client connections");
callBack(error);
return;
}
Error error("");
callBack(error);
struct sockaddr_in clientSocketInformation;
int len=sizeof(clientSocketInformation);
int clientSocketDescriptor;
while(1)
{
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
// not yet decided, will write this code later on
}
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer),0);
if(requestLength>0)
{
for(x=0;x<requestLength;x++) printf("%c",requestBuffer[x]);
const char *response=
"HTTP/1.1 200 ok\r\n"
"Connection: close\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 130\r\n\r\n"
"<html><head><title>Shahrukh Mansuri</title></head>"
"<body><h1>Shahrukh Mansuri<h1>"
"<h3>We creating a web server in c++</h3></body></html>";
send(clientSocketDescriptor,response,strlen(response),0);
}
//lot of code will be written here later on
} //infinite loop ends here

}

};



// Bobby [The Web Application Developer - User of Bro Web Server]
int main()
{
Bro bro;
bro.setStaticResourcesFolder("Whatever");
bro.get("/",[](Request &request,Response &response) void{
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

bro.get("/getCustomers",[](Request &request,Response &response) void{
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
</ul>
</body>
</html>
)"""";
response.setContentType("text/html"); // Setting MIME Type
response<<html;
});

bro.listen(6060,[](Error & error)void{
if(error.hasError())
{
cout<<error.getError();
return;
}
cout<<"Bro HTTP Server is ready to accept request on port 6060"<<endl;
});
return 0;
}

