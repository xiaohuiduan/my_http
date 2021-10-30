//#include <sys/socket.h>
//#include <sys/un.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
//
//#define PORT 8898                      // 服务器监听端口
//
//
//int main() {
//    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
//    struct sockaddr_in server_addr;
//    memset(&server_addr, 0, sizeof(server_addr));
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
//    server_addr.sin_port = htons(PORT);
//    int opt = 1;
//
//    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//
//    int suc = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
//    if (suc == -1) {
//        printf("绑定事变");
//    }
//    listen(server_socket, 500);
//
//    while (1) {
//        int client_socket = accept(server_socket, NULL, NULL);
//
//        char buf[1024];
//        read(client_socket, buf, 1024);
//
//        printf("%s", buf);
////
//        char status[] = "HTTP/1.0 200 OK\r\n";
//        char header[] = "Server: DWBServer\r\nContent-Type: text/html;charset=utf-8\r\n\r\n";
////        char body[] = "<html><head><title>你是来拉屎的吧</title></head><body><h2>欢迎</h2><p>你是来拉屎的吧你是来拉屎的吧</p></body></html>";
//        write(client_socket, status, sizeof(status));
//        write(client_socket, header, sizeof(header));
////        write(client_socket, body, sizeof(body));
//
//        //先检查文件是否存在
//        char *filename = "res/1.html";  //文件名
//        FILE *fp = fopen(filename, "rb");  //以二进制方式打开文件
//        if(fp == NULL){
//            printf("Cannot open file, press any key to exit!\n");
//            system("pause");
//            exit(0);
//        }
//
//        //循环发送数据，直到文件结尾
//        char buffer[1024] = {0};  //缓冲区
//        int nCount;
//        while( (nCount = fread(buffer, 1, 1024, fp)) > 0 ){
//            write(client_socket, buffer, nCount);
//        }
//
//
//        shutdown(server_socket,SHUT_WR); //阻塞，等待客户端接收完毕
////    close(server_socket);
//        close(client_socket);
//    }
//
//    return 0;
//}
//
//
