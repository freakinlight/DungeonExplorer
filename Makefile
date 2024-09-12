all: dungeon_game

dungeon_game: src/main.c
	clang -I/opt/homebrew/include -L/opt/homebrew/lib/ -lSDL2 -llSDL2_image src/main.c -o dungeon_game

clean:
	rm dungeon_game

