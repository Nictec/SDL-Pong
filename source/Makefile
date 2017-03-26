#don´t forget to add new .cpp files!! 
#note(Nicholas): sprites.cpp is not compiled because it is not in use (yet).
FILES = main.cpp renderer.cpp window.cpp 
SDL = /Library/Frameworks/SDL2.framework/Versions/A/SDL2 /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image 
OUT = "../build/SDL pong"

all:
	g++ $(FILES) -w $(SDL) -o $(OUT)

clean: 
	rm -f *.o 

run: 
	./$(OUT)
