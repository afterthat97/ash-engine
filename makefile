INCLUDE = include/
LIB = lib/
SRCS = scene.cpp light.cpp model.cpp mesh.cpp material.cpp load.cpp camera.cpp vector3f.cpp glconfig.cpp main.cpp
OBJS = ${SRCS:.cpp=.o}
CFLAGS = -O2 -std=c++11 -Wall -Wno-deprecated-declarations -I${INCLUDE}
CC = g++

atview : ${OBJS}
	g++ ${CFLAGS} ${OBJS} -o atview -framework OpenGL -framework GLUT -framework Cocoa -L${LIB} -lassimp -lfreeimage
	install_name_tool -change /usr/local/opt/assimp/lib/libassimp.4.dylib @executable_path/libassimp.dylib atview
	mkdir -p bin
	mv atview bin/
	cp ${LIB}libassimp.dylib bin/

.cpp.o : ${HEADERS}
	${CC} -c ${CFLAGS} $<

clean :
	rm -rf bin ${OBJS}
