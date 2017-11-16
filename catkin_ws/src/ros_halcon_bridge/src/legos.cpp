#include "HalconCpp.h"
#include "HDevThread.h"
#include <stdio.h>
#include <X11/Xlib.h>

#include <string>
#include <iostream>
#include <stdlib.h>
#include "ros/ros.h"
#include <sstream>
#include <math.h>
#include "halcon_image.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/Empty.h"
#include <exception>

using namespace std;
#include <string>
#include "HalconCpp.h"

//Ros global variables
ros::Subscriber sub;
ros::Publisher pub;

//halcon img pointer
halcon_bridge::HalconImagePtr img;

//bool used to init the window
bool inited;
bool frame_inited;

int check       = 0;
float pxbycm    = 17.0;
float pxWidth   = 640;
float pxHeight  = 480;
float cmWidth   = 43;
float cmHeight  = 33;


//variables created by ros parameters
bool    _firstInLine;
string  _input;
bool    _lastInLine;
string  _output;
int     _value;
string  _algorithm_ns;
int     _order;
int     _offsetX;
int     _offsetY;
bool    _display;
int     _mul;

namespace HalconCpp{
    //halconCPP global variables
    HalconCpp::HWindow w;
    HObject ho_Background;

    // Tostring function used to transform numbers to text
    string to_string(int _number){
      stringstream ss;
      ss << _number;
      return ss.str();
    }


    //image callback functions used in the chain or from a camera.
    void callback(const sensor_msgs::Image& source){
      //init the halcon objects used in the algorithm
      HObject  ho_Image, ho_IntersectionBlue, ho_IntersectionGreen;
      HObject  ho_ImageChannel2, ho_IntersectionRed, ho_ImageChannel3;
      HObject  ho_IntersectionYellowRed, ho_Circle, ho_RegionDilation1;
      HObject  ho_RegionErosion1, ho_RegionDilation2, ho_ConnectedRegions;
      HObject  ho_SelectedReq, ho_SelectedReqSized;
      HObject  ho_ContCircleTilt;
      Hlong width,height;

      //getting the image pointer from the asr-halcon-bridge and take the image
      halcon_bridge::HalconImagePtr halcon_bridge_imagePointer = halcon_bridge::toHalconCopy(source);
      ho_Image = *halcon_bridge_imagePointer->image;

      //generating the window if it is enabled.
      if(!inited && _display){
        ((HImage)ho_Image).GetImageSize(&width,&height);
        w = HWindow(_offsetY,_offsetX,width,height);
        inited = true;
      }

      /*
        HERE IS WHERE THE ALGORITHM BEGINS AND CAN BE IF SO WISHED.
        THIS ALGORITHM CONTAINS: needs a steady camera for background extraction,
        the first one frame will be transformed into the background.
      */

     // Local control variables
     HTuple  hv_Rectangularity, hv_Ra, hv_Rb, hv_Phi, hv_Arc;
     HTuple  hv_Area, hv_Column, hv_arrow_length, hv_Cos, hv_Row;
     HTuple  hv_Sin;

     Threshold(ho_Image, &ho_IntersectionBlue, 11, 29);
    //  Threshold(ho_Image, &ho_IntersectionGreen, 34, 55);
    //  //
    //  AccessChannel(ho_Image, &ho_ImageChannel2, 2);
    //  Threshold(ho_ImageChannel2, &ho_IntersectionRed, 22, 36);
    //  //
    //  AccessChannel(ho_Image, &ho_ImageChannel3, 3);
    //  Threshold(ho_ImageChannel3, &ho_IntersectionYellowRed, 13, 42);
     //

      try
      {

        GenCircle(&ho_Circle, 2, 2, 3);
        Dilation1(ho_IntersectionBlue, ho_Circle, &ho_RegionDilation1, 4);
        Erosion1(ho_RegionDilation1, ho_Circle, &ho_RegionErosion1, 4*2);
        Dilation1(ho_RegionErosion1, ho_Circle, &ho_RegionDilation2, 4);
        Connection(ho_RegionDilation2, &ho_ConnectedRegions);
        Rectangularity(ho_ConnectedRegions, &hv_Rectangularity);
        EllipticAxis(ho_RegionDilation2, &hv_Ra, &hv_Rb, &hv_Phi);
        AreaCenter(ho_RegionDilation2, &hv_Area, &hv_Row, &hv_Column);


        ClearWindow(w);
       ((HImage)ho_Image).DispImage(w);
       SetColor(w,"cyan");
       DispLine(w, 235, 315, 245, 325);
       DispLine(w, 235, 325, 245, 315);

        hv_arrow_length = 100;
        hv_Arc = hv_Phi;
        hv_Phi = hv_Phi-1.5707;

        TupleCos(hv_Phi, &hv_Cos);
        TupleSin(hv_Phi, &hv_Sin);
        std::cout << "rb " << hv_Rb.D() << " ra " << hv_Ra.D() << " w " << width << " h " << height <<  '\n';
        std::cout <<  " pxbycm " << pxbycm << " lw " << hv_Rb.D() * 2 / pxbycm << " lh " <<  hv_Ra.D() * 2 / pxbycm <<'\n';
         DispArrow(w, (float)hv_Row, (float)hv_Column, (float)hv_Row+((float)hv_arrow_length*(float)hv_Cos), (float)hv_Column+((float)hv_arrow_length*(float)hv_Sin), 1);


         DispLine(w, 0, hv_Column, hv_Row, hv_Column);
         DispLine(w, hv_Row, 0, hv_Row, hv_Column);
         DispArc(w, hv_Row, hv_Column,  (float)hv_Phi, hv_Row, hv_Column + 50);
         std::cout << "x "<< (float)hv_Column / pxbycm << " y " << (float)hv_Row / pxbycm << " Theta " << (float)hv_Phi <<  '\n';

         /*
           HERE IS WHERE THE ALGORITHM ENDS
         */

         //Display the region if it is enabled.
         // if(_display){
         //   w.ClearWindow();
         //   ((HImage)ho_ImageSub).DispImage(w);
         // }
         //
         // //transform the image back to a ROS-Image message and publish it.
         // halcon_bridge_imagePointer->image = new HalconCpp::HImage(ho_ImageSub);
         // sensor_msgs::ImagePtr image_message = halcon_bridge_imagePointer->toImageMsg();
         // pub.publish(image_message);;
      }
      catch (HalconCpp::HTupleAccessException& e)
      {
        cout <<  "arrow i guess"<< '\n';
      }

    }

    //default cycle function.
    void cycle(){
    }

    //default init function.
    void init(int argc, char **argv){
      //init ross and create public and private nodehandlers.
    	ros::init(argc, argv, "treshold_link");
      ros::NodeHandle nhCom;
    	ros::NodeHandle nh(nhCom, ros::this_node::getName());

      //assign parameters or set defaults.
      if(!nh.getParam("firstInLine", _firstInLine))
        _firstInLine = true;
      if(!nh.getParam("input", _input) && _firstInLine)
        _input = "/image";
      if(!nh.getParam("lastInLine", _lastInLine))
        _lastInLine = true;
      if(!nh.getParam("output", _output) && _lastInLine)
        _output = "/halcon_output_image";
      if(!nh.getParam("algorithm_ns", _algorithm_ns))
        _algorithm_ns = "/halcon";
      if(!nh.getParam("value", _value))
        _value = 128;
      if(!nh.getParam("order", _order))
        _order = 1;
      if(!nh.getParam("display", _display))
        _value = true;
      if(!nh.getParam("offsetX", _offsetX))
        _offsetX = 0;
      if(!nh.getParam("offsetY", _offsetY))
        _offsetY = 0;
      if(!nh.getParam("multiply", _mul))
        _mul = 1;


      // setup the publisher
      if(_lastInLine){
      	pub = nhCom.advertise<sensor_msgs::Image>(_algorithm_ns + "/" + _output, 10);
      }
      else{
        pub = nhCom.advertise<sensor_msgs::Image>(_algorithm_ns + "/" + to_string(_order), 10);
      }

      //setup the subscriber
      if(_firstInLine){
  	   sub = nhCom.subscribe(_input, 1000, callback);
      }
      else{
  	   sub = nhCom.subscribe(_algorithm_ns + "/" + to_string(_order-1), 1000, callback);
      }

      //set the window initzializer to false
      inited = false;
      frame_inited = false;
    }
  }

  //start the program
  int main(int argc, char **argv)
  {
    try
    {
      HalconCpp::init(argc, argv);
      while(ros::ok()){
        HalconCpp::cycle();
        ros::spinOnce();
      }
    }
    catch ( ros::Exception &e )
    {
     ROS_ERROR("Error occured: %s ", e.what());
    }

}
