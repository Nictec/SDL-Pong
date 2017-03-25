#don´t forget to add new cpp files!!
FILES = main.cpp renderer.cpp sprites.cpp window.cpp 
SDL = /Library/Frameworks/SDL2.framework/Versions/A/SDL2 
OUT = "../build/SDL pong"

all:
	g++ $(FILES) -w $(SDL) -o $(OUT)

clean: 
	rm -f *.o 

run: 
	./$(OUT)  
