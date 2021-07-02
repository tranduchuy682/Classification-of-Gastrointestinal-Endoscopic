#include <opencv2/opencv.hpp>
using namespace cv;

#include <bits/stdc++.h>
using namespace std;

class Image {
private:
    int label;
    cv::Mat des;
    int rows;
public:
    Image() {
        des = Mat(100, 128, CV_32FC1);
        label = 0;
        rows = 0;
    };
    Image(int l, Mat d, int r) {
        label = l;
        des = d;
        rows = r;
    }
    //Mat GetDes() { return des; }
    int GetLabel() { return label; }
    float GetDistance(Image x) {
        float dist = 0;
        float x1, x2;
        for (int a = 0; a < 200; a++) {
            for (int b = 0; b < 128; b++) {
                if (a < rows) {
                    x1 = des.at<float>(a, b);
                }
                else x1 = 0;
                if (a < x.rows) {
                    x2 = x.des.at<float>(a, b);
                }
                else x2 = 0;
                dist += (x1 - x2) * (x1 - x2);
            }

        }
        return sqrt(dist);
    }
};

const int n0 = 45;         //Normal
const int n1 = 46;         //96 Abnormal

const int nTrain = 91;
const int nTest = 35;      //= 11(n0) +24(n1)

Image Train[nTrain], Test[nTest];

int LabelData(int type, int i) {
    if (type == nTrain) {
        if (i < (n0))  return 0;
        else if ((i > (n0 - 1)) && (i < (n0 + n1))) return 1;
    }
    else if (type == nTest) {
        if (i < (11))                       return 0;
        else if ((i > 10) && (i < 35))      return 1;
    }
}

void InputData(string str1, string str2, int n, Image arr[], int count)
{
    for (int i = 1; i <= n; i++) {
        string path = str1 + to_string(i) + str2;
        cv::Mat img = imread(path, 0);
        //cv::resize(img, img, Size(320, 320), 0, 0);
        auto detector = SiftFeatureDetector::create();
        vector<KeyPoint>keypoints;
        detector->detect(img, keypoints);
        auto extractor = SiftDescriptorExtractor::create();
        Mat descriptors;
        extractor->compute(img, keypoints, descriptors);
        cout << count << "  " << img.rows << "  " << img.cols << "  " << descriptors.rows << "  " << descriptors.cols << endl;
        Image A(LabelData(n, count), descriptors, descriptors.rows);
        arr[count] = A;
        count++;
    }
}

int classifyAImage(Image arr[], const int n, int k, Image p)
{
    float* distance = new float[n];
    for (int i = 0; i < n; i++)
    {
        distance[i] = p.GetDistance(arr[i]);

    }
    int* num = new int[n];
    for (int i = 0; i < n; i++) {
        num[i] = i;
    }
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (distance[num[i]] < distance[num[j]]) {
                swap(num[i], num[j]);
            }
        }
    }
    int freq0 = 0;
    int freq1 = 0;
    for (int i = 0; i < k; i++)
    {
        if ((arr[num[i]].GetLabel()) == 0) freq0++;
        else if (arr[num[i]].GetLabel() == 1) freq1++;
    }
    int Max = std::max(freq0, freq1);
    if (Max == freq1) return 1;
    else if (Max == freq0) return 0;
}
