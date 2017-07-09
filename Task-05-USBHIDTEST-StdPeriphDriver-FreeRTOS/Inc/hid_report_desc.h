#ifndef __HIDREPORTDESC_H__
#define __HIDREPORTDESC_H__
#include "usb_conf.h"

// USB keyboard codes
#define USB_HID_MODIFIER_LEFT_CTRL   0x01
#define USB_HID_MODIFIER_LEFT_SHIFT  0x02
#define USB_HID_MODIFIER_LEFT_ALT    0x04
#define USB_HID_MODIFIER_LEFT_GUI    0x08 // (Win/Apple/Meta)
#define USB_HID_MODIFIER_RIGHT_CTRL  0x10
#define USB_HID_MODIFIER_RIGHT_SHIFT 0x20
#define USB_HID_MODIFIER_RIGHT_ALT   0x40
#define USB_HID_MODIFIER_RIGHT_GUI   0x80
#define USB_HID_KEY_L     0x0F
#define USB_HID_KEY_A     0x04
#define USB_HID_KEY_R     0x15

struct mediaHID_t {
    uint8_t id;
    uint8_t keys;
  };

struct keyboardHID_t {
      uint8_t id;
      uint8_t modifiers;
      uint8_t key1;
      uint8_t key2;
      uint8_t key3;
  };


#define HID_REPORT_DESC_SIZE    41
__ALIGN_BEGIN static uint8_t HID_ReportDesc[HID_REPORT_DESC_SIZE]  __ALIGN_END = {

 0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
 0x09, 0x06,        // Usage (Keyboard)
 0xA1, 0x01,        // Collection (Application)
 0x85, 0x01,        //   Report ID (1)
 0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
 0x75, 0x01,        //   Report Size (1)
 0x95, 0x08,        //   Report Count (8)
 0x19, 0xE0,        //   Usage Minimum (0xE0, Keyboard LeftControl)
 0x29, 0xE7,        //   Usage Maximum (0xE7, Keyboard Right GUI)
 0x15, 0x00,        //   Logical Minimum (0)
 0x25, 0x01,        //   Logical Maximum (1)
 0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
 0x95, 0x03,        //   Report Count (3)
 0x75, 0x08,        //   Report Size (8)
 0x15, 0x00,        //   Logical Minimum (0)
 0x25, 0x64,        //   Logical Maximum (100)
 0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
 0x19, 0x00,        //   Usage Minimum (0x00,0)
 0x29, 0x65,        //   Usage Maximum (0x65, 101)
 0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
 0xC0,              // End Collection
};

/*
#define HID_REPORT_DESC_SIZE    78
__ALIGN_BEGIN static uint8_t HID_ReportDesc[HID_REPORT_DESC_SIZE]  __ALIGN_END = {
  // 78 bytes
  0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
  0x09, 0x06,        // Usage (Keyboard)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x01,        //   Report ID (1)
  0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x08,        //   Report Count (8)
  0x19, 0xE0,        //   Usage Minimum (0xE0, Keyboard LeftControl)
  0x29, 0xE7,        //   Usage Maximum (0xE7, Keyboard Right GUI)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x95, 0x03,        //   Report Count (3)
  0x75, 0x08,        //   Report Size (8)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x64,        //   Logical Maximum (100)
  0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
  0x19, 0x00,        //   Usage Minimum (0x00,0)
  0x29, 0x65,        //   Usage Maximum (0x65, 101)
  0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0xC0,              // End Collection
  0x05, 0x0C,        // Usage Page (Consumer)
  0x09, 0x01,        // Usage (Consumer Control)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x02,        //   Report ID (2)
  0x05, 0x0C,        //   Usage Page (Consumer)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x08,        //   Report Count (8)
  0x09, 0xB5,        //   Usage (Scan Next Track)
  0x09, 0xB6,        //   Usage (Scan Previous Track)
  0x09, 0xB7,        //   Usage (Stop)
  0x09, 0xB8,        //   Usage (Eject)
  0x09, 0xCD,        //   Usage (Play/Pause)
  0x09, 0xE2,        //   Usage (Mute)
  0x09, 0xE9,        //   Usage (Volume Increment)
  0x09, 0xEA,        //   Usage (Volume Decrement)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0xC0,              // End Collection
};
*/
#endif
