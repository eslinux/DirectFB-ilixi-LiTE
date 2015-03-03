#include <core/Application.h>
#include <ui/Label.h>
#include <string.h>
#include <stdio.h>
#include <types/Enums.h>

#include <Debug.h>
using namespace ilixi;


int
main(int argc, char* argv[])
{

    Application app(&argc, &argv);


    Label *label = new Label("Hello World");


    label->setX(10);
    label->setY(10);

    app.addWidget(label);

    app.exec();
    return 0;
}


/*
 * FontCache::getFCFileName Error: return font_match = NULL
 *
 *
 *
 *
 * */
