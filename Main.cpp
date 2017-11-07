#include "PSO.h"
#define numofarea 2

void tracker();

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
	if (capturePoint.size() == 2*numofarea) {
		flag = true;

		for (int i = 1; i <= numofarea; ++i)
		{
			
			area[i].pr = capturePoint.back();
			capturePoint.pop_back();
			area[i].pl = capturePoint.back();
			capturePoint.pop_back();

			Rect rect(area[i].pl, area[i].pr);
			Mat image_cut = img(rect);
			Mat image_new;
			image_cut.copyTo(image_new);
			Mat dst = image_new.reshape(0, image_new.rows*image_new.cols);
			Mat tmp = ob[i].targetobserve.t();
			Mat ttmp = dst.t();
			ttmp.convertTo(ttmp, CV_8UC1);
			tmp.push_back(ttmp);
			tmp.convertTo(tmp, CV_8UC1);
			ob[i].targetobserve = tmp.t();

			cout << ob[i].targetobserve.rows << ":" << ob[i].targetobserve.cols << endl;

			cout << "OK!" << endl;
			rectangle(image, area[i].pl, area[i].pr, Scalar(0, 0, 255), 2, 8, 0);
			/*
			cout << area[i].pl.x << " " <<  area[i].pl.y << endl;
			cout << area[i].pr.x << " " << area[i].pr.y << endl;
			cout << endl;
			*/
		}
		//rectangle(image, area[1].pl, area[1].pr, Scalar(0, 0, 255), 2, 8, 0);
		imshow(Window_Name, image);
		tracker();
	}
}


int main()
{
	capturePoint.clear();
	const char* targetImg = "pic5/0001.jpg";
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

