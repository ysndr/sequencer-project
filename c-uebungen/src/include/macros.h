
#define ALLOC_ARRAY2D(ARRAY,TYPE,DIM1,DIM2,INIT) \
({\
    ARRAY = (TYPE **) malloc(sizeof(TYPE*)*DIM1 + DIM1*DIM2*sizeof(TYPE)); \
    for (int p = 0; p < DIM1; p++) { \
         ARRAY[p] = (TYPE*) (ARRAY + DIM1) + p * DIM2;\
         for (int e = 0; e < DIM2; e++) {\
                ARRAY[p][e] = INIT;\
         }\
    }\
})

#define DEL_BLOCK2D(ARRAY) (free(ARRAY))
