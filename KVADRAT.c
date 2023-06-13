#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FRAMEBUFFER_PATH "/dev/fb0"
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define BYTES_PER_PIXEL 4

void drawSquare(int *framebuffer, int x, int y, int size, int color) {
    int i, j;

    for (i = y; i < y + size; i++) {
        for (j = x; j < x + size; j++) {
            int pixel_offset = (i * SCREEN_WIDTH + j) * BYTES_PER_PIXEL;
            framebuffer[pixel_offset / sizeof(int)] = color;
        }
    }
}

int main() {
    int fbfd;
    int *framebuffer;
    int x = 0;
    int y = 0;
    int size = 10;
    int velocity_x = 1,velocity_y = 1;
    int color = 0xAA0000AA; // RGBA color format (0xRRGGBBAA)

    // Open the framebuffer device
    fbfd = open(FRAMEBUFFER_PATH, O_RDWR);
    if (fbfd == -1) {
        perror("Unable to open framebuffer");
        exit(1);
    }

    // Map the framebuffer device to memory
    framebuffer = mmap(NULL, SCREEN_WIDTH * SCREEN_HEIGHT * BYTES_PER_PIXEL, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (framebuffer == MAP_FAILED) {
        perror("mmap");
        close(fbfd);
        exit(1);
    }

    // Animation loop
    while (1) {
        // Clear the screen
        memset(framebuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * BYTES_PER_PIXEL);

        // Reverse direction if the square hits the screen boundaries
        if (x < 0 || x + size >= SCREEN_WIDTH){
            velocity_x = -velocity_x;
            if (x<0) x =0;
        }

        if (y < 0 || y + size >= SCREEN_HEIGHT)
        {
            velocity_y = -velocity_y;
            if(y<0) y=0;
        }


        // Draw the square at the current position
        drawSquare(framebuffer, x, y, size, color);

        // Update the position
        x += velocity_x;
        y += velocity_y;



        // Wait for a short time to control animation speed
        usleep(1000);
    }

    // Unmap the framebuffer and close the framebuffer device
    munmap(framebuffer, SCREEN_WIDTH * SCREEN_HEIGHT * BYTES_PER_PIXEL);
    close(fbfd);

    return 0;
}
