#pragma once

class IRenderElement
{
public:
    virtual void Show(RenderEngine &engine) = 0;

protected:
    IRenderElement() {}
    virtual ~IRenderElement() {}
};
