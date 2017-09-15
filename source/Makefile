#don´t forget to add new .cpp files!! 
#note(Nicholas): sprites.cpp is not compiled because it is not in use (yet).
FILES = main.cpp renderer.cpp window.cpp 
MAC_SDL = /Library/Frameworks/SDL2.framework/Versions/A/SDL2 /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image 
OUT_DIR = "../build/"
OUT_EXEC = "$(OUT_DIR)/SDL_pong"

wasm-html:
	/usr/lib/emscripten/emcc $(FILES) \
		-s USE_SDL=2 -s USE_SDL_IMAGE=2 \
		--preload-file build/images/background.bmp \
		-o "$(OUT_DIR)/pong.html"

wasm-node:
	/usr/lib/emscripten/emcc $(FILES) \
		-s USE_SDL=2 -s USE_SDL_IMAGE=2 \
		--preload-file build/images/background.bmp \
		-o "$(OUT_DIR)/pong.js"

linux:
	gcc $(FILES) -ldl `sdl2-config --cflags --libs` -lSDL2_image -o $(OUT_EXEC)

mac:
	g++ $(FILES) -w $(MAC_SDL) -o $(OUT_EXEC)

clean: 
	rm -f *.o 

run: 
	./$(OUT_EXEC)

run-html:
	/usr/lib/emscripten/emrun --no-browser --port 8008 $(OUT_DIR)

run-node:
	node --expose-wasm $(OUT_DIR)/pong.js
