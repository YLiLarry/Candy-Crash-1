/*
    Note that the Model should NOT hold any pointer to TextView.
    Model should hold pointers to View (the super abstract class) instead.
*/

#ifndef __TEXTVIEW__H__
#define __TEXTVIEW__H__

#include "view.h"

    class TextView : public View {};

#endif
