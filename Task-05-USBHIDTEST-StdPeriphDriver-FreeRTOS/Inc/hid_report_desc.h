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
//-----------------------------------------
// some convenience definitions for modifier keys
#define KEYCODE_MOD_LEFT_CONTROL	0x01
#define KEYCODE_MOD_LEFT_SHIFT		0x02
#define KEYCODE_MOD_LEFT_ALT		0x04
#define KEYCODE_MOD_LEFT_GUI		0x08
#define KEYCODE_MOD_RIGHT_CONTROL	0x10
#define KEYCODE_MOD_RIGHT_SHIFT		0x20
#define KEYCODE_MOD_RIGHT_ALT		0x40
#define KEYCODE_MOD_RIGHT_GUI		0x80

// some more keycodes
#define KEYCODE_LEFT_CONTROL	0xE0
#define KEYCODE_LEFT_SHIFT		0xE1
#define KEYCODE_LEFT_ALT		0xE2
#define KEYCODE_LEFT_GUI		0xE3
#define KEYCODE_RIGHT_CONTROL	0xE4
#define KEYCODE_RIGHT_SHIFT		0xE5
#define KEYCODE_RIGHT_ALT		0xE6
#define KEYCODE_RIGHT_GUI		0xE7
#define KEYCODE_1				0x1E
#define KEYCODE_2				0x1F
#define KEYCODE_3				0x20
#define KEYCODE_4				0x21
#define KEYCODE_5				0x22
#define KEYCODE_6				0x23
#define KEYCODE_7				0x24
#define KEYCODE_8				0x25
#define KEYCODE_9				0x26
#define KEYCODE_0				0x27
#define KEYCODE_A				0x04
#define KEYCODE_B				0x05
#define KEYCODE_C				0x06
#define KEYCODE_D				0x07
#define KEYCODE_E				0x08
#define KEYCODE_F				0x09
#define KEYCODE_G				0x0A
#define KEYCODE_H				0x0B
#define KEYCODE_I				0x0C
#define KEYCODE_J				0x0D
#define KEYCODE_K				0x0E
#define KEYCODE_L				0x0F
#define KEYCODE_M				0x10
#define KEYCODE_N				0x11
#define KEYCODE_O				0x12
#define KEYCODE_P				0x13
#define KEYCODE_Q				0x14
#define KEYCODE_R				0x15
#define KEYCODE_S				0x16
#define KEYCODE_T				0x17
#define KEYCODE_U				0x18
#define KEYCODE_V				0x19
#define KEYCODE_W				0x1A
#define KEYCODE_X				0x1B
#define KEYCODE_Y				0x1C
#define KEYCODE_Z				0x1D
#define KEYCODE_COMMA			0x36
#define KEYCODE_PERIOD			0x37
#define KEYCODE_MINUS			0x2D
#define KEYCODE_EQUAL			0x2E
#define KEYCODE_BACKSLASH		0x31
#define KEYCODE_SQBRAK_LEFT		0x2F
#define KEYCODE_SQBRAK_RIGHT	0x30
#define KEYCODE_SLASH			0x38
#define KEYCODE_F1				0x3A
#define KEYCODE_F2				0x3B
#define KEYCODE_F3				0x3C
#define KEYCODE_F4				0x3D
#define KEYCODE_F5				0x3E
#define KEYCODE_F6				0x3F
#define KEYCODE_F7				0x40
#define KEYCODE_F8				0x41
#define KEYCODE_F9				0x42
#define KEYCODE_F10				0x43
#define KEYCODE_F11				0x44
#define KEYCODE_F12				0x45
#define KEYCODE_APP				0x65
#define KEYCODE_ENTER			0x28
#define KEYCODE_BACKSPACE		0x2A
#define KEYCODE_ESC				0x29
#define KEYCODE_TAB				0x2B
#define KEYCODE_SPACE			0x2C
#define KEYCODE_INSERT			0x49
#define KEYCODE_HOME			0x4A
#define KEYCODE_PAGE_UP			0x4B
#define KEYCODE_DELETE			0x4C
#define KEYCODE_END				0x4D
#define KEYCODE_PAGE_DOWN		0x4E
#define KEYCODE_PRINTSCREEN		0x46
#define KEYCODE_ARROW_RIGHT		0x4F
#define KEYCODE_ARROW_LEFT		0x50
#define KEYCODE_ARROW_DOWN		0x51
#define KEYCODE_ARROW_UP		0x52

// multimedia keys
#define MMKEY_KB_VOL_UP			0x80 // do not use
#define MMKEY_KB_VOL_DOWN		0x81 // do not use
#define MMKEY_VOL_UP			0xE9
#define MMKEY_VOL_DOWN			0xEA
#define MMKEY_SCAN_NEXT_TRACK	0xB5
#define MMKEY_SCAN_PREV_TRACK	0xB6
#define MMKEY_STOP				0xB7
#define MMKEY_PLAYPAUSE			0xCD
#define MMKEY_MUTE				0xE2
#define MMKEY_BASSBOOST			0xE5
#define MMKEY_LOUDNESS			0xE7
#define MMKEY_KB_EXECUTE		0x74
#define MMKEY_KB_HELP			0x75
#define MMKEY_KB_MENU			0x76
#define MMKEY_KB_SELECT			0x77
#define MMKEY_KB_STOP			0x78
#define MMKEY_KB_AGAIN			0x79
#define MMKEY_KB_UNDO			0x7A
#define MMKEY_KB_CUT			0x7B
#define MMKEY_KB_COPY			0x7C
#define MMKEY_KB_PASTE			0x7D
#define MMKEY_KB_FIND			0x7E
#define MMKEY_KB_MUTE			0x7F // do not use

// system control keys
#define SYSCTRLKEY_POWER		0x01
#define SYSCTRLKEY_SLEEP		0x02
#define SYSCTRLKEY_WAKE			0x03

//-----------------------------------------
struct keyboardHID_t {
      uint8_t id;
      uint8_t modifier;
      uint8_t reserved;
      uint8_t keycodes[6];
 };

struct mouseHID_t {
      uint8_t id;
      uint8_t buttons;
      int8_t x;       //-127~127
      int8_t y;       //-127~127
      int8_t wheel;   //-127~127
  };
struct joystickHID_t {
	  uint8_t id;
      int8_t left_analog_x; //x: -127~127
      int8_t left_analog_y; //y: -127~127
      int8_t right_analog_x; //z: -127~127
      int8_t right_analog_y; //Rx: -127~127
      uint8_t buttons[2];
  };

//JOYSTICK

#define HID_REPORT_DESC_SIZE 47+54+46+46
__ALIGN_BEGIN static uint8_t HID_ReportDesc[HID_REPORT_DESC_SIZE]  __ALIGN_END = {
//Keyboard 47 bytes
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
//Mouse 54 bytes
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
//JoyStick1 46 bytes
			 0x05, 0x01,                   // USAGE_PAGE (Generic Desktop)
			 0x09, 0x05,                   // USAGE (Game Pad)
			 0xa1, 0x01,                   // COLLECTION (Application)
			 0xa1, 0x00,                   //   COLLECTION (Physical)
			 0x85, 0x03,                   //     REPORT_ID (3)
			 0x09, 0x30,                   //     USAGE (X)
			 0x09, 0x31,                   //     USAGE (Y)
			 0x09, 0x32,                   //     USAGE (Z)
			 0x09, 0x33,                   //     USAGE (Rx)
			 0x15, 0x81,                   //     LOGICAL_MINIMUM (-127)
			 0x25, 0x7f,                   //     LOGICAL_MAXIMUM (127)
			 0x75, 0x08,                   //     REPORT_SIZE (8)
			 0x95, 0x04,                   //     REPORT_COUNT (4)
			 0x81, 0x02,                   //     INPUT (Data,Var,Abs)
			 0x05, 0x09,                   //     USAGE_PAGE (Button)
			 0x19, 0x01,                   //     USAGE_MINIMUM (Button 1)
			 0x29, 0x10,                   //     USAGE_MAXIMUM (Button 16)
			 0x15, 0x00,                   //     LOGICAL_MINIMUM (0)
			 0x25, 0x01,                   //     LOGICAL_MAXIMUM (1)
			 0x75, 0x01,                   //     REPORT_SIZE (1)
			 0x95, 0x10,                   //     REPORT_COUNT (16)
			 0x81, 0x02,                   //     INPUT (Data,Var,Abs)
			 0xc0,                         //   END_COLLECTION
			 0xc0,                         // END_COLLECTION
//JoyStick2 46 bytes
			 0x05, 0x01,                   // USAGE_PAGE (Generic Desktop)
			 0x09, 0x05,                   // USAGE (Game Pad)
			 0xa1, 0x01,                   // COLLECTION (Application)
			 0xa1, 0x00,                   //   COLLECTION (Physical)
			 0x85, 0x04,                   //     REPORT_ID (4)
			 0x09, 0x30,                   //     USAGE (X)
			 0x09, 0x31,                   //     USAGE (Y)
			 0x09, 0x32,                   //     USAGE (Z)
			 0x09, 0x33,                   //     USAGE (Rx)
			 0x15, 0x81,                   //     LOGICAL_MINIMUM (-127)
			 0x25, 0x7f,                   //     LOGICAL_MAXIMUM (127)
			 0x75, 0x08,                   //     REPORT_SIZE (8)
			 0x95, 0x04,                   //     REPORT_COUNT (4)
			 0x81, 0x02,                   //     INPUT (Data,Var,Abs)
			 0x05, 0x09,                   //     USAGE_PAGE (Button)
			 0x19, 0x01,                   //     USAGE_MINIMUM (Button 1)
			 0x29, 0x10,                   //     USAGE_MAXIMUM (Button 16)
			 0x15, 0x00,                   //     LOGICAL_MINIMUM (0)
			 0x25, 0x01,                   //     LOGICAL_MAXIMUM (1)
			 0x75, 0x01,                   //     REPORT_SIZE (1)
			 0x95, 0x10,                   //     REPORT_COUNT (16)
			 0x81, 0x02,                   //     INPUT (Data,Var,Abs)
			 0xc0,                         //   END_COLLECTION
			 0xc0,                         // END_COLLECTION

};

#endif
