#### Projects

array-ptr:
	gcc -o bin/array-ptr projects/array-ptr/src/main.c

concurrency:
	gcc -o bin/concurrency projects/concurrency/src/main.c

gfx/2d-images:
	gcc -o bin/2d-images -g -Wall -std=gnu99 -Iinclude projects/gfx/2d-images/src/main.c

gfx/ray-sphere-intersection:
	gcc -o bin/ray-sphere-intersection -g -Wall -std=gnu99 -Iinclude projects/gfx/ray-sphere-intersection/src/main.c -lm

gfx/shading-and-reflections:
	gcc -o bin/shading-and-reflections -g -Wall -std=gnu99 -Iinclude projects/gfx/shading-and-reflections/src/main.c -lm

pgm/example-1:
	gcc -o bin/pgm-1 projects/pgm/example-1/src/main.c

pgm/example-2:
	gcc -o bin/pgm-2 projects/pgm/example-2/src/main.c

pgm/example-3: libimg_1.so
	gcc -o bin/pgm-3 -Llib -limg_1 -Iinclude projects/pgm/example-3/src/main.c

pgm/example-4: libimg_2.so
	gcc -o bin/pgm-4 -Llib -fPIC -l:libimg_2.so -l:libnetpbm.so.10 -Iinclude projects/pgm/example-4/src/main.c


word-count/example-1:
	gcc -o bin/word-count-1 projects/word-count/example-1/src/main.c

word-count/example-2:
	gcc -o bin/word-count-2 projects/word-count/example-2/src/main.c

word-count/example-3:
	gcc -o bin/word-count-3 projects/word-count/example-3/src/main.c


#### Dependencies

libimg_1.so:
	gcc -o lib/libimg_1.so -fPIC -shared -Iinclude projects/pgm/example-3/src/img.c

libimg_2.so:
	gcc -fPIC -l:libnetpbm.so.10 -shared -o libimg_2.so -Iinclude projects/pgm/example-4/src/img.c