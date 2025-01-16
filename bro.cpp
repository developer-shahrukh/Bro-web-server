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
if(clientSocketDescriptor<0)
{
// not yet decided, will write this code later on
}
forward_list<string> requestBufferDS;
forward_list<string>::iterator requestBufferDSIterator;
requestBufferDSIterator=requestBufferDS.before_begin();
int requestBufferDSSize=0;
int requestDataCount=0;
while(1)
{
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
if(requestLength==0) break;
requestBuffer[requestLength]='\0';
requestBufferDSIterator=requestBufferDS.insert_after(requestBufferDSIterator,string(requestBuffer));
requestBufferDSSize++;
requestDataCount+=requestLength;
}
if(requestBufferDSSize>0)
{
char *requestData=new char[requestDataCount+1];
char *p;
p=requestData;
const char *q;
requestBufferDSIterator=requestBufferDS.begin();
while(requestBufferDSIterator!=requestBufferDS.end())
{
q=(*requestBufferDSIterator).c_str();
while(*q)
{
*p=*q;
p++;
q++;
}
++requestBufferDSIterator;
}
*p='\0';
requestBufferDS.clear();
printf("------------- request data begin -------------\n");
printf("%s\n",requestData);
printf("------------ request data end ----------------\n");
// the code to parse the request goes here
delete [] requestData;
}
else
{
// something  if no data was received
}
close(clientSocketDescriptor);


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

