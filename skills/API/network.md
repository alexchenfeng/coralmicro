Network APIs
============

APIs to perform various networking tasks, such as open server sockets on the
Dev Board Micro, connect to other sockets as a client, create HTTP servers,
connect to Wi-Fi networks, and more.

Note

If using SSL (with Curl or Mbed TLS), you must first call
[`A71ChInit()`](../crypto/#_CPPv4N10coralmicro9A71ChInitEv "coralmicro::A71ChInit") to initialize the a71ch crypto chip.

TCP/IP sockets
--------------

APIs to create TCP/IP network socket servers and client connections with
the Dev Board Micro.

All socket communication is facilitated by read/write functions that require
a socket file descriptor, which you can get by creating a new server with
[`SocketServer()`](#_CPPv4N10coralmicro12SocketServerEii "coralmicro::SocketServer"), accepting an incoming client connection
with [`SocketAccept()`](#_CPPv4N10coralmicro12SocketAcceptEi "coralmicro::SocketAccept"), or initiating a client connection
with [`SocketClient()`](#_CPPv4N10coralmicro12SocketClientE9ip_addr_ti "coralmicro::SocketClient").

For an example, see `examples/audio_streaming/`.

[[network.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/network.h)

*namespace* `coralmicro`
:   Enums

    *enum class* `IOStatus`[Â¶](#_CPPv4N10coralmicro8IOStatusE "Permalink to this definition")
    :   Socket I/O status response codes.

        *Values:*

        *enumerator* `kOk`[Â¶](#_CPPv4N10coralmicro8IOStatus3kOkE "Permalink to this definition")
        :   Operation succeeded.

        *enumerator* `kEof`[Â¶](#_CPPv4N10coralmicro8IOStatus4kEofE "Permalink to this definition")
        :   Reached end-of-file during read operation.

        *enumerator* `kError`[Â¶](#_CPPv4N10coralmicro8IOStatus6kErrorE "Permalink to this definition")
        :   An error occurred during operation.

    Functions

    [IOStatus](#_CPPv4N10coralmicro8IOStatusE "coralmicro::IOStatus") `ReadBytes`(int *fd*, void \**bytes*, size\_t *size*)[Â¶](#_CPPv4N10coralmicro9ReadBytesEiPv6size_t "Permalink to this definition")
    :   Reads data from a socket file descriptor to a buffer.

        Parameters
        :   * **fd** â The file descriptor to read from.
            * **bytes** â The buffer to output the data.
            * **size** â The number of bytes to read.

        Returns
        :   The status result of the operation.

    template<typename `T`> [IOStatus](#_CPPv4N10coralmicro8IOStatusE "coralmicro::IOStatus") `ReadArray`(int *fd*, [T](#_CPPv4I0EN10coralmicro9ReadArrayE8IOStatusiP1T6size_t "coralmicro::ReadArray::T") \**array*, size\_t *array\_size*)[Â¶](#_CPPv4I0EN10coralmicro9ReadArrayE8IOStatusiP1T6size_t "Permalink to this definition")
    :   Read an array of data from a socket file descriptor.

        Parameters
        :   * **fd** â The file descriptor to read from.
            * **array** â The array to output the data to.
            * **array\_size** â The size of the array.

        Template Parameters
        :   **T** â The data type of the array.

        Returns
        :   The status result of the operation.

    [IOStatus](#_CPPv4N10coralmicro8IOStatusE "coralmicro::IOStatus") `WriteBytes`(int *fd*, *const* void \**bytes*, size\_t *size*, size\_t *chunk\_size* = 1024)[Â¶](#_CPPv4N10coralmicro10WriteBytesEiPKv6size_t6size_t "Permalink to this definition")
    :   Writes data from a buffer into a socket file descriptor.

        Parameters
        :   * **fd** â The file descriptor to write to.
            * **bytes** â The buffer of data to write.
            * **size** â The size of the buffer.
            * **chunk\_size** â The size of the chunk to write.

        Returns
        :   The status result of the operation.

    template<typename `T`> [IOStatus](#_CPPv4N10coralmicro8IOStatusE "coralmicro::IOStatus") `WriteArray`(int *fd*, *const* [T](#_CPPv4I0EN10coralmicro10WriteArrayE8IOStatusiPK1T6size_t "coralmicro::WriteArray::T") \**array*, size\_t *array\_size*)[Â¶](#_CPPv4I0EN10coralmicro10WriteArrayE8IOStatusiPK1T6size_t "Permalink to this definition")
    :   Writes data from an array into a socket file descriptor.

        Parameters
        :   * **fd** â The file descriptor to write to.
            * **array** â The array of data to write.
            * **size** â The size of the array.
            * **chunk\_size** â The size of the chunk to write.

        Template Parameters
        :   **T** â The data type of the array.

        Returns
        :   The status result of the operation.

    [IOStatus](#_CPPv4N10coralmicro8IOStatusE "coralmicro::IOStatus") `WriteMessage`(int *fd*, uint8\_t *type*, *const* void \**bytes*, size\_t *size*, size\_t *chunk\_size* = 1024)[Â¶](#_CPPv4N10coralmicro12WriteMessageEi7uint8_tPKv6size_t6size_t "Permalink to this definition")
    :   Writes a `message` with custom type from a buffer into a socket file descriptor.

        The `message` is going to have a 40 bits prefix that contains 32 bits for the size and 8 bits for the custom message type, following by the bytes.

        Parameters
        :   * **fd** â The file descriptor to write to.
            * **type** â The type of the message. This parameter can be used to create custom message type.
            * **bytes** â The buffer of data to write.
            * **size** â The size of the buffer.
            * **chunk\_size** â The size of the chunk to write.

        Returns
        :   The status result of the operation.

    bool `SocketHasPendingInput`(int *sockfd*)[Â¶](#_CPPv4N10coralmicro21SocketHasPendingInputEi "Permalink to this definition")
    :   Checks whether a socket file descriptor still has some bytes to read.

        Parameters
        :   **sockfd** â The socket file descriptor to check.

        Returns
        :   True if there are bytes remaining to read; false otherwise.

    int `SocketServer`(int *port*, int *backlog*)[Â¶](#_CPPv4N10coralmicro12SocketServerEii "Permalink to this definition")
    :   Starts a new TCP socket server.

        Parameters
        :   * **port** â The port to listen on.
            * **backlog** â The maximum length to which the queue of pending connections for sockfd may grow

        Returns
        :   The serverâs socket file descriptor.

    int `SocketAccept`(int *sockfd*)[Â¶](#_CPPv4N10coralmicro12SocketAcceptEi "Permalink to this definition")
    :   Accepts a connection request from a listening socket.

        Parameters
        :   **sockfd** â The listening socket file descriptor.

        Returns
        :   The clientâs socket file descriptor.

    void `SocketClose`(int *sockfd*)[Â¶](#_CPPv4N10coralmicro11SocketCloseEi "Permalink to this definition")
    :   Close a socket.

        Parameters
        :   **sockfd** â The socket file descriptor to close.

    int `SocketClient`(ip\_addr\_t *ip*, int *port*)[Â¶](#_CPPv4N10coralmicro12SocketClientE9ip_addr_ti "Permalink to this definition")
    :   Starts a client-side connection with a server.

        Parameters
        :   * **ip** â The serverâs ip address.
            * **port** â The serverâs port number.

        Returns
        :   The serverâs socket file descriptor.

    int `SocketClient`(*const* char \**host*, int *port*)[Â¶](#_CPPv4N10coralmicro12SocketClientEPKci "Permalink to this definition")
    :   Starts a client-side connection with a server.

        Parameters
        :   * **host** â The serverâs hostname.
            * **port** â The serverâs port number.

        Returns
        :   The serverâs socket file descriptor.

    int `SocketAvailable`(int *sockfd*)[Â¶](#_CPPv4N10coralmicro15SocketAvailableEi "Permalink to this definition")
    :   Checks if a socket has available bytes to read.

        Parameters
        :   **sockfd** â The socket file descriptor to check.

        Returns
        :   Number of bytes that can be read from the socket.

    ip4\_addr\_t `DnsGetServer`()[Â¶](#_CPPv4N10coralmicro12DnsGetServerEv "Permalink to this definition")
    :   Retrieves the configured DNS server for the network.

        Returns
        :   `ip4_addr_t` containing the IP address where DNS requests are sent.

    void `DnsSetServer`(ip4\_addr\_t *addr*, bool *persist*)[Â¶](#_CPPv4N10coralmicro12DnsSetServerE10ip4_addr_tb "Permalink to this definition")
    :   Sets the DNS server for the network, and optionally persists it to flash memory.

        Parameters
        :   * **addr** â The address for the DNS server.
            * **persist** â True to persist the address to flash; false otherwise.

Ethernet network
----------------

APIs to get online via Ethernet and read network details.

Note

Using Ethernet requires the Coral PoE Add-on board (or similar add-on).

To get started, just call [`EthernetInit()`](#_CPPv4N10coralmicro12EthernetInitEb "coralmicro::EthernetInit"). If it returns
true, then youâre connected to the network and you can use other functions to
query details such as the board IP address and MAC address.

For example code that uses Curl over Ethernet, see `examples/curl/`.

[[ethernet.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/ethernet.h)

*namespace* `coralmicro`
:   Functions

    *struct* netif \*`EthernetGetInterface`()[Â¶](#_CPPv4N10coralmicro20EthernetGetInterfaceEv "Permalink to this definition")
    :   Gets the ethernet interface, which contains info like ip and hw addresses, interface names, etc.

        Returns
        :   A pointer to the netif ethernet interface, or nullptr if `EthernetInit()` has not been called or the POE add-on board failed to initialize. See: <https://os.mbed.com/docs/mbed-os/v6.15/mbed-os-api-doxy/structnetif.html>

    bool `EthernetInit`(bool *default\_iface*)[Â¶](#_CPPv4N10coralmicro12EthernetInitEb "Permalink to this definition")
    :   Initializes the ethernet module.

        This function requires that the POE add-on board is connected and it must be called before `EthernetGetInterface()`.

        Parameters
        :   **default\_iface** â True sets ethernet as the default network interface, false disables it.

        Returns
        :   True if ethernet successfully initialized; false otherwise.

    std::optional<std::string> `EthernetGetIp`()[Â¶](#_CPPv4N10coralmicro13EthernetGetIpEv "Permalink to this definition")
    :   Gets the deviceâs Ethernet IP address, with a timeout of 30s.

        Returns
        :   A string representing the IPv4 IP address or `std::nullopt` on failure.

    std::optional<std::string> `EthernetGetIp`(uint64\_t *timeout\_ms*)[Â¶](#_CPPv4N10coralmicro13EthernetGetIpE8uint64_t "Permalink to this definition")
    :   Gets the deviceâs Ethernet IP address.

        Parameters
        :   **timeout\_ms** â Amount of time to wait for DHCP to finish, in milliseconds.

        Returns
        :   A string representing the IPv4 IP address or `std::nullopt` on failure.

    std::array<uint8\_t, 6> `EthernetGetMacAddress`()[Â¶](#_CPPv4N10coralmicro21EthernetGetMacAddressEv "Permalink to this definition")
    :   Gets the assigned MAC address from the device fuses.

        Returns
        :   The MAC address assigned to the device.

    std::optional<std::string> `EthernetGetSubnetMask`()[Â¶](#_CPPv4N10coralmicro21EthernetGetSubnetMaskEv "Permalink to this definition")

    std::optional<std::string> `EthernetGetSubnetMask`(uint64\_t *timeout\_ms*)[Â¶](#_CPPv4N10coralmicro21EthernetGetSubnetMaskE8uint64_t "Permalink to this definition")

    std::optional<std::string> `EthernetGetGateway`()[Â¶](#_CPPv4N10coralmicro18EthernetGetGatewayEv "Permalink to this definition")

    std::optional<std::string> `EthernetGetGateway`(uint64\_t *timeout\_ms*)[Â¶](#_CPPv4N10coralmicro18EthernetGetGatewayE8uint64_t "Permalink to this definition")

    int `EthernetGetSpeed`()[Â¶](#_CPPv4N10coralmicro16EthernetGetSpeedEv "Permalink to this definition")
    :   Retrieves the ethernet speed that is stored in flash memory.

        Returns
        :   The ethernet speed in Mbps. The default return value is 100, if no value is stored in flash.

    bool `EthernetSetStaticIp`(ip4\_addr\_t *addr*)[Â¶](#_CPPv4N10coralmicro19EthernetSetStaticIpE10ip4_addr_t "Permalink to this definition")
    :   Stores an IP address in flash memory, to be used as the Ethernet IP.

        Parameters
        :   **addr** â IP address to store.

        Returns
        :   True if the address was stored successfully; false otherwise.

    bool `EthernetSetStaticSubnetMask`(ip4\_addr\_t *addr*)[Â¶](#_CPPv4N10coralmicro27EthernetSetStaticSubnetMaskE10ip4_addr_t "Permalink to this definition")
    :   Stores an IP address in flash memory, to be used as the Ethernet subnet mask.

        Parameters
        :   **addr** â IP address to store.

        Returns
        :   True if the address was stored successfully; false otherwise.

    bool `EthernetSetStaticGateway`(ip4\_addr\_t *addr*)[Â¶](#_CPPv4N10coralmicro24EthernetSetStaticGatewayE10ip4_addr_t "Permalink to this definition")
    :   Stores an IP address in flash memory, to be used as the Ethernet gateway address.

        Parameters
        :   **addr** â IP address to store.

        Returns
        :   True if the address was stored successfully; false otherwise.

Wi-Fi network
-------------

APIs to get online and perform basic Wi-Fi tasks, such as scan for Wi-Fi
networks, connect to a Wi-Fi network, and read network details.

Note

Using Wi-Fi requires the Coral Wireless Add-on board (or similar add-on).

To get started, call [`WiFiTurnOn()`](#_CPPv4N10coralmicro10WiFiTurnOnEb "coralmicro::WiFiTurnOn") to enable the Wi-Fi
module, and then call [`WiFiConnect()`](#_CPPv4N10coralmicro11WiFiConnectERK19WIFINetworkParams_ti "coralmicro::WiFiConnect") to connect to a
Wi-Fi network.

For example code that uses Curl over Wi-Fi, see `examples/curl/`.

[[wifi.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/wifi.h)

*namespace* `coralmicro`
:   Enums

    *enum class* `WiFiAntenna`[Â¶](#_CPPv4N10coralmicro11WiFiAntennaE "Permalink to this definition")
    :   Represents the Wi-Fi antenna.

        *Values:*

        *enumerator* `kInternal`[Â¶](#_CPPv4N10coralmicro11WiFiAntenna9kInternalE "Permalink to this definition")
        :   Internal built in Wi-Fi antenna.

        *enumerator* `kExternal`[Â¶](#_CPPv4N10coralmicro11WiFiAntenna9kExternalE "Permalink to this definition")
        :   External custom Wi-Fi antenna.

    Functions

    bool `WiFiGetDefaultSsid`(std::string \**wifi\_ssid\_out*)[Â¶](#_CPPv4N10coralmicro18WiFiGetDefaultSsidEPNSt6stringE "Permalink to this definition")
    :   Gets the Wi-Fi SSID that is stored in flash memory.

        Parameters
        :   **wifi\_ssid\_out** â A pointer to a string in which to store the SSID.

        Returns
        :   True if the SSID was successfully retrieved; false otherwise.

    bool `WiFiSetDefaultSsid`(*const* std::string &*wifi\_ssid*)[Â¶](#_CPPv4N10coralmicro18WiFiSetDefaultSsidERKNSt6stringE "Permalink to this definition")
    :   Sets the Wi-Fi SSID in flash memory.

        Parameters
        :   **wifi\_ssid** â A pointer to a string containing the SSID.

        Returns
        :   True if the SSID was successfully stored; false otherwise.

    bool `WiFiGetDefaultPsk`(std::string \**wifi\_psk\_out*)[Â¶](#_CPPv4N10coralmicro17WiFiGetDefaultPskEPNSt6stringE "Permalink to this definition")
    :   Gets the Wi-Fi key that is stored in flash memory.

        Parameters
        :   **wifi\_ssid\_out** â A pointer to a string in which to store the SSID.

        Returns
        :   True if the SSID was successfully retrieved; false otherwise.

    bool `WiFiSetDefaultPsk`(*const* std::string &*wifi\_psk*)[Â¶](#_CPPv4N10coralmicro17WiFiSetDefaultPskERKNSt6stringE "Permalink to this definition")
    :   Sets the Wi-Fi key in flash memory.

        Parameters
        :   **wifi\_psk** â A pointer to a string containing the key.

        Returns
        :   True if the key was successfully stored; false otherwise.

    bool `WiFiTurnOn`(bool *default\_iface*)[Â¶](#_CPPv4N10coralmicro10WiFiTurnOnEb "Permalink to this definition")
    :   Turns on the Wi-Fi module.

        Parameters
        :   **default\_iface** â True sets Wi-Fi as the default network interface.

        Returns
        :   True if successfully turned on; false otherwise.

    bool `WiFiTurnOff`()[Â¶](#_CPPv4N10coralmicro11WiFiTurnOffEv "Permalink to this definition")
    :   Turns off the Wi-Fi module.

        Returns
        :   True if successfully turned off; false otherwise.

    bool `WiFiIsConnected`()[Â¶](#_CPPv4N10coralmicro15WiFiIsConnectedEv "Permalink to this definition")
    :   Checks if the board is connected to a Wi-Fi network.

        Returns
        :   True if it is connected; false otherwise.

    bool `WiFiConnect`(*const* WIFINetworkParams\_t &*network\_params*, int *retry\_count* = [kDefaultRetryCount](#_CPPv4N10coralmicro18kDefaultRetryCountE "coralmicro::kDefaultRetryCount"))[Â¶](#_CPPv4N10coralmicro11WiFiConnectERK19WIFINetworkParams_ti "Permalink to this definition")
    :   Connects to a Wi-Fi network.

        Parameters
        :   * **network\_params** â A pointer to a `WIFINetworkParams_t` that contains information of the ssid such as name, password, and security type. See: <https://aws.github.io/amazon-freertos/202107.00/html1/struct_w_i_f_i_network_params__t.html>
            * **retry\_count** â The max number of connection attempts. Default is 5.

        Returns
        :   True if successfully connected to Wi-Fi; false otherwise.

    bool `WiFiConnect`(*const* char \**ssid*, *const* char \**psk*, int *retry\_count* = [kDefaultRetryCount](#_CPPv4N10coralmicro18kDefaultRetryCountE "coralmicro::kDefaultRetryCount"))[Â¶](#_CPPv4N10coralmicro11WiFiConnectEPKcPKci "Permalink to this definition")
    :   Connects to a Wi-Fi network with the given network name and password.

        Parameters
        :   * **ssid** â The network name.
            * **psk** â The password for the ssid.
            * **retry\_count** â The max number of connection attempts. Default is 5.

        Returns
        :   True if successfully connected to Wi-Fi; false otherwise.

    bool `WiFiConnect`(int *retry\_count* = [kDefaultRetryCount](#_CPPv4N10coralmicro18kDefaultRetryCountE "coralmicro::kDefaultRetryCount"))[Â¶](#_CPPv4N10coralmicro11WiFiConnectEi "Permalink to this definition")
    :   Connects to the Wi-Fi network thatâs saved on the device.

        Internally, this API reads the stored ssid and password using `utils::GetWiFiSSID()` and `utils::GetWiFiPSK()`, which could both be set either during flash with the `--wifi_ssid` and `--wifi_psk` flags or with a direct call to `utils::SetWiFiSSID()` and `utils::SetWiFiPSK()`.

        Parameters
        :   **retry\_count** â The max number of connection attempts. Default is 5.

        Returns
        :   True if successfully connected to Wi-Fi; false otherwise.

    bool `WiFiDisconnect`(int *retry\_count* = [kDefaultRetryCount](#_CPPv4N10coralmicro18kDefaultRetryCountE "coralmicro::kDefaultRetryCount"))[Â¶](#_CPPv4N10coralmicro14WiFiDisconnectEi "Permalink to this definition")
    :   Disconnects from the Wi-Fi network.

        Parameters
        :   **retry\_count** â The max number of disconnect attempts. Default is 5.

        Returns
        :   True if Wi-Fi is successfully disconnected; false otherwise.

    std::vector<WIFIScanResult\_t> `WiFiScan`()[Â¶](#_CPPv4N10coralmicro8WiFiScanEv "Permalink to this definition")
    :   Scans for Wi-Fi networks.

        Returns
        :   A vector of `WIFIScanResult_t` which contains info like name, security type, etc. See: <https://aws.github.io/amazon-freertos/202107.00/html1/struct_w_i_f_i_scan_result__t.html>

    std::optional<std::string> `WiFiGetIp`()[Â¶](#_CPPv4N10coralmicro9WiFiGetIpEv "Permalink to this definition")
    :   Gets the deviceâs Wi-Fi IP address.

        Returns
        :   A string representing the IPv4 IP address or `std::nullopt` on failure.

    std::optional<std::array<uint8\_t, 6>> `WiFiGetMac`()[Â¶](#_CPPv4N10coralmicro10WiFiGetMacEv "Permalink to this definition")
    :   Gets the deviceâs Wi-Fi MAC address.

        Returns
        :   Byte array containing the MAC address or `std::nullopt` on failure.

    std::optional<std::array<uint8\_t, 6>> `WiFiGetBssid`()[Â¶](#_CPPv4N10coralmicro12WiFiGetBssidEv "Permalink to this definition")
    :   Gets the BSSID (MAC address of connected Access Point).

        Returns
        :   Byte array containing the MAC address or `std::nullopt` on failure.

    std::optional<int32\_t> `WiFiGetRssi`()[Â¶](#_CPPv4N10coralmicro11WiFiGetRssiEv "Permalink to this definition")
    :   Gets the RSSI of the connected access point.

        Returns
        :   Signal strength in dBm or `std::nullopt` on failure.

    void `WiFiSetAntenna`([WiFiAntenna](#_CPPv4N10coralmicro11WiFiAntennaE "coralmicro::WiFiAntenna") *antenna*)[Â¶](#_CPPv4N10coralmicro14WiFiSetAntennaE11WiFiAntenna "Permalink to this definition")
    :   Sets which Wi-Fi antenna type to use (internal or external).

        Parameters
        :   **antenna** â The type of antenna to use.

    Variables

    *constexpr* int `kDefaultRetryCount` = {5}[Â¶](#_CPPv4N10coralmicro18kDefaultRetryCountE "Permalink to this definition")

HTTP server
-----------

APIs to create an HTTP server on the Dev Board Micro.

To get started, create an instance of
[`HttpServer`](#_CPPv4N10coralmicro10HttpServerE "coralmicro::HttpServer") and call `UriHandler`
to specify a callback function that handles incoming requests. Then pass the
`HttpServer` to [`UseHttpServer()`](#_CPPv4N10coralmicro13UseHttpServerEP10HttpServer "coralmicro::UseHttpServer").

For example, this code creates an HTTP server that responds with a âHello Worldâ
web page (from `examples/http_server/`):

```
namespace coralmicro {
namespace {

HttpServer::Content UriHandler(const char* path) {
  printf("Request received for %s\r\n", path);
  std::vector<uint8_t> html;
  html.reserve(64);
  if (std::strcmp(path, "/hello.html") == 0) {
    printf("Hello World!\r\n");
    StrAppend(&html, "<html><body>Hello World!</body></html>");
    return html;
  }
  return {};
}

void Main() {
  printf("HTTP Server Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  printf("Starting server...\r\n");
  HttpServer http_server;
  http_server.AddUriHandler(UriHandler);
  UseHttpServer(&http_server);

  std::string ip;
  if (GetUsbIpAddress(&ip)) {
    printf("GO TO:   http://%s/%s\r\n", ip.c_str(), "hello.html");
  }
  vTaskSuspend(nullptr);
}

}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::Main();
  vTaskSuspend(nullptr);
}
```

[[http\_server.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/http_server.h)

*namespace* `coralmicro`
:   *class* `HttpServer`[Â¶](#_CPPv4N10coralmicro10HttpServerE "Permalink to this definition")
    :   *#include <http\_server.h>*

        Defines an HTTP server on the device.

        This is a light wrapper around the lwIP stack. For more detail, see <https://www.nongnu.org/lwip/2_1_x/index.html>.

        For an example, see examples/http\_server/http\_server.cc.

        *struct* `StaticBuffer`[Â¶](#_CPPv4N10coralmicro10HttpServer12StaticBufferE "Permalink to this definition")
        :   *#include <http\_server.h>*

            Defines a static buffer in which to return data from the server.

            Public Members

            *const* uint8\_t \*`buffer`[Â¶](#_CPPv4N10coralmicro10HttpServer12StaticBuffer6bufferE "Permalink to this definition")
            :   A pointer to the data buffer.

            size\_t `size`[Â¶](#_CPPv4N10coralmicro10HttpServer12StaticBuffer4sizeE "Permalink to this definition")
            :   The size of the buffer in bytes.

        Public Functions

        *virtual* `~HttpServer`() = default[Â¶](#_CPPv4N10coralmicro10HttpServerD0Ev "Permalink to this definition")

        *inline* *virtual* err\_t `PostBegin`(void \**connection*, *const* char \**uri*, *const* char \**http\_request*, u16\_t *http\_request\_len*, int *content\_len*, char \**response\_uri*, u16\_t *response\_uri\_len*, u8\_t \**post\_auto\_wnd*)[Â¶](#_CPPv4N10coralmicro10HttpServer9PostBeginEPvPKcPKc5u16_tiPc5u16_tP4u8_t "Permalink to this definition")
        :   Called when an HTTP POST request is first received. This must be implemented by subclasses that want to handle posts.

            Parameters
            :   * **connection** â Unique connection identifier, valid until `PostFinished()` is called.
                * **uri** â The HTTP header URI receiving the POST request.
                * **http\_request** â The raw HTTP request (the first packet, normally).
                * **http\_request\_len** â Size of âhttp\_requestâ.
                * **content\_len** â Content-Length from HTTP header.
                * **response\_uri** â Filename of response file, to be filled when denying the request.
                * **response\_uri\_len** â Size of the âresponse\_uriâ buffer.
                * **post\_auto\_wnd** â Set this to 0 to let the callback code handle window updates by calling `httpd_post_data_recved` (to throttle rx speed) default is 1 (httpd handles window updates automatically)

        *inline* *virtual* err\_t `PostReceiveData`(void \**connection*, *struct* pbuf \**p*)[Â¶](#_CPPv4N10coralmicro10HttpServer15PostReceiveDataEPvP4pbuf "Permalink to this definition")
        :   Called for each packet buffer of data that is received for a POST.

            Parameters
            :   * **connection** â Unique connection identifier.
                * **p** â Received data as a [pbuf](https://www.nongnu.org/lwip/2_1_x/structpbuf.html). **ATTENTION:** Your application is responsible for freeing the pbufs!

        *inline* *virtual* void `PostFinished`(void \**connection*, char \**response\_uri*, u16\_t *response\_uri\_len*)[Â¶](#_CPPv4N10coralmicro10HttpServer12PostFinishedEPvPc5u16_t "Permalink to this definition")
        :   Called when all data is received or when the connection is closed. The application must return the filename/URI of a file to send in response to this POST request. If the response\_uri buffer is untouched, a 404 response is returned.

            Parameters
            :   * **connection** â Unique connection identifier.
                * **response\_uri** â Filename of response file, to be filled when denying the request.
                * **response\_uri\_len** â Size of the âresponse\_uriâ buffer.

        *inline* *virtual* void `CgiHandler`(*struct* fs\_file \**file*, *const* char \**uri*, int *iNumParams*, char \*\**pcParam*, char \*\**pcValue*)[Â¶](#_CPPv4N10coralmicro10HttpServer10CgiHandlerEP7fs_filePKciPPcPPc "Permalink to this definition")
        :   Called once to handle CGI for every URI with parameters.

            Parameters
            :   * **file** â The file received.
                * **uri** â The HTTP header URI.
                * **iNumParams** â The number of parameters in the URI.
                * **pcParam** â Parameter names from the URI.
                * **pcValue** â Values for each parameter. file, uri, count, http\_cgi\_params, http\_cgi\_param\_vals

        *virtual* int `FsOpenCustom`(*struct* fs\_file \**file*, *const* char \**name*)[Â¶](#_CPPv4N10coralmicro10HttpServer12FsOpenCustomEP7fs_filePKc "Permalink to this definition")
        :   Called first for every opened file to allow opening custom files that are not included in fsdata(\_custom).c.

        *virtual* int `FsReadCustom`(*struct* fs\_file \**file*, char \**buffer*, int *count*)[Â¶](#_CPPv4N10coralmicro10HttpServer12FsReadCustomEP7fs_filePci "Permalink to this definition")
        :   Called to read custom files.

        *virtual* void `FsCloseCustom`(*struct* fs\_file \**file*)[Â¶](#_CPPv4N10coralmicro10HttpServer13FsCloseCustomEP7fs_file "Permalink to this definition")
        :   Called to close custom files.

        *inline* void `AddUriHandler`([UriHandler](#_CPPv4N10coralmicro10HttpServer10UriHandlerE "coralmicro::HttpServer::UriHandler") *handler*)[Â¶](#_CPPv4N10coralmicro10HttpServer13AddUriHandlerE10UriHandler "Permalink to this definition")
        :   Adds a URI handler function for the server.

            You can specify multiple handlers and incoming requests will be sent to the handlers in the order that each handler was added with this function. That is, the first handler you add receives all requests first, and if it does not handle it, then it is sent to the next handler, and so on.

            Parameters
            :   **handler** â A callback function to handle each incoming HTTP request. It must accept the URI path as a char string and return the response as `Content`.

        Public Types

        *using* `Content` = std::variant<std::monostate, std::string, std::vector<uint8\_t>, [StaticBuffer](#_CPPv4N10coralmicro10HttpServer12StaticBufferE "coralmicro::HttpServer::StaticBuffer")>[Â¶](#_CPPv4N10coralmicro10HttpServer7ContentE "Permalink to this definition")
        :   Defines the allowed response types returned by `AddUriHandler()`. Successful requests will typically respond with the content in a string, a dynamic buffer (a vector), or a `StaticBuffer`, or an empty vector if the URI is unhandled.

        *using* `UriHandler` = std::function<[Content](#_CPPv4N10coralmicro10HttpServer7ContentE "coralmicro::HttpServer::Content")(*const* char \*uri)>[Â¶](#_CPPv4N10coralmicro10HttpServer10UriHandlerE "Permalink to this definition")
        :   Represents the callback function type required by `AddUriHandler()`.

    Functions

    void `UseHttpServer`([HttpServer](#_CPPv4N10coralmicro10HttpServerE "coralmicro::HttpServer") \**server*)[Â¶](#_CPPv4N10coralmicro13UseHttpServerEP10HttpServer "Permalink to this definition")
    :   Starts an HTTP server.

        To handle server requests, you must pass your implementation of `UriHandler()` to `AddUriHandler()`.

        Parameters
        :   **The** â server to start.

RPC HTTP server
---------------

APIs to create an RPC server on the Dev Board Micro.

The setup is the same as [`HttpServer`](#_CPPv4N10coralmicro10HttpServerE "coralmicro::HttpServer") but you must pass
an instance of [`JsonRpcHttpServer`](#_CPPv4N10coralmicro17JsonRpcHttpServerE "coralmicro::JsonRpcHttpServer") to
[`UseHttpServer()`](#_CPPv4N10coralmicro13UseHttpServerEP10HttpServer "coralmicro::UseHttpServer") and also initialize `jsonrpc`.

Note

You must also include `third_party/mjson/src/mjson.h` to initialize
with `jsonrpc_init()` and specify RPC functions with `jsonrpc_export()`.

For example, this code creates an RPC server that responds with the board
serial number (from `examples/rpc_server/`):

```
namespace coralmicro {
namespace {

void SerialNumber(struct jsonrpc_request* r) {
  auto serial = GetSerialNumber();
  jsonrpc_return_success(r, "{%Q:%.*Q}", "serial_number", serial.size(),
                         serial.c_str());
}

void Main() {
  printf("RPC Server Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  jsonrpc_init(nullptr, nullptr);
  jsonrpc_export("serial_number", SerialNumber);
  UseHttpServer(new JsonRpcHttpServer);
  printf("RPC server ready\r\n");
}

}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::Main();
  vTaskSuspend(nullptr);
}
```

[[rpc\_http\_server.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/rpc/rpc_http_server.h)

*namespace* `coralmicro`
:   *class* `JsonRpcHttpServer` : *public* [coralmicro](../arduino/#_CPPv410coralmicro "coralmicro")::[HttpServer](#_CPPv4N10coralmicro10HttpServerE "coralmicro::HttpServer")[Â¶](#_CPPv4N10coralmicro17JsonRpcHttpServerE "Permalink to this definition")
    :   *#include <rpc\_http\_server.h>*

        Public Functions

        *inline* *explicit* `JsonRpcHttpServer`(*struct* jsonrpc\_ctx \**ctx* = &jsonrpc\_default\_context)[Â¶](#_CPPv4N10coralmicro17JsonRpcHttpServer17JsonRpcHttpServerEP11jsonrpc_ctx "Permalink to this definition")

        *virtual* err\_t `PostBegin`(void \**connection*, *const* char \**uri*, *const* char \**http\_request*, u16\_t *http\_request\_len*, int *content\_len*, char \**response\_uri*, u16\_t *response\_uri\_len*, u8\_t \**post\_auto\_wnd*) *override*[Â¶](#_CPPv4N10coralmicro17JsonRpcHttpServer9PostBeginEPvPKcPKc5u16_tiPc5u16_tP4u8_t "Permalink to this definition")
        :   Called when an HTTP POST request is first received. This must be implemented by subclasses that want to handle posts.

            Parameters
            :   * **connection** â Unique connection identifier, valid until `PostFinished()` is called.
                * **uri** â The HTTP header URI receiving the POST request.
                * **http\_request** â The raw HTTP request (the first packet, normally).
                * **http\_request\_len** â Size of âhttp\_requestâ.
                * **content\_len** â Content-Length from HTTP header.
                * **response\_uri** â Filename of response file, to be filled when denying the request.
                * **response\_uri\_len** â Size of the âresponse\_uriâ buffer.
                * **post\_auto\_wnd** â Set this to 0 to let the callback code handle window updates by calling `httpd_post_data_recved` (to throttle rx speed) default is 1 (httpd handles window updates automatically)

        *virtual* err\_t `PostReceiveData`(void \**connection*, *struct* pbuf \**p*) *override*[Â¶](#_CPPv4N10coralmicro17JsonRpcHttpServer15PostReceiveDataEPvP4pbuf "Permalink to this definition")
        :   Called for each packet buffer of data that is received for a POST.

            Parameters
            :   * **connection** â Unique connection identifier.
                * **p** â Received data as a [pbuf](https://www.nongnu.org/lwip/2_1_x/structpbuf.html). **ATTENTION:** Your application is responsible for freeing the pbufs!

        *virtual* void `PostFinished`(void \**connection*, char \**response\_uri*, u16\_t *response\_uri\_len*) *override*[Â¶](#_CPPv4N10coralmicro17JsonRpcHttpServer12PostFinishedEPvPc5u16_t "Permalink to this definition")
        :   Called when all data is received or when the connection is closed. The application must return the filename/URI of a file to send in response to this POST request. If the response\_uri buffer is untouched, a 404 response is returned.

            Parameters
            :   * **connection** â Unique connection identifier.
                * **response\_uri** â Filename of response file, to be filled when denying the request.
                * **response\_uri\_len** â Size of the âresponse\_uriâ buffer.

        *virtual* void `CgiHandler`(*struct* fs\_file \**file*, *const* char \**uri*, int *iNumParams*, char \*\**pcParam*, char \*\**pcValue*) *override*[Â¶](#_CPPv4N10coralmicro17JsonRpcHttpServer10CgiHandlerEP7fs_filePKciPPcPPc "Permalink to this definition")
        :   Called once to handle CGI for every URI with parameters.

            Parameters
            :   * **file** â The file received.
                * **uri** â The HTTP header URI.
                * **iNumParams** â The number of parameters in the URI.
                * **pcParam** â Parameter names from the URI.
                * **pcValue** â Values for each parameter. file, uri, count, http\_cgi\_params, http\_cgi\_param\_vals

        *virtual* int `FsOpenCustom`(*struct* fs\_file \**file*, *const* char \**name*) *override*[Â¶](#_CPPv4N10coralmicro17JsonRpcHttpServer12FsOpenCustomEP7fs_filePKc "Permalink to this definition")
        :   Called first for every opened file to allow opening custom files that are not included in fsdata(\_custom).c.

        *virtual* void `FsCloseCustom`(*struct* fs\_file \**file*) *override*[Â¶](#_CPPv4N10coralmicro17JsonRpcHttpServer13FsCloseCustomEP7fs_file "Permalink to this definition")
        :   Called to close custom files.

RPC utils
---------

[[rpc\_utils.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/rpc/rpc_utils.h)

*namespace* `coralmicro`
:   Functions

    void `JsonRpcReturnBadParam`(*struct* jsonrpc\_request \**request*, *const* char \**message*, *const* char \**param\_name*)[Â¶](#_CPPv4N10coralmicro21JsonRpcReturnBadParamEP15jsonrpc_requestPKcPKc "Permalink to this definition")
    :   Response a JSONRPC\_ERROR\_BAD\_PARAMS code to the requester.

        Parameters
        :   * **request** â The request to response to.
            * **message** â The message to send back to the requester.
            * **param\_name** â The name of the bad param.

    bool `JsonRpcGetIntegerParam`(*struct* jsonrpc\_request \**request*, *const* char \**param\_name*, int \**out*)[Â¶](#_CPPv4N10coralmicro22JsonRpcGetIntegerParamEP15jsonrpc_requestPKcPi "Permalink to this definition")
    :   Gets an integer param from RPC request.

        Parameters
        :   * **request** â The request to parse the integer.
            * **param\_name** â The name of the parameter to parse.
            * **out** â The integer to return the value to.

        Returns
        :   True if the param were parsed successfully, else False.

    bool `JsonRpcGetBooleanParam`(*struct* jsonrpc\_request \**request*, *const* char \**param\_name*, bool \**out*)[Â¶](#_CPPv4N10coralmicro22JsonRpcGetBooleanParamEP15jsonrpc_requestPKcPb "Permalink to this definition")
    :   Gets a boolean param from RPC request.

        Parameters
        :   * **request** â The request to parse the boolean.
            * **param\_name** â The name of the parameter to parse.
            * **out** â The boolean to return the value to.

        Returns
        :   True if the param were parsed successfully, else False.

    bool `JsonRpcGetStringParam`(*struct* jsonrpc\_request \**request*, *const* char \**param\_name*, std::string \**out*)[Â¶](#_CPPv4N10coralmicro21JsonRpcGetStringParamEP15jsonrpc_requestPKcPNSt6stringE "Permalink to this definition")
    :   Gets a string param from RPC request.

        Parameters
        :   * **request** â The request to parse the string.
            * **param\_name** â The name of the parameter to parse.
            * **out** â The string to return the value to.

        Returns
        :   True if the param were parsed successfully, else False.

    bool `JsonRpcGetBase64Param`(*struct* jsonrpc\_request \**request*, *const* char \**param\_name*, std::vector<uint8\_t> \**out*)[Â¶](#_CPPv4N10coralmicro21JsonRpcGetBase64ParamEP15jsonrpc_requestPKcPNSt6vectorI7uint8_tEE "Permalink to this definition")
    :   Gets a base64 encoded string param from RPC request.

        Parameters
        :   * **request** â The request to parse the string.
            * **param\_name** â The name of the parameter to parse.
            * **out** â The output array to return the value to.

        Returns
        :   True if the param were parsed successfully, else False.

Device information
------------------

[[utils.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/utils.h)

Defines

`FUSE_ADDRESS_TO_OCOTP_INDEX`(*fuse*)[Â¶](#c.FUSE_ADDRESS_TO_OCOTP_INDEX "Permalink to this definition")

`MAC1_ADDR_LO`[Â¶](#c.MAC1_ADDR_LO "Permalink to this definition")

`MAC1_ADDR_HI`[Â¶](#c.MAC1_ADDR_HI "Permalink to this definition")

*namespace* `coralmicro`
:   Functions

    uint64\_t `GetUniqueId`()[Â¶](#_CPPv4N10coralmicro11GetUniqueIdEv "Permalink to this definition")
    :   Gets the 64-bit unique identifiers of the RT1176.

        Returns
        :   64-bit value that is unique to the device.

    std::string `GetSerialNumber`()[Â¶](#_CPPv4N10coralmicro15GetSerialNumberEv "Permalink to this definition")
    :   Gets the hex string representation of the unique identifier.

        Returns
        :   String containing the 64-bit unique identifier, as a printable hex string.

    bool `GetUsbIpAddress`(std::string \**usb\_ip\_out*)[Â¶](#_CPPv4N10coralmicro15GetUsbIpAddressEPNSt6stringE "Permalink to this definition")
    :   Gets the USB IP address that is stored in flash memory.

        Parameters
        :   **usb\_ip\_out** â A pointer to a string in which to store a printable version of the IP.

        Returns
        :   True if the address was successfully retrieved; false otherwise.

    bool `GetIpFromFile`(*const* char \**path*, ip4\_addr\_t \**addr*)[Â¶](#_CPPv4N10coralmicro13GetIpFromFileEPKcP10ip4_addr_t "Permalink to this definition")
    :   Attempts to open a given file at `path`, and if the contents are an IP address, return them in `addr`.

        Parameters
        :   * **path** â File path to read an address from.
            * **addr** â Pointer to an `ip4_addr_t` that the address will be stored in.

        Returns
        :   True if successful; false otherwise.