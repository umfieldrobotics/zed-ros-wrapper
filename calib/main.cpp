///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2022, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


#include <sl/Camera.hpp>


// Standard includes
#include <string.h>

// OpenCV include (for display)
#include <opencv2/opencv.hpp>

using namespace sl;


int main(int argc, char **argv) {   
    cv::VideoCapture zed;
    // NOTICE:  /////////// IMPORTANT ///////////////
    // Assuming here that ZED is connected under device 0 (/dev/video0 for Linux or first camera listed on Windows)
    // On laptops, you may have to change to 1 if the first listed camera is the integrated webcam.
    if (argc == 2)
        zed.open(atoi(argv[1]));
    else
        zed.open(0);

    //define the camera resolution
    cv::Size size_sbs(1920 * 2, 1080);

    // change camera param to fit requested resolution (ZED camera gives side by side images)
    zed.set(cv::CAP_PROP_FRAME_WIDTH, size_sbs.width);
    zed.set(cv::CAP_PROP_FRAME_HEIGHT, size_sbs.height);

    // create a file to save images names
    std::vector<std::string> v_names;

    // alloc a mat to store acquired images
    cv::Mat imag_sbs(size_sbs, CV_8UC3);
    int w_ = size_sbs.width * .5;

    // define Left and Right reference Mat
    cv::Mat imL = imag_sbs(cv::Rect(0, 0, w_, size_sbs.height));
    cv::Mat imR = imag_sbs(cv::Rect(w_, 0, w_, size_sbs.height));

    int nb_save = 0;
    const int NB_REQUIRED = 20;
    while (nb_save < NB_REQUIRED) {

        // grab and retrieve the current image
        zed >> imag_sbs;

        // Left and Right mat are directly updated because they are ref.

        cv::imshow("Left", imL); // display left image
        auto k = cv::waitKey(30);

        // if Space-bar is pressed, save the image
        if (k == 32) {
            std::string im_name("zed_image_L" + std::to_string(nb_save) + ".png");
            cv::imwrite(im_name, imL);
            v_names.push_back(im_name);

            im_name = "zed_image_R" + std::to_string(nb_save) + ".png";
            cv::imwrite(im_name, imR);
            v_names.push_back(im_name);

            nb_save++;
            std::cout << "Save im " << nb_save << "/" << NB_REQUIRED << std::endl;
        }
    }
    // close file and camera
    cv::FileStorage fs("zed_image_list.xml", cv::FileStorage::WRITE);
    fs.write("images", v_names);
    fs.release();
    zed.release();
    return EXIT_SUCCESS;
}

// int main(int argc, char **argv) {

//     // Create a ZED camera object
//     Camera zed;

//     // Open the camera
//     ERROR_CODE returned_state = zed.open();
//     if (returned_state != ERROR_CODE::SUCCESS) {
//         std::cout << "Error " << returned_state << ", exit program.\n";
//         return EXIT_FAILURE;
//     }

//     // Get camera information (ZED serial number)
//     auto camera_infos = zed.getCameraInformation();
//     printf("Hello! This is my serial number: %d\n", camera_infos.serial_number);

//     // Close the camera
//     zed.close();
//     return EXIT_SUCCESS;
// }

