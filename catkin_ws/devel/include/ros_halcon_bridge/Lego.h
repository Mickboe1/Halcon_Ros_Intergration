// Generated by gencpp from file ros_halcon_bridge/Lego.msg
// DO NOT EDIT!


#ifndef ROS_HALCON_BRIDGE_MESSAGE_LEGO_H
#define ROS_HALCON_BRIDGE_MESSAGE_LEGO_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <geometry_msgs/Pose.h>

namespace ros_halcon_bridge
{
template <class ContainerAllocator>
struct Lego_
{
  typedef Lego_<ContainerAllocator> Type;

  Lego_()
    : header()
    , child_frame_id()
    , pose()
    , width(0.0)
    , lenght(0.0)
    , height(0.0)  {
    }
  Lego_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , child_frame_id(_alloc)
    , pose(_alloc)
    , width(0.0)
    , lenght(0.0)
    , height(0.0)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _child_frame_id_type;
  _child_frame_id_type child_frame_id;

   typedef  ::geometry_msgs::Pose_<ContainerAllocator>  _pose_type;
  _pose_type pose;

   typedef float _width_type;
  _width_type width;

   typedef float _lenght_type;
  _lenght_type lenght;

   typedef float _height_type;
  _height_type height;




  typedef boost::shared_ptr< ::ros_halcon_bridge::Lego_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::ros_halcon_bridge::Lego_<ContainerAllocator> const> ConstPtr;

}; // struct Lego_

typedef ::ros_halcon_bridge::Lego_<std::allocator<void> > Lego;

typedef boost::shared_ptr< ::ros_halcon_bridge::Lego > LegoPtr;
typedef boost::shared_ptr< ::ros_halcon_bridge::Lego const> LegoConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::ros_halcon_bridge::Lego_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::ros_halcon_bridge::Lego_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace ros_halcon_bridge

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'ros_halcon_bridge': ['/home/mickboe1/git/Halcon_Ros_Intergration/catkin_ws/src/ros_halcon_bridge/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::ros_halcon_bridge::Lego_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::ros_halcon_bridge::Lego_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ros_halcon_bridge::Lego_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ros_halcon_bridge::Lego_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ros_halcon_bridge::Lego_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ros_halcon_bridge::Lego_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::ros_halcon_bridge::Lego_<ContainerAllocator> >
{
  static const char* value()
  {
    return "2780f1061ffcc6642373d6a297d20c8f";
  }

  static const char* value(const ::ros_halcon_bridge::Lego_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x2780f1061ffcc664ULL;
  static const uint64_t static_value2 = 0x2373d6a297d20c8fULL;
};

template<class ContainerAllocator>
struct DataType< ::ros_halcon_bridge::Lego_<ContainerAllocator> >
{
  static const char* value()
  {
    return "ros_halcon_bridge/Lego";
  }

  static const char* value(const ::ros_halcon_bridge::Lego_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::ros_halcon_bridge::Lego_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# Lego.msg\n\
Header header\n\
string child_frame_id\n\
geometry_msgs/Pose pose\n\
float32 width\n\
float32 lenght\n\
float32 height\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Pose\n\
# A representation of pose in free space, composed of position and orientation. \n\
Point position\n\
Quaternion orientation\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point\n\
# This contains the position of a point in free space\n\
float64 x\n\
float64 y\n\
float64 z\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Quaternion\n\
# This represents an orientation in free space in quaternion form.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
float64 w\n\
";
  }

  static const char* value(const ::ros_halcon_bridge::Lego_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::ros_halcon_bridge::Lego_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.child_frame_id);
      stream.next(m.pose);
      stream.next(m.width);
      stream.next(m.lenght);
      stream.next(m.height);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Lego_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::ros_halcon_bridge::Lego_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::ros_halcon_bridge::Lego_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "child_frame_id: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.child_frame_id);
    s << indent << "pose: ";
    s << std::endl;
    Printer< ::geometry_msgs::Pose_<ContainerAllocator> >::stream(s, indent + "  ", v.pose);
    s << indent << "width: ";
    Printer<float>::stream(s, indent + "  ", v.width);
    s << indent << "lenght: ";
    Printer<float>::stream(s, indent + "  ", v.lenght);
    s << indent << "height: ";
    Printer<float>::stream(s, indent + "  ", v.height);
  }
};

} // namespace message_operations
} // namespace ros

#endif // ROS_HALCON_BRIDGE_MESSAGE_LEGO_H
