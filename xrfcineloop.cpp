#include "xrfcineloop.h"

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcrledrg.h>
//#include <dcmtk/dcmdata/dcdicdir.h>
//#include "dcmtk/oflog/oflog.h"
//#include "dcmtk/config/osconfig.h"
//#include "dcmtk/dcmdata/dcistrmf.h"
//#include "dcmtk/dcmimgle/dcmimage.h"

#include <memory>
#include <utility>
#include <functional>

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
                            };



    CineLoop CineLoop::Create(const QString &filename)
    {
        CineLoop loop;

        if (!dcmDataDict.isDictionaryLoaded()) {
            qDebug() << "no data dictionary loaded, check environment variable: " << DCM_DICT_ENVIRONMENT_VARIABLE;
            return loop;
        }

        DcmRLEDecoderRegistration::registerCodecs();
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

        loop.mIsValid = true;
        DcmRLEDecoderRegistration::cleanup();
        return loop;
    }
}
