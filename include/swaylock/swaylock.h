#ifndef _SWAYLOCK_H
#define _SWAYLOCK_H
#include <stdbool.h>
#include <stdint.h>
#include <wayland-client.h>
#include "background-image.h"
#include "cairo.h"
#include "pool-buffer.h"
#include "swaylock/seat.h"
#include "wlr-layer-shell-unstable-v1-client-protocol.h"

enum auth_state {
    AUTH_STATE_IDLE,
    AUTH_STATE_INPUT,
    AUTH_STATE_BACKSPACE,
    AUTH_STATE_VALIDATING,
    AUTH_STATE_INVALID,
};

struct swaylock_args {
	uint32_t color;
	enum background_mode mode;
	bool show_indicator;
};

struct swaylock_password {
	size_t size;
	size_t len;
	char *buffer;
};

struct swaylock_state {
	struct wl_display *display;
	struct wl_compositor *compositor;
	struct zwlr_layer_shell_v1 *layer_shell;
	struct zwlr_input_inhibit_manager_v1 *input_inhibit_manager;
	struct wl_shm *shm;
	struct wl_list surfaces;
	struct swaylock_args args;
	struct swaylock_password password;
	struct swaylock_xkb xkb;
	enum auth_state auth_state;
	bool run_display;
};

struct swaylock_surface {
	cairo_surface_t *image;
	struct swaylock_state *state;
	struct wl_output *output;
	struct wl_surface *surface;
	struct zwlr_layer_surface_v1 *layer_surface;
	struct pool_buffer buffers[2];
	struct pool_buffer *current_buffer;
	uint32_t width, height;
	int32_t scale;
	struct wl_list link;
};

void swaylock_handle_key(struct swaylock_state *state,
		xkb_keysym_t keysym, uint32_t codepoint);
void render_frame(struct swaylock_surface *surface);
void render_frames(struct swaylock_state *state);

#endif
