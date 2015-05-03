#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "iprocesstechnique.h"
#include "allalgorithms.h"

#include <QObject>
#include <QImage>
#include <QString>
#include <QTimer>

// --------------------------
// Controller class
// --------------------------
class processmanager : public QObject
{
    Q_OBJECT // derived from QObject to emit signals, for instance for displaying images

private:
    cv::Mat ip;
    cv::Mat op;
    std::vector<Iprocesstechnique*> processtechniquesList; // stores user selected algorithm instances to process in process function loop

    // storing image matrices used for display in rendering on pixmap
    cv::Mat ipRGB;
    cv::Mat opRGB;

    QImage ipQImage;
    QImage opQImage;


    // Video/live stream member attributes
    QTimer *timer;
    cv::VideoCapture capture; // video + livestream object
    int framedelay; // for timer object



public:
    processmanager(); // constructor

    // --------------------------
    // accessors and mutators for images
    // --------------------------
    // accessors Input/output
    cv::Mat getInputImage(void)const;
    cv::Mat getOutputImage(void)const;
    // mutators input/output
    bool setInputImage(const cv::Mat&);
    bool setInputImage(std::string filename); // overloaded mutator for input with filename

    // accessors (RGB)
    const cv::Mat getInputImageRGB(void)const;
    const cv::Mat getOutputImageRGB(void)const;
    // mutators (RGB)
    bool setInputImageRGB(const cv::Mat&);

    // accessors (QImage)
    QImage getInputImageQImage(void)const;
    QImage getOutputImageQImage(void)const;
    // mutators (QImage)
    bool setInputImageQImage(QImage);
    bool setOutputImageQImage(QImage);


    // --------------------------
    // accessor for techniqueslist
    // --------------------------



    // --------------------------
    // Add/remove processing techniques
    // --------------------------
    void addProcessTechnique(const int&);

    // --------------------------
    // Image/Video/live stream handlers
    // --------------------------
    void loadImage(std::string);
    void loadVideo(std::string);
    void loadLiveStream();

    void pauseTimer(void);
    void restartTimer(void);

private slots:
    // Execute process
    void process(); // defined as slot to allow connection with signals, such as when image input is ready

    // Read frame from video/live stream
    void ReadFrame();

    // Algorithm instance mutators
    void addSaltAndPepper(const double&);
    void updateSaltAndPepperParams(const int&, const double&);

    // Morphology erode
    void addMorphologyErode(const int&, const int&); // add to process flow
    void updateMorphologyErodeParams(const int &, const int&, const int&); // mutator

    // Flip image
    void addFlipImage(const short int&);
    void updateFlipImageParams(const int&, const short int&);

signals:
    void ImageReadyInput(); // to display input image
    void ImageReadyOutput(); // to display output image


};

#endif // PROCESSMANAGER_H
