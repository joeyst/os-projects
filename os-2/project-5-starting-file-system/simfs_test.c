
#include "image.h"
#include "ctest.h"

int image_fd;

int main() {
	#ifdef CTEST_ENABLE
	image_open("filename.txt", 0);
	image_close();
	
	#else
	#endif
}