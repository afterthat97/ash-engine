INCLUDE = include/
LIB = lib/
BIN = bin/
SRCS = scene.cpp light.cpp skybox.cpp model.cpp mesh.cpp material.cpp texture.cpp camera.cpp shader.cpp axis.cpp gridlines.cpp glconfig.cpp utilities.cpp extmath.cpp main.cpp
OBJS = ${SRCS:.cpp=.o}
CFLAGS = -O2 -std=c++11 -Wall -I${INCLUDE}
CC = g++

atview : ${OBJS}
	g++ ${CFLAGS} ${OBJS} -o atview -framework OpenGL -framework Cocoa -L${LIB} -lassimp -lfreeimage -lglfw -lanttweakbar -lglew
	install_name_tool -change /usr/local/opt/assimp/lib/libassimp.4.dylib @executable_path/libassimp.dylib atview
	install_name_tool -change /usr/local/opt/glfw/lib/libglfw.3.dylib @executable_path/libglfw.dylib atview
	mv atview ${BIN}
	cp ${LIB}*.dylib ${BIN}

run : atview
	bin/atview ~/Models/chair/chair.obj

clean :
	rm -rf ${OBJS} *.d bin/*

countline :
	@echo "Total code lines: `ls *include/*.h *.cpp | xargs cat | wc -l`"

%.d : %.cpp
	${CC} -MM ${CFLAGS} $< > $@
	sed -i '' '1s/^/$@ /' $@

%.o: %.cpp
	${CC} -c ${CFLAGS} $<

-include ${SRCS:.cpp=.d}
