#include "xrfcineloop.h"

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmimgle/dcmimage.h>
//#include <dcmtk/dcmdata/dcdicdir.h>
//#include "dcmtk/oflog/oflog.h"
//#include "dcmtk/config/osconfig.h"
//#include "dcmtk/dcmdata/dcistrmf.h"

#include <memory>
#include <utility>
#include <functional>
#include <stdexcept>

#include <QDebug>
#include <QVariant>

#include "xrfdcmutils.cpp"

namespace xrf {

    using namespace std::placeholders;
    GetDcmTagMap CineLoop::DcmTagFnMap = {
                             {IMAGE_TYPE,                   std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[IMAGE_TYPE],QString())},
                             {INSTANCE_NUMBER,              std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[INSTANCE_NUMBER],QString())},
                             {ROWS,                         std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[ROWS],uint(0))     },
                             {COLS,                         std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[COLS],uint(0))  },
                             {IMAGER_PIXEL_SPACING,         std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[IMAGER_PIXEL_SPACING],"0.616\\0.616")  },
                             {FRAME_TIME,                   std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[FRAME_TIME],"")  },
                             {NUMBER_OF_FRAMES,            std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[NUMBER_OF_FRAMES],int(0))  },
                             {RECOMMENDED_DISPLAY_FRAME_RATE,            std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[RECOMMENDED_DISPLAY_FRAME_RATE],int(0))  },
                             {ISOCENTER_TABLE_POSITION,     std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[ISOCENTER_TABLE_POSITION],QByteArray())  },
                             {DISTANCE_OBJECT_TO_TABLE,        std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[DISTANCE_OBJECT_TO_TABLE],QString())  },
                             {DISTANCE_SOURCE_TO_DETECTOR,           std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[DISTANCE_SOURCE_TO_DETECTOR],QString())  },
                             {DISTANCE_SOURCE_TO_PATIENT,            std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[DISTANCE_SOURCE_TO_PATIENT],QString())  },
                             {DISTANCE_SOURCE_TO_ISOCENTER,          std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[DISTANCE_SOURCE_TO_ISOCENTER],int(-1))  },
                             {POSITION_PRIMARY_ANGLE,       std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[POSITION_PRIMARY_ANGLE],QString())  },
                             {POSITION_SECONDARY_ANGLE,     std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[POSITION_SECONDARY_ANGLE],QString())  },
                             {DETECTOR_ROTATION_ANGLE,            std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[DETECTOR_ROTATION_ANGLE],QString())  },
                             {SAMPLES_PER_PIXEL,            std::bind(dcm_tag_read,_1,DcmTagIdToKeyMap[SAMPLES_PER_PIXEL],uint(0))  },
                            };



    CineLoop CineLoop::Create(const QString &filename)
    {
        CineLoop loop;

        LoadDcmDictionary();

        loop.mFileInfo = QFileInfo(filename);

        std::unique_ptr<DcmDecoder> decoder = std::make_unique<DcmDecoder>();
        std::unique_ptr<DcmFileFormat> dfile = std::make_unique<DcmFileFormat>();

        OFCondition cond = dfile->loadFile(filename.toStdString().c_str(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);

        if (cond.bad()) {
            qDebug() << "cannot load file due to: [" << cond.text() << "].";
            return loop;
        }

        cond = dfile->loadAllDataIntoMemory();
        if (cond.bad()) {
            qDebug() << "Error loading all DICOM tags into memory: [" << cond.text() << "].";
            return loop;
        }

        for(auto tag : DcmTagFnMap.keys()) {
            loop.mDcmTagValues[tag] = DcmTagFnMap[tag](std::ref(dfile));
        }

        loop.LoadFrames();

        loop.mIsValid = true;
        return loop;
    }

    std::unique_ptr<CineLoop> CineLoop::CreatePtr(const QString &filename)
    {
        std::unique_ptr<CineLoop> loop = std::unique_ptr<CineLoop>(new CineLoop());

        LoadDcmDictionary();

        loop->mFileInfo = QFileInfo(filename);

        std::unique_ptr<DcmDecoder> decoder = std::make_unique<DcmDecoder>();
        std::unique_ptr<DcmFileFormat> dfile = std::make_unique<DcmFileFormat>();

        OFCondition cond = dfile->loadFile(filename.toStdString().c_str(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);

        if (cond.bad()) {
            qDebug() << "cannot load file due to: [" << cond.text() << "].";
            return loop;
        }

        cond = dfile->loadAllDataIntoMemory();
        if (cond.bad()) {
            qDebug() << "Error loading all DICOM tags into memory: [" << cond.text() << "].";
            return loop;
        }

        for(auto tag : DcmTagFnMap.keys()) {
            loop->mDcmTagValues[tag] = DcmTagFnMap[tag](std::ref(dfile));
        }

        loop->LoadFrames();

        loop->mIsValid = true;
        return loop;
    }

    const QString CineLoop::GetDcmValuesAsHtml() const
    {
        QString htmlTable("<table border='1' align='center'>");
        htmlTable.append("<tr bgcolor='yellow'><th>Dcm Tag Key</th><th>Dcm Tag Value</th></tr>");
        bool odd = true;
        for(auto tag : GetDcmValues().keys()) {
            if(odd)
                htmlTable.append("<tr bgcolor='#9acd32'>");
            else
                htmlTable.append("<tr>");
            odd = !odd;
            htmlTable.append("<td min-width='50' align=left>").append(DcmTagIdToString[tag]).append("</td>");
            htmlTable.append("<td min-width='50' align=left>").append(GetDcmValues()[tag]).append("</td>");
            htmlTable.append("</tr>");
        }
        htmlTable.append("</table>");
        return htmlTable;
    }

    void CineLoop::LoadDcmDictionary()
    {
        if (!dcmDataDict.isDictionaryLoaded()) {
            throw std::runtime_error("dcm dictionary NOT loaded");
        }
    }

    void CineLoop::LoadFrames()
    {
        auto samples_per_pixel = mDcmTagValues[SAMPLES_PER_PIXEL].toUInt();
        auto img_depth = samples_per_pixel * 8;
        auto img_rows = mDcmTagValues[ROWS].toUInt();
        auto img_cols = mDcmTagValues[COLS].toUInt();

        std::unique_ptr<DicomImage> dcm_img =
                std::make_unique<DicomImage>(mFileInfo.absoluteFilePath().toLatin1(), CIF_UsePartialAccessToPixelData, 0, 1);

        if(dcm_img->getStatus() != EIS_Normal)
            throw std::runtime_error("dcm_img FAILED");

        dcm_img->setMinMaxWindow();
        do {
            uchar* img_buffer = new uchar[samples_per_pixel * img_rows * img_cols];

            dcm_img->getOutputData(img_buffer, img_rows * img_cols, img_depth, 0, 0);

            QSharedPointer<QImage> qimg(new QImage(img_buffer, dcm_img->getWidth(), dcm_img->getHeight(), QImage::Format_Grayscale8));

            mDcmFrames.push_back(qimg);
        } while (dcm_img->processNextFrames());
    }
}
