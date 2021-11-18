//
// Created by xiaohui on 2021/11/6.
//

#ifndef MY_HTTP_CONFIG_H
#define MY_HTTP_CONFIG_H

#define DO_SSL 0 // 1代表使用SLL 0代表不使用
#define RES_DIR "/home/xiaohui/homework/network/my_http/res/"
#define CA_PATH "/home/xiaohui/homework/network/my_http/res/cacert.pem"
#define PK_PATH "/home/xiaohui/homework/network/my_http/res/privkey.pem"
#define MAX_BUF_SIZE 1024
#define CHUNK 1 //1代表开启分块传输 0代表不开启
#define CHUNK_SIZE 2 // 分块传输大小

#define PORT 8899 // 服务器监听端口

#endif //MY_HTTP_CONFIG_H
