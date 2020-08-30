#pragma once

class UCAD3D_DragAndDropComponent;

/*
*
*/
class CAD3D_IDragAndDropObserver
{
public:
    virtual void OnChangeAlongX(UCAD3D_DragAndDropComponent* Dragger, float DeltaX) = 0;
    virtual void OnChangeAlongY(UCAD3D_DragAndDropComponent* Dragger, float DeltaY) = 0;
};