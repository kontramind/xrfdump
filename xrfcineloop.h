#ifndef XRFCINELOOP_H
#define XRFCINELOOP_H

#include <dcmtk/dcmdata/dctagkey.h>

#include <QHash>
#include <QString>
#include <QFileInfo>

#include <memory>
#include <functional>

#include "xrfdcmdeftag.h"

class DcmTagKey;
class DcmFileFormat;

namespace xrf {

    using GetDcmTagFnPtr = std::function<QString(std::unique_ptr<DcmFileFormat>&)>;
    using GetDcmTagMap = QHash<DcmTagId, GetDcmTagFnPtr>;
    using DcmTagValues = QHash<DcmTagId, QString>;

    class CineLoop
    {
    public:
        static CineLoop Create(const QString& filename);
        bool IsValid() { return mIsValid; }
        DcmTagValues& GetDcmValues() { return mDcmTagValues; }

        static GetDcmTagMap DcmTagFnMap;

    private:
        CineLoop() = default;

        QFileInfo mFileInfo;
        bool mIsValid = {false};
        DcmTagValues mDcmTagValues;
    };

}

#endif // XRFCINELOOP_H
