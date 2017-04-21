#ifndef XRFDCMTAG_H
#define XRFDCMTAG_H

#include <dcmtk/dcmdata/dcdeftag.h>

#include <QHash>

namespace xrf {

    enum DcmTagId {
        IMAGE_TYPE,
        INSTANCE_NUMBER,
        ROWS,
        COLS,
        IMAGER_PIXEL_SPACING,
        FRAME_TIME,
        ISOCENTER_TABLE_POSITION,
        DISTANCE_OBJECT_TO_TABLE,
        DISTANCE_SOURCE_TO_DETECTOR,
        DISTANCE_SOURCE_TO_PATIENT,
        DISTANCE_SOURCE_TO_ISOCENTER,
        POSITION_PRIMARY_ANGLE,
        POSITION_SECONDARY_ANGLE,
        DETECTOR_ROTATION_ANGLE
    };


    static QHash<DcmTagId, DcmTagKey> DcmTagIdToKeyMap = {
        {IMAGE_TYPE, DCM_ImageType},
        {INSTANCE_NUMBER, DCM_InstanceNumber},
        {ROWS, DCM_Rows},
        {COLS, DCM_Columns},
        {IMAGER_PIXEL_SPACING, DCM_ImagerPixelSpacing},
        {FRAME_TIME, DCM_FrameTime},
        {ISOCENTER_TABLE_POSITION, DcmTagKey(0x0021,0x1057)},
        {DISTANCE_OBJECT_TO_TABLE, DcmTagKey(0x0021,0x1058)},
        {DISTANCE_SOURCE_TO_DETECTOR, DCM_DistanceSourceToDetector},
        {DISTANCE_SOURCE_TO_PATIENT, DCM_DistanceSourceToPatient},
        {DISTANCE_SOURCE_TO_ISOCENTER, DcmTagKey(0x0021, 0x1017)},
        {POSITION_PRIMARY_ANGLE, DCM_PositionerPrimaryAngle},
        {POSITION_SECONDARY_ANGLE, DCM_PositionerSecondaryAngle},
        {DETECTOR_ROTATION_ANGLE, DcmTagKey(0x0021,0x1071)},
    };

    static QHash<DcmTagId, QString> DcmTagIdToString = {
        {IMAGE_TYPE, "ImageType"},
        {INSTANCE_NUMBER, "InstanceNumber"},
        {ROWS, "Rows"},
        {COLS, "Columns"},
        {IMAGER_PIXEL_SPACING, "ImagerPixelSpacing"},
        {FRAME_TIME, "FrameTime"},
        {ISOCENTER_TABLE_POSITION, "IsocenterTablePosition"},
        {DISTANCE_OBJECT_TO_TABLE, "ObjectToTable"},
        {DISTANCE_SOURCE_TO_DETECTOR, "DistanceSourceToDetector"},
        {DISTANCE_SOURCE_TO_PATIENT, "DistanceSourceToPatient"},
        {DISTANCE_SOURCE_TO_ISOCENTER, "DistanceSourceToIsocenter"},
        {POSITION_PRIMARY_ANGLE, "PositionerPrimaryAngle"},
        {POSITION_SECONDARY_ANGLE, "PositionerSecondaryAngle"},
        {DETECTOR_ROTATION_ANGLE, "DetectorRotationAngle"},
    };
}
#endif // XRFDCMTAG_H
