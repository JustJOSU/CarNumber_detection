#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"
#include "opencv2/core/types.hpp"

using namespace std;
using namespace cv;

//Canny
int nLowThreshold = 220;
int nMaxThreshold = 420;
Mat kernel(3, 3, CV_8U, Scalar(1));

int main(int argc, char** argv) {
	Mat image, image2, image3, image4, drawing;
	Rect rect, temp_rect;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	double ratio, delta_x, delta_y, gradient;
	int select, plate_width, count, friend_count = 0, refinery_count = 0;

	image = imread("../2.jpg");
	imshow("Original Image", image);

	cvtColor(image, image2, COLOR_BGR2GRAY);
	//imshow("gray", image2);
	
	adaptiveThreshold(image2, image2, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);
	//imshow("Thresh", image2);

	Canny(image2, image2, nLowThreshold, nMaxThreshold);
	//imshow("canny", image2);

	findContours(image2, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Rect> boundRect2(contours.size());

	//찾은 윤곽의 꼭짓점 수를 줄이고 새로운 박스로 만든 뒤 박스의 좌상단 꼭짓점 좌표와 가로 세로 값을 boundRect에 저장
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	
	drawing = Mat::zeros(image2.size(), CV_8UC3);

	// 검출한 윤곽의 박스중에 가로세로비가 0.5~3.5, 박스의 크기가 100~700인 것만 보여줌
	for (int i = 0; i < contours.size(); i++) {
		ratio = (double) boundRect[i].height / boundRect[i].width;

		if ((ratio <= 3.5) && (ratio >= 0.5) && (boundRect[i].area() <= 700) && (boundRect[i].area() >= 100)) {
			drawContours(drawing, contours, i, Scalar(0, 255, 255), 1, 8, hierarchy, 0, Point());
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0));

			refinery_count += 1;
			boundRect2[refinery_count] = boundRect[i];
		}
	}
	//imshow("drawing", drawing);
	boundRect2.resize(refinery_count);
	
	// boundRect2를 좌상단 x의 값을 가지고 버블소팅함
	for (int i = 0; i < boundRect2.size(); i++) {
		for (int j = 0; j < boundRect2.size() - i; i++) {
			if (boundRect2[j].tl().x > boundRect2[j + 1].tl().x) {
				temp_rect = boundRect2[i];
				boundRect2[j] = boundRect2[j + 1];
				boundRect2[j + 1] = temp_rect;
			}
		}
	}
	drawing.copyTo(image3);

	// 정렬된 박스에서 인접한 박스끼리의 기울기 값이 0.5 이하일 경우 번호판으로 인식하고 count 1 증가
	for (int i = 0; i < boundRect2.size(); i++) {
		rectangle(image3, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 255, 0));
		count = 0;

		for (int j = i + 1; j < boundRect2.size(); j++) {
			delta_x = abs(boundRect2[j].tl().x - boundRect2[i].tl().x);

			if (delta_x > 150) break;

			delta_y = abs(boundRect2[j].tl().y - boundRect2[i].tl().y);

			if (delta_x == 0) {
				delta_x = 1;
			}

			if (delta_y == 0) {
				delta_y = 1;
			}

			gradient = delta_y / delta_x;
			cout << gradient << endl;

			if (gradient < 0.5) {
				count += 1;
			}
		}
		
		if (count > friend_count) {
			select = i;
			friend_count = count;
			rectangle(image3, boundRect2[select].tl(), boundRect2[select].br(), Scalar(255, 0, 0));
			plate_width = delta_x;
		}
	}
	rectangle(image3, boundRect2[select].tl(), boundRect2[select].br(), Scalar(0, 0, 255));
	line(image3, boundRect2[select].tl(), Point(boundRect2[select].tl().x + plate_width, boundRect2[select].tl().y), Scalar(0, 0, 255));

	imshow("Rectangle On CarPlate", image3);
	imshow("Region of interest", image(Rect(boundRect2[select].tl().x - 20, boundRect2[select].tl().y - 20, plate_width + 40, plate_width*0.3)));
	
	waitKey(0);
	return 0;
}