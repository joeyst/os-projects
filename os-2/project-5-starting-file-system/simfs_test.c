
#include "image.h"

int image_fd;

int main() {
	image_open("filename.txt", 0);
	image_close();
}