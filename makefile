INCLUDE_DIR = include
INCLUDE_BULLET_DIR = include/bullet
LIB_DIR = lib/macosx
BIN_DIR = bin

SRCS = glad.cpp scene.cpp light.cpp skybox.cpp model.cpp mesh.cpp material.cpp texture.cpp camera.cpp shader.cpp axis.cpp gridlines.cpp glconfig.cpp utilities.cpp extmath.cpp window.cpp main.cpp
OBJS = ${SRCS:.cpp=.o}

CC = g++
EXE = masterEngine
CFLAGS = -O2 -std=c++11 -Wall -Iinclude -Iinclude/bullet
LIBS = -L${LIB_DIR} -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -lassimp -lfreeimage -lglfw -lanttweakbar
FRAMEWORKS = -framework OpenGL -framework Cocoa

${BIN_DIR}/${EXE}.app : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${EXE} ${FRAMEWORKS} ${LIBS} -w
	install_name_tool -change /usr/local/opt/assimp/lib/libassimp.4.dylib @executable_path/libassimp.dylib ${EXE}
	install_name_tool -change /usr/local/opt/glfw/lib/libglfw.3.dylib @executable_path/libglfw.dylib ${EXE}
	mkdir -p ${BIN_DIR}
	mkdir -p ${BIN_DIR}/${EXE}.app
	mkdir -p ${BIN_DIR}/${EXE}.app/Contents
	mkdir -p ${BIN_DIR}/${EXE}.app/Contents/MacOS
	mkdir -p ${BIN_DIR}/${EXE}.app/Contents/Resources
	mv ${EXE} ${BIN_DIR}/${EXE}.app/Contents/MacOS/
	cp ${LIB_DIR}/*.dylib ${BIN_DIR}/${EXE}.app/Contents/MacOS/
	cp Info.plist ${BIN_DIR}/${EXE}.app/Contents/
	cp icon/AppIcon.icns ${BIN_DIR}/${EXE}.app/Contents/Resources/

run : ${BIN_DIR}/${EXE}.app
	open ${BIN_DIR}/${EXE}.app

clean :
	rm -rf ${OBJS} *.d ${BIN_DIR}

%.d : %.cpp
	${CC} -MM ${CFLAGS} $< > $@
	sed -i '' '1s/^/$@ /' $@

%.o: %.cpp
	${CC} -c ${CFLAGS} $<

-include ${SRCS:.cpp=.d}
