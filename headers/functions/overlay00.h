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
int CheckRCode_Gsi(int error, int cast_error);
int socket_gsi(int domain, int family, int protocol);
int closesocket_gsi(int fd);
int bind_gsi(int sockfd, const struct sockaddr_in* addr, int addr_len);
int connect_gsi(int sockfd, const struct sockaddr_in* addr, int addr_len);
int recv_gsi(int sockfd, void* buf, size_t size, int flags);
int recvfrom_gsi(int sockfd, void* buf, size_t size, int flags, struct sockaddr_in* addr,
                   int addr_len);
int send_gsi(int sockfd, const void* buf, size_t size, int flags);
int sendto_gsi(int sockfd, const void* buf, size_t size, int flags,
                 const struct sockaddr_in* addr, int addr_len);
bool SetSockBlocking(int sockfd, bool blocking);
int nextlongrand_gsi(uint32_t ctx);
int longrand_gsi(void);
void Util_RandSeed(uint32_t seed);
int Util_RandInt(int x, int y);
void SendPacket_Gsi(const char* identifier);
void GpiEncodeString(const char* src, char* dst);

#endif
