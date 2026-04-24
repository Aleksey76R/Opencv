#include "ImageTransformer.h"

ImageTransformer::ImageTransformer(const Mat &img)
{
    set_image(img);
}

Mat ImageTransformer::get_image() const{
    return image.clone();
}

void ImageTransformer::set_image(const Mat& image){
    if(image.empty())
        return;
    this->image = image.clone();
}

bool ImageTransformer::isEmpty() const {
    return image.empty();
}

void ImageTransformer::flipHorizontal(){
    if(isEmpty())
        return;
    flip(image, image, 1);
}

void ImageTransformer::flipVertical(){
    if (isEmpty())
        return;
    flip(image, image, 0);
}

void ImageTransformer::transpone(){
    if (isEmpty())
        return;
    Mat temp;
    transpose(image, temp);
    image = temp;
}

void ImageTransformer::rotate90Clockwise(){
    if (isEmpty())
        return;
    Mat transImage;
    rotate(image, transImage, ROTATE_90_CLOCKWISE);
}

void ImageTransformer::rotate90CounterClockwise(){
    if (isEmpty())
        return;
    Mat transImage;
    rotate(image, transImage, ROTATE_90_COUNTERCLOCKWISE);
}