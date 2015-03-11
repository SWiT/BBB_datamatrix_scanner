/*  BeagleEye network datamatrix scanner
 *  Copyright (C) 2015 Matthew Gary Switlik
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */  

#include <iostream>
#include <signal.h>
#include <opencv2/opencv.hpp>
#include <dmtx.h>

using namespace std;
using namespace cv;

bool exitsignal = false;

void signalhandler(int num) {
    switch (num) {
        case SIGTERM:
        case SIGINT:
            exitsignal = true;
            break;
    }
}

int main(int argc, char *argv[]) {
    printf("\nBeagleEye v0.01 (Ctrl-C to Exit)\n");
    
    size_t          width, height, bytesPerPixel, timeout;
    unsigned char   *pxl;
    DmtxImage       *img;
    DmtxDecode      *dec;
    DmtxRegion      *reg;
    DmtxMessage     *msg;
    DmtxTime        time;
    Mat             frame;
    
    width = 1280;
    height = 720;
    bytesPerPixel = 3;
    pxl = (unsigned char *)malloc(width * height * bytesPerPixel);
        
    timeout = 500; //ms

    signal (SIGINT, signalhandler);
    signal (SIGTERM, signalhandler);
    
    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    
    if(!capture.isOpened()){
        cout << "Failed to connect to the camera." << endl;
        return -1;
    }

    for (;;) {
        capture >> frame;
        
        if (frame.empty()){
            cout << "Failed to capture frame" << endl;
            return -1;
        }
        memcpy(pxl, frame.data, width * height * bytesPerPixel); //TODO: try removing this and using frame.data directly
        
        img = dmtxImageCreate(pxl, width, height, DmtxPack24bppRGB);
        assert(img != NULL);

        dec = dmtxDecodeCreate(img, 1);
        assert(dec != NULL);

        time = dmtxTimeAdd(dmtxTimeNow(), timeout);
        reg = dmtxRegionFindNext(dec, &time);
        if (reg != NULL) {
            msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);
            if(msg != NULL) {
                fputs("found: \"", stdout);
                fwrite(msg->output, sizeof(unsigned char), msg->outputIdx, stdout);
                fputs("\"\n", stdout);
                dmtxMessageDestroy(&msg);
            }
            dmtxRegionDestroy(&reg);
        } else {
            cout << "." << endl;
        }
        
        if(exitsignal == true) {
            capture.release();
            dmtxDecodeDestroy(&dec);
            dmtxImageDestroy(&img);
            free(pxl);
            imwrite("capture.png", frame);
            break;
        }
    }

    cout << "Exit." <<  endl;
    return 0;
}