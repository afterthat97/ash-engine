INCLUDE = include/
LIB = lib/
SRCS = scene.cpp light.cpp model.cpp mesh.cpp material.cpp texture.cpp camera.cpp shader.cpp glconfig.cpp utilities.cpp sdf.cpp main.cpp
OBJS = ${SRCS:.cpp=.o}
CFLAGS = -O2 -std=c++11 -Wall -I${INCLUDE}
CC = g++

atview : ${OBJS}
	g++ ${CFLAGS} ${OBJS} -o atview -framework OpenGL -framework Cocoa -L${LIB} -lassimp -lfreeimage -lglfw -lanttweakbar -lglew
	install_name_tool -change /usr/local/opt/assimp/lib/libassimp.4.dylib @executable_path/libassimp.dylib atview
	install_name_tool -change /usr/local/opt/glfw/lib/libglfw.3.dylib @executable_path/libglfw.dylib atview
	mv atview bin/
	cp ${LIB}*.dylib bin/

run : atview
	bin/atview ~/Documents/model/chair/chair.obj

.cpp.o :
	${CC} -c ${CFLAGS} $<

clean :
	rm -rf ${OBJS} bin/
	mkdir bin
