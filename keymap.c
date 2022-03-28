/* Copyright 2021 Jay Greco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "action_layer.h"

// Define layer names

#define _BLEN 0
#define _ILL1 1
#define _PROC 2



// Define Macro names
enum custom_keycodes {
    PROG = SAFE_RANGE,
};



// Keymap with layers

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//Blender Layer
  [0] = LAYOUT(
             	                             LCTL(KC_S),	    KC_KP_PLUS,	    KC_KP_ENTER,
  KC_VOLD,  KC_VOLU,       TO(_ILL1),  	     KC_EQUAL,     	    KC_KP_MINUS,	KC_KP_0,
  KC_NO,    KC_NO,         KC_KP_DOT,	     KC_KP_9,       	KC_KP_6,		KC_KP_3,
  KC_NO,    KC_NO,         KC_KP_ASTERISK,   KC_KP_8,        	KC_KP_5,		KC_KP_2,
  KC_NO,    KC_NO,         KC_KP_SLASH,    	 KC_KP_7,       	KC_KP_4,		KC_KP_1
  ), 
    

  // Illustrator Base Layer
  [1] = LAYOUT(
             	                                              LCTL(KC_RBRC),      LCTL(KC_LBRC),    KC_I,
  LCTL(KC_MINS),       LCTL(KC_EQUAL),      TO(_PROC),        KC_M,               KC_X,             LCTL(KC_V),
  KC_TRNS,              KC_TRNS,            KC_DEL,    	      KC_R,               KC_Y,             LCTL(KC_C),
  KC_TRNS,              KC_TRNS,            LCTL(KC_K),       KC_A,               KC_P,             LCTL(KC_Z),
  KC_TRNS,              KC_TRNS,            KC_K,    	      KC_V,               KC_N,             KC_Q
  ),
    
     
  // Procreate Layer
  [2] = LAYOUT(
              	                        LGUI(KC_RBRC),  LGUI(KC_LBRC), 	LGUI(KC_J), 
  KC_LBRC,  KC_RBRC,   TO(_BLEN),   	LGUI(KC_U),     KC_X,          	LGUI(KC_X), 
  KC_NO,    KC_NO,     LGUI(KC_K), 	    KC_V,           KC_C,          	LGUI(KC_C), 
  KC_NO,    KC_NO,     KC_L,       	    LGUI(KC_D),     KC_E,          	LGUI(KC_Z), 
  KC_NO,    KC_NO,     LGUI(KC_0), 	    KC_S,           KC_B,          	KC_SPC
  ),

};  


// OLED Screen

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_0; }


// Include bitmaps that show the mapped keys on each layer

#include "l1_blender_text.c"
#include "l2_illustrator_text.c"
#include "l3_procreate_text.c"

// Render the correct image for each layer

bool oled_task_user(void) {

    switch (get_highest_layer(layer_state)) {
        case _BLEN:
            render_blen_keymap_1();
            break;
        case _ILL1:
            render_ai_keymap_1();
            break;
        case _PROC:
            render_proc_keymap_1();
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    return false;
}



#endif

void matrix_init_user(void) { matrix_init_remote_kb(); }

void matrix_scan_user(void) { matrix_scan_remote_kb(); }


// Define macros


// Rotary encoder

bool encoder_update_user(uint8_t index, bool clockwise) {
    //reversed VOLD and VOLU due to issue with clockwise = volume down for unknown reasons
    if (clockwise) {
        tap_code(KC_VOLD);
    } else {
        tap_code(KC_VOLU);
    }
     return true;
}

void led_set_kb(uint8_t usb_led) {
    if (usb_led & (1 << USB_LED_NUM_LOCK))
        set_bitc_LED(LED_DIM);
    else
        set_bitc_LED(LED_OFF);
}




// RGB LIGHTING LAYERS


void eeconfig_init_user(void) {  // EEPROM is getting reset!
  // use the non noeeprom versions, to write these values to EEPROM too
  rgblight_enable(); // Enable RGB by default      
  rgblight_sethsv(235,255,255);  // Set it to pink by default
  rgblight_mode(RGBLIGHT_MODE_STATIC_GRADIENT + 9);  // Set it to static gradient
}

layer_state_t layer_state_set_user(layer_state_t state) {
  rgblight_config_t rgblight_config;
  switch(biton32(state)) {
  default:
    // Blender, purple/blue gradient
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(188,149,219);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_GRADIENT + 8);
    break;
  case _ILL1:
    // Illustrator Layer 1, orange
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(10,255,255);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    break;
  case _PROC:
    // Procreate, pink/purple gradient
    //Read RGB Light State
    rgblight_config.raw = eeconfig_read_rgblight();
    //If enabled, set white
    if (rgblight_config.enable) {
		rgblight_sethsv_noeeprom(235,255,255);
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_GRADIENT + 9);
	} else { //Otherwise go back to disabled
		rgblight_disable_noeeprom();
	}
    break;
}
return state;
}
