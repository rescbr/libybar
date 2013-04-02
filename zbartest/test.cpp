#include <iostream>
#include <zbar.h>
#include "stb_image.h"
#define STR(s) #s

using namespace std;
using namespace zbar;

int main (int argc, char **argv)
{
    if(argc < 2) return(1);


	

    // create a reader
    ImageScanner scanner;
    // configure the reader
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	int width, height, k;
	unsigned char *raw = stbi_load(argv[1], &width, &height, &k, 1);

    // obtain image data
    //Magick::Image magick(argv[1]);  // read an image file
	
    //int width = magick.columns();   // extract dimensions
    //int height = magick.rows();
    //Magick::Blob blob;              // extract the raw data
    //magick.modifyImage();
    //magick.write(&blob, "GRAY", 8);
    //const void *raw = blob.data();

    // wrap image data
    Image image(width, height, "GREY", raw, width * height);

    // scan the image for barcodes
    int n = scanner.scan(image);

    // extract results
    for(Image::SymbolIterator symbol = image.symbol_begin();
        symbol != image.symbol_end();
        ++symbol) {
        // do something useful with results
        cout << "decoded " << symbol->get_type_name()
             << " symbol \"" << symbol->get_data() << '"' << endl;
    }

    // clean up
    //image.set_data(NULL, 0);

    return(0);
}
