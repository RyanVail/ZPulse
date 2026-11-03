#include <qoi.h>
#include <utils/bin.h>

typedef enum qoi_chunk_kind {
    QOI_CHUNK_RGB,
    QOI_CHUNK_RGBA,
    QOI_CHUNK_INDEX,
    QOI_CHUNK_DIFF,
    QOI_CHUNK_LUMA,
    QOI_CHUNK_RUN,
} qoi_chunk_kind;

typedef struct qoi_state {
    qoi_pixel buffer[64];
    qoi_pixel last_pixel;
} qoi_state;

static qoi_chunk_kind get_chunk_kind(u8 first_byte) {
    if (first_byte == 0b11111111) {
        return QOI_CHUNK_RGBA;
    } else if (first_byte == 0b11111110) {
        return QOI_CHUNK_RGB;
    }

    switch (first_byte >> 6) {
        case 0b00: return QOI_CHUNK_INDEX; break;
        case 0b01: return QOI_CHUNK_DIFF; break;
        case 0b10: return QOI_CHUNK_LUMA; break;
        case 0b11: return QOI_CHUNK_RUN; break;
    }

    UNREACHABLE();
}

static size_t pix_buf_index(qoi_pixel pixel) {
    return (pixel.r * 3 + pixel.g * 5 + pixel.b * 7 + pixel.a * 11) % 64;
}

static void record_pixel(qoi_state* state, qoi_pixel pixel) {
    const size_t index = pix_buf_index(pixel);
    state->buffer[index] = pixel;
    state->last_pixel = pixel;
}

/**
 * Decodes the pixels from a raw QOI data stream into a pixel buffer.
 */
static bool decode_raw(
    qoi_pixel* pix_buf,
    size_t num_pixels,
    const u8* data,
    size_t len
) {
    qoi_state state = {
        .last_pixel = {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = 255,
        },
    };

    memset(&state.buffer, 0, sizeof(state.buffer));

    size_t pixel_index = 0;
    size_t data_index = 0;
    while (data_index < len && pixel_index < num_pixels) {
        const size_t bytes_left = len - data_index;

        // The decoded pixel.
        qoi_pixel pixel;

        const u8 first_byte = data[data_index];
        qoi_chunk_kind kind = get_chunk_kind(first_byte);

        switch (kind) {
            case QOI_CHUNK_RGB:
                if (bytes_left < 4) {
                    return false;
                }

                pixel = (qoi_pixel) {
                    .r = data[data_index + 1],
                    .g = data[data_index + 2],
                    .b = data[data_index + 3],
                    .a = state.last_pixel.a,
                };

                data_index += 4;
                break;

            case QOI_CHUNK_RGBA:
                if (bytes_left < 5) {
                    return false;
                }

                pixel = (qoi_pixel) {
                    .r = data[data_index + 1],
                    .g = data[data_index + 2],
                    .b = data[data_index + 3],
                    .a = data[data_index + 4],
                };

                data_index += 5;
                break;

            case QOI_CHUNK_INDEX:
                pixel = state.buffer[first_byte % 64];
                data_index++;
                break;

            case QOI_CHUNK_DIFF: {
                const u8 dr = ((first_byte >> 4) % 4) - 2;
                const u8 dg = ((first_byte >> 2) % 4) - 2;
                const u8 db = (first_byte % 4) - 2;

                pixel = (qoi_pixel) {
                    .r = state.last_pixel.r + dr,
                    .g = state.last_pixel.g + dg,
                    .b = state.last_pixel.b + db,
                    .a = state.last_pixel.a,
                };

                data_index++;
            } break;

            case QOI_CHUNK_LUMA: {
                if (bytes_left < 2) {
                    return false;
                }

                const u8 next_byte = data[data_index + 1];

                const u8 dg = (first_byte % 64) - 32;
                const u8 dr = (next_byte >> 4) + dg - 8;
                const u8 db = (next_byte % 16) + dg - 8;

                pixel = (qoi_pixel) {
                    .r = state.last_pixel.r + dr,
                    .g = state.last_pixel.g + dg,
                    .b = state.last_pixel.b + db,
                    .a = state.last_pixel.a,
                };

                data_index += 2;
            } break;

            case QOI_CHUNK_RUN: {
                const size_t run = (first_byte % 64) + 1;

                // Avoid writing past the end of the pixel buffer.
                if (pixel_index + run > num_pixels) {
                    return false;
                }

                for (size_t j = 0; j < run; j++) {
                    pix_buf[pixel_index] = state.last_pixel;
                    pixel_index++;
                }

                data_index++;
                continue;
            } break;
        }

        pix_buf[pixel_index] = pixel;
        record_pixel(&state, pixel);
        pixel_index++;
    }

    return true;
}

bool qoi_decode(qoi_image* image, const u8* data, size_t len) {
    // Minimum size of the file (header + data of simplest 1x1 image).
    if (len < 16) {
        return false;
    }

    // Verifying magic.
    if (data[0] != 'q' || data[1] != 'o' || data[2] != 'i' || data[3] != 'f') {
        return false;
    }

    const u32 width = u_read_u32_be(&data[4]);
    const u32 height = u_read_u32_be(&data[8]);

    // Set a generous limit on the dimensions of the images.
    if (width > 8192 || height > 8192) {
        return false;
    }

    const size_t num_pixels = width * height;

    qoi_pixel* pixels = malloc(sizeof(qoi_pixel) * num_pixels);
    if (pixels == NULL) {
        return false;
    }

    const uint8_t channels = data[12];
    const uint8_t colorspace = data[13];

    const bool success = decode_raw(pixels, num_pixels, &data[14], len - 14);

    if (!success) {
        free(pixels);
        return false;
    }

    *image = (qoi_image) {
        .width = (u16)width,
        .height = (u16)height,
        .channels = (channels == 3) ? QOI_CHANNEL_RGB : QOI_CHANNEL_RGBA,
        .colorspace = (colorspace == 0)
            ? QOI_COLORSPACE_SRGB
            : QOI_COLORSPACE_LINEAR,
        .pixels = pixels,
    };

    return true;
}
