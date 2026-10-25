#ifndef ANIM_MODULE_H
#define ANIM_MODULE_H

#include "esp_err.h"
#include "lvgl.h"

#define ANIM_W            172
#define ANIM_H            160
#define ANIM_FRAME_BYTES  (ANIM_W * ANIM_H * 2)   // 55040
#define LISTEN_FRAMES     38
#define WAIT_FRAMES       90
#define ANIM_FRAME_MS     (1000 / 12)             // set to 1000 / your converted fps

typedef enum { ANIM_LISTEN, ANIM_WAIT, ANIM_COUNT } anim_id_t;

esp_err_t anim_init(lv_obj_t *parent);   // map partitions, create lv_image + timer (paused)
void      anim_play(anim_id_t id);       // start/switch clip, loops forever
void      anim_stop(void);               // pause timer

#endif
