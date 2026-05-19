#ifndef HEADERS_FUNCTIONS_OVERLAY00_H_
#define HEADERS_FUNCTIONS_OVERLAY00_H_

void SelectRandomBackground(void);
void Socl_Startup(void);
int close(int fd);
int Soc_Socket(int domain, int family, int protocol);
int Soc_Bind(int sockfd, const struct sockaddr_in* addr);
int Soc_Connect(int sockfd, const struct sockaddr_in* addr);
int Soc_Recv(int sockfd, void* buf, size_t size, int flags);
int Soc_RecvFrom(int sockfd, void* buf, size_t size, int flags, struct sockaddr_in* addr,
                 int addr_len);
int Soc_Send(int sockfd, const void* buf, size_t size, int flags);
int Soc_SendTo(int sockfd, const void* buf, size_t size, int flags, const struct sockaddr_in* addr,
               int addr_len);
int Soc_Close(int fd);
int Soc_Fcntl(int fd, int op, uint32_t op_arg);
void Soc_Startup(void);
int Dwc_AC_Process(void);
int SocketCastError(int error, int cast_error);
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
int DoRand(uint32_t ctx);
int rand(void);
void srand(uint32_t seed);
int RandRangeOverlay0(int x, int y);
void ResolveAvailableNintendoWifi(const char* identifier);
void PasswordEncryptString(const char* src, char* dst);

#endif
