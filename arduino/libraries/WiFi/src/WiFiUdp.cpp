/*
  WiFiUdp.cpp - UDP implementation for Coral Dev Board Micro using lwIP sockets.
*/

#include "WiFiUdp.h"
#include <string.h>

// Include lwIP socket headers
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/inet.h"

// Undefine lwIP macros that collide with Arduino Stream/Print methods
#undef write
#undef read
#undef close
#undef bind
#undef connect
#undef listen
#undef accept
#undef select

WiFiUDP::WiFiUDP()
    : _sock(-1), _port(0), _dest_ip(0), _dest_port(0),
      _remote_ip(0), _remote_port(0), _tx_len(0), _rx_len(0), _rx_offset(0) {}

WiFiUDP::~WiFiUDP() {
  stop();
}

/* Start listening for incoming UDP packets on local port */
uint8_t WiFiUDP::begin(uint16_t port) {
  if (_sock >= 0) {
    stop();
  }

  _sock = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (_sock < 0) {
    return 0;
  }

  // Set non-blocking mode so parsePacket() does not hang
  int flags = lwip_fcntl(_sock, F_GETFL, 0);
  lwip_fcntl(_sock, F_SETFL, flags | O_NONBLOCK);

  // Enable address reuse
  int reuse = 1;
  lwip_setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  // Bind to INADDR_ANY and target port
  struct sockaddr_in local_addr;
  memset(&local_addr, 0, sizeof(local_addr));
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(port);
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (lwip_bind(_sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
    stop();
    return 0;
  }

  _port = port;
  _rx_len = 0;
  _rx_offset = 0;
  return 1;
}

/* Join a multicast group */
uint8_t WiFiUDP::beginMulticast(IPAddress ip, uint16_t port) {
#if defined(LWIP_IGMP) && LWIP_IGMP
  if (!begin(port)) {
    return 0;
  }

  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = static_cast<uint32_t>(ip);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);

  if (lwip_setsockopt(_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
    stop();
    return 0;
  }

  return 1;
#else
  // IGMP / Multicast is not enabled in Coral Micro's default SDK build
  (void)ip;
  (void)port;
  return 0;
#endif
}

/* Close the socket and clear state */
void WiFiUDP::stop() {
  if (_sock >= 0) {
    lwip_close(_sock);
    _sock = -1;
  }
  _tx_len = 0;
  _rx_len = 0;
  _rx_offset = 0;
}

/* Prepare to send packet to DNS host name */
int WiFiUDP::beginPacket(const char *host, uint16_t port) {
  struct hostent *hp = lwip_gethostbyname(host);
  if (!hp || !hp->h_addr_list[0]) {
    return 0;
  }

  struct in_addr *addr = (struct in_addr *)hp->h_addr_list[0];
  IPAddress ip(addr->s_addr);
  return beginPacket(ip, port);
}

/* Prepare to send packet to IP address */
int WiFiUDP::beginPacket(IPAddress ip, uint16_t port) {
  if (_sock < 0) {
    _sock = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (_sock < 0) {
      return 0;
    }
    int flags = lwip_fcntl(_sock, F_GETFL, 0);
    lwip_fcntl(_sock, F_SETFL, flags | O_NONBLOCK);
  }

  _dest_ip = static_cast<uint32_t>(ip);
  _dest_port = port;
  _tx_len = 0;
  return 1;
}

/* Transmit the packet */
int WiFiUDP::endPacket() {
  if (_sock < 0 || _tx_len == 0) {
    return 0;
  }

  struct sockaddr_in dest_addr;
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(_dest_port);
  dest_addr.sin_addr.s_addr = _dest_ip;

  int sent = lwip_sendto(_sock, _tx_buffer, _tx_len, 0,
                         (struct sockaddr *)&dest_addr, sizeof(dest_addr));

  _tx_len = 0;
  return (sent > 0) ? 1 : 0;
}

size_t WiFiUDP::write(uint8_t byte) {
  return write(&byte, 1);
}

size_t WiFiUDP::write(const uint8_t *buffer, size_t size) {
  if (_tx_len + size > UDP_TX_BUF_SIZE) {
    size = UDP_TX_BUF_SIZE - _tx_len;
  }
  if (size == 0) {
    return 0;
  }

  memcpy(_tx_buffer + _tx_len, buffer, size);
  _tx_len += size;
  return size;
}

/* Receive incoming packet */
int WiFiUDP::parsePacket() {
  _rx_len = 0;
  _rx_offset = 0;

  if (_sock < 0) {
    return 0;
  }

  struct sockaddr_in from_addr;
  socklen_t addr_len = sizeof(from_addr);
  memset(&from_addr, 0, sizeof(from_addr));

  int len = lwip_recvfrom(_sock, _rx_buffer, UDP_RX_BUF_SIZE, 0,
                          (struct sockaddr *)&from_addr, &addr_len);

  if (len > 0) {
    _rx_len = len;
    _remote_ip = from_addr.sin_addr.s_addr;
    _remote_port = ntohs(from_addr.sin_port);
    return _rx_len;
  }

  return 0;
}

int WiFiUDP::available() {
  return (_rx_len - _rx_offset);
}

int WiFiUDP::read() {
  if (available() <= 0) {
    return -1;
  }
  return _rx_buffer[_rx_offset++];
}

int WiFiUDP::read(unsigned char *buffer, size_t len) {
  int avail = available();
  if (avail <= 0) {
    return -1;
  }

  if ((size_t)avail < len) {
    len = avail;
  }

  memcpy(buffer, &_rx_buffer[_rx_offset], len);
  _rx_offset += len;
  return len;
}

int WiFiUDP::peek() {
  if (available() <= 0) {
    return -1;
  }
  return _rx_buffer[_rx_offset];
}

void WiFiUDP::flush() {
  // Transmitted immediately on lwip_sendto
}

IPAddress WiFiUDP::remoteIP() {
  return IPAddress(_remote_ip);
}

uint16_t WiFiUDP::remotePort() {
  return _remote_port;
}