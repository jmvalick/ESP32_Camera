#include "wifi.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_wifi_netif.h"


// // Debug message tag
// static const char *TAG = "wifi";

// // Static global variables
// static esp_netif_t *s_wifi_netif = NULL;                // Handler for net-if instance
// static EventGroupHandle_t s_wifi_event_group = NULL;    // Handler for the FreeRTOS event group
// static wifi_netif_driver_t s_wifi_driver = NULL;        // Handler for the wifi driver


// /*
// * Private function prototypes
// */
// static void on_wifi_event(
//     void *arg,
//     esp_event_base_t event_base,
//     int32_t event_id,
//     void *event_data
// );

// static void on_ip_event(
//     void *arg,
//     esp_event_base_t event_base,
//     int32_t event_id,
//     void *event_data
// );

// static void wifi_start(
//     void *esp_netif,
//     esp_event_base_t event_base,
//     int32_t event_id,
//     void *event_data
// );


// /*
// * Private function definitions
// */
// // Event handler: wifi events
// static void on_wifi_event(
//     void *arg,
//     esp_event_base_t event_base,
//     int32_t event_id,
//     void *event_data
// ) {
//     // Determine event type
//     switch(event_id) {

//         // Wifi station started
//         case WIFI_EVENT_STA_START:
//             if (s_wifi_netif != NULL) {
//                 wifi_start(s_wifi_netif, event_base, event_id, event_data);
//             }
//             break;

//         // Connected to wifi
//         case WIFI_EVENT_STA_CONNECTED:
//             if (s_wifi_netif == NULL) {
//                 ESP_LOGE(TAG, "Wifi not started: interface handle is NULL");
//                 return;
//             }

//             // AP info
//             wifi_event_sta_connected_t *event_sta_connected = 
//                 (wifi_event_sta_connected_t *)event_data;
//             ESP_LOGI(TAG, "Connected to AP");
//             ESP_LOGI(TAG, "  SSID: %s", (char*)event_sta_connected->ssid);
//             ESP_LOGI(TAG, "  Channel: %d", event_sta_connected->authmode);
//             ESP_LOGI(TAG, "  Auth mode: %d", event_sta_connected->authmode);
//             ESP_LOGI(TAG, "  AID: %d", event_sta_connected->aid);

//             // Register interface receive callback
//             wifi_netif_driver_t driver = esp_netif_get_io_driver(s_wifi_netif);
//             if(!esp_wifi_is_if_ready_when_started(driver)) {
//                 esp_err_t esp_ret = esp_wifi_register_if_rxcb(
//                     driver,
//                     esp_netif_receive,
//                     s_wifi_netif
//                 );
//                 if(esp_ret != ESP_OK) {
//                     ESP_LOGE(TAG, "Failed to regiseter wifi RX callback");
//                     return;
//                 }
//             }

//             // Set up the wifi interface and start DHCP process
//             esp_netif_action_connected(
//                 s_wifi_netif,
//                 event_base,
//                 event_id,
//                 event_data
//             );

//             // Set Wifi connected bit
//             xEventGroupSetBits(s_wifi_event_group, WIFI_STA_CONNECTED_BIT);

// #if CONFIG_WIFI_STA_CONNECT_IPV6 || CONFIG_WIFI_STA_CONNECT_UNSPECIFIED
//             // Request IPv6 link-local address for the inferface
//             esp_ret = esp_netif_create_ip6_linklocal(s_wifi_netif);
//             if (esp_ret != ESP_OK) {
//                 ESP_LOGE(TAG, "Failed to create IPv6 link-local address");
//             }
// #endif
//             break;

//         // Disconnected from wifi
//         case WIFI_EVENT_STA_DISCONNECTED:
//             if (s_wifi_netif != NULL) {
//                 esp_netif_action_disconnected(
//                     s_wifi_netif,
//                     event_base,
//                     event_id,
//                     event_data
//                 );
//             }
//             xEventGroupClearBits(s_wifi_event_group, WIFI_STA_CONNECTED_BIT);
// #if CONFIG_WIFI_STA_AUTO_RECONNECT
//             ESP_LOGI(TAG, "Attempting to reconnect...");
//             wifi_sta_reconnect();
// #endif
//             break;
//     }
// }

// // Event handler: received IP address on wifi interface from DHCP
// static void on_ip_event(
//     void *arg,
//     esp_event_base_t event_base,
//     int32_t event_id,
//     void *event_data
// ) {
//     esp_err_t esp_ret;

//     // Determine event type
//     switch(event_id) {

// #if CONFIG_WIFI_STA_CONNECT_IPV4 || CONFIG_WIFI_STA_CONNECT_UNSPECIFIED
//         case IP_EVENT_STA_GOT_IP:

//             // Make sure we have a valid interface handle
//             if (s_wifi_netif == NULL) {
//                 ESP_LOGE(TAG, "On obtain IPv4 addr: Interface handle is NULL");
//                 return;
//             }

//             // Notify the wifi driver that we got an IP address
//             esp_ret = esp_wifi_internal_set_sta_ip();
//             if (esp_ret != ESP_OK) {
//                 ESP_LOGE(TAG, "Failed to notify wifi driver of IP address");
//             }

//             // Set IPv4 obtained bit
//             xEventGroupSetBits(s_wifi_event_group, WIFI_STA_IPV4_OBTAINED_BIT);

//             // Print IP address
//             ip_event_got_ip_t *event_ip = (ip_event_got_ip_t *)event_data;
//             esp_netif_ip_info_t *ip_info = &event_ip->ip_info;
//             ESP_LOGI(TAG, "Wifi IPv4 address obtained");
//             ESP_LOGI(TAG, "  IP address: ", IPSTR, IP2STR(&ip_info->ip));
//             ESP_LOGI(TAG, "  Netmask: ", IPSTR, IP2STR(&ip_info->netmask));
//             ESP_LOGI(TAG, "  Gateway: ", IPSTR, IP2STR(&ip_info->gw));

//             break;
// #endif
// #if CONFIG_WIFI_STA_CONNECT_IPV6 || CONFIG_WIFI_STA_CONNECT_UNSPECIFIED
//         case IP_EVENT_STA_GOT_IP6:

//             // Make sure we have a valid interface handle
//             if (s_wifi_netif == NULL) {
//                 ESP_LOGE(TAG, "On obtain IPv4 addr: Interface handle is NULL");
//                 return;
//             }

//             // Notify the wifi driver that we got an IP address
//             esp_ret = esp_wifi_internal_set_sta_ip();
//             if (esp_ret != ESP_OK) {
//                 ESP_LOGE(TAG, "Failed to notify wifi driver of IP address");
//             }

//             // Set IPv6 obtained bit
//             xEventGroupSetBits(s_wifi_event_group, WIFI_STA_IPV6_OBTAINED_BIT);

//             // Print IPv6 address
//             ip_event_got_ip6_t *event_ipv6 = (ip_event_got_ip6_t *)event_data;
//             esp_netif_ip6_info_t *ip6_info = &event_ipv6->ip6_info;
//             ESP_LOGI(TAG, "Wifi IPv6 address obtained");
//             ESP_LOGI(TAG, "  IPv6 address: " IPV6STR, IPV62STR(ip6_info->ip));

//             break;
// #endif

//             // Lost IP address
//             case IP_EVENT_STA_LOST_IP:
//                 xEventGroupClearBits(s_wifi_event_group, WIFI_STA_IPV4_OBTAINED_BIT);
//                 xEventGroupClearBits(s_wifi_event_group, WIFI_STA_IPV6_OBTAINED_BIT);
//                 ESP_LOGI(TAG, "Wifi lost IP address");
//                 break;

//             // Default case: do nothing
//             default:
//                 ESP_LOGI(TAG, "Unhandled IP event: %li", event_id);
//                 break;
//     }
// }

// // Set up the wifi interface and start DHCP process (called from on_wifi_event)
// static void wifi_start(
//     void *esp_netif,
//     esp_event_base_t event_base,
//     int32_t event_id,
//     void *event_data
// ) {
//     uint8_t mac_addr[6] = {0};
//     esp_err_t esp_ret;

//     // Get esp_netif driver handle
//     wifi_netif_driver_t driver = esp_netif_get_io_driver(esp_netif);
//     if (driver == NULL) {
//         ESP_LOGE(TAG, "Failed to get wifi driver handle");
//         return;
//     }

//     // Get MAC address of wifi interface
//     esp_ret = esp_wifi_get_if_mac(driver, mac_addr);
//     if (esp_ret != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to get MAC address");
//         return;
//     }

//     // Pring MAC adress
//     ESP_LOGI(
//         TAG, 
//         "WiFi MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
//         mac_addr[0], 
//         mac_addr[1], 
//         mac_addr[2], 
//         mac_addr[3], 
//         mac_addr[4], 
//         mac_addr[5]
//     );

//     // Register netstack buffer reference and free callback
//     esp_ret = esp_wifi_internal_reg_netstack_buf_cb(
//         esp_netif_netstack_buf_ref,
//         esp_netif_netstack_buf_free
//     );
//     if (esp_ret != ESP_OK) {
//         ESP_LOGE(
//             TAG,
//             "Error (%d): Netstack callback registration failed",
//             esp_ret
//         );
//         return;
//     }

//     // Set MAC address of the wifi interface
//     esp_ret = esp_netif_set_mac(esp_netif, mac_addr);
//     if (esp_ret != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to set MAC address");
//         return;
//     }

//     // Start the wifi interface
//     esp_netif_action_start(esp_netif, event_base, event_id, event_data);

//     // Connect to wifi
//     ESP_LOGI(TAG, "Connecting to %s...", CONFIG_WIFI_STA_SSID);
//     esp_ret = esp_wifi_connect();
//     if(esp_ret != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to connect to wifi");
//     }
// }


// /*
// * Private function definitions
// */
// // Initialize wifi in station (STA) mode
// esp_err_t wifi_sta_init(EventGroupHandle_t event_group) {
//     esp_err_t esp_ret;

//     ESP_LOGI(TAG, "Starting wifi in station mode...");

//     // Save the event group handle
//     if (event_group != NULL) {
//         s_wifi_event_group = event_group;
//     }
//     if (s_wifi_event_group == NULL) {
//         ESP_LOGE(TAG, "Event group handle in NULL");
//         return ESP_FAIL;
//     }
    
//     // Create default wifi network interface
//     //esp_netif_config_t netif_cfg = ESP_NETIF_DEFA


// }