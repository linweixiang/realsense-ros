
(cl:in-package :asdf)

(defsystem "car-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "car_status" :depends-on ("_package_car_status"))
    (:file "_package_car_status" :depends-on ("_package"))
  ))