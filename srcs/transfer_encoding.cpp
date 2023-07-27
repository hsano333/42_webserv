#include "utility.hpp"

const char* TransferEncoding::TRANSFER_ENCODING_TYPE_STRS[] = {"chunked", "compress", "deflate",
                                                               "gzip",    "identity", ""};

TransferEncoding::TransferEncoding() : _type(NONE) {}

TransferEncoding::TransferEncoding(TransferEncoding const& t) : _type(t.get_type()) {}

TransferEncoding::TransferEncoding(string transfer_encoding_str)
{
    for (int i = 0; i < TYPE_COUNT; i++) {
        if (TRANSFER_ENCODING_TYPE_STRS[i] == transfer_encoding_str) {
            _type = static_cast<TRANSFER_ENCODING_TYPE>(i);
            return;
        }
    }
    throw std::runtime_error("unsupported transfer encoding type: '" + transfer_encoding_str + "'");
}

TransferEncoding::TRANSFER_ENCODING_TYPE TransferEncoding::get_type() const
{
    return _type;
}

string TransferEncoding::get_str() const
{
    return TRANSFER_ENCODING_TYPE_STRS[_type];
}

TransferEncoding::~TransferEncoding() {}

TransferEncoding const& TransferEncoding::operator=(TransferEncoding const& x)
{
    if (this != &x)
        _type = x.get_type();
    return *this;
}

bool TransferEncoding::operator==(string type) const
{
    return get_str() == type;
}

bool TransferEncoding::operator==(TransferEncoding te) const
{
    return _type == te.get_type();
}

bool TransferEncoding::operator==(TransferEncoding::TRANSFER_ENCODING_TYPE te) const
{
    return _type == te;
}

bool TransferEncoding::operator!=(string type) const
{
    return !((*this) == type);
}

bool TransferEncoding::operator!=(TransferEncoding te) const
{
    return !((*this) == te);
}

bool TransferEncoding::operator!=(TransferEncoding::TRANSFER_ENCODING_TYPE te) const
{
    return !((*this) == te);
}
