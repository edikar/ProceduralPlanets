/*
    src/example4.cpp -- C++ version of an example application that shows
    how to use the OpenGL widget. For a Python implementation, see
    '../python/example4.py'.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "planet.h"

#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <nanogui/glcanvas.h>
#include <iostream>
#include <string>


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::to_string;

using namespace nanogui;


class MyGLCanvas : public nanogui::GLCanvas {
public:
    MyGLCanvas(Widget *parent, Planet ** _planet) : nanogui::GLCanvas(parent) {
        *_planet = planet = new Planet();
    }

    ~MyGLCanvas() {}

    virtual void drawGL() override {
        planet->planetDraw();

        //the widgets dotn't draw well if I dont return this back to GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void toggleNormals(bool state) {
        planet->toggleNormals(state);
    }

    void toggleOcean(bool state) {
        planet->toggleOcean(state);
    }

    void toggleWireframe(bool state) {
        planet->toggleWireframe(state);
    }

    int randomizePlanet(int detalization) {
        return planet->randomizePlanet(detalization);
    }

    void setDetalization(int detalization) {
        planet->setDetalization(detalization);
    }

    void setNoiseSeed(int seed) {
        planet->setNoiseSeed(seed);
    }

    //shape noise
    void setShapeNoiseLayers(int value) {
        planet->setShapeNoiseLayers(value);
    }
    void setShapeNoiseFrequency(float value) {
        planet->setShapeNoiseFrequency(value);
    }
    void setShapeNoiseAmplitude(float value) {
        planet->setShapeNoiseAmplitude(value);
    }
    void setShapeNoiseLacunarity(float value) {
        planet->setShapeNoiseLacunarity(value);
    }
    void setShapeNoisePersistance(float value) {
        planet->setShapeNoisePersistance(value);
    }
    void setShapeNoiseOffset(float value) {
        planet->setShapeNoiseOffset(value);
    }

    //detail noise
    void setDetNoiseLayers(int value) {
        planet->setDetNoiseLayers(value);
    }
    void setDetNoiseFrequency(float value) {
        planet->setDetNoiseFrequency(value);
    }
    void setDetNoiseAmplitude(float value) {
        planet->setDetNoiseAmplitude(value);
    }
    void setDetNoiseLacunarity(float value) {
        planet->setDetNoiseLacunarity(value);
    }
    void setDetNoisePersistance(float value) {
        planet->setDetNoisePersistance(value);
    }
    void setDetNoiseOffset(float value) {
        planet->setDetNoiseOffset(value);
    }


    //ridge noise
    void setRidgeNoiseLayers(int value) {
        planet->setRidgeNoiseLayers(value);
    }
    void setRidgeNoiseFrequency(float value) {
        planet->setRidgeNoiseFrequency(value);
    }
    void setRidgeNoiseAmplitude(float value) {
        planet->setRidgeNoiseAmplitude(value);
    }
    void setRidgeNoiseLacunarity(float value) {
        planet->setRidgeNoiseLacunarity(value);
    }
    void setRidgeNoisePersistance(float value) {
        planet->setRidgeNoisePersistance(value);
    }
    void setRidgeNoiseVShift(float value) {
        planet->setRidgeNoiseVShift(value);
    }
    void setRidgeNoisePower(int value) {
        planet->setRidgeNoisePower(value);
    }
    void setRidgeNoiseOffset(float value) {
        planet->setRidgeNoiseOffset(value);
    }

    void setColor(int colNum, vec3 color){
        planet->setColor(colNum, color);
    }
    void setColorHeight(int colNum, float height){
        planet->setColorHeight(colNum, height);
    }
    void setColorBlendingRange(int colNum, float range){
        planet->setColorBlendingRange(colNum, range);
    }

    void setSteepnessThreshold(float value){
        planet->setSteepnessThreshold(value);
    }

    void setRockBlendingFactor(float value){
        planet->setRockBlendingFactor(value);
    }

    void setNormalMapScale(float value){
        planet->setNormalMapScale(value);
    }

    void setViewport(unsigned int vx, unsigned int vy){
        planet->setViewport(vx,vy);
    }

    void setPostProcKernel(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22){
        planet->setPostProcKernel(
            m00, m01, m02,
            m10, m11, m12,
            m20, m21, m22
            );
    }

    void togglePostProc(bool state){
        planet->togglePostProc(state);
    }

private:
    Planet *planet;
};


class ExampleApplication : public nanogui::Screen {

private:
    MyGLCanvas *mCanvas;
    Planet *planet;

    void createPlanetParamWidget(Widget *parent){
        Widget *planetWidget = new Widget(parent);
        planetWidget->setLayout(new GridLayout(Orientation::Horizontal, 2 , Alignment::Minimum, 0, 5));
        
        /* detalization factor */ 
        new Label(planetWidget, "Planet Detalization :", "sans");
        auto intBox = new IntBox<int>(planetWidget);
        intBox->setEditable(true);
        intBox->setFixedSize(Vector2i(70, 20));
        intBox->setValue(5);
        intBox->setUnits("ui");
        intBox->setDefaultValue("5");
        intBox->setFontSize(16);
        intBox->setFormat("[1-9][0-9]*");
        intBox->setSpinnable(true);
        intBox->setMinValue(0);
        intBox->setValueIncrement(1);
        intBox->setCallback([this](int value) { mCanvas->setDetalization(value); return true; });

        /* noise seed */ 
        new Label(planetWidget, "Noise seed :", "sans");
        auto seedIB = new IntBox<int>(planetWidget);
        seedIB->setEditable(true);
        seedIB->setFixedSize(Vector2i(70, 20));
        seedIB->setValue(1);
        seedIB->setUnits("ui");
        seedIB->setDefaultValue("1");
        seedIB->setFontSize(16);
        seedIB->setFormat("[1-9][0-9]*");
        seedIB->setCallback([this](int value) { mCanvas->setNoiseSeed(value); return true; });
        
        Button *genPlanetBtn = new Button(planetWidget, "RandomizePlanet");
        genPlanetBtn->setCallback([this, intBox, seedIB]() { int newSeed = mCanvas->randomizePlanet(intBox->value());  seedIB->setValue(newSeed); });

    }

    void createRidgeNoiseParamsWidget(Widget *parent) {

        Widget *widget = new Widget(parent);
        widget->setLayout(new GridLayout(Orientation::Horizontal, 2 , Alignment::Minimum, 0, 5));

        /* noise layers */
        new Label(widget, "# Layers:", "sans");
        auto layersIB = new IntBox<int>(widget);
        layersIB->setEditable(true);
        layersIB->setFixedSize(Vector2i(70, 20));
        layersIB->setValue(0);
        layersIB->setUnits("ui");
        layersIB->setDefaultValue("0");
        layersIB->setFontSize(16);
        layersIB->setFormat("[1-9][0-9]*");
        layersIB->setSpinnable(true);
        layersIB->setMinValue(0);
        layersIB->setValueIncrement(1);
        layersIB->setCallback([this](int value) { mCanvas->setRidgeNoiseLayers(value); return true; });


        /* lacunarity */ 
        new Label(widget, "Lacunarity:", "sans");
        auto lacunarityFB = new FloatBox<float>(widget);
        lacunarityFB->setEditable(true);
        lacunarityFB->setFixedSize(Vector2i(70, 20));
        lacunarityFB->setValue(2.0f);
        lacunarityFB->setUnits("uf");
        lacunarityFB->setDefaultValue("2.0");
        lacunarityFB->setFontSize(16);
        lacunarityFB->setFormat("[0-9]*\\.?[0-9]+");
        lacunarityFB->setSpinnable(true);
        lacunarityFB->setMinValue(0);
        lacunarityFB->setValueIncrement(0.1f);
        lacunarityFB->setCallback([this](float value) { mCanvas->setRidgeNoiseLacunarity(value); return true; });
        

        /* persistance */ 
        new Label(widget, "Persistance:", "sans");
        auto persistanceFB = new FloatBox<float>(widget);
        persistanceFB->setEditable(true);
        persistanceFB->setFixedSize(Vector2i(70, 20));
        persistanceFB->setValue(0.05f);
        persistanceFB->setUnits("uf");
        persistanceFB->setDefaultValue("0.05");
        persistanceFB->setFontSize(16);
        persistanceFB->setFormat("[0-9]*\\.?[0-9]+");
        persistanceFB->setSpinnable(true);
        persistanceFB->setMinValue(0);
        persistanceFB->setValueIncrement(0.1f);
        persistanceFB->setCallback([this](float value) { mCanvas->setRidgeNoisePersistance(value); return true; });
        
        /* FP frequency */ 
        new Label(widget, "Frequency:", "sans");
        auto frequencyTb = new FloatBox<float>(widget);
        frequencyTb->setEditable(true);
        frequencyTb->setFixedSize(Vector2i(70, 20));
        frequencyTb->setValue(1.0f);
        frequencyTb->setUnits("uf");
        frequencyTb->setDefaultValue("1.0");
        frequencyTb->setFontSize(16);
        frequencyTb->setFormat("[0-9]*\\.?[0-9]+");
        frequencyTb->setSpinnable(true);
        frequencyTb->setMinValue(0);
        frequencyTb->setValueIncrement(0.1f);
        frequencyTb->setCallback([this](float value) { mCanvas->setRidgeNoiseFrequency(value); return true; });

        /* FP amplitude */ 
        new Label(widget, "Amplitude:", "sans");
        auto amplitudeTb = new FloatBox<float>(widget);
        amplitudeTb->setEditable(true);
        amplitudeTb->setFixedSize(Vector2i(70, 20));
        amplitudeTb->setValue(0.7f);
        amplitudeTb->setUnits("uf");
        amplitudeTb->setDefaultValue("0.7");
        amplitudeTb->setFontSize(16);
        //amplitudeTb->setFormat("[-]?[0-9]*\\.?[0-9]+");
        amplitudeTb->setFormat("[0-9]*\\.?[0-9]+");
        amplitudeTb->setSpinnable(true);
        amplitudeTb->setMinValue(0);
        amplitudeTb->setValueIncrement(0.1f);
        amplitudeTb->setCallback([this](float value) { mCanvas->setRidgeNoiseAmplitude(value); return true; });    


        /* vertical shift */ 
        new Label(widget, "Vertical shift:", "sans");
        auto vOffsetFB = new FloatBox<float>(widget);
        vOffsetFB->setEditable(true);
        vOffsetFB->setFixedSize(Vector2i(70, 20));
        vOffsetFB->setValue(2.0f);
        vOffsetFB->setUnits("uf");
        vOffsetFB->setDefaultValue("2.0");
        vOffsetFB->setFontSize(16);
        vOffsetFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        vOffsetFB->setSpinnable(true);
        //vOffsetFB->setMinValue(0);
        vOffsetFB->setValueIncrement(0.1f);
        vOffsetFB->setCallback([this](float value) { mCanvas->setRidgeNoiseVShift(value); return true; });    

        /* noise layers */
        new Label(widget, "Power:", "sans");
        auto powerIB = new IntBox<int>(widget);
        powerIB->setEditable(true);
        powerIB->setFixedSize(Vector2i(70, 20));
        powerIB->setValue(10);
        powerIB->setUnits("ui");
        powerIB->setDefaultValue("10");
        powerIB->setFontSize(16);
        powerIB->setFormat("[1-9][0-9]*");
        powerIB->setSpinnable(true);
        powerIB->setMinValue(0);
        powerIB->setValueIncrement(1);
        powerIB->setCallback([this](int value) { mCanvas->setRidgeNoisePower(value); return true; });

        /* offset */ 
        new Label(widget, "Offset:", "sans");
        auto offsetFb = new FloatBox<float>(widget);
        offsetFb->setEditable(true);
        offsetFb->setFixedSize(Vector2i(70, 20));
        offsetFb->setValue(0.0f);
        offsetFb->setUnits("uf");
        offsetFb->setDefaultValue("0.0");
        offsetFb->setFontSize(16);
        offsetFb->setFormat("[-]?[0-9]*\\.?[0-9]+");
        offsetFb->setFormat("[0-9]*\\.?[0-9]+");
        offsetFb->setSpinnable(true);
        //offsetFb->setMinValue(0);
        offsetFb->setValueIncrement(0.01f);
        offsetFb->setCallback([this](float value) { mCanvas->setRidgeNoiseOffset(value); return true; });    


    }
    void createDetNoiseParamsWidget(Widget *parent) {

        Widget *widget = new Widget(parent);
        widget->setLayout(new GridLayout(Orientation::Horizontal, 2 , Alignment::Minimum, 0, 5));

        /* noise layers */
        new Label(widget, "# Layers:", "sans");
        auto layersIB = new IntBox<int>(widget);
        layersIB->setEditable(true);
        layersIB->setFixedSize(Vector2i(70, 20));
        layersIB->setValue(5);
        layersIB->setUnits("ui");
        layersIB->setDefaultValue("5");
        layersIB->setFontSize(16);
        layersIB->setFormat("[1-9][0-9]*");
        layersIB->setSpinnable(true);
        layersIB->setMinValue(0);
        layersIB->setValueIncrement(1);
        layersIB->setCallback([this](int value) { mCanvas->setDetNoiseLayers(value); return true; });


        /* lacunarity */ 
        new Label(widget, "Lacunarity:", "sans");
        auto lacunarityFB = new FloatBox<float>(widget);
        lacunarityFB->setEditable(true);
        lacunarityFB->setFixedSize(Vector2i(70, 20));
        lacunarityFB->setValue(2.0f);
        lacunarityFB->setUnits("uf");
        lacunarityFB->setDefaultValue("2.0");
        lacunarityFB->setFontSize(16);
        lacunarityFB->setFormat("[0-9]*\\.?[0-9]+");
        lacunarityFB->setSpinnable(true);
        lacunarityFB->setMinValue(0);
        lacunarityFB->setValueIncrement(0.1f);
        lacunarityFB->setCallback([this](float value) { mCanvas->setDetNoiseLacunarity(value); return true; });
        

        /* persistance */ 
        new Label(widget, "Persistance:", "sans");
        auto persistanceFB = new FloatBox<float>(widget);
        persistanceFB->setEditable(true);
        persistanceFB->setFixedSize(Vector2i(70, 20));
        persistanceFB->setValue(0.5f);
        persistanceFB->setUnits("uf");
        persistanceFB->setDefaultValue("0.5");
        persistanceFB->setFontSize(16);
        persistanceFB->setFormat("[0-9]*\\.?[0-9]+");
        persistanceFB->setSpinnable(true);
        persistanceFB->setMinValue(0);
        persistanceFB->setValueIncrement(0.1f);
        persistanceFB->setCallback([this](float value) { mCanvas->setDetNoisePersistance(value); return true; });
        
        /* FP frequency */ 
        new Label(widget, "Frequency:", "sans");
        auto frequencyTb = new FloatBox<float>(widget);
        frequencyTb->setEditable(true);
        frequencyTb->setFixedSize(Vector2i(70, 20));
        frequencyTb->setValue(1.0f);
        frequencyTb->setUnits("uf");
        frequencyTb->setDefaultValue("1.0");
        frequencyTb->setFontSize(16);
        frequencyTb->setFormat("[0-9]*\\.?[0-9]+");
        frequencyTb->setSpinnable(true);
        frequencyTb->setMinValue(0);
        frequencyTb->setValueIncrement(0.1f);
        frequencyTb->setCallback([this](float value) { mCanvas->setDetNoiseFrequency(value); return true; });

        /* FP amplitude */ 
        new Label(widget, "Amplitude:", "sans");
        auto amplitudeTb = new FloatBox<float>(widget);
        amplitudeTb->setEditable(true);
        amplitudeTb->setFixedSize(Vector2i(70, 20));
        amplitudeTb->setValue(0.3f);
        amplitudeTb->setUnits("uf");
        amplitudeTb->setDefaultValue("0.3");
        amplitudeTb->setFontSize(16);
        //amplitudeTb->setFormat("[-]?[0-9]*\\.?[0-9]+");
        amplitudeTb->setFormat("[0-9]*\\.?[0-9]+");
        amplitudeTb->setSpinnable(true);
        amplitudeTb->setMinValue(0);
        amplitudeTb->setValueIncrement(0.1f);
        amplitudeTb->setCallback([this](float value) { mCanvas->setDetNoiseAmplitude(value); return true; });    

        /* offset */ 
        new Label(widget, "Offset:", "sans");
        auto offsetFb = new FloatBox<float>(widget);
        offsetFb->setEditable(true);
        offsetFb->setFixedSize(Vector2i(70, 20));
        offsetFb->setValue(0.0f);
        offsetFb->setUnits("uf");
        offsetFb->setDefaultValue("0.0");
        offsetFb->setFontSize(16);
        offsetFb->setFormat("[-]?[0-9]*\\.?[0-9]+");
        offsetFb->setFormat("[0-9]*\\.?[0-9]+");
        offsetFb->setSpinnable(true);
        //offsetFb->setMinValue(0);
        offsetFb->setValueIncrement(0.01f);
        offsetFb->setCallback([this](float value) { mCanvas->setDetNoiseOffset(value); return true; });    


    }


    void createShapeNoiseParamsWidget(Widget *parent) {

        Widget *widget = new Widget(parent);
        widget->setLayout(new GridLayout(Orientation::Horizontal, 2 , Alignment::Minimum, 0, 5));

        /* noise layers */
        new Label(widget, "# Layers:", "sans");
        auto layersIB = new IntBox<int>(widget);
        layersIB->setEditable(true);
        layersIB->setFixedSize(Vector2i(70, 20));
        layersIB->setValue(0);
        layersIB->setUnits("ui");
        layersIB->setDefaultValue("0");
        layersIB->setFontSize(16);
        layersIB->setFormat("[1-9][0-9]*");
        layersIB->setSpinnable(true);
        layersIB->setMinValue(0);
        layersIB->setValueIncrement(1);
        layersIB->setCallback([this](int value) { mCanvas->setShapeNoiseLayers(value); return true; });


        /* lacunarity */ 
        new Label(widget, "Lacunarity:", "sans");
        auto lacunarityFB = new FloatBox<float>(widget);
        lacunarityFB->setEditable(true);
        lacunarityFB->setFixedSize(Vector2i(70, 20));
        lacunarityFB->setValue(1.0f);
        lacunarityFB->setUnits("uf");
        lacunarityFB->setDefaultValue("1.0");
        lacunarityFB->setFontSize(16);
        lacunarityFB->setFormat("[0-9]*\\.?[0-9]+");
        lacunarityFB->setSpinnable(true);
        lacunarityFB->setMinValue(0);
        lacunarityFB->setValueIncrement(0.1f);
        lacunarityFB->setCallback([this](float value) { mCanvas->setShapeNoiseLacunarity(value); return true; });
        

        /* persistance */ 
        new Label(widget, "Persistance:", "sans");
        auto persistanceFB = new FloatBox<float>(widget);
        persistanceFB->setEditable(true);
        persistanceFB->setFixedSize(Vector2i(70, 20));
        persistanceFB->setValue(1.0f);
        persistanceFB->setUnits("uf");
        persistanceFB->setDefaultValue("1.0");
        persistanceFB->setFontSize(16);
        persistanceFB->setFormat("[0-9]*\\.?[0-9]+");
        persistanceFB->setSpinnable(true);
        persistanceFB->setMinValue(0);
        persistanceFB->setValueIncrement(0.1f);
        persistanceFB->setCallback([this](float value) { mCanvas->setShapeNoisePersistance(value); return true; });
        
        /* FP frequency */ 
        new Label(widget, "Frequency:", "sans");
        auto frequencyTb = new FloatBox<float>(widget);
        frequencyTb->setEditable(true);
        frequencyTb->setFixedSize(Vector2i(70, 20));
        frequencyTb->setValue(0.7f);
        frequencyTb->setUnits("uf");
        frequencyTb->setDefaultValue("0.7");
        frequencyTb->setFontSize(16);
        frequencyTb->setFormat("[0-9]*\\.?[0-9]+");
        frequencyTb->setSpinnable(true);
        frequencyTb->setMinValue(0);
        frequencyTb->setValueIncrement(0.1f);
        frequencyTb->setCallback([this](float value) { mCanvas->setShapeNoiseFrequency(value); return true; });

        /* FP amplitude */ 
        new Label(widget, "Amplitude:", "sans");
        auto amplitudeTb = new FloatBox<float>(widget);
        amplitudeTb->setEditable(true);
        amplitudeTb->setFixedSize(Vector2i(70, 20));
        amplitudeTb->setValue(0.5f);
        amplitudeTb->setUnits("uf");
        amplitudeTb->setDefaultValue("0.5");
        amplitudeTb->setFontSize(16);
        //amplitudeTb->setFormat("[-]?[0-9]*\\.?[0-9]+");
        amplitudeTb->setFormat("[0-9]*\\.?[0-9]+");
        amplitudeTb->setSpinnable(true);
        amplitudeTb->setMinValue(0);
        amplitudeTb->setValueIncrement(0.1f);
        amplitudeTb->setCallback([this](float value) { mCanvas->setShapeNoiseAmplitude(value); return true; });    

        /* offset */ 
        new Label(widget, "Offset:", "sans");
        auto offsetFb = new FloatBox<float>(widget);
        offsetFb->setEditable(true);
        offsetFb->setFixedSize(Vector2i(70, 20));
        offsetFb->setValue(0.0f);
        offsetFb->setUnits("uf");
        offsetFb->setDefaultValue("0.0");
        offsetFb->setFontSize(16);
        offsetFb->setFormat("[-]?[0-9]*\\.?[0-9]+");
        offsetFb->setFormat("[0-9]*\\.?[0-9]+");
        offsetFb->setSpinnable(true);
        //offsetFb->setMinValue(0);
        offsetFb->setValueIncrement(0.1f);
        offsetFb->setCallback([this](float value) { mCanvas->setShapeNoiseOffset(value); return true; });    
    }



    void createToolsWindow() {
        //Tools window
        Widget *toolsWindow = new Window(this, "Tools");
        //toolsWindow->setFixedSize(Vector2i(300,300));
        toolsWindow->setPosition(Vector2i(1005, 0));
        toolsWindow->setLayout(new GridLayout(Orientation::Horizontal, 1 , Alignment::Minimum, 0, 5));

        new Label(toolsWindow, "Planet Params", "sans-bold", 20);
        createPlanetParamWidget(toolsWindow);

        new Label(toolsWindow, " ", "sans-bold", 20); //dummy label for spacing
        new Label(toolsWindow, "Shape Noise Params", "sans-bold", 20);
        createShapeNoiseParamsWidget(toolsWindow);
        
        new Label(toolsWindow, " ", "sans-bold", 20); //dummy label for spacing
        new Label(toolsWindow, "Ridge Noise Params", "sans-bold", 20);
        createRidgeNoiseParamsWidget(toolsWindow);
        
        new Label(toolsWindow, " ", "sans-bold", 20); //dummy label for spacing
        new Label(toolsWindow, "Detail Noise Params", "sans-bold", 20);
        createDetNoiseParamsWidget(toolsWindow);

        
        new Label(toolsWindow, " ", "sans-bold", 20); //dummy label for spacing
        new Label(toolsWindow, "Draw Parameters", "sans-bold", 20);
        Button *b0 = new Button(toolsWindow, "Show normals");
        b0->setFlags(Button::ToggleButton);
        b0->setChangeCallback([this](bool state) { mCanvas->toggleNormals(state); });

        Button *b1 = new Button(toolsWindow, "Show ocean");
        b1->setFlags(Button::ToggleButton);
        b1->setPushed(true);
        b1->setChangeCallback([this](bool state) { mCanvas->toggleOcean(state); });

        Button *b2 = new Button(toolsWindow, "Show wireframe");
        b2->setFlags(Button::ToggleButton);
        b2->setChangeCallback([this](bool state) { mCanvas->toggleWireframe(state); });

    }


    void createColorsWindow() {

        Widget *window = new Window(this, "Color Picker");
        window->setPosition(Vector2i(10, 650));
        window->setLayout(new GridLayout(Orientation::Horizontal, 1 , Alignment::Minimum, 0, 5));


        Widget *colorsWidget = new Widget(window);
        colorsWidget->setLayout(new GridLayout(Orientation::Horizontal, 4 , Alignment::Minimum, 0, 5));
        new Label(colorsWidget, "| Color |", "sans-bold");
        new Label(colorsWidget, "| ColorValue |", "sans-bold");
        new Label(colorsWidget, "| StartHeight |", "sans-bold");
        new Label(colorsWidget, "| BlendingRange |", "sans-bold");

        new Label(colorsWidget, "Color 0:", "sans-bold");
        auto CP0 = new ColorPicker(colorsWidget, Color(0.0f, 0.0f, 0.1f, 1.0f));
        CP0->setFixedSize({100, 20});
        CP0->setFinalCallback([this](const Color &c) { mCanvas->setColor(0, vec3(c.r(), c.g(), c.b()));  printf("%f %f %f\n", c.r(), c.g(), c.b()); });

        auto C0_heightFB = new FloatBox<float>(colorsWidget);
        C0_heightFB->setEditable(true);
        C0_heightFB->setFixedSize(Vector2i(70, 20));
        C0_heightFB->setValue(0.0f);
        C0_heightFB->setUnits("uf");
        C0_heightFB->setDefaultValue("0.0");
        C0_heightFB->setFontSize(16);
        C0_heightFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        C0_heightFB->setFormat("[0-9]*\\.?[0-9]+");
        C0_heightFB->setSpinnable(true);
        C0_heightFB->setMinValue(0);
        C0_heightFB->setValueIncrement(0.01f);
        C0_heightFB->setCallback([this](float value) { mCanvas->setColorHeight(0, value); return true; }); 
        new Label(colorsWidget, " ", "sans-bold"); //dummy

        new Label(colorsWidget, "Color 1:", "sans-bold");
        auto CP1 = new ColorPicker(colorsWidget, Color(0.6f, 0.6f, 0.1f, 1.0f));
        CP1->setFixedSize({100, 20});
        CP1->setFinalCallback([this](const Color &c) { mCanvas->setColor(1, vec3(c.r(), c.g(), c.b())); });

        auto C1_heightFB = new FloatBox<float>(colorsWidget);
        C1_heightFB->setEditable(true);
        C1_heightFB->setFixedSize(Vector2i(70, 20));
        C1_heightFB->setValue(0.9f);
        C1_heightFB->setUnits("uf");
        C1_heightFB->setDefaultValue("0.9");
        C1_heightFB->setFontSize(16);
        C1_heightFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        C1_heightFB->setFormat("[0-9]*\\.?[0-9]+");
        C1_heightFB->setSpinnable(true);
        C1_heightFB->setMinValue(0);
        C1_heightFB->setValueIncrement(0.01f);
        C1_heightFB->setCallback([this](float value) { mCanvas->setColorHeight(1, value); return true; }); 

        auto C1_blendingRangeFB = new FloatBox<float>(colorsWidget);
        C1_blendingRangeFB->setEditable(true);
        C1_blendingRangeFB->setFixedSize(Vector2i(70, 20));
        C1_blendingRangeFB->setValue(0.03f);
        C1_blendingRangeFB->setUnits("uf");
        C1_blendingRangeFB->setDefaultValue("0.03");
        C1_blendingRangeFB->setFontSize(16);
        C1_blendingRangeFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        C1_blendingRangeFB->setFormat("[0-9]*\\.?[0-9]+");
        C1_blendingRangeFB->setSpinnable(true);
        C1_blendingRangeFB->setMinValue(0);
        C1_blendingRangeFB->setValueIncrement(0.01f);
        C1_blendingRangeFB->setCallback([this](float value) { mCanvas->setColorBlendingRange(1, value); return true; }); 

        new Label(colorsWidget, "Color 2:", "sans-bold");
        auto CP2 = new ColorPicker(colorsWidget, Color(0.1f, 0.5f, 0.1f, 1.0f));
        CP2->setFixedSize({100, 20});
        CP2->setFinalCallback([this](const Color &c) { mCanvas->setColor(2, vec3(c.r(), c.g(), c.b())); });

        auto C2_heightFB = new FloatBox<float>(colorsWidget);
        C2_heightFB->setEditable(true);
        C2_heightFB->setFixedSize(Vector2i(70, 20));
        C2_heightFB->setValue(1.05f);
        C2_heightFB->setUnits("uf");
        C2_heightFB->setDefaultValue("1.05");
        C2_heightFB->setFontSize(16);
        C2_heightFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        C2_heightFB->setFormat("[0-9]*\\.?[0-9]+");
        C2_heightFB->setSpinnable(true);
        C2_heightFB->setMinValue(0);
        C2_heightFB->setValueIncrement(0.01f);
        C2_heightFB->setCallback([this](float value) { mCanvas->setColorHeight(2, value); return true; }); 

        auto C2_blendingRangeFB = new FloatBox<float>(colorsWidget);
        C2_blendingRangeFB->setEditable(true);
        C2_blendingRangeFB->setFixedSize(Vector2i(70, 20));
        C2_blendingRangeFB->setValue(0.03f);
        C2_blendingRangeFB->setUnits("uf");
        C2_blendingRangeFB->setDefaultValue("0.03");
        C2_blendingRangeFB->setFontSize(16);
        C2_blendingRangeFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        C2_blendingRangeFB->setFormat("[0-9]*\\.?[0-9]+");
        C2_blendingRangeFB->setSpinnable(true);
        C2_blendingRangeFB->setMinValue(0);
        C2_blendingRangeFB->setValueIncrement(0.01f);
        C2_blendingRangeFB->setCallback([this](float value) { mCanvas->setColorBlendingRange(2, value); return true; }); 

        new Label(colorsWidget, "Color 3 :", "sans-bold");
        auto CP3 = new ColorPicker(colorsWidget, Color(1.0f, 1.0f, 1.0f, 1.0f));
        CP3->setFixedSize({100, 20});
        CP3->setFinalCallback([this](const Color &c) { mCanvas->setColor(3, vec3(c.r(), c.g(), c.b())); });

        auto C3_heightFB = new FloatBox<float>(colorsWidget);
        C3_heightFB->setEditable(true);
        C3_heightFB->setFixedSize(Vector2i(70, 20));
        C3_heightFB->setValue(1.2f);
        C3_heightFB->setUnits("uf");
        C3_heightFB->setDefaultValue("1.2");
        C3_heightFB->setFontSize(16);
        C3_heightFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        C3_heightFB->setFormat("[0-9]*\\.?[0-9]+");
        C3_heightFB->setSpinnable(true);
        C3_heightFB->setMinValue(0);
        C3_heightFB->setValueIncrement(0.01f);
        C3_heightFB->setCallback([this](float value) { mCanvas->setColorHeight(3, value); return true; }); 

        auto C3_blendingRangeFB = new FloatBox<float>(colorsWidget);
        C3_blendingRangeFB->setEditable(true);
        C3_blendingRangeFB->setFixedSize(Vector2i(70, 20));
        C3_blendingRangeFB->setValue(0.03f);
        C3_blendingRangeFB->setUnits("uf");
        C3_blendingRangeFB->setDefaultValue("0.03");
        C3_blendingRangeFB->setFontSize(16);
        C3_blendingRangeFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        C3_blendingRangeFB->setFormat("[0-9]*\\.?[0-9]+");
        C3_blendingRangeFB->setSpinnable(true);
        C3_blendingRangeFB->setMinValue(0);
        C3_blendingRangeFB->setValueIncrement(0.01f);
        C3_blendingRangeFB->setCallback([this](float value) { mCanvas->setColorBlendingRange(3, value); return true; }); 

        new Label(colorsWidget, "Color 4:", "sans-bold");
        auto CP4 = new ColorPicker(colorsWidget, Color(0.5f, 0.2f, 0.0f, 1.0f));
        CP4->setFixedSize({100, 20});
        CP4->setFinalCallback([this](const Color &c) { mCanvas->setColor(4, vec3(c.r(), c.g(), c.b())); });
        new Label(colorsWidget, "* rocks *", "sans-bold");//dummy
        new Label(colorsWidget, " ", "sans-bold");//dummy

        Button *btn = new Button(window, "RandomizeColors");
        btn->setCallback([this, CP0, CP1, CP2, CP3, CP4]() { 
            Color c;
            CP0->setColor(Color((float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, 1.0f));
            c = CP0->color();
            mCanvas->setColor(0, vec3(c.r(), c.g(), c.b()));
            CP1->setColor(Color((float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, 1.0f));
            c = CP1->color();
            mCanvas->setColor(1, vec3(c.r(), c.g(), c.b()));
            CP2->setColor(Color((float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, 1.0f));
            c = CP2->color();
            mCanvas->setColor(2, vec3(c.r(), c.g(), c.b()));
            CP3->setColor(Color((float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, 1.0f));
            c = CP3->color();
            mCanvas->setColor(3, vec3(c.r(), c.g(), c.b()));
            CP4->setColor(Color((float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, (float)(rand() % 1000) / 1000, 1.0f));
            c = CP4->color();
            mCanvas->setColor(4, vec3(c.r(), c.g(), c.b()));
            
        });


        Widget *steepnessWidget = new Widget(window);
        steepnessWidget->setLayout(new GridLayout(Orientation::Horizontal, 2 , Alignment::Minimum, 0, 5));
        
        /* offset */ 
        new Label(steepnessWidget, "Steepness threshold *:", "sans");
        auto steepnesFB = new FloatBox<float>(steepnessWidget);
        steepnesFB->setEditable(true);
        steepnesFB->setFixedSize(Vector2i(70, 20));
        steepnesFB->setValue(0.9f);
        steepnesFB->setUnits("uf");
        steepnesFB->setDefaultValue("0.9");
        steepnesFB->setFontSize(16);
        steepnesFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        steepnesFB->setFormat("[0-9]*\\.?[0-9]+");
        steepnesFB->setSpinnable(true);
        steepnesFB->setMinValue(0);
        steepnesFB->setMaxValue(1);
        steepnesFB->setValueIncrement(0.01f);
        steepnesFB->setCallback([this](float value) { mCanvas->setSteepnessThreshold(value); return true; });    

        /* rock blending factor */ 
        new Label(steepnessWidget, "rock blending factor:", "sans");
        auto rockBlendFactorFB = new FloatBox<float>(steepnessWidget);
        rockBlendFactorFB->setEditable(true);
        rockBlendFactorFB->setFixedSize(Vector2i(70, 20));
        rockBlendFactorFB->setValue(0.04f);
        rockBlendFactorFB->setUnits("uf");
        rockBlendFactorFB->setDefaultValue("0.04");
        rockBlendFactorFB->setFontSize(16);
        rockBlendFactorFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        rockBlendFactorFB->setFormat("[0-9]*\\.?[0-9]+");
        rockBlendFactorFB->setSpinnable(true);
        rockBlendFactorFB->setMinValue(0);
        rockBlendFactorFB->setMaxValue(1);
        rockBlendFactorFB->setValueIncrement(0.01f);
        rockBlendFactorFB->setCallback([this](float value) { mCanvas->setRockBlendingFactor(value); return true; });    

        /* normal map scale */ 
        new Label(steepnessWidget, "nMap scale factor:", "sans");
        auto nMapScaleFactorFB = new FloatBox<float>(steepnessWidget);
        nMapScaleFactorFB->setEditable(true);
        nMapScaleFactorFB->setFixedSize(Vector2i(70, 20));
        nMapScaleFactorFB->setValue(1.0f);
        nMapScaleFactorFB->setUnits("uf");
        nMapScaleFactorFB->setDefaultValue("1.0");
        nMapScaleFactorFB->setFontSize(16);
        nMapScaleFactorFB->setFormat("[-]?[0-9]*\\.?[0-9]+");
        nMapScaleFactorFB->setFormat("[0-9]*\\.?[0-9]+");
        nMapScaleFactorFB->setSpinnable(true);
        nMapScaleFactorFB->setMinValue(0);
        //nMapScaleFactorFB->setMaxValue(1);
        nMapScaleFactorFB->setValueIncrement(0.1f);
        nMapScaleFactorFB->setCallback([this](float value) { mCanvas->setNormalMapScale(value); return true; });    
        
        auto commentLabel = new Label(window, "*** there is a bug with normal map texture: the wrapipng is incorrect - there are artifacts", "sans");
        commentLabel->setColor(Color(1.0f, 1.0f, 0.0f, 1.0f));


    }


void createPostProcessingWindow() {

    Widget *window = new Window(this, "Post Processing Kernel");
    window->setPosition(Vector2i(500, 800));
    window->setLayout(new GridLayout(Orientation::Horizontal, 1 , Alignment::Minimum, 0, 5));

    Button *b1 = new Button(window, "Use PostProcessing Kernel");
    b1->setFlags(Button::ToggleButton);
    b1->setPushed(false);
    b1->setChangeCallback([this](bool state) { mCanvas->togglePostProc(state); });

    Widget *matrixWidget = new Widget(window);
    matrixWidget->setLayout(new GridLayout(Orientation::Horizontal, 3 , Alignment::Minimum, 0, 5));
    
/* offset */ 
    auto    mat_00_FB = new FloatBox<float>(matrixWidget);
    mat_00_FB->setEditable(true);
    mat_00_FB->setFixedSize(Vector2i(70, 20));
    mat_00_FB->setValue(0.0f);
    mat_00_FB->setUnits("f");
    mat_00_FB->setDefaultValue("0.0");
    mat_00_FB->setFontSize(16);
    mat_00_FB->setFormat("[-]?[0-9]*\\.?[0-9]+");
    mat_00_FB->setCallback([this](float value) { return true; });    


    auto    mat_01_FB = new FloatBox<float>(matrixWidget);
    mat_01_FB->setEditable(true);
    mat_01_FB->setFixedSize(Vector2i(70, 20));
    mat_01_FB->setValue(0.0f);
    mat_01_FB->setUnits("f");
    mat_01_FB->setDefaultValue("0.0");
    mat_01_FB->setFontSize(16);
    mat_01_FB->setFormat("[-]?[0-9]*\\.?[0-9]+");
    mat_01_FB->setCallback([this](float value) { return true; });    

    auto    mat_02_FB = new FloatBox<float>(matrixWidget);
    mat_02_FB->setEditable(true);
    mat_02_FB->setFixedSize(Vector2i(70, 20));
    mat_02_FB->setValue(0.0f);
    mat_02_FB->setUnits("f");
    mat_02_FB->setDefaultValue("0.0");
    mat_02_FB->setFontSize(16);
    mat_02_FB->setFormat("[-]?[0-9]*\\.?[0-9]+");
    mat_02_FB->setCallback([this](float value) { return true; });    

    auto    mat_10_FB = new FloatBox<float>(matrixWidget);
    mat_10_FB->setEditable(true);
    mat_10_FB->setFixedSize(Vector2i(70, 20));
    mat_10_FB->setValue(0.0f);
    mat_10_FB->setUnits("f");
    mat_10_FB->setDefaultValue("0.0");
    mat_10_FB->setFontSize(16);
    mat_10_FB->setFormat("[-]?[0-9]*\\.?[0-9]+");
    mat_10_FB->setCallback([this](float value) { return true; });    


    auto    mat_11_FB = new FloatBox<float>(matrixWidget);
    mat_11_FB->setEditable(true);
    mat_11_FB->setFixedSize(Vector2i(70, 20));
    mat_11_FB->setValue(1.0f);
    mat_11_FB->setUnits("f");
    mat_11_FB->setDefaultValue("1.0");
    mat_11_FB->setFontSize(16);
    mat_11_FB->setFormat("[-]?[0-9]*\\.?[0-9]+");
    mat_11_FB->setCallback([this](float value) { return true; });    

    auto    mat_12_FB = new FloatBox<float>(matrixWidget);
    mat_12_FB->setEditable(true);
    mat_12_FB->setFixedSize(Vector2i(70, 20));
    mat_12_FB->setValue(0.0f);
    mat_12_FB->setUnits("f");
    mat_12_FB->setDefaultValue("0.0");
    mat_12_FB->setFontSize(16);
    mat_12_FB->setFormat("[-]?[0-9]*\\.?[0-9]+");
    mat_12_FB->setCallback([this](float value) { return true; });    

    auto    mat_20_FB = new FloatBox<float>(matrixWidget);
    mat_20_FB->setEditable(true);
    mat_20_FB->setFixedSize(Vector2i(70, 20));
    mat_20_FB->setValue(0.0f);
    mat_20_FB->setUnits("f");
    mat_20_FB->setDefaultValue("0.0");
    mat_20_FB->setFontSize(16);
    mat_20_FB->setFormat("[-]?[0-9]*\\.?[0-9]+");
    mat_20_FB->setCallback([this](float value) { return true; });    


    auto    mat_21_FB = new FloatBox<float>(matrixWidget);
    mat_21_FB->setEditable(true);
    mat_21_FB->setFixedSize(Vector2i(70, 20));
    mat_21_FB->setValue(0.0f);
    mat_21_FB->setUnits("f");
    mat_21_FB->setDefaultValue("0.0");
    mat_21_FB->setFontSize(16);
    mat_21_FB->setFormat("[-]?[0-9]*\\.?[0-9]+");
    mat_21_FB->setCallback([this](float value) { return true; });    

    auto mat_22_FB = new FloatBox<float>(matrixWidget);
    mat_22_FB->setEditable(true);
    mat_22_FB->setFixedSize(Vector2i(70, 20));
    mat_22_FB->setValue(0.0f);
    mat_22_FB->setUnits("f");
    mat_22_FB->setDefaultValue("0.0");
    mat_22_FB->setFontSize(16);
    mat_22_FB->setFormat("[-]?[0-9]*\\.?[0-9]+");
    mat_22_FB->setCallback([this](float value) { return true; });    


    Button *btn = new Button(window, "Set Kernel");
    btn->setCallback([this, mat_00_FB, mat_01_FB, mat_02_FB, mat_10_FB, mat_11_FB, mat_12_FB, mat_20_FB, mat_21_FB, mat_22_FB]() { 
        mCanvas->setPostProcKernel(
            mat_00_FB->value(), mat_01_FB->value(), mat_02_FB->value(),
            mat_10_FB->value(), mat_11_FB->value(), mat_12_FB->value(),
            mat_20_FB->value(), mat_21_FB->value(), mat_22_FB->value()
            );
    });
    
    auto commentLabel = new Label(window, "*** there is a bug with texture: doesnt show the full image", "sans");
    commentLabel->setColor(Color(1.0f, 1.0f, 0.0f, 1.0f));
    }


    void createGLCanvas() {
        //GL Canvas window
        Window *window = new Window(this, "Procedural Planets");
        window->setPosition(Vector2i(0, 0));
        window->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 5));//new GroupLayout());

        mCanvas = new MyGLCanvas(window, &planet);
        mCanvas->setBackgroundColor({100, 100, 100, 255});
        mCanvas->setSize({1000, 800});

    }
public:
    ExampleApplication() : nanogui::Screen(Eigen::Vector2i(1429, 989), "NanoGUI Test", false) {
        using namespace nanogui;

        createGLCanvas();
        createToolsWindow();
        createColorsWindow();
        createPostProcessingWindow();
        
        performLayout();
    }

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) {
        if (Screen::keyboardEvent(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            setVisible(false);
            return true;
        }

        switch(key){
            case GLFW_KEY_W:
            case GLFW_KEY_S:
            case GLFW_KEY_A: 
            case GLFW_KEY_D:
            case GLFW_KEY_LEFT:
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_UP:
            case GLFW_KEY_DOWN:
                planet->processInput(key, action);
        }

        return false;
    }

    virtual void draw(NVGcontext *ctx) {
        /* Draw the user interface */
        Screen::draw(ctx);
    }
};

int main(int /* argc */, char ** /* argv */) {
    try {
        nanogui::init();

        /* scoped variables */ {
            nanogui::ref<ExampleApplication> app = new ExampleApplication();
            app->drawAll();
            app->setVisible(true);
            
            nanogui::mainloop();
        }

        nanogui::shutdown();
    } catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        #if defined(_WIN32)
            MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
        #else
            std::cerr << error_msg << endl;
        #endif
        return -1;
    }

    return 0;
}
