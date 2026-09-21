/*
  WiFiUdp.h - UDP implementation for Coral Dev Board Micro using lwIP sockets.
*/

#ifndef WIFIUDP_H
#define WIFIUDP_H

#include <Arduino.h>
#include <api/Udp.h>
#include <api/IPAddress.h>

#define UDP_TX_BUF_SIZE 1460
#define UDP_RX_BUF_SIZE 1460

class WiFiUDP : public UDP {

public:
  WiFiUDP();
  virtual ~WiFiUDP();

  // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
  virtual uint8_t begin(uint16_t port) override;

  // initialize, start listening on specified multicast IP address and port. Returns 1 if successful, 0 if there are no sockets available to use
  virtual uint8_t beginMulticast(IPAddress ip, uint16_t port);
  // Finish with the UDP socket
  virtual void stop() override;

  // Sending UDP packets

  // Start building up a packet to send to the remote host specific in ip and port
  // Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
  virtual int beginPacket(IPAddress ip, uint16_t port) override;
  // Start building up a packet to send to the remote host specific in host and port
  // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
  virtual int beginPacket(const char *host, uint16_t port) override;
  // Finish off this packet and send it
  // Returns 1 if the packet was sent successfully, 0 if there was an error
  virtual int endPacket() override;
  // Write a single byte into the packet
  virtual size_t write(uint8_t byte) override;
  // Write size bytes from buffer into the packet
  virtual size_t write(const uint8_t *buffer, size_t size) override;

  // Start processing the next available incoming packet
  // Returns the size of the packet in bytes, or 0 if no packets are available
  virtual int parsePacket() override;
  // Number of bytes remaining in the current packet
  virtual int available() override;
  // Read a single byte from the current packet
  virtual int read() override;
  // Read up to len characters from the current packet and place them into buffer
  // Returns the number of characters read, or 0 if none are available
  virtual int read(unsigned char *buffer, size_t len) override;
  // Read up to len characters from the current packet and place them into buffer
  // Returns the number of characters read, or 0 if none are available
  virtual int read(char *buffer, size_t len) override {
    return read((unsigned char *)buffer, len);
  }
  // Return the next byte from the current packet without moving on to the next byte
  virtual int peek() override;
  // Finish reading the current packet
  virtual void flush() override;

  // Return the IP address of the host who sent the current incoming packet
  virtual IPAddress remoteIP() override;
  // Return the port of the host who sent the current incoming packet
  virtual uint16_t remotePort() override;

private:
  int _sock; // socket ID for UDP
  uint16_t _port; // local port to listen on

  // Stored as 32-bit IPv4 network addresses and port numbers
  uint32_t _dest_ip;
  uint16_t _dest_port;
  uint32_t _remote_ip;
  uint16_t _remote_port;

  uint8_t _tx_buffer[UDP_TX_BUF_SIZE];
  size_t _tx_len;

  uint8_t _rx_buffer[UDP_RX_BUF_SIZE];
  int _rx_len;
  int _rx_offset;
};

#endif // WIFIUDP_H