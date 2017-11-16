
(cl:in-package :asdf)

(defsystem "ros_halcon_bridge-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Lego" :depends-on ("_package_Lego"))
    (:file "_package_Lego" :depends-on ("_package"))
  ))