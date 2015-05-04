#ifndef IMAGO_H
#define IMAGO_H

// default images in application
#define DefResourceImage ":/boldt.jpg" // resource file
#define DefImage "boldt.jpg" // run directory

#include <QMainWindow>
#include "processmanager.h"

namespace Ui {
class Imago;
}

class Imago : public QMainWindow
{
    Q_OBJECT

public:
    explicit Imago(QWidget *parent = 0);
    ~Imago();

private:
    Ui::Imago *ui;

    // controller
    processmanager *controller;

    void initializeConnections(void)const;

    void initializeDefaultInputImage(void)const;

    void setRanges(void);

    void UpdateListWidget(QString);
    void UpdateListWidgetCurr(QString);
    void UpdateListWidgetMove(const int&, const int&);
    void UpdateListWidgetRemove(const int&);
    void UpdateListWidgetRefresh();

    short determineFlipCode(void)const;


private slots:
    void on_actionOpen_Image_triggered();

    void DisplayInputImage(); // function to display input image in input label
    void DisplayOutputImage(); // function to display output image in output label

    void on_actionOpen_Video_triggered();
    void on_actionOpen_Livestream_triggered();


    void on_PauseTimer_clicked();
    void on_RestartTimer_clicked();

    void on_AddMorphologyOperation_clicked();
    void morphologyOperationParamChanged(void);

    void on_AddFlipImage_clicked();

    void flipImageParamChanged();

    void on_AddSaltAndPepper_clicked();
    void saltAndPepperParamChanged();

    void on_moveProcessDown_clicked();

    void on_moveProcessUp_clicked();

    void on_removeProcess_clicked();

    void on_refreshProcess_clicked();

    void on_AdEqualizeHistogram_clicked();

    void on_AddLowPassFilter_clicked();
    void lowPassFilterParamChanged(void);

signals:
    void s_addSaltandPepper(const double&);
    void updateSaltAndPepper(const int&, const double&);

    void s_addMorphologyOperation(const int&, const int&, const int&);
    void updateMorphologyOperation(const int&, const int&, const int&, const int&);

    void s_addFlipImage(const short int);
    void updateFlipImage(const int&, const short int&);

    void s_addHistogramEqualize();

    void s_addLowPassFilter(const int&, const int&);
    void updateLowPassFilter(const int&, const int&, const int&);
};

#endif // IMAGO_H
