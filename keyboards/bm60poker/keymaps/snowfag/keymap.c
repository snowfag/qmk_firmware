/* Copyright 2020 snowfag
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

bool kana_tog = false;

enum layers {
    _qwerty,
    _voip,
    _gaymen,
    _fn,
    _fn2,
    _rgb,
    _spcfn
};


enum keycodes {
    gaymen = SAFE_RANGE,
    qwerty,
    voip
};

enum td_keycodes {
    TD_CLIP_UPLOAD,
    TD_SS_WINDOW,
    TD_SS_FULL,
    TD_SS_REGION,
    TD_CAPS_KANA,
    TD_SPCFN,
};

typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD
};

uint8_t cur_dance(qk_tap_dance_state_t *state);

//#define KC_SPFN LT(_spcfn, KC_SPC)
#define KC_SPFN TD(TD_SPCFN)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_qwerty] = LAYOUT_60_ansi(
        KC_GESC,        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,         KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        TD(TD_CAPS_KANA), KC_A,  KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSPO,                 KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSPC,
        KC_LCTL,        KC_LGUI, KC_LALT,                            KC_SPFN,                            KC_RALT, KC_RCTL, MO(_fn),   MO(_rgb)
    ),
    [_voip] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______,
        KC_F13,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______
    ),
    [_gaymen] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______,
        KC_F13,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______,                            KC_SPACE,                            _______, _______, _______, _______
    ),
    [_fn] = LAYOUT_60_ansi(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        _______, _______, KC_UP,   _______, _______, _______, _______, _______, KC_INS,  _______, KC_PSCR, KC_SLCK, KC_PAUS, RESET,
        _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGUP,          _______,
        _______,          _______, _______, _______, _______, _______, NK_TOGG, _______, _______, KC_END,  KC_PGDN,          _______,
        qwerty,  voip, gaymen,                             _______,                            _______, _______, _______, _______
    ),
    [_fn2] = LAYOUT_60_ansi(
        KC_GRV,  KC_F1,     KC_F2,   KC_F3,   KC_F4,   KC_F5,     KC_F6,   KC_F7,   KC_F8,      KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        _______, _______,   _______, _______, _______, A(KC_F22), KC_JYEN, _______, KC_INS,     _______, KC_PSCR, KC_SLCK, KC_PAUS, RESET,
        _______, A(KC_F23), _______, _______, _______, _______,   _______, _______, C(S(KC_K)), _______, KC_HOME, KC_PGUP,          A(KC_F24),
        _______,            _______, _______, KC_CALC, _______,   _______, _______, _______,    _______, KC_END,  KC_PGDN,          _______,
        _______, _______,   _______,                              _______,                               _______, _______, _______, _______
    ),
    [_rgb] = LAYOUT_60_ansi(
        _______, KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,  KC_DEL,
        _______, RGB_TOG, _______, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, RGB_MOD, _______, _______, _______, RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, RGB_SPI, RGB_SPD, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______
    ),
    [_spcfn] = LAYOUT_60_ansi(
        _______, TD(TD_CLIP_UPLOAD), TD(TD_SS_WINDOW), TD(TD_SS_FULL), TD(TD_SS_REGION), HYPR(KC_F18), LCAG(KC_F18), LCAG(KC_F19),  LCAG(KC_F20), LCAG(KC_F21), LCAG(KC_F22), LCAG(KC_F23), LCAG(KC_F24), KC_DEL,
        _______, _______, KC_PGUP, _______, _______, KC_LBRC, KC_RBRC,  _______, KC_UP,   _______, _______, _______,  _______, _______,
        KC_CAPS, KC_HOME, KC_PGDN, KC_END,  _______, KC_LCBR, KC_RCBR,  KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______,          _______,
        _______,          _______, C(KC_X), C(S(KC_C)), S(KC_INS), _______, _______,  _______, KC_MPRV, KC_MNXT, KC_MPLY,           _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______
    )
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(TD_CLIP_UPLOAD):
            return TAPPING_TERM + 100;
        case TD(TD_CAPS_KANA):
            return TAPPING_TERM + 25;
        case KC_SPFN:
            return 120;
        default:
            return TAPPING_TERM;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case qwerty:
            if (record->event.pressed) {
                layer_clear();
                layer_on(_qwerty);
                set_single_persistent_default_layer(_qwerty);
            }
            return false;
            break;
        case voip:
            if (record->event.pressed) {
                layer_clear();
                layer_on(_voip);
            }
            return false;
            break;
        case gaymen:
            if (record->event.pressed) {
                layer_clear();
                layer_on(_gaymen);
            }
            return false;
            break;
    }
    return true;
}

uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (!state->pressed) return  DOUBLE_TAP;
        else return DOUBLE_HOLD;
    }
    else return 8;
}

static tap spcfn_tap_state = {
    .is_press_action = true,
    .state = 0
};

void spcfn_finished(qk_tap_dance_state_t *state, void *user_data) {
    spcfn_tap_state.state = cur_dance(state);
    switch (spcfn_tap_state.state) {
        case SINGLE_TAP:
            tap_code(KC_SPC);
            break;
        case SINGLE_HOLD:
            layer_clear();
            layer_on(_spcfn);
            break;
        case DOUBLE_TAP:
            tap_code(KC_SPC);
            tap_code(KC_SPC);
            break;
        case DOUBLE_HOLD:
            tap_code(KC_SPC);
            layer_clear();
            layer_on(_spcfn);
    }
}

void spcfn_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (spcfn_tap_state.state) {
        case SINGLE_HOLD:
            layer_off(_spcfn);
            break;
        case DOUBLE_HOLD:
            layer_off(_spcfn);
            break;
    }
}

static tap caps_kana_tap_state = {
    .is_press_action = true,
    .state = 0
};

void dance_caps_kana_finished(qk_tap_dance_state_t *state, void *user_data) {
    caps_kana_tap_state.state = cur_dance(state);
    switch (caps_kana_tap_state.state) {
        case SINGLE_TAP:
            tap_code(KC_CAPS);
            break;
        case SINGLE_HOLD:
            layer_clear();
            layer_on(_fn2);
            break;
        case DOUBLE_TAP:
            if (kana_tog) {
                unregister_code(KC_KANA);
                kana_tog = 0;
            } else {
                register_code(KC_KANA);
                kana_tog = 1;
            }
            break;
        case DOUBLE_HOLD:
            register_code(KC_KANA);
            kana_tog = 1;
            break;
    }
}

void dance_caps_kana_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (caps_kana_tap_state.state) {
        case SINGLE_HOLD:
            layer_off(_fn2);
            break;
        case DOUBLE_HOLD:
            unregister_code(KC_KANA);
            kana_tog = 0;
            break;
    }
}

void dance_clup_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
      case 1:
        register_code16(A(KC_F14));
        break;
      case 2:
        register_code16(LCA(KC_F14));
        break;
      case 3:
        register_code16(LCAG(KC_F14));
        break;
      case 4:
        register_code16(HYPR(KC_F14));
        break;
    }
}

void dance_clup_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
          unregister_code16(A(KC_F14));
          break;
        case 2:
            unregister_code16(LCA(KC_F14));
            break;
        case 3:
            unregister_code16(LCAG(KC_F14));
            break;
        case 4:
            unregister_code16(HYPR(KC_F14));
            break;
        }
}

void dance_swin_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code16(A(KC_F15));
    } else {
        register_code16(LCA(KC_F15));
    }
}

void dance_swin_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_code16(A(KC_F15));
    } else {
        unregister_code16(LCA(KC_F15));
    }
}

void dance_sful_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code16(A(KC_F16));
    } else {
        register_code16(LCA(KC_F16));
    }
}

void dance_sful_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_code16(A(KC_F16));
    } else {
        unregister_code16(LCA(KC_F16));
    }
}

void dance_sreg_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code16(A(KC_F17));
    } else {
        register_code16(LCA(KC_F17));
    }
}

void dance_sreg_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_code16(A(KC_F17));
    } else {
        unregister_code16(LCA(KC_F17));
    }
}

// All tap dance functions would go here. Only showing this one.
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_CLIP_UPLOAD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_clup_finished, dance_clup_reset),
    [TD_SS_WINDOW] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_swin_finished, dance_swin_reset),
    [TD_SS_FULL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_sful_finished, dance_sful_reset),
    [TD_SS_REGION] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_sreg_finished, dance_sreg_reset),
    [TD_CAPS_KANA] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_caps_kana_finished, dance_caps_kana_reset),
    [TD_SPCFN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, spcfn_finished, spcfn_reset),
};

void rgb_matrix_layer_helper (uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
            rgb_matrix_set_color( i, red, green, blue );
        }
    }
}


void rgb_matrix_indicators_kb(void) {
	if (!g_suspend_state && rgb_matrix_config.enable) {
        switch (get_highest_layer(layer_state)) {
            //case _qwerty:
            //    rgb_matrix_set_color(26, 0xFF, 0x00, 0x00);
            //    break;
            case _spcfn:
                rgb_matrix_set_color(22, 0xFF, 0xFF, 0xFF);
                rgb_matrix_set_color(35, 0xFF, 0xFF, 0xFF);
                rgb_matrix_set_color(36, 0xFF, 0xFF, 0xFF);
                rgb_matrix_set_color(37, 0xFF, 0xFF, 0xFF);
                rgb_matrix_set_color(16, 0xFF, 0xFF, 0x00);
                rgb_matrix_set_color(29, 0xFF, 0xFF, 0x00);
                rgb_matrix_set_color(30, 0xFF, 0xFF, 0x00);
                rgb_matrix_set_color(31, 0xFF, 0xFF, 0x00);
                rgb_matrix_set_color(49, 0x00, 0xFF, 0x00);
                rgb_matrix_set_color(50, 0x00, 0xFF, 0x00);
                rgb_matrix_set_color(51, 0x00, 0xFF, 0x00);
                break;
            case _gaymen:
                rgb_matrix_set_color(16, 0xFF, 0x00, 0x00);
                rgb_matrix_set_color(29, 0xFF, 0x00, 0x00);
                rgb_matrix_set_color(30, 0xFF, 0x00, 0x00);
                rgb_matrix_set_color(31, 0xFF, 0x00, 0x00);
                break;
            case _fn2:
                rgb_matrix_set_color(40, 0xFF, 0x00, 0x00);
                rgb_matrix_set_color(28, 0xFF, 0xFF, 0xFF);
                break;
        }
        led_t led_state = host_keyboard_led_state();
        if (kana_tog && !led_state.caps_lock) {
            rgb_matrix_set_color(28, 0x00, 0xFF, 0x00);
        } else if (!kana_tog && led_state.caps_lock) {
            rgb_matrix_set_color(28, 0xFF, 0x00, 0x00);
        } else if (kana_tog && led_state.caps_lock) {
            rgb_matrix_set_color(28, 0xFF, 0xFF, 0x00);
        }
    }
}
