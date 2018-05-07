#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#include <iostream>
#include <regex>
#include <cstring>
#include <string>
#include <sstream>


using namespace std;

#define MYPORT 80
#define BUFFER_SIZE 1024

size_t readline(int fd, void *vptr, size_t maxlen)  
{  
    size_t n, rc;  
    char c, *ptr;  
    ptr = (char*)vptr;  
    for(n = 1 ;n < maxlen; n++)  
    {  
        if( (rc = read(fd, &c, 1)) == 1)  
        {  
            *ptr++ = c;  
            if(c == '\n')  
                break;  
        }else if( rc == 0) {  
            *ptr = 0;  
            return (n-1);  
        }else{  
            return -1;  
        }  
    }  
    *ptr = 0;  
    return n;  
}

string send_post(string hostname, string api, string data)
{
    // 基础数据初始化
    string host(hostname);
    string path(api);
    string post_content = data;
    int content_length = post_content.size();

    // 生成http post请求包
    stringstream stream;    
    stream << "POST " << path << " HTTP/1.1\r\n"
        <<"Host: " << hostname << "\r\n"
        <<"Connection: close\r\n"
        <<"User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36\r\n"
        <<"Cache-Control: no-cache\r\n"
        <<"Content-Type: text/xml\r\n"
        <<"Accept: */*\r\n"
        <<"Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-US;q=0.7,zh-TW;q=0.6\r\n"
        <<"Accept-Encoding: gzip, deflate\r\n"
        <<"Content-Length:" << content_length << "\r\n"
        << "\r\n"
        << post_content.c_str();
    string request = stream.str();

    // socket初始化
    struct sockaddr_in servaddr;  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(MYPORT);  // 服务器端口 

    struct hostent* host_addr = gethostbyname(hostname.c_str());
    if(host_addr == NULL)
    {
        cout << "Unable to locate host" << endl;
        return "Unable to locate host";
    }
    servaddr.sin_addr.s_addr = *((int*)*host_addr->h_addr_list);  // 服务器ip
    
    int len = sizeof(servaddr);
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);

    // 建立连接
    if( connect(sock_cli,(struct sockaddr *)&servaddr,len) < 0)
    {  
        perror("connect");  
        exit(1);  
    }
    printf("connect sucess!\n");

    send(sock_cli, request.c_str(), request.size(), 0);
    char buf[BUFFER_SIZE*BUFFER_SIZE];
    memset(buf, 0, BUFFER_SIZE*BUFFER_SIZE);
    int offset = 0;
    int rc;

    while((rc = readline(sock_cli, buf+offset, BUFFER_SIZE)) > 0 ){
        offset += rc;
    }
    buf[offset] = 0;

    //cout<<strlen(buf)<<endl;
    //cout<<buf<<endl;

    close(sock_cli);

    return buf;
}

string xml_data_init(string sys_name, string key_type)
{
    string req_xml = "";
    req_xml += "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">";
    req_xml += "<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"";
    req_xml += " xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">";
    req_xml += "<KeyRequest xmlns=\"http://www.yourdomain.com\">";
    req_xml += "<systemName>" + sys_name + "</systemName>";
    req_xml += "<keyType>" + key_type + "</keyType>";
    req_xml += "</KeyRequest></s:Body></s:Envelope>\r\n";   
    return req_xml;
}

string extract_key_from_str(string inputstr)
{	
	// fuck the c++ regex!!
    string substr = inputstr.substr(inputstr.find("<ns2:keyCode>")+13, 65);
    return substr;
}

int main()
{
    string hostname = "kms.yourdomain.com";
    string api = "/yourAPI/WebService";
    string data = xml_data_init("arg1", "arg2");

    string response = send_post(hostname, api, data);
    // cout << response << endl;
    string key = extract_key_from_str(response);
    cout << key << endl;
}
