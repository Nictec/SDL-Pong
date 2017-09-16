FILE_SDL = source/sdl_only.cpp
FILE_SDL_WASM = source/sdl_wasm.cpp
MAC_SDL = /Library/Frameworks/SDL2.framework/Versions/A/SDL2 /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image
OUT_DIR = ./build/
OUT_EXEC = $(OUT_DIR)/SDL_pong
WASM_FLAGS = \
		-s USE_SDL=2 \
		-s TOTAL_MEMORY=256*1024*1024
WASM_COMP = /usr/lib/emscripten/emcc
LINUX_SDL_FLAGS = -ldl `sdl2-config --cflags --libs` -lSDL2_image

wasm-html:
	$(WASM_COMP) $(FILE_SDL_WASM) \
		$(WASM_FLAGS) \
		-o $(OUT_DIR)/pong.html

wasm-node:
	$(WASM_COMP) $(FILE_SDL_WASM) \
		$(WASM_FLAGS) \
		-o $(OUT_DIR)/pong.js

linux:
	gcc $(FILE_SDL) $(LINUX_SDL_FLAGS) -o $(OUT_EXEC)

mac:
	g++ $(FILE_SDL) -w $(MAC_SDL) -o $(OUT_EXEC)

clean:
	rm -f *.o

run:
	./$(OUT_EXEC)

run-html:
	/usr/lib/emscripten/emrun --no-browser --port 8008 $(OUT_DIR)

run-node:
	node --expose-wasm $(OUT_DIR)/pong.js
