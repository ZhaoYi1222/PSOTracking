#include "PSO.h"
#include<cmath>
//using namespace std;
#define exp  2.71828182845904523536028747135266249775724709369995
#define numofparticle 40
#define numofarea 2
#define numofallpar numofparticle*numofarea
void tracker()
{
	srand(time(NULL));

	for (int zz = 1; zz <= numofarea; ++zz)
	{
		area[zz].width = area[zz].pr.x - area[zz].pl.x + 1;
		area[zz].height = area[zz].pr.y - area[zz].pl.y + 1;
		area[zz].vxmin = -(area[zz].pr.x - area[zz].pl.x);
		area[zz].vymin = -(area[zz].pr.y - area[zz].pl.y);
		area[zz].vxmax = (area[zz].pr.x - area[zz].pl.x);
		area[zz].vymax = (area[zz].pr.y - area[zz].pl.y);
	}

	Mat img;
	char targetImg[22], Window_Name[20];



	for (int frame = 2; frame <= 288; frame++) {
		for (int zz = 1; zz <= numofarea; ++zz)
		{
			for (int i = 1; i <= numofparticle; ++i)
			{
				pa[zz][i].px = pa[zz][i].pbestx = pa[zz][i].gbestx = rand() % (area[zz].pr.x - area[zz].pl.x) + area[zz].pl.x;
				pa[zz][i].py = pa[zz][i].pbesty = pa[zz][i].gbesty = rand() % (area[zz].pr.y - area[zz].pl.y) + area[zz].pl.y;
				pa[zz][i].vx = rand() % ((area[zz].pr.x - area[zz].pl.x)) - (area[zz].pr.x - area[zz].pl.x) / 2;
				pa[zz][i].vy = rand() % ((area[zz].pr.y - area[zz].pl.y)) - (area[zz].pr.y - area[zz].pl.y) / 2;
				pa[zz][i].fitness = pa[zz][i].pbest = pa[zz][i].gbest = -1;
			}
		}

		sprintf_s(targetImg, "pic5/%04d.jpg", frame);
		int index = 259 - 2 + 1;
		sprintf_s(Window_Name, "Frame%d", index);
		Mat image = imread(targetImg);
		if (image.channels()>2)
			cvtColor(image, img, CV_BGR2GRAY);
		else
			img = image;
		img.copyTo(image);
		Mat img1;
		cvtColor(img, img1, CV_GRAY2BGR);
		for (int zz = 1; zz <= numofarea; ++zz)
		{

			Rect rect(area[zz].pl, area[zz].pr);
			Mat image_cut = img(rect);
			Mat image_new;
			Mat dst = image_new.reshape(0, image_new.rows*image_new.cols);
			Mat tmp = ob[zz].targetobserve.t();
			Mat tmp1;
			tmp1 = tmp;
			Mat ttmp = dst.t();
			tmp1.convertTo(tmp1, CV_8UC1);
			tmp1.push_back(ttmp);
			ob[zz].targetobserve = tmp1.t();
			ob[zz].targetobserve.convertTo(ob[zz].targetobserve, CV_32FC1);
			Mat U, W, V;
			SVD::compute(ob[zz].targetobserve, W, U, V);
			int idofgbest;
			Mat UU = U*U.t();
			


			for (int z = 1; z <= 20; ++z)
			{
				for (int i = 1; i <= numofparticle; ++i)
				{
					area[zz].pl.x = pa[zz][i].px - area[zz].width / 2;
					area[zz].pl.y = pa[zz][i].py - area[zz].height / 2;
					area[zz].pr.x = pa[zz][i].px + area[zz].width / 2 - (area[zz].width & 1 ? 0 : 1);
					area[zz].pr.y = pa[zz][i].py + area[zz].height / 2 - (area[zz].height & 1 ? 0 : 1);

					Rect rect(area[zz].pl, area[zz].pr);
					Mat image_cut = img(rect);
					Mat image_new;
					image_cut.copyTo(image_new);
					Mat dst = image_new.reshape(0, image_new.rows*image_new.cols);
					float res = 0;
					dst.convertTo(dst, CV_32FC1);
					Mat ot = dst - UU*dst;
					for (int i = 0; i < ot.rows; i++)
					{
						const float* pData = ot.ptr<float>(i);
						for (int j = 0; j < ot.cols; j++)
							res += pData[j] * pData[j];
					}
					pa[zz][i].fitness = pow(exp, -sqrt(res));
					if (pa[zz][i].fitness > pa[zz][i].pbest)
					{
						pa[zz][i].pbest = pa[zz][i].fitness;
						pa[zz][i].pbestx = pa[zz][i].px;
						pa[zz][i].pbesty = pa[zz][i].py;
					}
				}
				double maxa = -1;
				int gx, gy;
				for (int i = 1; i <= numofparticle; ++i)
				{
					if (pa[zz][i].pbest > maxa)
					{
						maxa = pa[zz][i].pbest;
						gx = pa[zz][i].pbestx;
						gy = pa[zz][i].pbesty;
						idofgbest = i;
					}
				}
				for (int i = 1; i <= numofparticle; ++i)
				{
					pa[zz][i].gbest = maxa;
					pa[zz][i].gbestx = gx;
					pa[zz][i].gbesty = gy;
				}

				double w = 0.729844;
				double fi1 = rand() / (32767 + 1.0);
				double fi2 = rand() / (32767 + 1.0);
				for (int i = 1; i <= numofparticle; ++i)
				{
					pa[zz][i].vx = w*pa[zz][i].vx + 1.49*fi1*(pa[zz][i].pbestx - pa[zz][i].px) + 1.49*fi2*(pa[zz][i].gbestx - pa[zz][i].px);
					pa[zz][i].vy = w*pa[zz][i].vy + 1.49*fi1*(pa[zz][i].pbesty - pa[zz][i].py) + 1.49*fi2*(pa[zz][i].gbesty - pa[zz][i].py);
					pa[zz][i].vx = min((double)pa[zz][i].vx, area[zz].vxmax);
					pa[zz][i].vx = max((double)pa[zz][i].vx, area[zz].vxmin);
					pa[zz][i].vy = min((double)pa[zz][i].vy, area[zz].vymax);
					pa[zz][i].vy = max((double)pa[zz][i].vy, area[zz].vymin);
					pa[zz][i].px = pa[zz][i].px + pa[zz][i].vx;
					pa[zz][i].py = pa[zz][i].py + pa[zz][i].vy;
					
					//circle(img1, Point(pa[zz][i].px, pa[zz][i].py), 2, Scalar(0, 255, 0));
				}
			}


			area[zz].pl.x = pa[zz][1].gbestx - area[zz].width / 2;
			area[zz].pl.y = pa[zz][1].gbesty - area[zz].height / 2;
			area[zz].pr.x = pa[zz][1].gbestx + area[zz].width / 2 - (area[zz].width & 1 ? 0 : 1);
			area[zz].pr.y = pa[zz][1].gbesty + area[zz].height / 2 - (area[zz].height & 1 ? 0 : 1);

			//rectangle(img1, area[zz].pl, area[zz].pr, Scalar(0, 0, 255), 2, 8, 0);
			/*
			cout << zz << endl;
			cout << area[zz].pl.x << " " << area[zz].pl.y << endl;
			cout << area[zz].pr.x << " " << area[zz].pr.y << endl;
			cout << endl;
			*/

		}
		rectangle(img1, area[1].pl, area[1].pr, Scalar(0, 0, 255), 2, 8, 0);
		rectangle(img1, area[2].pl, area[2].pr, Scalar(0, 0, 255), 2, 8, 0);
		imshow(Window_Name, img1); waitKey(1);
		

		waitKey(1);
	}
	
}