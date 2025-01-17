#include<iostream>
#include<map>
#include<forward_list>
#include<string.h>
#include<unistd.h>
#ifdef _WIN32
#include<windows.h>
#endif
#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
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

enum __request_method__{__GET__,__POST__,__PUT__,__DELETE__,__CONNECT__,__TRACE__,__HEAD__,__OPETIONS__};
typedef struct __url__mapping
{
__request_method__ requestMethod;
void (*mappedFunction)(Request &,Response &);
}URLMapping;
class Bro
{
private:
string staticResourcesFolder;
map<string,URLMapping> urlMappings;
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
urlMappings.insert (pair<string,URLMapping>(url,{__GET__,callBack}));
}
// do nothing
}
void listen(int portNumber,void(*callBack)(Error &))
{
#ifdef _WIN32
WSADATA wsaData;
WORD ver;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
#endif
int serverSocketDescriptor;
char requestBuffer[4097];
int requestLength;
int x;
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(serverSocketDescriptor<0)
{
#ifdef _WIN32
WSACleanup();
#endif
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
#ifdef _WIN32
WSACleanup();
#endif
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
#ifdef _WIN32
WSACleanup();
#endif
Error error("Unable to accept client connections");
callBack(error);
return;
}
Error error("");
callBack(error);
struct sockaddr_in clientSocketInformation;
#ifdef linux
socklen_t len=sizeof(clientSocketInformation);
#endif
#ifdef _WIN32
int len=sizeof(clientSocketInformation);
#endif
int clientSocketDescriptor;
while(1)
{
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
if(requestLength==0 || requestLength==-1)
{
close(clientSocketDescriptor);
continue;
}
int i;
char *method,*requestURI,*httpVersion;
requestBuffer[requestLength]='\0';
// code to parse the first line of the http request starts here
// first line should be REQUEST_METHOD SPACE URI SPACE HTTPVersionCRLF
method=requestBuffer;
i=0;
while(requestBuffer[i] && requestBuffer[i]!=' ') i++;



close(clientSocketDescriptor);
//lot of code will be written here later on
} //infinite loop ends here
#ifdef __WIN32
WSACleanup();
#endif
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

