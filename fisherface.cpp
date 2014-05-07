#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

String face_cascade_name = "haarcascade_frontalface_alt.xml";


/** @function detectAndDisplay */
Mat detectAndResize(Mat frame)
{
  std::vector<Rect> faces;
  Mat frame_gray;
  CascadeClassifier face_cascade;
  face_cascade.load( face_cascade_name );

  cvtColor( frame, frame_gray, CV_RGB2GRAY );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  if (faces.size() <= 0) {
      Mat blank(100, 100, CV_8UC1);
      return blank;
  }
  Range a(faces[0].x, faces[0].x + faces[0].width);
  Range b(faces[0].y, faces[0].y + faces[0].height);
  Mat faceROI = Mat(frame_gray, b, a);
  Mat output;
  resize(faceROI, faceROI, Size(100, 100), 0, 0, INTER_NEAREST);

  return faceROI;
}

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    int i = 0;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            Mat face;
            face = detectAndResize(imread(path, 1));
            cout << "dim" << face.dims << ", " << face.rows << ", " << face.cols << "\n";
            images.push_back(face);
            labels.push_back(atoi(classlabel.c_str()));
            cout << "face " << path << ", label  " << atoi(classlabel.c_str()) << "\n";
            i++;
            cout << "training: " << i << "\n";
        }
    }
}
void trainAndSave(String csv, String resultname)
{
    vector<Mat> images;
    vector<int> labels;
    try {
        read_csv(csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << csv << "\". Reason: " << e.msg << endl;
        exit(1);
    }
    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    model->train(images, labels);
    model->save(resultname);
}

String getEmotion(int e)
{
    switch (e) {
        case 1:
            return "Angry";
        case 2:
            return "Disgust";
        case 3:
            return "Fear";
        case 4:
            return "Happy";
        case 5:
            return "Sadness";
        case 6:
            return "Surprise";
        case 7:
            return "Contempt";
    }
}

int main(int argc, const char *argv[]) {
    if (argc == 2 && argv[1][0] == 't') {
        trainAndSave("label.csv", "result.xml");
        return 0;
    }

    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    model->load("result.xml");
    CvCapture* capture;
    Mat frame;

   //-- 2. Read the video stream
   capture = cvCaptureFromCAM( 0 );
   if( capture )
   {
     while( true )
     {
       frame = cvQueryFrame( capture );

   //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       {
            Mat img = detectAndResize(frame);
            cout << "result:" <<getEmotion(model->predict(img))<<"\n";
            imshow("face", img);
       }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
   return 0; 
    return 0;
}

