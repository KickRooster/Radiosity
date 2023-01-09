#include "Color.h"

namespace Core
{
    Color::Color()
    {
        R = G = B = A = 0;
    }
    
    Color::Color(uint8 InR, uint8 InG, uint8 InB, uint8 InA)
        :R(InR),
    G(InG),
    B(InB),
    A(InA)
    {
    }

    Color& Color::operator*=(uint8 Scale)
    {
        R *= Scale;
        G *= Scale;
        B *= Scale;
        A *= Scale;

        return *this;
    }
}
