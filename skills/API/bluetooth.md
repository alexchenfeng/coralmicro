Bluetooth APIs
==============

APIs for classic Bluetooth (BR/EDR) and Bluetooth low-energy (LE).

Note

Using Bluetooth requires the Coral Wireless Add-on board (or similar add-on).

You need to include only the
`libs/nxp/rt1176-sdk/edgefast_bluetooth/edgefast_bluetooth.h` header.
This header defines just the `InitEdgefastBluetooth()` function but also
includes all the other Bluetooth APIs shown here.

[[bluetooth.h source]](https://github.com/google-coral/coralmicro/blob/main/third_party/nxp/rt1176-sdk/middleware/edgefast_bluetooth/include/bluetooth/bluetooth.h)

Functions

void `InitEdgefastBluetooth`(bt\_ready\_cb\_t *cb*)[Â¶](#_CPPv421InitEdgefastBluetooth13bt_ready_cb_t "Permalink to this definition")
:   Initializes Bluetooth for the Dev Board Micro.

    Requires the Coral Wireless Add-on (or similar add-on board). This is a wrapper for `bt_enable()` and performs other board-specific setup. Note: This function is non-blocking and bluetooth could be uninitialized after return. Please check `BluetoothReady()` to make sure it is in fact initialized before using other functions.

    Parameters
    :   **cb** â Callback to notify when Bluetooth is enabled or fails to enable.

bool `BluetoothReady`()[Â¶](#_CPPv414BluetoothReadyv "Permalink to this definition")
:   Check if bluetooth is ready.

    Returns
    :   true if bluetooth is ready, false if it is not.

bool `BluetoothAdvertise`()[Â¶](#_CPPv418BluetoothAdvertisev "Permalink to this definition")
:   Start bluetooth advertising.

    Returns
    :   true id advertised successfully, false if advertise failed.

std::optional<std::vector<std::string>> `BluetoothScan`(int *max\_results*, unsigned int *scan\_period\_ms*)[Â¶](#_CPPv413BluetoothScanij "Permalink to this definition")
:   Performs bluetooth scan.

    Parameters
    :   * **max\_results** â Number of scan results.
        * **scan\_period\_ms** â Time in ms to scan for devices.

    Returns
    :   An array of bluetooth ids.

---

Bluetooth subsystem core APIs.

*struct* `bt_le_ext_adv_sent_info`[Â¶](#_CPPv423bt_le_ext_adv_sent_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    uint8\_t `num_sent`[Â¶](#_CPPv4N23bt_le_ext_adv_sent_info8num_sentE "Permalink to this definition")
    :   The number of advertising events completed.

*struct* `bt_le_ext_adv_connected_info`[Â¶](#_CPPv428bt_le_ext_adv_connected_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    *struct* bt\_conn \*`conn`[Â¶](#_CPPv4N28bt_le_ext_adv_connected_info4connE "Permalink to this definition")
    :   Connection object of the new connection

*struct* `bt_le_ext_adv_scanned_info`[Â¶](#_CPPv426bt_le_ext_adv_scanned_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    bt\_addr\_le\_t \*`addr`[Â¶](#_CPPv4N26bt_le_ext_adv_scanned_info4addrE "Permalink to this definition")
    :   Active scanner LE address and type

*struct* `bt_le_ext_adv_cb`[Â¶](#_CPPv416bt_le_ext_adv_cb "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    void (\*`sent`)(*struct* bt\_le\_ext\_adv \*adv, *struct* [bt\_le\_ext\_adv\_sent\_info](#_CPPv423bt_le_ext_adv_sent_info "bt_le_ext_adv_sent_info") \*info)[Â¶](#_CPPv4N16bt_le_ext_adv_cb4sentE "Permalink to this definition")
    :   The advertising set has finished sending adv data.

        This callback notifies the application that the advertising set has finished sending advertising data. The advertising set can either have been stopped by a timeout or because the specified number of advertising events has been reached.

        Parameters
        :   * **adv** â The advertising set object.
            * **info** â Information about the sent event.

    void (\*`connected`)(*struct* bt\_le\_ext\_adv \*adv, *struct* [bt\_le\_ext\_adv\_connected\_info](#_CPPv428bt_le_ext_adv_connected_info "bt_le_ext_adv_connected_info") \*info)[Â¶](#_CPPv4N16bt_le_ext_adv_cb9connectedE "Permalink to this definition")
    :   The advertising set has accepted a new connection.

        This callback notifies the application that the advertising set has accepted a new connection.

        Parameters
        :   * **adv** â The advertising set object.
            * **info** â Information about the connected event.

    void (\*`scanned`)(*struct* bt\_le\_ext\_adv \*adv, *struct* [bt\_le\_ext\_adv\_scanned\_info](#_CPPv426bt_le_ext_adv_scanned_info "bt_le_ext_adv_scanned_info") \*info)[Â¶](#_CPPv4N16bt_le_ext_adv_cb7scannedE "Permalink to this definition")
    :   The advertising set has sent scan response data.

        This callback notifies the application that the advertising set has has received a Scan Request packet, and has sent a Scan Response packet.

        Parameters
        :   * **adv** â The advertising set object.
            * **addr** â Information about the scanned event.

*struct* `bt_data`[Â¶](#_CPPv47bt_data "Permalink to this definition")
:   *#include <bluetooth.h>*

    Bluetooth data.

    Description of different data types that can be encoded into advertising data. Used to form arrays that are passed to the [bt\_le\_adv\_start()](#group__bt__gap_1gad2e3caef88d52d720e8e4d21df767b02) function.

    Public Members

    uint8\_t `type`[Â¶](#_CPPv4N7bt_data4typeE "Permalink to this definition")

    uint8\_t `data_len`[Â¶](#_CPPv4N7bt_data8data_lenE "Permalink to this definition")

    *const* uint8\_t \*`data`[Â¶](#_CPPv4N7bt_data4dataE "Permalink to this definition")

*struct* `bt_le_adv_param`[Â¶](#_CPPv415bt_le_adv_param "Permalink to this definition")
:   *#include <bluetooth.h>*

    LE Advertising Parameters.

    Public Members

    uint8\_t `id`[Â¶](#_CPPv4N15bt_le_adv_param2idE "Permalink to this definition")
    :   Local identity.

        Note

        When extended advertising @option{CONFIG\_BT\_EXT\_ADV} is not enabled or not supported by the controller it is not possible to scan and advertise simultaneously using two different random addresses.

    uint8\_t `sid`[Â¶](#_CPPv4N15bt_le_adv_param3sidE "Permalink to this definition")
    :   Advertising Set Identifier, valid range 0x00 - 0x0f.

        Note

        Requires BT\_LE\_ADV\_OPT\_EXT\_ADV

    uint8\_t `secondary_max_skip`[Â¶](#_CPPv4N15bt_le_adv_param18secondary_max_skipE "Permalink to this definition")
    :   Secondary channel maximum skip count.

        Maximum advertising events the advertiser can skip before it must send advertising data on the secondary advertising channel.

        Note

        Requires BT\_LE\_ADV\_OPT\_EXT\_ADV

    uint32\_t `options`[Â¶](#_CPPv4N15bt_le_adv_param7optionsE "Permalink to this definition")
    :   Bit-field of advertising options

    uint32\_t `interval_min`[Â¶](#_CPPv4N15bt_le_adv_param12interval_minE "Permalink to this definition")
    :   Minimum Advertising Interval (N \* 0.625 milliseconds) Minimum Advertising Interval shall be less than or equal to the Maximum Advertising Interval. The Minimum Advertising Interval and Maximum Advertising Interval should not be the same value (as stated in Bluetooth Core Spec 5.2, section 7.8.5) Range: 0x0020 to 0x4000

    uint32\_t `interval_max`[Â¶](#_CPPv4N15bt_le_adv_param12interval_maxE "Permalink to this definition")
    :   Maximum Advertising Interval (N \* 0.625 milliseconds) Minimum Advertising Interval shall be less than or equal to the Maximum Advertising Interval. The Minimum Advertising Interval and Maximum Advertising Interval should not be the same value (as stated in Bluetooth Core Spec 5.2, section 7.8.5) Range: 0x0020 to 0x4000

    *const* bt\_addr\_le\_t \*`peer`[Â¶](#_CPPv4N15bt_le_adv_param4peerE "Permalink to this definition")
    :   Directed advertising to peer.

        When this parameter is set the advertiser will send directed advertising to the remote device.

        The advertising type will either be high duty cycle, or low duty cycle if the BT\_LE\_ADV\_OPT\_DIR\_MODE\_LOW\_DUTY option is enabled. When using BT\_LE\_ADV\_OPT\_EXT\_ADV then only low duty cycle is allowed.

        In case of connectable high duty cycle if the connection could not be established within the timeout the connected() callback will be called with the status set to BT\_HCI\_ERR\_ADV\_TIMEOUT.

*struct* `bt_le_per_adv_param`[Â¶](#_CPPv419bt_le_per_adv_param "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    uint16\_t `interval_min`[Â¶](#_CPPv4N19bt_le_per_adv_param12interval_minE "Permalink to this definition")
    :   Minimum Periodic Advertising Interval (N \* 1.25 ms)

        Shall be greater or equal to BT\_GAP\_PER\_ADV\_MIN\_INTERVAL and less or equal to interval\_max.

    uint16\_t `interval_max`[Â¶](#_CPPv4N19bt_le_per_adv_param12interval_maxE "Permalink to this definition")
    :   Maximum Periodic Advertising Interval (N \* 1.25 ms)

        Shall be less or equal to BT\_GAP\_PER\_ADV\_MAX\_INTERVAL and greater or equal to interval\_min.

    uint32\_t `options`[Â¶](#_CPPv4N19bt_le_per_adv_param7optionsE "Permalink to this definition")
    :   Bit-field of periodic advertising options

*struct* `bt_le_ext_adv_start_param`[Â¶](#_CPPv425bt_le_ext_adv_start_param "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    uint16\_t `timeout`[Â¶](#_CPPv4N25bt_le_ext_adv_start_param7timeoutE "Permalink to this definition")
    :   Advertiser timeout (N \* 10 ms).

        Application will be notified by the advertiser sent callback. Set to zero for no timeout.

        When using high duty cycle directed connectable advertising then this parameters must be set to a non-zero value less than or equal to the maximum of BT\_GAP\_ADV\_HIGH\_DUTY\_CYCLE\_MAX\_TIMEOUT.

        If privacy @option{CONFIG\_BT\_PRIVACY} is enabled then the timeout must be less than @option{CONFIG\_BT\_RPA\_TIMEOUT}.

    uint8\_t `num_events`[Â¶](#_CPPv4N25bt_le_ext_adv_start_param10num_eventsE "Permalink to this definition")
    :   Number of advertising events.

        Application will be notified by the advertiser sent callback. Set to zero for no limit.

*struct* `bt_le_ext_adv_info`[Â¶](#_CPPv418bt_le_ext_adv_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Advertising set info structure.

    Public Members

    uint8\_t `id`[Â¶](#_CPPv4N18bt_le_ext_adv_info2idE "Permalink to this definition")

    int8\_t `tx_power`[Â¶](#_CPPv4N18bt_le_ext_adv_info8tx_powerE "Permalink to this definition")
    :   Currently selected Transmit Power (dBM).

*struct* `bt_le_per_adv_sync_synced_info`[Â¶](#_CPPv430bt_le_per_adv_sync_synced_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    *const* bt\_addr\_le\_t \*`addr`[Â¶](#_CPPv4N30bt_le_per_adv_sync_synced_info4addrE "Permalink to this definition")
    :   Advertiser LE address and type.

    uint8\_t `sid`[Â¶](#_CPPv4N30bt_le_per_adv_sync_synced_info3sidE "Permalink to this definition")
    :   Advertiser SID

    uint16\_t `interval`[Â¶](#_CPPv4N30bt_le_per_adv_sync_synced_info8intervalE "Permalink to this definition")
    :   Periodic advertising interval (N \* 1.25 ms)

    uint8\_t `phy`[Â¶](#_CPPv4N30bt_le_per_adv_sync_synced_info3phyE "Permalink to this definition")
    :   Advertiser PHY

    bool `recv_enabled`[Â¶](#_CPPv4N30bt_le_per_adv_sync_synced_info12recv_enabledE "Permalink to this definition")
    :   True if receiving periodic advertisements, false otherwise.

    uint16\_t `service_data`[Â¶](#_CPPv4N30bt_le_per_adv_sync_synced_info12service_dataE "Permalink to this definition")
    :   Service Data provided by the peer when sync is transferred.

        Will always be 0 when the sync is locally created.

    *struct* bt\_conn \*`conn`[Â¶](#_CPPv4N30bt_le_per_adv_sync_synced_info4connE "Permalink to this definition")
    :   Peer that transferred the periodic advertising sync.

        Will always be 0 when the sync is locally created.

*struct* `bt_le_per_adv_sync_term_info`[Â¶](#_CPPv428bt_le_per_adv_sync_term_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    *const* bt\_addr\_le\_t \*`addr`[Â¶](#_CPPv4N28bt_le_per_adv_sync_term_info4addrE "Permalink to this definition")
    :   Advertiser LE address and type.

    uint8\_t `sid`[Â¶](#_CPPv4N28bt_le_per_adv_sync_term_info3sidE "Permalink to this definition")
    :   Advertiser SID

*struct* `bt_le_per_adv_sync_recv_info`[Â¶](#_CPPv428bt_le_per_adv_sync_recv_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    *const* bt\_addr\_le\_t \*`addr`[Â¶](#_CPPv4N28bt_le_per_adv_sync_recv_info4addrE "Permalink to this definition")
    :   Advertiser LE address and type.

    uint8\_t `sid`[Â¶](#_CPPv4N28bt_le_per_adv_sync_recv_info3sidE "Permalink to this definition")
    :   Advertiser SID

    int8\_t `tx_power`[Â¶](#_CPPv4N28bt_le_per_adv_sync_recv_info8tx_powerE "Permalink to this definition")
    :   The TX power of the advertisement.

    int8\_t `rssi`[Â¶](#_CPPv4N28bt_le_per_adv_sync_recv_info4rssiE "Permalink to this definition")
    :   The RSSI of the advertisement excluding any CTE.

    uint8\_t `cte_type`[Â¶](#_CPPv4N28bt_le_per_adv_sync_recv_info8cte_typeE "Permalink to this definition")
    :   The Constant Tone Extension (CTE) of the advertisement (bt\_df\_cte\_type)

*struct* `bt_le_per_adv_sync_state_info`[Â¶](#_CPPv429bt_le_per_adv_sync_state_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    bool `recv_enabled`[Â¶](#_CPPv4N29bt_le_per_adv_sync_state_info12recv_enabledE "Permalink to this definition")
    :   True if receiving periodic advertisements, false otherwise.

*struct* `bt_le_per_adv_sync_cb`[Â¶](#_CPPv421bt_le_per_adv_sync_cb "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    void (\*`synced`)(*struct* bt\_le\_per\_adv\_sync \*sync, *struct* [bt\_le\_per\_adv\_sync\_synced\_info](#_CPPv430bt_le_per_adv_sync_synced_info "bt_le_per_adv_sync_synced_info") \*info)[Â¶](#_CPPv4N21bt_le_per_adv_sync_cb6syncedE "Permalink to this definition")
    :   The periodic advertising has been successfully synced.

        This callback notifies the application that the periodic advertising set has been successfully synced, and will now start to receive periodic advertising reports.

        Parameters
        :   * **sync** â The periodic advertising sync object.
            * **info** â Information about the sync event.

    void (\*`term`)(*struct* bt\_le\_per\_adv\_sync \*sync, *const* *struct* [bt\_le\_per\_adv\_sync\_term\_info](#_CPPv428bt_le_per_adv_sync_term_info "bt_le_per_adv_sync_term_info") \*info)[Â¶](#_CPPv4N21bt_le_per_adv_sync_cb4termE "Permalink to this definition")
    :   The periodic advertising sync has been terminated.

        This callback notifies the application that the periodic advertising sync has been terminated, either by local request, remote request or because due to missing data, e.g. by being out of range or sync.

        Parameters
        :   **sync** â The periodic advertising sync object.

    void (\*`recv`)(*struct* bt\_le\_per\_adv\_sync \*sync, *const* *struct* [bt\_le\_per\_adv\_sync\_recv\_info](#_CPPv428bt_le_per_adv_sync_recv_info "bt_le_per_adv_sync_recv_info") \*info, *struct* net\_buf\_simple \*buf)[Â¶](#_CPPv4N21bt_le_per_adv_sync_cb4recvE "Permalink to this definition")
    :   Periodic advertising data received.

        This callback notifies the application of an periodic advertising report.

        Parameters
        :   * **sync** â The advertising set object.
            * **info** â Information about the periodic advertising event.
            * **buf** â Buffer containing the periodic advertising data.

    void (\*`state_changed`)(*struct* bt\_le\_per\_adv\_sync \*sync, *const* *struct* [bt\_le\_per\_adv\_sync\_state\_info](#_CPPv429bt_le_per_adv_sync_state_info "bt_le_per_adv_sync_state_info") \*info)[Â¶](#_CPPv4N21bt_le_per_adv_sync_cb13state_changedE "Permalink to this definition")
    :   The periodic advertising sync state has changed.

        This callback notifies the application about changes to the sync state. Initialize sync and termination is handled by their individual callbacks, and wonât be notified here.

        Parameters
        :   * **sync** â The periodic advertising sync object.
            * **info** â Information about the state change.

    void (\*`biginfo`)(*struct* bt\_le\_per\_adv\_sync \*sync, *const* *struct* bt\_iso\_biginfo \*biginfo)[Â¶](#_CPPv4N21bt_le_per_adv_sync_cb7biginfoE "Permalink to this definition")
    :   BIGInfo advertising report received.

        This callback notifies the application of a BIGInfo advertising report. This is received if the advertiser is broadcasting isochronous streams in a BIG. See iso.h for more information.

        Parameters
        :   * **sync** â The advertising set object.
            * **biginfo** â The BIGInfo report.

    void (\*`cte_report_cb`)(*struct* bt\_le\_per\_adv\_sync \*sync, *struct* bt\_df\_per\_adv\_sync\_iq\_samples\_report*const* \*info)[Â¶](#_CPPv4N21bt_le_per_adv_sync_cb13cte_report_cbE "Permalink to this definition")
    :   Callback for IQ samples report collected when sampling CTE received with periodic advertising PDU.

        Parameters
        :   * **sync** â The periodic advertising sync object.
            * **info** â Information about the sync event.

    sys\_snode\_t `node`[Â¶](#_CPPv4N21bt_le_per_adv_sync_cb4nodeE "Permalink to this definition")

*struct* `bt_le_per_adv_sync_param`[Â¶](#_CPPv424bt_le_per_adv_sync_param "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    bt\_addr\_le\_t `addr`[Â¶](#_CPPv4N24bt_le_per_adv_sync_param4addrE "Permalink to this definition")
    :   Periodic Advertiser Address.

        Only valid if not using the periodic advertising list

    uint8\_t `sid`[Â¶](#_CPPv4N24bt_le_per_adv_sync_param3sidE "Permalink to this definition")
    :   Advertiser SID.

        Only valid if not using the periodic advertising list

    uint32\_t `options`[Â¶](#_CPPv4N24bt_le_per_adv_sync_param7optionsE "Permalink to this definition")
    :   Bit-field of periodic advertising sync options.

    uint16\_t `skip`[Â¶](#_CPPv4N24bt_le_per_adv_sync_param4skipE "Permalink to this definition")
    :   Maximum event skip.

        Maximum number of periodic advertising events that can be skipped after a successful receive

    uint16\_t `timeout`[Â¶](#_CPPv4N24bt_le_per_adv_sync_param7timeoutE "Permalink to this definition")
    :   Synchronization timeout (N \* 10 ms)

        Synchronization timeout for the periodic advertising sync. Range 0x000A to 0x4000 (100 ms to 163840 ms)

*struct* `bt_le_per_adv_sync_info`[Â¶](#_CPPv423bt_le_per_adv_sync_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Advertising set info structure.

    Public Members

    bt\_addr\_le\_t `addr`[Â¶](#_CPPv4N23bt_le_per_adv_sync_info4addrE "Permalink to this definition")
    :   Periodic Advertiser Address

    uint8\_t `sid`[Â¶](#_CPPv4N23bt_le_per_adv_sync_info3sidE "Permalink to this definition")
    :   Advertiser SID

    uint16\_t `interval`[Â¶](#_CPPv4N23bt_le_per_adv_sync_info8intervalE "Permalink to this definition")
    :   Periodic advertising interval (N \* 1.25 ms)

    uint8\_t `phy`[Â¶](#_CPPv4N23bt_le_per_adv_sync_info3phyE "Permalink to this definition")
    :   Advertiser PHY

*struct* `bt_le_per_adv_sync_transfer_param`[Â¶](#_CPPv433bt_le_per_adv_sync_transfer_param "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    uint16\_t `skip`[Â¶](#_CPPv4N33bt_le_per_adv_sync_transfer_param4skipE "Permalink to this definition")
    :   Maximum event skip.

        The number of periodic advertising packets that can be skipped after a successful receive.

    uint16\_t `timeout`[Â¶](#_CPPv4N33bt_le_per_adv_sync_transfer_param7timeoutE "Permalink to this definition")
    :   Synchronization timeout (N \* 10 ms)

        Synchronization timeout for the periodic advertising sync. Range 0x000A to 0x4000 (100 ms to 163840 ms)

    uint32\_t `options`[Â¶](#_CPPv4N33bt_le_per_adv_sync_transfer_param7optionsE "Permalink to this definition")
    :   Periodic Advertising Sync Transfer options

*struct* `bt_le_scan_param`[Â¶](#_CPPv416bt_le_scan_param "Permalink to this definition")
:   *#include <bluetooth.h>*

    LE scan parameters

    Public Members

    uint8\_t `type`[Â¶](#_CPPv4N16bt_le_scan_param4typeE "Permalink to this definition")
    :   Scan type (BT\_LE\_SCAN\_TYPE\_ACTIVE or BT\_LE\_SCAN\_TYPE\_PASSIVE)

    uint32\_t `options`[Â¶](#_CPPv4N16bt_le_scan_param7optionsE "Permalink to this definition")
    :   Bit-field of scanning options.

    uint16\_t `interval`[Â¶](#_CPPv4N16bt_le_scan_param8intervalE "Permalink to this definition")
    :   Scan interval (N \* 0.625 ms)

    uint16\_t `window`[Â¶](#_CPPv4N16bt_le_scan_param6windowE "Permalink to this definition")
    :   Scan window (N \* 0.625 ms)

    uint16\_t `timeout`[Â¶](#_CPPv4N16bt_le_scan_param7timeoutE "Permalink to this definition")
    :   Scan timeout (N \* 10 ms)

        Application will be notified by the scan timeout callback. Set zero to disable timeout.

    uint16\_t `interval_coded`[Â¶](#_CPPv4N16bt_le_scan_param14interval_codedE "Permalink to this definition")
    :   Scan interval LE Coded PHY (N \* 0.625 MS)

        Set zero to use same as LE 1M PHY scan interval.

    uint16\_t `window_coded`[Â¶](#_CPPv4N16bt_le_scan_param12window_codedE "Permalink to this definition")
    :   Scan window LE Coded PHY (N \* 0.625 MS)

        Set zero to use same as LE 1M PHY scan window.

*struct* `bt_le_scan_recv_info`[Â¶](#_CPPv420bt_le_scan_recv_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    LE advertisement packet information

    Public Members

    *const* bt\_addr\_le\_t \*`addr`[Â¶](#_CPPv4N20bt_le_scan_recv_info4addrE "Permalink to this definition")
    :   Advertiser LE address and type.

        If advertiser is anonymous then this address will be BT\_ADDR\_LE\_ANY.

    uint8\_t `sid`[Â¶](#_CPPv4N20bt_le_scan_recv_info3sidE "Permalink to this definition")
    :   Advertising Set Identifier.

    int8\_t `rssi`[Â¶](#_CPPv4N20bt_le_scan_recv_info4rssiE "Permalink to this definition")
    :   Strength of advertiser signal.

    int8\_t `tx_power`[Â¶](#_CPPv4N20bt_le_scan_recv_info8tx_powerE "Permalink to this definition")
    :   Transmit power of the advertiser.

    uint8\_t `adv_type`[Â¶](#_CPPv4N20bt_le_scan_recv_info8adv_typeE "Permalink to this definition")
    :   Advertising packet type.

    uint16\_t `adv_props`[Â¶](#_CPPv4N20bt_le_scan_recv_info9adv_propsE "Permalink to this definition")
    :   Advertising packet properties.

    uint16\_t `interval`[Â¶](#_CPPv4N20bt_le_scan_recv_info8intervalE "Permalink to this definition")
    :   Periodic advertising interval.

        If 0 there is no periodic advertising.

    uint8\_t `primary_phy`[Â¶](#_CPPv4N20bt_le_scan_recv_info11primary_phyE "Permalink to this definition")
    :   Primary advertising channel PHY.

    uint8\_t `secondary_phy`[Â¶](#_CPPv4N20bt_le_scan_recv_info13secondary_phyE "Permalink to this definition")
    :   Secondary advertising channel PHY.

*struct* `bt_le_scan_cb`[Â¶](#_CPPv413bt_le_scan_cb "Permalink to this definition")
:   *#include <bluetooth.h>*

    Listener context for (LE) scanning.

    Public Members

    void (\*`recv`)(*const* *struct* [bt\_le\_scan\_recv\_info](#_CPPv420bt_le_scan_recv_info "bt_le_scan_recv_info") \*info, *struct* net\_buf\_simple \*buf)[Â¶](#_CPPv4N13bt_le_scan_cb4recvE "Permalink to this definition")
    :   Advertisement packet received callback.

        Parameters
        :   * **info** â Advertiser packet information.
            * **buf** â Buffer containing advertiser data.

    void (\*`timeout`)(void)[Â¶](#_CPPv4N13bt_le_scan_cb7timeoutE "Permalink to this definition")
    :   The scanner has stopped scanning after scan timeout.

    sys\_snode\_t `node`[Â¶](#_CPPv4N13bt_le_scan_cb4nodeE "Permalink to this definition")

*struct* `bt_le_oob_sc_data`[Â¶](#_CPPv417bt_le_oob_sc_data "Permalink to this definition")
:   *#include <bluetooth.h>*

    LE Secure Connections pairing Out of Band data.

    Public Members

    uint8\_t `r`[16][Â¶](#_CPPv4N17bt_le_oob_sc_data1rE "Permalink to this definition")
    :   Random Number.

    uint8\_t `c`[16][Â¶](#_CPPv4N17bt_le_oob_sc_data1cE "Permalink to this definition")
    :   Confirm Value.

*struct* `bt_le_oob`[Â¶](#_CPPv49bt_le_oob "Permalink to this definition")
:   *#include <bluetooth.h>*

    LE Out of Band information.

    Public Members

    bt\_addr\_le\_t `addr`[Â¶](#_CPPv4N9bt_le_oob4addrE "Permalink to this definition")
    :   LE address. If privacy is enabled this is a Resolvable Private Address.

    *struct* [bt\_le\_oob\_sc\_data](#_CPPv417bt_le_oob_sc_data "bt_le_oob_sc_data") `le_sc_data`[Â¶](#_CPPv4N9bt_le_oob10le_sc_dataE "Permalink to this definition")
    :   LE Secure Connections pairing Out of Band data.

*struct* `bt_br_discovery_result`[Â¶](#_CPPv422bt_br_discovery_result "Permalink to this definition")
:   *#include <bluetooth.h>*

    BR/EDR discovery result structure.

    Public Members

    uint8\_t `_priv`[4][Â¶](#_CPPv4N22bt_br_discovery_result5_privE "Permalink to this definition")
    :   private

    bt\_addr\_t `addr`[Â¶](#_CPPv4N22bt_br_discovery_result4addrE "Permalink to this definition")
    :   Remote device address

    int8\_t `rssi`[Â¶](#_CPPv4N22bt_br_discovery_result4rssiE "Permalink to this definition")
    :   RSSI from inquiry

    uint8\_t `cod`[3][Â¶](#_CPPv4N22bt_br_discovery_result3codE "Permalink to this definition")
    :   Class of Device

    uint8\_t `eir`[240][Â¶](#_CPPv4N22bt_br_discovery_result3eirE "Permalink to this definition")
    :   Extended Inquiry Response

*struct* `bt_br_discovery_param`[Â¶](#_CPPv421bt_br_discovery_param "Permalink to this definition")
:   *#include <bluetooth.h>*

    BR/EDR discovery parameters

    Public Members

    uint8\_t `length`[Â¶](#_CPPv4N21bt_br_discovery_param6lengthE "Permalink to this definition")
    :   Maximum length of the discovery in units of 1.28 seconds. Valid range is 0x01 - 0x30.

    bool `limited`[Â¶](#_CPPv4N21bt_br_discovery_param7limitedE "Permalink to this definition")
    :   True if limited discovery procedure is to be used.

*struct* `bt_br_oob`[Â¶](#_CPPv49bt_br_oob "Permalink to this definition")
:   *#include <bluetooth.h>*

    Public Members

    bt\_addr\_t `addr`[Â¶](#_CPPv4N9bt_br_oob4addrE "Permalink to this definition")
    :   BR/EDR address.

*struct* `bt_bond_info`[Â¶](#_CPPv412bt_bond_info "Permalink to this definition")
:   *#include <bluetooth.h>*

    Information about a bond with a remote device.

    Public Members

    bt\_addr\_le\_t `addr`[Â¶](#_CPPv4N12bt_bond_info4addrE "Permalink to this definition")
    :   Address of the remote device.

Defines

`BT_ID_DEFAULT`[Â¶](#c.BT_ID_DEFAULT "Permalink to this definition")
:   Convenience macro for specifying the default identity. This helps make the code more readable, especially when only one identity is supported.

`BT_DATA`(*\_type*, *\_data*, *\_data\_len*)[Â¶](#c.BT_DATA "Permalink to this definition")
:   Helper to declare elements of [bt\_data](#structbt__data) arrays.

    This macro is mainly for creating an array of struct [bt\_data](#structbt__data) elements which is then passed to e.g. [bt\_le\_adv\_start()](#group__bt__gap_1gad2e3caef88d52d720e8e4d21df767b02).

    Parameters
    :   * **\_type** â Type of advertising data field
        * **\_data** â Pointer to the data field payload
        * **\_data\_len** â Number of bytes behind the \_data pointer

`BT_DATA_BYTES(_type, _bytes...)`
:   Helper to declare elements of [bt\_data](#structbt__data) arrays.

    This macro is mainly for creating an array of struct [bt\_data](#structbt__data) elements which is then passed to e.g. [bt\_le\_adv\_start()](#group__bt__gap_1gad2e3caef88d52d720e8e4d21df767b02).

    Parameters
    :   * **\_type** â Type of advertising data field
        * **\_bytes** â Variable number of single-byte parameters

`BT_LE_ADV_PARAM_INIT`(*\_options*, *\_int\_min*, *\_int\_max*, *\_peer*)[Â¶](#c.BT_LE_ADV_PARAM_INIT "Permalink to this definition")
:   Initialize advertising parameters.

    Parameters
    :   * **\_options** â Advertising Options
        * **\_int\_min** â Minimum advertising interval
        * **\_int\_max** â Maximum advertising interval
        * **\_peer** â Peer address, set to NULL for undirected advertising or address of peer for directed advertising.

`BT_LE_ADV_PARAM`(*\_options*, *\_int\_min*, *\_int\_max*, *\_peer*)[Â¶](#c.BT_LE_ADV_PARAM "Permalink to this definition")
:   Helper to declare advertising parameters inline.

    Parameters
    :   * **\_options** â Advertising Options
        * **\_int\_min** â Minimum advertising interval
        * **\_int\_max** â Maximum advertising interval
        * **\_peer** â Peer address, set to NULL for undirected advertising or address of peer for directed advertising.

`BT_LE_ADV_CONN_DIR`(*\_peer*)[Â¶](#c.BT_LE_ADV_CONN_DIR "Permalink to this definition")

`BT_LE_ADV_CONN`[Â¶](#c.BT_LE_ADV_CONN "Permalink to this definition")

`BT_LE_ADV_CONN_NAME`[Â¶](#c.BT_LE_ADV_CONN_NAME "Permalink to this definition")

`BT_LE_ADV_CONN_NAME_AD`[Â¶](#c.BT_LE_ADV_CONN_NAME_AD "Permalink to this definition")

`BT_LE_ADV_CONN_DIR_LOW_DUTY`(*\_peer*)[Â¶](#c.BT_LE_ADV_CONN_DIR_LOW_DUTY "Permalink to this definition")

`BT_LE_ADV_NCONN`[Â¶](#c.BT_LE_ADV_NCONN "Permalink to this definition")
:   Non-connectable advertising with private address

`BT_LE_ADV_NCONN_NAME`[Â¶](#c.BT_LE_ADV_NCONN_NAME "Permalink to this definition")
:   Non-connectable advertising with BT\_LE\_ADV\_OPT\_USE\_NAME

`BT_LE_ADV_NCONN_IDENTITY`[Â¶](#c.BT_LE_ADV_NCONN_IDENTITY "Permalink to this definition")
:   Non-connectable advertising with BT\_LE\_ADV\_OPT\_USE\_IDENTITY

`BT_LE_EXT_ADV_CONN_NAME`[Â¶](#c.BT_LE_EXT_ADV_CONN_NAME "Permalink to this definition")
:   Connectable extended advertising with BT\_LE\_ADV\_OPT\_USE\_NAME

`BT_LE_EXT_ADV_NCONN`[Â¶](#c.BT_LE_EXT_ADV_NCONN "Permalink to this definition")
:   Non-connectable extended advertising with private address

`BT_LE_EXT_ADV_NCONN_NAME`[Â¶](#c.BT_LE_EXT_ADV_NCONN_NAME "Permalink to this definition")
:   Non-connectable extended advertising with BT\_LE\_ADV\_OPT\_USE\_NAME

`BT_LE_EXT_ADV_NCONN_IDENTITY`[Â¶](#c.BT_LE_EXT_ADV_NCONN_IDENTITY "Permalink to this definition")
:   Non-connectable extended advertising with BT\_LE\_ADV\_OPT\_USE\_IDENTITY

`BT_LE_EXT_ADV_CODED_NCONN`[Â¶](#c.BT_LE_EXT_ADV_CODED_NCONN "Permalink to this definition")
:   Non-connectable extended advertising on coded PHY with private address

`BT_LE_EXT_ADV_CODED_NCONN_NAME`[Â¶](#c.BT_LE_EXT_ADV_CODED_NCONN_NAME "Permalink to this definition")
:   Non-connectable extended advertising on coded PHY with BT\_LE\_ADV\_OPT\_USE\_NAME

`BT_LE_EXT_ADV_CODED_NCONN_IDENTITY`[Â¶](#c.BT_LE_EXT_ADV_CODED_NCONN_IDENTITY "Permalink to this definition")
:   Non-connectable extended advertising on coded PHY with BT\_LE\_ADV\_OPT\_USE\_IDENTITY

`BT_LE_EXT_ADV_START_PARAM_INIT`(*\_timeout*, *\_n\_evts*)[Â¶](#c.BT_LE_EXT_ADV_START_PARAM_INIT "Permalink to this definition")
:   Helper to initialize extended advertising start parameters inline

    Parameters
    :   * **\_timeout** â Advertiser timeout
        * **\_n\_evts** â Number of advertising events

`BT_LE_EXT_ADV_START_PARAM`(*\_timeout*, *\_n\_evts*)[Â¶](#c.BT_LE_EXT_ADV_START_PARAM "Permalink to this definition")
:   Helper to declare extended advertising start parameters inline

    Parameters
    :   * **\_timeout** â Advertiser timeout
        * **\_n\_evts** â Number of advertising events

`BT_LE_EXT_ADV_START_DEFAULT`[Â¶](#c.BT_LE_EXT_ADV_START_DEFAULT "Permalink to this definition")

`BT_LE_PER_ADV_PARAM_INIT`(*\_int\_min*, *\_int\_max*, *\_options*)[Â¶](#c.BT_LE_PER_ADV_PARAM_INIT "Permalink to this definition")
:   Helper to declare periodic advertising parameters inline

    Parameters
    :   * **\_int\_min** â Minimum periodic advertising interval
        * **\_int\_max** â Maximum periodic advertising interval
        * **\_options** â Periodic advertising properties bitfield.

`BT_LE_PER_ADV_PARAM`(*\_int\_min*, *\_int\_max*, *\_options*)[Â¶](#c.BT_LE_PER_ADV_PARAM "Permalink to this definition")
:   Helper to declare periodic advertising parameters inline

    Parameters
    :   * **\_int\_min** â Minimum periodic advertising interval
        * **\_int\_max** â Maximum periodic advertising interval
        * **\_options** â Periodic advertising properties bitfield.

`BT_LE_PER_ADV_DEFAULT`[Â¶](#c.BT_LE_PER_ADV_DEFAULT "Permalink to this definition")

`BT_LE_SCAN_PARAM_INIT`(*\_type*, *\_options*, *\_interval*, *\_window*)[Â¶](#c.BT_LE_SCAN_PARAM_INIT "Permalink to this definition")
:   Initialize scan parameters.

    Parameters
    :   * **\_type** â Scan Type, BT\_LE\_SCAN\_TYPE\_ACTIVE or BT\_LE\_SCAN\_TYPE\_PASSIVE.
        * **\_options** â Scan options
        * **\_interval** â Scan Interval (N \* 0.625 ms)
        * **\_window** â Scan Window (N \* 0.625 ms)

`BT_LE_SCAN_PARAM`(*\_type*, *\_options*, *\_interval*, *\_window*)[Â¶](#c.BT_LE_SCAN_PARAM "Permalink to this definition")
:   Helper to declare scan parameters inline.

    Parameters
    :   * **\_type** â Scan Type, BT\_LE\_SCAN\_TYPE\_ACTIVE or BT\_LE\_SCAN\_TYPE\_PASSIVE.
        * **\_options** â Scan options
        * **\_interval** â Scan Interval (N \* 0.625 ms)
        * **\_window** â Scan Window (N \* 0.625 ms)

`BT_LE_SCAN_ACTIVE`[Â¶](#c.BT_LE_SCAN_ACTIVE "Permalink to this definition")
:   Helper macro to enable active scanning to discover new devices.

`BT_LE_SCAN_PASSIVE`[Â¶](#c.BT_LE_SCAN_PASSIVE "Permalink to this definition")
:   Helper macro to enable passive scanning to discover new devices.

    This macro should be used if information required for device identification (e.g., UUID) are known to be placed in Advertising Data.

`BT_LE_SCAN_CODED_ACTIVE`[Â¶](#c.BT_LE_SCAN_CODED_ACTIVE "Permalink to this definition")
:   Helper macro to enable active scanning to discover new devices. Include scanning on Coded PHY in addition to 1M PHY.

`BT_LE_SCAN_CODED_PASSIVE`[Â¶](#c.BT_LE_SCAN_CODED_PASSIVE "Permalink to this definition")
:   Helper macro to enable passive scanning to discover new devices. Include scanning on Coded PHY in addition to 1M PHY.

    This macro should be used if information required for device identification (e.g., UUID) are known to be placed in Advertising Data.

Functions

int `bt_enable`(bt\_ready\_cb\_t *cb*)[Â¶](#_CPPv49bt_enable13bt_ready_cb_t "Permalink to this definition")
:   Enable Bluetooth.

    Enable Bluetooth. Must be the called before any calls that require communication with the local Bluetooth hardware.

    Parameters
    :   **cb** â Callback to notify completion or NULL to perform the enabling synchronously.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_set_name`(*const* char \**name*)[Â¶](#_CPPv411bt_set_namePKc "Permalink to this definition")
:   Set Bluetooth Device Name.

    Set Bluetooth GAP Device Name.

    When advertising with device name in the advertising data the name should be updated by calling [bt\_le\_adv\_update\_data](#group__bt__gap_1ga9a406ebfefac3dd09935a4ae0e317817) or [bt\_le\_ext\_adv\_set\_data](#group__bt__gap_1gad731f829b3566be3e56485b2a64f80b1).

    Parameters
    :   **name** â New name

    Returns
    :   Zero on success or (negative) error code otherwise.

*const* char \*`bt_get_name`(void)[Â¶](#_CPPv411bt_get_namev "Permalink to this definition")
:   Get Bluetooth Device Name.

    Get Bluetooth GAP Device Name.

    Returns
    :   Bluetooth Device Name

`__deprecated int bt_set_id_addr (const bt_addr_le_t *addr)`
:   Set the local Identity Address.

    Allows setting the local Identity Address from the application. This API must be called before calling [bt\_enable()](#group__bt__gap_1gac45d16bfe21c3c38e834c293e5ebc42b). Calling it at any other time will cause it to fail. In most cases the application doesnât need to use this API, however there are a few valid cases where it can be useful (such as for testing).

    At the moment, the given address must be a static random address. In the future support for public addresses may be added.

    *Deprecated:*
    :   in 2.5 release, replace with bt\_id\_create before bt\_enable.

    Returns
    :   Zero on success or (negative) error code otherwise.

void `bt_id_get`(bt\_addr\_le\_t \**addrs*, size\_t \**count*)[Â¶](#_CPPv49bt_id_getP12bt_addr_le_tP6size_t "Permalink to this definition")
:   Get the currently configured identities.

    Returns an array of the currently configured identity addresses. To make sure all available identities can be retrieved, the number of elements in the *addrs* array should be CONFIG\_BT\_ID\_MAX. The identity identifier that some APIs expect (such as advertising parameters) is simply the index of the identity in the *addrs* array.

    If *addrs* is passed as NULL, then returned *count* contains the count of all available identities that can be retrieved with a subsequent call to this function with non-NULL *addrs* parameter.

    Parameters
    :   * **addrs** â Array where to store the configured identities.
        * **count** â Should be initialized to the array size. Once the function returns it will contain the number of returned identities.

    Note

    Deleted identities may show up as BT\_LE\_ADDR\_ANY in the returned array.

int `bt_id_create`(bt\_addr\_le\_t \**addr*, uint8\_t \**irk*)[Â¶](#_CPPv412bt_id_createP12bt_addr_le_tP7uint8_t "Permalink to this definition")
:   Create a new identity.

    Create a new identity using the given address and IRK. This function can be called before calling [bt\_enable()](#group__bt__gap_1gac45d16bfe21c3c38e834c293e5ebc42b), in which case it can be used to override the controllerâs public address (in case it has one). However, the new identity will only be stored persistently in flash when this API is used after [bt\_enable()](#group__bt__gap_1gac45d16bfe21c3c38e834c293e5ebc42b). The reason is that the persistent settings are loaded after [bt\_enable()](#group__bt__gap_1gac45d16bfe21c3c38e834c293e5ebc42b) and would therefore cause potential conflicts with the stack blindly overwriting whatâs stored in flash. The identity will also not be written to flash in case a pre-defined address is provided, since in such a situation the app clearly has some place it got the address from and will be able to repeat the procedure on every power cycle, i.e. it would be redundant to also store the information in flash.

    Generating random static address or random IRK is not supported when calling this function before [bt\_enable()](#group__bt__gap_1gac45d16bfe21c3c38e834c293e5ebc42b).

    If the application wants to have the stack randomly generate identities and store them in flash for later recovery, the way to do it would be to first initialize the stack (using bt\_enable), then call settings\_load(), and after that check with [bt\_id\_get()](#group__bt__gap_1ga06d0ae35cbf4382679cc3cfe612cee4d) how many identities were recovered. If an insufficient amount of identities were recovered the app may then call [bt\_id\_create()](#group__bt__gap_1gae11eb8ad254418c38a0e8689df25a159) to create new ones.

    Parameters
    :   * **addr** â Address to use for the new identity. If NULL or initialized to BT\_ADDR\_LE\_ANY the stack will generate a new random static address for the identity and copy it to the given parameter upon return from this function (in case the parameter was non-NULL).
        * **irk** â Identity Resolving Key (16 bytes) to be used with this identity. If set to all zeroes or NULL, the stack will generate a random IRK for the identity and copy it back to the parameter upon return from this function (in case the parameter was non-NULL). If privacy @option{CONFIG\_BT\_PRIVACY} is not enabled this parameter must be NULL.

    Returns
    :   Identity identifier (>= 0) in case of success, or a negative error code on failure.

int `bt_id_reset`(uint8\_t *id*, bt\_addr\_le\_t \**addr*, uint8\_t \**irk*)[Â¶](#_CPPv411bt_id_reset7uint8_tP12bt_addr_le_tP7uint8_t "Permalink to this definition")
:   Reset/reclaim an identity for reuse.

    The semantics of the *addr* and *irk* parameters of this function are the same as with [bt\_id\_create()](#group__bt__gap_1gae11eb8ad254418c38a0e8689df25a159). The difference is the first *id* parameter that needs to be an existing identity (if it doesnât exist this function will return an error). When given an existing identity this function will disconnect any connections created using it, remove any pairing keys or other data associated with it, and then create a new identity in the same slot, based on the *addr* and *irk* parameters.

    Parameters
    :   * **id** â Existing identity identifier.
        * **addr** â Address to use for the new identity. If NULL or initialized to BT\_ADDR\_LE\_ANY the stack will generate a new static random address for the identity and copy it to the given parameter upon return from this function (in case the parameter was non-NULL).
        * **irk** â Identity Resolving Key (16 bytes) to be used with this identity. If set to all zeroes or NULL, the stack will generate a random IRK for the identity and copy it back to the parameter upon return from this function (in case the parameter was non-NULL). If privacy @option{CONFIG\_BT\_PRIVACY} is not enabled this parameter must be NULL.

    Returns
    :   Identity identifier (>= 0) in case of success, or a negative error code on failure.

    Note

    the default identity (BT\_ID\_DEFAULT) cannot be reset, i.e. this API will return an error if asked to do that.

int `bt_id_delete`(uint8\_t *id*)[Â¶](#_CPPv412bt_id_delete7uint8_t "Permalink to this definition")
:   Delete an identity.

    When given a valid identity this function will disconnect any connections created using it, remove any pairing keys or other data associated with it, and then flag is as deleted, so that it can not be used for any operations. To take back into use the slot the identity was occupying the [bt\_id\_reset()](#group__bt__gap_1gabb3353edc8a3a8d29a0370049b20cbe4) API needs to be used.

    Parameters
    :   **id** â Existing identity identifier.

    Returns
    :   0 in case of success, or a negative error code on failure.

    Note

    the default identity (BT\_ID\_DEFAULT) cannot be deleted, i.e. this API will return an error if asked to do that.

int `bt_le_adv_start`(*const* *struct* [bt\_le\_adv\_param](#_CPPv415bt_le_adv_param "bt_le_adv_param") \**param*, *const* *struct* [bt\_data](#_CPPv47bt_data "bt_data") \**ad*, size\_t *ad\_len*, *const* *struct* [bt\_data](#_CPPv47bt_data "bt_data") \**sd*, size\_t *sd\_len*)[Â¶](#_CPPv415bt_le_adv_startPK15bt_le_adv_paramPK7bt_data6size_tPK7bt_data6size_t "Permalink to this definition")
:   Start advertising.

    Set advertisement data, scan response data, advertisement parameters and start advertising.

    When the advertisement parameter peer address has been set the advertising will be directed to the peer. In this case advertisement data and scan response data parameters are ignored. If the mode is high duty cycle the timeout will be BT\_GAP\_ADV\_HIGH\_DUTY\_CYCLE\_MAX\_TIMEOUT.

    Parameters
    :   * **param** â Advertising parameters.
        * **ad** â Data to be used in advertisement packets.
        * **ad\_len** â Number of elements in ad
        * **sd** â Data to be used in scan response packets.
        * **sd\_len** â Number of elements in sd

    Returns
    :   Zero on success or (negative) error code otherwise.

    Returns
    :   -ENOMEM No free connection objects available for connectable advertiser.

    Returns
    :   -ECONNREFUSED When connectable advertising is requested and there is already maximum number of connections established in the controller. This error code is only guaranteed when using Zephyr controller, for other controllers code returned in this case may be -EIO.

int `bt_le_adv_update_data`(*const* *struct* [bt\_data](#_CPPv47bt_data "bt_data") \**ad*, size\_t *ad\_len*, *const* *struct* [bt\_data](#_CPPv47bt_data "bt_data") \**sd*, size\_t *sd\_len*)[Â¶](#_CPPv421bt_le_adv_update_dataPK7bt_data6size_tPK7bt_data6size_t "Permalink to this definition")
:   Update advertising.

    Update advertisement and scan response data.

    Parameters
    :   * **ad** â Data to be used in advertisement packets.
        * **ad\_len** â Number of elements in ad
        * **sd** â Data to be used in scan response packets.
        * **sd\_len** â Number of elements in sd

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_adv_stop`(void)[Â¶](#_CPPv414bt_le_adv_stopv "Permalink to this definition")
:   Stop advertising.

    Stops ongoing advertising.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_ext_adv_create`(*const* *struct* [bt\_le\_adv\_param](#_CPPv415bt_le_adv_param "bt_le_adv_param") \**param*, *const* *struct* [bt\_le\_ext\_adv\_cb](#_CPPv416bt_le_ext_adv_cb "bt_le_ext_adv_cb") \**cb*, *struct* bt\_le\_ext\_adv \*\**adv*)[Â¶](#_CPPv420bt_le_ext_adv_createPK15bt_le_adv_paramPK16bt_le_ext_adv_cbPP13bt_le_ext_adv "Permalink to this definition")
:   Create advertising set.

    Create a new advertising set and set advertising parameters. Advertising parameters can be updated with [bt\_le\_ext\_adv\_update\_param](#group__bt__gap_1ga1aabdb81cb1a1841ff0fb91d849123fc).

    Parameters
    :   * **param** â **[in]** Advertising parameters.
        * **cb** â **[in]** Callback struct to notify about advertiser activity. Can be NULL. Must point to valid memory during the lifetime of the advertising set.
        * **adv** â **[out]** Valid advertising set object on success.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_ext_adv_start`(*struct* bt\_le\_ext\_adv \**adv*, *struct* [bt\_le\_ext\_adv\_start\_param](#_CPPv425bt_le_ext_adv_start_param "bt_le_ext_adv_start_param") \**param*)[Â¶](#_CPPv419bt_le_ext_adv_startP13bt_le_ext_advP25bt_le_ext_adv_start_param "Permalink to this definition")
:   Start advertising with the given advertising set.

    If the advertiser is limited by either the timeout or number of advertising events the application will be notified by the advertiser sent callback once the limit is reached. If the advertiser is limited by both the timeout and the number of advertising events then the limit that is reached first will stop the advertiser.

    Parameters
    :   * **adv** â Advertising set object.
        * **param** â Advertise start parameters.

int `bt_le_ext_adv_stop`(*struct* bt\_le\_ext\_adv \**adv*)[Â¶](#_CPPv418bt_le_ext_adv_stopP13bt_le_ext_adv "Permalink to this definition")
:   Stop advertising with the given advertising set.

    Stop advertising with a specific advertising set. When using this function the advertising sent callback will not be called.

    Parameters
    :   **adv** â Advertising set object.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_ext_adv_set_data`(*struct* bt\_le\_ext\_adv \**adv*, *const* *struct* [bt\_data](#_CPPv47bt_data "bt_data") \**ad*, size\_t *ad\_len*, *const* *struct* [bt\_data](#_CPPv47bt_data "bt_data") \**sd*, size\_t *sd\_len*)[Â¶](#_CPPv422bt_le_ext_adv_set_dataP13bt_le_ext_advPK7bt_data6size_tPK7bt_data6size_t "Permalink to this definition")
:   Set an advertising setâs advertising or scan response data.

    Set advertisement data or scan response data. If the advertising set is currently advertising then the advertising data will be updated in subsequent advertising events.

    When both BT\_LE\_ADV\_OPT\_EXT\_ADV and BT\_LE\_ADV\_OPT\_SCANNABLE are enabled then advertising data is ignored. When BT\_LE\_ADV\_OPT\_SCANNABLE is not enabled then scan response data is ignored.

    If the advertising set has been configured to send advertising data on the primary advertising channels then the maximum data length is BT\_GAP\_ADV\_MAX\_ADV\_DATA\_LEN bytes. If the advertising set has been configured for extended advertising, then the maximum data length is defined by the controller with the maximum possible of BT\_GAP\_ADV\_MAX\_EXT\_ADV\_DATA\_LEN bytes.

    Parameters
    :   * **adv** â Advertising set object.
        * **ad** â Data to be used in advertisement packets.
        * **ad\_len** â Number of elements in ad
        * **sd** â Data to be used in scan response packets.
        * **sd\_len** â Number of elements in sd

    Returns
    :   Zero on success or (negative) error code otherwise.

    Note

    Not all scanners support extended data length advertising data.

    Note

    When updating the advertising data while advertising the advertising data and scan response data length must be smaller or equal to what can be fit in a single advertising packet. Otherwise the advertiser must be stopped.

int `bt_le_ext_adv_update_param`(*struct* bt\_le\_ext\_adv \**adv*, *const* *struct* [bt\_le\_adv\_param](#_CPPv415bt_le_adv_param "bt_le_adv_param") \**param*)[Â¶](#_CPPv426bt_le_ext_adv_update_paramP13bt_le_ext_advPK15bt_le_adv_param "Permalink to this definition")
:   Update advertising parameters.

    Update the advertising parameters. The function will return an error if the advertiser set is currently advertising. Stop the advertising set before calling this function.

    Parameters
    :   * **adv** â Advertising set object.
        * **param** â Advertising parameters.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_ext_adv_delete`(*struct* bt\_le\_ext\_adv \**adv*)[Â¶](#_CPPv420bt_le_ext_adv_deleteP13bt_le_ext_adv "Permalink to this definition")
:   Delete advertising set.

    Delete advertising set. This will free up the advertising set and make it possible to create a new advertising set.

    Returns
    :   Zero on success or (negative) error code otherwise.

uint8\_t `bt_le_ext_adv_get_index`(*struct* bt\_le\_ext\_adv \**adv*)[Â¶](#_CPPv423bt_le_ext_adv_get_indexP13bt_le_ext_adv "Permalink to this definition")
:   Get array index of an advertising set.

    This function is used to map bt\_adv to index of an array of advertising sets. The array has CONFIG\_BT\_EXT\_ADV\_MAX\_ADV\_SET elements.

    Parameters
    :   **adv** â Advertising set.

    Returns
    :   Index of the advertising set object. The range of the returned value is 0..CONFIG\_BT\_EXT\_ADV\_MAX\_ADV\_SET-1

int `bt_le_ext_adv_get_info`(*const* *struct* bt\_le\_ext\_adv \**adv*, *struct* [bt\_le\_ext\_adv\_info](#_CPPv418bt_le_ext_adv_info "bt_le_ext_adv_info") \**info*)[Â¶](#_CPPv422bt_le_ext_adv_get_infoPK13bt_le_ext_advP18bt_le_ext_adv_info "Permalink to this definition")
:   Get advertising set info.

    Parameters
    :   * **adv** â Advertising set object
        * **info** â Advertising set info object

    Returns
    :   Zero on success or (negative) error code on failure.

int `bt_le_per_adv_set_param`(*struct* bt\_le\_ext\_adv \**adv*, *const* *struct* [bt\_le\_per\_adv\_param](#_CPPv419bt_le_per_adv_param "bt_le_per_adv_param") \**param*)[Â¶](#_CPPv423bt_le_per_adv_set_paramP13bt_le_ext_advPK19bt_le_per_adv_param "Permalink to this definition")
:   Set or update the periodic advertising parameters.

    The periodic advertising parameters can only be set or updated on an extended advertisement set which is neither scannable, connectable nor anonymous.

    Parameters
    :   * **adv** â Advertising set object.
        * **param** â Advertising parameters.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_set_data`(*const* *struct* bt\_le\_ext\_adv \**adv*, *const* *struct* [bt\_data](#_CPPv47bt_data "bt_data") \**ad*, size\_t *ad\_len*)[Â¶](#_CPPv422bt_le_per_adv_set_dataPK13bt_le_ext_advPK7bt_data6size_t "Permalink to this definition")
:   Set or update the periodic advertising data.

    The periodic advertisement data can only be set or updated on an extended advertisement set which is neither scannable, connectable nor anonymous.

    Parameters
    :   * **adv** â Advertising set object.
        * **ad** â Advertising data.
        * **ad\_len** â Advertising data length.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_start`(*struct* bt\_le\_ext\_adv \**adv*)[Â¶](#_CPPv419bt_le_per_adv_startP13bt_le_ext_adv "Permalink to this definition")
:   Starts periodic advertising.

    Enabling the periodic advertising can be done independently of extended advertising, but both periodic advertising and extended advertising shall be enabled before any periodic advertising data is sent. The periodic advertising and extended advertising can be enabled in any order.

    Once periodic advertising has been enabled, it will continue advertising until [bt\_le\_per\_adv\_stop()](#group__bt__gap_1ga1b15206fc552d597c12af369d48ff7d5) has been called, or if the advertising set is deleted by [bt\_le\_ext\_adv\_delete()](#group__bt__gap_1ga62310a27f7fea925dfcf3abd7c454787). Calling [bt\_le\_ext\_adv\_stop()](#group__bt__gap_1ga1c864c4b183f9a86c9f70a11471c5b15) will not stop the periodic advertising.

    Parameters
    :   **adv** â Advertising set object.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_stop`(*struct* bt\_le\_ext\_adv \**adv*)[Â¶](#_CPPv418bt_le_per_adv_stopP13bt_le_ext_adv "Permalink to this definition")
:   Stops periodic advertising.

    Disabling the periodic advertising can be done independently of extended advertising. Disabling periodic advertising will not disable extended advertising.

    Parameters
    :   **adv** â Advertising set object.

    Returns
    :   Zero on success or (negative) error code otherwise.

uint8\_t `bt_le_per_adv_sync_get_index`(*struct* bt\_le\_per\_adv\_sync \**per\_adv\_sync*)[Â¶](#_CPPv428bt_le_per_adv_sync_get_indexP18bt_le_per_adv_sync "Permalink to this definition")
:   Get array index of an periodic advertising sync object.

    This function is get the index of an array of periodic advertising sync objects. The array has CONFIG\_BT\_PER\_ADV\_SYNC\_MAX elements.

    Parameters
    :   **per\_adv\_sync** â The periodic advertising sync object.

    Returns
    :   Index of the periodic advertising sync object. The range of the returned value is 0..CONFIG\_BT\_PER\_ADV\_SYNC\_MAX-1

int `bt_le_per_adv_sync_get_info`(*struct* bt\_le\_per\_adv\_sync \**per\_adv\_sync*, *struct* [bt\_le\_per\_adv\_sync\_info](#_CPPv423bt_le_per_adv_sync_info "bt_le_per_adv_sync_info") \**info*)[Â¶](#_CPPv427bt_le_per_adv_sync_get_infoP18bt_le_per_adv_syncP23bt_le_per_adv_sync_info "Permalink to this definition")
:   Get periodic adv sync information.

    Parameters
    :   * **per\_adv\_sync** â Periodic advertising sync object.
        * **info** â Periodic advertising sync info object

    Returns
    :   Zero on success or (negative) error code on failure.

*struct* bt\_le\_per\_adv\_sync \*`bt_le_per_adv_sync_lookup_addr`(*const* bt\_addr\_le\_t \**adv\_addr*, uint8\_t *sid*)[Â¶](#_CPPv430bt_le_per_adv_sync_lookup_addrPK12bt_addr_le_t7uint8_t "Permalink to this definition")
:   Look up an existing periodic advertising sync object by advertiser address.

    Parameters
    :   * **adv\_addr** â Advertiser address.
        * **sid** â The advertising set ID.

    Returns
    :   Periodic advertising sync object or NULL if not found.

int `bt_le_per_adv_sync_create`(*const* *struct* [bt\_le\_per\_adv\_sync\_param](#_CPPv424bt_le_per_adv_sync_param "bt_le_per_adv_sync_param") \**param*, *struct* bt\_le\_per\_adv\_sync \*\**out\_sync*)[Â¶](#_CPPv425bt_le_per_adv_sync_createPK24bt_le_per_adv_sync_paramPP18bt_le_per_adv_sync "Permalink to this definition")
:   Create a periodic advertising sync object.

    Create a periodic advertising sync object that can try to synchronize to periodic advertising reports from an advertiser. Scan shall either be disabled or extended scan shall be enabled.

    Parameters
    :   * **param** â **[in]** Periodic advertising sync parameters.
        * **out\_sync** â **[out]** Periodic advertising sync object on.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_sync_delete`(*struct* bt\_le\_per\_adv\_sync \**per\_adv\_sync*)[Â¶](#_CPPv425bt_le_per_adv_sync_deleteP18bt_le_per_adv_sync "Permalink to this definition")
:   Delete periodic advertising sync.

    Delete the periodic advertising sync object. Can be called regardless of the state of the sync. If the syncing is currently syncing, the syncing is cancelled. If the sync has been established, it is terminated. The periodic advertising sync object will be invalidated afterwards.

    If the state of the sync object is syncing, then a new periodic advertising sync object may not be created until the controller has finished canceling this object.

    Parameters
    :   **per\_adv\_sync** â The periodic advertising sync object.

    Returns
    :   Zero on success or (negative) error code otherwise.

void `bt_le_per_adv_sync_cb_register`(*struct* [bt\_le\_per\_adv\_sync\_cb](#_CPPv421bt_le_per_adv_sync_cb "bt_le_per_adv_sync_cb") \**cb*)[Â¶](#_CPPv430bt_le_per_adv_sync_cb_registerP21bt_le_per_adv_sync_cb "Permalink to this definition")
:   Register periodic advertising sync callbacks.

    Adds the callback structure to the list of callback structures for periodic adverising syncs.

    This callback will be called for all periodic advertising sync activity, such as synced, terminated and when data is received.

    Parameters
    :   **cb** â Callback struct. Must point to memory that remains valid.

int `bt_le_per_adv_sync_recv_enable`(*struct* bt\_le\_per\_adv\_sync \**per\_adv\_sync*)[Â¶](#_CPPv430bt_le_per_adv_sync_recv_enableP18bt_le_per_adv_sync "Permalink to this definition")
:   Enables receiving periodic advertising reports for a sync.

    If the sync is already receiving the reports, -EALREADY is returned.

    Parameters
    :   **per\_adv\_sync** â The periodic advertising sync object.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_sync_recv_disable`(*struct* bt\_le\_per\_adv\_sync \**per\_adv\_sync*)[Â¶](#_CPPv431bt_le_per_adv_sync_recv_disableP18bt_le_per_adv_sync "Permalink to this definition")
:   Disables receiving periodic advertising reports for a sync.

    If the sync report receiving is already disabled, -EALREADY is returned.

    Parameters
    :   **per\_adv\_sync** â The periodic advertising sync object.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_sync_transfer`(*const* *struct* bt\_le\_per\_adv\_sync \**per\_adv\_sync*, *const* *struct* bt\_conn \**conn*, uint16\_t *service\_data*)[Â¶](#_CPPv427bt_le_per_adv_sync_transferPK18bt_le_per_adv_syncPK7bt_conn8uint16_t "Permalink to this definition")
:   Transfer the periodic advertising sync information to a peer device.

    This will allow another device to quickly synchronize to the same periodic advertising train that this device is currently synced to.

    Parameters
    :   * **per\_adv\_sync** â The periodic advertising sync to transfer.
        * **conn** â The peer device that will receive the sync information.
        * **service\_data** â Application service data provided to the remote host.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_set_info_transfer`(*const* *struct* bt\_le\_ext\_adv \**adv*, *const* *struct* bt\_conn \**conn*, uint16\_t *service\_data*)[Â¶](#_CPPv431bt_le_per_adv_set_info_transferPK13bt_le_ext_advPK7bt_conn8uint16_t "Permalink to this definition")
:   Transfer the information about a periodic advertising set.

    This will allow another device to quickly synchronize to periodic advertising set from this device.

    Parameters
    :   * **adv** â The periodic advertising set to transfer info of.
        * **conn** â The peer device that will receive the information.
        * **service\_data** â Application service data provided to the remote host.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_sync_transfer_subscribe`(*const* *struct* bt\_conn \**conn*, *const* *struct* [bt\_le\_per\_adv\_sync\_transfer\_param](#_CPPv433bt_le_per_adv_sync_transfer_param "bt_le_per_adv_sync_transfer_param") \**param*)[Â¶](#_CPPv437bt_le_per_adv_sync_transfer_subscribePK7bt_connPK33bt_le_per_adv_sync_transfer_param "Permalink to this definition")
:   Subscribe to periodic advertising sync transfers (PASTs).

    Sets the parameters and allow other devices to transfer periodic advertising syncs.

    Parameters
    :   * **conn** â The connection to set the parameters for. If NULL default parameters for all connections will be set. Parameters set for specific connection will always have precedence.
        * **param** â The periodic advertising sync transfer parameters.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_sync_transfer_unsubscribe`(*const* *struct* bt\_conn \**conn*)[Â¶](#_CPPv439bt_le_per_adv_sync_transfer_unsubscribePK7bt_conn "Permalink to this definition")
:   Unsubscribe from periodic advertising sync transfers (PASTs).

    Remove the parameters that allow other devices to transfer periodic advertising syncs.

    Parameters
    :   **conn** â The connection to remove the parameters for. If NULL default parameters for all connections will be removed. Unsubscribing for a specific device, will still allow other devices to transfer periodic advertising syncs.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_list_add`(*const* bt\_addr\_le\_t \**addr*, uint8\_t *sid*)[Â¶](#_CPPv422bt_le_per_adv_list_addPK12bt_addr_le_t7uint8_t "Permalink to this definition")
:   Add a device to the periodic advertising list.

    Add peer device LE address to the periodic advertising list. This will make it possibly to automatically create a periodic advertising sync to this device.

    Parameters
    :   * **addr** â Bluetooth LE identity address.
        * **sid** â The advertising set ID. This value is obtained from the [bt\_le\_scan\_recv\_info](#structbt__le__scan__recv__info) in the scan callback.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_list_remove`(*const* bt\_addr\_le\_t \**addr*, uint8\_t *sid*)[Â¶](#_CPPv425bt_le_per_adv_list_removePK12bt_addr_le_t7uint8_t "Permalink to this definition")
:   Remove a device from the periodic advertising list.

    Removes peer device LE address from the periodic advertising list.

    Parameters
    :   * **addr** â Bluetooth LE identity address.
        * **sid** â The advertising set ID. This value is obtained from the [bt\_le\_scan\_recv\_info](#structbt__le__scan__recv__info) in the scan callback.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_per_adv_list_clear`(void)[Â¶](#_CPPv424bt_le_per_adv_list_clearv "Permalink to this definition")
:   Clear the periodic advertising list.

    Clears the entire periodic advertising list.

    Returns
    :   Zero on success or (negative) error code otherwise.

int `bt_le_scan_start`(*const* *struct* [bt\_le\_scan\_param](#_CPPv416bt_le_scan_param "bt_le_scan_param") \**param*, bt\_le\_scan\_cb\_t *cb*)[Â¶](#_CPPv416bt_le_scan_startPK16bt_le_scan_param15bt_le_scan_cb_t "Permalink to this definition")
:   Start (LE) scanning.

    Start LE scanning with given parameters and provide results through the specified callback.

    Parameters
    :   * **param** â Scan parameters.
        * **cb** â Callback to notify scan results. May be NULL if callback registration through [bt\_le\_scan\_cb\_register](#group__bt__gap_1gad7c2f18de7f67b73ccc0e7813f48f096) is preferred.

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error.

    Note

    The LE scanner by default does not use the Identity Address of the local device when @option{CONFIG\_BT\_PRIVACY} is disabled. This is to prevent the active scanner from disclosing the identity information when requesting additional information from advertisers. In order to enable directed advertiser reports then @option{CONFIG\_BT\_SCAN\_WITH\_IDENTITY} must be enabled.

int `bt_le_scan_stop`(void)[Â¶](#_CPPv415bt_le_scan_stopv "Permalink to this definition")
:   Stop (LE) scanning.

    Stops ongoing LE scanning.

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error.

void `bt_le_scan_cb_register`(*struct* [bt\_le\_scan\_cb](#_CPPv413bt_le_scan_cb "bt_le_scan_cb") \**cb*)[Â¶](#_CPPv422bt_le_scan_cb_registerP13bt_le_scan_cb "Permalink to this definition")
:   Register scanner packet callbacks.

    Adds the callback structure to the list of callback structures that monitors scanner activity.

    This callback will be called for all scanner activity, regardless of what API was used to start the scanner.

    Parameters
    :   **cb** â Callback struct. Must point to memory that remains valid.

void `bt_le_scan_cb_unregister`(*struct* [bt\_le\_scan\_cb](#_CPPv413bt_le_scan_cb "bt_le_scan_cb") \**cb*)[Â¶](#_CPPv424bt_le_scan_cb_unregisterP13bt_le_scan_cb "Permalink to this definition")
:   Unregister scanner packet callbacks.

    Remove the callback structure from the list of scanner callbacks.

    Parameters
    :   **cb** â Callback struct. Must point to memory that remains valid.

int `bt_le_whitelist_add`(*const* bt\_addr\_le\_t \**addr*)[Â¶](#_CPPv419bt_le_whitelist_addPK12bt_addr_le_t "Permalink to this definition")
:   Add device (LE) to whitelist.

    Add peer device LE address to the whitelist.

    Parameters
    :   **addr** â Bluetooth LE identity address.

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error.

    Note

    The whitelist cannot be modified when an LE role is using the whitelist, i.e advertiser or scanner using a whitelist or automatic connecting to devices using whitelist.

int `bt_le_whitelist_rem`(*const* bt\_addr\_le\_t \**addr*)[Â¶](#_CPPv419bt_le_whitelist_remPK12bt_addr_le_t "Permalink to this definition")
:   Remove device (LE) from whitelist.

    Remove peer device LE address from the whitelist.

    Parameters
    :   **addr** â Bluetooth LE identity address.

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error.

    Note

    The whitelist cannot be modified when an LE role is using the whitelist, i.e advertiser or scanner using a whitelist or automatic connecting to devices using whitelist.

int `bt_le_whitelist_clear`(void)[Â¶](#_CPPv421bt_le_whitelist_clearv "Permalink to this definition")
:   Clear whitelist.

    Clear all devices from the whitelist.

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error.

    Note

    The whitelist cannot be modified when an LE role is using the whitelist, i.e advertiser or scanner using a whitelist or automatic connecting to devices using whitelist.

int `bt_le_set_chan_map`(uint8\_t *chan\_map*[5])[Â¶](#_CPPv418bt_le_set_chan_mapAL5E_7uint8_t "Permalink to this definition")
:   Set (LE) channel map.

    Parameters
    :   **chan\_map** â Channel map.

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error.

void `bt_data_parse`(*struct* net\_buf\_simple \**ad*, bool (\**func*)(*struct* [bt\_data](#_CPPv47bt_data "bt_data") \*data, void \*user\_data), void \**user\_data*, )[Â¶](#_CPPv413bt_data_parseP14net_buf_simplePFbP7bt_dataPvEPv "Permalink to this definition")
:   Helper for parsing advertising (or EIR or OOB) data.

    A helper for parsing the basic data types used for Extended Inquiry Response (EIR), Advertising Data (AD), and OOB data blocks. The most common scenario is to call this helper on the advertising data received in the callback that was given to [bt\_le\_scan\_start()](#group__bt__gap_1gac5e19c26b53a08dadb8efa7ecc692ad6).

    Parameters
    :   * **ad** â Advertising data as given to the bt\_le\_scan\_cb\_t callback.
        * **func** â Callback function which will be called for each element thatâs found in the data. The callback should return true to continue parsing, or false to stop parsing.
        * **user\_data** â User data to be passed to the callback.

int `bt_le_oob_get_local`(uint8\_t *id*, *struct* [bt\_le\_oob](#_CPPv49bt_le_oob "bt_le_oob") \**oob*)[Â¶](#_CPPv419bt_le_oob_get_local7uint8_tP9bt_le_oob "Permalink to this definition")
:   Get local LE Out of Band (OOB) information.

    This function allows to get local information that are useful for Out of Band pairing or connection creation.

    If privacy @option{CONFIG\_BT\_PRIVACY} is enabled this will result in generating new Resolvable Private Address (RPA) that is valid for @option{CONFIG\_BT\_RPA\_TIMEOUT} seconds. This address will be used for advertising started by [bt\_le\_adv\_start](#group__bt__gap_1gad2e3caef88d52d720e8e4d21df767b02), active scanning and connection creation.

    Parameters
    :   * **id** â **[in]** Local identity, in most cases BT\_ID\_DEFAULT.
        * **oob** â **[out]** LE OOB information

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error.

    Note

    If privacy is enabled the RPA cannot be refreshed in the following cases:

    * Creating a connection in progress, wait for the connected callback. In addition when extended advertising @option{CONFIG\_BT\_EXT\_ADV} is not enabled or not supported by the controller:
    * Advertiser is enabled using a Random Static Identity Address for a different local identity.
    * The local identity conflicts with the local identity used by other roles.

int `bt_le_ext_adv_oob_get_local`(*struct* bt\_le\_ext\_adv \**adv*, *struct* [bt\_le\_oob](#_CPPv49bt_le_oob "bt_le_oob") \**oob*)[Â¶](#_CPPv427bt_le_ext_adv_oob_get_localP13bt_le_ext_advP9bt_le_oob "Permalink to this definition")
:   Get local LE Out of Band (OOB) information.

    This function allows to get local information that are useful for Out of Band pairing or connection creation.

    If privacy @option{CONFIG\_BT\_PRIVACY} is enabled this will result in generating new Resolvable Private Address (RPA) that is valid for @option{CONFIG\_BT\_RPA\_TIMEOUT} seconds. This address will be used by the advertising set.

    Parameters
    :   * **adv** â **[in]** The advertising set object
        * **oob** â **[out]** LE OOB information

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error.

    Note

    When generating OOB information for multiple advertising set all OOB information needs to be generated at the same time.

    Note

    If privacy is enabled the RPA cannot be refreshed in the following cases:

    * Creating a connection in progress, wait for the connected callback.

int `bt_br_discovery_start`(*const* *struct* [bt\_br\_discovery\_param](#_CPPv421bt_br_discovery_param "bt_br_discovery_param") \**param*, *struct* [bt\_br\_discovery\_result](#_CPPv422bt_br_discovery_result "bt_br_discovery_result") \**results*, size\_t *count*, bt\_br\_discovery\_cb\_t *cb*)[Â¶](#_CPPv421bt_br_discovery_startPK21bt_br_discovery_paramP22bt_br_discovery_result6size_t20bt_br_discovery_cb_t "Permalink to this definition")
:   Start BR/EDR discovery.

    Start BR/EDR discovery (inquiry) and provide results through the specified callback. When bt\_br\_discovery\_cb\_t is called it indicates that discovery has completed. If more inquiry results were received during session than fits in provided result storage, only ones with highest RSSI will be reported.

    Parameters
    :   * **param** â Discovery parameters.
        * **results** â Storage for discovery results.
        * **count** â Number of results in storage. Valid range: 1-255.
        * **cb** â Callback to notify discovery results.

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error

int `bt_br_discovery_stop`(void)[Â¶](#_CPPv420bt_br_discovery_stopv "Permalink to this definition")
:   Stop BR/EDR discovery.

    Stops ongoing BR/EDR discovery. If discovery was stopped by this call results wonât be reported

    Returns
    :   Zero on success or error code otherwise, positive in case of protocol error or negative (POSIX) in case of stack internal error.

int `bt_br_oob_get_local`(*struct* [bt\_br\_oob](#_CPPv49bt_br_oob "bt_br_oob") \**oob*)[Â¶](#_CPPv419bt_br_oob_get_localP9bt_br_oob "Permalink to this definition")
:   Get BR/EDR local Out Of Band information.

    This function allows to get local controller information that are useful for Out Of Band pairing or connection creation process.

    Parameters
    :   **oob** â Out Of Band information

int `bt_br_set_discoverable`(bool *enable*)[Â¶](#_CPPv422bt_br_set_discoverableb "Permalink to this definition")
:   Enable/disable set controller in discoverable state.

    Allows make local controller to listen on INQUIRY SCAN channel and responds to devices making general inquiry. To enable this state itâs mandatory to first be in connectable state.

    Parameters
    :   **enable** â Value allowing/disallowing controller to become discoverable.

    Returns
    :   Negative if fail set to requested state or requested state has been already set. Zero if done successfully.

int `bt_br_set_connectable`(bool *enable*)[Â¶](#_CPPv421bt_br_set_connectableb "Permalink to this definition")
:   Enable/disable set controller in connectable state.

    Allows make local controller to be connectable. It means the controller start listen to devices requests on PAGE SCAN channel. If disabled also resets discoverability if was set.

    Parameters
    :   **enable** â Value allowing/disallowing controller to be connectable.

    Returns
    :   Negative if fail set to requested state or requested state has been already set. Zero if done successfully.

int `bt_unpair`(uint8\_t *id*, *const* bt\_addr\_le\_t \**addr*)[Â¶](#_CPPv49bt_unpair7uint8_tPK12bt_addr_le_t "Permalink to this definition")
:   Clear pairing information.

    Parameters
    :   * **id** â Local identity (mostly just BT\_ID\_DEFAULT).
        * **addr** â Remote address, NULL or BT\_ADDR\_LE\_ANY to clear all remote devices.

    Returns
    :   0 on success or negative error value on failure.

void `bt_foreach_bond`(uint8\_t *id*, void (\**func*)(*const* *struct* [bt\_bond\_info](#_CPPv412bt_bond_info "bt_bond_info") \*info, void \*user\_data), void \**user\_data*, )[Â¶](#_CPPv415bt_foreach_bond7uint8_tPFvPK12bt_bond_infoPvEPv "Permalink to this definition")
:   Iterate through all existing bonds.

    Parameters
    :   * **id** â Local identity (mostly just BT\_ID\_DEFAULT).
        * **func** â Function to call for each bond.
        * **user\_data** â Data to pass to the callback function.
