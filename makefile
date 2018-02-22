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

${BIN_DIR}/atview : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${EXE} ${FRAMEWORKS} ${LIBS}
	install_name_tool -change /usr/local/opt/assimp/lib/libassimp.4.dylib @executable_path/libassimp.dylib ${EXE}
	install_name_tool -change /usr/local/opt/glfw/lib/libglfw.3.dylib @executable_path/libglfw.dylib ${EXE}
	install_name_tool -change /usr/local/opt/bullet/lib/libBulletSoftBody.2.87.dylib @executable_path/libBulletSoftBody.dylib ${EXE}
	install_name_tool -change /usr/local/opt/bullet/lib/libBulletDynamics.2.87.dylib @executable_path/libBulletDynamics.dylib ${EXE}
	install_name_tool -change /usr/local/opt/bullet/lib/libBulletCollision.2.87.dylib @executable_path/libBulletCollision.dylib ${EXE}
	install_name_tool -change /usr/local/opt/bullet/lib/libLinearMath.2.87.dylib @executable_path/libLinearMath.dylib ${EXE}
	mkdir -p ${BIN_DIR}
	mv atview ${BIN_DIR}
	cp ${LIB_DIR}/*.dylib ${BIN_DIR}

run : ${BIN_DIR}/atview
	bin/atview

clean :
	rm -rf ${OBJS} *.d bin/

%.d : %.cpp
	${CC} -MM ${CFLAGS} $< > $@
	sed -i '' '1s/^/$@ /' $@

%.o: %.cpp
	${CC} -c ${CFLAGS} $<

-include ${SRCS:.cpp=.d}
