#include "body.hpp"

Body::Body() : is_text(false), is_chunk(false), content_length(0), boundary_("")
{
    ;
}

void Body::set_boundary(const char *boundary)
{
    this->boundary_ = boundary;
}

std::string &Body::boundary()
{
    return (boundary_);
}
