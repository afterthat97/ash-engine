INCLUDE_DIR = include
INCLUDE_BULLET_DIR = include/bullet
LIB_DIR = lib/macosx
BIN_DIR = bin

SRCS = scene.cpp light.cpp skybox.cpp model.cpp mesh.cpp material.cpp texture.cpp camera.cpp shader.cpp axis.cpp gridlines.cpp glconfig.cpp utilities.cpp extmath.cpp window.cpp main.cpp
OBJS = ${SRCS:.cpp=.o}

CC = g++
EXE = atview
CFLAGS = -O2 -std=c++11 -Wall -Iinclude -Iinclude/bullet
LIBS = -L${LIB_DIR} -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -lassimp -lfreeimage -lglfw -lanttweakbar -lglew
FRAMEWORKS = -framework OpenGL -framework Cocoa

atview : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${EXE} ${FRAMEWORKS} ${LIBS}
	install_name_tool -change /usr/local/opt/assimp/lib/libassimp.4.dylib @executable_path/libassimp.4.dylib ${EXE}
	install_name_tool -change /usr/local/opt/glfw/lib/libglfw.3.dylib @executable_path/libglfw.3.dylib ${EXE}
	install_name_tool -change /usr/local/opt/bullet/lib/libBulletSoftBody.2.87.dylib @executable_path/libBulletSoftBody.2.87.dylib ${EXE}
	install_name_tool -change /usr/local/opt/bullet/lib/libBulletDynamics.2.87.dylib @executable_path/libBulletDynamics.2.87.dylib ${EXE}
	install_name_tool -change /usr/local/opt/bullet/lib/libBulletCollision.2.87.dylib @executable_path/libBulletCollision.2.87.dylib ${EXE}
	install_name_tool -change /usr/local/opt/bullet/lib/libLinearMath.2.87.dylib @executable_path/libLinearMath.2.87.dylib ${EXE}
	mkdir -p ${BIN_DIR}
	mv atview ${BIN_DIR}
	cp ${LIB_DIR}/*.dylib ${BIN_DIR}

run : atview
	bin/atview

clean :
	rm -rf ${OBJS} *.d bin/

install : atview
	cp ${BIN_DIR}/atview /usr/local/bin

%.d : %.cpp
	${CC} -MM ${CFLAGS} $< > $@
	sed -i '' '1s/^/$@ /' $@

%.o: %.cpp
	${CC} -c ${CFLAGS} $<

-include ${SRCS:.cpp=.d}
