import cv2
import numpy as np
import rospy
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
from geometry_msgs.msg import Point

class image_converter:

    def __init__(self, kp, des):
        self.publisher = rospy.Publisher("pos_ugv",Point)
        self.detected = False
        self.counter = 0
        self.bridge = CvBridge()
        self.image_sub = rospy.Subscriber("/ardrone/image_raw",Image,self.callback)
        self.tracker = cv2.TrackerKCF_create()
        self.kp = kp
        self.des = des
        

    def callback(self,data):
        try:
            cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError as e:
            print(e)
       
        if not self.detected:
            ok, box = self.detect_ugv(cv_image)
            if ok:
                self.tracker = cv2.TrackerKCF_create()
                self.tracker.init(cv_image, box)
                self.detected = True
                '''p1 = (int(box[0]), int(box[1]))
                p2 = (int(box[0]+box[2]), int(box[1]+box[3]))
                cv2.rectangle(cv_image,p1,p2,(0,0,255),2)
                x = (box[0]+box[2]/2.0)-cv_image[0].shape[1]/2.
                y = (box[1]+box[3]/2.0)-cv_image.shape[0]/2.
                center = Point(x, y, 0.0)'''
                center = Point(0.0, 0.0, 0.0)
            else:
                center = Point(0.0, 0.0, 0.0)
                cv2.putText(cv_image, "Tracking failure detected", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0,0,255),2)
        else:
            ok, box = self.tracker.update(cv_image)       
            if ok:
                self.counter += 1
                if self.counter > 5:
                    p1 = (int(box[0]), int(box[1]))
                    p2 = (int(box[0]+box[2]), int(box[1]+box[3]))
                    cv2.rectangle(cv_image,p1,p2,(0,0,255),2)
                    x = int(box[0]+box[2]/2.0)
                    y = int(box[1]+box[3]/2.0)
                    cv2.circle(cv_image, (x, y), 3,(255,0,0), thickness=-1)
                    center = Point(x-cv_image.shape[1]/2., y-cv_image.shape[0]/2., 0.0)
                else:
                    center = Point(0.0, 0.0, 0.0)
            else:
                self.counter = 0
                center = Point(0.0, 0.0, 0.0)
                self.detected = False
                cv2.putText(cv_image, "Tracking failure detected", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0,0,255),2)
        
        self.publisher.publish(center)
        
        cv2.imshow("Image window", cv_image)
        cv2.waitKey(1)
            
            
    def checkWithOrb(self,img,x,y,w,h):
        
        orb = cv2.ORB_create(nfeatures=10000, scoreType=cv2.ORB_FAST_SCORE)
        mask = np.zeros(img.shape[:2])
        mask[y:y+h,x:x+w] = 1
        
        kp2 = orb.detect(img, mask)
        
        kp2, des2 = orb.compute(img, kp2)
        
        bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
        # Match descriptors.
        print(type(self.des),type(des2))
        '''matches = bf.match(self.des, des2)
        # Sort them in the order of their distance.
        
        matches = sorted(matches, key = lambda x:x.distance)
        # Draw first 10 matches.
        # res = np.zeros(2)
        # res = cv2.drawMatches(ugv,kp1,img2,kp2,matches[:5], res,flags=2)
        # plt.imshow(res)
        # plt.show()
        if len(matches) >= 5 and matches[4].distance < 40:
            #cv2.rectangle(img2,(x,y),(x+w,y+h),(255,0,0),2)  
            return True
        else:
            return False '''
        return True     
            
    def detect_ugv(self, img):
        # Detecting by forground and background
        '''gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray,(5,5),0)
        ret,th = cv2.threshold(blur,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
        im2, contours, hierarchy = cv2.findContours(th, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        c_max = None
        area = 0
        for c in contours:
            a = cv2.contourArea(c)
            if a > area:
                c_max = c
                area = a
        x,y,w,h = cv2.boundingRect(c_max)'''
        # Detecting by yellow color
        blur = cv2.GaussianBlur(img,(5,5),0)
        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)
        lower_yellow = np.array([10, 50, 50])
        upper_yellow = np.array([80, 255, 255]) # 80 240 240
        mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
        kernel = np.ones((5,5),np.uint8)
        opening = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
        dilation = cv2.dilate(opening,kernel)
        im2, contours, hierarchy = cv2.findContours(dilation, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        if len(contours) > 0:
            c_max = None
            area = 0
            for c in contours:
                a = cv2.contourArea(c)
                if a > area and a > 3000:
                    c_max = c
                    area = a
            if c_max is None:
                return False, None
            x,y,w,h = cv2.boundingRect(c_max)
            #if self.checkWithOrb(img,x,y,w,h):
            return True, (x,y,w,h)
        return False, None

def main():
    rospy.init_node('pos_ugv')
    kp = np.load('/home/victor/catkin_ws/src/landing_drone/src/landing_drone/kp.npy')
    des = np.load('/home/victor/catkin_ws/src/landing_drone/src/landing_drone/des.npy')
    ic = image_converter(kp, des)
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
        cv2.destroyAllWindows()
        

