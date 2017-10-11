#include "PSO.h"


void tracker(Point p1, Point p2);

vector<Point> capturePoint;
Mat img, image;
char* Window_Name = "PSO目标跟踪";
bool flag = false;

void onMouse(int event, int x, int y, int flags, void* param)
{
	if (flag)
		return;
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标和灰度
		Point pt = Point(x, y);
		capturePoint.push_back(pt);
		break;

	}
	if (capturePoint.size() == 2) {
		flag = true;
		Point p2 = capturePoint.back();
		capturePoint.pop_back();
		Point p1 = capturePoint.back();
		capturePoint.pop_back();
		
		Rect rect(p1, p2);
		Mat image_cut = img(rect);
		Mat image_new;
		image_cut.copyTo(image_new);
		Mat dst = image_new.reshape(0, image_new.rows*image_new.cols);	
		Mat tmp = ob[1].targetobserve.t();
		Mat ttmp = dst.t();
		ttmp.convertTo(ttmp, CV_8UC1);
		tmp.push_back(ttmp);
		tmp.convertTo(tmp, CV_8UC1);
		ob[1].targetobserve=tmp.t();

		cout << ob[1].targetobserve.rows << ":" << ob[1].targetobserve.cols << endl;
		
		cout << "OK!" << endl;
		rectangle(image, p1, p2, Scalar(0, 0, 255), 2, 8, 0);
		imshow(Window_Name, image);
	
		tracker(p1, p2);
	}
}


int main()
{
	capturePoint.clear();
	const char* targetImg = "pic4/0001.jpg";
	image = imread(targetImg);
	if (image.empty())
	{
		cout << "图像加载失败！" << endl;
		return -1;
	}
	cout << image.channels() << endl;
	if (image.channels()>2)
		cvtColor(image, img, CV_BGR2GRAY);
	else
		img = image;

	//cvtColor(img, img, CV_GRAY2BGR);
	img.copyTo(image);
	namedWindow(Window_Name, CV_WINDOW_AUTOSIZE);
	imshow(Window_Name, img);
	setMouseCallback(Window_Name, onMouse, 0);
	waitKey(0);
	return 0;
}

