/* dmnetscan.cpp
 *
 * Data Matrix Scanner for the Beaglebone Black
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <dmtx.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    //cout << "Hello, OpenCV version "<< CV_VERSION << endl;
    
    size_t          width, height, bytesPerPixel;
    unsigned char   str[] = "00";
    unsigned char   *pxl;
    DmtxEncode      *enc;
    DmtxImage       *img;
    DmtxDecode      *dec;
    DmtxRegion      *reg;
    DmtxMessage     *msg;
    
    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    //capture.set(CV_CAP_PROP_FORMAT, CV_32F);
    if(!capture.isOpened()){
            cout << "Failed to connect to the camera." << endl;
    }
    
    Mat frame;
    
    capture >> frame;
    if(frame.empty()){
        cout << "Failed to capture an image" << endl;
        return -1;
    }
    
    imwrite("capture.png", frame);
    
    width = frame.cols;
    height = frame.rows;
     
    cout << "width: " << width << endl;
    cout << "height: " << height << endl;
    //cout << "depth: " << frame.depth() << endl;

    bytesPerPixel = 3;
    pxl = (unsigned char *)malloc(width * height * bytesPerPixel);
    assert(pxl != NULL);
    memcpy(pxl, frame.data, width * height * bytesPerPixel);
    
    img = dmtxImageCreate(pxl, width, height, DmtxPack24bppRGB);
    assert(img != NULL);

    cout << "created img." << endl;
    
    dec = dmtxDecodeCreate(img, 1);
    assert(dec != NULL);

    cout << "Decoded img." << endl;
    
    DmtxTime time = dmtxTimeAdd(dmtxTimeNow(), 500); //500 ms time out
    reg = dmtxRegionFindNext(dec, &time);
    
    cout << "Region Found Next. " << reg << endl;
    
    if(reg != NULL) {
        cout << "reg != NULL. " << endl;
        msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);
        if(msg != NULL) {
            cout << "msg != NULL. " << endl;
            fputs("output: \"", stdout);
            fwrite(msg->output, sizeof(unsigned char), msg->outputIdx, stdout);
            fputs("\"\n", stdout);
            dmtxMessageDestroy(&msg);
        }
        dmtxRegionDestroy(&reg);
    }

    dmtxDecodeDestroy(&dec);
    dmtxImageDestroy(&img);
    free(pxl);
    
    cout << "Exiting." << endl;
    
    return 0;
}