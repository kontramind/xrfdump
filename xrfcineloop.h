#ifndef XRFCINELOOP_H
#define XRFCINELOOP_H

#include <dcmtk/dcmdata/dctagkey.h>

#include <QHash>
#include <QImage>
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
        static GetDcmTagMap DcmTagFnMap;
        static CineLoop Create(const QString& filename);

        bool IsValid() const { return mIsValid; }
        const DcmTagValues& GetDcmValues() const { return mDcmTagValues; }
        const std::vector<QSharedPointer<QImage>>& GetFrames() const { return mDcmFrames; }


    private:
        static void LoadDcmDictionary();
        CineLoop() = default;
        void LoadFrames();

        QFileInfo mFileInfo;
        bool mIsValid = {false};
        DcmTagValues mDcmTagValues;
        std::vector<QSharedPointer<QImage>> mDcmFrames;
    };

}

#endif // XRFCINELOOP_H
