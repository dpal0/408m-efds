#include <vector>
#ifndef PTIS_H
#define PTIS_H

// struct PTIS {
// 	unsigned char* data; /*pointer to 
// 	                    raw grayscale pixel*/
// 	int width;
// 	int height;
// };

struct PTIS{
   std::vector<std::vector<int>> pix;
   PTIS() : pix(24,std::vector<int>(24,0)){}
};

#endif 
