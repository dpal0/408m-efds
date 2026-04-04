#ifndef haar_feature_h
#define haar_feature_h

#define IMG_DIM 6

enum Feature {
    TWO_RECT_VERTICAL, // height must be multiple of 2
    TWO_RECT_HORIZONTAL, // width must be multiple of 2
    THREE_RECT_VERTICAL, // height must be multiple of 3
    THREE_RECT_HORIZONTAL, // width must be multiple of 3
    FOUR_RECT // height & width must be multiple of 2
};

class HaarFeature {

public:

    // constructor
    HaarFeature (int x, int y, int w, int h, Feature type);

    // accessors
    int getX() { return x; }
    int getY() { return y; }
    int getWidth() { return w; }
    int getHeight() { return h; }
    int getType() { return type; }

    // mutators
    void setX (int x) { this->x = x; }
    void setY (int y) { this->y = y; }
    void setWidth (int w) { this->w = w; }
    void setHeight (int h) { this->h = h; }
    void setType (Feature type) { this->type = type; }

    // compute haar feature value
    int computeFeatureVal (int* integralImg);

    // resize feature
    void scale (float factor);

    // print feature (for debugging purposes)
    void toString ();
    
private:
    // helper compute haar feature value
    int compute (int* integralImg, int x_start, int y_start, int x_end, 
        int y_end);

    // variables
    int x;
    int y;
    int w;
    int h;
    Feature type;

};

#endif

