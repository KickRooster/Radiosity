#pragma once
#include "Color.h"

namespace Core
{
    struct LinearColor
    {
        float R;
        float G;
        float B;
        float A;

        LinearColor();
        LinearColor(float InR, float InG, float InB, float InA);
        LinearColor& operator /= (float Scale);
        LinearColor& operator *= (float Scale);
        Color ToRGBM() const;
    };
}