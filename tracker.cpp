#include "PSO.h"
#include<cmath>
//using namespace std;
#define exp  2.71828182845904523536028747135266249775724709369995
#define numofparticle 40
void tracker(Point p1, Point p2)
{
	srand(time(NULL));
	int width = p2.x - p1.x+1;
	int height = p2.y - p1.y+1;
	double vxmin = -(p2.x - p1.x);
	double vymin = -(p2.y - p1.y);
	double vxmax = (p2.x - p1.x);
	double vymax = (p2.y - p1.y);
	
	

	Mat img;
	
	char targetImg[22], Window_Name[20];
	for (int frame = 2; frame <= 259; frame++) {


		for (int i = 1; i <= numofparticle; ++i)
		{
			pa[1][i].px = pa[1][i].pbestx = pa[1][i].gbestx = rand() % (p2.x - p1.x) + p1.x;
			pa[1][i].py = pa[1][i].pbesty = pa[1][i].gbesty = rand() % (p2.y - p1.y) + p1.y;
			pa[1][i].vx = rand() % ((p2.x - p1.x))- (p2.x - p1.x)/2;
			pa[1][i].vy = rand() % ((p2.y - p1.y))- (p2.y - p1.y)/2;
			pa[1][i].fitness = pa[1][i].pbest=pa[1][i].gbest= -1;
		}



		sprintf_s(targetImg, "pic4/%04d.jpg", frame);
		int index = 259 - 2 + 1;
		sprintf_s(Window_Name, "Frame%d", index);
		Mat image = imread(targetImg);
		if (image.channels()>2)
			cvtColor(image, img, CV_BGR2GRAY);
		else
			img = image;

		img.copyTo(image);
		Rect rect(p1, p2);
		Mat image_cut = img(rect);
		Mat oou;
		image_cut.copyTo(oou);

		Mat image_new;
		Canny(oou, image_new, 30, 150, 3, true);
		Mat dst = image_new.reshape(0, image_new.rows*image_new.cols);
		Mat tmp = ob[1].targetobserve.t();

		
		Mat tmp1;
		/*
		if (tmp.rows == 20&&0)
		{
			tmp1 = tmp(Rect(Point(0, 1), Point(tmp.cols - 1, 20)));
		}
		else
		{
			tmp1 = tmp;
		}
		*/
		tmp1 = tmp;
		Mat ttmp = dst.t();
		tmp1.convertTo(tmp1, CV_8UC1);
		tmp1.push_back(ttmp);
		ob[1].targetobserve = tmp1.t();

		ob[1].targetobserve.convertTo(ob[1].targetobserve, CV_32FC1);
		Mat U, W, V;
		SVD::compute(ob[1].targetobserve, W, U, V);
		
		int idofgbest;
		Mat UU = U*U.t();

		Mat img1;
		cvtColor(img, img1, CV_GRAY2BGR);


		for (int z = 1; z <= 10; ++z)
		{
			//printf("µÚ%d´Îµü´ú\n", z);

			for (int i = 1; i <= numofparticle; ++i)
			{

				//cout << "before" << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << endl;
				//cout << width << " " << height << endl;
				p1.x = pa[1][i].px - width / 2;
				p1.y = pa[1][i].py - height / 2;
				p2.x = pa[1][i].px + width / 2 - (width & 1 ? 0 : 1);
				p2.y = pa[1][i].py + height / 2 - (height & 1 ? 0 : 1);

				//cout << "after" << p1.x << " " << p1.y << " " << p2.x << " " << p2.y<<endl;

				Rect rect(p1, p2);
				Mat image_cut = img(rect);
				Mat image_new;
				image_cut.copyTo(image_new);
				Mat dst = image_new.reshape(0, image_new.rows*image_new.cols);
				float res = 0;
				dst.convertTo(dst, CV_32FC1);
			
				//cout << dst.rows << " " << dst.cols << endl;
				//cout << U.rows << " " << U.cols << endl;

				Mat ot = dst - UU*dst;

				for (int i = 0; i < ot.rows; i++)
				{
					const float* pData = ot.ptr<float>(i);
					for (int j = 0; j < ot.cols; j++)
						res += pData[j] * pData[j];
				}

				//printf("RE=%f\n", sqrt(res));


				pa[1][i].fitness = pow(exp, -sqrt(res));

				//cout << "fitness=" << pa[1][i].fitness << endl;


				if (pa[1][i].fitness > pa[1][i].pbest)
				{
					pa[1][i].pbest = pa[1][i].fitness;
					pa[1][i].pbestx = pa[1][i].px;
					pa[1][i].pbesty = pa[1][i].py;
				}
			}
			double maxa = -1;
			double gx, gy;
			for (int i = 1; i <= numofparticle; ++i)
			{
				if (pa[1][i].pbest > maxa)
				{
					maxa = pa[1][i].pbest;
					gx = pa[1][i].pbestx;
					gy = pa[1][i].pbesty;
					idofgbest = i;
				}
			}
			for (int i = 1; i <= numofparticle; ++i)
			{
				pa[1][i].gbest = maxa;
				pa[1][i].gbestx = gx;
				pa[1][i].gbesty = gy;
			}

			//printf("gbest=%f\n", pa[1][1].gbest);
			cout << "gbest=" << pa[1][1].gbest << endl;


			double w = 0.729844;
			//double w = 0.4;
			double fi1 = rand() / (32767 + 1.0);
			double fi2 = rand() / (32767 + 1.0);
			for (int i = 1; i <= numofparticle; ++i)
			{
				pa[1][i].vx = w*pa[1][i].vx + 1.49*fi1*(pa[1][i].pbestx - pa[1][i].px) + 1.49*fi2*(pa[1][i].gbestx - pa[1][i].px);
				pa[1][i].vy = w*pa[1][i].vy + 1.49*fi1*(pa[1][i].pbesty - pa[1][i].py) + 1.49*fi2*(pa[1][i].gbesty - pa[1][i].py);
				pa[1][i].vx = min((double)pa[1][i].vx, vxmax);
				pa[1][i].vx = max((double)pa[1][i].vx, vxmin);
				pa[1][i].vy = min((double)pa[1][i].vy, vymax);
				pa[1][i].vy = max((double)pa[1][i].vy, vymin);
				pa[1][i].px = pa[1][i].px + pa[1][i].vx;
				pa[1][i].py = pa[1][i].py + pa[1][i].vy;

				circle(img1, Point(pa[1][i].px, pa[1][i].py), 2, Scalar(0, 255, 0));
			}
			//if (pa[1][1].gbest > 0.999) break;

		}
		p1.x = pa[1][1].gbestx - width / 2;
		p1.y = pa[1][1].gbesty - height / 2;
		p2.x = pa[1][1].gbestx + width / 2 - (width & 1 ? 0 : 1);
		p2.y = pa[1][1].gbesty + height / 2 - (height & 1 ? 0 : 1);




		/*

		Point test(50, 50);
		Point test1, test2;

		test1.x = test.x - width / 2;
		test1.y = test.y - height / 2;
		test2.x = test.x + width / 2 - (width & 1 ? 0 : 1);
		test2.y = test.y + height / 2 - (height & 1 ? 0 : 1);

		//cout << "after" << p1.x << " " << p1.y << " " << p2.x << " " << p2.y<<endl;

		Rect rect1(test1, test2);
		Mat image_cut1 = img(rect1);
		Mat image_new1;
		image_cut1.copyTo(image_new1);
		Mat dst1 = image_new1.reshape(0, image_new1.rows*image_new1.cols);
		float res = 0;
		dst1.convertTo(dst, CV_32FC1);

		//cout << dst.rows << " " << dst.cols << endl;
		//cout << U.rows << " " << U.cols << endl;

		Mat ot = dst - UU*dst;

		for (int i = 0; i < ot.rows; i++)
		{
			const float* pData1 = ot.ptr<float>(i);
			for (int j = 0; j < ot.cols; j++)
				res += pData1[j] * pData1[j];
		}

		//printf("RE=%f\n", sqrt(res));


		double outa1 = pow(exp, -sqrt(res));

		cout << "outa=" << outa1 << endl;


		*/


		rectangle(img1, p1, p2, Scalar(0, 0, 255), 2, 8, 0);
		imshow(Window_Name, img1); waitKey(1);
		
		//imshow(Window_Name, img); 
		waitKey(1);
	}
	
}