#include <iostream>
#include <cstdint>
#include <opencv2/opencv.hpp>
#include <time.h>

int32_t main() {
	
	cv::VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) {
		return 0;
	}

	clock_t start;

	while (1) {

		
		
		cv::Mat frame, processedFrame, copy, res;
		bool status = cap.read(frame);
		if (!status) {
			break;
		}

		start = clock();

		cv::rotate(frame, processedFrame, cv::ROTATE_180);
		cv::flip(processedFrame, processedFrame, 1);

		cv::cvtColor(processedFrame, copy, cv::COLOR_BGR2RGB);

		cv::inRange(copy, cv::Scalar(0, 0, 0), cv::Scalar(100, 255, 255), copy);

		cv::bitwise_and(processedFrame, processedFrame, res, copy);

		std::cout << (((double)(clock() - start)) / CLOCKS_PER_SEC) << std::endl;

		cv::imshow("original", frame);
		cv::imshow("processed", res);

		

		char c = cvWaitKey(33);
		
		if (c == 27) {
			break;
		}
	}

	return 0;
}

