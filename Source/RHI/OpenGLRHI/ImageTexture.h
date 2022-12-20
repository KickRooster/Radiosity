#pragma once
#include <BaseInclude.h>
#include "BaseGL.h"
#include "GLTexture.h"

namespace Core
{
    class GLImageTexture : public GLTexture
    {
        GLImageUnit m_unit;

    public:
        GLImageTexture(GLTextureTarget target, GLInternalFormat internalFormat, GLPixelFormat pixelFormat, GLDataType dataType, GLTextureWrapMode wrapMode, GLTextureFilterMode filterMode, GLImageUnit Unit, int32 BoundLevel, GLImageAccess Access);
        GLImageUnit GetUnit() const;
        ~GLImageTexture();
    };
}
