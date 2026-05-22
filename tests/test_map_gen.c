#include "map/map.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("--- BSP Map Generation Test ---\n");

    int32_t width = 60;
    int32_t height = 20;
    
    Map* map = Map_Create(width, height);
    if (!map) {
        printf("Failed to create map\n");
        return 1;
    }

    // Use a fixed seed for reproducibility during testing
    uint32_t seed = 12345;
    if (Map_Generate(map, seed)) {
        printf("Map generated successfully with seed %u:\n\n", seed);
        Map_PrintToConsole(map);
    } else {
        printf("Map generation failed!\n");
    }

    Map_Destroy(map);
    printf("\n--- Test Complete ---\n");
    return 0;
}
