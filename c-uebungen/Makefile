CC=gcc
SRC=src
TARGET=target

BUILD=${TARGET}/build
BIN=${TARGET}/bin
INCLUDE := ${SRC}/include

_LDFLAGS=-I ./include -I ${INCLUDE} ${LDFLAGS}
_CFLAGS=-g3 -Wall ${CFLAGS}

_PROG := ${BIN}/${PROG}
_SRC := $(wildcard ${SRC}/${PROG}/*.c)


_OBJ := $(patsubst ${SRC}/${PROG}/%.c,${BUILD}/${PROG}/%.o,$(_SRC))


${BUILD}/%.o: ${SRC}/%.c
	@echo "($< |  $@)"
	${CC} ${_CFLAGS} -c $< -o $@ 


${_PROG}: ${_OBJ}
	@echo ${_OBJ}
	${CC} ${_LDFLAGS} -o ${_PROG} ${_OBJ}


prepare:
	mkdir -p ${BIN}
	mkdir -p ${BUILD}/${PROG}
	mkdir -p ${INCLUDE}

# ${OUT}: ${SRCS}:
# 	echo "srsc: ${SRCS}"
all: prepare ${_PROG}
	@echo ${_PROG}




# ${BUILD}/${PROG}/$(basename $(notdir src/${PROG}))
