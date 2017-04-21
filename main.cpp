#include <QDebug>
#include <QCoreApplication>

#include "xrfcineloop.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    xrf::CineLoop loop = xrf::CineLoop::Create("C:/dev/xrfdump/data/pat4_biplaneA.dcm");
    if(loop.IsValid())
        for(auto tag : loop.GetDcmValues().keys())
            qDebug() << xrf::DcmTagIdToString[tag] << loop.GetDcmValues()[tag];

    //return a.exec();
}
