#include <emscripten.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

// Placeholder content for book pages
std::vector<std::string> pages = {
    "Page 1: Once upon a time in a distant kingdom, there lived a brave knight named Arthur. He was known far and wide for his courage and kindness. Arthur often wandered the forests and valleys, seeking to help those in need.",
    "Page 2: One day, Arthur heard about a dragon terrorizing a nearby village. Without hesitation, he mounted his horse and rode to the village. The dragon was fierce and breathed fire, but Arthur’s determination never wavered.",
    "Page 3: After a long and exhausting battle, Arthur managed to outwit the dragon and save the village. The villagers celebrated his bravery, throwing a grand feast in his honor.",
    "Page 4: Years later, Arthur became the protector of the kingdom. His deeds were etched in history, and songs were sung about his heroic acts, inspiring generations to come."
};

// Original content for reverting changes
std::vector<std::string> originalPages = pages;

// Current page index
int currentPage = 0;

// Image dimensions for shape generation
const int WIDTH = 200;
const int HEIGHT = 200;

// Buffer to hold image pixel data (RGBA format)
std::vector<unsigned char> imageBuffer(WIDTH * HEIGHT * 4);

extern "C" {
    // Get the text of the current page
    EMSCRIPTEN_KEEPALIVE
    const char* getCurrentPage() {
        return pages[currentPage].c_str();
    }

    // Move to the next page
    EMSCRIPTEN_KEEPALIVE
    void nextPage() {
        if (currentPage < pages.size() - 1) {
            currentPage++;
        }
    }

    // Move to the previous page
    EMSCRIPTEN_KEEPALIVE
    void prevPage() {
        if (currentPage > 0) {
            currentPage--;
        }
    }

    // Update the text of the current page
    EMSCRIPTEN_KEEPALIVE
    void updatePageText(const char* newText) {
        pages[currentPage] = newText;
    }

    // Revert text to the original content
    EMSCRIPTEN_KEEPALIVE
    void rejectPageChanges() {
        pages[currentPage] = originalPages[currentPage];
    }

    // Get the total number of pages
    EMSCRIPTEN_KEEPALIVE
    int getTotalPages() {
        return pages.size();
    }

    // Get the current page index
    EMSCRIPTEN_KEEPALIVE
    int getCurrentPageIndex() {
        return currentPage + 1; // 1-based index
    }

    // Generate a random number and call the JavaScript callback
    EMSCRIPTEN_KEEPALIVE
    void giveRandomNumber() {
        std::srand(std::time(0)); // Seed the random number generator
        int randomNumber = std::rand() % 100 + 1; // Generate a number between 1 and 100

        // Call the JavaScript function with the random number
        EM_ASM({
            // Pass the random number to the JavaScript handler
            handleRandomNumber($0);
        }, randomNumber);
    }

    // Generate an image with a square and a line
    EMSCRIPTEN_KEEPALIVE
    unsigned char* generateShape() {
        // Clear the buffer with a white background
        for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
            imageBuffer[i] = 255;     // Red
            imageBuffer[i + 1] = 255; // Green
            imageBuffer[i + 2] = 255; // Blue
            imageBuffer[i + 3] = 255; // Alpha
        }

        // Draw a black square
        for (int y = 50; y < 150; ++y) {
            for (int x = 50; x < 150; ++x) {
                int index = (y * WIDTH + x) * 4;
                imageBuffer[index] = 0;      // Red
                imageBuffer[index + 1] = 0;  // Green
                imageBuffer[index + 2] = 0;  // Blue
                imageBuffer[index + 3] = 255; // Alpha
            }
        }

        // Draw a red diagonal line
        for (int i = 0; i < WIDTH && i < HEIGHT; ++i) {
            int index = (i * WIDTH + i) * 4;
            imageBuffer[index] = 255;       // Red
            imageBuffer[index + 1] = 0;     // Green
            imageBuffer[index + 2] = 0;     // Blue
            imageBuffer[index + 3] = 255;   // Alpha
        }

        return imageBuffer.data();
    }

    // Return image width
    EMSCRIPTEN_KEEPALIVE
    int getImageWidth() {
        return WIDTH;
    }

    // Return image height
    EMSCRIPTEN_KEEPALIVE
    int getImageHeight() {
        return HEIGHT;
    }
}