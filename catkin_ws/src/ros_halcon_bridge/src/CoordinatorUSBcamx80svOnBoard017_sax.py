#!/usr/bin/env python

# This script a central coordinator that coordinates the intecation between the three main subcomponents of the mobile manipulator
# 

import roslib
import sys
import rospy
import cv2
import std_srvs
import math
import numpy as np
from std_msgs.msg import Float64
from std_msgs.msg import Char
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Vector3
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Pose
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import tf
import time
from std_srvs.srv import *
#from move_base.srv import *

class coordinator(object):
    def __init__(self):
        rospy.init_node('coordinator', anonymous=False)
        self.pub4x80sv_pose = rospy.Publisher('x80sv2map', PoseStamped, queue_size=100)
        self.pub4x80sv_cmd_vel = rospy.Publisher('cmd_vel', Twist, queue_size=100)
        #self.pub4target_pose = rospy.Publisher('target2map', PoseStamped, queue_size=100)
        self.pub4target_pose = rospy.Publisher('/move_base_simple/goal', PoseStamped, queue_size=100)
        self.pub2manipulationStateCommand = rospy.Publisher('navi2manip', Float64, queue_size=10)
        
        self.publish2Amigo=rospy.Publisher('/amigo/trigger', String, queue_size=10)
        self.publish_vision2manip=rospy.Publisher('vision2manip', Pose, queue_size=10)
        
        self.x80sv_clear_costmap=rospy.ServiceProxy('/move_base/clear_costmaps', std_srvs.srv.Empty)
        
        self.x80sv2amigo=String()
        self.x80sv_pose=PoseStamped()
        self.target_pose=PoseStamped()
        self.target_camera=PoseStamped()
        self.flowstate=Char('Idle')
        self.manipStateCommand=0.0
        #1 == Approach_Target 2== Release_Target 
        
        self.manipStateFB=0.0
        #0 == Home 1==Object_Grasped  2== Object Released
        
        self.target_pose.header.frame_id="map"
        #self.target_pose.header.frame_id="base_link"
        self.target_pose.pose.orientation.w=1
        self.cmd_vel=Twist()
        self.initiateManipulation=0.0
        self.navi2manip = 0.00
        #self.navi2manip = 0.00
        self.manip2navi = Vector3(0.0, 0.0, 0.0)
        self.target2map_tf_br = tf.TransformBroadcaster()
        self.naviStateMachine=Char('Navigate2Target')    
        

        #rospy.Subscriber('target2camera_pose', PoseStamped, self.callback_target2map)
        rospy.Subscriber("target_inPlace", Float64, self.callback_target_inPlace)
        rospy.Subscriber('manip2navi', Float64, self.manipulationStateCallback)
        rospy.Subscriber('vision2manip_virtual', Pose, self.vision2manipulationCallback)
        self.listener = tf.TransformListener()
        self.vision2manip=Pose()
        
    def callback_target_inPlace(self, data_target):
		self.initiateManipulation=data_target.data
		#if self.initiateManipulation == 1.0:
			#print('Approach Target')
    def manipulationStateCallback(self, data_manip2navi):
		print('I received data from manipulation')
		self.manipStateFB=data_manip2navi.data
		print(self.manipStateFB)
    def vision2manipulationCallback(self, data_vision2manip):
		print('I received data from manipulation')
		self.vision2manip=data_vision2manip
		#print(self.vision2manip)

    def spin(self):
		#self.listener = tf.TransformListener()
		
		#r = rospy.Rate(1)
		r = rospy.Rate(2.5)
		while not rospy.is_shutdown():

			#self.flowstate.data='Going_to_trashbin'
					
			try:
				#(self.trans_target,self.rot_target) = self.listener.lookupTransform('map','target2camera', rospy.Time(0))
				(self.trans_target,self.rot_target) = self.listener.lookupTransform('map','target2base', rospy.Time(0))
				(self.trans_target_camera,self.rot_target_camera) = self.listener.lookupTransform('camera','target2camera', rospy.Time(0))
				#print('I am here in the broadcast')
				#(self.trans_target,self.rot_target) = self.listener.lookupTransform('camera', 'map', rospy.Time(0))
				print('I am here in the broadcast')
				print(self.trans_target_camera)
				self.target_camera.pose.position.x=self.trans_target_camera[0]-0.135*0.0
				self.target_camera.pose.position.y=self.trans_target_camera[1]+0.12*0.0
				print(self.target_camera.pose.position.x)
				#self.trans_target=(1, 1, 1)
				#self.rot_target=(1, 1, 1, 1)
				#self.target_pose.header.frame_id="map"
				self.target_pose.header.stamp=rospy.Time.now()
				self.target_pose.pose.position.x=self.trans_target[0] #self.trans_target[0]-0.135-0.05*0.75
				self.target_pose.pose.position.y=self.trans_target[1]#self.trans_target[1]+0.12 -0.06
				self.target_pose.pose.position.z=0 #self.trans_target[2]
				self.target_pose.pose.orientation.x=0 #self.rot_target[0]
				self.target_pose.pose.orientation.y=0 #self.rot_target[1]
				self.target_pose.pose.orientation.z=self.rot_target[2]#0.3*self.rot_target[2]
				self.target_pose.pose.orientation.w=self.rot_target[3]#1 #self.rot_target[3]
			except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
				pass#continue


			if (self.naviStateMachine.data=='Navigate2Target'):
				print('I am navigating')
				if (self.initiateManipulation== 1.0):
					self.cmd_vel.linear.x=0.0
					self.cmd_vel.angular.z=0.0
					self.pub4x80sv_cmd_vel.publish(self.cmd_vel)
					time.sleep(5)
					"""
					self.publish_vision2manip.publish(self.vision2manip)
					print('test')
					self.manipStateCommand=1.0
					self.pub2manipulationStateCommand.publish(self.manipStateCommand)
					"""
					self.naviStateMachine.data='InitiateManipulation'
					#self.publish_vision2manip.publish(self.vision2manip)
			if (self.naviStateMachine.data=='InitiateManipulation'):
				if (self.initiateManipulation== 1.0):
					self.publish_vision2manip.publish(self.vision2manip)
					print('test')
					self.manipStateCommand=1.0
					self.pub2manipulationStateCommand.publish(self.manipStateCommand)
					print('I am manipulating')
					self.cmd_vel.linear.x=0.0
					self.cmd_vel.angular.z=0.0
					self.pub4x80sv_cmd_vel.publish(self.cmd_vel)
					#self.publish_vision2manip.publish(self.vision2manip)
					time.sleep(5)
				elif (self.manipStateCommand!=1.0):
					self.naviStateMachine.data='Navigate2Target'
					time.sleep(2)
				if (self.manipStateFB == 1.0):
					self.naviStateMachine.data='ClearMap'
			if (self.naviStateMachine.data=='ClearMap'):
				print('I am clearing cost map')
				self.x80sv_clear_costmap()
				"""
				self.cmd_vel.linear.x=-0.125
				self.cmd_vel.angular.z=0.125
				self.pub4x80sv_cmd_vel.publish(self.cmd_vel)
				"""
				time.sleep(5)
				self.naviStateMachine.data='Navigate2TrashBin'
			if (self.naviStateMachine.data=='Navigate2TrashBin'):
				print('I am navigating to trash bin')
				self.target_pose.header.stamp=rospy.Time.now()
				self.target_pose.pose.position.x=0.9 #2.0#1.8 #2.6
				self.target_pose.pose.position.y=1.4#0.7#3.7
				self.target_pose.pose.position.z=0 #self.trans_target[2]
				self.target_pose.pose.orientation.x=0 #self.rot_target[0]
				self.target_pose.pose.orientation.y=0 #self.rot_target[1]
				self.target_pose.pose.orientation.z=0.707
				self.target_pose.pose.orientation.w=0.707
				self.pub4target_pose.publish(self.target_pose)
				rospy.loginfo(self.target_pose)
				print('printing move base command....trashbin')
				time.sleep(15)
				self.manipStateCommand=2.0
				self.cmd_vel.linear.x=0.0
				self.cmd_vel.angular.z=0.0
				self.pub4x80sv_cmd_vel.publish(self.cmd_vel)
				self.pub2manipulationStateCommand.publish(self.manipStateCommand)
				self.naviStateMachine.data='DisposeTrash'
			if (self.naviStateMachine.data=='DisposeTrash'):
				print('I am disposing trash')
				if (self.manipStateFB == 2.0):
					time.sleep(10)
					self.naviStateMachine.data='Navigate2Home'
			if (self.naviStateMachine.data=='Navigate2Home'):
				print('I am navigating to home')
				self.target_pose.header.stamp=rospy.Time.now()
				self.target_pose.pose.position.x=0#1.0#0.06
				self.target_pose.pose.position.y=0#0.3#1.0 #1.6 #0
				self.target_pose.pose.position.z=0 #self.trans_target[2]
				self.target_pose.pose.orientation.x=0 #self.rot_target[0]
				self.target_pose.pose.orientation.y=0 #self.rot_target[1]
				self.target_pose.pose.orientation.z=0#-0.707#-0.707#-1.57
				self.target_pose.pose.orientation.w=1#0.707#1 
				self.pub4target_pose.publish(self.target_pose)
				time.sleep(20)
				self.x80sv2amigo='continue'
				self.publish2Amigo.publish(self.x80sv2amigo)
				print(self.x80sv2amigo)
				self.naviStateMachine.data='Navigate2Target'


			#self.target2map_tf_br.sendTransform((self.target_pose.pose.position.x, self.target_pose.pose.position.y, 0),(0, 0, self.target_pose.pose.orientation.z, 1),  rospy.Time.now(), "target2map", "map")
			#if  (self.target_camera.pose.position.x > 0.0 or self.target_camera.pose.position.y !=0.0) and (self.navi2manip==0) and (self.manip2navi.x==0) and (self.flowstate.data=='Idle'): 
			#if  self.target_camera.pose.position.x > 0.05 and (self.navi2manip==0) and (self.manip2navi.x==0) and (self.naviStateMachine.data='Navigate2Target'):
			if  (self.initiateManipulation!=1.0) and (self.target_camera.pose.position.x > 0.05) and (self.naviStateMachine.data=='Navigate2Target'): 
				self.pub4target_pose.publish(self.target_pose)
				rospy.loginfo(self.target_pose)
				print('printing move base command')
			#self.pub4x80sv_pose.publish(self.x80sv_pose)
			#rospy.loginfo(self.x80sv_pose)
			#self.pub2manipulationState.publish(self.navi2manip)
			#print(self.navi2manip)
			"""
			if (self.naviStateMachine.data=='ClearMap'):
				self.cmd_vel.linear.x=-0.125
				self.cmd_vel.angular.z=0.125
				self.pub4x80sv_cmd_vel.publish(self.cmd_vel)

			if (self.naviStateMachine.data=='DisposeTrash'):
				self.x80sv2amigo='continue'
				self.publish2Amigo.publish(self.x80sv2amigo)
				print(self.x80sv2amigo)
				
			if (self.naviStateMachine.data=='Navigate2TrashBin'):
				self.target_pose.header.stamp=rospy.Time.now()
				self.target_pose.pose.position.x=2 #2.6
				self.target_pose.pose.position.y=0.8#3.7
				self.target_pose.pose.position.z=0 #self.trans_target[2]
				self.target_pose.pose.orientation.x=0 #self.rot_target[0]
				self.target_pose.pose.orientation.y=0 #self.rot_target[1]
				self.target_pose.pose.orientation.z=0
				self.target_pose.pose.orientation.w=1 
				self.pub4target_pose.publish(self.target_pose)
				rospy.loginfo(self.target_pose)
				print('printing move base command....trashbin')
				
			if (self.naviStateMachine.data=='Navigate2Home'):
				self.target_pose.header.stamp=rospy.Time.now()
				self.target_pose.pose.position.x=0.06
				self.target_pose.pose.position.y=1.6 #0
				self.target_pose.pose.position.z=0 #self.trans_target[2]
				self.target_pose.pose.orientation.x=0 #self.rot_target[0]
				self.target_pose.pose.orientation.y=0 #self.rot_target[1]
				self.target_pose.pose.orientation.z=0
				self.target_pose.pose.orientation.w=1 
				self.pub4target_pose.publish(self.target_pose)
			"""
			#print(self.flowstate)
			#self.publish2Amigo.publish(self.x80sv2amigo)
			r.sleep()
                    
if __name__ == "__main__":
    coordinator = coordinator()
    coordinator.spin()
