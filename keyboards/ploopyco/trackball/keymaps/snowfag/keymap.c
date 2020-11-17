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
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [Base] = LAYOUT( /* Base */
        KC_BTN1, KC_BTN3, KC_BTN2,
          LT(Fn, KC_WBAK), LT(Fn2, KC_WFWD)
    ),
    [Fn] = LAYOUT(
        C(KC_PGDN), C(KC_W), C(KC_PGUP),
          KC_TRNS, LT(Fn3, KC_F14)
      ),
    [Fn2] = LAYOUT(
        KC_BTN3, DPI_CONFIG, HYPR(KC_F16),
          MO(Fn4), KC_TRNS
    ),
    [Fn3] = LAYOUT(
        HYPR(KC_F17), HYPR(KC_F18), HYPR(KC_F19),
          HYPR(KC_F20), KC_TRNS
    ),
    [Fn4] = LAYOUT(
        HYPR(KC_F21), KC_BTLD, KC_H_SCROLL,
          KC_TRNS, HYPR(KC_F23)
    )
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
                xprintf("Horizontal scroll: %u\n", horizontal_scroll);
            }
            break;
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case Fn:
        marble_scroll = 1;
        pmw_set_cpi(TRACKBALL_SCROLL_DPI);
        break;
    default:
        marble_scroll = 0;
        pmw_set_cpi(dpi_array[keyboard_config.dpi_config]);
        break;
    }
    return state;
}
void process_mouse_user(report_mouse_t* mouse_report, int16_t x, int16_t y) {
    if (marble_scroll) {
        if (timer_elapsed(lastScroll) < TRACKBALL_SCROLL_DEBOUNCE) {
            return;
        }
        lastScroll = timer_read();
        if (horizontal_scroll) {
            mouse_report->h = x;
        }
        mouse_report->v = -y;
    } else {
        mouse_report->x = x;
        mouse_report->y = y;
    }
}
