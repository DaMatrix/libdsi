#include <porklib.h>

#include <ifaddrs.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <unistd.h>

#include <sys/time.h>
#include <stdint.h>

using namespace porklib; //evil

struct NetInterface {
    const char* name;
    u32 flags;
    sockaddr_in* addr;
    sockaddr_in* netmask;
    union {
        sockaddr_in* bcastAddr;
        sockaddr_in* dstAddr;
    } ifu;
};

struct BcastSocket  {
    NetInterface* interface;
    int bcastSocket;
    sockaddr_in bcastAddr;
    int recvSocket;
    sockaddr_in recvAddr;
};

constexpr u16 DSILINK_PORT = 17491;

auto interfaces = new ArrayList<NetInterface*, lambda::noop, lambda::add<u_size, 1>>(0);

/*---------------------------------------------------------------------------------
	Subtract the `struct timeval' values Y from X,
	storing the result in RESULT.
	Return 1 if the difference is negative, otherwise 0.

	From http://www.gnu.org/software/libtool/manual/libc/Elapsed-Time.html
---------------------------------------------------------------------------------*/
int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y) {
    struct timeval tmp;
    tmp.tv_sec = y->tv_sec;
    tmp.tv_usec = y->tv_usec;

    if (x->tv_usec < tmp.tv_usec) {
        int nsec = (tmp.tv_usec - x->tv_usec) / 1000000 + 1;
        tmp.tv_usec -= 1000000 * nsec;
        tmp.tv_sec += nsec;
    }

    if (x->tv_usec - tmp.tv_usec > 1000000) {
        int nsec = (x->tv_usec - tmp.tv_usec) / 1000000;
        tmp.tv_usec += 1000000 * nsec;
        tmp.tv_sec -= nsec;
    }

    result->tv_sec = x->tv_sec - tmp.tv_sec;
    result->tv_usec = x->tv_usec - tmp.tv_usec;

    return x->tv_sec < tmp.tv_sec;
}

void timeval_add (struct timeval *result, struct timeval *x, struct timeval *y) {
    result->tv_sec = x->tv_sec + y->tv_sec;
    result->tv_usec = x->tv_usec + y->tv_usec;

    if ( result->tv_usec > 1000000) {
        result->tv_sec += result->tv_usec / 1000000;
        result->tv_usec = result->tv_usec % 1000000;
    }
}

int main(int argc, char* args[]) {
    { //get list of interfaces
        ifaddrs* ifaddr;
        if (getifaddrs(&ifaddr))  {
            perror("getifaddrs");
            return 1;
        }

        for (ifaddrs* curr = ifaddr; curr; curr = curr->ifa_next)   {
            if (!curr->ifa_addr || (curr->ifa_addr->sa_family != AF_INET && curr->ifa_addr->sa_family != AF_INET6)) continue;

            char host[NI_MAXHOST];

            if (getnameinfo(curr->ifa_addr, curr->ifa_addr->sa_family == AF_INET ? sizeof(sockaddr_in) : sizeof(sockaddr_in6), host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST))   {
                perror("getnameinfo");
                return 2;
            }

            printf(
                    "%-10s %-8s %c%s%c\n",
                    curr->ifa_name,
                    curr->ifa_addr->sa_family == AF_INET ? "AF_INET" : "AF_INET6",
                    curr->ifa_addr->sa_family == AF_INET ? '<' : '[',
                    host,
                    curr->ifa_addr->sa_family == AF_INET ? '>' : ']'
            );

            if (curr->ifa_addr->sa_family == AF_INET && curr->ifa_flags & IFF_BROADCAST) {
                auto interface = new NetInterface();
                interface->name = strCopy(curr->ifa_name);
                interface->flags = curr->ifa_flags;
                interface->addr = (sockaddr_in*) curr->ifa_addr;
                interface->netmask = (sockaddr_in*) curr->ifa_netmask;
                interface->ifu.bcastAddr = (sockaddr_in*) curr->ifa_ifu.ifu_broadaddr;
                interface->ifu.dstAddr = (sockaddr_in*) curr->ifa_ifu.ifu_dstaddr;
                interfaces->add(interface);
            }
        }
        freeifaddrs(ifaddr);
    }

    auto sockets = new ArrayList<BcastSocket*>(interfaces->size());
    interfaces->forEach([=](NetInterface* interface) {
        auto sock = new BcastSocket();
        sock->interface = interface;
        memset(&sock->bcastAddr, 0, sizeof(sockaddr_in));
        sock->bcastAddr.sin_family = AF_INET;
        sock->bcastAddr.sin_addr.s_addr = interface->ifu.bcastAddr->sin_addr.s_addr;
        sock->bcastAddr.sin_port = htons(DSILINK_PORT);
        memset(&sock->recvAddr, 0, sizeof(sockaddr_in));
        sock->recvAddr.sin_family = AF_INET;
        sock->recvAddr.sin_addr.s_addr = interface->addr->sin_addr.s_addr;
        sock->recvAddr.sin_port = htons(DSILINK_PORT);

        if ((sock->bcastSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) exit(3);
        if ((sock->recvSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) exit(3);

        int one = 1;
        setsockopt(sock->bcastSocket, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one));

        //setsockopt(sock->bcastSocket, SOL_SOCKET, SO_BINDTODEVICE, interface->name, strlen(interface->name));
        //setsockopt(sock->recvSocket, SOL_SOCKET, SO_BINDTODEVICE, interface->name, strlen(interface->name));

        if (bind(sock->recvSocket, (sockaddr*) &sock->recvAddr, sizeof(sock->recvAddr)) < 0) {
            perror("bind");
            exit(4);
        }

        sockets->add(sock);
        printf("Prepared %s...\n", sock->interface->name);
    });

    in_addr_t ds = INADDR_NONE;
    BcastSocket* dsSocket = nullptr;
    {
        timeval wanted, now, result;
        gettimeofday(&wanted, nullptr);
        int timeout = 10;
        do {
            gettimeofday(&now, nullptr);
            if (timeval_subtract(&result, &wanted, &now))   {
                //broadcast to each interface

                sockets->forEach([](BcastSocket* sock) {
                    if (sendto(sock->bcastSocket, "dsboot", 6, 0, (sockaddr*) &sock->bcastAddr, sizeof(sockaddr_in)) < 0) perror("sendto");
                });
                result.tv_sec = 0;
                result.tv_usec = 150000;
                timeval_add(&wanted, &now, &result);
                timeout--;
            }

            //try to receive from each interface
            sockets->forEach([&](BcastSocket* sock) {
                //printf("Checking %s...\n", sock->interface->name);

                int available;
                if (ioctl(sock->recvSocket, FIONREAD, &available) < 0)  {
                    perror("ioctl");
                    return;
                }

                //printf("%d bytes available.\n", available);
                if (available < 6) return;

                char rbuf[7] = {};
                socklen_t len = sizeof(sockaddr_in);
                if (recvfrom(sock->recvSocket, rbuf, 6, 0, (sockaddr*) &sock->recvAddr, &len) < 0) {
                    perror("recv");
                    return;
                }
                printf("Received: \"%s\"\n", rbuf);
                if (strncmp("bootds", rbuf, 6) == 0)   {
                    ds = sock->recvAddr.sin_addr.s_addr;
                    dsSocket = sock;
                }
            });
        } while (timeout && !dsSocket);
    }

    if (ds == INADDR_NONE)  {
        printf("No DS found!\n");
        return 1;
    } else {
        printf("DS found on %s! IP: %x\n", dsSocket->interface->name, ds);
    }
}
