#ifndef HEADERS_FUNCTIONS_OVERLAY00_H_
#define HEADERS_FUNCTIONS_OVERLAY00_H_

void SelectRandomBackground(void);
int close(int fd);
int socket(int domain, int family, int protocol);
int bind(int sockfd, const struct sockaddr_in* addr);
int connect(int sockfd, const struct sockaddr_in* addr);
int recv(int sockfd, void* buf, size_t size, int flags);
int recvfrom(int sockfd, void* buf, size_t size, int flags, struct sockaddr_in* addr, int addr_len);
int send(int sockfd, const void* buf, size_t size, int flags);
int sendto(int sockfd, const void* buf, size_t size, int flags, const struct sockaddr_in* addr,
           int addr_len);
int CloseVeneer(int fd);
int fcntl(int fd, int op, uint32_t op_arg);
int InitWfc();
int SocketCastError(int error, int casted_error);
int SocketCreate(int domain, int family, int protocol);
int SocketClose(int fd);
int SocketBind(int sockfd, const struct sockaddr_in* addr, int addr_len);
int SocketConnect(int sockfd, const struct sockaddr_in* addr, int addr_len);
int SocketRecv(int sockfd, void* buf, size_t size, int flags);
int SocketRecvFrom(int sockfd, void* buf, size_t size, int flags, struct sockaddr_in* addr,
                   int addr_len);
int SocketSend(int sockfd, const void* buf, size_t size, int flags);
int SocketSendTo(int sockfd, const void* buf, size_t size, int flags,
                 const struct sockaddr_in* addr, int addr_len);
bool SocketSetBlocking(int sockfd, bool blocking);
int do_rand(uint32_t ctx);
int rand(void);
void srand(uint32_t seed);
int randrange(int x, int y);
void ResolveAvailableNintendoWifi(const char* identifier);
void PasswordEncryptString(const char* src, char* dst);

#endif
