#include <QDebug>
#include <QCoreApplication>

#include "xrfcineloop.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    xrf::CineLoop loop = xrf::CineLoop::Create("C:/dev/xrfdump/data/pat4_biplaneA.dcm");
    if(loop.IsValid()) {
        for(auto tag : loop.GetDcmValues().keys())
            qDebug() << xrf::DcmTagIdToString[tag] << loop.GetDcmValues()[tag];

        int frmNo = 0;
        for(auto frame : loop.GetFrames())
            frame->save("C:/dev/xrfdump/data/img_frm_" + QString::number(frmNo++) + ".png", "PNG");
    }

    //return a.exec();
}
