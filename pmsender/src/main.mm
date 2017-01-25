#include "ofMain.h"
#include "ofApp.h"

int main(){
    ofAppiOSWindow * window = new ofAppiOSWindow();
    window->enableHardwareOrientation();
    window->enableOrientationAnimation();
    
    //window->enableRetina();
    
	ofSetupOpenGL(window, 1024,768, OF_FULLSCREEN);
	ofRunApp(new ofApp);
}

extern "C"{
    size_t fwrite$UNIX2003( const void *a, size_t b, size_t c, FILE *d )
    {
        return fwrite(a, b, c, d);
    }
    char* strerror$UNIX2003( int errnum )
    {
        return strerror(errnum);
    }
    time_t mktime$UNIX2003(struct tm * a)
    {
        return mktime(a);
    }
    double strtod$UNIX2003(const char * a, char ** b) {
        return strtod(a, b);
    }
}