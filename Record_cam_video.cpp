#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main( int argc, char* argv[] ) {
	namedWindow( "Webcam", WINDOW_AUTOSIZE );
	namedWindow( "Log_Polar", WINDOW_AUTOSIZE );
	VideoCapture capture;
	capture.open(0);
	double fps = capture.get( CAP_PROP_FPS );
	Size size(
		(int)capture.get( CAP_PROP_FRAME_WIDTH ),
		(int)capture.get( CAP_PROP_FRAME_HEIGHT )
	);
	VideoWriter writer;
	writer.open( argv[1], CV_FOURCC('M','J','P','G'), fps, size );
	Mat logpolar_frame(size, CV_8UC4), bgr_frame;
	for(;;) {
		capture >> bgr_frame;
		if( bgr_frame.empty() ) break; // end if done
		imshow( "Webcam", bgr_frame );
		logPolar(
			bgr_frame, // Input color frame
			logpolar_frame, // Output log-polar frame
			Point2f( // Centerpoint for log-polar transformation
				bgr_frame.cols/2, // x
				bgr_frame.rows/2 // y
			),
			40, // Magnitude (scale parameter)
			WARP_FILL_OUTLIERS // Fill outliers with ‘zero’
		);
		imshow( "Log_Polar", logpolar_frame );
		writer << logpolar_frame;
		char c = cv::waitKey(10);
		if( c == 27 ) break; // allow the user to break out
	}
	capture.release();
}
