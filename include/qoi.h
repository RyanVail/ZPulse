#ifndef QOI_H
#define QOI_H

#include <common.h>

typedef enum qoi_channels {
    QOI_CHANNEL_RGB,
    QOI_CHANNEL_RGBA,
} qoi_channels;

typedef enum qoi_colorspace {
    QOI_COLORSPACE_SRGB,
    QOI_COLORSPACE_LINEAR,
} qoi_colorspace;

typedef struct qoi_pixel {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} qoi_pixel;

typedef struct qoi_image {
    u16 width;
    u16 height;
    qoi_channels channels;
    qoi_colorspace colorspace;
    qoi_pixel* pixels;
} qoi_image;

bool qoi_decode(qoi_image* image, const u8* data, size_t len);

#endif
