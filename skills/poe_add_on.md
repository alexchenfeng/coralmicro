Set up the PoE Add-on board
===========================

The Coral PoE Add-on Board provides wired connectivity and power for the Coral
Dev Board Micro. It adds an RJ45 Ethernet jack that can provide power to
the main board. This page shows how to connect the boards
and start developing with Ethernet.

For technical details, see the [Coral PoE Add-on
datasheet](/docs/dev-board-micro/poe-datasheet/).

Connect the boards together
---------------------------

Carefully press the boards together as follows:

1. Orient the boards to align the board-to-board connectors and mounting holes.
   (The PoE Add-on uses just one board-to-board
   connector, so the other connector pad is unpopulated.)

   **Caution:** Do not to press on the
   camera lens or you might damage the sensor.
2. Pinch the boards together using a flat part of the board near the connectors
   (it's okay to press on the mounted chips).

   You should hear a firm click.

![](/static/docs/images/devboard-micro/micro-attach-poe.png)


**Figure 1.** Connecting the PoE Add-on

When viewed from the side, the boards should be perfectly aligned and the
four standoffs should be touching the main board.

**Caution:** To avoid accidental detachment and damage
to the boards, fasten the boards together with M2.5 screws
and/or put the boards into a case.

To remove the PoE Add-on board, separate the boards at the short side with the
Ethernet jack. **Do not** peel them apart from the long sides (where the
header pins reside) or you risk detatching the connector.

Power the board over Ethernet
-----------------------------

As long as your Ethernet cable is connected to a switch capable of
Power-over-Ethernet, just plug the Ethernet cable into the PoE
Add-on board and it will power the whole system.

Connect to an Ethernet network
------------------------------

**Arduino users:** See the
[Arduino Ethernet API](/docs/reference/micro/arduino/#ethernet) and
try the examples in the Arduino IDE: **File > Examples >
Dev Board Micro + PoE**.

To get online with Ethernet, include `coralmicro/libs/base/ethernet.h` and then
call [`coralmicro::EthernetInit()`](/docs/reference/micro/network/#_CPPv4N10coralmicro12EthernetInitEb).
That's it.

For example, try the `curl` code example using Ethernet as follows
(run this from the coralmicro repo root):

```
bash build.sh

python3 scripts/flashtool.py -e curl --subapp curl_ethernet
```

You should see output in the
[serial console](/docs/dev-board-micro/serial-console/) like this:

```
Attempting to use ethernet...
Failed to read ethernet speed, assuming 100M.
Initializing PHY...
DHCP succeeded, our IP is 192.168.86.54.
www.example.com -> 93.184.216.34
Curling http://www.example.com:80/
Curling of http://www.example.com:80/ successful! (1256 bytes curled)
Done.
```