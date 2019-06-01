CC=gcc
SRC=src
TARGET=target

BUILD=${TARGET}/build
BIN=${TARGET}/bin
INCLUDE := ${SRC}/include

_LDFLAGS=${LDFLAGS}
_CFLAGS=-g3 -O0 -Wall

_PROG := ${BIN}/${PROG}
_SRC := $(wildcard ${SRC}/${PROG}/*.c)


_OBJ := $(patsubst ${SRC}/${PROG}/%.c,${BUILD}/${PROG}/%.o,$(_SRC))


${BUILD}/%.o: ${BUILD}/%.c
	@echo "($< |  $@)"
	${CC} ${_CFLAGS} -c $< -o $@ 


${_PROG}: ${_OBJ}
	@echo ${_OBJ}
	${CC} ${_LDFLAGS} -o ${_PROG} ${_OBJ}


prepare:
	mkdir -p ${BIN}
	mkdir -p ${BUILD}/${PROG}
	mkdir -p ${BUILD}/${PROG}/include/
	mkdir -p ${INCLUDE}
	cp -r ${INCLUDE}/* ${BUILD}/${PROG}/include/
	cp -r ${SRC}/${PROG}/* ${BUILD}/${PROG}

# ${OUT}: ${SRCS}:
# 	echo "srsc: ${SRCS}"
build: prepare ${_PROG}
	@echo ${_PROG}
