#include "SocketHelpers.h"

#include "utility/wl_definitions.h"
#include <zephyr/net/wifi_mgmt.h>

#define NET_EVENT_WIFI_MASK                                                                        \
	(NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT |                        \
	 NET_EVENT_WIFI_AP_ENABLE_RESULT | NET_EVENT_WIFI_AP_DISABLE_RESULT |                      \
	 NET_EVENT_WIFI_AP_STA_CONNECTED | NET_EVENT_WIFI_AP_STA_DISCONNECTED |                    \
	 NET_EVENT_WIFI_SCAN_RESULT)


class WiFiClass: public NetworkInterface
{
public:
    WiFiClass() {}
    ~WiFiClass() {}

    bool begin(const char* ssid, const char* passphrase, wl_enc_type security = ENC_TYPE_UNKNOWN, bool blocking = false) {
        sta_iface = net_if_get_wifi_sta();
        netif = sta_iface;
        sta_config.ssid = (const uint8_t *)ssid;
        sta_config.ssid_length = strlen(ssid);
        sta_config.psk = (const uint8_t *)passphrase;
        sta_config.psk_length = strlen(passphrase);
        // TODO: change these fields with scan() results
        sta_config.security = WIFI_SECURITY_TYPE_PSK;
        sta_config.channel = WIFI_CHANNEL_ANY;
        sta_config.band = WIFI_FREQ_BAND_2_4_GHZ;
	    sta_config.bandwidth = WIFI_FREQ_BANDWIDTH_20MHZ;

        int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, sta_iface, &sta_config,
                sizeof(struct wifi_connect_req_params));
        if (ret) {
            return false;
        }

        NetworkInterface::begin(false, NET_EVENT_WIFI_MASK);
        if (blocking) {
            net_mgmt_event_wait_on_iface(sta_iface, NET_EVENT_WIFI_AP_STA_CONNECTED, NULL, NULL, NULL, K_FOREVER);
        }

        return true;
    }

    bool beginAP(char* ssid, char* passphrase, int channel = WIFI_CHANNEL_ANY, bool blocking = false) {
        if (ap_iface != NULL) {
            return false;
        }
        ap_iface = net_if_get_wifi_sap();
        netif = ap_iface;
        ap_config.ssid = (const uint8_t *)ssid;
        ap_config.ssid_length = strlen(ssid);
        ap_config.psk = (const uint8_t *)passphrase;
        ap_config.psk_length = strlen(passphrase);
        ap_config.security = WIFI_SECURITY_TYPE_PSK;
        ap_config.channel = channel;
        ap_config.band = WIFI_FREQ_BAND_2_4_GHZ;
	    ap_config.bandwidth = WIFI_FREQ_BANDWIDTH_20MHZ;

        int ret = net_mgmt(NET_REQUEST_WIFI_AP_ENABLE, ap_iface, &ap_config,
                sizeof(struct wifi_connect_req_params));

        if (ret) {
            return false;
        }

        enable_dhcpv4_server(ap_iface);

        if (blocking) {
            net_mgmt_event_wait_on_iface(ap_iface, NET_EVENT_WIFI_AP_ENABLE_RESULT, NULL, NULL, NULL, K_FOREVER);
        }

        return true;
    }

    int status() {
        struct wifi_iface_status status = { 0 };

        if (net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, netif, &status,
                sizeof(struct wifi_iface_status))) {
            return WL_NO_SHIELD;
        }

	    if (status.state >= WIFI_STATE_ASSOCIATED) {
            return WL_CONNECTED;
        } else {
            return WL_DISCONNECTED;
        }
        return WL_NO_SHIELD;
    }

    int8_t scanNetworks() {
        // TODO: borrow code from mbed core for scan results handling
    }

private:
    struct net_if *sta_iface = nullptr;
    struct net_if *ap_iface = nullptr;

    struct wifi_connect_req_params ap_config;
    struct wifi_connect_req_params sta_config;
};

extern WiFiClass WiFi;
