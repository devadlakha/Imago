#include "processmanager.h"

// --------------------------
// Controller class
// --------------------------

processmanager::processmanager()
{
    QObject::connect(this, SIGNAL(ImageReadyInput()), this, SLOT(process())); // this way, opening a new image/video/live stream continues the defined process flow processing for the new frame(s) as well, as process is called

    framedelay = 0; // error-handling: otherwise garbage value. (though handlers set timer framedelay anyway, so not necessarily required)
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()),this, SLOT(ReadFrame()));
}


// --------------------------
// Accessors/mutators for images
// --------------------------
bool processmanager::setInputImage(const cv::Mat &M){
    if (M.data){
        ip = M.clone();
        ipRGB = ip.clone(); // set as copy of input image (otherwise error in display image when uninitialized)
        op = ip.clone(); // set as copy of input image (otherwise error in display image when uninitialized)
        opRGB = ip.clone(); // set as copy of input image (otherwise error in display image when uninitialized)
        emit ImageReadyInput();
        return true;
    }
    else{
        // error handling: if no image received (eg: user pressed cancel in dialog box)
        return false;
    }
}

bool processmanager::setInputImage(std::string filename){
    ip = cv::imread(filename);

    if (!ip.data){
        // error handling: if no image received (eg: user pressed cancel in dialog box)
        return false;
    }
    else{
        ipRGB = ip.clone(); // set as copy of input image for initialization (otherwise error in display image when uninitialized)
        op = ip.clone(); // set as copy of input image (otherwise error in display image when uninitialized)
        opRGB = ip.clone(); // set as copy of input image (otherwise error in display image when uninitialized)
        emit ImageReadyInput();
        return true;
    }
}

cv::Mat processmanager::getInputImage()const{
    return ip;
}

cv::Mat processmanager::getOutputImage()const{
    return op;
}

// --------------------------
// Accessors/mutators for images (RGB)
// --------------------------
bool processmanager::setInputImageRGB(const cv::Mat &M){
    if (M.data){
        ipRGB = M.clone();
        return true;
    }
    else{
        return false;
    }
}

const cv::Mat processmanager::getInputImageRGB()const{
    return ipRGB;
}

const cv::Mat processmanager::getOutputImageRGB()const{
    return opRGB;
}


// --------------------------
// Accessors/mutators for images (QImage)
// --------------------------
bool processmanager::setInputImageQImage(QImage M){
    if (!M.isNull()){
        ipQImage = M.copy();
        return true;
    }
    else{
        return false;
    }
}

bool processmanager::setOutputImageQImage(QImage M){
    if (!M.isNull()){
        opQImage = M.copy();
        return true;
    }
    else{
        return false;
    }
}

QImage processmanager::getInputImageQImage()const{
    return ipQImage;
}

QImage processmanager::getOutputImageQImage()const{
    return opQImage;
}


// --------------------------
// Add/remove process technique
// --------------------------

void processmanager::addSaltAndPepper(const double &d){
    Iprocesstechnique* pt = new saltandpepper(d);
    processtechniquesList.push_back(pt);

    process();
}

void processmanager::updateSaltAndPepperParams(const int& indx, const double& d){
    dynamic_cast<saltandpepper*>(processtechniquesList[indx])->setParams(d);
    process();
}


void processmanager::addMorphologyErode(const int &val1, const int &val2){
    Iprocesstechnique* pt = new morphologyerode(val1, val2);
    processtechniquesList.push_back(pt);

    process();
}

void processmanager::updateMorphologyErodeParams(const int& indx, const int &val1, const int &val2){
    dynamic_cast<morphologyerode*>(processtechniquesList[indx])->setParams(val1, val2);
    process();
}


void processmanager::addFlipImage(const short &flipcode){
    Iprocesstechnique* pt = new flipimage(flipcode);
    processtechniquesList.push_back(pt);

    process();
}

void processmanager::updateFlipImageParams(const int & indx, const short int &flipcode){
    dynamic_cast<flipimage*>(processtechniquesList[indx])->setParams(flipcode);
    process();
}

// --------------------------
// Process function
// --------------------------
void processmanager::process(){
    // error-handling: necessary as output matrix is used in sucession, which should be refreshed to the original input at the beginning of the process loop
    ip.copyTo(op);

    std::vector<Iprocesstechnique*>::iterator it;
    for (it=processtechniquesList.begin(); it!=processtechniquesList.end(); it++){
        (*it)->process(op, op);
    }

    emit ImageReadyOutput();
}


// --------------------------
// Image/Video/live stream handlers
// --------------------------
void processmanager::loadImage(std::string filename){
    // handles the case where image is opened after video/live stream. (active timer must be stopped for new image to be used)
    if (timer->isActive()){
        timer->stop();
        capture.release();
    }
    setInputImage(filename);
}

void processmanager::loadVideo(std::string filename){
    // callback from open video push button

    if (timer->isActive()){
        timer->stop();
        capture.release();
    }

    capture.open(filename);
    if (capture.isOpened()){
        double rate = capture.get(CV_CAP_PROP_FPS);
        framedelay=1000/rate;
        // timer restarts (no need to stop and start) [2]
        timer->start(framedelay); // set for pause/play utilization
    }
}

void processmanager::loadLiveStream(){
    // callback from open live stream push button

    if (timer->isActive()){
        timer->stop();
        capture.release();
    }

    capture.open(0);
    if (capture.isOpened()){
        // timer restarts (no need to stop and start) [2]
        framedelay = 0; // set for pause/play utilization
        timer->start(framedelay);
    }
}

void processmanager::ReadFrame(){
    // timer callback function to set input image frame from video/live stream
    cv::Mat frame;
    capture>>frame;
    setInputImage(frame);
}

void processmanager::pauseTimer(void){
    // capture is not released to simulate pause

    if (capture.isOpened()){
        if (timer->isActive()){
            timer->stop();
        }
    }
}

void processmanager::restartTimer(void){
    // play after pause

    if (capture.isOpened()){
        if (!timer->isActive()){
            timer->start(framedelay);
        }
    }
}
