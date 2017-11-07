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

ros::Subscriber sub;
ros::Publisher pub;
halcon_bridge::HalconImagePtr img;
bool inited;

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
    string to_string(int _number){
      stringstream ss;
      ss << _number;
      return ss.str();
    }
    HalconCpp::HWindow w;

    void callback(const sensor_msgs::Image& source){
      HObject  ho_Image, ho_Regions, ho_ImageMean;
      halcon_bridge::HalconImagePtr halcon_bridge_imagePointer = halcon_bridge::toHalconCopy(source);
      ho_Image = *halcon_bridge_imagePointer->image;

      if(!inited && _display){
        Hlong width,height;
        ((HImage)ho_Image).GetImageSize(&width,&height);
        w = HWindow(_offsetY,_offsetX,width,height);
        inited = true;
      }


      Threshold(ho_Image, &ho_Regions, _value, 255);
      RegionToMean(ho_Regions, ho_Image, &ho_ImageMean);


      if(_display){
        w.ClearWindow();
        ((HRegion)ho_Regions).DispRegion(w);
      }

      halcon_bridge_imagePointer->image = new HalconCpp::HImage(ho_ImageMean);
      sensor_msgs::ImagePtr image_message = halcon_bridge_imagePointer->toImageMsg();
      pub.publish(image_message);
    }

    void cycle(){
    }

    void init(int argc, char **argv){
    	ros::init(argc, argv, "treshold_link");
      ros::NodeHandle nhCom;
    	ros::NodeHandle nh(nhCom, ros::this_node::getName());

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

      std::cout << _algorithm_ns + "/" + to_string(_order-1) << '\n';
      if(_lastInLine){
        std::cout << "im number " << ros::this_node::getName() << '\n';
      	pub = nhCom.advertise<sensor_msgs::Image>(_algorithm_ns + "/" + _output, 10);
      }
      else{
        pub = nhCom.advertise<sensor_msgs::Image>(_algorithm_ns + "/" + to_string(_order), 10);
      }

      if(_firstInLine){
  	   sub = nhCom.subscribe(_input, 1000, callback);
      }
      else{
  	   sub = nhCom.subscribe(_algorithm_ns + "/" + to_string(_order-1), 1000, callback);
      }

      inited = false;
    }
  }

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
