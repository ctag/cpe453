#include "edge.h"

void edge::setDS(QString _ds)
{
    ds = _ds;
}

QString edge::getDS()
{
    return(ds);
}

void edge::setVertTo(vertex * _vert)
{
    vert_to = _vert;
}

vertex* edge::getVertTo()
{
    return(vert_to);
}

void edge::setVertFrom(vertex * _vert)
{
    vert_from = _vert;
}

vertex* edge::getVertFrom()
{
    return(vert_from);
}

void edge::setType(QString _type)
{
    type = _type;
}

QString edge::getType()
{
    return(type);
}
















