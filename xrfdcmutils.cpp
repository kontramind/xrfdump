#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcrledrg.h>

#include <QVariant>

namespace xrf {

    class DcmDecoder {
    public:
        DcmDecoder() {
            DcmRLEDecoderRegistration::registerCodecs();
        }
        ~DcmDecoder() {
            DcmRLEDecoderRegistration::cleanup();
        }

    };

template<typename F, typename... Ts>
auto prebind(F&& f, Ts&&... args) {
    return [=] (auto&&... xs)
    {
        return f(args..., std::forward<decltype(xs)>(xs)...);
    };
}

QString dcm_tag_read(std::unique_ptr<DcmFileFormat>& dfile, const DcmTagKey& dcmkey, const QVariant& defaultvalue) {
    OFCondition cond;
    switch(defaultvalue.type()) {
    case QVariant::UInt:
    {
        Uint16 value16;
        cond = dfile->getDataset()->findAndGetUint16(dcmkey, value16);
        if(!cond.bad())
            return QString::number(value16);
        Uint32 value32;
        cond = dfile->getDataset()->findAndGetUint32(dcmkey, value32);
        if(!cond.bad())
            return QString::number(value32);
    }
    case QVariant::Int:
    {
        Sint16 value16;
        cond = dfile->getDataset()->findAndGetSint16(dcmkey, value16);
        if(!cond.bad())
            return QString::number(value16);
        Sint32 value32;
        cond = dfile->getDataset()->findAndGetSint32(dcmkey, value32);
        if(!cond.bad())
            return QString::number(value32);
    }
    case QVariant::String:
    {
        const char* value;
        cond = dfile->getDataset()->findAndGetString(dcmkey, value);
        if(!cond.bad())
            return QString(value);
    }
    case QVariant::ByteArray:
    {
        unsigned long count;
        const Sint16* value;
        cond = dfile->getDataset()->findAndGetSint16Array(dcmkey, value, &count);
        if(!cond.bad()) {
            QString temp;
            for(unsigned long i = 0;  i < count - 1; ++i){
                temp.append(QString::number(value[i])).append("\\");
            }
            temp.append(QString::number(value[count-1]));
            return temp;
        }
    }
    default:
        return QString();
    }
    return defaultvalue.toString();
}

}
