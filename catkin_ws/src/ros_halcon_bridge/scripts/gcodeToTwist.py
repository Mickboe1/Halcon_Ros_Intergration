#!/usr/bin/env python
import rospy
import re
from std_msgs.msg import Empty
from geometry_msgs.msg import Twist

class ClassName():
    def __init__(self):
        rospy.init_node('gcode_twist_parser', anonymous=True)

        # rospy.Subscriber('subscriber_topic', Empty, self.callback_function)
        self.pub = rospy.Publisher('gcode_to_twist', Twist, queue_size=10)

        self.filename = rospy.get_param('filename', "/home/mickboe1/Documents/input.gcode")
        self.freq = float(rospy.get_param('freq', 1))
        self.speed = float(rospy.get_param('speed', 0.1))
        self.message = Twist()
        self.rate = rospy.Rate(self.freq)
        self.metric = 0.001

        self.init()
        self.run()


    def init(self):
        self.file = open(self.filename, "r")


    def moveLinear(self, line):
        message.linear.x = line[1] * self.metric
        message.linear.y = line[2] * self.metric
        self.rate.sleep()

    def moveAngular(self, line):
        message.linear.x = lineSplit[1] * self.metric
        message.linear.y = lineSplit[2] * self.metric
        self.pub.publish(message)
        self.rate.sleep()


    def run(self):
        while not rospy.is_shutdown():
            for line in self.file:
                message = Twist()
                removedLetters = re.sub('[XYIJ]', '', line)
                lineSplit = re.sub(' +',' ', removedLetters).replace("\r\n", "").split(" ")

                if(lineSplit[0] == "G1"):
                    self.moveLinear(lineSplit)
                    # coordinates = [lineSplit[1], lineSplit[2]]



                elif(lineSplit[0] == "G2"):
                    self.moveAngular(lineSplit)



if __name__ == '__main__':
    try:
        cn = ClassName()
    except rospy.ROSInterruptException:
        pass
    except Exception as e:
        print e
