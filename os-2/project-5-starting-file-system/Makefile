
mkfs: simfs.a mkfs.o 
	gcc -o $@ $^

simfs.a: image.o block.o free.o inode.o  
	ar rcs $@ $^

image.o: image.c
	gcc -Wall -Wextra -c $<

block.o: block.c
	gcc -Wall -Wextra -c $<

free.o: free.c
	gcc -Wall -Wextra -c $<

inode.o: inode.c
	gcc -Wall -Wextra -c $<

mkfs.o: mkfs.c
	gcc -Wall -Wextra -c $<

simfs_test: simfs_test.c simfs.a
	gcc -Wall -Wextra -o $@ $^ -DCTEST_ENABLE 

.PHONY: clean test

clean:
	rm -f *.o

test: simfs_test
	./simfs_test

.SILENT: run
run:
	make simfs_test
	./simfs_test
	make clean 
	rm -f filename.txt
	rm -f simfs_test
	rm -f simfs.a
	rm image_file.txt
