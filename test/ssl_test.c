// #include <stdio.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <openssl/ssl.h>
// #include <openssl/err.h>
// #include <memory.h>
//
// #define MAXBUF 2048
//
// struct Mypara {
//     int *p_clifd;
//     struct sockaddr_in *cliaddr;//参数1
//     SSL_CTX *ctx;//参数2
// };
//
// void *process_http(struct Mypara *mypara) {
//     int *p_clifd = mypara->p_clifd;
//     struct sockaddr_in *cliaddr = mypara->cliaddr;
//     SSL_CTX *ctx = mypara->ctx;
//
//     int clifd = *p_clifd;
//     SSL *ssl;
//     char buff[MAXBUF + 1];
//     int rc;
//     struct sockaddr_in svraddr;
//
//     if (clifd < 0) {
//         fprintf(stderr, "listen socket failed, %s.\n", strerror(errno));
//         return;
//     }
//     fprintf(stdout, "got connection #%d from [%s:%d]\n", clifd,
//             inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port));
//
//     ssl = SSL_new(ctx);
//     if (ssl == NULL) {
//         fprintf(stderr, "create SSL context failed.\n");
//         close(clifd);
//         return;
//     }
//
//     SSL_set_fd(ssl, clifd);
//     SSL_accept(ssl);
//     // 建立连接
//     // int suc_connect = SSL_connect(ssl);
//     // if(suc_connect <= 0){
//     //     printf("ssl connect error\n");
//     //     close(clifd);
//     //     exit(1);
//     // }
//
//     printf(">>>>\n");
//     rc = SSL_read(ssl, buff, sizeof(buff)); /* for '\0' */
//     if (rc < 0) {
//         fprintf(stderr, "read from SSL failed.\n");
//         close(clifd);
//         return;
//     }
//
//     buff[rc] = 0;
//     // fprintf(stdout, "got data(%2d): %s\n", rc, buff);
//     char status[] = "HTTP/1.0 200 OK\r\n";
//     char header[] = "Server: DWBServer\r\nContent-Type: text/html;charset=utf-8\r\n\r\n";
//     char body[] = "<html><head><title>C语言构建小型Web服务器</title></head><body><h2>欢迎</h2><p>Hello，World</p></body></html>";
//
//     // printf("%d\n", rc);
//     // printf("%s\n", buff);
//     SSL_write(ssl, status, sizeof(status));
//     SSL_write(ssl, header, sizeof(header));
//     SSL_write(ssl, body, sizeof(body));
//
//     // SSL_shutdown(ssl);
//     // SSL_free(ssl);
//     close(clifd);
// }
//
// int main(void) {
//
//     SSL_CTX *ctx;
//     socklen_t len;
//
//     int sockfd, clifd;
//     struct sockaddr_in svraddr, cliaddr;
//
//     SSL_library_init();
//     //OpenSSL_add_all_algorithms();
//     SSL_load_error_strings();
//
//     ctx = SSL_CTX_new(SSLv23_server_method()); /* support both V2 and V3 */
//     if (ctx == NULL) {
//         ERR_print_errors_fp(stderr);
//         exit(1);
//     }
//
//     /* load certificate file, which will send public key to client. */
//     if (SSL_CTX_use_certificate_file(ctx, "/home/xiaohui/homework/network/my_http/test/cacert.pem", SSL_FILETYPE_PEM) <= 0) {
//         ERR_print_errors_fp(stderr);
//         exit(1);
//     }
//     /* and also private key file */
//     if (SSL_CTX_use_PrivateKey_file(ctx, "/home/xiaohui/homework/network/my_http/test/privkey.pem", SSL_FILETYPE_PEM) <= 0) {
//         ERR_print_errors_fp(stderr);
//         exit(1);
//     }
//
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     struct sockaddr_in server_addr;
//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
//     server_addr.sin_port = htons(8899);
//     int opt = 1;
//     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//     int suc = bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
//
//     if (suc == -1) {
//         printf("绑定事变");
//     }
//     listen(sockfd, 500);
//
//     if (listen(sockfd, 128) < 0) {
//         fprintf(stderr, "listen socket failed, %s.\n", strerror(errno));
//         close(sockfd);
//         exit(1);
//     }
//
//     len = sizeof(struct sockaddr);
//     while (1) {
//         clifd = accept(sockfd, (struct sockaddr *) &cliaddr, &len);
//
//         pthread_t *new_thread;
//         new_thread = (pthread_t *) malloc(sizeof(*new_thread));
//         struct Mypara mypara;
//         memset(&mypara, 0, sizeof(struct Mypara));
//         mypara.p_clifd = &clifd;
//         mypara.cliaddr = &cliaddr;
//         mypara.ctx = ctx;
//         pthread_create(new_thread, NULL, process_http, &mypara);
//
//     }
//
//     // SSL_CTX_free(ctx);
//     return 0;
// }