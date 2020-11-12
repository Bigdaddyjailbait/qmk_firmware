#include QMK_KEYBOARD_H
#include "analog.h"
#include "qmk_midi.h"
extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layers {
_BL = 0,
_NV,
_FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap _BL: (Base Layer) Default Numpad Layer
 * ,-------------------.
 * | NV | /  | *  |BK/FN|
 * |----|----|----|-----|
 * | 7  | 8  | 9  |  -  |
 * |----|----|----|-----|
 * | 4  | 5  | 6  |  +  |
 * |----|----|----|-----|
 * | 1  | 2  | 3  |     |
 * |----|----|----| En  |
 * |    0    | .  |     |
 * `--------------------'
 */
 [_BL] = LAYOUT(
   TG(_NV),  KC_PSLS,  KC_PAST,   LT(_FN, KC_BSPC), \
   KC_7,    KC_8,    KC_9,     KC_PMNS, \
   KC_4,    KC_5,    KC_6,     KC_PPLS, \
   KC_1,    KC_2,    KC_3,     KC_PENT, \
   KC_0,    KC_0,  KC_DOT,   KC_PENT
 ),

/* Keymap _NV: Navigation layer
 * ,-------------------.
 * |INS |HOME|PGUP|    |
 * |----|----|----|----|
 * |DEL |END |PGDN|    |
 * |----|----|----|----|
 * |    | UP |    |    |
 * |----|----|----|----|
 * |LEFT|DOWN|RIGH|    |
 * |----|----|----|    |
 * |         |    |    |
 * `-------------------'
 */
  [_NV] = LAYOUT(
    KC_INS,   KC_HOME,  KC_PGUP,   TG(_NV), \
    KC_DEL,   KC_END,   KC_PGDN,   XXXXXXX, \
    XXXXXXX,  KC_UP,    XXXXXXX,   XXXXXXX, \
    KC_LEFT,  KC_DOWN,  KC_RGHT,   XXXXXXX, \
    XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX
  ),

/* Keymap _FN: RGB Function Layer
 * ,-------------------.
 * |RMOD|RGBP|RTOG| FN |
 * |----|----|----|----|
 * |HUD |HUI |    |    |
 * |----|----|----|----|
 * |SAD |SAI |    |    |
 * |----|----|----|----|
 * |VAD |VAS |    |    |
 * |----|----|----|    |
 * |RST |    |    |    |
 * `-------------------'
 */
  [_FN] = LAYOUT(
    RGB_MOD,  RGB_M_P,  RGB_TOG,   _______, \
    RGB_HUD,  RGB_HUI,  XXXXXXX,   XXXXXXX, \
    RGB_SAD,  RGB_SAI,  XXXXXXX,   XXXXXXX, \
    RGB_VAD,  RGB_VAI,  XXXXXXX,   XXXXXXX, \
    RESET,    XXXXXXX,  XXXXXXX,   XXXXXXX
  ),
};

void encoder_update_user(uint8_t index, bool clockwise) {
  if (clockwise) {
      tap_code(KC_VOLU);
  } else {
      tap_code(KC_VOLD);
  }
}

uint8_t divisor = 0;

void slider(void) {
    if (divisor++) { // only run the slider function 1/256 times it's called
        return;
    }

    midi_send_cc(&midi_device, 2, 0x3E, 0x7F - (analogReadPin(SLIDER_PIN) >> 3));
}

void matrix_scan_user(void) {
    slider();
}

// Old yampad OLED code for layers and stuff
#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;  // flips the display 270 degrees
}

void oled_task_user(void) {

  oled_write_P(PSTR(" U D "), false);
  oled_write_P(PSTR(" N E "), false);
  oled_write_P(PSTR(" G S "), false);
  oled_write_P(PSTR(" O I "), false);
  oled_write_P(PSTR(" D G "), false);
  oled_write_P(PSTR(" L N "), false);
  oled_write_P(PSTR(" Y   "), false);

  // Host Keyboard Layer Status
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("Layer"), false);
  switch (biton32(layer_state)) {
    case _BL:
      oled_write_ln_P(PSTR(" BAS"), false);
      break;
    case _NV:
      oled_write_ln_P(PSTR(" NAV"), false);
      break;
    case _FN:
      oled_write_ln_P(PSTR(" RGB"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR(" UND"), false);
  }

  // Host Keyboard LED Status
  uint8_t led_usb_state = host_keyboard_leds();
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("Stats"), false);
  oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("num:*") : PSTR("num:."), false);
  oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("cap:*") : PSTR("cap:."), false);
  oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("scr:*") : PSTR("scr:."), false);

}

#endif


// New OLED code for showing logo from glcdfont.c file
#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;  // flips the display 270 degrees
}

void render_logo(void) {
  static const char PROGMEM crkbd_logo[] = {
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
      0};
  oled_write_P(crkbd_logo, false);
}

void oled_task_user(void) {
    render_logo();
}

#endif
