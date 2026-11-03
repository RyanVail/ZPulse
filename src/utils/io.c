#include <utils/io.h>

static const size_t read_buf_size = 8192;

u8* io_read_file(const char* filename, size_t* len) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return NULL;
    }

    // Seeking to the end of the file to determine the size of the file.
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    // Getting the size of the file and going back to the beginning.
    const size_t size = ftell(file);
    *len = size;
    rewind(file);

    u8* data = malloc(size);
    if (data == NULL) {
        return NULL;
    }

    bool err = false;

    size_t i = 0;
    while (i < size) {
        const size_t bytes_left = size - i;
        const size_t num = (bytes_left >= read_buf_size)
            ? read_buf_size
            : bytes_left;

        const size_t num_read = fread(&data[i], 1, num, file);
        i += num_read;

        // Either an error or eof.
        if (num_read == 0) {
            if (feof(file) != 0) {
                break;
            } else {
                err = true;
                break;
            }
        }
    }

    // The whole file wasn't read or there was an error.
    if (i != size || err) {
        free(data);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return data;
}
