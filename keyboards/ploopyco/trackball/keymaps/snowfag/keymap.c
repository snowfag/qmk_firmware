/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
 * Copyright 2020 snowfag <yuki@lolicon.eu>
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
#include <trackball.h>

bool marble_scroll = false;
bool horizontal_scroll = false;
bool wheel_h_scroll = false;
bool wheelscroll = false;
bool tab_switch = false;
bool inverted_horizontal_scroll = false;
extern uint16_t lastScroll;
extern uint16_t dpi_array[];

enum layer_names {
     Base,
     Fn,
     Fn2,
     Fn3,
     Fn4
 };

enum custom_keycodes {
    KC_BTLD = PLOOPY_SAFE_RANGE,
    KC_H_SCROLL,
    KC_IHS,
};

enum td_keycodes {
    td_fn,
    td_fn2,
};

typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    single_tap = 1,
    single_hold,
    double_tap,
    double_hold
};

uint8_t cur_dance(qk_tap_dance_state_t *state);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [Base] = LAYOUT( /* Base */
        KC_BTN1, KC_BTN3, KC_BTN2, TD(td_fn), TD(td_fn2)
    ),
    [Fn] = LAYOUT(
        KC_BTN1, C(KC_W), MEH(KC_F17), KC_TRNS, LT(Fn3, KC_F14)
    ),
    [Fn2] = LAYOUT(
        KC_BTN1, DPI_CONFIG, KC_BTN2, MO(Fn4), KC_TRNS
    ),
    [Fn3] = LAYOUT(
        HYPR(KC_F17), HYPR(KC_F18), KC_IHS, HYPR(KC_F20), KC_TRNS
    ),
    [Fn4] = LAYOUT(
        HYPR(KC_F21), KC_BTLD, KC_H_SCROLL, KC_TRNS, HYPR(KC_F23)
    )
};

uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return single_tap;
        else return single_hold;
    } else if (state->count == 2) {
        if (!state->pressed) return  double_tap;
        else return double_hold;
    }
    else return 8;
}

static tap fn_tap_state = {
    .is_press_action = true,
    .state = 0
};

void fn_finished(qk_tap_dance_state_t *state, void *user_data) {
    fn_tap_state.state = cur_dance(state);
    switch (fn_tap_state.state) {
        case single_tap:
            if (marble_scroll) {
                marble_scroll = 0;
                pmw_set_cpi(dpi_array[keyboard_config.dpi_config]);
            } else {
                marble_scroll = 1;
                pmw_set_cpi(TRACKBALL_SCROLL_DPI);
            }
            break;
        case single_hold:
            layer_clear();
            layer_on(Fn);
            break;
        case double_tap:
            wheel_h_scroll ^= 1;
            xprintf("Wheel horizontal scrolling: %u\n", wheel_h_scroll);
            break;
        case double_hold:
            layer_clear();
            layer_on(Fn3);
            break;
    }
}

void fn_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (fn_tap_state.state) {
        case single_hold:
            layer_off(Fn);
            break;
        case double_hold:
            layer_off(Fn3);
            break;
    }
}

static tap fn2_tap_state = {
    .is_press_action = true,
    .state = 0
};

void fn2_finished(qk_tap_dance_state_t *state, void *user_data) {
    fn2_tap_state.state = cur_dance(state);
    switch (fn2_tap_state.state) {
        case single_tap:
            break;
        case single_hold:
            layer_clear();
            layer_on(Fn2);
            break;
        case double_tap:
            wheelscroll ^= 1;
            xprintf("Wheel Scrolling: %u\n", wheelscroll);
            break;
        case double_hold:
            layer_clear();
            layer_on(Fn4);
            break;
    }
}

void fn2_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (fn2_tap_state.state) {
        case single_hold:
            layer_off(Fn2);
            break;
        case double_hold:
            layer_off(Fn4);
            break;
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [td_fn] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn_finished, fn_reset),
    [td_fn2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn2_finished, fn2_reset),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_BTLD:
            if (record->event.pressed) {
                bootloader_jump();
            }
            break;
        case KC_H_SCROLL:
            if (record->event.pressed) {
                horizontal_scroll ^= 1;
                xprintf("Ball Horizontal scrolling: %u\n", horizontal_scroll);
            }
            break;
        case KC_IHS:
            if (record->event.pressed) {
                inverted_horizontal_scroll ^=1;
                xprintf("Inverted Horizontal scrolling: %u\n", inverted_horizontal_scroll);
            }
            break;
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case Fn:
        marble_scroll = 1;
        tab_switch = 1;
        pmw_set_cpi(TRACKBALL_SCROLL_DPI);
        break;
    case Fn2:
        pmw_set_cpi(PRECISION_DPI);
        break;
    default:
        marble_scroll = 0;
        tab_switch = 0;
        pmw_set_cpi(dpi_array[keyboard_config.dpi_config]);
        break;
    }
    return state;
}
void process_wheel_user(report_mouse_t* mouse_report, int16_t h, int16_t v) {
    if (tab_switch) {
        if (v ==  255) {
            tap_code16(A(KC_PGDN));
        } else if (v == 1 ) {
            tap_code16(A(KC_PGUP));
        }
    } else if (wheel_h_scroll) {
        mouse_report->h = -v;
        mouse_report->v = h;
    } else if (wheelscroll) {
        mouse_report->h = h;
        mouse_report->v = v;
    } else {
        xprintf("v: %u\n", v);
        return;
    }
}
void process_mouse_user(report_mouse_t* mouse_report, int16_t x, int16_t y) {
    if (marble_scroll) {
        if (timer_elapsed(lastScroll) < TRACKBALL_SCROLL_DEBOUNCE) {
            return;
        }
        lastScroll = timer_read();
        if (horizontal_scroll) {
            if (inverted_horizontal_scroll) {
                mouse_report->h = -x;
            } else {
                mouse_report->h = x;
            }
        }
        mouse_report->v = -y;
    } else {
        mouse_report->x = x;
        mouse_report->y = y;
    }
}
void keyboard_post_init_user(void) {
    xprintf("Ball Horizontal scrolling: %u\nWheel Scrolling: %u\nWheel horizontal scrolling: %u\n", horizontal_scroll, wheelscroll, wheel_h_scroll);
}
