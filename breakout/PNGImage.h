#pragma once
#include "Texture.h"
#include <string>
#include "RenderEngine.h"

class PNGImage : public Texture
{
public:
    PNGImage(const std::string &filename);

    virtual ~PNGImage(void);

    bool Load(RenderEngine &engine);

    inline void UnLoad() {Free();}

protected:
    std::string _filename;
};

