#include <porklib.h>

#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace porklib; //evil

struct NetInterface {
    const char* name;
    u32 flags;
    sockaddr_in6* addr;
    sockaddr_in6* netmask;
    union {
        sockaddr_in6* bcastAddr;
        sockaddr_in6* dstAddr;
    } ifu;
};

struct BcastSocket  {
    NetInterface* interface;
    int fd;
    sockaddr_in6* bcastAddr;
    sockaddr_in6* recvAddr;
};

constexpr u16 DSILINK_PORT = 17491;

auto interfaces = new ArrayList<NetInterface*, lambda::noop, lambda::add<u_size, 1>>(0);

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

            if (curr->ifa_addr->sa_family == AF_INET6) {
                auto interface = new NetInterface();
                interface->name = strCopy(curr->ifa_name);
                interface->flags = curr->ifa_flags;
                interface->addr = (sockaddr_in6*) curr->ifa_addr;
                interface->netmask = (sockaddr_in6*) curr->ifa_netmask;
                interface->ifu.bcastAddr = (sockaddr_in6*) curr->ifa_ifu.ifu_broadaddr;
                interface->ifu.dstAddr = (sockaddr_in6*) curr->ifa_ifu.ifu_dstaddr;
                interfaces->add(interface);
            }
        }
        freeifaddrs(ifaddr);
    }

    auto sockets = new ArrayList<BcastSocket*>(interfaces->size());
    interfaces->forEach([=](NetInterface* interface) {
        auto sock = new BcastSocket();
        sock->interface = interface;
        //memset(&sock->bcastAddr, 0, sizeof(sockaddr_in6));
        //sock->bcastAddr->sin6_port = DSILINK_PORT;
        //memset(&sock->recvAddr, 0, sizeof(sockaddr_in6));
        //sock->recvAddr->sin6_port = DSILINK_PORT;

        if ((sock->fd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1) exit(3);

        int one = 1;
        setsockopt(sock->fd, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one));
        setsockopt(sock->fd, SOL_SOCKET, SO_BINDTODEVICE, interface->name, strlen(interface->name));

        sockets->add(sock);
    });

    sockets->forEach([](BcastSocket* sock) {
        printf("%s\n", sock->interface->name);

        if (!sock->interface->ifu.bcastAddr) {
            printf("Ignoring %s due to lack of broadcast address\n", sock->interface->name);
            return;
        }
        if (sendto(sock->fd, "dsboot", 6, 0, (sockaddr*) sock->interface->ifu.bcastAddr, sizeof(sockaddr_in6)) < 0) perror("sendto");
    });
}
