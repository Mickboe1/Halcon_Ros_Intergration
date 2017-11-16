#include "HalconCpp.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include "ros/ros.h"
#include <sstream>
#include <math.h>
#include "halcon_image.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/Empty.h"

using namespace std;
#include <string>

//Ros global variables
ros::Subscriber sub;
ros::Publisher pub;

//halcon img pointer
halcon_bridge::HalconImagePtr img;

//bool used to init the window
bool inited;

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

namespace HalconCpp{
    //halconCPP global variables
    HalconCpp::HWindow w;

    // Tostring function used to transform numbers to text
    string to_string(int _number){
      stringstream ss;
      ss << _number;
      return ss.str();
    }


    //image callback functions used in the chain or from a camera.
    void callback(const sensor_msgs::Image& source){
      //init the halcon objects used in the algorithm
      HObject  ho_Image, ho_Regions, ho_ImageMean, ho_RegionDilation;

      //getting the image pointer from the asr-halcon-bridge and take the image
      halcon_bridge::HalconImagePtr halcon_bridge_imagePointer = halcon_bridge::toHalconCopy(source);
      ho_Image = *halcon_bridge_imagePointer->image;

      //generating the window if it is enabled.
      if(!inited && _display){
        Hlong width,height;
        ((HImage)ho_Image).GetImageSize(&width,&height);
        w = HWindow(_offsetY,_offsetX,width,height);
        inited = true;
      }

      /*
        HERE IS WHERE THE ALGORITHM BEGINS AND CAN BE IF SO WISHED.
        THIS ALGORITHM CONTAINS: Dilation
        uses circle dilation with the size of the value parameter
      */
      Threshold(ho_Image, &ho_Regions, 10, 255);
      DilationCircle(ho_Regions, &ho_RegionDilation, _value);
      RegionToMean(ho_RegionDilation, ho_Image, &ho_ImageMean);
      /*
        HERE IS WHERE THE ALGORITHM ENDS
      */

      //Display the region if it is enabled.
      if(_display){
        w.ClearWindow();
        ((HRegion)ho_RegionDilation).DispRegion(w);
      }

      //transform the image back to a ROS-Image message and publish it.
      halcon_bridge_imagePointer->image = new HalconCpp::HImage(ho_ImageMean);
      sensor_msgs::ImagePtr image_message = halcon_bridge_imagePointer->toImageMsg();
      pub.publish(image_message);
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
