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

struct mouseHID_t {
	  uint8_t id;
      uint8_t buttons;
      uint8_t x;
      uint8_t y;
  };
/*
struct joystickHID_t {
	  //uint8_t id;
      uint8_t left_trigger;  //0~31
      uint8_t right_trigger; //0~31
      uint8_t left_analog_x; //x: -127~127
      uint8_t left_analog_y; //y: -127~127
      uint8_t right_analog_x; //z: -127~127
      uint8_t right_analog_y; //Rx: -127~127
      uint16_t buttons;
  };
*/
struct joystickHID_t {
	  uint8_t id;
	  uint16_t buttons;
      int8_t left_analog_x; //x: -127~127
      int8_t left_analog_y; //y: -127~127
      int8_t right_analog_x; //z: -127~127
      int8_t right_analog_y; //Rx: -127~127
  };

//JOYSTICK

#define HID_REPORT_DESC_SIZE    197
//+52
__ALIGN_BEGIN static uint8_t HID_ReportDesc[HID_REPORT_DESC_SIZE]  __ALIGN_END = {
		   0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		    0x09, 0x06,                    // USAGE (Keyboard)
		    0xa1, 0x01,                    // COLLECTION (Application)
		    0x85, 0x01,                    //   REPORT_ID (1)
		    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
		    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
		    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
		    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
		    0x75, 0x01,                    //   REPORT_SIZE (1)
		    0x95, 0x08,                    //   REPORT_COUNT (8)
		    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
		    0x95, 0x01,                    //   REPORT_COUNT (1)
		    0x75, 0x08,                    //   REPORT_SIZE (8)
		    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
		    0x95, 0x06,                    //   REPORT_COUNT (6)
		    0x75, 0x08,                    //   REPORT_SIZE (8)
		    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
		    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
		    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
		    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
		    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
		    0xc0,                          // END_COLLECTION
		    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		    0x09, 0x02,                    // USAGE (Mouse)
		    0xa1, 0x01,                    // COLLECTION (Application)
		    0x09, 0x01,                    //   USAGE (Pointer)
		    0xa1, 0x00,                    //   COLLECTION (Physical)
		    0x85, 0x02,                    //     REPORT_ID (2)
		    0x05, 0x09,                    //     USAGE_PAGE (Button)
		    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
		    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
		    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
		    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
		    0x95, 0x03,                    //     REPORT_COUNT (3)
		    0x75, 0x01,                    //     REPORT_SIZE (1)
		    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		    0x95, 0x01,                    //     REPORT_COUNT (1)
		    0x75, 0x05,                    //     REPORT_SIZE (5)
		    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
		    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
		    0x09, 0x30,                    //     USAGE (X)
		    0x09, 0x31,                    //     USAGE (Y)
		    0x09, 0x38,                    //     USAGE (Wheel)
		    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
		    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
		    0x75, 0x08,                    //     REPORT_SIZE (8)
		    0x95, 0x03,                    //     REPORT_COUNT (3)
		    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
		    0xc0,                          //   END_COLLECTION
		    0xc0,                          // END_COLLECTION
		    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		    0x09, 0x05,                    // USAGE (Game Pad)
		    0xa1, 0x01,                    // COLLECTION (Application)
		    0xa1, 0x00,                    //   COLLECTION (Physical)
		    0x85, 0x03,                    //     REPORT_ID (3)
		    0x05, 0x09,                    //     USAGE_PAGE (Button)
		    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
		    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
		    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
		    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
		    0x95, 0x10,                    //     REPORT_COUNT (16)
		    0x75, 0x01,                    //     REPORT_SIZE (1)
		    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
		    0x09, 0x30,                    //     USAGE (X)
		    0x09, 0x31,                    //     USAGE (Y)
		    0x09, 0x32,                    //     USAGE (Z)
		    0x09, 0x33,                    //     USAGE (Rx)
		    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
		    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
		    0x75, 0x08,                    //     REPORT_SIZE (8)
		    0x95, 0x04,                    //     REPORT_COUNT (4)
		    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		    0xc0,                          //   END_COLLECTION
		    0xc0,                          // END_COLLECTION
		    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		    0x09, 0x05,                    // USAGE (Game Pad)
		    0xa1, 0x01,                    // COLLECTION (Application)
		    0xa1, 0x00,                    //   COLLECTION (Physical)
		    0x85, 0x04,                    //     REPORT_ID (4)
		    0x05, 0x09,                    //     USAGE_PAGE (Button)
		    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
		    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
		    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
		    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
		    0x95, 0x10,                    //     REPORT_COUNT (16)
		    0x75, 0x01,                    //     REPORT_SIZE (1)
		    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
		    0x09, 0x30,                    //     USAGE (X)
		    0x09, 0x31,                    //     USAGE (Y)
		    0x09, 0x32,                    //     USAGE (Z)
		    0x09, 0x33,                    //     USAGE (Rx)
		    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
		    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
		    0x75, 0x08,                    //     REPORT_SIZE (8)
		    0x95, 0x04,                    //     REPORT_COUNT (4)
		    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		    0xc0,                          //     END_COLLECTION
		    0xc0                           // END_COLLECTION
};


/*
//MOUSE
#define HID_REPORT_DESC_SIZE    52
__ALIGN_BEGIN static uint8_t HID_ReportDesc[HID_REPORT_DESC_SIZE]  __ALIGN_END = {
		 0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		    0x09, 0x02,                    // USAGE (Mouse)
		    0xa1, 0x01,                    // COLLECTION (Application)
		    0x09, 0x01,                    //   USAGE (Pointer)
		    0xa1, 0x00,                    //   COLLECTION (Physical)
			0x85, 0x01,                    //     Report ID (1)
		    0x05, 0x09,                    //     USAGE_PAGE (Button)
		    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
		    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
		    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
		    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)

		    0x95, 0x03,                    //     REPORT_COUNT (3)
		    0x75, 0x01,                    //     REPORT_SIZE (1)
		    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		    0x95, 0x01,                    //     REPORT_COUNT (1)
		    0x75, 0x05,                    //     REPORT_SIZE (5)

		    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
		    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
		    0x09, 0x30,                    //     USAGE (X)
		    0x09, 0x31,                    //     USAGE (Y)
		    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)

		    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
		    0x75, 0x08,                    //     REPORT_SIZE (8)
		    0x95, 0x02,                    //     REPORT_COUNT (2)
		    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
		    0xc0,                          //   END_COLLECTION
		    0xc0                           // END_COLLECTION
};
*/
/*
//KEYBOARD
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
*/
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
