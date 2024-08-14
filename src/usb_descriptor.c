#include <stdint.h>
#include "debug.h"
#include "bsp/board_api.h"
#include "tusb.h"
#include "usb_descriptor.h"
#include "variables.h"

#define USB_VID   0xCafe
#define USB_PID   0x5678
#define USB_BCD   0x0200

tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = USB_BCD,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0101,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

uint8_t const desc_report[] = {
    HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                 ,
    HID_USAGE      ( HID_USAGE_DESKTOP_GAMEPAD  )                 ,
    HID_COLLECTION ( HID_COLLECTION_APPLICATION )                 ,
        /* 8 bit X, Y, Z, Rz, Rx, Ry (min -128, max 127 ) */ 
        HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ),
        HID_USAGE          ( HID_USAGE_DESKTOP_X                    ),
        // HID_USAGE          ( HID_USAGE_DESKTOP_Y                    ),
        // HID_USAGE          ( HID_USAGE_DESKTOP_Z                    ),
        // HID_USAGE          ( HID_USAGE_DESKTOP_RZ                   ),
        // HID_USAGE          ( HID_USAGE_DESKTOP_RX                   ),
        // HID_USAGE          ( HID_USAGE_DESKTOP_RY                   ),
        HID_LOGICAL_MIN    ( 0x80                                   ),
        HID_LOGICAL_MAX    ( 0x7f                                   ),
        HID_REPORT_COUNT   ( 1                                      ),
        HID_REPORT_SIZE    ( 8                                      ),
        HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),
        // /* 8 bit DPad/Hat Button Map  */ 
        // HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ),
        // HID_USAGE          ( HID_USAGE_DESKTOP_HAT_SWITCH           ),
        // HID_LOGICAL_MIN    ( 1                                      ),
        // HID_LOGICAL_MAX    ( 8                                      ),
        // HID_PHYSICAL_MIN   ( 0                                      ),
        // HID_PHYSICAL_MAX_N ( 315, 2                                 ),
        // HID_REPORT_COUNT   ( 1                                      ),
        // HID_REPORT_SIZE    ( 8                                      ),
        // HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),
        /* 32 bit Button Map */ 
        HID_USAGE_PAGE     ( HID_USAGE_PAGE_BUTTON                  ),
        HID_USAGE_MIN      ( 1                                      ),
        HID_USAGE_MAX      ( 32                                     ),
        HID_LOGICAL_MIN    ( 0                                      ),
        HID_LOGICAL_MAX    ( 1                                      ),
        HID_REPORT_COUNT   ( 32                                     ),
        HID_REPORT_SIZE    ( 1                                      ),
        HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),
    HID_COLLECTION_END 
};
// uint8_t const desc_report[] = {
//     TUD_HID_REPORT_DESC_GAMEPAD()
// };

#define  CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

uint8_t const desc_config[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, 0, 200),
    TUD_HID_DESCRIPTOR(0, 0, 0, sizeof(desc_report), 0x81, CFG_TUD_HID_EP_BUFSIZE, 1)
};



const uint16_t string_desc_lang[] = {
    4 | (3 << 8), // bLength & bDescriptorType
    0x411 // ja-JP
};

static uint16_t _desc_str[32 + 1];
#define STRING_DESCRIPTOR_MAX_LENGTH 32

const char *string_descriptors[] = {
    "",
    "Natane",
    "N's HID test device"
};

uint8_t const * tud_descriptor_device_cb(void) {
  return (uint8_t const *) &desc_device;
}

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    return desc_config;
}

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    size_t max_count;
    switch (index) {
    // lang
    case 0: {
        return string_desc_lang;
    }
    // serial
    case 3: {
        char serial_buf[32];
        sprintf(serial_buf, "%08x%08x%08x", *UID0, *UID1, *UID2);
        for (size_t i = 0; i < 32; i++) {
            char c = serial_buf[i];
            if (c == 0) {
                break;
            }
            _desc_str[i + 1] = c;
            max_count = i + 1;
        }
        break;
    }
    default: {
        // 境界チェック
        if (index > (sizeof(string_descriptors) / sizeof(string_descriptors[0]))) {
            return NULL;
        }
        const char *string_descriptor = string_descriptors[index];
        max_count = strlen(string_descriptor);
        for (size_t i = 0; i < max_count; i++) {
            _desc_str[i + 1] = string_descriptor[i];
        }
        break;
    }
    }
    _desc_str[0] = (uint16_t)(TUSB_DESC_STRING << 8 | (2 * max_count + 2));
    return _desc_str;
}

uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance) {
    return desc_report;
}

uint16_t tud_hid_get_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t* buffer,
    uint16_t reqlen) {
    printf("tud_hid_get_report_cb : instance : %d, id : %d, type : %d, len : %d", instance, report_id, report_type, reqlen);
    return 0;
}

void tud_hid_set_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t const* buffer,
    uint16_t bufsize) {
printf("tud_hid_set_report_cb : %d, %d, %d, %d", instance, report_id, report_type, bufsize);
}