#include "Header.h"


int main() {
    cout << endl << "Load Training Data" << endl;
    InputData("data (", ").png", nTrain, Train, 0);
    cout << "Load Testing Data" << endl;
    InputData("test (", ").png", nTest, Test, 0);
    cout << "Start Knn" << endl;
    float acc = 0;
    for (int k = 1; k < 10; k++)
    {
        float acc = 0;
        for (int i = 0; i < (nTest); i++) {
            int res = classifyAImage(Train, nTrain, k, Test[i]);
            //cout << "The value classified to Test " << i << " Image is " << res << endl;
            if (res == Test[i].GetLabel()) {
                acc++;
            }
            //cout << "       accuracy : " << acc * 100 / (nTest) << "%" << endl << endl;
        }
        cout << "k= " << k << "      accuracy : " << acc * 100 / (nTest) << " % " << endl << endl;
    }

    return 0;
}