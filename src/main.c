#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>

// Function to load textures from a file
SDL_Texture* load_texture(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (texture == NULL) {
        printf("Failed to load texture %s! SDL_image Error: %s\n", path, IMG_GetError());
    }
    return texture;
}

// Function to check for collision between two rectangles (player and walls)
int check_collision(SDL_Rect a, SDL_Rect b) {
    if (a.x + a.w <= b.x || a.x >= b.x + b.w || a.y + a.h <= b.y || a.y >= b.y + b.h) {
        return 0;  // No collision
    }
    return 1;  // Collision detected
}

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;
    int running = 1;
    float player_x = 320.0f, player_y = 240.0f;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    window = SDL_CreateWindow("Dungeon Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (window == NULL || renderer == NULL) {
        printf("Window/Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load textures
    SDL_Texture* player_texture = load_texture("assets/player.png", renderer);
    SDL_Texture* enemy_texture = load_texture("assets/enemy.png", renderer);  // Load enemy texture
    SDL_Texture* wall_texture = load_texture("assets/wall.png", renderer);
    SDL_Texture* background_texture = load_texture("assets/background.png", renderer);

    if (player_texture == NULL || wall_texture == NULL || background_texture == NULL || enemy_texture == NULL) {
        return 1;  // Exit if textures are not loaded
    }

    // Define the wall (static for now)
    SDL_Rect wall_rect = {200, 100, 100, 50};  // Example static wall

    // Define the enemy
    SDL_Rect enemy_rect = {400, 150, 50, 50};  // Static enemy for now

    // Main loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Player movement logic
        SDL_Rect player_rect = {(int)player_x, (int)player_y, 50, 50};
        const Uint8* state = SDL_GetKeyboardState(NULL);

        // Store player's new position before moving
        float new_player_x = player_x;
        float new_player_y = player_y;

        // Move the player with WASD keys
        if (state[SDL_SCANCODE_W]) { new_player_y -= 5; }
        if (state[SDL_SCANCODE_S]) { new_player_y += 5; }
        if (state[SDL_SCANCODE_A]) { new_player_x -= 5; }
        if (state[SDL_SCANCODE_D]) { new_player_x += 5; }

        // Check for collisions with walls
        SDL_Rect new_player_rect = {(int)new_player_x, (int)new_player_y, 50, 50};
        if (!check_collision(new_player_rect, wall_rect)) {
            player_x = new_player_x;  // Update player position if no collision
            player_y = new_player_y;
        }

        // Clear the renderer and render the background
        SDL_RenderClear(renderer);
        SDL_Rect background_rect = {0, 0, 640, 480};  // Full screen background
        SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);

        // Render walls
        SDL_RenderCopy(renderer, wall_texture, NULL, &wall_rect);

        // Render player
        player_rect = (SDL_Rect){(int)player_x, (int)player_y, 50, 50};
        SDL_RenderCopy(renderer, player_texture, NULL, &player_rect);

        // Render enemy
        SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect);

        // Present the renderer
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyTexture(player_texture);
    SDL_DestroyTexture(wall_texture);
    SDL_DestroyTexture(enemy_texture);  // Destroy enemy texture
    SDL_DestroyTexture(background_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
