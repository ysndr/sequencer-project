CC=gcc
SRC=src
TARGET=target

BUILD=${TARGET}/build
BIN=${TARGET}/bin
INCLUDE := ${SRC}/include

_LDFLAGS=-I ./include -I ${INCLUDE} ${LDFLAGS}
_CFLAGS=-g3 -Wall ${CFLAGS}

_PROG := ${BIN}/${PROG}
_SRC := $(wildcard ${BUILD}/${PROG}/*.c)


_OBJ := $(patsubst ${BUILD}/${PROG}/%.c,${BUILD}/${PROG}/%.o,$(_SRC))


${BUILD}/%.o: ${build}/%.c
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
all: prepare ${_PROG}
	@echo ${_PROG}




# ${BUILD}/${PROG}/$(basename $(notdir src/${PROG}))
