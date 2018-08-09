#include "image.h"


int main(int argc, char**argv){

  ros::init(argc, argv, "Image");
  ros::NodeHandle nh;
  pub = nh.advertise<geometry_msgs::Point>("circle_center_data",10);
  sub = nh.subscribe("stepflag",10, callback); 
  /*if(argc>1){
    if(argv[1][0]=='y' || argv[1][0]=='Y'){
      show_frame= true;
    }
  }*/
  VideoCapture cap(0);
  cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
  //ros::Rate rate(30);
  while (cap.isOpened() && ros::ok()){
        cap >> frame;        
        FindCircle();
	pub.publish(msg);
	imshow("Video",frame);
	waitKey(1);
        ros::spinOnce();
        //rate.sleep();
  }
}
//-----------------------------------------------------------------------------------




//--------------------------------------Subroutines----------------------------------

void FindCircle(){
  cvtColor(frame, dst, COLOR_BGR2GRAY);
  GaussianBlur(dst, dst, Size(5,5), 1.3, 1.3);
  HoughCircles(dst, circles, CV_HOUGH_GRADIENT, 2, 50, 200, 100); //30
  for (int i=0; i<circles.size(); i++){
    Point center(circles[i][0],circles[i][1]);   //declare variable that are two integer type variables(x,y).
    radius=cvRound(circles[i][2]);  //rounding the data number before assign to radius.
    circle(frame, center, radius, Scalar(0,0,255), 1, 8, 0);  //drawing a circle and the circle is solid
    circle(frame, center, 1, Scalar(0,255,0), 1, 8, 0); 
    if( (radius >= 0) && (radius <= 100) ){ 
      ROS_INFO("Center X: [%i] , Y: [%i] ",center.x,center.y);
      msg.x = center.x;      
      msg.y = center.y;
      msg.z = radius;
      pub.publish(msg);
      break; 
    }
    else{
      ROS_WARN("Out of Radius ");
    }
  }
}

void callback(const std_msgs::Float64 &msg){
  flag = msg.data;
}
